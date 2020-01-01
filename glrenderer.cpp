#include "glrenderer.h"
#include "settings.h"

HGLRC glContext;
HDC hDC;

bool rendering3d = false;

unsigned char* videoBuffer;
GLuint video_ScreenTexture;

#define NUM_TEXTURES 3000
GLuint textures[NUM_TEXTURES];
bool usedTextures[NUM_TEXTURES];
GLuint nextFreeTextureIndex = 0;

t_AllocateTexture allocTexture;
unsigned char scratchBuffer[256 * 256 * 4];

void OpenRenderer() {
	rendering3d = true;
}

void CloseRenderer() {
	rendering3d = false;
	memset(usedTextures, 0, NUM_TEXTURES);
	nextFreeTextureIndex = -1;
}

void SetPixelFormatAndCreateContext(HWND hWnd) {
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
		PFD_TYPE_RGBA,            //The kind of framebuffer. RGBA or palette.
		16,                        //Colordepth of the framebuffer.
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,                        //Number of bits for the depthbuffer
		0,                        //Number of bits for the stencilbuffer
		0,                        //Number of Aux buffers in the framebuffer.
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	hDC = GetDC(hWnd);
		
	int letWindowsChooseThisPixelFormat = ChoosePixelFormat(hDC, &pfd);
	SetPixelFormat(hDC, letWindowsChooseThisPixelFormat, &pfd);

	glContext = wglCreateContext(hDC);
	wglMakeCurrent(hDC, glContext);
}

void AllocatePixelBuffer(unsigned int width, unsigned int height, int bytesPerPixel) {
	videoBuffer = (unsigned char*)malloc(width * height * bytesPerPixel);

	glGenTextures(1, &video_ScreenTexture);
	glGenTextures(NUM_TEXTURES, textures);
}

void ResetVideo() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, currentDisplayMode.width, currentDisplayMode.height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, windowResolutionWidth, windowResolutionHeight, 0, 0, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glViewport(0, 0, windowResolutionWidth, windowResolutionHeight);
}

void DrawPixelBuffer() {
	glLoadIdentity();

	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentDisplayMode.width, currentDisplayMode.height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, videoBuffer);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(1, 0);
	glVertex2i(windowResolutionWidth, 0);
	glTexCoord2i(1, 1);
	glVertex2i(windowResolutionWidth, windowResolutionHeight);
	glTexCoord2i(0, 1);
	glVertex2i(0, windowResolutionHeight);
	glEnd();
}

void VideoSwapBuffers() {
	SwapBuffers(hDC);
}

int AllocateTexture(t_AllocateTextureInOut input) {
	input.alloc->bytesPerPixel = 1;
	input.alloc->buffer = scratchBuffer;

	allocTexture.width = input.alloc->width;
	allocTexture.height = input.alloc->height;
	allocTexture.bytesPerPixel = input.alloc->bytesPerPixel;
	allocTexture.buffer = scratchBuffer;
	   	
	input.result = &allocTexture;
	return 0;
}

int AddTexture(t_AddTextureInput input) {
	GLuint index = nextFreeTextureIndex + 1;
	bool foundFree = false;
	while(1)
	{
		if (index >= NUM_TEXTURES)
			index = 0;

		if (!usedTextures[index]) {
			foundFree = true;
			break;
		}
		index++;
		if (index == nextFreeTextureIndex)
			break;
	}	

	if (foundFree) {
		glBindTexture(GL_TEXTURE_2D, textures[index]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, allocTexture.width, allocTexture.height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, allocTexture.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		*input.textureTag = index;
		nextFreeTextureIndex = index;
		usedTextures[index] = true;
		return 0;
	}

	return 1;
}

int RemoveTexture(unsigned int* textureTag) {
	usedTextures[*textureTag] = false;
	nextFreeTextureIndex = NUM_TEXTURES;
	return 0;
}

void drawVertex(t_Vertex* v) {
	float s = v->s * v->w;
	float t = v->t * v->w;
			
	float r = ((v->color >> 16) & 0xFF) / 255.0f;
	float g = ((v->color >> 8) & 0xFF) / 255.0f;
	float b = ((v->color) & 0xFF) / 255.0f;
	glColor3f(r, g, b);

	glTexCoord4f(s, t, 0, v->w);
	glVertex3f(v->x, v->y, -v->z);
}

void Render3d(t_Render3dInput input) {
	glEnable(GL_DEPTH_TEST);

	glLoadIdentity();
	double sx = windowResolutionWidth / (double)currentDisplayMode.width;
	double sy = windowResolutionHeight / (double)currentDisplayMode.height;
	glScaled(sx, sy, 1);

	for (size_t i = 0; i < input.numTriangles; i++)
	{
		t_Triangle triangle = input.triangles[i];
		t_Vertex v1 = input.vertices[triangle.vertexIndex1];
		t_Vertex v2 = input.vertices[triangle.vertexIndex2];
		t_Vertex v3 = input.vertices[triangle.vertexIndex3];
		
		if (triangle.flags & 0x1000)
			glDepthFunc(GL_LEQUAL);
		else
			glDepthFunc(GL_ALWAYS);

		if (triangle.flags & 0x2000)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

		glBindTexture(GL_TEXTURE_2D, textures[triangle.textureTag]);
		glBegin(GL_TRIANGLES);
		drawVertex(&v3);
		drawVertex(&v2);
		drawVertex(&v1);
		glEnd();
	}

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glColor3f(1, 1, 1);
}