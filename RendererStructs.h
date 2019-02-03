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
	unsigned char* pixelData;
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
	int isFullscreen;
} t_InitializeArgs;


typedef struct {
	unsigned int noSpriteAlpha;
	unsigned int smoothClose;
	unsigned int smoothFar;
	unsigned int unk1; //1
	unsigned int translucentWater;
	unsigned int tintingFX;
	unsigned int smallTextures;
	unsigned int smallSprites;
	unsigned int notAllSpriteDirections;
	unsigned int unk2; // 0
	unsigned int unk3; // 1
	unsigned int unk4; // 1
} t_RenderOptions;

typedef struct {
	unsigned int* buffer;
	unsigned int width;
	unsigned int height;
	unsigned int bytesPerPixel;
	unsigned int tag2;
	unsigned int tag3;
} t_AllocateTexture;

typedef struct {
	unsigned int type;
	t_AllocateTexture* alloc;	
	unsigned int tag;
} t_AllocateTextureQuery;

typedef struct {
	unsigned int action;
	union {
		t_RenderOptions renderOptions;
		t_AllocateTextureQuery allocateTextureQuery;
		unsigned int ary[];
		unsigned char data[];
	};
} t_RasterizeHook;