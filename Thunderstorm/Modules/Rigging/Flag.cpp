#include "Flag.h"
#include "defines.h"
#include "sail_msg.h"
#include "Weather_Base.h"
#include "rigging_define.h"
#include "EntityManager.h"

FLAG::FLAG()
{
	bUse = false;
	RenderService = nullptr;
	TextureName = nullptr;
	bFirstRun = true;
	texl = -1;
	flist = nullptr;
	flagQuantity = 0;
	gdata = nullptr;
	groupQuantity = 0;
	bYesDeleted = false;
	wFlagLast = 0;
	vBuf = iBuf = -1;
	nVert = nIndx = 0;
}

FLAG::~FLAG()
{
	TEXTURE_RELEASE(RenderService, texl);
	STORM_DELETE(TextureName);
	STORM_DELETE(gdata);
	VERTEX_BUFFER_RELEASE(RenderService, vBuf);
	INDEX_BUFFER_RELEASE(RenderService, iBuf);

	while (flagQuantity > 0)
	{
		flagQuantity--;
		STORM_DELETE(flist[flagQuantity]);
	}
	STORM_DELETE(flist);
}

bool FLAG::Init()
{
	//GUARD(FLAG::FLAG())
	SetDevice();
	//UNGUARD
	return true;
}

void FLAG::SetDevice()
{
	// �������� ������ �������
	RenderService = (VDX9RENDER *)api->CreateService("dx9render");
	if (!RenderService)
	{
		throw std::exception("No service: dx9render");
	}
	globalWind.ang.x = 0.f;
	globalWind.ang.y = 0.f;
	globalWind.ang.z = 1.f;
	globalWind.base = 1.f;
	LoadIni();
	texl = RenderService->TextureCreate(TextureName);
}

bool FLAG::CreateState(ENTITY_STATE_GEN* state_gen)
{
	return true;
}

bool FLAG::LoadState(ENTITY_STATE* state)
{
	return true;
}

void FLAG::Execute(uint32_t Delta_Time)
{
	if (bFirstRun) FirstRun();
	if (bYesDeleted) DoSTORM_DELETE();
	if (bUse)
	{
		//====================================================
		// ���� ��� ������� ���-����, �� ������� ���� �� ����
		WIN32_FIND_DATA wfd;
    const auto h = fio->_FindFirstFile("resource\\ini\\rigging.ini", &wfd);
		if (INVALID_HANDLE_VALUE != h)
		{
      auto ft_new = wfd.ftLastWriteTime;
			fio->_FindClose(h);

			if (CompareFileTime(&ft_old, &ft_new) != 0)
			{
				LoadIni();
			}
		}

		// ������� �������� �����
		if (const auto ei = EntityManager::GetEntityId("weather"))
		{
			auto* wb = (WEATHER_BASE*)EntityManager::GetEntityPointer(ei);
			globalWind.ang.x = wb->GetFloat(whf_wind_angle);
			globalWind.ang.z = cosf(globalWind.ang.x);
			globalWind.ang.x = sinf(globalWind.ang.x);
			globalWind.base = wb->GetFloat(whf_wind_speed) / fWindMaxValue;
		}

		// ������ ����� �����
		vertBuf = (FLAGLXVERTEX*)RenderService->LockVertexBuffer(vBuf);
		if (vertBuf)
		{
      const auto dt = (float)Delta_Time * 0.02f;
			for (auto fn = 0; fn < flagQuantity; fn++)
				DoMove(flist[fn], dt);
			RenderService->UnLockVertexBuffer(vBuf);
		}
	}
}

void FLAG::Realize(uint32_t Delta_Time)
{
	if (bUse)
	{
		//_asm rdtsc _asm mov rtm,eax

		RenderService->TextureSet(0, texl);
		uint32_t ambient;
		RenderService->GetRenderState(D3DRS_AMBIENT, &ambient);
		RenderService->SetRenderState(D3DRS_TEXTUREFACTOR, ambient);
		RenderService->SetTransform(D3DTS_WORLD, &rootMatrix);

		// draw nature flag
		if (nVert != 0 && nIndx != 0) RenderService->DrawBuffer(vBuf, sizeof(FLAGLXVERTEX), iBuf, 0, nVert, 0, nIndx,
		                                                        "ShipFlag");
		//_asm rdtsc  _asm sub eax,rtm _asm mov rtm,eax
		// Print info
		// RenderService->Print(0,220,"Flags tics= %d",rtm);
	}
}

uint64_t FLAG::ProcessMessage(MESSAGE& message)
{
  const auto code = message.Long();

	switch (code)
	{
		//**********************
		// Start Initialisation
		//----------------------
	case MSG_FLAG_INIT:
		{
      const auto eidModel = message.EntityID();
      const auto nNation = message.Long();

			MODEL* host_mdl;
			host_mdl = (MODEL*)EntityManager::GetEntityPointer(eidModel);
			if (host_mdl == nullptr)
			{
				api->Trace("Missing INIT message to FLAG: bad MODEL");
			}

			if (groupQuantity == 0)
			{
				gdata = new GROUPDATA[1];
				if (gdata == nullptr)
					throw std::exception("Not memory allocation");

				groupQuantity = 1;
			}
			else
			{
        const auto oldgdata = gdata;
				gdata = new GROUPDATA[groupQuantity + 1];
				if (gdata == nullptr)
					throw std::exception("Not memory allocation");
				memcpy(gdata, oldgdata, sizeof(GROUPDATA) * groupQuantity);
				delete oldgdata;
				groupQuantity++;
			}
			gdata[groupQuantity - 1].model_id = eidModel;
			gdata[groupQuantity - 1].nation = nNation;
			gdata[groupQuantity - 1].bDeleted = false;

			NODE* nod;
			GEOS::INFO gi;
			GEOS::LABEL gl;
			int i, j;
			for (j = 0; true; j++)
			{
				nod = host_mdl->GetNode(j);
				if (nod == nullptr || nod->geo == nullptr) break;

				nod->geo->GetInfo(gi);
				for (i = 0; i < gi.nlabels; i++)
				{
					nod->geo->GetLabel(i, gl);
					if (!strncmp(gl.group_name, "flag", 4))
						AddLabel(gl, nod);
				}
			}
			bFirstRun = true;
		}
		break;

	case MSG_FLAG_DEL_GROUP:
		GroupSTORM_DELETE(message.EntityID());
		break;

	case MSG_FLAG_TO_NEWHOST: // format "lili" (msg_code,oldmodel_id,groupNum,newmodel_id)
		{
      const auto oldModelID = message.EntityID();
      const auto flagGroupNum = message.Long();
      const auto newModelID = message.EntityID();
			MoveOtherHost(newModelID, flagGroupNum, oldModelID);
		}
		break;
	}
	return 0;
}

void FLAG::SetTextureCoordinate() const {
	if (bUse)
	{
		int i;
		long sIdx;
		float stu, addtu, dtu;
		float stv, dtv;

		auto* pv = (FLAGLXVERTEX*)RenderService->LockVertexBuffer(vBuf);
		if (pv)
		{
			for (auto fn = 0; fn < flagQuantity; fn++)
			{
				if (flist[fn] == nullptr) continue;
				sIdx = flist[fn]->sv;
				addtu = 1.f / (float)FlagTextureQuantity;
				stu = addtu * flist[fn]->texNum;
				stv = 0.f;
				if (flist[fn]->triangle)
				{
					dtu = addtu / (float)(flist[fn]->vectQuant + 1);
					dtv = .5f / (float)(flist[fn]->vectQuant + 1);
				}
				else
				{
					dtu = addtu / (float)flist[fn]->vectQuant;
					dtv = 0.f;
				}
				for (i = 0; i <= flist[fn]->vectQuant; i++)
				{
					pv[sIdx].tu = stu;
					pv[sIdx++].tv = stv;
					pv[sIdx].tu = stu;
					pv[sIdx++].tv = 1.f - stv;

					stu += dtu;
					stv += dtv;
				}
				if (flist[fn]->triangle)
				{
					pv[sIdx].tv = stv;
					pv[sIdx].tu = stu;
				}
			}
			RenderService->UnLockVertexBuffer(vBuf);
		}
	}
}

void FLAG::DoMove(FLAGDATA* pr, float delta_time) const {
	if (pr == nullptr) return;
	CVECTOR cPos;
	cPos = *pr->pMatWorld * pr->spos;

  auto xMul = globalWind.ang.x;
  auto zMul = globalWind.ang.z;

	if ((pr->Alfa += (ALFA_DEPEND + ALFA_RAND * (float)rand() / (float)RAND_MAX) * delta_time) > PIm2) pr->Alfa = 0.f;
	if ((pr->Beta += (BETA_DEPEND + BETA_RAND * (float)rand() / (float)RAND_MAX) * delta_time) > PIm2) pr->Beta = 0.f;
  auto Alfa = -pr->Alfa;
  auto Beta = -pr->Beta;
  auto sAlfa = sinf(Alfa);
  auto cAlfa = cosf(Alfa);
  auto sBeta = sinf(Beta);
  auto cBeta = cosf(Beta);

	CVECTOR dhVect;
	dhVect = *pr->pMatWorld * pr->dhv - pr->pMatWorld->Pos();

	long sIdx = pr->sv;
	CVECTOR dopVect;
	CVECTOR incrVect;
	incrVect.x = FLAGVECTORLEN * xMul;
	incrVect.z = FLAGVECTORLEN * zMul;
	incrVect.y = -DOWNVAL;

  auto oldSa = sAlfa;
  auto oldCa = cAlfa;
  auto oldSb = sBeta;
  auto oldCb = cBeta;
  auto hIncr = pr->dhv.y;

  auto tmp = fAlfaStep + fAlfaMax / (float)pr->vectQuant;
  auto sa = sinf(tmp);
  auto ca = cosf(tmp);
	tmp = fBetaStep + fBetaMax / (float)pr->vectQuant;
  auto sb = sinf(tmp);
  auto cb = cosf(tmp);

	for (auto i = 0; i <= pr->vectQuant; i++)
	{
		tmp = fWindAm * (sAlfa - oldSa);
		dopVect.y = fWindAm * (cAlfa - oldCa);
		dopVect.x = tmp * zMul;
		dopVect.z = tmp * xMul;

		vertBuf[sIdx].pos = cPos + dopVect + dhVect;
		sIdx++;
		vertBuf[sIdx].pos = cPos + dopVect - dhVect;
		sIdx++;

		// ��������� ����� ���� (������\��������)
		tmp = sAlfa;
		sAlfa = sAlfa * ca + sa * cAlfa;
		cAlfa = cAlfa * ca - tmp * sa;

		// ��������� ����� ���� (������\��������)
		tmp = sBeta;
		sBeta = sBeta * cb + sb * cBeta;
		cBeta = cBeta * cb - tmp * sb;

		hIncr -= pr->ddhv.y;
		tmp = hIncr * (sBeta - oldSb) * fRotAm;
		if (tmp > hIncr || tmp < -hIncr)
		{
			dhVect.y = 0.f;
			dhVect.x = hIncr * zMul;
			dhVect.z = hIncr * xMul;
		}
		else
		{
      auto fT = hIncr * hIncr - tmp * tmp;
			if (fT < 0.f) fT = -fT;
			dhVect.y = sqrtf(hIncr * hIncr - tmp * tmp);
			dhVect.x = tmp * zMul;
			dhVect.z = tmp * xMul;
		}
		dhVect = *pr->pMatWorld * dhVect - pr->pMatWorld->Pos();

		cPos += incrVect;
	}

	if (pr->triangle)
	{
		dopVect.x = fWindAm * (sAlfa - oldSa);
		dopVect.z = dopVect.x * xMul;
		dopVect.x *= zMul;
		dopVect.y = -fWindAm * (cAlfa - oldCa);
		vertBuf[sIdx].pos = cPos + dopVect;
	}
}

void FLAG::AddLabel(GEOS::LABEL& gl, NODE* nod)
{
	FLAGDATA* fd;
	int grNum;

	// for fail parameters do not set of data
	if (nod == nullptr) return;

	grNum = atoi(&gl.group_name[4]);

	int fn;
	for (fn = 0; fn < flagQuantity; fn++)
		if (flist[fn] != nullptr &&
			flist[fn]->HostGroup == groupQuantity - 1 &&
			flist[fn]->grNum == grNum &&
			flist[fn]->nod == nod)
		{
			fd = flist[fn];
			break;
		}
	if (fn == flagQuantity)
	{
		// create new flag
		fd = new FLAGDATA;
		if (fd == nullptr)
			throw std::exception("Not memory allocation");
		PZERO(fd, sizeof(FLAGDATA));
		fd->triangle = true; // this is Vimpel
		fd->pMatWorld = &nod->glob_mtx;
		fd->nod = nod;
		fd->grNum = grNum;
		fd->Alfa = 0.f;
		fd->Beta = 0.f;
		fd->HostGroup = groupQuantity - 1;
		fd->bDeleted = false;

		if (flagQuantity == 0)
		{
			flist = new FLAGDATA*[1];
			if (flist == nullptr)
				throw std::exception("Not memory allocation");
			flagQuantity = 1;
		}
		else
		{
      const auto oldflist = flist;
			flist = new FLAGDATA*[flagQuantity + 1];
			if (flist == nullptr)
				throw std::exception("Not memory allocation");
			memcpy(flist, oldflist, sizeof(FLAGDATA*) * flagQuantity);
			delete oldflist;
			flagQuantity++;
		}
		flist[flagQuantity - 1] = fd;
	}

	CVECTOR cv;
	cv.x = gl.m[3][0] + nod->loc_mtx.matrix[3];
	cv.y = gl.m[3][1] + nod->loc_mtx.matrix[7];
	cv.z = gl.m[3][2] + nod->loc_mtx.matrix[11];
	/*cv.x = gl.m[3][0];
	cv.y = gl.m[3][1];
	cv.z = gl.m[3][2];*/
	// set flag point
	switch (gl.name[1])
	{
	case '2': // bottom begin of flag
		fd->dhv = cv;
		break;
	case '3': // top end of flag (or center of the flag end for triangle flag)
		fd->ddhv = cv;
		break;
	case '4': // bottom ebd of flag
		fd->dv = cv;
		fd->triangle = false; // this not Vimpel - that Flag
		break;
	default: // case 1: top begin of flag
		fd->spos = cv;
	}
}

void FLAG::SetAll()
{
	// set vertex and index buffers
	nVert = nIndx = 0;
	SetAdd(0);
}

void FLAG::SetTreangle() const {
	int i, idx;

	auto* pt = (uint16_t*)RenderService->LockIndexBuffer(iBuf);
	if (pt)
	{
		for (auto fn = 0; fn < flagQuantity; fn++)
		{
			if (flist[fn] == nullptr) continue;
			idx = flist[fn]->st;
			for (i = 0; i < (int)flist[fn]->nt; i++)
			{
				pt[idx++] = (uint16_t)(i + flist[fn]->sv);
				pt[idx++] = (uint16_t)(i + flist[fn]->sv + 1);
				pt[idx++] = (uint16_t)(i + flist[fn]->sv + 2);
			}
		}

		RenderService->UnLockIndexBuffer(iBuf);
	}
}

void FLAG::LoadIni()
{
	//GUARD(FLAG::LoadIni());
	char section[256];
	char param[256];

	INIFILE* ini;
	WIN32_FIND_DATA wfd;
  const auto h = fio->_FindFirstFile("resource\\ini\\rigging.ini", &wfd);
	if (INVALID_HANDLE_VALUE != h)
	{
		ft_old = wfd.ftLastWriteTime;
		fio->_FindClose(h);
	}
	ini = fio->OpenIniFile("resource\\ini\\rigging.ini");
	if (!ini) throw std::exception("rigging.ini file not found!");

	sprintf_s(section, "FLAGS");

  auto texChange = false;
	int tmp;
	// load texture parameters
	ini->ReadString(section, "TextureName", param, sizeof(param) - 1, "flagall.tga");
	if (TextureName != nullptr)
	{
		if (strcmp(TextureName, param))
		{
			delete TextureName;
			const auto len = strlen(param) + 1;
			TextureName = new char[len];
			memcpy(TextureName, param, len);
			RenderService->TextureRelease(texl);
			texl = RenderService->TextureCreate(TextureName);
		}
	}
	else
	{
		const auto len = strlen(param) + 1;
		TextureName = new char[len];
		memcpy(TextureName, param, len);
	}


	tmp = FlagTextureQuantity;
	FlagTextureQuantity = (int)ini->GetLong(section, "TextureCount", 1);
	if (FlagTextureQuantity != tmp) texChange = true;
	//---
	tmp = ENGLISH_PENNON_TEX;
	ENGLISH_PENNON_TEX = (int)ini->GetLong(section, "EnglandPennonTex", 0);
	if (ENGLISH_PENNON_TEX != tmp) texChange = true;
	//---
	tmp = SPANISH_PENNON_TEX;
	SPANISH_PENNON_TEX = (int)ini->GetLong(section, "SpanishPennonTex", 0);
	if (SPANISH_PENNON_TEX != tmp) texChange = true;
	//---
	tmp = FRENCH_PENNON_TEX;
	FRENCH_PENNON_TEX = (int)ini->GetLong(section, "FrenchPennonTex", 0);
	if (FRENCH_PENNON_TEX != tmp) texChange = true;
	//---
	tmp = PIRATE_PENNON_TEX;
	PIRATE_PENNON_TEX = (int)ini->GetLong(section, "PiratePennonTex", 0);
	if (PIRATE_PENNON_TEX != tmp) texChange = true;
	//---
	tmp = ENGLISH_FLAG_TEX;
	ENGLISH_FLAG_TEX = (int)ini->GetLong(section, "EnglishFlagTex", 0);
	if (ENGLISH_FLAG_TEX != tmp) texChange = true;
	//---
	tmp = SPANISH_FLAG_TEX;
	SPANISH_FLAG_TEX = (int)ini->GetLong(section, "SpanishFlagTex", 0);
	if (SPANISH_FLAG_TEX != tmp) texChange = true;
	//---
	tmp = FRENCH_FLAG_TEX;
	FRENCH_FLAG_TEX = (int)ini->GetLong(section, "FrenchFlagTex", 0);
	if (FRENCH_FLAG_TEX != tmp) texChange = true;
	//---
	tmp = PIRATE_FLAG_TEX;
	PIRATE_FLAG_TEX = (int)ini->GetLong(section, "PirateFlagTex", 0);
	if (PIRATE_FLAG_TEX != tmp) texChange = true;

	SetTextureCoordinate();

	// ����� �������� �����
	FLAGVECTORLEN = ini->GetFloat(section, "fSegLen", 0.2f);

	// ��������� ������� �����
	// �������� ����� �� ���������
	ALFA_DEPEND = ini->GetFloat(section, "alfa_depend", 0.1f);
	// �������� ����� �� �����������
	BETA_DEPEND = ini->GetFloat(section, "beta_depend", 0.06f);
	// ������������ �������� ���������� ��������� ���� Alpha
	ALFA_RAND = ini->GetFloat(section, "alpha_rand", 0.1f);
	// ������������ �������� ���������� ��������� ���� Beta
	BETA_RAND = ini->GetFloat(section, "beta_rand", 0.1f);
	// ������������ ���� (��� ����� �����)
	fAlfaMax = ini->GetFloat(section, "fAlfaMax", 4.71f);
	fAlfaStep = ini->GetFloat(section, "fAlfaStep", .2f);
	// ������������ ���� (��� ����� �����)
	fBetaMax = ini->GetFloat(section, "fBetaMax", 7.85f);
	fBetaStep = ini->GetFloat(section, "fBetaStep", .3f);
	// ��������� ������������ ����� ����� � ��������� ���
	// ��������� ��������� ������������ ����� �����
	fWindAm = ini->GetFloat(section, "fWindAm", 0.2f);
	// ��������� ���������(��������) ����������� �����
	fRotAm = ini->GetFloat(section, "fRotAm", 0.2f);
	// �������� ��������� ����� �� ������ ���� �� �����
	DOWNVAL = ini->GetFloat(section, "fDownVal", 0.05f);
	// ������������ �������� �����
	fWindMaxValue = ini->GetFloat(section, "fWindMaxValue", 12.f);
	// ����������� ���������� ��������� �� �����
	MinSegmentQuantity = ini->GetLong(section, "MinSegQuantity", 4);

	delete ini;
	//UNGUARD
}

void FLAG::FirstRun()
{
	if (wFlagLast)
		SetAdd(wFlagLast);
	else
	{
		SetAll();

		ZERO(rootMatrix);
		rootMatrix._11 = rootMatrix._22 = rootMatrix._33 = rootMatrix._44 = 1.f;
	}

	if (nVert)
	{
		bUse = true;
		iBuf = RenderService->CreateIndexBuffer(nIndx * 2);
		SetTreangle();
		vBuf = RenderService->CreateVertexBuffer(FLAGLXVERTEX_FORMAT, nVert * sizeof(FLAGLXVERTEX),D3DUSAGE_WRITEONLY);
		SetTextureCoordinate();
		nIndx /= 3;
	}

	bFirstRun = false;
	wFlagLast = flagQuantity;
}

void FLAG::GroupSTORM_DELETE(entid_t m_id)
{
	// ������ ������ ��������������� ���������� ������
	for (auto gn = 0; gn < groupQuantity; gn++)
		if (gdata[gn].model_id == m_id)
		{
			gdata[gn].bDeleted = true; // ������� �� ��� ���������
			bYesDeleted = true;
			break;
		}
}

void FLAG::DoSTORM_DELETE()
{
	// �������� �� ���� ��������� ������� � ������� ������������ � ��� �����
  auto ngn = 0;
	nVert = 0;
	nIndx = 0;
	for (auto gn = 0; gn < groupQuantity; gn++)
	{
    auto gs = 0; // ����� ����������� ������ � ������
		for (auto fn = 0; fn < flagQuantity; fn++)
			if (flist[fn] != nullptr && flist[fn]->HostGroup == gn)
			{
				// ���� ���� ������� ���������, �� ������� ���������
				if (gdata[gn].bDeleted || flist[fn]->bDeleted || flist[fn]->nv <= 0 || flist[fn]->nt <= 0)
				{
					flist[fn]->bDeleted = true;
					continue;
				}

				flist[fn]->HostGroup = ngn;
				flist[fn]->sv = nVert;
				flist[fn]->st = nIndx;
				nVert += flist[fn]->nv;
				nIndx += flist[fn]->nt * 3;
				gs++;
			}
		// ���� ��� ����� ������� �� ������, �� ������� ������
		if (gs == 0)
			continue;

		// �������� ������ �� ���� �������
		if (ngn < gn)
			memcpy(&gdata[ngn], &gdata[gn], sizeof(GROUPDATA));
		ngn++;
	}

  auto nfn = 0;
	for (auto fn = 0; fn < flagQuantity; fn++)
	{
		if (flist[fn] == nullptr) continue;
		if (flist[fn]->bDeleted)
		{
			delete flist[fn];
			flist[fn] = nullptr;
			continue;
		}

		flist[nfn++] = flist[fn];
	}

	nIndx /= 3;
	if (nfn == 0 || ngn == 0)
	{
		VERTEX_BUFFER_RELEASE(RenderService, vBuf);
		INDEX_BUFFER_RELEASE(RenderService, iBuf);
		flagQuantity = groupQuantity = 0;
		delete flist;
		flist = nullptr;
		delete gdata;
		gdata = nullptr;
	}
	else if (nfn != flagQuantity || ngn != groupQuantity)
	{
		VERTEX_BUFFER_RELEASE(RenderService, vBuf);
		INDEX_BUFFER_RELEASE(RenderService, iBuf);
		vBuf = RenderService->CreateVertexBuffer(FLAGLXVERTEX_FORMAT, nVert * sizeof(FLAGLXVERTEX),D3DUSAGE_WRITEONLY);
		iBuf = RenderService->CreateIndexBuffer(nIndx * 6);

		flagQuantity = nfn;
		groupQuantity = ngn;

    const auto oldflist = flist;
		flist = new FLAGDATA*[flagQuantity];
		if (flist)
		{
			memcpy(flist, oldflist, sizeof(FLAGDATA*) * flagQuantity);
			delete oldflist;
		}
		else
			flist = oldflist;

    const auto oldgdata = gdata;
		gdata = new GROUPDATA[groupQuantity];
		if (gdata)
		{
			memcpy(gdata, oldgdata, sizeof(GROUPDATA) * groupQuantity);
			delete oldgdata;
		}
		else
			gdata = oldgdata;

		SetTreangle();
		SetTextureCoordinate();
	}

	wFlagLast = flagQuantity;
	bYesDeleted = false;
	bUse = flagQuantity > 0;
}

void FLAG::SetAdd(int flagNum)
{
	// set vertex and index buffers
	nIndx *= 3;
	for (auto fn = flagNum; fn < flagQuantity; fn++)
	{
		if (flist[fn] == nullptr) continue;
		CVECTOR bmpos, empos;
		float len;

    auto p0 = flist[fn]->spos;
    auto p1 = flist[fn]->dv;
    auto p2 = flist[fn]->dhv;
    auto p3 = flist[fn]->ddhv;
		flist[fn]->spos = bmpos = (p0 + p1) * .5f; // �������� ���������� ���� �����
		// ��������� �������� ��������� ���� �����
		if (flist[fn]->triangle) empos = p2;
		else empos = (p2 + p3) * .5f;
		flist[fn]->dhv = p0 - bmpos;
		len = sqrtf(~(empos - bmpos)); // ����� �����
		if (len < FLAGVECTORLEN)
		{
			delete flist[fn];
			if (flagQuantity == 1)
			{
				delete flist;
				flist = nullptr;
				flagQuantity = 0;
			}
			else
			{
        const auto oldflist = flist;
				flagQuantity--;
				flist = new FLAGDATA*[flagQuantity];
				if (flist == nullptr)
					flist = oldflist;
				if (fn > 0)
					memcpy(flist, oldflist, sizeof(FLAGDATA*) * fn);
				if (fn < flagQuantity)
					memcpy(&flist[fn], &oldflist[fn + 1], sizeof(FLAGDATA*) * (flagQuantity - fn));
				if (flist != oldflist)
					delete oldflist;
			}
		}
		else
		{
			// ���������� ����� ��������
      auto pvdat = api->Event("GetRiggingData", "sll", "GetFlagTexNum", flist[fn]->triangle,
                              gdata[flist[fn]->HostGroup].nation);
			if (pvdat == nullptr) flist[fn]->texNum = 0;
			else flist[fn]->texNum = pvdat->GetLong();

			flist[fn]->vectQuant = (int)(len / FLAGVECTORLEN); // ����� ��������� �����
			if (flist[fn]->vectQuant < MinSegmentQuantity)
				flist[fn]->vectQuant = MinSegmentQuantity;
			// ��������� ���������� �����
			flist[fn]->dv = (empos - bmpos) / (float)flist[fn]->vectQuant;
			if (flist[fn]->triangle)
				flist[fn]->ddhv = (p2 - p0 - empos + bmpos) / (float)(flist[fn]->vectQuant);
			else
				flist[fn]->ddhv = (p2 - p0 - empos + bmpos) / (float)(flist[fn]->vectQuant + 1);

			flist[fn]->sv = nVert;
			flist[fn]->st = nIndx;
			flist[fn]->vectQuant; //~!~
			if (flist[fn]->triangle)
			{
				nVert += (flist[fn]->nv = flist[fn]->vectQuant * 2 + 3);
				nIndx += (flist[fn]->nt = flist[fn]->vectQuant * 2 + 1) * 3;
			}
			else
			{
				nVert += (flist[fn]->nv = flist[fn]->vectQuant * 2 + 2);
				nIndx += (flist[fn]->nt = flist[fn]->vectQuant * 2) * 3;
			}
		}
	}
	// ������ ������ ������
	VERTEX_BUFFER_RELEASE(RenderService, vBuf);
	INDEX_BUFFER_RELEASE(RenderService, iBuf);
}

void FLAG::MoveOtherHost(entid_t newm_id, long flagNum, entid_t oldm_id)
{
	// ������ ������ ������
	int oldgn;
	for (oldgn = 0; oldgn < groupQuantity; oldgn++)
		if (gdata[oldgn].model_id == oldm_id) break;
	// ���� ��� ����� ������, �� ������ �������
	if (oldgn == groupQuantity)
		return;

	// ������ ����� ������
	int newgn;
	for (newgn = 0; newgn < groupQuantity; newgn++)
		if (gdata[newgn].model_id == newm_id) break;
	// ���� ��� ����� ������, �� ������� ��
	if (newgn == groupQuantity)
	{
    const auto oldgdata = gdata;
		gdata = new GROUPDATA[groupQuantity + 1];
		if (gdata == nullptr)
			throw std::exception("Not memory allocation");
		memcpy(gdata, oldgdata, sizeof(GROUPDATA) * groupQuantity);
		delete oldgdata;
		groupQuantity++;

		// �������� ��������� ����� ������
		gdata[newgn].bDeleted = false;
		gdata[newgn].model_id = newm_id;
		gdata[newgn].nation = gdata[oldgn].nation;
	}

	// ������ ������ ��� ����
	int fn;
	for (fn = 0; fn < flagQuantity; fn++)
		if (flist[fn] != nullptr && flist[fn]->grNum == flagNum && flist[fn]->HostGroup == oldgn) break;
	// ������������ ��� ������� �� ������ �������
	if (fn < flagQuantity)
		flist[fn]->HostGroup = newgn;
}
