#include "RendererStructs.h"
#include "glrenderer.h"
#include <stdio.h>
#include <math.h>

extern HINSTANCE hInstance;
HWND hWnd;
t_DisplayMode currentDisplayMode;

int __cdecl Startup() {
	
	return 0;
}

int __cdecl Initialize(t_InitializeArgs *a1) {
	if (a1->hWnd) {
		a1->isFullscreen = 0;
		hWnd = a1->hWnd;
		SetPixelFormatAndCreateContext(hWnd);
	}
	return 0;
}

int __cdecl Pause() {
	return 0;
}

int __cdecl Resume() {
	return 0;
}

int __cdecl GDIPause() {
	return 0;
}

int __cdecl GDIResume() {
	
	//SetWindowPos(hWnd, HWND_TOP, 0, 0, 640, 480, SWP_NOOWNERZORDER);
	return 0;
}

int __cdecl DirtyRect() {
	return 0;
}

int __cdecl Save() {
	return 0;
}

int __cdecl Restore() {
	return 0;
}

HWND __cdecl GetMainWindow() {
	return IsWindow(hWnd) != 0 ? hWnd : NULL;
}

int __cdecl Allocate(t_AllocateArgs *a1) {
	if (IsWindow(hWnd)) {
		int width = GetSystemMetrics(0);
		int height = GetSystemMetrics(1);

		currentDisplayMode.width = a1->width;
		currentDisplayMode.height = a1->height;
		currentDisplayMode.bpp = a1->bpp;

		SetWindowPos(hWnd, HWND_TOP, 0, 0, a1->width, a1->height, 0);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);

		AllocatePixelBuffer(a1->width, a1->height, a1->bpp / 8);
		return 0;
	}
	return 1;
}

int __cdecl Free() {
	free(video_Buffer);
	return 0;
}

t_DisplayMode displayModes[2] = {
	{1024,768,16,0,{0}},
	{800,600,16,0,{0}}
};
signed int __cdecl GetDisplayModeList(int *numModes, t_DisplayMode **modes) {
	*numModes = 2;
	*modes = displayModes;
	return 0;
}

int __cdecl SetDisplayMode(t_DisplayMode *displayMode) {
	currentDisplayMode.width = displayMode->width;
	currentDisplayMode.height = displayMode->height;
	currentDisplayMode.bpp = displayMode->bpp;

	ResetVideo();
	return 0;
}

int __cdecl GetDisplayMode(t_DisplayMode **a1) {
	*a1 = &currentDisplayMode;
	return 0;
}

int __cdecl LockBuffer(int a1, t_LockBufferArgs *a2) {
	a2->pixelData = video_Buffer;
	a2->stride = currentDisplayMode.width;
	a2->bytesPerPixel = 1;
	a2->dword14 = 0;
	a2->dword10 = 0;
	a2->bufferWidth = currentDisplayMode.width;
	a2->height = a2->bufferHeight = currentDisplayMode.height;
	return 0;
}

int __cdecl UnlockBuffer(int a1, int a2) {
	return 0;
}

int __cdecl PageFlip() {
	DrawPixelBuffer();
	VideoSwapBuffers();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Sleep(1);
	return 0;
}

void glhFrustumf2(float *matrix, float left, float right, float bottom, float top,
	float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0 * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0;
	matrix[2] = 0.0;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0;
	matrix[7] = 0.0;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0;
	matrix[12] = 0.0;
	matrix[13] = 0.0;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0;
}

#define M_PI 3.14159265358979323846264338327950288
void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio,
	float znear, float zfar)
{
	float ymax, xmax;
	float temp, temp2, temp3, temp4;
	ymax = znear * tanf(fovyInDegrees * M_PI / 360.0);
	// ymin = -ymax;
	// xmin = -ymax * aspectRatio;
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

float matrix[4 * 4];

int __cdecl RasterizerHook(t_RasterizeHook* data) {
	switch (data->action)
	{
	case 0:	// Startup
		OutputDebugString("Rasterizer: Startup\n");
		return 0;
	case 1: // Shutdown
		OutputDebugString("Rasterizer: Shutdown\n");
		return 0;
	case 2: // Open
		OutputDebugString("Rasterizer: Open\n");

		return 0;
	case 3: // Close
		OutputDebugString("Rasterizer: Close\n");
		return 0;
	case 4: // Clear z-buffer
		glClear(GL_DEPTH_BUFFER_BIT);
		return 0;
	case 5: // Clear viewport
		glClear(GL_COLOR_BUFFER_BIT);
		return 0;
	case 6: // Allocate texture	{
		AllocateTexture(&data->allocateTextureQuery);
		//data->allocateTextureQuery.buffer = (unsigned int*)10;
		return 0;
		//return AllocateTexture(&data->allocateTextureQuery);
	case 7: // Add texture
		return 0;
	case 8: // Render vertices and triangles
	{	
		do3d = true;

		unsigned int numVertices = data->ary[0];
		float *vertices = (float*)data->ary[1];
		unsigned int numTriangles = data->ary[2];
		unsigned int *triangles = (unsigned int*)data->ary[3];
				

		glDisable(GL_TEXTURE_2D);
		glColor3f(1, 1, 1);
		glCullFace(GL_FRONT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		
		glBegin(GL_TRIANGLES);
		for (size_t i = 0; i < numTriangles; i++)
		{
			unsigned int i1 = triangles[i * 5 ];
			unsigned int i2 = triangles[i * 5 + 1];
			unsigned int i3 = triangles[i * 5 + 2];
			unsigned int ptr = triangles[i * 5 + 4];

			int r = (ptr & 0xFF000000) >> 24;
			int g = (ptr & 0xFF0000) >> 16;
			int b = (ptr & 0xFF00) >> 8;
			
			float *v1 = &vertices[i1 * 8];
			float *v2 = &vertices[i2 * 8];
			float *v3 = &vertices[i3 * 8];

			glColor3f(r / 255.0f, g / 255.0f, b / 255.0f);

			glVertex3f(v1[0], v1[1], -v1[2]);
			glVertex3f(v2[0], v2[1], -v2[2]);
 			glVertex3f(v3[0], v3[1], -v3[2]);
		}
		glEnd();
		return 0;
	}
	case 9: // Remove texture
		return 0;
	case 10: // Unk 1
		return 0;
	case 11: // Unk 2
		return 0;
	case 12: // Set palette
		return 0;
	case 13: // Get options
		OutputDebugString("Rasterizer: Get options\n");
		return 0;
	}
	return 1;
}

float palette_r[256];
float palette_g[256];
float palette_b[256];

int __cdecl SetPaletteRange(unsigned char *a1, int a2, int numPaletteEntries) {
	for (int i = 0; i < numPaletteEntries; i++)
	{
		palette_r[i] = a1[i * 4 + 0] / 255.0f;
		palette_g[i] = a1[i * 4 + 1] / 255.0f;
		palette_b[i] = a1[i * 4 + 2] / 255.0f;
	}

	glPixelTransferi(GL_MAP_COLOR, TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 256, palette_r);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 256, palette_g);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 256, palette_b);

	return 0;
}

int __cdecl Shutdown() {
	return 0;
}

int __cdecl Terminate() {
	return 0;
}

int __cdecl BlankBuffers(int a1, int a2) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	return 1;
}

BOOL __cdecl Configure(HWND hWndParent) {
	return FALSE;
}