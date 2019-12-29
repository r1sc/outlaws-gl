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

	//glOrtho(0, 1, 1, 0, -1, 1);

	glGenTextures(1, &video_ScreenTexture);
	glGenTextures(256, textures);
}

void ResetVideo() {
	glEnable(GL_TEXTURE_2D);

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
	glColor3f(1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);

	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentDisplayMode.width, currentDisplayMode.height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, video_Buffer);

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

GLuint lastTexture;
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

			lastTexture = index;
			usedTextures[index] = true;
			nextFree = (index + 1) % 256;
			return index;
		}
	}

	return 0;
}

void drawVertex(float* v) {
	float oow = v[3];
	float w =  oow;
	float sow = v[6];
	float tow = v[7];
	float s = w * sow;
	float t = w * tow;

	glTexCoord2f(s, t);
	glVertex3f(v[0], v[1], -v[2]);
}

void Render3d(t_Render3dInput input) {
	/*glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textures[120]);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(1, 0);
	glVertex2i(currentDisplayMode.width, 0);
	glTexCoord2i(1, 1);
	glVertex2i(currentDisplayMode.width, currentDisplayMode.height);
	glTexCoord2i(0, 1);
	glVertex2i(0, currentDisplayMode.height);
	glEnd();*/

	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textures[lastTexture]);

	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < input.numTriangles; i++)
	{
		unsigned int i1 = input.triangles[i * 5];
		unsigned int i2 = input.triangles[i * 5 + 1];
		unsigned int i3 = input.triangles[i * 5 + 2];
		unsigned int flags = input.triangles[i * 5 + 4];

		float* v1 = &input.vertices[i1 * 8];
		float* v2 = &input.vertices[i2 * 8];
		float* v3 = &input.vertices[i3 * 8];

		if (flags & 0x1000)
			glDepthFunc(GL_LESS);
		else
			glDepthFunc(GL_ALWAYS);

		if (flags & 0x2000)
			glDepthMask(GL_TRUE);
		else
			glDepthMask(GL_FALSE);

		drawVertex(v3);
		drawVertex(v2);
		drawVertex(v1);
	}
	glEnd();
}