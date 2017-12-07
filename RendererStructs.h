#pragma once

#include <Windows.h>

typedef struct {
	int action;
	void *unk;
} t_RasterizerArgs;

#pragma pack(push, 1)
typedef struct  {
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
	int unk1;
	char str[20];
} t_DisplayMode;
#pragma pack(pop)

typedef struct {
	unsigned int width;
	unsigned int height;
	unsigned int bpp;
} t_AllocateArgs;

#pragma pack(push, 1)
typedef struct
{
	unsigned short* pixelData;
	int stride = 1024;
	int height;
	int bytesPerPixel = 2;
	int dword10 = 0;
	int dword14 = 0;
	unsigned int bufferWidth;
	unsigned int bufferHeight;
} t_LockBufferArgs;
#pragma pack(pop)

typedef struct {
	HWND hWnd;
	int unk;
} t_InitializeArgs;
