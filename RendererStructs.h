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
	unsigned int hardwareDepthBuffer; // 1
} t_RenderOptionsOutput;

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
} t_AllocateTextureInOut;

typedef struct {
	unsigned int vertexIndex1;
	unsigned int vertexIndex2;
	unsigned int vertexIndex3;
	unsigned int textureTag;
	unsigned int flags;
} t_Triangle;

typedef struct {
	float x, y, z;
	float w;
	unsigned int color;
	float unk;
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
} t_AddTextureInput;

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

enum RasterizerHookAction {
	STARTUP = 0,
	SHUTDOWN = 1,
	OPEN_DEVICE = 2,
	CLOSE_DEVICE = 3,
	CLEAR_Z = 4,
	CLEAR_VIEWPORT = 5,
	ALLOCATE_TEXTURE = 6,
	ADD_TEXTURE = 7,
	RENDER = 8,
	REMOVE_TEXTURE = 9,
	GET_COLOR_FORMAT_INFO = 10,
	UNK_1 = 11,
	SET_PALETTE = 12,
	GET_OPTIONS = 13
};

typedef struct {
	RasterizerHookAction action;
	union {
		t_RenderOptionsOutput renderOptions;
		t_AllocateTextureInOut allocateTextureInOut;
		t_Render3dInput render3dInput;
		t_AddTextureInput addTextureInput;
		t_GetColorFormatInfoInput getColorFormatInfoInput;
		unsigned int* removeTextureInput;
		unsigned int* paletteInput;
	};
} t_RasterizeHook;

