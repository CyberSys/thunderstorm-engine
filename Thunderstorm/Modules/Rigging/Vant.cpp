#include "Vant.h"
#include "defines.h"
#include "sail_msg.h"
#include "ship_base.h"
#include "rigging_define.h"
#include <Entity.h>
#include "vfile_service.h"
#include <core.h>

VANT::VANT() {
  bUse = false;
  RenderService = nullptr;
  TextureName = nullptr;
  texl = -1;
  bRunFirstTime = true;
  bYesDeleted = false;
  wVantLast = 0;
  gdata = nullptr;
  groupQuantity = 0;
  vlist = nullptr;
  vantQuantity = 0;
  vBuf = iBuf = -1;
  nVert = nIndx = 0;
}

VANT::~VANT() {
  TEXTURE_RELEASE(RenderService, texl);
  STORM_DELETE(TextureName);
  while (groupQuantity > 0) {
    groupQuantity--;
    STORM_DELETE(gdata[groupQuantity].vantIdx);
  }
  STORM_DELETE(gdata);
  while (vantQuantity > 0) {
    vantQuantity--;
    STORM_DELETE(vlist[vantQuantity]);
  }
  STORM_DELETE(vlist);
  VERTEX_BUFFER_RELEASE(RenderService, vBuf);
  INDEX_BUFFER_RELEASE(RenderService, iBuf);
  nVert = nIndx = 0;
}

bool VANT::Init() {
  //GUARD(VANT::VANT())
  SetDevice();
  //UNGUARD
  return true;
}

void VANT::SetDevice() {
  // �������� ������ �������
  RenderService = static_cast<VDX9RENDER*>(core.CreateService("dx9render"));
  if (!RenderService) {
    throw std::exception("No service: dx9render");
  }

  LoadIni();

  if (texl == -1) texl = RenderService->TextureCreate(TextureName);
}

bool VANT::CreateState(ENTITY_STATE_GEN* state_gen) {
  return true;
}

bool VANT::LoadState(ENTITY_STATE* state) {
  return true;
}

void VANT::Execute(uint32_t Delta_Time) {
  if (bRunFirstTime) FirstRun();
  if (bYesDeleted) DoSTORM_DELETE();

  if (bUse) {
    //====================================================
    // ���� ��� ������� ���-����, �� ������� ���� �� ����
    WIN32_FIND_DATA wfd;
    auto* const h = fio->_FindFirstFile("resource\\ini\\rigging.ini", &wfd);
    if (INVALID_HANDLE_VALUE != h) {
      auto ft_new = wfd.ftLastWriteTime;
      fio->_FindClose(h);

      if (CompareFileTime(&ft_old, &ft_new) != 0) {
        LoadIni();
      }
    }

    doMove();
  }
}

void VANT::Realize(uint32_t Delta_Time) const {
  if (bUse) {
    // _asm rdtsc _asm mov rtm,eax

    RenderService->TextureSet(0, texl);
    uint32_t ambient;
    RenderService->GetRenderState(D3DRS_AMBIENT, &ambient);
    RenderService->SetRenderState(D3DRS_TEXTUREFACTOR, ambient);
    const auto bDraw = RenderService->TechniqueExecuteStart("ShipVant");
    if (!bDraw) return;

    // draw nature vants
    CVECTOR cp, ca;
    float pr;
    RenderService->GetCamera(cp, ca, pr);
    pr = tanf(pr * .5f);
    for (auto gn = 0; gn < groupQuantity; gn++)
      if (gdata[gn].nIndx && nVert && (~(gdata[gn].pMatWorld->Pos() - cp)) * pr < fVantMaxDist) {
        static_cast<SHIP_BASE*>(EntityManager::GetEntityPointer(gdata[gn].shipEI))->SetLightAndFog(true);
        static_cast<SHIP_BASE*>(EntityManager::GetEntityPointer(gdata[gn].shipEI))->SetLights();

        RenderService->SetTransform(D3DTS_WORLD, (D3DXMATRIX*)gdata[gn].pMatWorld);
        RenderService->DrawBuffer(vBuf, sizeof(VANTVERTEX), iBuf, 0, nVert, gdata[gn].sIndx, gdata[gn].nIndx);

        static_cast<SHIP_BASE*>(EntityManager::GetEntityPointer(gdata[gn].shipEI))->UnSetLights();
        static_cast<SHIP_BASE*>(EntityManager::GetEntityPointer(gdata[gn].shipEI))->RestoreLightAndFog();
        //_asm rdtsc  _asm sub eax,rtm _asm mov rtm,eax
      }
    while (RenderService->TechniqueExecuteNext()) {
    }
    //RenderService->Print(0,200,"Vants vert=%d, tr=%d, time=%d",nVert,nIndx,rtm);
  }
}

uint64_t VANT::ProcessMessage(MESSAGE& message) {
  const auto code = message.Long();

  switch (code) {
  case MSG_VANT_INIT:
    {
      const auto oldvantQuantity = vantQuantity;
      if (gdata == nullptr) {
        if ((gdata = new GROUPDATA[1]) == nullptr)
          throw std::exception("Not memory allocation");
        groupQuantity = 1;
      }
      else {
        auto* const oldgdata = gdata;
        if ((gdata = new GROUPDATA[groupQuantity + 1]) == nullptr)
          throw std::exception("Not memory allocation");
        memcpy(gdata, oldgdata, sizeof(GROUPDATA) * groupQuantity);
        delete oldgdata;
        groupQuantity++;
      }
      ZERO(gdata[groupQuantity-1]);
      gdata[groupQuantity - 1].shipEI = message.EntityID();
      gdata[groupQuantity - 1].model_id = message.EntityID();
      MODEL* mdl;
      mdl = static_cast<MODEL*>(EntityManager::GetEntityPointer(gdata[groupQuantity - 1].model_id));
      if (mdl == nullptr)
        throw std::exception("Bad Vant INIT");

      gdata[groupQuantity - 1].pMatWorld = &mdl->mtx;
      NODE* nod;
      GEOS::INFO gi;
      GEOS::LABEL gl;
      int i, j;
      for (j = 0; j < 1000; j++) {
        nod = mdl->GetNode(j);
        if (!nod) break;

        nod->geo->GetInfo(gi);
        for (i = 0; i < gi.nlabels; i++) {
          nod->geo->GetLabel(i, gl);
          if (!strncmp(gl.name, "vant", 4))
            AddLabel(gl, nod);
        }
      }

      if (vantQuantity == oldvantQuantity) // �� ���� ������ - ������� ��� ������
      {
        if (groupQuantity == 1) {
          delete gdata;
          gdata = nullptr;
          groupQuantity = 0;
        }
        else {
          groupQuantity--;
          auto* const oldgdata = gdata;
          gdata = new GROUPDATA[groupQuantity];
          if (gdata == nullptr) gdata = oldgdata;
          else {
            memcpy(gdata, oldgdata, sizeof(GROUPDATA) * groupQuantity);
            delete oldgdata;
          }
        }
        return 0;
      }

      bRunFirstTime = true;
      SetAdd(oldvantQuantity);

      nIndx *= 3;
      //			if(gdata[groupQuantity-1].vantIdx) delete gdata[groupQuantity-1].vantIdx;
      gdata[groupQuantity - 1].sIndx = nIndx;
      gdata[groupQuantity - 1].nIndx = 0;
      gdata[groupQuantity - 1].sVert = nVert;
      gdata[groupQuantity - 1].nVert = 0;

      gdata[groupQuantity - 1].vantQuantity = vantQuantity - oldvantQuantity;
      gdata[groupQuantity - 1].vantIdx = new int[vantQuantity - oldvantQuantity];
      if (gdata[groupQuantity - 1].vantIdx == nullptr) { throw std::exception("allocate memory error"); }

      auto idx = 0;
      for (int vn = oldvantQuantity; vn < vantQuantity; vn++) {
        gdata[groupQuantity - 1].vantIdx[idx++] = vn;
        vlist[vn]->sv = nVert;
        vlist[vn]->st = nIndx;
        gdata[groupQuantity - 1].nIndx += vlist[vn]->nt;
        gdata[groupQuantity - 1].nVert += vlist[vn]->nv;
        nVert += vlist[vn]->nv;
        nIndx += vlist[vn]->nt * 3;
      }

      nIndx /= 3;
    }
    break;

  case MSG_VANT_DEL_GROUP:
    {
      const entid_t tmp_id = message.EntityID();
      for (int i = 0; i < groupQuantity; i++)
        if (gdata[i].model_id == tmp_id) {
          gdata[i].bDeleted = true;
          bYesDeleted = true;
          break;
        }
    }
    break;

  case MSG_VANT_DEL_MAST:
    {
      const entid_t tmp_id = message.EntityID();
      NODE* mastNode = (NODE*)message.Pointer();
      if (mastNode == nullptr) break;
      for (int i = 0; i < groupQuantity; i++)
        if (gdata[i].model_id == tmp_id) {
          for (int j = 0; j < gdata[i].vantQuantity; j++)
            if (&mastNode->glob_mtx == vlist[gdata[i].vantIdx[j]]->pUpMatWorld) {
              vlist[gdata[i].vantIdx[j]]->bDeleted = true;
              bYesDeleted = true;
            }
          break;
        }
    }
    break;
  }

  return 0;
}

void VANT::SetIndex() const {
  int i, j;
  int ti, vi;

  uint16_t* pt = static_cast<uint16_t*>(RenderService->LockIndexBuffer(iBuf));
  if (pt) {
    for (int vn = 0; vn < vantQuantity; vn++) {
      ti = vlist[vn]->st;
      vi = vlist[vn]->sv;

      // set center treangle & square
      pt[ti++] = vi;
      pt[ti++] = vi + 2;
      pt[ti++] = vi + 1;
      pt[ti++] = vi + 3;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 5;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 6;
      pt[ti++] = vi + 5;
      vi += 7;

      // set balk treangles
      pt[ti++] = vi;
      pt[ti++] = vi + 1;
      pt[ti++] = vi + 2;
      pt[ti++] = vi + 3;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 5;
      pt[ti++] = vi;
      pt[ti++] = vi + 3;
      pt[ti++] = vi + 4;
      pt[ti++] = vi;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 1;
      pt[ti++] = vi;
      pt[ti++] = vi + 2;
      pt[ti++] = vi + 3;
      pt[ti++] = vi + 3;
      pt[ti++] = vi + 2;
      pt[ti++] = vi + 5;
      pt[ti++] = vi + 2;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 5;
      pt[ti++] = vi + 1;
      pt[ti++] = vi + 4;
      pt[ti++] = vi + 2;
      vi += 6;

      int dIdx = vi + (VANT_EDGE + 1) * ROPE_QUANT;
      // set ropes treangles
      for (i = 0; i < ROPE_QUANT; i++) {
        for (j = 0; j < VANT_EDGE; j++) {
          pt[ti] = pt[ti + 3] = vi + j;
          pt[ti + 2] = dIdx + j;
          //                    if(j<(VANT_EDGE-1))
          //                    {
          pt[ti + 4] = vi + j + 1;
          pt[ti + 1] = pt[ti + 5] = dIdx + j + 1;
          //                    }
          /*                    else
                              {
                                  pt[ti+4]=vi;
                                  pt[ti+1]=pt[ti+5]=dIdx;
                              }*/
          ti += 6;
        }
        dIdx += VANT_EDGE + 1;
        vi += VANT_EDGE + 1;
      }
    }

    RenderService->UnLockIndexBuffer(iBuf);
  }
}

void VANT::SetVertexes() const {
  int j, i;
  uint32_t iv;
  CVECTOR uPos, lPos, rPos;

  auto* pv = static_cast<VANTVERTEX*>(RenderService->LockVertexBuffer(vBuf));
  if (pv) {
    for (int vn = 0; vn < vantQuantity; vn++) {
      if (gdata[vlist[vn]->HostGroup].bDeleted) continue;
      iv = vlist[vn]->sv;

      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv((*vlist[vn]->pUpMatWorld) * vlist[vn]->pUp, uPos);
      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv((*vlist[vn]->pDownMatWorld) * vlist[vn]->pLeft, lPos);
      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv((*vlist[vn]->pDownMatWorld) * vlist[vn]->pRight, rPos);

      // Set last parameters
      vlist[vn]->pLeftStart = vlist[vn]->pLeftOld = lPos;
      vlist[vn]->pUpStart = vlist[vn]->pUpOld = uPos;

      CVECTOR horzDirect = !(rPos - lPos);
      CVECTOR vertDirect = !((rPos + lPos) * .5f - uPos);
      // Set angles point
      pv[iv].pos = uPos;
      pv[iv + 3].pos = pv[iv + 1].pos = uPos - horzDirect * (upWidth * .5f) + vertDirect * upHeight * (1.f -
        fBalkHeight);
      pv[iv + 4].pos = pv[iv + 2].pos = uPos + horzDirect * (upWidth * .5f) + vertDirect * upHeight * (1.f -
        fBalkHeight);
      pv[iv + 5].pos = lPos;
      pv[iv + 6].pos = rPos;
      //
      pv[iv].tu = (treangXl + treangXr) * .5f;
      pv[iv].tv = treangYu;
      pv[iv + 1].tu = treangXl;
      pv[iv + 1].tv = treangYd;
      pv[iv + 2].tu = treangXr;
      pv[iv + 2].tv = treangYd;
      //
      const float fh = sqrtf(~((rPos + lPos) * .5f - uPos));
      auto ftmp = static_cast<float>(static_cast<int>(fh / hRopeHeight + .5f));
      pv[iv + 3].tu = ropeXl;
      pv[iv + 3].tv = 0.f;
      pv[iv + 4].tu = ropeXr;
      pv[iv + 4].tv = 0.f;
      pv[iv + 5].tu = ropeXl;
      pv[iv + 5].tv = ftmp;
      pv[iv + 6].tu = ropeXr;
      pv[iv + 6].tv = ftmp;
      iv += 7;

      // ���������� ����� �����
      CVECTOR tvec = uPos - horzDirect * (upWidth * .5f) + vertDirect * upHeight;
      pv[iv].pos = tvec - vertDirect * upHeight * fBalkHeight;
      pv[iv + 1].pos = tvec + vlist[vn]->pos[0] * fBalkWidth;
      pv[iv + 2].pos = tvec + vlist[vn]->pos[VANT_EDGE / 2] * fBalkWidth;
      tvec += horzDirect * upWidth;
      pv[iv + 3].pos = tvec - vertDirect * upHeight * fBalkHeight;
      pv[iv + 4].pos = tvec + vlist[vn]->pos[0] * fBalkWidth;
      pv[iv + 5].pos = tvec + vlist[vn]->pos[VANT_EDGE / 2] * fBalkWidth;
      //
      pv[iv].tu = pv[iv + 1].tu = pv[iv + 2].tu = treangXl;
      pv[iv + 3].tu = pv[iv + 4].tu = pv[iv + 5].tu = treangXr;
      pv[iv].tv = pv[iv + 3].tv = balkYu;
      pv[iv + 1].tv = pv[iv + 2].tv = pv[iv + 4].tv = pv[iv + 5].tv = balkYd;
      iv += 6;

      // Set up ropes points
      CVECTOR sp = uPos - horzDirect * (.5f * upWidth) + vertDirect * upHeight;
      CVECTOR dp = horzDirect * (upWidth / static_cast<float>(ROPE_QUANT - 1));
      const float dtmp = (vRopeXr - vRopeXl) / static_cast<float>(VANT_EDGE);
      for (i = 0; i < ROPE_QUANT; i++) {
        for (j = 0; j <= VANT_EDGE; j++) {
          if (j == VANT_EDGE)
            pv[iv + j].pos = sp + vlist[vn]->pos[0];
          else
            pv[iv + j].pos = sp + vlist[vn]->pos[j];
          pv[iv + j].tu = vRopeXl + dtmp * static_cast<float>(j);
          pv[iv + j].tv = 0.f;
        }
        iv += VANT_EDGE + 1;
        sp += dp;
      }

      // Set down ropes points
      sp = lPos;
      dp = (rPos - lPos) / static_cast<float>(ROPE_QUANT - 1);
      ftmp = fh / vRopeHeight;
      for (i = 0; i < ROPE_QUANT; i++) {
        for (j = 0; j <= VANT_EDGE; j++) {
          if (j == VANT_EDGE)
            pv[iv + j].pos = sp + vlist[vn]->pos[0];
          else
            pv[iv + j].pos = sp + vlist[vn]->pos[j];
          pv[iv + j].tu = vRopeXl + dtmp * static_cast<float>(j);
          pv[iv + j].tv = ftmp;
        }
        iv += VANT_EDGE + 1;
        sp += dp;
      }
    }

    RenderService->UnLockVertexBuffer(vBuf);
  }
}

void VANT::AddLabel(GEOS::LABEL& lbl, NODE* nod) {
  VANTDATA* vd;
  int vantNum;

  if (nod == nullptr) return;

  vantNum = atoi(&lbl.name[4]);

  int vn;
  for (vn = 0; vn < vantQuantity; vn++)
    if ((vlist[vn]->HostGroup == groupQuantity - 1) &&
      (vlist[vn]->vantNum == vantNum)) {
      vd = vlist[vn];
      break;
    }
  if (vn == vantQuantity) {
    //������� ����� ����
    vd = new VANTDATA;
    if (vd == nullptr)
      throw std::exception("Not memory allocate");
    PZERO(vd, sizeof(VANTDATA));
    vd->bDeleted = false;
    vd->vantNum = vantNum;
    vd->pUpMatWorld = vd->pDownMatWorld = nullptr;
    vd->HostGroup = groupQuantity - 1;

    if (vantQuantity == 0) {
      vlist = new VANTDATA*[1];
      vantQuantity = 1;
    }
    else {
      VANTDATA** oldvlist = vlist;
      vlist = new VANTDATA*[vantQuantity + 1];
      if (vlist == nullptr)
        throw std::exception("Not memory allocate");
      memcpy(vlist, oldvlist, sizeof(VANTDATA*) * vantQuantity);
      delete oldvlist;
      vantQuantity++;
    }

    vlist[vantQuantity - 1] = vd;
  }

  switch (lbl.name[5]) {
  case 'u': // up edge of vant
    vd->pUp = CVECTOR(lbl.m[3][0], lbl.m[3][1], lbl.m[3][2]) - gdata[groupQuantity - 1].pMatWorld->Pos();
    // + nod->glob_mtx.Pos();
    vd->pUpMatWorld = &nod->glob_mtx; // get host matrix
    break;
  case 'l': // left edge of vant
    vd->pLeft = CVECTOR(lbl.m[3][0], lbl.m[3][1], lbl.m[3][2]) - gdata[groupQuantity - 1].pMatWorld->Pos();
    // + nod->glob_mtx.Pos();
    if (vd->pDownMatWorld == nullptr)
      vd->pDownMatWorld = &nod->glob_mtx; // get host matrix
    else if (vd->pDownMatWorld != &nod->glob_mtx)
      vd->pDownMatWorld->MulToInv(nod->glob_mtx * vd->pLeft, vd->pLeft);
    break;
  case 'r': // right edge of vant
    vd->pRight = CVECTOR(lbl.m[3][0], lbl.m[3][1], lbl.m[3][2]) - gdata[groupQuantity - 1].pMatWorld->Pos();
    // + nod->glob_mtx.Pos();
    if (vd->pDownMatWorld == nullptr)
      vd->pDownMatWorld = &nod->glob_mtx; // get host matrix
    else if (vd->pDownMatWorld != &nod->glob_mtx)
      vd->pDownMatWorld->MulToInv(nod->glob_mtx * vd->pLeft, vd->pLeft);
    break;
  }
}

void VANT::SetAll() {
  // set vertex and index buffers
  for (int vn = 0; vn < vantQuantity; vn++) {
    while (true) {
      if (!gdata[vlist[vn]->HostGroup].bDeleted)
        if (vlist[vn]->pUpMatWorld && vlist[vn]->pDownMatWorld) break;

      delete vlist[vn];
      vantQuantity--;
      if (vantQuantity > 0) {
        VANTDATA** oldvlist = vlist;
        vlist = new VANTDATA*[vantQuantity];
        if (vlist) {
          if (vn > 0)
            memcpy(vlist, oldvlist, sizeof(VANTDATA*) * vn);
          if (vn < vantQuantity)
            memcpy(&vlist[vn], &oldvlist[vn + 1], sizeof(VANTDATA*) * (vantQuantity - vn));
          delete oldvlist;
        }
        else
          vlist = oldvlist;
      }
      else {
        delete vlist;
        vlist = nullptr;
      }
      if (vn == vantQuantity) break;
    }
    if (vn == vantQuantity) break;

    // Set normals with length equal the rope width
    CMatrix tmat;
    tmat.BuildViewMatrix(.5f * (vlist[vn]->pLeft + vlist[vn]->pRight), vlist[vn]->pUp, CVECTOR(0.f, 0.f, 1.f));
    float ca, sa;
    for (int i = 0; i < VANT_EDGE; i++) {
      ca = cosf(static_cast<float>(i) / static_cast<float>(VANT_EDGE) * 2.f * PI);
      sa = sinf(static_cast<float>(i) / static_cast<float>(VANT_EDGE) * 2.f * PI);
      //tmat.MulToInvNorm(CVECTOR(ROPE_WIDTH/2.f*ca,ROPE_WIDTH/2.f*sa,0.f),vlist[vn]->pos[i]);
      vlist[vn]->pos[i] = CVECTOR(ROPE_WIDTH / 2.f * ca, 0.f, ROPE_WIDTH / 2.f * sa);
    }

    vlist[vn]->nv = (VANT_EDGE + 1) * ROPE_QUANT * 2 + 7 + 6;
    vlist[vn]->nt = VANT_EDGE * ROPE_QUANT * 2 + 3 + 8;
  }
}

void VANT::LoadIni() {
  //GUARD(VANT::LoadIni());
  char section[256];
  char param[256];

  INIFILE* ini;
  WIN32_FIND_DATA wfd;
  const HANDLE h = fio->_FindFirstFile("resource\\ini\\rigging.ini", &wfd);
  if (INVALID_HANDLE_VALUE != h) {
    ft_old = wfd.ftLastWriteTime;
    fio->_FindClose(h);
  }
  ini = fio->OpenIniFile("resource\\ini\\rigging.ini");
  if (!ini) throw std::exception("rigging.ini file not found!");

  sprintf_s(section, "VANTS");

  // ��� ��������
  ini->ReadString(section, "TextureName", param, sizeof(param) - 1, "vant.tga");
  if (texl != -1) {
    if (strcmp(TextureName, param))
      if (RenderService) {
        delete TextureName;
        const auto len = strlen(param) + 1;
        TextureName = new char[len];
        memcpy(TextureName, param, len);
        RenderService->TextureRelease(texl);
        texl = RenderService->TextureCreate(TextureName);
      }
  }
  else {
    const auto len = strlen(param) + 1;
    TextureName = new char[len];
    memcpy(TextureName, param, len);
  }
  // ������� �������
  ROPE_WIDTH = ini->GetFloat(section, "fWidth", 0.1f);
  // ���������� �������
  ROPE_QUANT = static_cast<int>(ini->GetLong(section, "fRopeQuant", 5));
  if (ROPE_QUANT < 2) ROPE_QUANT = 2;
  // ���������� �������� �������������� ������� xBeg
  ropeXl = ini->GetFloat(section, "fHRopeXbeg", 0.5f);
  ropeXr = ini->GetFloat(section, "fHRopeXend", 1.f);
  // ���������� �������� ������������
  treangXl = ini->GetFloat(section, "fTreangXbeg", 0.f);
  treangXr = ini->GetFloat(section, "fTreangXend", 0.5f);
  treangYu = ini->GetFloat(section, "fTreangYbeg", 0.f);
  treangYd = ini->GetFloat(section, "fTreangYend", 1.f);
  // ���������� �������� �����
  balkYu = ini->GetFloat(section, "fBalkYbeg", 0.6f);
  balkYd = ini->GetFloat(section, "fBalkYend", 1.f);
  // ���������� �������� ������������ �������
  vRopeXl = ini->GetFloat(section, "fVRopeXbeg", 0.f);
  vRopeXr = ini->GetFloat(section, "fVRopeXend", 0.1f);
  // ������ �������� ������������
  upWidth = ini->GetFloat(section, "fTreangWidth", 1.f);
  // ������ �������� ������������
  upHeight = ini->GetFloat(section, "fTreangHeight", 1.f);
  // ������ ������������ �������
  vRopeHeight = ini->GetFloat(section, "fVRopeHeight", 1.f);
  // ������ �������������� �������
  hRopeHeight = ini->GetFloat(section, "fHRopeHeight", 1.f);
  // ������ ����� ������������ ������ ������������
  fBalkHeight = ini->GetFloat(section, "fBalkHeight", 0.1f);
  fBalkWidth = ini->GetFloat(section, "fBalkWidth", 1.2f);
  // ������� ���������� � �������� �� ����� �����
  fVantMaxDist = ini->GetFloat(section, "fVantMaxDist", 10000.f);
  // ��� ������������� �������� �����
  ZERO_CMP_VAL = ini->GetFloat(section, "fDiscrValue", 0.01f);
  // ������������ ��������� ��������� ����� ��� ������� ���� ��������� ������������
  MAXFALL_CMP_VAL = ini->GetFloat(section, "fDisapearValue", 5.f);

  delete ini;
  //UNGUARD
}

void VANT::doMove() {
  int j, i;
  uint32_t iv;
  CVECTOR uPos, lPos, rPos;

  auto* pv = static_cast<VANTVERTEX*>(RenderService->LockVertexBuffer(vBuf));
  if (pv) {
    for (int vn = 0; vn < vantQuantity; vn++) {
      if (gdata[vlist[vn]->HostGroup].bDeleted || vlist[vn]->bDeleted) {
        bYesDeleted = true;
        continue;
      }
      CVECTOR vtmp, htmp;
      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv(*vlist[vn]->pUpMatWorld * vlist[vn]->pUp, uPos);
      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv(*vlist[vn]->pDownMatWorld * vlist[vn]->pLeft, lPos);
      gdata[vlist[vn]->HostGroup].pMatWorld->MulToInv(*vlist[vn]->pDownMatWorld * vlist[vn]->pRight, rPos);

      if (!VectCmp(lPos, vlist[vn]->pLeftStart, MAXFALL_CMP_VAL) ||
        !VectCmp(uPos, vlist[vn]->pUpStart, MAXFALL_CMP_VAL)) {
        vlist[vn]->bDeleted = true; // ��������� ������� �������� �����
        bYesDeleted = true;
      }

      if (!VectCmp(lPos, vlist[vn]->pLeftOld, ZERO_CMP_VAL) ||
        !VectCmp(uPos, vlist[vn]->pUpOld, ZERO_CMP_VAL)) {
        // Set last parameters
        vlist[vn]->pLeftOld = lPos;
        vlist[vn]->pUpOld = uPos;

        CVECTOR horzDirect = !(rPos - lPos);
        CVECTOR vertDirect = !((rPos + lPos) * .5f - uPos);

        iv = vlist[vn]->sv;

        // Set angles point
        pv[iv].pos = uPos;
        htmp = horzDirect * (upWidth * .5f);
        vtmp = vertDirect * upHeight * (1.f - fBalkHeight);
        pv[iv + 3].pos = pv[iv + 1].pos = uPos - htmp + vtmp;
        pv[iv + 4].pos = pv[iv + 2].pos = uPos + htmp + vtmp;
        pv[iv + 5].pos = lPos;
        pv[iv + 6].pos = rPos;
        iv += 7;

        // ���������� ����� �����
        CVECTOR tvec = uPos - htmp + vertDirect * upHeight;
        pv[iv].pos = uPos - htmp + vtmp;
        pv[iv + 1].pos = tvec + vlist[vn]->pos[0] * fBalkWidth;
        pv[iv + 2].pos = tvec + vlist[vn]->pos[VANT_EDGE / 2] * fBalkWidth;
        tvec += horzDirect * upWidth;
        pv[iv + 3].pos = uPos + htmp + vtmp;
        pv[iv + 4].pos = tvec + vlist[vn]->pos[0] * fBalkWidth;
        pv[iv + 5].pos = tvec + vlist[vn]->pos[VANT_EDGE / 2] * fBalkWidth;
        iv += 6;

        // Set up ropes points
        CVECTOR sp = uPos - horzDirect * (.5f * upWidth) + vertDirect * upHeight;
        CVECTOR dp = horzDirect * (upWidth / static_cast<float>(ROPE_QUANT - 1));
        for (i = 0; i < ROPE_QUANT; i++) {
          for (j = 0; j <= VANT_EDGE; j++) {
            if (j == VANT_EDGE)
              pv[iv + j].pos = sp + vlist[vn]->pos[0];
            else
              pv[iv + j].pos = sp + vlist[vn]->pos[j];
          }
          iv += VANT_EDGE + 1;
          sp += dp;
        }

        // Set down ropes points
        sp = lPos;
        dp = (rPos - lPos) / static_cast<float>(ROPE_QUANT - 1);
        for (i = 0; i < ROPE_QUANT; i++) {
          for (j = 0; j <= VANT_EDGE; j++) {
            if (j == VANT_EDGE)
              pv[iv + j].pos = sp + vlist[vn]->pos[0];
            else
              pv[iv + j].pos = sp + vlist[vn]->pos[j];
          }
          iv += VANT_EDGE + 1;
          sp += dp;
        }
      }
    }

    RenderService->UnLockVertexBuffer(vBuf);
  }
}

bool VANT::VectCmp(CVECTOR v1, CVECTOR v2, float minCmpVal) // return true if equal
{
  const CVECTOR dv = v1 - v2;

  if (dv.x > minCmpVal || dv.x < -minCmpVal ||
    dv.y > minCmpVal || dv.y < -minCmpVal ||
    dv.z > minCmpVal || dv.z < -minCmpVal)
    return false;
  return true;
}

void VANT::FirstRun() {
  if (nVert > 0 && nIndx > 0) {
    VERTEX_BUFFER_RELEASE(RenderService, vBuf);
    INDEX_BUFFER_RELEASE(RenderService, iBuf);
    vBuf = RenderService->CreateVertexBuffer(VANTVERTEX_FORMAT, nVert * sizeof(VANTVERTEX),D3DUSAGE_WRITEONLY);
    iBuf = RenderService->CreateIndexBuffer(nIndx * 6);
    SetVertexes();
    SetIndex();
  }

  bUse = (vBuf != -1 && iBuf != -1);

  bRunFirstTime = false;
  wVantLast = vantQuantity;
}

void VANT::SetAdd(int firstNum) {
  // set vertex and index buffers
  for (int vn = firstNum; vn < vantQuantity; vn++) {
    while (vlist[vn]->pUpMatWorld == nullptr || vlist[vn]->pDownMatWorld == nullptr) {
      delete vlist[vn];
      vantQuantity--;
      if (vantQuantity > 0) {
        VANTDATA** oldvlist = vlist;
        vlist = new VANTDATA*[vantQuantity];
        if (vlist == nullptr)
          vlist = oldvlist;
        if (vn > 0)
          memcpy(vlist, oldvlist, sizeof(VANTDATA*) * vn);
        if (vn < vantQuantity)
          memcpy(&vlist[vn], &oldvlist[vn + 1], sizeof(VANTDATA*) * (vantQuantity - vn));
        if (vlist != oldvlist)
          delete oldvlist;
      }
      else {
        delete vlist;
        vlist = nullptr;
      }
      if (vn == vantQuantity) break;
    }
    if (vn == vantQuantity) break;

    // Set normals with length equal the rope width
    CMatrix tmat;
    tmat.BuildViewMatrix(.5f * (vlist[vn]->pLeft + vlist[vn]->pRight), vlist[vn]->pUp, CVECTOR(0.f, 0.f, 1.f));
    float ca, sa;
    for (int i = 0; i < VANT_EDGE; i++) {
      ca = cosf(static_cast<float>(i) / static_cast<float>(VANT_EDGE) * 2.f * PI);
      sa = sinf(static_cast<float>(i) / static_cast<float>(VANT_EDGE) * 2.f * PI);
      tmat.MulToInvNorm(CVECTOR(ROPE_WIDTH / 2.f * ca, ROPE_WIDTH / 2.f * sa, 0.f), vlist[vn]->pos[i]);
    }

    vlist[vn]->nv = (VANT_EDGE + 1) * ROPE_QUANT * 2 + 7 + 6;
    vlist[vn]->nt = VANT_EDGE * ROPE_QUANT * 2 + 3 + 8;
  }
}

void VANT::DoSTORM_DELETE() {
  // ��� ���� ��������� ����� ������ ������������� �� �����
  int ngn = 0;
  int nvn = 0;
  nVert = 0;
  nIndx = 0;
  for (int gn = 0; gn < groupQuantity; gn++) {
    int gs = 0; // ������ ������
    gdata[gn].sVert = nVert;
    gdata[gn].sIndx = nIndx;
    gdata[gn].nVert = 0;
    gdata[gn].nIndx = 0;
    for (int idx = 0; idx < gdata[gn].vantQuantity; idx++) {
      const int vn = gdata[gn].vantIdx[idx];
      if (vlist[vn]->bDeleted || gdata[gn].bDeleted) {
        delete vlist[vn];
        continue;
      }

      vlist[vn]->HostGroup = ngn;
      vlist[vn]->sv = nVert;
      vlist[vn]->st = nIndx;
      gdata[gn].nVert += vlist[vn]->nv;
      gdata[gn].nIndx += vlist[vn]->nt;
      nVert += vlist[vn]->nv;
      nIndx += vlist[vn]->nt * 3;

      gdata[gn].vantIdx[gs++] = nvn;

      // �������� ���� �� ������ ������ ������
      vlist[nvn++] = vlist[vn];
    }

    // ���� ������ ������, �� ������ ��
    if (gs == 0) {
      delete gdata[gn].vantIdx;
      continue;
    }

    gdata[gn].vantQuantity = gs;
    // �������� ������ �� ������ ����� � ������
    if (ngn < gn)
      memcpy(&gdata[ngn], &gdata[gn], sizeof(GROUPDATA));
    ngn++;
  }

  nIndx /= 3;
  // ���� ��� ������ ����, �� ������ �� ���
  if (ngn == 0 || nvn == 0) {
    vantQuantity = groupQuantity = 0;
    VERTEX_BUFFER_RELEASE(RenderService, vBuf);
    INDEX_BUFFER_RELEASE(RenderService, iBuf);
    delete vlist;
    vlist = nullptr;
    delete gdata;
    gdata = nullptr;
  }
  else if (nvn != vantQuantity || ngn != groupQuantity) {
    vantQuantity = nvn;
    groupQuantity = ngn;
    VERTEX_BUFFER_RELEASE(RenderService, vBuf);
    INDEX_BUFFER_RELEASE(RenderService, iBuf);
    vBuf = RenderService->CreateVertexBuffer(VANTVERTEX_FORMAT, nVert * sizeof(VANTVERTEX),D3DUSAGE_WRITEONLY);
    iBuf = RenderService->CreateIndexBuffer(nIndx * 6);
    SetVertexes();
    SetIndex();
  }

  bYesDeleted = false;
  wVantLast = vantQuantity;
  bUse = vantQuantity > 0;
}
