#include <stdio.h>
#include <math.h>

#include "RendererStructs.h"
#include "glrenderer.h"
#include "settings.h"

extern HINSTANCE hInstance;
HWND hWnd;
t_DisplayMode currentDisplayMode;

int __cdecl Startup() {	
	return 0;
}

int __cdecl Initialize(t_InitializeArgs *a1) {
	ReadSettings();

	if (a1->hWnd) {
		a1->isFullscreen = fullscreen;
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
		
		DWORD dwStyle, dwStyleEx;

		if (fullscreen) {
			windowResolutionWidth = screenWidth;
			windowResolutionHeight = screenHeight;
			dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			dwStyleEx = WS_EX_APPWINDOW;
			ShowCursor(FALSE);
		}
		else {
			dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
			dwStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		}

		SetWindowLongPtr(hWnd, GWL_STYLE, dwStyle);
		SetWindowLongPtr(hWnd, GWL_EXSTYLE, dwStyleEx);
		SetWindowPos(hWnd, 0, screenWidth/2 - (windowResolutionWidth/2), screenHeight/2 - (windowResolutionHeight / 2), windowResolutionWidth, windowResolutionHeight, SWP_SHOWWINDOW);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);

		AllocatePixelBuffer(a1->width, a1->height, a1->bpp / 8);
		return 0;
	}
	return 1;
}

int __cdecl Free() {
	return 0;
}

t_DisplayMode displayModes[2] = {
	{640,480,16,0,{0}},
	{640,360,16,0,{0}}
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
	a2->pixelData = videoBuffer;
	a2->stride = currentDisplayMode.width;
	a2->bytesPerPixel = 1;
	a2->dword14 = 0;
	a2->dword10 = 0;
	a2->bufferWidth = currentDisplayMode.width;
	a2->height = a2->bufferHeight = currentDisplayMode.height;	
	return 0;
}

int __cdecl UnlockBuffer(int a1, int a2) {
	if (!rendering3d) {
		DrawPixelBuffer();
		VideoSwapBuffers();
	}
	return 0;
}

int __cdecl PageFlip() {
	if (rendering3d) {
		DrawPixelBuffer();
		VideoSwapBuffers();
		memset(videoBuffer, 0, currentDisplayMode.width * currentDisplayMode.height);
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	return 0;
}

int __cdecl SetPaletteRange(unsigned int* palette, int a2, int numPaletteEntries) {
	float palette_r[256];
	float palette_g[256];
	float palette_b[256];
	float palette_a[256];

	for (int i = 0; i < numPaletteEntries; i++)
	{
		palette_b[i] = ((palette[i] >> 16) & 0xFF) / 255.0f;
		palette_g[i] = ((palette[i] >> 8) & 0xFF) / 255.0f;
		palette_r[i] = (palette[i] & 0xFF) / 255.0f;
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

int __cdecl RasterizerHook(t_RasterizeHook* data) {
	switch (data->action)
	{
	case RasterizerHookAction::STARTUP:
		OutputDebugString("Rasterizer: Startup\n");
		return 0;
	case RasterizerHookAction::SHUTDOWN:
		OutputDebugString("Rasterizer: Shutdown\n");
		return 0;
	case RasterizerHookAction::OPEN_DEVICE:		
		OutputDebugString("Rasterizer: Open\n");
		OpenRenderer();
		return 0;
	case RasterizerHookAction::CLOSE_DEVICE:
		OutputDebugString("Rasterizer: Close\n");
		CloseRenderer();
		return 0;
	case RasterizerHookAction::CLEAR_Z:
		glClear(GL_DEPTH_BUFFER_BIT);
		return 0;
	case RasterizerHookAction::CLEAR_VIEWPORT:
		glClear(GL_COLOR_BUFFER_BIT);
		return 0;
	case RasterizerHookAction::ALLOCATE_TEXTURE:
		return AllocateTexture(data->allocateTextureInOut);
	case RasterizerHookAction::ADD_TEXTURE:
		return AddTexture(data->addTextureInput);
	case RasterizerHookAction::RENDER:
	{	
		Render3d(data->render3dInput);
		return 0;
	}
	case RasterizerHookAction::REMOVE_TEXTURE:
		return RemoveTexture(data->removeTextureInput);
	case RasterizerHookAction::GET_COLOR_FORMAT_INFO:
		return 0;
	case RasterizerHookAction::UNK_1:
		return 0;
	case RasterizerHookAction::SET_PALETTE:
	{
		SetPaletteRange(data->paletteInput, 0, 256);
		return 0;
	}
	case RasterizerHookAction::GET_OPTIONS:
		data->renderOptions.useSpriteAlpha = 1;
		data->renderOptions.smoothClose = 1;
		data->renderOptions.smoothFar = 1;
		data->renderOptions.translucencyFx = 1;
		data->renderOptions.translucentWater = 1;
		data->renderOptions.smallTextures = 0;
		data->renderOptions.smallSprites = 0;
		data->renderOptions.notAllSpriteDirections = 0;
		data->renderOptions.singlePassRender = 1;
		data->renderOptions.hardwareOverlays = 1;
		data->renderOptions.hardwareDepthBuffer = 1;
		OutputDebugString("Rasterizer: Get options\n");
		return 0;
	}
	return 1;
}

int __cdecl Shutdown() {
	return 0;
}

int __cdecl Terminate() {
	return 0;
}

int __cdecl BlankBuffers(int a1, int a2) {
	memset(videoBuffer, 0, currentDisplayMode.width * currentDisplayMode.height);
	return 1;
}

BOOL __cdecl Configure(HWND hWndParent) {
	return FALSE;
}