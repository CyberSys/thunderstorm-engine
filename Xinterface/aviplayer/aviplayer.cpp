#include <stdio.h>
#include "aviplayer.h"
#include "../../Common_h/defines.h"
#include "../../Shared/interface/messages.h"

#pragma comment(lib,"amstrmid.lib")
#pragma comment(lib,"ddraw.lib")

#define IRELEASE(i)	{if(i) i->Release(); i=NULL;}

#define VIDEO_DIRECTORY		"resource\\videos"

long AVI_GetTextureSize(long width)
{
	long i=2;
	while((width>>=1L)>0)
		i<<=1L;
	return i;
}

CAviPlayer::CAviPlayer()
{
	m_bContinue = true;
	m_bShowVideo = true;

    pDD = nullptr;
    pPrimarySurface = nullptr;
	pVideoSurface	= nullptr;
	pAMStream = nullptr;
	pPrimaryVidStream = nullptr;
	pDDStream = nullptr;
	pSample = nullptr;

	pTmpRenderTarget = nullptr;
	pTex = nullptr;
	m_bFirstDraw = true;
	m_bMakeUninitializeDD = false;
}

CAviPlayer::~CAviPlayer()
{
	ReleaseAll();
}

bool CAviPlayer::Init()
{
	if( (rs=(VDX9RENDER *)_CORE_API->CreateService("dx9render")) == nullptr )
	{
		STORM_THROW("Can`t create render service");
	}

	api->LayerCreate("vRealize",true,false);
	api->LayerSetRealize("vRealize",true);
	api->LayerAdd("vRealize",GetID(),-1);

	api->LayerCreate("vExecute",true,false);
	api->LayerSetExecute("vExecute",true);
	api->LayerAdd("vExecute",GetID(),1);

	if(!GetInterfaces())
	{
		m_bContinue = false;
		CleanupInterfaces();
	}

	return true;
}

void CAviPlayer::Execute(uint32_t delta_time)
{
	if(m_bContinue==false)
	{
		if(pAMStream!= nullptr)
			pAMStream->SetState(STREAMSTATE_STOP);
		CleanupInterfaces();
		api->Event("ievntEndVideo");
		return;
	}
}

void CAviPlayer::Realize(uint32_t delta_time)
{
	//~!~
	//rs->BeginScene();
	int i;
	HRESULT hr;
	DDSURFACEDESC ddsd;
	ddsd.dwSize = sizeof(ddsd);
	if(pSample== nullptr)
	{
		m_bContinue = false;
		return;
	}

	if(m_bFirstDraw)
	{
		rs->Clear( 0, nullptr,D3DCLEAR_TARGET,0,0,0);
		m_bFirstDraw = false;
	}

	if( (hr=pSample->Update(0, nullptr, nullptr, NULL)) == S_OK )
	{
		hr = pVideoSurface->Lock(nullptr, &ddsd, 0, nullptr );
		if(hr!=S_OK) return;

		D3DLOCKED_RECT d3dlkRect;
		hr = pTex->LockRect(0,&d3dlkRect,&lockRect,0);
		if(hr!=S_OK) return;

		int nPitch = d3dlkRect.Pitch;
		char * pOutData = (char*)d3dlkRect.pBits;
		char * pInData = (char*)ddsd.lpSurface;
		long copySize = ddsd.lPitch<d3dlkRect.Pitch ? ddsd.lPitch : d3dlkRect.Pitch;
		for(i=0; i<(int)ddsd.dwHeight; i++)
		{
			memcpy(pOutData,pInData,copySize);
			pInData += ddsd.lPitch;
			pOutData += d3dlkRect.Pitch;
		}

		pTex->UnlockRect(0);
		pVideoSurface->Unlock(nullptr);

		if(m_bShowVideo)
		{
			rs->SetTexture(0,pTex);
			rs->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,XI_AVIVIDEO_FVF,2,v,sizeof(XI_AVIVIDEO_VERTEX),"battle_icons");
		}
    }
	else
	{
		m_bContinue=false;
	}
}

uint32_t _cdecl CAviPlayer::ProcessMessage(MESSAGE & message)
{
	switch(message.Long())
	{
	case MSG_SET_VIDEO_PLAY:
		{
			char param[256];
			char vidName[256];
			message.String(sizeof(param)-1,param);
			sprintf_s(vidName,"%s\\%s",VIDEO_DIRECTORY,param);
			if( !PlayMedia(vidName) )
			{
				CleanupInterfaces();
				api->PostEvent("ievntEndVideo",1, nullptr);
			}
		}
		break;
	}
	return 0;
}

bool CAviPlayer::PlayMedia(char * fileName)
{
	HRESULT hr = S_OK;
    DDSURFACEDESC ddsd;

	WCHAR    wPath[MAX_PATH];        // wide (32-bit) string name
	MultiByteToWideChar(CP_ACP, 0, fileName, -1, wPath,
		sizeof(wPath)/sizeof(wPath[0]));    

	if(pAMStream== nullptr) return false;

	hr = pAMStream->OpenFile(wPath, 0);
    if (FAILED(hr)) {
		api->Trace("Video Error!!!(0x%8x) Can`t load video file = %s",hr,fileName);
        return false;
    }

    hr = pAMStream->GetMediaStream(MSPID_PrimaryVideo, &pPrimaryVidStream);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t get media stream");
        return false;
    }
    hr = pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&pDDStream);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t query interface DirectDrawMediaStream");
        return false;
    }
    ddsd.dwSize = sizeof(ddsd);
    hr = pDDStream->GetFormat(&ddsd, nullptr, nullptr, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t get stream format");
        return false;
    }
	long srcWidth = ddsd.dwWidth;
	long srcHeight = ddsd.dwHeight;
	hr = pDD->CreateSurface(&ddsd, &pVideoSurface, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t create surface for video imaging");
        return false;
    }

	lockRect.left = 0;			lockRect.top = 0;
	lockRect.right = srcWidth;	lockRect.bottom = srcHeight;

	hr = pDDStream->CreateSample((IDirectDrawSurface*)pVideoSurface, nullptr, 0, &pSample);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t create sample for this video");
        return false;
    }

	RECT dstRect;
	GetWindowRect(api->GetAppHWND(),&dstRect);
	long dstWidth = dstRect.right - dstRect.left;
	long dstHeight = dstRect.bottom - dstRect.top;

	float horzK = (float)dstWidth/srcWidth;
	float vertK = (float)dstHeight/srcHeight;
	if(horzK<vertK)	vertK=horzK;
	else			horzK=vertK;

	dstRect.left = dstRect.top = 0;
	dstRect.right = (long)(srcWidth*horzK+.5f);
	dstRect.bottom = (long)(srcHeight*vertK+.5f);

	dstRect.left += (dstWidth-dstRect.right)/2;
	dstRect.top += (dstHeight-dstRect.bottom)/2;
	dstRect.right += dstRect.left;
	dstRect.bottom += dstRect.top;

	D3DSURFACE_DESC d3d9surf_desc;

	IDirect3DSurface9 * pd3dsurf = nullptr;
	rs->GetRenderTarget(&pd3dsurf);
	pd3dsurf->GetDesc(&d3d9surf_desc);
	pd3dsurf->Release();

	hr = rs->CreateTexture(AVI_GetTextureSize(srcWidth), AVI_GetTextureSize(srcHeight), 1, 0, d3d9surf_desc.Format, D3DPOOL_MANAGED, &pTex);
	if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t create texture for this video");
		return false;
	}

	for(int i=0; i<4; i++)
	{
		v[i].w = .5f;
		v[i].pos.z = 1.f;
	}
	v[0].pos.x = (float)dstRect.left;  v[0].pos.y = (float)dstRect.top;
	v[0].tu=0.f; v[0].tv=0.f;
	v[1].pos.x = (float)dstRect.left;  v[1].pos.y = (float)dstRect.bottom;
	v[1].tu=0.f; v[1].tv = (float)lockRect.bottom/AVI_GetTextureSize(srcHeight);
	v[2].pos.x = (float)dstRect.right; v[2].pos.y = (float)dstRect.top;
	v[2].tu = (float)lockRect.right/AVI_GetTextureSize(srcWidth); v[2].tv=0.f;
	v[3].pos.x = (float)dstRect.right, v[3].pos.y = (float)dstRect.bottom;
	v[3].tu = (float)lockRect.right/AVI_GetTextureSize(srcWidth); v[3].tv = (float)lockRect.bottom/AVI_GetTextureSize(srcHeight);

    hr = pAMStream->SetState(STREAMSTATE_RUN);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t run media stream");
        return false;
    }

    return true;
}

bool CAviPlayer::GetInterfaces()
{
    HRESULT hr = S_OK;

    // Initialize COM
    if (FAILED(hr = CoInitialize(NULL)))
        return false;
	m_bMakeUninitializeDD = true;

	hr = DirectDrawCreate(nullptr, &pDD, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t create DirectDraw interface");
        return false;
    }
	hr = pDD->SetCooperativeLevel(api->GetAppHWND(), DDSCL_NORMAL);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t SetCooperativeLevel for DirectDraw");
        return false;
    }
	hr = CoCreateInstance(CLSID_AMMultiMediaStream, nullptr, CLSCTX_INPROC_SERVER,
		IID_IAMMultiMediaStream, (void **)&pAMStream);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t create interface AMMultiMediaStream");
        return false;
    }

	hr = pAMStream->Initialize(STREAMTYPE_READ, AMMSF_NOGRAPHTHREAD, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t initialize interface AMMultiMediaStream");
        return false;
    }
	hr = pAMStream->AddMediaStream(pDD, &MSPID_PrimaryVideo, 0, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t add video stream");
        return false;
    }
	hr = pAMStream->AddMediaStream(nullptr, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, nullptr);
    if (FAILED(hr)) {
		api->Trace("Video Error!!! Can`t add audio stream");
        return false;
    }

    return true;
}

void CAviPlayer::CleanupInterfaces()
{
	IRELEASE(pSample);
	IRELEASE(pDDStream);
	IRELEASE(pPrimaryVidStream);

	IRELEASE(pAMStream);
	IRELEASE(pVideoSurface);
    IRELEASE(pPrimarySurface);
    IRELEASE(pDD);

	IRELEASE(pTmpRenderTarget);

	if(pTex!= nullptr && rs!= nullptr)	rs->Release(pTex);
	pTex= nullptr;

	if( m_bMakeUninitializeDD ) CoUninitialize();
	m_bMakeUninitializeDD = false;
}

void CAviPlayer::ReleaseAll()
{
	CleanupInterfaces();
}
