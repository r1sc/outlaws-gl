#pragma once

#include <Windows.h>

typedef struct {
	int action;
	void* unk;
} t_RasterizerArgs;

#pragma pack(push, 1)
typedef struct {
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
	unsigned int useSpriteAlpha;
	unsigned int smoothClose;
	unsigned int smoothFar;
	unsigned int translucencyFx; //1
	unsigned int translucentWater;
	unsigned int tintingFX;
	unsigned int smallTextures;
	unsigned int smallSprites;
	unsigned int notAllSpriteDirections;
	unsigned int singlePassRender; // 0
	unsigned int hardwareOverlays; // 1
	unsigned int noGun; // 1
} t_RenderOptions;

typedef struct {
	unsigned char* buffer;
	unsigned int width;
	unsigned int height;
	unsigned int bytesPerPixel;
} t_AllocateTexture;

typedef struct {
	unsigned int val;
	t_AllocateTexture* alloc;
	void* result;
} t_AllocateTextureQuery;

typedef struct {
	unsigned int i1;
	unsigned int i2;
	unsigned int i3;
	unsigned int textureTag;
	unsigned int flags;
} t_Triangle;

typedef struct {
	float x, y, z;
	float w;
	float unk1;
	float unk2;
	float s, t;
} t_Vertex;

typedef struct {
	unsigned int numVertices;
	t_Vertex* vertices;
	unsigned int numTriangles;
	t_Triangle* triangles;
} t_Render3dInput;


typedef struct {
	unsigned int textureNumber;
	void* tag;
	unsigned int* textureTag;
} t_AddTexture;

typedef struct {
	int width;
	int bitColorDepth;
	int bytesPerPixel;
	int unk3;
	int unk4;
	int unk5;
	int unk6;
	int unk7;
	int initAs3;
	int unk9;
	int unk10;
	int unk11;
	int unk12;
} t_UnkForNow;

typedef struct {
	int bytesPerPixel;
	int field_4;
	int field_8;
	int field_C;
	int field_10;
	int field_14;
	int field_18;
	int field_1C;
} t_GetColorFormatInfoResult;

typedef struct {
	int texId;
	t_GetColorFormatInfoResult* result;
} t_GetColorFormatInfoInput;

typedef struct {
	unsigned int action;
	union {
		t_RenderOptions renderOptions;
		t_AllocateTextureQuery allocateTextureQuery;
		unsigned int ary[];
		unsigned char data[];
		t_Render3dInput render3dInput;
		t_AddTexture addTextureInput;
		t_GetColorFormatInfoInput getColorFormatInfoInput;
		unsigned int* removeTextureInput;
	};
} t_RasterizeHook;

