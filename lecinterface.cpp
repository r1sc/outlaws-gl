#include "RendererInterface.h"
#include "RendererStructs.h"

extern HINSTANCE hInstance;

//extern f_Startup origStartup;
//extern f_Initialize origInitialize;
//extern f_Pause origPause;
//extern f_Resume origResume;
//extern f_GDIPause origGDIPause;
//extern f_GDIResume origGDIResume;
//extern f_DirtyRect origDirtyRect;
//extern f_Save origSave;
//extern f_Restore origRestore;
//extern f_GetMainWindow origGetMainWindow;
//extern f_Allocate origAllocate;
//extern f_Free origFree;
//extern f_GetDisplayModeList origGetDisplayModeList;
//extern f_SetDisplayMode origSetDisplayMode;
//extern f_GetDisplayMode origGetDisplayMode;
//extern f_LockBuffer origLockBuffer;
//extern f_UnlockBuffer origUnlockBuffer;
//extern f_PageFlip origPageFlip;
//extern f_RasterizerHook origRasterizerHook;
//extern f_SetPaletteRange origSetPaletteRange;
//extern f_Shutdown origShutdown;
//extern f_Terminate origTerminate;
//extern f_BlankBuffers origBlankBuffers;
//extern f_Configure origConfigure;

typedef struct {
	HWND hWnd;
	int unk;
} t_InitializeArgs;

HWND hWnd;
void *scratchMem = NULL;

int __cdecl Startup() {
	//return origStartup();
	return 0;
}

int __cdecl Initialize(t_InitializeArgs *a1) {
	//return origInitialize(a1);
	if (a1->hWnd) {
		a1->unk = 1;
		hWnd = a1->hWnd;
	}
	return 0;
}

int __cdecl Pause() {
	//return origPause();
	return 0;
}

int __cdecl Resume() {
	//return origResume();
	return 0;
}

int __cdecl GDIPause() {
	//return origGDIPause();
	return 0;
}

int __cdecl GDIResume() {
	//return origGDIResume();
	return 0;
}

int __cdecl DirtyRect() {
	//return origDirtyRect();
	return 0;
}

int __cdecl Save() {
	//return origSave();
	return 0;
}

int __cdecl Restore() {
	//return origRestore();
	return 0;
}

HWND __cdecl GetMainWindow() {
	//return origGetMainWindow();
	return IsWindow(hWnd) != 0 ? hWnd : NULL;
}

int __cdecl Allocate(t_AllocateArgs *a1) {
	//return origAllocate(a1);
	if (IsWindow(hWnd)) {
		SetWindowPos(hWnd, (HWND)0xFFFFFFFE, 0, 0, a1->width, a1->height, 0);
		SetActiveWindow(hWnd);
		SetFocus(hWnd);

		if (!scratchMem)
			scratchMem = malloc(640 * 480 * 2);
		return 0;
	}
	return 1;
}

int __cdecl Free() {
	//return origFree();
	free(scratchMem);
	return 0;
}

t_DisplayMode displayModes[2] = {
	{640,480,16,0,{0}},
	{800,600,16,0,{0}}
};
signed int __cdecl GetDisplayModeList(int *a1, t_DisplayMode **a2) {
	//return origGetDisplayModeList(a1, a2);
	*a1 = 2;
	*a2 = displayModes;
	return 0;
}

int __cdecl SetDisplayMode(t_SetDisplayModeArgs *a1) {
	//return origSetDisplayMode(a1);
	return 0;
}

t_SetDisplayModeArgs defaultDisplayMode;
int __cdecl GetDisplayMode(t_SetDisplayModeArgs *a1) {
	//return origGetDisplayMode(a1);
	memset(&defaultDisplayMode, 0, 36);
	defaultDisplayMode.width = 640;
	defaultDisplayMode.height = 480;
	defaultDisplayMode.bpp = 16;
	a1 = &defaultDisplayMode;
	return 0;
}

int __cdecl LockBuffer(int a1, t_LockBufferArgs *a2) {
	//return origLockBuffer(a1, a2);
	a2->pixelData = scratchMem;
	a2->dword4 = 640;
	a2->dwordC = 2;
	a2->dword14 = 0;
	a2->dword10 = 0;
	a2->bufferWidth = 640;
	a2->height = a2->bufferHeight = 480;
	return 0;
}

int __cdecl UnlockBuffer(int a1, int a2) {
	//return origUnlockBuffer(a1, a2);
	return 0;
}

int __cdecl PageFlip() {
	//return origPageFlip();
	return 0;
}

char __cdecl RasterizerHook(t_RasterizerArgs *a1) {
	//return origRasterizerHook(a1);
	switch (a1->action)
	{
	case 0:	// Startup - return 1 on fail
		return 0;
	case 1: // Shutdown
		return 0;
	case 2: // Open - return 0 on fail
		return 1;
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

int __cdecl SetPaletteRange(char *a1, int a2, int numPaletteEntries) {
	//return origSetPaletteRange(a1, a2, numPaletteEntries);
	return 0;
}

int __cdecl Shutdown() {
	//return origShutdown();
	return 0;
}

int __cdecl Terminate() {
	//return origTerminate();
	return 0;
}

int __cdecl BlankBuffers(int a1, int a2) {
	//return origBlankBuffers(a1, a2);
	return 0;
}

BOOL __cdecl Configure(HWND hWndParent) {
	return FALSE;
}