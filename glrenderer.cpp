#include "glrenderer.h"

HGLRC ourOpenGLRenderingContext;
HDC ourWindowHandleToDeviceContext;

unsigned char *video_Buffer;
unsigned int textureBytesPerPixel;

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

void AllocatePixelBuffer(unsigned int width, unsigned int height, int bytesPerPixel) {	
	textureBytesPerPixel = 1;
	video_Buffer = (unsigned char *)malloc(width * height * bytesPerPixel);

	glOrtho(0, 1, 1, 0, -1, 1);
}

void ResetVideo() {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &video_ScreenTexture);
	glBindTexture(GL_TEXTURE_2D, video_ScreenTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, currentDisplayMode.width, currentDisplayMode.height, 0, GL_COLOR_INDEX, GL_UNSIGNED_SHORT, NULL);
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

bool do3d = false;
void DrawPixelBuffer() {

	unsigned int glDataType = GL_UNSIGNED_BYTE;
	/*if (textureBytesPerPixel == 2)
		glDataType = GL_UNSIGNED_SHORT;*/

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, currentDisplayMode.width, currentDisplayMode.height, GL_COLOR_INDEX, glDataType, video_Buffer);
	
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0);
		glVertex2i(0, 0);
		glTexCoord2i(1, 0);
		glVertex2i(640, 0);
		glTexCoord2i(1, 1);
		glVertex2i(640, 480);
		glTexCoord2i(0, 1);
		glVertex2i(0, 480);
		glEnd();
}

void VideoSwapBuffers() {
	SwapBuffers(ourWindowHandleToDeviceContext);
}

unsigned char blahBuffer[256 * 256 * 2];

unsigned char *textureBufferList[256];
//int tag = 1;

int AllocateTexture(t_AllocateTextureQuery *t) {	
	//if (!textureBufferList[t->tag]) {
	//	textureBufferList[t->tag] = (unsigned char*)malloc(t->alloc->width * t->alloc->height);		
	//}
	////}
	t->alloc->bytesPerPixel = 70000;
	t->alloc->buffer = (unsigned int*)blahBuffer; //(unsigned int*)textureBufferList[t->type];	
	
	return 0;
}

int AddTexture(int t) {
	return 0;
}
