#pragma once
#include "RendererStructs.h"
#include <stdio.h>
#include <Windows.h>

typedef int (__cdecl *f_Startup)(void); // return 0
typedef int (__cdecl *f_Initialize)(void *a1);

typedef int (__cdecl *f_Pause)();
typedef int (__cdecl *f_Resume)();
typedef int (__cdecl *f_GDIPause)();
typedef int (__cdecl *f_GDIResume)();

// The following 2 methods all point to the same function, a stub that just returns 0
typedef int (__cdecl *f_DirtyRect)(); // Always return 0
typedef int (__cdecl *f_Save)(); // Always 0

typedef int (__cdecl *f_Restore)(); // Always 0
typedef int (__cdecl *f_GetMainWindow)(); // Return window handle
typedef int (__cdecl *f_Allocate)(t_AllocateArgs *a1); // ?
typedef int (__cdecl *f_Free)();
typedef signed int (__cdecl *f_GetDisplayModeList)(int *numModes, t_DisplayMode **a2);
typedef int(__cdecl *f_SetDisplayMode)(t_SetDisplayModeArgs *a1);
typedef int (__cdecl *f_GetDisplayMode)(int *a1);
typedef int (__cdecl *f_LockBuffer)(int a1, int a2);
typedef int (__cdecl *f_UnlockBuffer)(int a1, int a2);
typedef int (__cdecl *f_PageFlip)();
typedef char (__cdecl *f_RasterizerHook)(t_RasterizerArgs *a1);

typedef int (__cdecl *f_SetPaletteRange)(char *a1, int a2, int numPaletteEntries); // a2 is multiplied by 4 and addded
typedef void (__cdecl *f_Shutdown)();
typedef void (__cdecl *f_Terminate)();

typedef int(__cdecl *f_BlankBuffers)(int a1, int a2);
typedef BOOL(__cdecl *f_Configure)(HWND hWndParent);