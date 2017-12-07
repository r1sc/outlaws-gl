#include "glrenderer.h"

HGLRC ourOpenGLRenderingContext;
HDC ourWindowHandleToDeviceContext;

unsigned short *video_Buffer;
GLuint video_ScreenTexture;

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

void AllocatePixelBuffer(unsigned int width, unsigned int height) {	
	video_Buffer = (unsigned short *)malloc(width * height * 4);

	glOrtho(0, 1, 1, 0, -1, 1);
}

void ResetVideo() {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &video_ScreenTexture);
	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, currentDisplayMode.width, currentDisplayMode.height, 0, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}

void DrawPixelBuffer() {
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentDisplayMode.width, currentDisplayMode.height, GL_COLOR_INDEX, GL_UNSIGNED_BYTE, video_Buffer);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0);
	glVertex2i(0, 0);
	glTexCoord2i(1, 0);
	glVertex2i(1, 0);
	glTexCoord2i(1, 1);
	glVertex2i(1, 1);
	glTexCoord2i(0, 1);
	glVertex2i(0, 1);
	glEnd();
}

void VideoSwapBuffers() {
	SwapBuffers(ourWindowHandleToDeviceContext);
}