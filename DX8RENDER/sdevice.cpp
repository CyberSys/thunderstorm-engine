#include "sdevice.h"
#include "stdio.h"
#include <d3dx9math.h>
#include "..\common_h\matrix.h"
#include "texture.h"
#include <io.h>

#define POST_PROCESS_FVF D3DFVF_XYZRHW | D3DFVF_TEX4

#define S_RELEASE(a,b) if (a) { ULONG refc = a->Release(); a = NULL;}

class DX8RENDER_SCRIPT_LIBRIARY : public SCRIPT_LIBRIARY
{
public:
	DX8RENDER_SCRIPT_LIBRIARY(){};
	~DX8RENDER_SCRIPT_LIBRIARY(){};
	virtual bool Init();
};


INTERFACE_FUNCTION
CREATE_SERVICE(DX8RENDER)
CREATE_SCRIPTLIBRIARY(DX8RENDER_SCRIPT_LIBRIARY)


DX8RENDER * DX8RENDER::pRS = null;

DWORD __cdecl DX8SetTexturePath(VS_STACK * pS)
{
	VDATA * pString = (VDATA*)pS->Pop();
	VDATA * pNumber = (VDATA*)pS->Pop();

	long iNumber = pNumber->GetLong();
	char * pStr = pString->GetString();

	if (!DX8RENDER::pRS)
	{
		api->CreateService("dx8render"); Assert(DX8RENDER::pRS);
	}

	VDATA * pVR = (VDATA*)pS->Push();
	if (!pVR || iNumber<0 || iNumber>=4)
	{
		pVR->Set(long(0));
		return IFUNCRESULT_OK;
	}

	DX8RENDER::pRS->TextureCreate((const char *)-1);
	DX8RENDER::pRS->TextureCreate((const char *)iNumber);
	DX8RENDER::pRS->TextureCreate((const char *)pStr);

	pVR->Set(long(1));

	return IFUNCRESULT_OK;
}

DWORD __cdecl RPrint(VS_STACK * pS)
{
	VDATA * pString = (VDATA*)pS->Pop();
	long y = ((VDATA*)pS->Pop())->GetLong();
	long x = ((VDATA*)pS->Pop())->GetLong();

	if (pString->GetString()) DX8RENDER::pRS->Print(x, y, pString->GetString());
	VDATA * pVR = (VDATA*)pS->Push();
	pVR->Set(long(0));
	return IFUNCRESULT_OK;
}

DWORD __cdecl SetGlowParams(VS_STACK * pS)
{
	float fBlurBrushSize = ((VDATA*)pS->Pop())->GetFloat();
	long Intensivity = ((VDATA*)pS->Pop())->GetLong();
	long BlurPasses = ((VDATA*)pS->Pop())->GetLong();

	DX8RENDER::pRS->SetGLOWParams(fBlurBrushSize, Intensivity, BlurPasses);

	VDATA * pVR = (VDATA*)pS->Push();
	pVR->Set(long(0));
	return IFUNCRESULT_OK;
}


bool DX8RENDER_SCRIPT_LIBRIARY::Init()
{
	IFUNCINFO sIFuncInfo;

	sIFuncInfo.nArguments = 2;
	sIFuncInfo.pFuncName = "SetTexturePath";
	sIFuncInfo.pReturnValueName = "int";
	sIFuncInfo.pFuncAddress = DX8SetTexturePath;
	api->SetScriptFunction(&sIFuncInfo);

	sIFuncInfo.nArguments = 3;
	sIFuncInfo.pFuncName = "RPrint";
	sIFuncInfo.pReturnValueName = "int";
	sIFuncInfo.pFuncAddress = RPrint;
	api->SetScriptFunction(&sIFuncInfo);

	sIFuncInfo.nArguments = 3;
	sIFuncInfo.pFuncName = "SetGlowParams";
	sIFuncInfo.pReturnValueName = "int";
	sIFuncInfo.pFuncAddress = SetGlowParams;
	api->SetScriptFunction(&sIFuncInfo);

	return true;
}

dword _rdtsc;
dword dwTotalSize = 0;
dword dwSplashTime = 0;
bool bSplash = false;
#pragma warning(push)
#pragma warning(disable : 4305 4309)
char sSplashText[] = {0xbb,0x9a,0x89,0x9a,0x93,0x90,0x8f,0x9a,0x8d,0x8c,0xdf,0xbc,0x90,
0x8f,0x86,0xd1,0xdf,0xb1,0x90,0x8b,0xdf,0x99,0x90,0x8d,0xdf,0xac,0x9e,0x93,0x9a,0xd1,0};
#pragma warning(pop)
char splashbuffer[256];

#ifndef _XBOX
	#define TEXTURESDIR		"resource\\textures\\%s.tx"
	#define VIDEODIR		"Resource\\Videos\\%s"
#else
	#define TEXTURESDIR		"resource\\textures\\%s.tx"
	#define VIDEODIR		"d:\\Resource\\Videos\\%s"

	#include <xgraphics.h>
#endif

struct DX8SphVertex
{
	CVECTOR v;
	dword	c;
};

dword DX8sphereNumTrgs;
DX8SphVertex * DX8sphereVertex = null;

void CreateSphere()
{
#define CalcKColor(ind) {kColor = light | !CVECTOR(DX8sphereVertex[t*3 + ind].v.x, DX8sphereVertex[t*3 + ind].v.y, DX8sphereVertex[t*3 + ind].v.z); if(kColor < 0.0f) kColor = 0.0f; }
#define CLerp(c, min) (dword(c*(kColor*(1.0f - min) + min)))
#define Color1 ((CLerp(255.0f, 0.5f) << 24) | (CLerp(255.0f, 0.7f) << 16) | (CLerp(255.0f, 0.7f) << 8) | (CLerp(255.0f, 0.7f) << 0));

	if(DX8sphereVertex) return;

	const float myPI = 3.1415926535897932f;
	const long a1 = 32;
	const long a2 = (a1/2);

	DX8sphereNumTrgs = a1*a2*2;
	DX8sphereVertex = NEW DX8SphVertex[DX8sphereNumTrgs*6];

	CVECTOR light = !CVECTOR(0.0f, 0.0f, 1.0f);
	float kColor;
	//��������� ������v
	for(long i = 0, t = 0; i < a2; i++)
	{
		float r1 = sinf(myPI*i/float(a2));
		float y1 = cosf(myPI*i/float(a2));
		float r2 = sinf(myPI*(i + 1)/float(a2));
		float y2 = cosf(myPI*(i + 1)/float(a2));
		for(long j = 0; j < a1; j++)
		{
			float x1 = sinf(2.0f*myPI*j/float(a1));
			float z1 = cosf(2.0f*myPI*j/float(a1));
			float x2 = sinf(2.0f*myPI*(j + 1)/float(a1));
			float z2 = cosf(2.0f*myPI*(j + 1)/float(a1));
			//0
			DX8sphereVertex[t*3 + 0].v.x = r1*x1;
			DX8sphereVertex[t*3 + 0].v.y = y1;
			DX8sphereVertex[t*3 + 0].v.z = r1*z1;
			CalcKColor(0);
			DX8sphereVertex[t*3 + 0].c = Color1;
			//1
			DX8sphereVertex[t*3 + 1].v.x = r2*x1;
			DX8sphereVertex[t*3 + 1].v.y = y2;
			DX8sphereVertex[t*3 + 1].v.z = r2*z1;
			CalcKColor(1);
			DX8sphereVertex[t*3 + 1].c = Color1;
			//2
			DX8sphereVertex[t*3 + 2].v.x = r1*x2;
			DX8sphereVertex[t*3 + 2].v.y = y1;
			DX8sphereVertex[t*3 + 2].v.z = r1*z2;
			CalcKColor(2);
			DX8sphereVertex[t*3 + 2].c = Color1;
			//3 = 2
			DX8sphereVertex[t*3 + 3] = DX8sphereVertex[t*3 + 2];
			//4 = 1
			DX8sphereVertex[t*3 + 4] = DX8sphereVertex[t*3 + 1];
			//5
			DX8sphereVertex[t*3 + 5].v.x = r2*x2;
			DX8sphereVertex[t*3 + 5].v.y = y2;
			DX8sphereVertex[t*3 + 5].v.z = r2*z2;
			CalcKColor(5);
			DX8sphereVertex[t*3 + 5].c = Color1;
			//-������� 2 ������������
			t += 2;
		}
	}
}

D3DERRORS D3D_ERRORS[] =
{
	{	D3D_OK,								"No error occurred"	},
	{	D3DERR_CONFLICTINGRENDERSTATE,		"The currently set render states cannot be used together"	},
	{	D3DERR_CONFLICTINGTEXTUREFILTER,	"The current texture filters cannot be used together"	},
	{	D3DERR_CONFLICTINGTEXTUREPALETTE,	"The current textures cannot be used simultaneously This generally occurs when a multitexture device requires that all palletized textures simultaneously enabled also share the same palette"	},
	{	D3DERR_DEVICELOST,					"The device is lost and cannot be restored at the current time, so rendering is not possible"	},
	{	D3DERR_DEVICENOTRESET,				"The device cannot be reset"	},
	{	D3DERR_DRIVERINTERNALERROR,			"Internal driver error"	},
	{	D3DERR_INVALIDCALL,					"The method call is invalid For example, a method's parameter may have an invalid value"	},
	{	D3DERR_INVALIDDEVICE,				"The requested device type is not valid"	},
	{	D3DERR_MOREDATA,					"There is more data available than the specified buffer size can hold"	},
	{	D3DERR_NOTAVAILABLE,				"This device does not support the queried technique"	},
	{	D3DERR_NOTFOUND,					"The requested item was not found"	},
	{	D3DERR_OUTOFVIDEOMEMORY,			"Direct3D does not have enough display memory to perform the operation"	},
	{	D3DERR_TOOMANYOPERATIONS,			"The application is requesting more texture-filtering operations than the device supports"	},
	{	D3DERR_UNSUPPORTEDALPHAARG,			"The device does not support a specified texture-blending argument for the alpha channel"	},
	{	D3DERR_UNSUPPORTEDALPHAOPERATION,	"The device does not support a specified texture-blending operation for the alpha channel"	},
	{	D3DERR_UNSUPPORTEDCOLORARG,			"The device does not support a specified texture-blending argument for color values"	},
	{	D3DERR_UNSUPPORTEDCOLOROPERATION,	"The device does not support a specified texture-blending operation for color values"	},
	{	D3DERR_UNSUPPORTEDFACTORVALUE,		"The device does not support the specified texture factor value"	},
	{	D3DERR_UNSUPPORTEDTEXTUREFILTER,	"The device does not support the specified texture filter"	},
	{	D3DERR_WRONGTEXTUREFORMAT,			"The pixel format of the texture surface is not valid"	},
	{	E_FAIL,								"An undetermined error occurred inside the Direct3D subsystem"	},
	{	E_INVALIDARG,						"An invalid parameter was passed to the returning function"	},
	//{	E_INVALIDCALL,						"The method call is invalid For example, a method's parameter may have an invalid value"	},
	{	E_OUTOFMEMORY,						"Direct3D could not allocate sufficient memory to complete the call"	},
	{	S_OK,								"No error occurred"	},
	{	0,									NULL}
};

struct SD_TEXTURE_FORMAT
{
	TX_FORMAT	txFormat;
	D3DFORMAT	d3dFormat;
	bool		isSwizzled;
	const char	* format;
};

SD_TEXTURE_FORMAT textureFormats[] =
{
	{TXF_DXT1, D3DFMT_DXT1, true, "D3DFMT_DXT1"},
	{TXF_DXT3, D3DFMT_DXT3, true, "D3DFMT_DXT3"},
	{TXF_DXT5, D3DFMT_DXT5, true, "D3DFMT_DXT5"},
	{TXF_A8R8G8B8, D3DFMT_A8R8G8B8, false, "D3DFMT_A8R8G8B8"},
	{TXF_X8R8G8B8, D3DFMT_X8R8G8B8, false, "D3DFMT_X8R8G8B8"},
	{TXF_R5G6B5, D3DFMT_R5G6B5, false, "D3DFMT_R5G6B5"},
	{TXF_A4R4G4B4, D3DFMT_A4R4G4B4, false, "D3DFMT_A4R4G4B4"},
	{TXF_A1R5G5B5, D3DFMT_A1R5G5B5, false, "D3DFMT_A1R5G5B5"},
};

dword dwSoundBuffersCount = 0;
dword dwSoundBytes = 0;
dword dwSoundBytesCached = 0;

//################################################################################
__forceinline bool DX8RENDER::ErrorHandler(char * message, HRESULT hres)
{
	if (hres == D3D_OK)	return false;

	for(long e=0; ; e++)
	{
		if (D3D_ERRORS[e].descrition == NULL)	break;
		if (D3D_ERRORS[e].err == hres)
		{
			api->Trace(message);
			api->Trace(D3D_ERRORS[e].descrition);
			return true;
		}
	}
	api->Trace(message);
	api->Trace("Undefined error");
	return true;
}

const dword DX8RENDER::rectsVBuffer_SizeInRects = 512;

//################################################################################
DX8RENDER::DX8RENDER() :
	stRenderTarget(_FL_),
	aCaptureBuffers(_FL_)
{
	rectsVBuffer = null;

	bPostProcessEnabled = true;
	bPostProcessError = false;
	iBlurPasses = 4;
	GlowIntensity = 200;
	fBlurSize = 0.8f;

	pPostProcessTexture = NULL;
	pSmallPostProcessTexture = NULL;
	pSmallPostProcessTexture2 = NULL;


	pPostProcessSurface = NULL;
	pSmallPostProcessSurface = NULL;
	pSmallPostProcessSurface2 = NULL;


	pOriginalScreenSurface = NULL;
	pOriginalDepthSurface = NULL;

	pRS = this;
	d3d = NULL;
	d3d8 = NULL;
	aniVBuffer = null;
	numAniVerteces = 0;
	pVTL = NULL;
	nFontQuantity = 0;
    idFontCurrent = 0;
    fontIniFileName = NULL;
	pTechnique = null;

	bLoadTextureEnabled = true;

	bSeaEffect = false;
	fSeaEffectSize = 0.0f;
	fSeaEffectSpeed = 0.0f;
	dwBackColor = 0x0;

	bTrace = true;
	iSetupPath = 0;
	ZERO(TexPaths);

	bDropVideoConveyor = false;
	pDropConveyorVBuffer = null;

	aspectRatio = -1.0f;
	ZeroMemory(FontList,sizeof(FontList));
	ZeroMemory(Textures,sizeof(Textures));

	bMakeShoot = false;
	bShowFps = false;
	bShowExInfo = false;
	bInsideScene = false;

	m_fHeightDeformator = 1.f;

	progressImage = null;
	progressImageSize = 0;
	progressTexture = -1;
	progressTipsImage = null;
	progressTipsImageSize = 0;
	progressTipsTexture = -1;
	loadFrame = 0;
	backTexture = -1;
	progressSafeCounter = 0;
	isInPViewProcess = false;
	progressUpdateTime = 0;
	progressFramesPosX = 0.85f;
	progressFramesPosY = 0.8f;
	progressFramesWidth = 64;
	progressFramesHeight = 64;
	progressFramesCountX = 8;
	progressFramesCountY = 8;

	bVideoCapture = false;
	bPreparedCapture = false;
	iCaptureFrameIndex = 0;

	vViewRelationPos = CVECTOR(0.f,0.f,0.f);
	vWordRelationPos = -vViewRelationPos;
	bUseLargeBackBuffer = false;
}

static bool texLog = false;
static float fSin = 0.0f;

bool  DX8RENDER::Init()
{
	GUARD(DX8RENDER::Init)
	char str[256];
	for (long i=0; i<MAX_STEXTURES; i++) Textures[i].ref = NULL;

	d3d = NULL;
	d3d8 = NULL;

	INIFILE * ini;
	ini = api->fio->OpenIniFile(api->EngineIniFileName());
	if (ini)
	{
		bPostProcessEnabled = ini->GetLong(0,"PostProcess", 0) == 1;
		bShowFps = ini->GetLong(0, "show_fps", 0) == 1;
		bShowExInfo = ini->GetLong(0, "show_exinfo", 0) == 1;
		bSafeRendering = ini->GetLong(0, "safe_render", 0) == 0;
		bDropVideoConveyor = ini->GetLong(0, "DropVideoConveyor", 0) != 0;
		texLog = ini->GetLong(0, "texture_log", 0) == 1;
		bUseLargeBackBuffer = ini->GetLong(0, "UseLargeBackBuffer", 0) != 0;

#ifndef _XBOX
		bWindow = ini->GetLong(0,"full_screen", 1) == 0;
#else
		bWindow = false;
#endif

		nTextureDegradation = ini->GetLong(0,"texture_degradation",0);

		screen_size.x = ini->GetLong(0,"screen_x",1024);
		screen_size.y = ini->GetLong(0,"screen_y",768);
		fNearClipPlane = ini->GetFloat(0, "NearClipPlane", 0.1f);
		fFarClipPlane = ini->GetFloat(0, "FarClipPlane", 4000.0f);
		bBackBufferCanLock = ini->GetLong(0, "lockable_back_buffer", 0) != 0;
		ini->ReadString(0, "screen_bpp", str, sizeof(str), "D3DFMT_R5G6B5");
		screen_bpp = D3DFMT_R5G6B5;
		stencil_format = D3DFMT_D16;
		if (stricmp(str, "D3DFMT_A8R8G8B8") == 0)	{ screen_bpp = D3DFMT_A8R8G8B8; stencil_format = D3DFMT_D24S8; }
		if (stricmp(str, "D3DFMT_X8R8G8B8") == 0)	{ screen_bpp = D3DFMT_X8R8G8B8; stencil_format = D3DFMT_D24S8; }
		if (stricmp(str, "D3DFMT_R5G6B5") == 0)		{ screen_bpp = D3DFMT_R5G6B5;	stencil_format = D3DFMT_D16; }

		//stencil_format = D3DFMT_D24S8;
	    if(!InitDevice(bWindow, api->GetAppHWND(), screen_size.x, screen_size.y)) return false;

	    pTechnique = NEW CTechnique(this);
	    pTechnique->DecodeFiles();

        // �������� ��������� ��� ���� ��� �������
        if (!ini->ReadString(0, "startFontIniFile", str, sizeof(str)-1, ""))
		{
            api->Trace("Not finded 'startFontIniFile' parameter into ENGINE.INI file");
			sprintf(str, "resource\\ini\\fonts.ini");
		}
        if ((fontIniFileName = NEW char[strlen(str) + 1]) == null)
            _THROW("allocate memory error");
        strcpy(fontIniFileName,str);
		// get start font quantity
		if (!ini->ReadString(0, "font", str, sizeof(str)-1, ""))
		{
            api->Trace("Start font not defined");
			sprintf(str, "normal");
		}
        if( LoadFont(str) == -1L )
			api->Trace("can not init start font: %s", str);
        idFontCurrent=0L;

		//Progress image parameters
		progressFramesPosX = ini->GetFloat("ProgressImage", "RelativePosX", 0.85f);
		progressFramesPosY = ini->GetFloat("ProgressImage", "RelativePosY", 0.8f);
		progressFramesWidth = ini->GetFloat("ProgressImage", "RelativeWidth", 0.0625f);
		if(progressFramesWidth < 0.0f) progressFramesWidth = 0.0f;
		if(progressFramesWidth > 10.0f) progressFramesWidth = 10.0f;
		progressFramesHeight = ini->GetFloat("ProgressImage", "RelativeHeight", 0.0625f);
		if(progressFramesHeight < 0.0f) progressFramesHeight = 0.0f;
		if(progressFramesHeight > 10.0f) progressFramesHeight = 10.0f;
		progressFramesCountX = (long)ini->GetFloat("ProgressImage", "HorisontalFramesCount", 8);
		if(progressFramesCountX < 1) progressFramesCountX = 1;
		if(progressFramesCountX > 64) progressFramesCountX = 64;
		progressFramesCountY = (long)ini->GetFloat("ProgressImage", "VerticalFramesCount", 8);
		if(progressFramesCountY < 1) progressFramesCountY = 1;
		if(progressFramesCountY > 64) progressFramesCountY = 64;

		// videocapture section
		fFixedFPS = ini->GetFloat("VideoCapture", "FPS", 25); if (fFixedFPS == 0.0f) fFixedFPS = 25.0f;
		long iCapBuffers = ini->GetLong("VideoCapture", "Buffers", 0);
		for (long i=0; i<iCapBuffers; i++)
			aCaptureBuffers.Add(NEW char[sizeof(dword) * screen_size.x * screen_size.y]);

		delete ini;

#ifndef _XBOX
		if(bWindow)
		{
			long xs,ys;
			xs = GetSystemMetrics(SM_CXSCREEN);
			ys = GetSystemMetrics(SM_CYSCREEN);
			if( bUseLargeBackBuffer )
				MoveWindow(api->GetAppHWND(),0, 0, xs,ys,true);
			else
				MoveWindow(api->GetAppHWND(),(xs-screen_size.x)/2, (ys-screen_size.y)/2, screen_size.x,screen_size.y,true);
		}
#endif

		CreateSphere();
		VDATA * pScriptRender = (VDATA *)api->GetScriptVariable("Render");
		ATTRIBUTES * pARender = pScriptRender->GetAClass();

		pARender->SetAttributeUseDword("full_screen", !bWindow);
		pARender->SetAttributeUseDword("screen_x", screen_size.x);
		pARender->SetAttributeUseDword("screen_y", screen_size.y);
	}

	pDropConveyorVBuffer = null;
	rectsVBuffer = null;
	d3d8->CreateVertexBuffer(2 * sizeof(CVECTOR), D3DUSAGE_WRITEONLY, D3DFVF_XYZ, D3DPOOL_MANAGED, &pDropConveyorVBuffer, NULL);
	d3d8->CreateVertexBuffer(rectsVBuffer_SizeInRects*6*sizeof(RECT_VERTEX), D3DUSAGE_WRITEONLY | D3DUSAGE_DYNAMIC, RS_RECT_VERTEX_FORMAT, D3DPOOL_DEFAULT, &rectsVBuffer, NULL);//D3DPOOL_MANAGED
	if(!pDropConveyorVBuffer || !rectsVBuffer)
	{
		return false;
	}

	dwCaptureBuffersReady = 0;

	word * pI = &qi[0];
	// setup ibuffer
	for (long y=0; y<31; y++)
	{
		for (long x=0; x<31; x++)
		{
			*pI++ = word( (y + 0) * 32 + x + 1 );
			*pI++ = word( (y + 1) * 32 + x );
			*pI++ = word( (y + 0) * 32 + x );

			*pI++ = word( (y + 0) * 32 + x + 1 );
			*pI++ = word( (y + 1) * 32 + x + 1 );
			*pI++ = word( (y + 1) * 32 + x );
		}
	}

	UNGUARD
	return true;
}

//################################################################################
DX8RENDER::~DX8RENDER()
{
	d3d8->SetRenderTarget( NULL, NULL );

	S_RELEASE(pSmallPostProcessSurface2, 0);
	S_RELEASE(pPostProcessSurface, 1);
	S_RELEASE(pSmallPostProcessSurface, 2);

	S_RELEASE(pOriginalScreenSurface, 3);
	S_RELEASE(pOriginalDepthSurface, 4);

	S_RELEASE(pSmallPostProcessTexture2, 5);
	S_RELEASE(pPostProcessTexture, 6);
	S_RELEASE(pSmallPostProcessTexture, 7);

	S_RELEASE(rectsVBuffer, 8);


	if(progressImage) delete progressImage; progressImage = null;
	for(int i=0; i<nFontQuantity; i++)
	{
        if(FontList[i].font!=NULL)
		    delete FontList[i].font;
		if(FontList[i].name!=NULL)
			delete FontList[i].name;
	}
    nFontQuantity = 0;
    if(fontIniFileName!=NULL) delete fontIniFileName;

	DELETE(DX8sphereVertex);
	DELETE(pTechnique);
	ReleaseDevice();
	api->EngineDisplay(true);

	if (bPreparedCapture)
	{
		DELETE_ARRAY(lpbi);
		ReleaseDC(api->GetAppHWND(), hDesktopDC);
		DeleteDC(hCaptureDC);
		DeleteObject(hCaptureBitmap);
	}

	aCaptureBuffers.DelAllWithPointers();
}

//################################################################################
#ifdef _XBOX
bool getBestVideoMode(IDirect3D9 *d3, D3DPRESENT_PARAMETERS &d3dpp, bool bpp32)
{
	memset( &d3dpp, 0, sizeof(d3dpp) );

	bool _1080i = false;
	bool _720p = false;
	bool PAL0 = false;
	bool PAL1 = false;
	bool NTSC0 = false;
	bool NTSC1 = false;

	long n_modes = d3->GetAdapterModeCount(D3DADAPTER_DEFAULT);
	for(long nm=0; nm<n_modes; nm++)
	{
		D3DDISPLAYMODE am;
		d3->EnumAdapterModes(D3DADAPTER_DEFAULT, nm, &am);

		if((bpp32 && am.Format==D3DFMT_LIN_X8R8G8B8) || (!bpp32 && am.Format==D3DFMT_LIN_R5G6B5))
		{
			//if(am.Width==1920 && am.Height==1080)	_1080i = true;
			//if(am.Width==1280 && am.Height==720)	_720p = true;
			if(am.Width==640 && am.Height==576)	PAL0 = true;
			if(am.Width==720 && am.Height==576)	PAL1 = true;
			if(am.Width==640 && am.Height==480)	NTSC0 = true;
			if(am.Width==720 && am.Height==480)	NTSC1 = true;
		}
	}

	if(PAL1)
	{
		d3dpp.BackBufferWidth = 720;
		d3dpp.BackBufferHeight = 576;
	}
	else
		if(PAL0)
		{
			d3dpp.BackBufferWidth = 640;
			d3dpp.BackBufferHeight = 576;
		}
		else
			if(NTSC1)
			{
				d3dpp.BackBufferWidth = 720;
				d3dpp.BackBufferHeight = 480;
			}
			else
				if(NTSC0)
				{
					d3dpp.BackBufferWidth = 640;
					d3dpp.BackBufferHeight = 480;
				}
				else return false;
					/*if(_720p)
					{
						d3dpp.BackBufferWidth = 1280;
						d3dpp.BackBufferHeight = 720;
					}
					else
						if(_1080i)
						{
							d3dpp.BackBufferWidth = 1920;
							d3dpp.BackBufferHeight = 1080;
						}
						else
						{
							return false;
						}*/

	if(bpp32)
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	}
	else
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
	}
	d3dpp.BackBufferCount        = 1;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;

	//d3dpp.BackBufferWidth = 640;
	//d3dpp.BackBufferHeight = 480;

	return true;
}
#endif

bool DX8RENDER::InitDevice(bool windowed, HWND _hwnd, long width, long height)
{
	GUARD(DX8RENDER::InitDevice)

	aniVBuffer = null;
	numAniVerteces = 0;

	screen_size.x = width;
	screen_size.y = height;
	bWindow = windowed;

	hwnd = _hwnd;
	api->Trace("Initializing DirectX 9");
	d3d = Direct3DCreate9( D3D_SDK_VERSION );
	if(d3d==NULL)
	{
		//MessageBox(hwnd, "Direct3DCreate9 error", "InitDevice::Direct3DCreate9", MB_OK);
		api->Trace("Direct3DCreate9 error : InitDevice::Direct3DCreate9");
		return false;
	}

	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = screen_bpp;
	d3dpp.BackBufferCount = 1;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = stencil_format;


	if(windowed)
	{
		D3DDISPLAYMODE d3ddm;
		if(FAILED(d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&d3ddm))) THROW;
		d3dpp.BackBufferFormat = d3ddm.Format;
		if(d3dpp.BackBufferFormat == D3DFMT_R5G6B5) d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		else d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		stencil_format = d3dpp.AutoDepthStencilFormat;
		if( !bUseLargeBackBuffer )
			if(d3ddm.Width < (DWORD)width || d3ddm.Height < (DWORD)height)
			{
				d3dpp.BackBufferWidth = d3ddm.Width;
				d3dpp.BackBufferHeight = d3ddm.Height;
				width = d3ddm.Width;
				height = d3ddm.Height;
			}
	}


	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	//d3dpp.MultiSampleType = D3DMULTISAMPLEMODE_4X ;
	/*dword n;
	for(n=D3DMULTISAMPLE_16_SAMPLES;n>=D3DMULTISAMPLE_3_SAMPLES;n--)
	{
		if(SUCCEEDED(d3d->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
		d3dpp.BackBufferFormat,false,(D3DMULTISAMPLE_TYPE)n)))
		{
			d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)n;
			break;
		}
	}//*/



	if (bBackBufferCanLock) d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
	else d3dpp.Flags = 0;
	//d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	//if(windowed) d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;//FLIP;
	//else d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;

	if(!windowed)
	{
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	}


#ifndef _XBOX
	if(d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_MIXED_VERTEXPROCESSING, &d3dpp, &d3d8)!=D3D_OK)
	{
		//if(ErrorHandler("InitDevice::CreateDevice", E_FAIL)==true)	return false;
		if(ErrorHandler("InitDevice::CreateDevice",
			d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3d8))==true)	return false;
	}
#else
	if(getBestVideoMode(d3d, d3dpp, !(d3dpp.BackBufferFormat == D3DFMT_R5G6B5) ))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3d->CreateDevice( 0, D3DDEVTYPE_HAL, NULL, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &d3d8 );
		width = d3dpp.BackBufferWidth;
		height = d3dpp.BackBufferHeight;
	}
	else
	{
		api->Trace("no standard supported video medes found [ 640*480, 720*480, 640*576, 720*576, 1280*720. 1920*1080");
	}
#endif



//������� ������target� ��� POST PROCESS ��������...
	d3d8->GetRenderTarget(0, &pOriginalScreenSurface);
	d3d8->GetDepthStencilSurface(&pOriginalDepthSurface);

	fSmallWidth = 128;
	fSmallHeight = 128;

	//if (bPostProcessEnabled)
	{
		d3d8->CreateTexture(width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pPostProcessTexture, NULL);
		d3d8->CreateTexture((int)fSmallWidth, (int)fSmallHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pSmallPostProcessTexture, NULL);
		d3d8->CreateTexture((int)(fSmallWidth*2.0f), (int)(fSmallHeight*2.0f), 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &pSmallPostProcessTexture2, NULL);
	}

	if (!pPostProcessTexture || !pSmallPostProcessTexture || !pSmallPostProcessTexture2)
	{
		bPostProcessEnabled = false;
		bPostProcessError = true;
	}


	if (!bPostProcessError)
	{
		pPostProcessTexture->GetSurfaceLevel(0, &pPostProcessSurface);
		pSmallPostProcessTexture2->GetSurfaceLevel(0, &pSmallPostProcessSurface2);
		pSmallPostProcessTexture->GetSurfaceLevel(0, &pSmallPostProcessSurface);

		if (!pPostProcessSurface || !pSmallPostProcessSurface2 || !pSmallPostProcessSurface)
		{
			bPostProcessEnabled = false;
			bPostProcessError = true;
		}
	}



	if (!bPostProcessError)
	{
		ClearPostProcessSurface(pPostProcessSurface);
		ClearPostProcessSurface(pSmallPostProcessSurface);
		ClearPostProcessSurface(pSmallPostProcessSurface2);
	}

	ClearPostProcessSurface(pOriginalScreenSurface);

	OriginalViewPort.X = 0;
	OriginalViewPort.Y = 0;
	OriginalViewPort.Width = width;
	OriginalViewPort.Height = height;
	OriginalViewPort.MinZ = 0.0f;
	OriginalViewPort.MaxZ = 1.0f;


	//if(ErrorHandler("InitDevice::SetViewport", d3d8->SetViewport(&vprt))==true)	return false;

	//������ �� ��� ������ ������...
	//FIX
	//SetPostProcessTextureAsRenderTarget();
	//������ �� ��� ������ ������...


	for(long b=0; b<MAX_BUFFERS; b++)
	{
		VertexBuffers[b].buff=0;
		IndexBuffers[b].buff=0;
	}

	long num_stages;
#ifndef _XBOX
	num_stages = 8;
#else
	num_stages = 4;
#endif
	for(long s=0; s<num_stages; s++)
	{
		//texture operation
		d3d8->SetTextureStageState(s, D3DTSS_COLORARG1, D3DTA_CURRENT);
		d3d8->SetTextureStageState(s, D3DTSS_COLORARG2, D3DTA_TEXTURE);
		d3d8->SetTextureStageState(s, D3DTSS_COLOROP, D3DTOP_DISABLE);

		//texture coord
		d3d8->SetTextureStageState(s, D3DTSS_TEXCOORDINDEX, s);

		//texture filtering
		d3d8->SetSamplerState(s, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		d3d8->SetSamplerState(s, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		d3d8->SetSamplerState(s, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	}

	//set base texture and diffuse+specular lighting
	d3d8->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	d3d8->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);

	//texture op for lightmaps
	//d3d8->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);


	SetCamera(&CVECTOR(0.0f, 0.0f, 0.0f), &CVECTOR(0.0f, 0.0f, 0.0f), 1.0f);

	D3DLIGHT9 l;
	ZERO(l);
	l.Type = D3DLIGHT_POINT;
	l.Range = 100.0f;
	l.Attenuation0 = 1.0f;

	for (int i=0; i<8; i++)
	{
		d3d8->SetLight(i, &l);
		d3d8->LightEnable(i,false);
	}

	func();
	api->EngineDisplay(false);

	screen_size.x = width;
	screen_size.y = height;

	m_fHeightDeformator = (float)(height*4)/(float)(width*3);
/*#ifdef _XBOX
	DWORD videoFlags = XGetVideoFlags();
	if( videoFlags & XC_VIDEO_FLAGS_WIDESCREEN ) {
		m_fHeightDeformator = (float)(height*16)/(float)(width*9);
	}
#endif*/

	d3d8->GetGammaRamp(0, &DefaultRamp);
#ifdef _XBOX
	for (i=0; i<256; i++)
	{
		DefaultRamp.red[i] *= 256;
		DefaultRamp.green[i] *= 256;
		DefaultRamp.blue[i] *= 256;
	}
#endif

	UNGUARD
	return true;
}

//################################################################################
bool DX8RENDER::ReleaseDevice()
{
	if (aniVBuffer) aniVBuffer->Release();
	aniVBuffer = null;
	numAniVerteces = 0;
	for(long b=0; b<MAX_BUFFERS; b++)
	{
		if (VertexBuffers[b].buff)	VertexBuffers[b].buff->Release(); VertexBuffers[b].buff = 0;
		if (IndexBuffers[b].buff)	IndexBuffers[b].buff->Release(); IndexBuffers[b].buff = 0;
	}

	bool res = true;
	for(long t=0; t<MAX_STEXTURES; t++)
		if(Textures[t].ref && Textures[t].loaded && Textures[t].d3dtex)
		{
			if (ErrorHandler("Release texture", Textures[t].d3dtex->Release())==false)	res = false;
			Textures[t].ref = NULL;
			if( Textures[t].name!=NULL )	delete Textures[t].name;
		}

	if (d3d8) d3d8->SetGammaRamp(0, D3DSGR_NO_CALIBRATION, &DefaultRamp);

	if (d3d8!=NULL && ErrorHandler("Release d3d8", d3d8->Release())==false)	res = false;
	d3d8 = NULL;
	if (d3d!=NULL && ErrorHandler("Release d3d", d3d->Release())==false)	res = false;
	d3d = NULL;
	return res;
}


//################################################################################
bool DX8RENDER::DX8Clear(long type)
{
	if(ErrorHandler("Clear", d3d8->Clear(0L, NULL, type, dwBackColor, 1.0f, 0L))==true)	return false;
	//if(ErrorHandler("Clear", d3d8->Clear(0L, NULL, type, 0x0, 1.0f, 0L))==true)	return false;
	return true;
}

//################################################################################
bool DX8RENDER::DX8BeginScene()
{
	dwNumDrawPrimitive = 0;
	dwNumLV = 0;
	dwNumLI = 0;
	if(ErrorHandler("BeginScene", d3d8->BeginScene())==true)	return false;
	return true;
}

void DX8RENDER::CreateRenderQuad (float fWidth, float fHeight, float fSrcWidth, float fSrcHeight, float fMulU, float fMulV)
{
	float StartX = -0.5f;
	float StartY = -0.5f;
	fWidth -= 0.5f;
	fHeight -= 0.5f;
	PostProcessQuad[0].vPos = Vector4 (  StartX, fHeight, 0.0f, 1.0f );
	PostProcessQuad[1].vPos = Vector4 (  StartX,  StartY, 0.0f, 1.0f );
	PostProcessQuad[2].vPos = Vector4 ( fWidth, fHeight, 0.0f, 1.0f );
	PostProcessQuad[3].vPos = Vector4 ( fWidth,  StartY, 0.0f, 1.0f );

	float fTexelU = 1.0f / fSrcWidth;
	float fTexelV = 1.0f / fSrcHeight;

	float fNearV = fTexelV * 0.5f;
	float fFarV = 1.0f - (fTexelV * 0.5f);

	float fNearU = fTexelU * 0.5f;
	float fFarU = 1.0f - (fTexelU * 0.5f);

	PostProcessQuad[0].v0 = fFarV;   PostProcessQuad[0].u0 = fNearU;
	PostProcessQuad[1].v0 = fNearV;  PostProcessQuad[1].u0 = fNearU;
	PostProcessQuad[2].v0 = fFarV;   PostProcessQuad[2].u0 = fFarU;
	PostProcessQuad[3].v0 = fNearV;  PostProcessQuad[3].u0 = fFarU;

	for (int i = 0; i < 4; i++)
	{
		float u = PostProcessQuad[i].u0;
		float v = PostProcessQuad[i].v0;

		u = u * fMulU;
		v = v * fMulV;

		PostProcessQuad[i].u0 = u - (fTexelU*fBlurSize);
		PostProcessQuad[i].v0 = v - (fTexelV*fBlurSize);

		PostProcessQuad[i].u1 = u + (fTexelU*fBlurSize);
		PostProcessQuad[i].v1 = v - (fTexelV*fBlurSize);

		PostProcessQuad[i].u2 = u - (fTexelU*fBlurSize);
		PostProcessQuad[i].v2 = v + (fTexelV*fBlurSize);

		PostProcessQuad[i].u3 = u + (fTexelU*fBlurSize);
		PostProcessQuad[i].v3 = v + (fTexelV*fBlurSize);

	}

}

void DX8RENDER::BlurGlowTexture ()
{
	//�������� ��� � ��������� ��������...
	CreateRenderQuad(fSmallWidth*2.0f, fSmallHeight*2.0f, 1024.0f, 1024.0f);
	SetTexture (0, pPostProcessTexture);
	SetTexture (1, pPostProcessTexture);
	SetTexture (2, pPostProcessTexture);
	SetTexture (3, pPostProcessTexture);
	d3d8->SetRenderTarget( 0, pSmallPostProcessSurface2 );
	DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcessBlur");


	//��������������� �������� iBlurPasses ��� :))))
	for (int i = 0; i < iBlurPasses; i++)
	{
		CreateRenderQuad(fSmallWidth, fSmallHeight, fSmallWidth*2.0f, fSmallHeight*2.0f);

		SetTexture (0, pSmallPostProcessTexture2);
		SetTexture (1, pSmallPostProcessTexture2);
		SetTexture (2, pSmallPostProcessTexture2);
		SetTexture (3, pSmallPostProcessTexture2);
		d3d8->SetRenderTarget( 0, pSmallPostProcessSurface );
		DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcessBlur");

		CreateRenderQuad(fSmallWidth*2.0f, fSmallHeight*2.0f, fSmallWidth, fSmallHeight);

		SetTexture (0, pSmallPostProcessTexture);
		SetTexture (1, pSmallPostProcessTexture);
		SetTexture (2, pSmallPostProcessTexture);
		SetTexture (3, pSmallPostProcessTexture);
		d3d8->SetRenderTarget( 0, pSmallPostProcessSurface2 );
		DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcessBlur");
	}


	CreateRenderQuad(fSmallWidth, fSmallHeight, fSmallWidth*2.0f, fSmallHeight*2.0f);
	SetTexture (0, pSmallPostProcessTexture2);
	SetTexture (1, pSmallPostProcessTexture2);
	SetTexture (2, pSmallPostProcessTexture2);
	SetTexture (3, pSmallPostProcessTexture2);
	d3d8->SetRenderTarget( 0, pSmallPostProcessSurface );
	DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcessBlur");
}

void DX8RENDER::CopyGlowToScreen ()
{
	FLOAT sx = (FLOAT)screen_size.x;
	FLOAT sy = (FLOAT)screen_size.y;
	//�������� �� �����
	PostProcessQuad[0].vPos = Vector4 (  0, sy, 0.0f, 1.0f );
	PostProcessQuad[1].vPos = Vector4 (  0,  0, 0.0f, 1.0f );
	PostProcessQuad[2].vPos = Vector4 ( sx, sy, 0.0f, 1.0f );
	PostProcessQuad[3].vPos = Vector4 ( sx,  0, 0.0f, 1.0f );

	PostProcessQuad[0].v0 = 1.0f;  PostProcessQuad[0].u0 = 0.0f;
	PostProcessQuad[1].v0 = 0.0f;  PostProcessQuad[1].u0 = 0.0f;
	PostProcessQuad[2].v0 = 1.0f;  PostProcessQuad[2].u0 = 1.0f;
	PostProcessQuad[3].v0 = 0.0f;  PostProcessQuad[3].u0 = 1.0f;

	d3d8->SetRenderTarget( 0, pOriginalScreenSurface );
	d3d8->SetDepthStencilSurface(pOriginalDepthSurface);

	if (GlowIntensity < 0) GlowIntensity = 0;
	if (GlowIntensity > 255) GlowIntensity = 255;
	BYTE bGLOW = (BYTE)GlowIntensity;
	DWORD dwTFactor = (bGLOW << 24) | (bGLOW << 16) | (bGLOW << 8) | bGLOW;

	//GLOW ����� ������....
	SetRenderState(D3DRS_TEXTUREFACTOR, dwTFactor);

	SetTexture (0, pSmallPostProcessTexture);
	SetTexture (1, pSmallPostProcessTexture);
	SetTexture (2, pSmallPostProcessTexture);
	SetTexture (3, pSmallPostProcessTexture);

	DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcessGlow");
}

void DX8RENDER::CopyPostProcessToScreen()
{
	FLOAT sx = (FLOAT)screen_size.x;
	FLOAT sy = (FLOAT)screen_size.y;
	PostProcessQuad[0].vPos = Vector4 (  0, sy, 0.0f, 1.0f );
	PostProcessQuad[1].vPos = Vector4 (  0,  0, 0.0f, 1.0f );
	PostProcessQuad[2].vPos = Vector4 ( sx, sy, 0.0f, 1.0f );
	PostProcessQuad[3].vPos = Vector4 ( sx,  0, 0.0f, 1.0f );

	PostProcessQuad[0].v0 = 1.0f;  PostProcessQuad[0].u0 = 0.0f;
	PostProcessQuad[1].v0 = 0.0f;  PostProcessQuad[1].u0 = 0.0f;
	PostProcessQuad[2].v0 = 1.0f;  PostProcessQuad[2].u0 = 1.0f;
	PostProcessQuad[3].v0 = 0.0f;  PostProcessQuad[3].u0 = 1.0f;

	d3d8->SetRenderTarget(0, pOriginalScreenSurface);
	d3d8->SetDepthStencilSurface(pOriginalDepthSurface);

	//������������ ����� ������....
	SetTexture (0, pPostProcessTexture);
	SetTexture (1, pPostProcessTexture);
	SetTexture (2, pPostProcessTexture);
	SetTexture (3, pPostProcessTexture);

	if (bSeaEffect)
	{
		d3d8->SetFVF(POST_PROCESS_FVF);
		DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 32 * 32, 31 * 31 * 2, qi, D3DFMT_INDEX16, qv, sizeof(QuadVertex), "PostProcess");
	}
	else
	{
		DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, POST_PROCESS_FVF, 2, PostProcessQuad, sizeof(QuadVertex), "PostProcess");
	}
}

void DX8RENDER::ClearPostProcessSurface (IDirect3DSurface9* pSurf)
{
	HRESULT hr = d3d8->SetRenderTarget( 0, pSurf );
	hr = d3d8->BeginScene();
	hr = d3d8->Clear(0, NULL, D3DCLEAR_TARGET, 0x0, 0.0f, 0x0);
	hr = d3d8->EndScene();
}

void DX8RENDER::SetScreenAsRenderTarget ()
{
	d3d8->SetRenderTarget(0, pOriginalScreenSurface);
	d3d8->SetDepthStencilSurface(pOriginalDepthSurface);
	SetViewport(&OriginalViewPort);
}

void DX8RENDER::SetPostProcessTextureAsRenderTarget ()
{
	d3d8->SetRenderTarget(0, pPostProcessSurface);
	d3d8->SetDepthStencilSurface(pOriginalDepthSurface);
	SetViewport(&OriginalViewPort);
}


void DX8RENDER::MakePostProcess()
{
	if (bPostProcessError) return;
	if (!bSeaEffect && !bPostProcessEnabled) return;
	if (!bNeedCopyToScreen) return;

	IDirect3DStateBlock9* pStateBlock = nullptr;
	d3d8->CreateStateBlock(D3DSBT_ALL, &pStateBlock);
	pStateBlock->Capture();

	bNeedCopyToScreen = false;

	CopyPostProcessToScreen();
	BlurGlowTexture();
	CopyGlowToScreen();

	pStateBlock->Apply();
	pStateBlock->Release();

	SetScreenAsRenderTarget();
/*
	Print (0, 0, "Blur = %3.2f",	fBlurSize);
	Print (0, 16, "Intensity = %d",	GlowIntensity);
	Print (0, 32, "BlurPasses = %d",	iBlurPasses);
*/

}

//################################################################################
bool DX8RENDER::DX8EndScene()
{
	MEMORYSTATUS ms;
	GlobalMemoryStatus(&ms);

	dword	dwTotalTexSize = 0;
	dword	dwTotalTexNum = 0, dwTotalVB = 0, dwTotalIB = 0, dwTotalVBSize = 0, dwTotalIBSize = 0;

	if(bShowFps)
	{
		long t;
		for(t=0; t<MAX_STEXTURES; t++) if(Textures[t].ref>0) { dwTotalTexSize += Textures[t].dwSize; dwTotalTexNum++; }

		for(t=0; t<MAX_BUFFERS; t++)
		{
			if (VertexBuffers[t].buff) { dwTotalVBSize += VertexBuffers[t].size; dwTotalVB++; }
			if (IndexBuffers[t].buff) { dwTotalIBSize += IndexBuffers[t].ntrgs; dwTotalIB++; }
		}
	}

#ifndef _XBOX
	if(api->Controls->GetDebugAsyncKeyState(VK_MULTIPLY) < 0)
#endif
	{
		//MEMORYSTATUS ms;
	//GlobalMemoryStatus(&ms);
	//trace("Allocate(long size) %d   - FREE( %d )",size,ms.dwAvailPhys);
	//Print(screen_size.x - 100,150,"%d",api->fio->_GetDiskFreeSpaceEx();
		bShowFps = 0;
		CMatrix mView; GetTransform(D3DTS_VIEW, mView); mView.Transposition();
		Print(0, 0, "Cam: %.3f, %.3f, %.3f", mView.Pos().x, mView.Pos().y, mView.Pos().z);
		Print(screen_size.x - 100,50,"%d fps",api->EngineFps());

	if(bShowFps)
	{
		MSTATE sms;
		api->GetMemoryState(&sms);
		Print(screen_size.x - 100,50,"%d",api->EngineFps());

		//Print(80,50,"%d Kb",(ms.dwTotalPhys - ms.dwAvailPhys)/(1024));

		Print(80,50,"%.3f Mb",(ms.dwTotalPhys - ms.dwAvailPhys)/(1024.f*1024.f));
		Print(80,80,"%.3f Mb in %d blocks",sms.nMemorySize/(1024.f*1024.f),sms.nBlocksNum);


		if(bShowExInfo)
		{
			Print(80,70,"t: %d, %.3f Mb", dwTotalTexNum, float(dwTotalTexSize) / (1024.0f*1024.0f));
			Print(80,90,"v: %d, %.3f Mb", dwTotalVB, float(dwTotalVBSize) / (1024.0f*1024.0f));
			Print(80,110,"i: %d, %.3f Mb", dwTotalIB, float(dwTotalIBSize) / (1024.0f*1024.0f));
			Print(80,130,"d: %d, lv: %d, li: %d", dwNumDrawPrimitive, dwNumLV, dwNumLI);
			Print(80,150,"s: %d, %.3f, %.3f",dwSoundBuffersCount, dwSoundBytes / 1024.f, dwSoundBytesCached / 1024.f);
		}
	}

#ifdef _XBOX

#endif
	}

	// Try to drop video conveyor
	if (bDropVideoConveyor && pDropConveyorVBuffer)
	{
		CVECTOR * pV;
		pDropConveyorVBuffer->Lock(0, 0, (VOID**)&pV, 0);
		for (long i=0; i<2; i++)
			pV[i] = CVECTOR(1e6f, 1e6f, 1e6f);
		pDropConveyorVBuffer->Unlock();
		d3d8->SetStreamSource(0, pDropConveyorVBuffer, 0, sizeof(CVECTOR));
		d3d8->SetFVF(D3DFVF_XYZ);
		DrawPrimitive(D3DPT_LINELIST, 0, 1);
	}

	if (ErrorHandler("EndScene", d3d8->EndScene()))	return false;

#ifndef _XBOX
	/*if (api->Controls->GetDebugAsyncKeyState(VK_SHIFT) < 0 && api->Controls->GetDebugAsyncKeyState(VK_F6) < 0)
	{
		bVideoCapture ^= 1;
		api->SetDeltaTime((bVideoCapture) ? long(1000.0f / fFixedFPS) : 0);
		if (!bVideoCapture)
			SaveCaptureBuffers();
		Sleep(300);
	}*/

	// boal if (bMakeShoot || api->Controls->GetDebugAsyncKeyState(VK_F8) < 0)  MakeScreenShot();
	if (bMakeShoot || GetAsyncKeyState(VK_F8) < 0)  MakeScreenShot();
#endif

	if (bVideoCapture) MakeCapture();

	HRESULT hRes = d3d8->Present(NULL, NULL, NULL, NULL);

	if (hRes == D3DERR_DEVICELOST)
	{
		bDeviceLost = true;
	}

#ifndef _XBOX
	if(bSafeRendering)
	{
		HDC dc = GetDC(hwnd);
		SetPixel(dc, 0,0,0);
		ReleaseDC(hwnd, dc);
	}
#endif

	return true;
}

//################################################################################
static int totSize = 0;
long DX8RENDER::TextureCreate(const char *fname)
{
	// start add texture path
	if (dword(fname) == -1)
	{
		iSetupPath = 1;
		return -1;
	}

	// continue add texture path process
	if (iSetupPath)
	{
		if (iSetupPath == 1)
		{
			dwSetupNumber = dword(fname);
			iSetupPath = 2;
			return -1;
		}

		TexPaths[dwSetupNumber].str[0] = 0;
		if (fname && fname[0]) strcpy(TexPaths[dwSetupNumber].str, fname);
		iSetupPath = 0;
		return -1;
	}

	// delete relative path "resource\textures\"
	string sTexName = fname;
	sTexName.GetRelativePath("resource\\textures\\");
	sTexName = sTexName - string(".tx");
	fname = sTexName.c_str();
	// delete last extension ".tx"

	if (fname==NULL)
    {
		api->Trace("Can't create texture with null name");
		return -1L;
	}

	if (!bLoadTextureEnabled) return -1;

	for (long i=3; i>=-1; i--)
	{
		char _fname[256];

		if (i>=0 && TexPaths[i].str[0] == 0) continue;
		if (i>=0)
		{
			dword dwLen = strlen(fname);

			long j;
			for (j=dwLen-1; j>=0; j--)
				if (fname[j] == '\\') break;

			_fname[0] = 0;
			strncpy(_fname, fname, j + 1);
			_fname[j + 1] = 0;
			strcat(_fname, TexPaths[i].str);
			strcat(_fname, &fname[j + 1]);
			bTrace = false;
		}
		else
		{
			bTrace = true;
			strcpy(_fname, fname);
		}

		if (strlen(_fname) > strlen(".tx"))
		{
			if (stricmp(&_fname[strlen(_fname) - 3], ".tx") == 0)
				_fname[strlen(_fname) - 3] = 0;
		}

		strupr(_fname);

		unsigned long hf = hash_string(_fname);

		long t;
		for(t=0; t<MAX_STEXTURES; t++)
			if(Textures[t].ref!=0)
				if(Textures[t].name)
				if(Textures[t].hash==hf && stricmp(Textures[t].name,_fname)==0)
				{
					Textures[t].ref++;
					return t;
				}

		for(t=0; t<MAX_STEXTURES; t++)
			if(Textures[t].ref==0)	break;

		Textures[t].hash = hf;

		if( (Textures[t].name=NEW char[strlen(_fname)+1]) == NULL )
			_THROW("allocate memory error");
		strcpy(Textures[t].name,_fname);
		Textures[t].isCubeMap = false;
		Textures[t].loaded = false;
		Textures[t].ref++;
		if(TextureLoad(t)) return t;
		Textures[t].ref--;
		DELETE(Textures[t].name);
	}
	return -1;
}
bool DX8RENDER::TextureLoad(long t)
{
	ProgressView();
	//��������� ���� �� ��������
	char fn[_MAX_FNAME];
	Textures[t].dwSize = 0;
	//wsprintf(fn,"resource\\textures\\%s.tx",fname);
	if(Textures[t].name == 0) return false;
	wsprintf(fn,TEXTURESDIR, Textures[t].name);
	for(long s = 0, d = 0; fn[d]; s++)
	{
		if(d > 0 && fn[d - 1] == '\\' && fn[s] == '\\') continue;
		fn[d++] = fn[s];
	}
	//��������� ����
	//api->fio->SetDrive(XBOXDRIVE_CACHE);
	HANDLE file = api->fio->_CreateFile(fn);
	//api->fio->SetDrive();
	if(file == INVALID_HANDLE_VALUE)
	{
		if (bTrace)
		{
			api->Trace("Can't load texture %s",fn);
		}
		delete Textures[t].name;
		Textures[t].name = 0;
		return false;
	}
	//������ ���������
	TX_FILE_HEADER head;
	DWORD readingBytes = 0;
	if(!api->fio->_ReadFile(file, &head, sizeof(head), &readingBytes) || readingBytes != sizeof(head))
	{
		if (bTrace)
		{
			api->Trace("Can't load texture %s",fn);
		}
		delete Textures[t].name;
		Textures[t].name = 0;
		api->fio->_CloseHandle(file);
		return false;
	}
	//����������� ������
	D3DFORMAT d3dFormat = D3DFMT_UNKNOWN;
	long textureFI;
	for(textureFI = 0; textureFI < sizeof(textureFormats)/sizeof(SD_TEXTURE_FORMAT); textureFI++)
		if(textureFormats[textureFI].txFormat == head.format) break;
	if(textureFI == sizeof(textureFormats)/sizeof(SD_TEXTURE_FORMAT) || head.flags & TX_FLAGS_PALLETTE)
	{
		if (bTrace) api->Trace("Invalidate texture format %s, not loading it.", fn);
		delete Textures[t].name;
		Textures[t].name = 0;
		api->fio->_CloseHandle(file);
		return false;
	}
	d3dFormat = textureFormats[textureFI].d3dFormat;
	bool isSwizzled = textureFormats[textureFI].isSwizzled;
	const char * formatTxt = textureFormats[textureFI].format;
	//���������� ����
	dword seekposition = 0;
	for(long nTD = nTextureDegradation; nTD > 0; nTD--)
	{
		if(head.nmips <= 1 || head.width <= 32 || head.height <= 32) break;	// degradation limit
		seekposition += head.mip_size;
		head.nmips--;
		head.width /= 2; head.height /= 2; head.mip_size /= 4;
	}
	//��������� ��������
	if(!(head.flags & TX_FLAGS_CUBEMAP))
	{
		//�������� ������� ��������
		//������� � �����
		if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
		//������ ��������
		IDirect3DTexture9 * tex = null;
		if(ErrorHandler("CreateTexture", d3d8->CreateTexture(head.width, head.height, head.nmips, 0, d3dFormat, D3DPOOL_MANAGED, &tex, NULL))==true || !tex)
		{
			if (bTrace) api->Trace("Texture %s is not created (width: %i, height: %i, num mips: %i, format: %s), not loading it.", fn, head.width, head.height, head.nmips, formatTxt);
			delete Textures[t].name;
			Textures[t].name = 0;
			api->fio->_CloseHandle(file);
			return false;
		}
		//��������� ������
		for(long m = 0; m < head.nmips; m++)
		{
			//��������� ������ ����
			Textures[t].dwSize += head.mip_size;
			//�������� ����������� ����
			bool isError = false;
			IDirect3DSurface9 * surface = null;
			if(ErrorHandler("tex->GetSurfaceLevel", tex->GetSurfaceLevel(m, &surface))==true || !surface)
			{
				isError = true;
			}else{
				//���������� ���
				isError = !LoadTextureSurface(file, surface, head.mip_size, head.width, head.height, isSwizzled);
			}
			//����������� �����������
			if(surface) surface->Release();
			//���� ���� ������, �� ��������� ��������
			if(isError)
			{
				if (bTrace) api->Trace("Can't loading mip %i, texture %s is not created (width: %i, height: %i, num mips: %i, format: %s), not loading it.", m, fn, head.width, head.height, head.nmips, formatTxt);
				delete Textures[t].name;
				Textures[t].name = 0;
				api->fio->_CloseHandle(file);
				tex->Release();
				return false;
			}
			//������������� ������� ��� ���������� ����
			head.width /= 2;
			head.height /= 2;
			head.mip_size /= 4;
		}
		Textures[t].d3dtex = tex;
		Textures[t].isCubeMap = false;
	}else{
		//�������� cubemap
		if(head.width != head.height)
		{
			if (bTrace) api->Trace("Cube map texture can't has not squared sides %s, not loading it.", fn);
			delete Textures[t].name;
			Textures[t].name = 0;
			api->fio->_CloseHandle(file);
			return false;
		}
		//���������� �����
		D3DCAPS9 devcaps;
		if(ErrorHandler("d3d8->GetDeviceCaps", d3d8->GetDeviceCaps(&devcaps)))
		{
			if (bTrace) api->Trace("Cube map texture %s is not created (size: %i, num mips: %i, format: %s), not loading it.", fn, head.width, head.nmips, formatTxt);
			delete Textures[t].name;
			Textures[t].name = 0;
			api->fio->_CloseHandle(file);
			return false;
		}
		if(!(devcaps.TextureCaps & D3DPTEXTURECAPS_MIPCUBEMAP)) head.nmips = 1;
		//������ ��������
		IDirect3DCubeTexture9 * tex = null;
		if(ErrorHandler("d3d8->CreateCubeTexture", d3d8->CreateCubeTexture(head.width, head.nmips, 0, d3dFormat, D3DPOOL_MANAGED, &tex, NULL))==true || !tex)
		{
			if (bTrace) api->Trace("Cube map texture %s is not created (size: %i, num mips: %i, format: %s), not loading it.", fn, head.width, head.nmips, formatTxt);
			delete Textures[t].name;
			Textures[t].name = 0;
			api->fio->_CloseHandle(file);
			return false;
		}
		//��������� ������
		bool isError = false;
		if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
		dword sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_POSITIVE_Z, head.nmips, head.mip_size, head.width, isSwizzled);
		if(sz)
		{
			Textures[t].dwSize += sz;
			if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
			sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_POSITIVE_X, head.nmips, head.mip_size, head.width, isSwizzled);
			if(sz)
			{
				Textures[t].dwSize += sz;
				if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
				sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_NEGATIVE_Z, head.nmips, head.mip_size, head.width, isSwizzled);
				if(sz)
				{
					Textures[t].dwSize += sz;
					if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
					sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_NEGATIVE_X, head.nmips, head.mip_size, head.width, isSwizzled);
					if(sz)
					{
						Textures[t].dwSize += sz;
						if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
						sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_POSITIVE_Y, head.nmips, head.mip_size, head.width, isSwizzled);
						if(sz)
						{
							Textures[t].dwSize += sz;
							if(seekposition) api->fio->_SetFilePointer(file, seekposition, 0, FILE_CURRENT);
							sz = LoadCubmapSide(file, tex, D3DCUBEMAP_FACE_NEGATIVE_Y, head.nmips, head.mip_size, head.width, isSwizzled);
							if(!sz) isError = true;
							Textures[t].dwSize += sz;
						}else isError = true;
					}else isError = true;
				}else isError = true;
			}else isError = true;
		}else isError = true;
		if(isError)
		{
			if (bTrace) api->Trace("Cube map texture %s can't loading (size: %i, num mips: %i, format: %s), not loading it.", fn, head.width, head.nmips, formatTxt);
			delete Textures[t].name;
			Textures[t].name = 0;
			api->fio->_CloseHandle(file);
			tex->Release();
			return false;
		}
		Textures[t].d3dtex = tex;
		Textures[t].isCubeMap = true;
	}

//---------------------------------------------------------------
//print statistics
//---------------------------------------------------------------
//#ifndef _XBOX
	if(texLog)
	{
		char s[256];
		if(totSize == 0) api->fio->_DeleteFile("texLoad.txt");
		HANDLE fh = api->fio->_CreateFile("texLoad.txt",GENERIC_WRITE,FILE_SHARE_WRITE,OPEN_ALWAYS);
		api->fio->_SetFilePointer(fh,0,0,FILE_END);
		totSize += Textures[t].dwSize;
		sprintf(s, "%.2f, size: %d, %d * %d, %s\n", totSize/1024.0f/1024.0f, Textures[t].dwSize, head.width, head.height, Textures[t].name);
		api->fio->_WriteFile(fh,s,strlen(s),0);
		api->fio->_FlushFileBuffers(fh);
		api->fio->_CloseHandle(fh);

	}
//#endif
	dwTotalSize += Textures[t].dwSize;
//---------------------------------------------------------------
	Textures[t].loaded = true;
	//��������� ����
	api->fio->_CloseHandle(file);
	return true;
}

IDirect3DBaseTexture9 * DX8RENDER::GetBaseTexture(long iTexture)
{
	return (iTexture >= 0) ? Textures[iTexture].d3dtex : null;
}

dword DX8RENDER::LoadCubmapSide(HANDLE file, IDirect3DCubeTexture9 * tex, D3DCUBEMAP_FACES face, dword numMips, dword mipSize, dword size, bool isSwizzled)
{
	dword texsize = 0;
	//��������� ������
	for(dword m = 0; m < numMips; m++)
	{
		//��������� ������ ����
		texsize += mipSize;
		//�������� ����������� ����
		bool isError = false;
		IDirect3DSurface9 * surface = null;
		if(ErrorHandler("tex->GetCubeMapSurface", tex->GetCubeMapSurface(face, m, &surface))==true || !surface)
		{
			isError = true;
		}else{
			//���������� ���
			isError = !LoadTextureSurface(file, surface, mipSize, size, size, isSwizzled);
		}
		//����������� �����������
		if(surface) surface->Release();
		//���� ���� ������, �� ��������� ��������
		if(isError)
		{
			if (bTrace) api->Trace("Can't loading cubemap mip %i (side: %i), not loading it.", m, face);
			return 0;
		}
		//������������� ������� ��� ���������� ����
		size /= 2;
		mipSize /= 4;
	}
	return texsize;
}

bool DX8RENDER::LoadTextureSurface(HANDLE file, IDirect3DSurface9 * suface, dword mipSize, dword width, dword height, bool isSwizzled)
{
#ifndef _XBOX
//------------------------------------------------------------------------------------------
//PC version
//------------------------------------------------------------------------------------------
	//��������� �� �����������
	D3DLOCKED_RECT lock;
	if(ErrorHandler("suface->LockRect", suface->LockRect(&lock, NULL, 0L))==true) return false;
	//�����������
	DWORD readingBytes = 0;
	if(!api->fio->_ReadFile(file, lock.pBits, mipSize, &readingBytes) || readingBytes != mipSize)
	{
		if(ErrorHandler("suface->UnlockRect", suface->UnlockRect())==true) return false;
		return false;
	}
	//������������ �����������
	if(ErrorHandler("suface->UnlockRect", suface->UnlockRect())==true) return false;
	return true;
//------------------------------------------------------------------------------------------
#else
//------------------------------------------------------------------------------------------
//XBOX version
//------------------------------------------------------------------------------------------
	//����� ��� ������������� �������
	void * buffer = null;
	if(!isSwizzled) buffer = NEW char[mipSize];
	//���� �� �������
	dword bytesPerPixel = mipSize/(width*height);
	//��������� �� �����������
	D3DLOCKED_RECT lock;
	if(ErrorHandler("suface->LockRect", suface->LockRect(&lock, NULL, 0L))==true) return false;
	if(isSwizzled) buffer = lock.pBits;
	//�����������
	DWORD readingBytes = 0;
	if(!api->fio->_ReadFile(file, buffer, mipSize, &readingBytes) || readingBytes != mipSize)
	{
		if(ErrorHandler("suface->UnlockRect", suface->UnlockRect())==true) return false;
		return false;
	}
	//������������������
	if(!isSwizzled)
	{
		byte * notAlignedBuffer = NEW byte[mipSize + 16];
		byte * alignedBuffer = (byte *)(((dword)notAlignedBuffer + 15) & ~15);
		XGSwizzleRect(buffer, width*bytesPerPixel, 0, alignedBuffer, width, height, 0, bytesPerPixel);
		memcpy(lock.pBits, alignedBuffer, mipSize);
		delete notAlignedBuffer;
	}
	//������������ �����������
	if(ErrorHandler("suface->UnlockRect", suface->UnlockRect())==true) return false;
	//������������ ������
	if(!isSwizzled) delete buffer;
	return true;
//------------------------------------------------------------------------------------------
#endif
}



//################################################################################
bool DX8RENDER::TextureSet(long stage, long texid)
{
	if(texid==-1)
	{
		if(ErrorHandler("SetTexture", d3d8->SetTexture(stage, NULL))==true)	return false;
		return true;
	}

/*
	if(Textures[texid].loaded==false)
	{
		int tex2load[MAX_STEXTURES];
		int t2l=0;
		for(long t=0; t<MAX_STEXTURES; t++)
			if(Textures[t].ref>0 && Textures[t].loaded==false)	tex2load[t2l++] = t;
*/
		/*/sort textures
		for(t=0; t<t2l; t++)
			for(long tt=t; tt<t2l; tt++)
				if(strcmpi(Textures[tex2load[tt]].name, Textures[tex2load[t]].name)<0)
				{
					int ttemp = tex2load[t];
					tex2load[t] = tex2load[tt];
					tex2load[tt] = ttemp;
				}*/
/*
		for(t=0; t<t2l; t++)
		{
			Textures[tex2load[t]].loaded = true;
			TextureLoad(tex2load[t]);
		}
		//Textures[texid].loaded = true;	TextureLoad(texid);
	}
*/

	if(ErrorHandler("SetTexture", d3d8->SetTexture(stage, Textures[texid].d3dtex))==true)	return false;
	return true;
}

//################################################################################
bool DX8RENDER::TextureRelease(long texid)
{
	if(texid==-1)	return true;
	Textures[texid].ref--;
	if(Textures[texid].ref!=0) return false;
	if(Textures[texid].name!=NULL)
	{

//#ifndef _XBOX
		if(texLog)
		{


			HANDLE fh = api->fio->_CreateFile("texLoad.txt",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,OPEN_ALWAYS);

			FILE *flstat = fopen("texLoad.txt", "r+b");
			totSize -= Textures[texid].dwSize;
			int bytes = api->fio->_GetFileSize(fh,0);
			char *buf = NEW char[bytes+1];
			api->fio->_ReadFile(fh,buf, bytes, 0);
			buf[bytes] = 0;

			char *str = strstr(buf, Textures[texid].name);
			if(str!=0)
			{
				api->fio->_SetFilePointer(fh,str-buf,0,FILE_BEGIN);
				const char *s = "*";
				api->fio->_WriteFile(fh,s,1,0);
			}
			delete buf;
			api->fio->_FlushFileBuffers(fh);
			api->fio->_CloseHandle(fh);

			/*FILE *flstat = fopen("texLoad.txt", "r+b");
			totSize -= Textures[texid].dwSize;
			fseek(flstat, 0, SEEK_END);
			int bytes = ftell(flstat);
			char *buf = NEW char[bytes+1];
			fseek(flstat, 0, SEEK_SET);
			fread(buf, bytes, sizeof *buf, flstat);
			buf[bytes] = 0;

			char *str = strstr(buf, Textures[texid].name);
			if(str!=0)
			{
				fseek(flstat, str-buf, SEEK_SET);
				const char *s = "*";
				fwrite(s, 1, 1, flstat);
			}
			delete buf;
			fclose(flstat);*/
		}
//#endif

		delete Textures[texid].name;
		Textures[texid].name = NULL;
	}
	if (Textures[texid].loaded==false)	return false;

	if(Textures[texid].d3dtex)
	if(ErrorHandler("Release", Textures[texid].d3dtex->Release())==true)	return false;
	Textures[texid].d3dtex = NULL;
	dwTotalSize -= Textures[texid].dwSize;

	return true;
}


//################################################################################
bool DX8RENDER::SetCamera(CVECTOR *pos, CVECTOR *ang, float fov)
{
	if(!SetCamera(pos, ang)) return false;
	if(!SetPerspective(fov, aspectRatio)) return false;

	ProcessScriptPosAng(*pos, *ang);
	return true;
}

bool DX8RENDER::SetCamera(CVECTOR *pos, CVECTOR *ang)
{
	CMatrix mtx;
	CVECTOR vOldWordRelationPos = vWordRelationPos;
	if(!pos || !ang)
	{
		if(!pos && !ang) return true;
		//if(ErrorHandler("SetCamera::GetTransform", GetTransform(D3DTS_VIEW, mtx))==true) return false;
		d3d8->GetTransform(D3DTS_VIEW, mtx);
		if(pos)
		{
			//mtx.SetInversePosition(pos->x, pos->y, pos->z);
			Pos = *pos;
			vWordRelationPos = -Pos;
		}else{
			//CVECTOR p;
			//mtx.MulToInvNorm(mtx.Pos(), p);
			mtx.BuildMatrix(*ang);
			mtx.Transposition3X3();
			//mtx.SetInversePosition(p.x, p.y, p.z);
			Ang = *ang;
		}
	}else{
		mtx.BuildMatrix(*ang);
		mtx.Transposition3X3();
		//mtx.SetInversePosition(pos->x, pos->y, pos->z);
		Pos = *pos;
		Ang = *ang;
		vWordRelationPos = -Pos;
	}
	//if(ErrorHandler("SetCamera::SetTransform", SetTransform(D3DTS_VIEW, mtx))==true)	return false;

	d3d8->SetTransform(D3DTS_VIEW, mtx);
	vViewRelationPos = -(mtx * vWordRelationPos);
	d3d8->GetTransform(D3DTS_WORLD, mtx);
	mtx.Pos() += vWordRelationPos - vOldWordRelationPos;
	d3d8->SetTransform(D3DTS_WORLD, mtx);

	FindPlanes(d3d8);
	ProcessScriptPosAng(*pos, *ang);
	return true;
}

bool DX8RENDER::SetCamera(CVECTOR lookFrom, CVECTOR lookTo, CVECTOR up)
{
	CMatrix mtx;
	if(!mtx.BuildViewMatrix(lookFrom, lookTo, up)) return false;
	//if(ErrorHandler("SetCamera::SetTransform", SetTransform(D3DTS_VIEW, mtx))==true)	return false;
	SetTransform(D3DTS_VIEW, mtx);
	Pos = lookFrom;

	// �������� ���� ������
	//Ang = 0.0f;
	CVECTOR vNorm = !(lookTo - lookFrom);
	Ang.y = atan2f(vNorm.x,vNorm.z);
	Ang.x = atan2f(-vNorm.y,sqrtf(vNorm.x*vNorm.x + vNorm.z*vNorm.z));
	Ang.z = 0.f;

	FindPlanes(d3d8);
	ProcessScriptPosAng(lookFrom, Ang);
	return true;
}

void DX8RENDER::ProcessScriptPosAng(CVECTOR & vPos, CVECTOR & vAng)
{
	api->Event("CameraPosAng","ffffff",vPos.x,vPos.y,vPos.z,vAng.x,vAng.y,vAng.z);
}

void DX8RENDER::GetNearFarPlane(float & fNear, float & fFar)
{
	fNear = fNearClipPlane;
	fFar = fFarClipPlane;
}

void DX8RENDER::SetNearFarPlane(float fNear, float fFar)
{
	fNearClipPlane = fNear;
	fFarClipPlane = fFar;

	SetPerspective(Fov, aspectRatio);
}

bool DX8RENDER::SetPerspective(float perspective, float fAspectRatio)
{
	float near_plane = fNearClipPlane; // Distance to near clipping
	float far_plane = fFarClipPlane;  // Distance to far clipping
	float fov_horiz = perspective;  // Horizontal field of view  angle, in radians
	if (fAspectRatio<0)
	{
		fAspectRatio = float(screen_size.y) / screen_size.x;
	}
	aspectRatio = fAspectRatio;
	float fov_vert = perspective*fAspectRatio;   // Vertical field of view  angle, in radians

	float w = 1.0f/tanf(fov_horiz*0.5f);
	float h = 1.0f/tanf(fov_vert*0.5f);
	float Q = far_plane/(far_plane - near_plane);

	D3DXMATRIX mtx;
	ZeroMemory(&mtx, sizeof(mtx));

	mtx._11 = w;
	mtx._22 = h;
	mtx._33 = Q;
	mtx._43 = -Q*near_plane;
	mtx._34 = 1.0f;

	if(ErrorHandler("SetPerspective::SetTransform", d3d8->SetTransform(D3DTS_PROJECTION, &mtx))==true)	return false;
	Fov = perspective;
	FindPlanes(d3d8);
	return true;
}

//################################################################################
bool DX8RENDER::SetCurrentMatrix(D3DMATRIX *mtx)
{
	//if(ErrorHandler("SetCurrentMatrix::SetTransform", SetTransform(D3DTS_WORLD, mtx))==true)	return false;
	SetTransform(D3DTS_WORLD, mtx);
	return true;
}

bool DX8RENDER::SetMaterial(D3DMATERIAL9 &m)
{
	if (ErrorHandler("SetMaterial::SetMaterial", d3d8->SetMaterial(&m))) return false;
	return true;
}
bool DX8RENDER::SetLight(dword dwIndex, const D3DLIGHT9 * pLight)
{
	// Set the property information for the first light.
	D3DLIGHT9 tmpLight = *pLight;
	tmpLight.Position.x += vWordRelationPos.x;
	tmpLight.Position.y += vWordRelationPos.y;
	tmpLight.Position.z += vWordRelationPos.z;
	if (ErrorHandler("SetLight::SetLight", d3d8->SetLight(dwIndex, &tmpLight))) return false;
	return true;
}

bool DX8RENDER::LightEnable(dword dwIndex, bool bOn)
{
	if (ErrorHandler("LightEnable::LightEnable", d3d8->LightEnable(dwIndex, bOn))) return false;
	return true;
}

bool DX8RENDER::GetLightEnable(DWORD dwIndex, BOOL * pEnable)
{
	if (ErrorHandler("LightEnable::LightEnable", d3d8->GetLightEnable(dwIndex, pEnable))) return false;
	return true;
}

bool DX8RENDER::GetLight(DWORD dwIndex, D3DLIGHT9 * pLight)
{
	if (ErrorHandler("LightEnable::LightEnable", d3d8->GetLight(dwIndex, pLight))) return false;
	pLight->Position.x -= vWordRelationPos.x;
	pLight->Position.y -= vWordRelationPos.y;
	pLight->Position.z -= vWordRelationPos.z;
	return true;
}

//################################################################################

long DX8RENDER::CreateVertexBuffer(long type, long size, dword dwUsage)
{
	if (size <=0 ) return -1; // fix
	long b;
	for(b=0; b<MAX_BUFFERS; b++)
		if(VertexBuffers[b].buff==NULL)	break;
	if(b==MAX_BUFFERS)	return -1;

	if(ErrorHandler("CreateVertexBuffer::CreateVertexBuffer",
	d3d8->CreateVertexBuffer(size, dwUsage,
		type, D3DPOOL_DEFAULT, &VertexBuffers[b].buff, NULL)
		)==true)	return -1;

	VertexBuffers[b].type = type;
	VertexBuffers[b].size = size;

	return b;
}

IDirect3DVertexBuffer9 * DX8RENDER::GetVertexBuffer(long id)
{
	if(id < 0 || id >= MAX_BUFFERS) return null;
	return VertexBuffers[id].buff;
}

long DX8RENDER::GetVertexBufferFVF(long id)
{
	if(id < 0 || id >= MAX_BUFFERS) return 0;
	return VertexBuffers[id].type;
}

//################################################################################
long DX8RENDER::CreateIndexBuffer(long size, dword dwUsage)
{
	long b;
	for(b=0; b<MAX_BUFFERS; b++)
		if(IndexBuffers[b].buff==NULL)	break;
	if(b==MAX_BUFFERS)	return -1;

	if(ErrorHandler("CreateIndexBuffer::CreateIndexBuffer",
	//d3d8->CreateIndexBuffer(size, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
	d3d8->CreateIndexBuffer(size, dwUsage, D3DFMT_INDEX16,
		D3DPOOL_DEFAULT, &IndexBuffers[b].buff, NULL)
		)==true)	return -1;

	IndexBuffers[b].ntrgs = size;

	return b;
}
//################################################################################
void DX8RENDER::DrawBuffer(long vbuff, long stride, long ibuff, long minv,
												 long numv, long startidx, long numtrg, const char *cBlockName, dword dwNumParams, ...)
{
	bool bDraw = true;

	if(vbuff>=0)
		if(ErrorHandler("DrawBuffer::SetFVF",
			d3d8->SetFVF(VertexBuffers[vbuff].type))==true)	return;
	//else VertexBuffer already set

	if(ErrorHandler("DrawBuffer::SetIndices",
		d3d8->SetIndices(IndexBuffers[ibuff].buff)
		)==true)	return;

	if(vbuff>=0)
		if(ErrorHandler("DrawBuffer::SetStreamSource",
			d3d8->SetStreamSource(0, VertexBuffers[vbuff].buff, 0, stride)
			)==true)	return;
	//else VertexBuffer already set

	if (cBlockName && cBlockName[0]) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		dwNumDrawPrimitive++;
		ErrorHandler("DrawBuffer::DrawIndexedPrimitive",
			d3d8->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, minv, 0, numv,  startidx, numtrg));
	} while (cBlockName && cBlockName[0] && TechniqueExecuteNext());
}

void DX8RENDER::DrawIndexedPrimitiveNoVShader(D3DPRIMITIVETYPE dwPrimitiveType, long iVBuff, long iStride, long iIBuff, long iMinV,
												 long iNumV, long iStartIdx, long iNumTrg, const char *cBlockName, dword dwNumParams, ...)
{
	bool bDraw = true;

	if(ErrorHandler("DrawIndexedPrimitiveNoVShader::SetIndices",
		d3d8->SetIndices(IndexBuffers[iIBuff].buff)
		)==true)	return;

	if (iVBuff >= 0)
	{
		if(ErrorHandler("DrawIndexedPrimitiveNoVShader::SetStreamSource",
			d3d8->SetStreamSource(0, VertexBuffers[iVBuff].buff, 0, iStride)
			)==true)	return;
	}

	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		dwNumDrawPrimitive++;
		ErrorHandler("DrawIndexedPrimitiveNoVShader::DrawIndexedPrimitive",
			d3d8->DrawIndexedPrimitive( dwPrimitiveType, iMinV, 0, iNumV,  iStartIdx, iNumTrg));
	} while (cBlockName && TechniqueExecuteNext());
}

void DX8RENDER::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE dwPrimitiveType, dword dwMinIndex, dword dwNumVertices, dword dwPrimitiveCount, const void *pIndexData, D3DFORMAT IndexDataFormat, const void *pVertexData, dword dwVertexStride, const char *cBlockName, dword dwNumParams, ...)
{
	bool bDraw = true;
	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		dwNumDrawPrimitive++;
		ErrorHandler("DrawIndexedPrimitiveUP::DrawIndexedPrimitiveUP",
			d3d8->DrawIndexedPrimitiveUP( dwPrimitiveType, dwMinIndex, dwNumVertices, dwPrimitiveCount, pIndexData, IndexDataFormat, pVertexData, dwVertexStride));
	} while (cBlockName && TechniqueExecuteNext());
}

void DX8RENDER::DrawPrimitiveUP(D3DPRIMITIVETYPE dwPrimitiveType, dword dwVertexBufferFormat, dword dwNumPT, void *pVerts, dword dwStride, const char *cBlockName, dword dwNumParams, ...)
{
	bool bDraw = true;

	if(ErrorHandler("DrawPrimitiveUP::SetFVF",
		d3d8->SetFVF(dwVertexBufferFormat))==true)	return;

	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		dwNumDrawPrimitive++;
		ErrorHandler("DrawPrimitiveUP::DrawPrimitiveUP",
			d3d8->DrawPrimitiveUP( dwPrimitiveType, dwNumPT, pVerts, dwStride));
	} while (cBlockName && TechniqueExecuteNext());
}

void DX8RENDER::DrawPrimitive(D3DPRIMITIVETYPE dwPrimitiveType, long iVBuff, long iStride, long iStartV, long iNumPT, const char *cBlockName, dword dwNumParams, ...)
{
	bool bDraw = true;

	if(ErrorHandler("DrawPrimitive::SetFVF",
		d3d8->SetFVF(VertexBuffers[iVBuff].type))==true)	return;

	if(ErrorHandler("DrawPrimitive::SetStreamSource",
		d3d8->SetStreamSource(0, VertexBuffers[iVBuff].buff, 0, iStride)
		)==true)	return;

	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		dwNumDrawPrimitive++;
		ErrorHandler("DrawPrimitive::DrawPrimitive",
			d3d8->DrawPrimitive( dwPrimitiveType, iStartV, iNumPT));
	} while (cBlockName && TechniqueExecuteNext());
}

//################################################################################
void DX8RENDER::RenderAnimation(long ib, void * src, long numVrts, long minv, long numv,  long startidx, long numtrg, bool isUpdateVB)
{
	if(numVrts <= 0 || !src || ib < 0) return;
	dword type = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	long size = numVrts*sizeof(FVF_VERTEX);
	if(isUpdateVB || numVrts > numAniVerteces || !aniVBuffer)
	{
		//Create vertex buffer
		if(numVrts > numAniVerteces)
		{
			if(aniVBuffer) aniVBuffer->Release();
			aniVBuffer = null;
			numAniVerteces = 0;
			if(ErrorHandler("RenderAnimation::CreateVertexBuffer",
				d3d8->CreateVertexBuffer(size,
											D3DUSAGE_WRITEONLY,
											type,
											D3DPOOL_MANAGED,
											&aniVBuffer, NULL))==true) return;
			numAniVerteces = numVrts;
		}
		//Copy verteces
		BYTE *ptr;
	RDTSC_B(_rdtsc);
		if(ErrorHandler("RenderAnimation::LockVertexBuffer",
			aniVBuffer->Lock(0, size, (VOID**)&ptr, 0)
			)==true) return;
	dwNumLV++;
	RDTSC_E(_rdtsc);
		memcpy(ptr, src, size);
		ErrorHandler("RenderAnimation::UnLockVertexBuffer",
			aniVBuffer->Unlock());
	}
	//Render
	if(ErrorHandler("RenderAnimation::SetFVF",
		d3d8->SetFVF(type))==true)	return;

	if(ErrorHandler("RenderAnimation::SetIndices",
		d3d8->SetIndices(IndexBuffers[ib].buff)
		)==true)	return;

	if(ErrorHandler("RenderAnimation::SetStreamSource",
		d3d8->SetStreamSource(0, aniVBuffer, 0, sizeof(FVF_VERTEX))
		)==true)	return;

	ErrorHandler("RenderAnimation::DrawIndexedPrimitive",
		d3d8->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, minv, 0, numv,  startidx, numtrg));

	dwNumDrawPrimitive++;
}

//################################################################################
void * DX8RENDER::LockVertexBuffer(long id, dword dwFlags)
{
	BYTE * ptr;
	VertexBuffers[id].dwNumLocks++;
	if (ErrorHandler("DX8RENDER::LockVertexBuffer", VertexBuffers[id].buff->Lock(0, VertexBuffers[id].size, (VOID**)&ptr, dwFlags)))	return 0;
	dwNumLV++;
	return ptr;
}
//################################################################################
void DX8RENDER::UnLockVertexBuffer(long id)
{
	VertexBuffers[id].dwNumLocks--;
	ErrorHandler("UnLockVertexBuffer::UnLockVertexBuffer",
		VertexBuffers[id].buff->Unlock());
}
long DX8RENDER::GetVertexBufferSize(long id) { return VertexBuffers[id].size; }

void * DX8RENDER::LockIndexBuffer(long id, dword dwFlags)
{
	BYTE * ptr = null;
	IndexBuffers[id].dwNumLocks++;
	if(ErrorHandler("LockIndexBuffer::LockIndexBuffer",
		IndexBuffers[id].buff->Lock(0, IndexBuffers[id].ntrgs, (VOID**)&ptr, dwFlags)
		)==true)	return 0;
	dwNumLI++;
	return ptr;
}

void DX8RENDER::UnLockIndexBuffer(long id)
{
	IndexBuffers[id].dwNumLocks--;
	ErrorHandler("UnLockIndexBuffer::UnLockIndexBuffer",
		IndexBuffers[id].buff->Unlock());
}
//################################################################################
void DX8RENDER::ReleaseVertexBuffer(long id)
{
	if(VertexBuffers[id].buff==NULL)	return;
	ErrorHandler("ReleaseVertexBuffer::ReleaseVertexBuffer",
		VertexBuffers[id].buff->Release());
	VertexBuffers[id].buff = NULL;
	VertexBuffers[id].dwNumLocks = 0;
}
//################################################################################
void DX8RENDER::ReleaseIndexBuffer(long id)
{
	if(IndexBuffers[id].buff==NULL)	return;
	ErrorHandler("ReleaseIndexBuffer::ReleaseIndexBuffer",
		IndexBuffers[id].buff->Release());
	IndexBuffers[id].buff = NULL;
	IndexBuffers[id].dwNumLocks = 0;
}

void DX8RENDER::SetTransform(long type, D3DMATRIX *mtx)
{
	CMatrix m = *(CMatrix*)mtx;
	if( type == D3DTS_VIEW )
	{
		vViewRelationPos.x = -mtx->_41;
		vViewRelationPos.y = -mtx->_42;
		vViewRelationPos.z = -mtx->_43;

		CVECTOR vDeltaWorld = vWordRelationPos;
		m.MulToInvNorm(-vViewRelationPos,vWordRelationPos);
		vDeltaWorld -= vWordRelationPos;
		CMatrix mw;
		d3d8->GetTransform( D3DTS_WORLD, (D3DMATRIX*)&mw );
		mw.Pos() -= vDeltaWorld;
		d3d8->SetTransform( D3DTS_WORLD, mw );

		m.Pos() += vViewRelationPos;
	}
	else if( type == D3DTS_WORLD )
	{
		m.Pos() += vWordRelationPos;
	}

	d3d8->SetTransform((D3DTRANSFORMSTATETYPE)type, (D3DMATRIX*)&m);
}

void DX8RENDER::GetTransform(long type, D3DMATRIX *mtx)
{
	d3d8->GetTransform((D3DTRANSFORMSTATETYPE)type, mtx);

	if( type == D3DTS_VIEW )
	{
		mtx->_41 -= vViewRelationPos.x;
		mtx->_42 -= vViewRelationPos.y;
		mtx->_43 -= vViewRelationPos.z;
	}
	else if( type == D3DTS_WORLD )
	{
		mtx->_41 -= vWordRelationPos.x;
		mtx->_42 -= vWordRelationPos.y;
		mtx->_43 -= vWordRelationPos.z;
	}
}

bool DX8RENDER::CreateState(ENTITY_STATE_GEN * state_gen)
{
	//state_gen->SetState("vm",sizeof(screen_size),screen_size,sizeof(bool),&window);
	return true;
}

bool DX8RENDER::LoadState(ENTITY_STATE * state)
{
	GUARD(DX8RENDER::Init)
	//for(long t=0; t<MAX_STEXTURES; t++)	Textures[t].d3dtex = NULL;
	//d3d = NULL;
	//d3d8 = NULL;
	//state->Struct(sizeof(screen_size),(char *)&screen_size);
	//state->MemoryBlock(sizeof(bool),(char *)&window);
	//InitDevice(window,api->GetAppHWND(),screen_size.x,screen_size.y);
	UNGUARD
	return true;

}

bool DX8RENDER::ResetDevice()
{
	ENTITY_ID eid;

	api->SetEntityScanLayer(null);
	if (api->GetEntity(&eid)) do { ((ENTITY*)eid.pointer)->LostRender(); } while (api->GetEntityNext(&eid));

	HRESULT hr = d3d8->Reset(&d3dpp);
	api->Trace("Device try to reset");
	api->Trace("Device reset %s", (hr == D3D_OK) ? "successfully" : "bad");
	ErrorHandler("reset", hr);

	api->SetEntityScanLayer(null);
	if (api->GetEntity(&eid)) do { ((ENTITY*)eid.pointer)->RestoreRender(); } while (api->GetEntityNext(&eid));
	return true;
}

void DX8RENDER::SetGLOWParams (float _fBlurBrushSize, long _GlowIntensity, long _GlowPasses)
{
	fBlurSize = _fBlurBrushSize;
	GlowIntensity = _GlowIntensity;
	iBlurPasses =	 _GlowPasses;
}

void DX8RENDER::RunStart()
{
	bDeviceLost = true;

	VDATA * pScriptRender = (VDATA *)api->GetScriptVariable("Render");
	ATTRIBUTES * pARender = pScriptRender->GetAClass();

	bSeaEffect = pARender->GetAttributeAsDword("SeaEffect", 0) != 0;
	fSeaEffectSize = pARender->GetAttributeAsFloat("SeaEffectSize", 0.0f);
	fSeaEffectSpeed = pARender->GetAttributeAsFloat("SeaEffectSpeed", 0.0f);
	dwBackColor = pARender->GetAttributeAsDword("BackColor", 0);

	if (bSeaEffect)
	{
		fSin += float(api->GetRDeltaTime()) * 0.001f * fSeaEffectSpeed;

		float sx = (float)screen_size.x;
		float sy = (float)screen_size.y;

		float fDX = float(long(sx * fSeaEffectSize));
		float fDY = float(long(sy * fSeaEffectSize));

		float sx2 = sx + fDX * 2.0f;
		float sy2 = sy + fDY * 2.0f;

		for (long y=0; y<32; y++)
		{
			for (long x=0; x<32; x++)
			{
				qv[x + y * 32].vPos = Vector4(sx * float(x) / 31.0f, sy * float(y) / 31.0f, 0.0f, 1.0f);

				qv[x + y * 32].u0 = (sx * float(x) / 31.0f + sinf(fSin + float(x) / 31.0f * PI * 16.0f) * fDX + fDX) / sx2;
				qv[x + y * 32].v0 = (sy * float(y) / 31.0f + sinf(fSin + float(y) / 31.0f * PI * 16.0f) * fDY + fDY) / sy2;

			}
		}
	}

	switch (d3d8->TestCooperativeLevel())
	{
		case D3DERR_DEVICENOTRESET:
			if (!ResetDevice()) return;
		break;
		case D3DERR_DEVICELOST:
			return;
		break;
	}

	bDeviceLost = false;

	bNeedCopyToScreen = true;
	//------------------------------------------
	if ((bPostProcessEnabled || bSeaEffect) && (!bPostProcessError))
	{
		SetPostProcessTextureAsRenderTarget();
	} else
	{
		SetScreenAsRenderTarget();
	}


	DX8Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | ((stencil_format==D3DFMT_D24S8) ? D3DCLEAR_STENCIL : 0));
	DX8BeginScene();


	//------------------------------------------
	bInsideScene = true;

	// execute default technique for set default render/texture states
	if (TechniqueExecuteStart("default")) do {} while (TechniqueExecuteNext());

#ifndef _XBOX

	 // boal del_cheat
	if (api->Controls->GetDebugAsyncKeyState(VK_F11) < 0)
	{
		DELETE(pTechnique);
		pTechnique = NEW CTechnique(this);
		pTechnique->DecodeFiles();
	}

	d3d8->SetRenderState(D3DRS_FILLMODE, (api->Controls->GetDebugAsyncKeyState('F') < 0) ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
	//d3d8->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID); eddy

#else
	d3d8->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif
PlayToTexture();
}

void DX8RENDER::RunEnd()
{
	MakePostProcess();

	bInsideScene = false;
	DX8EndScene();
	if(progressTexture >= 0)
	{
		long oldCnt = progressSafeCounter;
		ProgressView();
		progressSafeCounter = oldCnt;
		if(progressSafeCounter >= 50)
		{
			EndProgressView();
		}else progressSafeCounter++;
	}
}


char Buff_4k[4096];

long _cdecl DX8RENDER::Print(long x, long y,char * format,...)
{
	GUARD(DX8RENDER::Print)
    if(idFontCurrent<0 || idFontCurrent>=nFontQuantity) return 0;
    if(FontList[idFontCurrent].font==NULL || FontList[idFontCurrent].ref==0) return 0;

	va_list args;
	va_start(args,format);
	_vsnprintf(Buff_4k,sizeof(Buff_4k),format,args);
	va_end(args);

	return FontList[idFontCurrent].font->Print(x,y,Buff_4k);
	UNGUARD
}

long _cdecl DX8RENDER::Print(long nFontNum, DWORD color, long x, long y,char * format,...)
{
	GUARD(DX8RENDER::Print)
    if(nFontNum<0 || nFontNum>=nFontQuantity) return 0;
    if(FontList[nFontNum].font==NULL || FontList[nFontNum].ref==0) return 0;

	va_list args;
	va_start(args,format);
	_vsnprintf(Buff_4k,sizeof(Buff_4k),format,args);
	va_end(args);

	FontList[nFontNum].font->StoreFontParameters();
    FontList[nFontNum].font->SetColor(color);
	long retVal = FontList[nFontNum].font->Print(x,y,Buff_4k);
	FontList[nFontNum].font->RestoreFontParameters();
	return retVal;
	UNGUARD
}

long DX8RENDER::StringWidth(char * string, long nFontNum, float fScale, long scrWidth)
{
	if(nFontNum<0 || nFontNum>=nFontQuantity) return 0;
    FONT * pFont = FontList[nFontNum].font;
    if(FontList[nFontNum].ref==0 || pFont==NULL) return 0;

    pFont->StoreFontParameters();

	long xs = screen_size.x;
    if(scrWidth==0) scrWidth=xs;
	if(xs!=scrWidth)	fScale *= (float)xs/scrWidth;
	pFont->SetScale(fScale);

	long retVal = pFont->GetStringWidth(string);
	pFont->RestoreFontParameters();
	return retVal;
}

long DX8RENDER::CharWidth(char ch, long nFontNum, float fScale, long scrWidth)
{
    char str[2];
    str[0]=ch; str[1]=0;
	return StringWidth(str,nFontNum,fScale,scrWidth);
}

long DX8RENDER::CharHeight(long fontID)
{
    if(fontID<0 || fontID>=nFontQuantity) return 0;
    if(FontList[fontID].ref==0 || FontList[fontID].font==NULL) return 0;

    return FontList[fontID].font->GetHeight();
}

long _cdecl DX8RENDER::ExtPrint(long nFontNum,DWORD foreColor, DWORD backColor, int wAlign,
								bool bShadow, float fScale, long scrWidth, long scrHeight,
								long x, long y, char * format,...)
{
	GUARD(DX8RENDER::ExtPrint)

	if(nFontNum<0 || nFontNum>=nFontQuantity) return 0;
    FONT * pFont = FontList[nFontNum].font;
    if(FontList[nFontNum].ref==0 || pFont==NULL) return 0;

	va_list args;
	va_start(args,format);
	_vsnprintf(Buff_4k,sizeof(Buff_4k),format,args);
	va_end(args);

    pFont->StoreFontParameters();

	IDirect3DSurface9 * pRenderTarget;
	GetRenderTarget(&pRenderTarget);
	D3DSURFACE_DESC dscrSurface;
	pRenderTarget->GetDesc(&dscrSurface);
	long xs = dscrSurface.Width;
	long ys = dscrSurface.Height;
	pRenderTarget->Release();
    if(scrWidth==0) scrWidth=xs;
    if(scrHeight==0) scrHeight=ys;
	if(xs!=scrWidth)
	{
		float fHorzScale = (float)xs/scrWidth;
		x = long(x*fHorzScale);
		fScale *= fHorzScale;
	}
	if(ys!=scrHeight)
		y = long(y*((float)ys/scrHeight));

	switch(wAlign)
	{
	case ALIGN_CENTER:
		x-=long((pFont->GetStringWidth(Buff_4k)+1L)/2L*fScale);
		break;
	case ALIGN_RIGHT:
		x-=long(pFont->GetStringWidth(Buff_4k)*fScale);
		break;
	}

	pFont->SetColor(foreColor);
	pFont->SetShadow(bShadow);
	pFont->SetScale(fScale);
	long retVal = pFont->Print(x,y,Buff_4k);

	pFont->RestoreFontParameters();
	return retVal;
	UNGUARD
}

long DX8RENDER::LoadFont(char * fontName)
{
	if(fontName==NULL) return -1L;
	char sDup[256];
	if(strlen(fontName)<sizeof(sDup)-1) strcpy(sDup,fontName);
	else
	{
		strncpy(sDup,fontName,sizeof(sDup)-1);
		sDup[sizeof(sDup)-1] = 0;
	}
	fontName = strupr(sDup);
    unsigned long hashVal = hash_string(fontName);

	long i;
    for(i=0; i<nFontQuantity; i++)
        if( FontList[i].hash==hashVal && stricmp(FontList[i].name,fontName)==0 )
        {
            if( FontList[i].ref>0 )
                FontList[i].ref++;
            else
            {
                FontList[i].ref=1;
                FontList[i].font->RepeatInit();
            }
            return i;
        }
    if(nFontQuantity<MAX_FONTS)
    {
        if( (FontList[i].font=NEW FONT) == NULL )
            _THROW("allocate memory error");
        if( !FontList[i].font->Init(fontName,fontIniFileName,d3d8,this) )
		{
			delete FontList[i].font;
			api->Trace("Can't init font %s",fontName);
			return -1L;
		}
        FontList[i].hash = hashVal;
        FontList[i].ref = 1;
		if( (FontList[i].name=NEW char[strlen(fontName)+1])==NULL )
			_THROW("allocate memory error");
		strcpy(FontList[i].name,fontName);
        nFontQuantity++;
    }
    else
        _THROW("maximal font quantity exceeded")
    return i;
}

bool DX8RENDER::UnloadFont(char * fontName)
{
	if(fontName==NULL) return false;
	char sDup[256];
	if(strlen(fontName)<sizeof(sDup)-1) strcpy(sDup,fontName);
	else
	{
		strncpy(sDup,fontName,sizeof(sDup)-1);
		sDup[sizeof(sDup)-1] = 0;
	}
	fontName = strupr(sDup);
    unsigned long hashVal = hash_string(fontName);

    for(int i=0; i<nFontQuantity; i++)
        if( FontList[i].hash==hashVal && stricmp(FontList[i].name,fontName)==0 )
            return UnloadFont(i);
    api->Trace("Font name \"%s\" is not containing",fontName);
    return false;
}
bool DX8RENDER::UnloadFont(long fontID)
{
    if(fontID<0 || fontID>=nFontQuantity) return false;

    if( FontList[fontID].ref>0 )
    {
        FontList[fontID].ref--;
        if( FontList[fontID].ref == 0 )
        {
            FontList[fontID].font->TempUnload();
            if(idFontCurrent==fontID)
                idFontCurrent=FONT_DEFAULT;
        }
        else return false;
    }

    return true;
}

bool DX8RENDER::SetCurFont(char * fontName)
{
	if(fontName==NULL) return false;
	char sDup[256];
	if(strlen(fontName)<sizeof(sDup)-1) strcpy(sDup,fontName);
	else
	{
		strncpy(sDup,fontName,sizeof(sDup)-1);
		sDup[sizeof(sDup)-1] = 0;
	}
	fontName = strupr(sDup);
    unsigned long hashVal = hash_string(fontName);

    for(int i=0; i<nFontQuantity; i++)
        if( FontList[i].hash==hashVal )
        {
            idFontCurrent=i;
            return true;
        }
    api->Trace("Font name \"%s\" is not containing",fontName);
    return false;
}
bool DX8RENDER::SetCurFont(long fontID)
{
    if(fontID<0 || fontID>=nFontQuantity) return false;
    idFontCurrent=fontID;
    return true;
}

long DX8RENDER::GetCurFont()
{
    if(idFontCurrent>=0 && idFontCurrent<nFontQuantity)
        return idFontCurrent;
    else return -1L;
}

char * DX8RENDER::GetFontIniFileName()
{
    return fontIniFileName;
}
bool DX8RENDER::SetFontIniFileName(char * iniName)
{
	if( fontIniFileName!=NULL && iniName!=NULL && stricmp(fontIniFileName,iniName)==0 ) return true;
    if(fontIniFileName!=NULL)
        delete fontIniFileName;
    if(iniName==NULL)
    {
        fontIniFileName = NULL;
        return false;
    }
    else
    {
        if( (fontIniFileName=NEW char[strlen(iniName)+1]) == NULL )
            _THROW("allocate memory error")
        strcpy(fontIniFileName,iniName);
    }

	for(int n=0; n<nFontQuantity; n++)
	{
		if( FontList[n].font != NULL ) {
			delete FontList[n].font;
		}
		if( (FontList[n].font=NEW FONT) == NULL )
			_THROW("allocate memory error")
		FontList[n].font->Init( FontList[n].name, fontIniFileName, d3d8, this );
		if( FontList[n].ref==0 ) FontList[n].font->TempUnload();
	}

    return true;
}

void DX8RENDER::func()
{
	d3d8->SetRenderState(D3DRS_DITHERENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_ALPHATESTENABLE, true );
	//----------Z---------
	d3d8->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	d3d8->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

	d3d8->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	d3d8->SetRenderState(D3DRS_ALPHABLENDENABLE, true );
	d3d8->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3d8->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	d3d8->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	d3d8->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	d3d8->SetRenderState(D3DRS_LIGHTING, TRUE);
	d3d8->SetRenderState(D3DRS_LOCALVIEWER, FALSE);
	d3d8->SetRenderState(D3DRS_AMBIENT, 0x505050);
	d3d8->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);

	d3d8->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	d3d8->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	d3d8->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d8->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3d8->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_ALPHAREF, 0xa0);
	d3d8->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// *****************************************************************88
	//texture filtering
	d3d8->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3d8->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	d3d8->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	d3d8->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 3);

	//unchanged texture stage states - both for base and detal texture
	d3d8->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	d3d8->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	d3d8->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
	d3d8->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	d3d8->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_CURRENT);
	d3d8->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	d3d8->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	d3d8->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
	d3d8->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	//general
	d3d8->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	d3d8->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	d3d8->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3d8->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	d3d8->SetRenderState(D3DRS_ALPHAREF, 0xa0);
	d3d8->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	//lighting effects
	d3d8->SetRenderState(D3DRS_AMBIENT, 0x404040);
	d3d8->SetRenderState(D3DRS_LIGHTING, FALSE);//TRUE);
	d3d8->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	d3d8->SetRenderState(D3DRS_SPECULARENABLE, FALSE);//TRUE);


}

HRESULT DX8RENDER::GetViewport(D3DVIEWPORT9 * pViewport)
{
	return d3d8->GetViewport(pViewport);
}

HRESULT DX8RENDER::SetViewport(const D3DVIEWPORT9 * pViewport)
{
	return d3d8->SetViewport(pViewport);
}

dword DX8RENDER::SetRenderState(dword State, dword Value)
{
	return d3d8->SetRenderState((D3DRENDERSTATETYPE)State,Value);
}

dword DX8RENDER::GetRenderState(dword State, dword * pValue)
{
	return d3d8->GetRenderState((D3DRENDERSTATETYPE)State,pValue);
}

dword DX8RENDER::SetTextureStageState(dword Stage,dword Type,dword Value)
{
	return d3d8->SetTextureStageState(Stage,(D3DTEXTURESTAGESTATETYPE)Type,Value);
}

dword DX8RENDER::GetTextureStageState(dword Stage,dword Type,dword* pValue)
{
	return d3d8->GetTextureStageState(Stage,(D3DTEXTURESTAGESTATETYPE)Type,pValue);
}

void DX8RENDER::GetCamera(CVECTOR& pos, CVECTOR& ang, float& perspective)
{
	pos = Pos;
	ang = Ang;
	perspective = Fov;
}

typedef struct tagTGA_H{
	BYTE byte1; // = 0
	BYTE byte2; // = 0
	BYTE type;
	BYTE byte4_9[9]; // = 0
	WORD width;
	WORD height;
	BYTE bpp; // bit per pixel
	BYTE attr:4;
	BYTE rez:1;
	BYTE origin:1;
	BYTE storage:2;
}TGA_H;

//WORD Temp[1600*4];

void DX8RENDER::SaveShoot()
{
	bMakeShoot = true;
}

void DX8RENDER::MakeScreenShot()
{
	GUARD(DX8RENDER::MakeScreenShot)
	static long file_number = 0;
	static TGA_H Dhdr = {0,0,2,0,0,0,0,0,0,0,0,0,0,0,32};
	char file_name[_MAX_PATH];
	long i;
	HANDLE fh;
	DWORD * Surface;
	RECT r;
	D3DLOCKED_RECT lr;
	POINT p;
	IDirect3DSurface9 * renderTarget, * surface;

	bMakeShoot = false;

	if(!(screen_bpp == D3DFMT_X8R8G8B8 || screen_bpp == D3DFMT_A8R8G8B8))
	{
		api->Trace("Can't make screenshots in non 32bit video modes");
		return;
	}

	r.left = 0; r.right = screen_size.x;
	r.top = 0; r.bottom = screen_size.y;
	p.x = p.y = 0;
	renderTarget = null;
	surface = null;

	//�������� ��������
	if(FAILED(d3d8->GetRenderTarget(0, &renderTarget)))
	{
		api->Trace("Falure get render target for make screenshot");
		return;
	}
	if(FAILED(d3d8->CreateOffscreenPlainSurface(screen_size.x, screen_size.y, D3DFMT_X8R8G8B8, D3DPOOL_SCRATCH, &surface, NULL)))
	{
		renderTarget->Release();
		api->Trace("Falure create buffer for make screenshot");
		return;
	}

	if(FAILED(d3d8->UpdateSurface(renderTarget, null, surface, null)) ||
		FAILED(surface->LockRect(&lr, &r, 0)))
	{
		surface->Release();
		renderTarget->Release();
		api->Trace("Falure make screenshot");
		return;
	}
	renderTarget->Release();
	renderTarget = null;
	//�������� ��� �����
	for(i = 0; i < 10000; i++)
	{
		wsprintf(file_name, "Corsairs3_%04d.tga", i);
		if(_access(file_name, 0) == -1) break;
	}
	//��������� ��������
	Dhdr.width = (unsigned short)screen_size.x; Dhdr.height = (unsigned short)screen_size.y;
	fh = fio->_CreateFile(file_name,GENERIC_WRITE,FILE_SHARE_READ,CREATE_ALWAYS);
	if(fh == INVALID_HANDLE_VALUE)
	{
		surface->UnlockRect();
		surface->Release();
		api->Trace("Can't create screenshot file");
		return;
	}
	fio->_WriteFile(fh,&Dhdr,sizeof(TGA_H),0);

	Surface = (DWORD*)lr.pBits;
	Surface += lr.Pitch*screen_size.y>>2;
	for(i=0;i<screen_size.y;i++)
	{
		Surface -= lr.Pitch>>2;
		fio->_WriteFile(fh,Surface,screen_size.x*4,0);
	}
	surface->UnlockRect();
	surface->Release();
	fio->_CloseHandle(fh);

	UNGUARD
}

PLANE * DX8RENDER::GetPlanes()
{
	FindPlanes(d3d8);
	return viewplane;
}

void DX8RENDER::FindPlanes(IDirect3DDevice9 * d3dDevice)
{
	D3DMATRIX m;
	CVECTOR v[4];
	GetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&m);
	//left
	v[0].x = m.m[0][0]; v[0].y = 0.0f; v[0].z = 1.0f;
	//right
	v[1].x = -m.m[0][0]; v[1].y = 0.0f; v[1].z = 1.0f;
	//top
	v[2].x = 0.0f; v[2].y = -m.m[1][1]; v[2].z = 1.0f;
	//bottom
	v[3].x = 0.0f; v[3].y = m.m[1][1]; v[3].z = 1.0f;
	v[0] = !v[0];
	v[1] = !v[1];
	v[2] = !v[2];
	v[3] = !v[3];

	GetTransform(D3DTS_VIEW, (D3DMATRIX *)&m);
	CVECTOR pos;

	pos.x = -m._41*m._11 - m._42*m._12 - m._43*m._13;
	pos.y = -m._41*m._21 - m._42*m._22 - m._43*m._23;
	pos.z = -m._41*m._31 - m._42*m._32 - m._43*m._33;

	viewplane[0].Nx = v[0].x*m._11 + v[0].y*m._12 + v[0].z*m._13;
	viewplane[0].Ny = v[0].x*m._21 + v[0].y*m._22 + v[0].z*m._23;
	viewplane[0].Nz = v[0].x*m._31 + v[0].y*m._32 + v[0].z*m._33;

	viewplane[1].Nx = v[1].x*m._11 + v[1].y*m._12 + v[1].z*m._13;
	viewplane[1].Ny = v[1].x*m._21 + v[1].y*m._22 + v[1].z*m._23;
	viewplane[1].Nz = v[1].x*m._31 + v[1].y*m._32 + v[1].z*m._33;

	viewplane[2].Nx = v[2].x*m._11 + v[2].y*m._12 + v[2].z*m._13;
	viewplane[2].Ny = v[2].x*m._21 + v[2].y*m._22 + v[2].z*m._23;
	viewplane[2].Nz = v[2].x*m._31 + v[2].y*m._32 + v[2].z*m._33;

	viewplane[3].Nx = v[3].x*m._11 + v[3].y*m._12 + v[3].z*m._13;
	viewplane[3].Ny = v[3].x*m._21 + v[3].y*m._22 + v[3].z*m._23;
	viewplane[3].Nz = v[3].x*m._31 + v[3].y*m._32 + v[3].z*m._33;

	viewplane[0].D = (pos.x*viewplane[0].Nx + pos.y*viewplane[0].Ny + pos.z*viewplane[0].Nz);
	viewplane[1].D = (pos.x*viewplane[1].Nx + pos.y*viewplane[1].Ny + pos.z*viewplane[1].Nz);
	viewplane[2].D = (pos.x*viewplane[2].Nx + pos.y*viewplane[2].Ny + pos.z*viewplane[2].Nz);
	viewplane[3].D = (pos.x*viewplane[3].Nx + pos.y*viewplane[3].Ny + pos.z*viewplane[3].Nz);
}

bool DX8RENDER::TechniqueSetParamsAndStart(const char *cBlockName, dword _dwNumParams, void *pParams)
{
	if (!cBlockName) return false;
	pTechnique->SetCurrentBlock(cBlockName,_dwNumParams,pParams);
	return pTechnique->ExecutePassStart();
}

bool _cdecl DX8RENDER::TechniqueExecuteStart(const char *cBlockName, dword _dwNumParams, ...)
{
	if (!cBlockName) return false;
	pTechnique->SetCurrentBlock(cBlockName,_dwNumParams,1 + &_dwNumParams);
	return pTechnique->ExecutePassStart();
}

bool DX8RENDER::TechniqueExecuteNext()
{
	return pTechnique->ExecutePassNext();
}

void DX8RENDER::DrawRects(RS_RECT *pRSR, dword dwRectsNum, const char *cBlockName, dword dwSubTexturesX, dword dwSubTexturesY, float fScaleX, float fScaleY, dword dwNumParams, ...)
{
	if (!pRSR || dwRectsNum==0 || !rectsVBuffer) return;

	bool		bDraw = true;

	static CMatrix		camMtx, IMatrix;
	d3d8->GetTransform(D3DTS_VIEW, camMtx);

	fScaleY *= GetHeightDeformator();

	float du, dv;
	bool bUseSubTextures = false;
	if(dwSubTexturesX > 1)
	{
		bUseSubTextures = true;
		du = 1.0f/dwSubTexturesX;
	}else{
		dwSubTexturesX = 1;
		du = 1.0f;
	}
	if(dwSubTexturesY > 1)
	{
		bUseSubTextures = true;
		dv = 1.0f/dwSubTexturesY;
	}else{
		dwSubTexturesY = 1;
		dv = 1.0f;
	}

	d3d8->SetTransform(D3DTS_VIEW,IMatrix);
	d3d8->SetTransform(D3DTS_WORLD,IMatrix);

	for(dword cnt = 0; cnt < dwRectsNum; )
	{
		//���������� �������� ��������������� �� ���
		dword drawCount = dwRectsNum;
		if(drawCount > rectsVBuffer_SizeInRects) drawCount = rectsVBuffer_SizeInRects;
		//�����
		RECT_VERTEX * data = null;
		if(rectsVBuffer->Lock(0, drawCount*6*sizeof(RECT_VERTEX), (VOID**)&data, D3DLOCK_DISCARD) != D3D_OK) return;
		if(!data) return;
		//��������� �����
		for(dword i = 0; i < drawCount; i++)
		{
			//��������� ������ ��������
			RECT_VERTEX * buffer = &data[i*6];
			RS_RECT & rect = pRSR[cnt++];
			CVECTOR pos = camMtx*(rect.vPos+vWordRelationPos);
			float sizex = rect.fSize*fScaleX;
			float sizey = rect.fSize*fScaleY;
			float sn = sinf(rect.fAngle);
			float cs = cosf(rect.fAngle);
			long color = rect.dwColor;
			float u1, v1, u2, v2;
			if(!bUseSubTextures)
			{
				v1 = u1 = 0.0f;
				v2 = u2 = 1.0f;
			}else{
				u1 = (rect.dwSubTexture % dwSubTexturesX)*du;
				v1 = ((rect.dwSubTexture / dwSubTexturesX) % dwSubTexturesY)*dv;
				u2 = u1 + du;
				v2 = v1 + dv;
			}
			//��������� ����� ��� ��������
			buffer[0].pos = pos + CVECTOR(sizex*(-cs - sn), sizey*(sn - cs), 0.0f);
			buffer[0].color = color;
			buffer[0].u = u1;
			buffer[0].v = v2;
			buffer[1].pos = pos + CVECTOR(sizex*(-cs + sn), sizey*(sn + cs), 0.0f);
			buffer[1].color = color;
			buffer[1].u = u1;
			buffer[1].v = v1;
			buffer[2].pos = pos + CVECTOR(sizex*(cs + sn), sizey*(-sn + cs), 0.0f);
			buffer[2].color = color;
			buffer[2].u = u2;
			buffer[2].v = v1;
			buffer[3].pos = buffer[0].pos;
			buffer[3].color = color;
			buffer[3].u = u1;
			buffer[3].v = v2;
			buffer[4].pos = buffer[2].pos;
			buffer[4].color = color;
			buffer[4].u = u2;
			buffer[4].v = v1;
			buffer[5].pos = pos + CVECTOR(sizex*(cs - sn), sizey*(-sn - cs), 0.0f);
			buffer[5].color = color;
			buffer[5].u = u2;
			buffer[5].v = v2;
		}
		//������ �����
		rectsVBuffer->Unlock();
		ErrorHandler("SetCamera::GetTransform -> SetFVF", SetFVF(RS_RECT_VERTEX_FORMAT));
		if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
		if (bDraw) do
		{
			ErrorHandler("SetCamera::GetTransform -> SetStreamSource", SetStreamSource(0, rectsVBuffer, sizeof(RECT_VERTEX)));
			ErrorHandler("SetCamera::GetTransform -> DrawPrimitive", DrawPrimitive(D3DPT_TRIANGLELIST, 0, drawCount*2));
		} while (cBlockName && TechniqueExecuteNext());
	}

	d3d8->SetTransform(D3DTS_VIEW, camMtx);
}

void DX8RENDER::DrawSprites(RS_SPRITE * pRSS, dword dwSpritesNum, const char * cBlockName, dword dwNumParams, ...)
{
	dword	i;
#define RS_SPRITE_VERTEX_FORMAT	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
	if (dwSpritesNum==0) return;

	WORD * pIndices = NEW WORD[dwSpritesNum * 6];

	for (i=0; i<dwSpritesNum; i++)
	{
		pIndices[i*6 + 0] = WORD(i * 4 + 0); pIndices[i * 6 + 1] = WORD(i * 4 + 3); pIndices[i * 6 + 2] = WORD(i * 4 + 2);
		pIndices[i*6 + 3] = WORD(i * 4 + 0); pIndices[i * 6 + 4] = WORD(i * 4 + 2); pIndices[i * 6 + 5] = WORD(i * 4 + 1);
	}

	SetFVF(RS_SPRITE_VERTEX_FORMAT);

	bool bDraw = true;
	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName, dwNumParams, 1 + &dwNumParams);
	if (bDraw) do
	{
#ifndef _XBOX
		DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, dwSpritesNum * 4, dwSpritesNum * 2, pIndices, D3DFMT_INDEX16, pRSS, sizeof(RS_SPRITE));
#else
		DrawPrimitiveUP(D3DPT_QUADLIST, RS_SPRITE_VERTEX_FORMAT, dwSpritesNum, pRSS, sizeof(RS_SPRITE));
#endif
	} while (cBlockName && TechniqueExecuteNext());
	delete pIndices;
}

void DX8RENDER::DrawLines(RS_LINE *pRSL, dword dwLinesNum, const char *cBlockName, dword dwNumParams, ...)
{
	if (!pRSL || dwLinesNum==0) return;

#define RS_LINE_VERTEX_FORMAT	(D3DFVF_XYZ|D3DFVF_DIFFUSE)

	bool		bDraw = true;

	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		DrawPrimitiveUP(D3DPT_LINELIST,RS_LINE_VERTEX_FORMAT,dwLinesNum,pRSL,sizeof(RS_LINE));
	} while (cBlockName && TechniqueExecuteNext());
}

void DX8RENDER::DrawLines2D(RS_LINE2D *pRSL2D, dword dwLinesNum, const char *cBlockName, dword dwNumParams, ...)
{
	if (!pRSL2D || dwLinesNum==0) return;

#define RS_LINE2D_VERTEX_FORMAT	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

	bool		bDraw = true;

	if (cBlockName) bDraw = TechniqueSetParamsAndStart(cBlockName,dwNumParams,1 + &dwNumParams);
	if (bDraw) do
	{
		DrawPrimitiveUP(D3DPT_LINELIST,RS_LINE2D_VERTEX_FORMAT,dwLinesNum,pRSL2D,sizeof(RS_LINE2D));
	} while (cBlockName && TechniqueExecuteNext());
}


//-----------------------
HRESULT DX8RENDER::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer)
{
	return d3d8->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, NULL);
}

HRESULT DX8RENDER::VBLock(IDirect3DVertexBuffer9 * pVB, UINT OffsetToLock,UINT SizeToLock,BYTE** ppbData, DWORD Flags)
{
	dwNumLV++;
	return pVB->Lock(OffsetToLock, SizeToLock, (VOID**)ppbData, Flags);
}

void DX8RENDER::VBUnlock(IDirect3DVertexBuffer9 * pVB)
{
	pVB->Unlock();
}

HRESULT DX8RENDER::SetFVF(DWORD handle)
{
	return d3d8->SetFVF(handle);
}

HRESULT DX8RENDER::SetStreamSource(UINT StreamNumber, void * pStreamData, UINT Stride)
{
	return d3d8->SetStreamSource(StreamNumber,(IDirect3DVertexBuffer9*)pStreamData, 0, Stride);
}

HRESULT DX8RENDER::SetIndices(void * pIndexData)
{
	return d3d8->SetIndices((IDirect3DIndexBuffer9*)pIndexData);
}

HRESULT DX8RENDER::DrawPrimitive(D3DPRIMITIVETYPE dwPrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	dwNumDrawPrimitive++;
	return d3d8->DrawPrimitive(dwPrimitiveType, StartVertex, PrimitiveCount);
}

#ifndef _XBOX
HRESULT DX8RENDER::Release(IUnknown *pSurface)
{
	if (pSurface) return pSurface->Release();
	return D3D_OK;
}
#else
HRESULT DX8RENDER::Release(IDirect3DResource8 * pSurface)
{
	if (pSurface) return pSurface->Release();
	return D3D_OK;
}
#endif

HRESULT DX8RENDER::GetRenderTarget(IDirect3DSurface9** ppRenderTarget)
{
	return d3d8->GetRenderTarget(0, ppRenderTarget);
}

HRESULT DX8RENDER::GetDepthStencilSurface( IDirect3DSurface9** ppZStencilSurface )
{
	return d3d8->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT DX8RENDER::GetCubeMapSurface( IDirect3DCubeTexture9* ppCubeTexture, D3DCUBEMAP_FACES FaceType, UINT Level, IDirect3DSurface9** ppCubeMapSurface )
{
	return ppCubeTexture->GetCubeMapSurface( FaceType, Level, ppCubeMapSurface);
}

HRESULT DX8RENDER::SetRenderTarget( IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pNewZStencil )
{
	d3d8->SetRenderTarget(0, pRenderTarget);
	return d3d8->SetDepthStencilSurface(pNewZStencil);
}

HRESULT DX8RENDER::Clear( DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil )
{
	return d3d8->Clear( Count, pRects, Flags, Color, Z, Stencil );
}

HRESULT DX8RENDER::BeginScene()
{
	return d3d8->BeginScene();
}

HRESULT DX8RENDER::EndScene()
{
	return d3d8->EndScene();
}

HRESULT DX8RENDER::SetClipPlane( DWORD Index, CONST float* pPlane )
{
	//return d3d8->SetClipPlane( Index, pPlane );
	return D3D_OK;
}

HRESULT DX8RENDER::CreateTexture( UINT Width, UINT Height, UINT  Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture )
{
	return d3d8->CreateTexture( Width, Height, Levels, Usage, Format, Pool, ppTexture, NULL );
}

HRESULT DX8RENDER::CreateCubeTexture( UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture )
{
	return d3d8->CreateCubeTexture( EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, NULL );
}

HRESULT DX8RENDER::CreateImageSurface( UINT Width, UINT Height, D3DFORMAT Format, IDirect3DSurface9 * * ppSurface)
{
	return d3d8->CreateOffscreenPlainSurface( Width, Height, Format, D3DPOOL_SCRATCH, ppSurface, NULL);
}

HRESULT DX8RENDER::CreateDepthStencilSurface( UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, IDirect3DSurface9** ppSurface )
{
	return d3d8->CreateDepthStencilSurface( Width, Height, Format, MultiSample, 0, TRUE, ppSurface, NULL );
}

HRESULT DX8RENDER::CreateVertexShader(CONST DWORD * pDeclaration, CONST DWORD * pFunction, DWORD * pHandle, DWORD Usage )
{
	return d3d8->CreateVertexShader( pDeclaration, pFunction, pHandle, Usage );
}

HRESULT DX8RENDER::CreatePixelShader(CONST DWORD * pFunction, DWORD * pHandle)
{
#ifndef _XBOX
	return d3d8->CreatePixelShader( pFunction, pHandle );
#else
	return d3d8->CreatePixelShader( (D3DPIXELSHADERDEF*)(pFunction /*+ 1*/), pHandle );
#endif
}

HRESULT DX8RENDER::GetVertexShader(DWORD * pHandle)
{
	return d3d8->GetVertexShader(pHandle);
}

HRESULT DX8RENDER::GetPixelShader(DWORD * pHandle)
{
	return d3d8->GetPixelShader(pHandle);
}

HRESULT DX8RENDER::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture )
{
	return d3d8->SetTexture(Stage, pTexture );
}

HRESULT DX8RENDER::GetLevelDesc( IDirect3DTexture9* ppTexture, UINT Level, D3DSURFACE_DESC* pDesc )
{
	return ppTexture->GetLevelDesc( Level, pDesc );
}

HRESULT DX8RENDER::GetLevelDesc( IDirect3DCubeTexture9* ppCubeTexture, UINT Level, D3DSURFACE_DESC* pDesc )
{
	return ppCubeTexture->GetLevelDesc( Level, pDesc );
}

HRESULT DX8RENDER::LockRect( IDirect3DCubeTexture9* ppCubeTexture, D3DCUBEMAP_FACES FaceType, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags )
{
	return ppCubeTexture->LockRect( FaceType, Level, pLockedRect, pRect, Flags );
}

HRESULT DX8RENDER::LockRect( IDirect3DTexture9* ppTexture, UINT Level, D3DLOCKED_RECT* pLockedRect, CONST RECT* pRect, DWORD Flags )
{
	return ppTexture->LockRect( Level, pLockedRect, pRect, Flags );
}

HRESULT DX8RENDER::UnlockRect( IDirect3DCubeTexture9 *pCubeTexture, D3DCUBEMAP_FACES FaceType, UINT Level )
{
	return pCubeTexture->UnlockRect( FaceType, Level );
}

HRESULT DX8RENDER::UnlockRect( IDirect3DTexture9 *pTexture, UINT Level )
{
	return pTexture->UnlockRect( Level );
}

HRESULT DX8RENDER::GetSurfaceLevel( IDirect3DTexture9* ppTexture, UINT Level, IDirect3DSurface9** ppSurfaceLevel )
{
	return ppTexture->GetSurfaceLevel( Level, ppSurfaceLevel );
}

HRESULT DX8RENDER::CopyRects( IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRectsArray, UINT cRects, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPointsArray )
{
	return d3d8->UpdateSurface( pSourceSurface, pSourceRectsArray, pDestinationSurface, pDestPointsArray );
}

HRESULT DX8RENDER::DeletePixelShader( DWORD Handle )
{
	return d3d8->DeletePixelShader( Handle );
}

HRESULT DX8RENDER::DeleteVertexShader( DWORD Handle )
{
	return d3d8->DeleteVertexShader( Handle );
}

HRESULT DX8RENDER::SetPixelShader( DWORD Handle )
{
	return d3d8->SetPixelShader( Handle );
}

HRESULT DX8RENDER::SetFVFConstant(DWORD Register, CONST void* pConstantData, DWORD  ConstantCount )
{
	return d3d8->SetFVFConstant( Register, pConstantData, ConstantCount );
}

HRESULT DX8RENDER::SetPixelShaderConstant( DWORD Register, CONST void* pConstantData, DWORD ConstantCount )
{
	return d3d8->SetPixelShaderConstant( Register, pConstantData, ConstantCount );
}

HRESULT DX8RENDER::GetDeviceCaps(D3DCAPS9 * pCaps)
{
	return d3d8->GetDeviceCaps(pCaps);
}

CVideoTexture* DX8RENDER::GetVideoTexture(char* sVideoName)
{
	if(sVideoName==null) return null;
	CVideoTexture *retVal = null;
	VideoTextureEntity *pVTLcur = pVTL;

	// check already loaded
	unsigned long newHash = hash_string(sVideoName);
	while(pVTLcur!=null)
	{
		if(pVTLcur->hash==newHash && stricmp(pVTLcur->name,sVideoName)==0)
		{
			if( api->ValidateEntity(&pVTLcur->videoTexture_id) )
			{
				pVTLcur->ref++;
				return pVTLcur->VideoTexture;
			}
			VideoTextureEntity *pVTmp = pVTLcur;
			pVTLcur = pVTLcur->next;
			if(pVTmp->name) delete pVTmp->name;
			delete pVTmp;
			continue;
		}
		pVTLcur = pVTLcur->next;
	}

	// create new video texture
	pVTLcur = NEW VideoTextureEntity;
	if(pVTLcur==null)
	{
		_THROW("memory allocate error")
	}
	pVTLcur->next = pVTL;
	pVTLcur->VideoTexture = 0;
	pVTLcur->hash = newHash;
	pVTLcur->ref = 1;
	if( (pVTLcur->name=NEW char[strlen(sVideoName)+1]) == null )
	{
		_THROW("memory allocate error")
	}
	strcpy(pVTLcur->name,sVideoName);
	ENTITY_ID ei;
	api->CreateEntity(&ei,"TextureSequence");
	pVTLcur->VideoTexture =	(CVideoTexture*)api->GetEntityPointer(&ei);
	if(pVTLcur->VideoTexture != null)
	{
		pVTLcur->videoTexture_id = ei;
		if( pVTLcur->VideoTexture->Initialize(this,sVideoName,true) == null ) {
			delete pVTLcur;
			api->DeleteEntity(ei);
		} else {
			pVTL=pVTLcur;
			retVal = pVTLcur->VideoTexture;
		}
	}
	else
	{
		delete pVTLcur;
		api->DeleteEntity(ei);
	}

	return retVal;
}

void DX8RENDER::ReleaseVideoTexture(CVideoTexture* pVTexture)
{
	VideoTextureEntity *cur = pVTL;
	VideoTextureEntity *prev = null;

	if( cur!=null ) do
	{
		if(cur->VideoTexture == pVTexture)
		{
			cur->ref--;
			if(cur->ref>0) return;
			if(prev==null)
				pVTL = cur->next;
			else
				prev->next = cur->next;
			api->DeleteEntity(cur->videoTexture_id);
			if(cur->name!=NULL) delete cur->name;
			delete cur;
			break;
		}
		prev=cur;
	} while((cur=cur->next)!=null);
}

void DX8RENDER::PlayToTexture()
{
	VideoTextureEntity *cur = pVTL;
	while(cur!=null)
	{
		if(api->ValidateEntity(&pVTL->videoTexture_id))
		{
			cur->VideoTexture->FrameUpdate();
			cur = cur->next;
		}
		else
		{
			api->Trace("ERROR: void DX8RENDER::PlayToTexture()");
			if(cur->name!=NULL) delete cur->name;
			VideoTextureEntity * pcur = cur;
			cur = cur->next;
			if(pVTL == pcur) pVTL = cur;
			delete pcur;
		}
	}
}

HRESULT DX8RENDER::ImageBlt(long TextureID, RECT * pDstRect, RECT * pSrcRect)
{
	struct F3DVERTEX
	{
		float x,y,z,rhw;
		float tu,tv;
	};
	RECT dr;
	F3DVERTEX v[6];
	HRESULT hRes;

	if(pDstRect)
	{
		dr = *pDstRect;
	}
	else
	{
		dr.left = 0;
		dr.top = 0;
		dr.right = screen_size.x-1;
		dr.bottom = screen_size.y-1;
	}

	for(DWORD n=0;n<6;n++)
	{
		v[n].rhw = 1.0f;
		v[n].z = 0.5f;
	}

	v[0].x = (float)dr.left;	v[0].y = (float)dr.top;
	v[0].tu = 0.0f;				v[0].tv = 0.0f;

	v[1].x = (float)dr.left;	v[1].y = (float)dr.bottom;
	v[1].tu = 0.0f;				v[1].tv = 1.0f;

	v[2].x = (float)dr.right;	v[2].y = (float)dr.top;
	v[2].tu = 1.0f;				v[2].tv = 0.0f;

	v[3].x = (float)dr.right;	v[3].y = (float)dr.top;
	v[3].tu = 1.0f;				v[3].tv = 0.0f;

	v[4].x = (float)dr.left;	v[4].y = (float)dr.bottom;
	v[4].tu = 0.0f;				v[4].tv = 1.0f;

	v[5].x = (float)dr.right;	v[5].y = (float)dr.bottom;
	v[5].tu = 1.0f;				v[5].tv = 1.0f;


	TextureSet(0,TextureID);

	bool bDraw = TechniqueExecuteStart("texturedialogfon");
	if (bDraw) do
	{
		d3d8->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXTUREFORMAT2);
		hRes = d3d8->DrawPrimitiveUP(D3DPT_TRIANGLELIST,2,&v,sizeof(F3DVERTEX));
		dwNumDrawPrimitive++;
	} while (TechniqueExecuteNext());

	return hRes;

}


HRESULT DX8RENDER::ImageBlt(const char * pName, RECT * pDstRect, RECT * pSrcRect)
{
	long TextureID;
	TextureID = TextureCreate(pName);
	HRESULT hRes = ImageBlt(TextureID,pDstRect,pSrcRect);
	TextureRelease(TextureID);
	SetProgressImage(pName);
	return hRes;
}

void DX8RENDER::SetProgressImage(const char * image)
{
	if(!image || !image[0])
	{
		if(progressImageSize > 0 && progressImage) progressImage[0] = 0;
		return;
	}
	long s = strlen(image) + 1;
	if(s > progressImageSize)
	{
		progressImageSize = s;
		if(progressImage) delete progressImage;
		progressImage = new char[progressImageSize];
	}
	strcpy(progressImage, image);
}

void DX8RENDER::SetTipsImage(const char * image)
{
	if(!image || !image[0])
	{
		if(progressTipsImageSize > 0 && progressTipsImage) progressTipsImage[0] = 0;
		return;
	}
	long s = strlen(image) + 1;
	if(s > progressTipsImageSize)
	{
		progressTipsImageSize = s;
		if(progressTipsImage) delete progressTipsImage;
		progressTipsImage = NEW char[progressTipsImageSize];
	}
	strcpy(progressTipsImage, image);
}

char * DX8RENDER::GetTipsImage()
{
	return progressTipsImage;
}

void DX8RENDER::StartProgressView()
{
	progressSafeCounter = 0;
	if(progressTexture < 0)
	{
		//��������� ��������
		loadFrame = 0;
		isInPViewProcess = true;
		long t = TextureCreate("Loading\\progress.tga");
		isInPViewProcess = false;
		if(t < 0)
		{
			api->Trace("Progress error!");
			return;
		}
		progressTexture = t;
	}else return;
	//��������� ������� ��������
	if(progressImage && progressImage[0])
	{
		isInPViewProcess = true;
		if(backTexture >= 0) TextureRelease(backTexture);
		backTexture = -1;
		backTexture = TextureCreate(progressImage);
		isInPViewProcess = false;
	}else{
		backTexture = -1;
	}
	//��������� �����
	if(progressTipsImage && progressTipsImage[0])
	{
		isInPViewProcess = true;
		if(progressTipsTexture >= 0) TextureRelease(progressTipsTexture);
		progressTipsTexture = -1;
		progressTipsTexture = TextureCreate(progressTipsImage);
		isInPViewProcess = false;
	}
	progressUpdateTime = GetTickCount() - 1000;
}


void DX8RENDER::ProgressView()
{
	//�������� ��������
 	if(progressTexture < 0) return;
	if(isInPViewProcess) return;
	//����������� �����
	dword time = GetTickCount();
	if(abs((long)(progressUpdateTime - time)) < 50) return;
	progressUpdateTime = time;
	isInPViewProcess = true;
	progressSafeCounter = 0;
	//����� ���������
	if(!bInsideScene) BeginScene();
	//��������� ������� ��������
	struct LoadVertex
	{
		float x, y, z, rhw;
		dword color;
		float u, v;
	} v[4];
	for(long i = 0; i < 4; i++)
	{
		v[i].z = 0.5;
		v[i].rhw = 2.0;
		v[i].color = 0xffffffff;
	}
	//��������� ������������� � ������� ����� ��������
	D3DVIEWPORT9 vp;
	GetViewport(&vp);
	v[0].x = 0.0f; v[0].y = 0.0f;
	v[1].x = float(vp.Width); v[1].y = 0.0f;
	v[2].x = 0.0f; v[2].y = float(vp.Height);
	v[3].x = float(vp.Width); v[3].y = float(vp.Height);
	v[0].u = 0.0f; v[0].v = 0.0f;
	v[1].u = 1.0f; v[1].v = 0.0f;
	v[2].u = 0.0f; v[2].v = 1.0f;
	v[3].u = 1.0f; v[3].v = 1.0f;
	TextureSet(0, backTexture);
	if(backTexture < 0) for(long i = 0; i < 4; i++) v[i].color = 0;
	if(backTexture >= 0 && progressTipsTexture >= 0)
	{
		TextureSet(1, progressTipsTexture);
		DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, 2, v, sizeof(v[0]), "ProgressBackTechWithTips");
	}else{
		DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, 2, v, sizeof(v[0]), "ProgressBackTech");
	}
	if(backTexture < 0) for(long i = 0; i < 4; i++) v[i].color = 0xffffffff;
	//������������� ������
	CVECTOR pos(vp.Width*progressFramesPosX, vp.Height*progressFramesPosY, 0.0f);
	CVECTOR size(vp.Width*progressFramesWidth, vp.Width*progressFramesHeight*m_fHeightDeformator, 0.0f);
	v[0].x = pos.x; v[0].y = pos.y;
	v[1].x = pos.x + size.x + 0.5f; v[1].y = pos.y;
	v[2].x = pos.x; v[2].y = pos.y + size.y + 0.5f;
	v[3].x = pos.x + size.x + 0.5f; v[3].y = pos.y + size.y + 0.5f;
	//������ ����� ������
	long sizeX = progressFramesCountX;
	long sizeY = progressFramesCountY;
	//������� �������� �����
	long fx = loadFrame % sizeX;
	long fy = loadFrame / sizeY;
	v[0].u = fx/float(sizeX); v[0].v = fy/float(sizeY);
	v[1].u = (fx + 1.0f)/float(sizeX); v[1].v = fy/float(sizeY);
	v[2].u = fx/float(sizeX); v[2].v = (fy + 1.0f)/float(sizeY);
	v[3].u = (fx + 1.0f)/float(sizeX); v[3].v = (fy + 1.0f)/float(sizeY);
	//������
	TextureSet(0, progressTexture);
	DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1, 2, v, sizeof(v[0]), "ProgressTech");
	EndScene();
	d3d8->Present(NULL, NULL, NULL, NULL);
	if(bInsideScene) BeginScene();
	//��������� ����
	loadFrame++;
	if(loadFrame >= sizeX*sizeY) loadFrame = 0;
	//�������
	isInPViewProcess = false;
}


void DX8RENDER::EndProgressView()
{
	if(progressTexture >= 0) TextureRelease(progressTexture);
	progressTexture = -1;
	if(backTexture >= 0) TextureRelease(backTexture);
	backTexture = -1;
	if(progressTipsTexture >= 0) TextureRelease(progressTipsTexture);
	progressTipsTexture = -1;
	if(progressImage && progressImageSize > 0) progressImage[0] = 0;
	if(progressTipsImage && progressTipsImageSize > 0) progressTipsImage[0] = 0;
}

void DX8RENDER::SetColorParameters(float fGamma, float fBrightness, float fContrast)
{
	D3DGAMMARAMP ramp;

	for (dword i=0; i<256; i++)
	{
		float fRamp = fContrast * 255.0f * 256.0f * powf(float(i / 255.0f), 1.0f / fGamma) + fBrightness * 256.0f;
		if (fRamp < 0.0f) fRamp = 0.0f;
		if (fRamp > 65535.0f) fRamp = 65535.0f;
#ifdef _XBOX
		ramp.red[i] = BYTE(fRamp / 256.0f);
		ramp.green[i] = BYTE(fRamp / 256.0f);
		ramp.blue[i] = BYTE(fRamp / 256.0f);
#else
		ramp.red[i] = WORD(fRamp);
		ramp.green[i] = WORD(fRamp);
		ramp.blue[i] = WORD(fRamp);
#endif
	}
	d3d8->SetGammaRamp(0, D3DSGR_NO_CALIBRATION , &ramp);
}

void DX8RENDER::MakeDrawVector(RS_LINE * pLines, dword dwNumSubLines, const CMatrix & mMatrix, CVECTOR vUp, CVECTOR v1, CVECTOR v2, float fScale, dword dwColor)
{
	dword i;
    dword k;

	//for (i=0; i<dwNumSubLines * 2 + 2; i++) pLines[i].dwColor = dwColor;
	k = dwNumSubLines * 2 + 2; // boal �����������, ���� ��� ������ ���������� ������ ������.
	for (i=0; i<k; i++)
	{
		pLines[i].dwColor = dwColor;
	}
	pLines[0].vPos = v1;
	pLines[1].vPos = v1 + (fScale * v2);

	float fRadius = 0.03f * fScale;
	float fDist = 0.85f * fScale * sqrtf(~v2);

	for (i=0; i<dwNumSubLines; i++)
	{
		float fAng = PIm2 * float(i) / float(dwNumSubLines);

		float x = fRadius * sinf(fAng);
		float z = fRadius * cosf(fAng);

		CVECTOR vRes;

		if (fabsf(vUp.x) < 1e-5f) vRes = CVECTOR(fDist, x, z);
		if (fabsf(vUp.y) < 1e-5f) vRes = CVECTOR(x, fDist, z);
		if (fabsf(vUp.z) < 1e-5f) vRes = CVECTOR(x, z, fDist);
		vRes = (CMatrix&)mMatrix * vRes;
		pLines[2 + i * 2 + 0].vPos = vRes;
		pLines[2 + i * 2 + 1].vPos = pLines[1].vPos;
	}
}

void DX8RENDER::DrawVector(const CVECTOR & v1, const CVECTOR & v2, dword dwColor, const char * pTechniqueName, dword dwNumParams, ...)
{
	RS_LINE lines[51 * 2];
	CMatrix	mView;

	float fScale = sqrtf(~(v2 - v1));
	if (!mView.BuildViewMatrix(v1, v2, CVECTOR(0.0f, 1.0f, 0.0f)))
		if (!mView.BuildViewMatrix(v1, v2, CVECTOR(1.0f, 0.0f, 0.0f))) return;

	mView.Transposition();

	MakeDrawVector(&lines[0], 50, mView, CVECTOR(1.0f, 1.0f, 0.0f), mView.Pos(), mView.Vz(), fScale, dwColor);

	CMatrix mWorldSave;
	GetTransform(D3DTS_WORLD, mWorldSave);
	SetTransform(D3DTS_WORLD, CMatrix());
	DrawLines(lines, 51, pTechniqueName, 0);
	SetTransform(D3DTS_WORLD, mWorldSave);
}

void DX8RENDER::DrawSphere(const CVECTOR & vPos, float fRadius, dword dwColor)
{
	CMatrix m;
	m.BuildPosition(vPos.x, vPos.y, vPos.z);
	m.m[0][0] = fRadius;
	m.m[1][1] = fRadius;
	m.m[2][2] = fRadius;

	SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);
	SetTransform(D3DTS_WORLD, m);
	DrawPrimitiveUP(D3DPT_TRIANGLELIST, D3DFVF_XYZ | D3DFVF_DIFFUSE, DX8sphereNumTrgs, DX8sphereVertex, sizeof(DX8SphVertex), "DX8Sphere");
}

void DX8RENDER::SetLoadTextureEnable(bool bEnable)
{
	bLoadTextureEnabled = bEnable;
}

IDirect3DBaseTexture9 * DX8RENDER::CreateTextureFromFileInMemory(const char * pFile, dword dwSize)
{
	if (!pFile || !dwSize) return null;

	IDirect3DTexture9 * pTexture = null;
	TGA_H * pTga = (TGA_H *)pFile;
	D3DFORMAT d3dFormat = (pTga->bpp == 16) ? D3DFMT_DXT1 : D3DFMT_DXT3;
	D3DXCreateTextureFromFileInMemoryEx((LPDIRECT3DDEVICE9)GetD3DDevice(), pFile, dwSize, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, d3dFormat, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, null, null, &pTexture);

	return pTexture;
}

IDirect3DVolumeTexture9 * DX8RENDER::CreateVolumeTexture(dword Width, dword Height, dword Depth, dword Levels, dword Usage, D3DFORMAT Format, D3DPOOL Pool)
{
	IDirect3DVolumeTexture9 * pVolumeTexture = null;
	d3d8->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, &pVolumeTexture, NULL);
	return pVolumeTexture;
}

bool DX8RENDER::PushRenderTarget()
{
	IDirect3DSurface9 * pD3DRenderTarget = null, * pD3DDepthSurface = null;
	D3DVIEWPORT9 vp;

	GetViewport(&vp);

	stRenderTarget.Push();
	stRenderTarget.Top().pRenderTarget = null;
	stRenderTarget.Top().pDepthSurface = null;
	stRenderTarget.Top().ViewPort = vp;

	GetRenderTarget(&stRenderTarget.Top().pRenderTarget);
	d3d8->GetDepthStencilSurface(&stRenderTarget.Top().pDepthSurface);

	return true;
}

bool DX8RENDER::PopRenderTarget()
{
	if (!stRenderTarget.Size())
	{
		api->Trace("DX8Error: Try to pop RenderTarget, but RenderTarget stack is empty");
		return false;
	}


	SetRenderTarget(stRenderTarget.Top().pRenderTarget, stRenderTarget.Top().pDepthSurface);

	SetViewport(&stRenderTarget.Top().ViewPort);

	RELEASE(stRenderTarget.Top().pRenderTarget);
	RELEASE(stRenderTarget.Top().pDepthSurface);

	stRenderTarget.Pop();
	return true;
}

bool DX8RENDER::SetRenderTarget(IDirect3DCubeTexture9 * pRenderTarget, dword FaceType, dword dwLevel, IDirect3DSurface9 * pZStencil)
{
	IDirect3DSurface9 * pSurface;
	pRenderTarget->GetCubeMapSurface((D3DCUBEMAP_FACES)FaceType, dwLevel, &pSurface);
	bool bSuccess = D3D_OK == SetRenderTarget(pSurface, pZStencil);
	Release(pSurface);
	return bSuccess;
}

void DX8RENDER::SetView(const CMatrix & mView)				{ SetTransform(D3DTS_VIEW, mView); }
void DX8RENDER::SetWorld(const CMatrix & mWorld)			{ SetTransform(D3DTS_WORLD, mWorld); }
void DX8RENDER::SetProjection(const CMatrix & mProjection)	{ SetTransform(D3DTS_PROJECTION, mProjection); }

const CMatrix & DX8RENDER::GetView()
{
	GetTransform(D3DTS_VIEW, mView);
	return mView;
}

const CMatrix & DX8RENDER::GetWorld()
{
	GetTransform(D3DTS_WORLD, mWorld);
	return mWorld;
}

const CMatrix & DX8RENDER::GetProjection()
{
	GetTransform(D3DTS_PROJECTION, mProjection);
	return mProjection;
}

IDirect3DBaseTexture9* DX8RENDER::GetTextureFromID(long nTextureID)
{
	if( nTextureID<0 ) return null;
	return Textures[nTextureID].d3dtex;
}