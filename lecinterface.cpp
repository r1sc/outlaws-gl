#include "RendererStructs.h"
#include "glrenderer.h"

extern HINSTANCE hInstance;
HWND hWnd;
t_DisplayMode currentDisplayMode;

int __cdecl Startup() {
	SetPixelFormatAndCreateContext(hWnd);
	return 0;
}

int __cdecl Initialize(t_InitializeArgs *a1) {
	if (a1->hWnd) {
		a1->unk = 1;
		hWnd = a1->hWnd;
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

		SetWindowPos(hWnd, HWND_TOP, 0, 0, a1->width, a1->height, 0);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);

		AllocatePixelBuffer(a1->width, a1->height);
		return 0;
	}
	return 1;
}

int __cdecl Free() {
	free(video_Buffer);
	return 0;
}

t_DisplayMode displayModes[2] = {
	{640,480,16,0,{0}},
	{800,600,16,0,{0}}
};
signed int __cdecl GetDisplayModeList(int *a1, t_DisplayMode **a2) {
	*a1 = 2;
	*a2 = displayModes;
	return 0;
}

int __cdecl SetDisplayMode(t_DisplayMode *a1) {
	currentDisplayMode.width = a1->width;
	currentDisplayMode.height = a1->height;
	currentDisplayMode.bpp = a1->bpp;

	ResetVideo();
	return 0;
}

int __cdecl GetDisplayMode(t_DisplayMode **a1) {
	*a1 = &currentDisplayMode;
	return 0;
}

int __cdecl LockBuffer(int a1, t_LockBufferArgs *a2) {
	a2->pixelData = video_Buffer;
	a2->stride = 640;
	a2->bytesPerPixel = 1;
	a2->dword14 = 0;
	a2->dword10 = 0;
	a2->bufferWidth = 640;
	a2->height = a2->bufferHeight = 480;
	return 0;
}

int __cdecl UnlockBuffer(int a1, int a2) {
	return 0;
}

int __cdecl PageFlip() {
	DrawPixelBuffer();
	VideoSwapBuffers();
	Sleep(1);
	return 0;
}

char __cdecl RasterizerHook(t_RasterizerArgs *a1) {
	switch (a1->action)
	{
	case 0:	// Startup
		return 1;
	case 1: // Shutdown
		return 0;
	case 2: // Open
		return 0;
	case 3: // Close
		return 1;
	case 4: // Clear z-buffer
		return 0;
	case 5: // Clear viewport
		return 0;
	case 6: // Allocate texture
		return 0;
	case 7: // Add texture
		return 0;
	case 8: // Add vertices and triangles
		return 1;
	case 9: // Remove texture
		return 0;
	case 10: // Unk 1
		return 0;
	case 11: // Unk 2
		return 0;
	case 12: // Unk 3
		return 1;
	case 13: // Clean up?
		return 0;
	}
	return 0;
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
	return 0;
}

BOOL __cdecl Configure(HWND hWndParent) {
	return FALSE;
}