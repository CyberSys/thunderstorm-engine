﻿#include "../xdefines.h"
#include "scrshoter.h"

#include <core.h>

#define SS_TEXTURE_WIDTH		128
#define SS_TEXTURE_HEIGHT		128
#define SS_TEXTURE_FONECOLOR	0xFF000000

uint32_t GetA8R8G8B8_FromFMT(void* p, uint32_t fmt) {
  uint32_t retVal;

#ifdef _XBOX
	if(fmt==D3DFMT_R5G6B5 || fmt==D3DFMT_LIN_R5G6B5)
#else
  if (fmt == D3DFMT_R5G6B5)
#endif
  {
    retVal = 0xFF000000 |
      (static_cast<uint32_t>(*static_cast<uint16_t*>(p) & 0xF800) << 8) |
      (static_cast<uint32_t>(*static_cast<uint16_t*>(p) & 0x7E0) << 5) |
      (static_cast<uint32_t>(*static_cast<uint16_t*>(p) & 0x1F) << 3);
  }
  else {
    retVal = 0xFF000000 | (*static_cast<uint32_t*>(p));
  }

  return retVal;
}

SCRSHOTER::SCRSHOTER() {
  m_pScrShotTex = nullptr;
  m_list = nullptr;
}

SCRSHOTER::~SCRSHOTER() {
  if (m_pScrShotTex != nullptr && rs != nullptr) rs->Release(m_pScrShotTex);
  m_pScrShotTex = nullptr;
  SAVETEXTURES* pst;
  while (m_list) {
    pst = m_list;
    m_list = pst->next;
    delete pst->fileName;
    if (pst->m_pTex != nullptr && rs != nullptr) rs->Release(pst->m_pTex);
    delete pst;
  }
}

bool SCRSHOTER::Init() {
  //GUARD(SCRSHOTER::Init())
  SetDevice();
  //UNGUARD
  return true;
}

void SCRSHOTER::SetDevice() {
  // получить сервис рендера
  rs = static_cast<VDX9RENDER*>(core.CreateService("dx9render"));
  if (!rs)
    throw std::exception("No service: dx9render");
}

void SCRSHOTER::Execute(uint32_t Delta_Time) {
}

void SCRSHOTER::Realize(uint32_t Delta_Time) {
  if (m_pScrShotTex == nullptr)
    if (!MakeScreenShot()) {
      core.Trace("ERROR!!! screen shot create error");
      core.Event("makescrshot", nullptr);
    }
    else
      core.Event("makescrshot", nullptr);
}

bool SCRSHOTER::MakeScreenShot() {
  int n;
  D3DLOCKED_RECT inRect, outRect;

  auto hr = D3D_OK;

  // Заставим видео карту отрисовать все незакоченные задания
  hr = rs->EndScene();
  if (hr != D3D_OK) {
    core.Trace("ERROR!!! Can`t EndScene");
    return false;
  }
  hr = rs->BeginScene();
  if (hr != D3D_OK) {
    core.Trace("ERROR!!! Can`t BeginScene");
    return false;
  }

  // удалим старый скрин шот
  if (m_pScrShotTex != nullptr && rs != nullptr) rs->Release(m_pScrShotTex);
  m_pScrShotTex = nullptr;

  // получим данные о старой поверхности рендера
  D3DSURFACE_DESC desc;
  IDirect3DSurface9* pOldRenderTarg = nullptr;
  if (hr == D3D_OK) hr = rs->GetRenderTarget(&pOldRenderTarg);
  if (hr == D3D_OK) hr = pOldRenderTarg->GetDesc(&desc);

  // получим копию рендер буфера
  IDirect3DSurface9* pRenderTarg = nullptr;
  if (hr == D3D_OK) hr = rs->CreateOffscreenPlainSurface(desc.Width, desc.Height, desc.Format, &pRenderTarg);
  if (hr == D3D_OK) hr = rs->GetRenderTargetData(pOldRenderTarg, pRenderTarg);
  if (pOldRenderTarg != nullptr) pOldRenderTarg->Release();

  // создадим новый скрин шот
  if (hr == D3D_OK)
    hr = rs->CreateTexture(SS_TEXTURE_WIDTH,SS_TEXTURE_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
                           &m_pScrShotTex); //~!~

  // получим буфер для копии поверхности рендеринга
  void* pIn = nullptr;
  if (hr == D3D_OK) hr = pRenderTarg->LockRect(&inRect, nullptr, 0);
  if (hr == D3D_OK) pIn = inRect.pBits;
  // получим буфер для текстуры
  void* pOut = nullptr;
  if (hr == D3D_OK) hr = m_pScrShotTex->LockRect(0, &outRect, nullptr, 0);
  if (hr == D3D_OK) pOut = outRect.pBits;

  // заполним эту текстуру из копии нашего рендер буфера
  if (hr == D3D_OK) {
    // Создать набор отступов по ординатам
    auto* const pHorzOff = new int[SS_TEXTURE_WIDTH];
    auto* const pVertOff = new int[SS_TEXTURE_HEIGHT];
    if (!pHorzOff || !pVertOff) {
      throw std::exception("allocate memory error");
    }
    int nHorzSize, nVertSize;
    if (static_cast<float>(desc.Width) / desc.Height < static_cast<float>(SS_TEXTURE_WIDTH) / SS_TEXTURE_HEIGHT) {
      nHorzSize = desc.Width;
      nVertSize = desc.Width * SS_TEXTURE_HEIGHT / SS_TEXTURE_WIDTH;
    }
    else {
      nHorzSize = desc.Height * SS_TEXTURE_WIDTH / SS_TEXTURE_HEIGHT;
      nVertSize = desc.Height;
    }
    // Заполним горизонтальные смещения
    for (n = 0; n < SS_TEXTURE_WIDTH; n++)
      pHorzOff[n] = (n * desc.Width / SS_TEXTURE_WIDTH) * (inRect.Pitch / desc.Width);
    // Заполним вертикальные смещения
    for (n = 0; n < SS_TEXTURE_HEIGHT; n++)
      pVertOff[n] = n * desc.Height / SS_TEXTURE_HEIGHT;

    int vi, hi;
    for (vi = 0; vi < SS_TEXTURE_HEIGHT; vi++) {
      auto* const pInPxl = static_cast<uint8_t*>(pIn) + inRect.Pitch * pVertOff[vi];
      auto* pOutPxl = (uint32_t*)(static_cast<uint8_t*>(pOut) + outRect.Pitch * vi);
      for (hi = 0; hi < SS_TEXTURE_WIDTH; hi++) {
        pOutPxl[hi] = GetA8R8G8B8_FromFMT(&pInPxl[pHorzOff[hi]], desc.Format);
      }
    }

    delete[] pHorzOff;
    delete[] pVertOff;
  }

  // закрываем открытые буфера
  if (pIn != nullptr) pRenderTarg->UnlockRect();

#ifdef _XBOX
	// Делаем перевод в смешанную текстуру
	uint32_t dwTmp = outRect.Pitch*SS_TEXTURE_HEIGHT;
	//DWORD dwPixelSize = XGBytesPerPixelFromFormat( D3DFMT_A8R8G8B8 );
	if( (pIn=new char[dwTmp]) == null ) { throw std::exception("allocate memory error") }
	memcpy(pIn,outRect.pBits,dwTmp);
    XGSwizzleRect( pIn, 0, NULL, outRect.pBits,
				   SS_TEXTURE_WIDTH, SS_TEXTURE_HEIGHT,
				   NULL, XGBytesPerPixelFromFormat( D3DFMT_A8R8G8B8 ) );
	delete pIn;
#endif

  // закрываем открытые буфера
  if (pOut != nullptr) m_pScrShotTex->UnlockRect(0);

  // Удалим ненужную уже копию экрана
  if (pRenderTarg != nullptr) pRenderTarg->Release();

  // Наложим на шот текстуру с рамкой
  const int nTextureID = rs->TextureCreate("interfaces\\EmptyBorder.tga");
  if (nTextureID >= 0) {
    IDirect3DTexture9* pScrShotTex = nullptr;
    if (D3D_OK == rs->CreateTexture(SS_TEXTURE_WIDTH,SS_TEXTURE_HEIGHT, 1,D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
                                    D3DPOOL_DEFAULT, &pScrShotTex)) {
      const uint32_t BI_SCRSHOTER_VERTEX_FORMAT = (D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEXTUREFORMAT2);
      struct BI_SCRSHOTER_VERTEX {
        CVECTOR pos;
        float w;
        float tu, tv;
      } vert[4];
      for (auto i = 0; i < 4; i++) {
        vert[i].pos.z = 1.f;
        vert[i].w = 0.5f;
      }
      vert[1].pos.x = vert[0].pos.x = 0;
      vert[2].pos.y = vert[0].pos.y = 0;
      vert[3].pos.x = vert[2].pos.x = SS_TEXTURE_WIDTH;
      vert[3].pos.y = vert[1].pos.y = SS_TEXTURE_HEIGHT;
      vert[1].tu = vert[0].tu = 0;
      vert[2].tv = vert[0].tv = 0;
      vert[3].tu = vert[2].tu = 1.f;
      vert[3].tv = vert[1].tv = 1.f;

      pOldRenderTarg = nullptr;
      pRenderTarg = nullptr;
      if (rs->GetRenderTarget(&pOldRenderTarg) == S_OK) {
        IDirect3DSurface9* pStencil = nullptr;
        rs->GetDepthStencilSurface(&pStencil);
        pScrShotTex->GetSurfaceLevel(0, &pRenderTarg);
        if (rs->SetRenderTarget(pRenderTarg, nullptr) == S_OK) {
          rs->SetTexture(0, m_pScrShotTex);
          rs->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, BI_SCRSHOTER_VERTEX_FORMAT, 2, vert,
                              sizeof(BI_SCRSHOTER_VERTEX), "battle_icons");
          rs->TextureSet(0, nTextureID);
          rs->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, BI_SCRSHOTER_VERTEX_FORMAT, 2, vert,
                              sizeof(BI_SCRSHOTER_VERTEX), "battle_icons");
          rs->SetRenderTarget(pOldRenderTarg, pStencil);
        }
        if (pRenderTarg) pRenderTarg->Release();
        if (pStencil) pStencil->Release();
        if (pOldRenderTarg) pOldRenderTarg->Release();
      }
      IDirect3DSurface9 *pSurf1 = nullptr, *pSurf2 = nullptr;
      rs->GetSurfaceLevel(m_pScrShotTex, 0, &pSurf1);
      rs->GetSurfaceLevel(pScrShotTex, 0, &pSurf2);
      //rs->UpdateSurface(pSurf2,null,0,pSurf1,null);
      hr = D3DXLoadSurfaceFromSurface(pSurf1, nullptr, nullptr, pSurf2, nullptr, nullptr, D3DX_DEFAULT, 0);
      if (pSurf1) rs->Release(pSurf1);
      if (pSurf2) rs->Release(pSurf2);
      rs->Release(pScrShotTex);
    }

    rs->TextureRelease(nTextureID);
  }

  return hr == D3D_OK;
}

uint64_t SCRSHOTER::ProcessMessage(MESSAGE& message) {
  switch (message.Long()) {
  case MSG_SCRSHOT_MAKE: return (uintptr_t)m_pScrShotTex;
    break;
  case MSG_SCRSHOT_READ:
    {
      char param[512], param2[256];
      VDATA* pvdat;

      message.String(sizeof(param) - 1, param);
      message.String(sizeof(param2) - 1, param2);
      pvdat = message.ScriptVariablePointer();

      auto* pRetTex = AddSaveTexture(param, param2);
      auto* const strDat = FindSaveData(param2);
      if (pvdat)
        if (!strDat) pvdat->Set("\0");
        else pvdat->Set(strDat);
      return (uintptr_t)pRetTex;
    }
    break;
  case MSG_SCRSHOT_RELEASE:
    {
      char param[512];
      message.String(sizeof(param) - 1, param);
      DelSaveTexture(param);
    }
    break;
  }
  return 0;
}

IDirect3DTexture9* SCRSHOTER::FindSaveTexture(char* fileName) const {
  if (!fileName) return nullptr;
  auto* ps = m_list;
  while (ps) {
    if (ps->fileName && _stricmp(fileName, ps->fileName) == 0) return ps->m_pTex;
    ps = ps->next;
  }
  return nullptr;
}

char* SCRSHOTER::FindSaveData(char* fileName) const {
  if (!fileName) return nullptr;
  SAVETEXTURES* ps = m_list;
  while (ps) {
    if (ps->fileName && _stricmp(fileName, ps->fileName) == 0) return ps->dataString;
    ps = ps->next;
  }
  return nullptr;
}

IDirect3DTexture9* SCRSHOTER::AddSaveTexture(char* dirName, char* fileName) {
  if (fileName == nullptr) return nullptr;
  IDirect3DTexture9* rval = FindSaveTexture(fileName);
  if (rval) return rval;
  if (_stricmp(fileName, "newsave") == 0) return m_pScrShotTex;
  auto* ps = new SAVETEXTURES;
  if (ps == nullptr) { throw std::exception("Allocate memory error"); }
  ps->dataString = nullptr;
  ps->next = m_list;
  m_list = ps;
  const auto len = strlen(fileName) + 1;
  m_list->fileName = new char[len];
  if (m_list->fileName == nullptr) { throw std::exception("Allocate memory error"); }
  memcpy(m_list->fileName, fileName, len);
  char param[1024];
  if (dirName == nullptr || dirName[0] == 0) sprintf_s(param, "%s", fileName);
  else sprintf_s(param, "%s\\%s", dirName, fileName);
  m_list->m_pTex = GetTexFromSave(param, &ps->dataString);
  return m_list->m_pTex;
}

void SCRSHOTER::DelSaveTexture(char* fileName) {
  if (!fileName) return;
  SAVETEXTURES* oldps = nullptr;
  SAVETEXTURES* ps = m_list;
  while (ps) {
    if (ps->fileName && _stricmp(fileName, ps->fileName) == 0) {
      if (oldps) oldps->next = ps->next;
      else m_list = ps->next;
      delete ps->fileName;
      delete ps->dataString;
      if (ps->m_pTex != nullptr && rs != nullptr) rs->Release(ps->m_pTex);
      delete ps;
      return;
    }
    oldps = ps;
    ps = ps->next;
  }
}

IDirect3DTexture9* SCRSHOTER::GetTexFromSave(char* fileName, char** pDatStr) const {
  HRESULT hr = D3D_OK;
  D3DLOCKED_RECT outRect;
  IDirect3DTexture9* pt = nullptr;
  *pDatStr = nullptr; //~!~

  long datSize = 0;
  char* pdat = nullptr;
  pdat = static_cast<char*>(core.GetSaveData(fileName, datSize));
  long startIdx = 0;
  long texSize = 0;
  if (pdat != nullptr && datSize > sizeof(SAVE_DATA_HANDLE)) {
    startIdx = ((SAVE_DATA_HANDLE*)pdat)->StringDataSize + sizeof(SAVE_DATA_HANDLE);
    texSize = ((SAVE_DATA_HANDLE*)pdat)->SurfaceDataSize;
  }
  if (datSize > sizeof(SAVE_DATA_HANDLE) && datSize == startIdx + texSize && texSize == 4 * SS_TEXTURE_WIDTH *
    SS_TEXTURE_HEIGHT) {
    hr = rs->CreateTexture(SS_TEXTURE_WIDTH,SS_TEXTURE_HEIGHT, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pt);
    if (hr == D3D_OK) hr = pt->LockRect(0, &outRect, nullptr, 0);
    if (hr == D3D_OK) {
      memcpy(outRect.pBits, &pdat[startIdx], texSize);
      pt->UnlockRect(0);
    }
    if (pDatStr) {
      const int strLen = startIdx - sizeof(SAVE_DATA_HANDLE);
      *pDatStr = new char[strLen + 1];
      if (!*pDatStr) { throw std::exception("allocate memory error"); }
      strncpy_s(*pDatStr, strLen + 1, &pdat[sizeof(SAVE_DATA_HANDLE)], strLen);
      (*pDatStr)[strLen] = 0;
    }
  }
  /*	else
    {
      int idx=0;
      for(int i=0; i<SS_TEXTURE_HEIGHT; i++)
        for(int j=0; j<SS_TEXTURE_WIDTH; j++)
        {
          ((DWORD*)outRect.pBits)[idx] = 0xFF000000;
          idx++;
        }
    }*/
  delete pdat;

  if (hr == D3D_OK) return pt;
  if (*pDatStr) delete (*pDatStr);
  *pDatStr = nullptr;
  if (pt && rs) rs->Release(pt);
  return nullptr;
}
