#include <Windows.h>
#include "RendererInterface.h"

HMODULE d3dLibModule;
f_Startup origStartup;
f_Initialize origInitialize;
f_Pause origPause;
f_Resume origResume;
f_GDIPause origGDIPause;
f_GDIResume origGDIResume;
f_DirtyRect origDirtyRect;
f_Save origSave;
f_Restore origRestore;
f_GetMainWindow origGetMainWindow;
f_Allocate origAllocate;
f_Free origFree;
f_GetDisplayModeList origGetDisplayModeList;
f_SetDisplayMode origSetDisplayMode;
f_GetDisplayMode origGetDisplayMode;
f_LockBuffer origLockBuffer;
f_UnlockBuffer origUnlockBuffer;
f_PageFlip origPageFlip;
f_RasterizerHook origRasterizerHook;
f_SetPaletteRange origSetPaletteRange;
f_Shutdown origShutdown;
f_Terminate origTerminate;
f_BlankBuffers origBlankBuffers;
f_Configure origConfigure;

HINSTANCE hInstance;

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
	hInstance = hinstDLL;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		/*d3dLibModule = LoadLibrary("LECD3D.dll");
		origStartup = (f_Startup)GetProcAddress(d3dLibModule, "Startup");
		origInitialize = (f_Initialize)GetProcAddress(d3dLibModule, "Initialize");
		origPause = (f_Pause)GetProcAddress(d3dLibModule, "Pause");
		origResume = (f_Resume)GetProcAddress(d3dLibModule, "Resume");
		origGDIPause = (f_GDIPause)GetProcAddress(d3dLibModule, "GDIPause");
		origGDIResume = (f_GDIResume)GetProcAddress(d3dLibModule, "GDIResume");
		origDirtyRect = (f_DirtyRect)GetProcAddress(d3dLibModule, "DirtyRect");
		origSave = (f_Save)GetProcAddress(d3dLibModule, "Save");
		origRestore = (f_Restore)GetProcAddress(d3dLibModule, "Restore");
		origGetMainWindow = (f_GetMainWindow)GetProcAddress(d3dLibModule, "GetMainWindow");
		origAllocate = (f_Allocate)GetProcAddress(d3dLibModule, "Allocate");
		origFree = (f_Free)GetProcAddress(d3dLibModule, "Free");
		origGetDisplayModeList = (f_GetDisplayModeList)GetProcAddress(d3dLibModule, "GetDisplayModeList");
		origSetDisplayMode = (f_SetDisplayMode)GetProcAddress(d3dLibModule, "SetDisplayMode");
		origGetDisplayMode = (f_GetDisplayMode)GetProcAddress(d3dLibModule, "GetDisplayMode");
		origLockBuffer = (f_LockBuffer)GetProcAddress(d3dLibModule, "LockBuffer");
		origUnlockBuffer = (f_UnlockBuffer)GetProcAddress(d3dLibModule, "UnlockBuffer");
		origPageFlip = (f_PageFlip)GetProcAddress(d3dLibModule, "PageFlip");
		origRasterizerHook = (f_RasterizerHook)GetProcAddress(d3dLibModule, "RasterizerHook");
		origSetPaletteRange = (f_SetPaletteRange)GetProcAddress(d3dLibModule, "SetPaletteRange");
		origShutdown = (f_Shutdown)GetProcAddress(d3dLibModule, "Shutdown");
		origTerminate = (f_Terminate)GetProcAddress(d3dLibModule, "Terminate");
		origBlankBuffers = (f_BlankBuffers)GetProcAddress(d3dLibModule, "BlankBuffers");
		origConfigure = (f_Configure)GetProcAddress(d3dLibModule, "Configure");*/

		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		FreeLibrary(d3dLibModule);
		break;
	}
	return TRUE;
}

