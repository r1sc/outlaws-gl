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
		int screenWidth = GetSystemMetrics(0);
		int screenHeight = GetSystemMetrics(1);

		currentDisplayMode.width = a1->width;
		currentDisplayMode.height = a1->height;
		currentDisplayMode.bpp = a1->bpp;

		SetWindowPos(hWnd, 0, screenWidth/2 - (a1->width/2), screenHeight/2 - (a1->height / 2), a1->width, a1->height, 0);
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
	memset(video_Buffer, 0, currentDisplayMode.width * currentDisplayMode.height);
	Sleep(1);
	return 0;
}

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
		return AllocateTexture(data->allocateTextureQuery);
	case 7: // Add texture
		return AddTexture(data->addTextureInput);
	case 8: // Render vertices and triangles	
	{	
		do3d = true;
		Render3d(data->render3dInput);
		return 0;
	}
	case 9: // Remove texture
		return RemoveTexture(data->removeTextureInput);
	case 10: // Get color format info
		//GetColorFormatInfo(data->getColorFormatInfoInput);
		return 0;
	case 11: // Unk 2
		return 0;
	case 12: // Set palette
	{
		float palette_r[256];
		float palette_g[256];
		float palette_b[256];
		float palette_a[256];

		unsigned char* palette = (unsigned char*)data->ary[0];

		for (int i = 0; i < 256; i++)
		{
			palette_r[i] = palette[i * 4 + 0] / 255.0f;
			palette_g[i] = palette[i * 4 + 1] / 255.0f;
			palette_b[i] = palette[i * 4 + 2] / 255.0f;
			palette_a[i] = i == 0 ? 0 : 1.0f;
		}

		glPixelTransferi(GL_MAP_COLOR, TRUE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 256, palette_r);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 256, palette_g);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 256, palette_b);
		glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 256, palette_a);
		return 0;
	}
	case 13: // Get options
		data->renderOptions.useSpriteAlpha = 1;
		data->renderOptions.smoothClose = 1;
		data->renderOptions.smoothFar = 1;
		data->renderOptions.translucencyFx = 0;
		data->renderOptions.translucentWater = 0;
		data->renderOptions.smallTextures = 0;
		data->renderOptions.smallSprites = 0;
		data->renderOptions.notAllSpriteDirections = 0;
		data->renderOptions.singlePassRender = 1;
		data->renderOptions.hardwareOverlays = 0;
		data->renderOptions.noGun = 0;
		OutputDebugString("Rasterizer: Get options\n");
		return 0;
	}
	return 1;
}


int __cdecl SetPaletteRange(unsigned char *a1, int a2, int numPaletteEntries) {
	float palette_r[256];
	float palette_g[256];
	float palette_b[256];
	float palette_a[256];

	for (int i = 0; i < numPaletteEntries; i++)
	{
		palette_r[i] = a1[i * 4 + 0] / 255.0f;
		palette_g[i] = a1[i * 4 + 1] / 255.0f;
		palette_b[i] = a1[i * 4 + 2] / 255.0f;
		palette_a[i] = 1.0f;
	}

	glPixelTransferi(GL_MAP_COLOR, TRUE);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_R, 256, palette_r);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_G, 256, palette_g);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_B, 256, palette_b);
	glPixelMapfv(GL_PIXEL_MAP_I_TO_A, 256, palette_a);

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