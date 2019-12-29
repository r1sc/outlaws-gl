#include "glrenderer.h"

HGLRC ourOpenGLRenderingContext;
HDC ourWindowHandleToDeviceContext;

unsigned char* video_Buffer;
unsigned int textureBytesPerPixel;

GLuint video_ScreenTexture;
GLuint textures[256];

bool do3d = false;

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

	ourWindowHandleToDeviceContext = GetDC(hWnd);

	int letWindowsChooseThisPixelFormat;
	letWindowsChooseThisPixelFormat = ChoosePixelFormat(ourWindowHandleToDeviceContext, &pfd);
	SetPixelFormat(ourWindowHandleToDeviceContext, letWindowsChooseThisPixelFormat, &pfd);

	ourOpenGLRenderingContext = wglCreateContext(ourWindowHandleToDeviceContext);
	wglMakeCurrent(ourWindowHandleToDeviceContext, ourOpenGLRenderingContext);
}

void AllocatePixelBuffer(unsigned int width, unsigned int height, int bytesPerPixel) {
	textureBytesPerPixel = 1;
	video_Buffer = (unsigned char*)malloc(width * height * bytesPerPixel);

	glGenTextures(1, &video_ScreenTexture);
	glGenTextures(256, textures);
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
	glOrtho(0, currentDisplayMode.width, currentDisplayMode.height, 0, 0, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void DrawPixelBuffer() {
	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentDisplayMode.width, currentDisplayMode.height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, video_Buffer);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(1, 0);
	glVertex2i(currentDisplayMode.width, 0);
	glTexCoord2i(1, 1);
	glVertex2i(currentDisplayMode.width, currentDisplayMode.height);
	glTexCoord2i(0, 1);
	glVertex2i(0, currentDisplayMode.height);
	glEnd();
}

void VideoSwapBuffers() {
	SwapBuffers(ourWindowHandleToDeviceContext);
}

t_AllocateTexture allocTexture;
GLuint nextFree = 0;
unsigned char scratchBuffer[256 * 256 * 4];
bool usedTextures[256];

int AllocateTexture(t_AllocateTextureQuery input) {
	input.alloc->bytesPerPixel = 1;
	input.alloc->buffer = scratchBuffer;

	allocTexture.width = input.alloc->width;
	allocTexture.height = input.alloc->height;
	allocTexture.bytesPerPixel = input.alloc->bytesPerPixel;
	allocTexture.buffer = scratchBuffer;
	   	
	input.result = &allocTexture;
	return 0;
}

int AddTexture(t_AddTexture input) {
	for (size_t i = 0; i < 256; i++)
	{
		GLuint index = (nextFree + i) % 256;
		if (!usedTextures[index]) {
			glBindTexture(GL_TEXTURE_2D, textures[index]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, allocTexture.width, allocTexture.height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, allocTexture.buffer);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			*input.textureTag = index;
			usedTextures[index] = true;
			nextFree = (index + 1) % 256;
			return 0;
		}
	}

	return 0;
}

int RemoveTexture(unsigned int* textureTag) {
	usedTextures[*textureTag] = false;
	return 0;
}

void drawVertex(t_Vertex* v) {
	float s = v->s * v->w; //sow / w;
	float t = v->t * v->w;
			
	glTexCoord4f(s, t, 0, v->w);
	glVertex3f(v->x, v->y, -v->z);
}

void Render3d(t_Render3dInput input) {
	glEnable(GL_DEPTH_TEST);

	for (size_t i = 0; i < input.numTriangles; i++)
	{
		t_Triangle triangle = input.triangles[i];
		t_Vertex v1 = input.vertices[triangle.i1];
		t_Vertex v2 = input.vertices[triangle.i2];
		t_Vertex v3 = input.vertices[triangle.i3];

		unsigned int f = ((unsigned int*)input.vertices)[triangle.i1 * 8 + 4];
		unsigned int hej = (f & 0xFF3FFFFF) >> 22;
		
		if (triangle.flags & 0x1000)
			glDepthFunc(GL_LESS);
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
}