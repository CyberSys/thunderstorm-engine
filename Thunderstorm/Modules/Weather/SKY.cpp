#include "SKY.h"
#include "SunGlow.h"

SKY::SKY()
{
	iSkyVertsID = -1;
	iSkyIndexID = -1;
	iFogVertsID = -1;
	iFogIndexID = -1;
	fAngleY = 0.0f;
	pRS = nullptr;
	memset(TexturesID, -1, sizeof(TexturesID));
	memset(TexturesNextID, -1, sizeof(TexturesNextID));
	pAstronomy = nullptr;
	pSunGlow = nullptr;
}

SKY::~SKY()
{
	Release();
}

void SKY::Release()
{
	for (long i = 0; i < SKY_NUM_TEXTURES; i++)
	{
		if (TexturesID[i] >= 0)
		{
			pRS->TextureRelease(TexturesID[i]);
			TexturesID[i] = -1;
		}
		if (TexturesNextID[i] >= 0)
		{
			pRS->TextureRelease(TexturesNextID[i]);
			TexturesNextID[i] = -1;
		}
	}

	if (iSkyVertsID >= 0) pRS->ReleaseVertexBuffer(iSkyVertsID);
	iSkyVertsID = -1;
	if (iSkyIndexID >= 0) pRS->ReleaseIndexBuffer(iSkyIndexID);
	iSkyIndexID = -1;

	if (iFogVertsID >= 0) pRS->ReleaseVertexBuffer(iFogVertsID);
	iFogVertsID = -1;
	if (iFogIndexID >= 0) pRS->ReleaseIndexBuffer(iFogIndexID);
	iFogIndexID = -1;
}

bool SKY::Init()
{
	SetDevice();
	return true;
}

void SKY::SetDevice()
{
	pRS = (VDX9RENDER *)api->CreateService("dx9render");
	Assert(pRS);
}

uint32_t SKY::CalcFogDiffuse(CVECTOR& vPos)
{
  auto fAlpha = vPos.y / 500.0f;
	fAlpha = CLAMP(fAlpha);
  const auto dwAlpha = 255 - uint32_t(fAlpha * 255.0f);
	return ARGB(dwAlpha, 167, 153, 196);
}

void SKY::CreateFogSphere()
{
	long x, y, iNumLevels, iNumAngles, idx = 0;

  const auto R = 2000.0f;
	iNumLevels = 64;
	iNumAngles = 8;
	iFogNumVerts = iNumAngles * iNumLevels + 1;
	auto* pVerts = new FOGVERTEX[iFogNumVerts];
	iFogVertsID = pRS->CreateVertexBuffer(FOGVERTEX_FORMAT, iFogNumVerts * sizeof(SKYVERTEX),D3DUSAGE_WRITEONLY);
	auto* pVertBuf = (FOGVERTEX*)pRS->LockVertexBuffer(iFogVertsID);

	iFogNumTrgs = 3 * (iNumAngles + (iNumLevels - 1) * iNumAngles * 2);
	iFogIndexID = pRS->CreateIndexBuffer(iFogNumTrgs * 2);

	auto* pTrgs = (uint16_t*)pRS->LockIndexBuffer(iFogIndexID);
	for (y = 0; y < iNumLevels; y++)
	{
    const auto h = y * R / iNumLevels;
    const auto R1 = sqrtf(SQR(R) - SQR(h));
		for (x = 0; x < iNumAngles; x++)
		{
      const auto fCos = cosf(float(x) / float(iNumAngles) * PIm2);
      const auto fSin = sinf(float(x) / float(iNumAngles) * PIm2);
      const auto vPos = CVECTOR(R1 * fCos, h, R1 * fSin);
			pVerts[idx].pos = vPos;
      auto* pvData = api->Event(WEATHER_CALC_FOG_COLOR, "fff", vPos.x, vPos.y, vPos.z);
			Assert(pvData);
			pVerts[idx].diffuse = (uint32_t)pvData->GetLong();
			//pVerts[idx].diffuse = CalcFogDiffuse(pVerts[idx].pos);

			if (y == iNumLevels - 1)
			{
				*pTrgs++ = uint16_t(iFogNumVerts - 1);
				*pTrgs++ = uint16_t(idx);
				*pTrgs++ = uint16_t((x == iNumAngles - 1) ? idx - (iNumAngles - 1) : idx + 1);
			}
			else
			{
				*pTrgs++ = uint16_t(idx);
				*pTrgs++ = uint16_t((x == iNumAngles - 1) ? idx - (iNumAngles - 1) : idx + 1);
				*pTrgs++ = uint16_t(idx + iNumAngles);

				*pTrgs++ = uint16_t((x == iNumAngles - 1) ? idx - (iNumAngles - 1) : idx + 1);
				*pTrgs++ = uint16_t(idx + iNumAngles);
				*pTrgs++ = uint16_t((x == iNumAngles - 1) ? idx + 1 : idx + iNumAngles + 1);
			}
			idx++;
		}
	}
  const auto vPos = CVECTOR(0.0f, R, 0.0f);
	pVerts[idx].pos = vPos;
  auto* pvData = api->Event(WEATHER_CALC_FOG_COLOR, "fff", vPos.x, vPos.y, vPos.z);
	Assert(pvData);
	pVerts[idx].diffuse = (uint32_t)pvData->GetLong();
	if (pVerts) memcpy(pVertBuf, pVerts, iFogNumVerts * sizeof(FOGVERTEX));
	pRS->UnLockVertexBuffer(iFogVertsID);
	pRS->UnLockIndexBuffer(iFogIndexID);

	delete[] pVerts;
}

void SKY::UpdateFogSphere()
{
	if (iFogVertsID == -1) return;

	long x, y, iNumLevels, iNumAngles, idx = 0;

	CVECTOR vPos;
  const auto R = 2000.0f;
	iNumLevels = 64;
	iNumAngles = 8;
	iFogNumVerts = iNumAngles * iNumLevels + 1;
  auto* const pVertBuf = (FOGVERTEX*)pRS->LockVertexBuffer(iFogVertsID);

	for (y = 0; y < iNumLevels; y++)
	{
    const auto h = y * R / iNumLevels;
    const auto R1 = sqrtf(SQR(R) - SQR(h));
		for (x = 0; x < iNumAngles; x++)
		{
      const auto fCos = cosf(float(x) / float(iNumAngles) * PIm2);
      const auto fSin = sinf(float(x) / float(iNumAngles) * PIm2);
			vPos = CVECTOR(R1 * fCos, h, R1 * fSin);
			pVertBuf[idx].pos = vPos;
      auto* pvData = api->Event(WEATHER_CALC_FOG_COLOR, "fff", vPos.x, vPos.y, vPos.z);
			Assert(pvData);
			pVertBuf[idx].diffuse = (uint32_t)pvData->GetLong();

			idx++;
		}
	}
	vPos = CVECTOR(0.0f, R, 0.0f);
	pVertBuf[idx].pos = vPos;
  auto* pvData = api->Event(WEATHER_CALC_FOG_COLOR, "fff", vPos.x, vPos.y, vPos.z);
	Assert(pvData);
	pVertBuf[idx].diffuse = (uint32_t)pvData->GetLong();
	pRS->UnLockVertexBuffer(iFogVertsID);
}

void SKY::GenerateSky()
{
	SKYVERTEX Verts[SKY_NUM_VERTEX];
	long i;
	SKYVERTEX v[8];

	Release();

	v[0].pos = CVECTOR(-1.0f, 0.0f, -1.0f);
	v[1].pos = CVECTOR(1.0f, 0.0f, -1.0f);
	v[2].pos = CVECTOR(1.0f, 0.0f, 1.0f);
	v[3].pos = CVECTOR(-1.0f, 0.0f, 1.0f);
	v[4].pos = CVECTOR(-1.0f, 1.0f, -1.0f);
	v[5].pos = CVECTOR(1.0f, 1.0f, -1.0f);
	v[6].pos = CVECTOR(1.0f, 1.0f, 1.0f);
	v[7].pos = CVECTOR(-1.0f, 1.0f, 1.0f);

  const auto fpdelta = 1.0f / 1024.0f;
  const auto fpdx = 1.0f - fpdelta; //.25f - fpdelta;
  const auto fp0 = fpdelta;
  auto fp1 = 0.25f + fpdelta;
  auto fp2 = 0.5f + fpdelta;
  auto fp3 = 0.75f + fpdelta;
  const auto fp4 = 1.0f - fpdelta;

	Verts[0] = v[1];
	Verts[0].tu = fp4;
	Verts[0].tv = fp0 + fpdx;
	Verts[1] = v[5];
	Verts[1].tu = fp4;
	Verts[1].tv = fp0;
	Verts[2] = v[4];
	Verts[2].tu = fp0;
	Verts[2].tv = fp0;
	Verts[3] = v[0];
	Verts[3].tu = fp0;
	Verts[3].tv = fp0 + fpdx;

	Verts[4] = v[2];
	Verts[4].tu = fp4;
	Verts[4].tv = fp0 + fpdx;
	Verts[5] = v[6];
	Verts[5].tu = fp4;
	Verts[5].tv = fp0;
	Verts[6] = v[5];
	Verts[6].tu = fp0;
	Verts[6].tv = fp0;
	Verts[7] = v[1];
	Verts[7].tu = fp0;
	Verts[7].tv = fp0 + fpdx;

	Verts[8] = v[3];
	Verts[8].tu = fp4;
	Verts[8].tv = fp0 + fpdx;
	Verts[9] = v[7];
	Verts[9].tu = fp4;
	Verts[9].tv = fp0;
	Verts[10] = v[6];
	Verts[10].tu = fp0;
	Verts[10].tv = fp0;
	Verts[11] = v[2];
	Verts[11].tu = fp0;
	Verts[11].tv = fp0 + fpdx;

	Verts[12] = v[0];
	Verts[12].tu = fp4;
	Verts[12].tv = fp0 + fpdx;
	Verts[13] = v[4];
	Verts[13].tu = fp4;
	Verts[13].tv = fp0;
	Verts[14] = v[7];
	Verts[14].tu = fp0;
	Verts[14].tv = fp0;
	Verts[15] = v[3];
	Verts[15].tu = fp0;
	Verts[15].tv = fp0 + fpdx;

	Verts[16] = v[5];
	Verts[16].tu = fp0;
	Verts[16].tv = fp0;
	Verts[17] = v[6];
	Verts[17].tu = fp0;
	Verts[17].tv = fp4;
	Verts[18] = v[7];
	Verts[18].tu = fp4;
	Verts[18].tv = fp4;
	Verts[19] = v[4];
	Verts[19].tu = fp4;
	Verts[19].tv = fp0;

	for (i = 0; i < 20; i++)
	{
		Verts[i].diffuse = 0xFFFFFFFF;
		Verts[i].pos *= fSkySize;
		Verts[i + 20] = Verts[i];
		Verts[i + 20].pos.y = -Verts[i + 20].pos.y;
		Verts[i].tu2 = Verts[i].tu;
		Verts[i].tv2 = Verts[i].tv;
	}
	iSkyVertsID = pRS->CreateVertexBuffer(SKYVERTEX_FORMAT, SKY_NUM_VERTEX * sizeof(SKYVERTEX), D3DUSAGE_WRITEONLY);
	iSkyIndexID = pRS->CreateIndexBuffer(20 * 3 * 2);
	auto* pVertBuf = (SKYVERTEX*)pRS->LockVertexBuffer(iSkyVertsID);
	if (pVertBuf) memcpy(pVertBuf, &Verts[0], sizeof(Verts));
	pRS->UnLockVertexBuffer(iSkyVertsID);

	auto* pTrgs = (uint16_t*)pRS->LockIndexBuffer(iSkyIndexID);
	for (i = 0; i < 10; i++)
	{
		*pTrgs++ = (uint16_t)(i * 4) + 0;
		*pTrgs++ = (uint16_t)(i * 4) + 1;
		*pTrgs++ = (uint16_t)(i * 4) + 2;
		*pTrgs++ = (uint16_t)(i * 4) + 0;
		*pTrgs++ = (uint16_t)(i * 4) + 2;
		*pTrgs++ = (uint16_t)(i * 4) + 3;
	}
	pRS->UnLockIndexBuffer(iSkyIndexID);

	LoadTextures();
	CreateFogSphere();
}

void SKY::LoadTextures()
{
	char str[256];
	const char *names[SKY_NUM_TEXTURES] = { "sky_fr.tga", "sky_lf.tga", "sky_bk.tga", "sky_rt.tga", "sky_up.tga" };

	std::string sSkyDir, sSkyDirNext;
	GetSkyDirStrings(sSkyDir, sSkyDirNext);

	for (long i = 0; i < SKY_NUM_TEXTURES; i++)
	{
		sprintf_s(str, "%s%s", (const char*)sSkyDir.c_str(), names[i]);
		TexturesID[i] = pRS->TextureCreate(str);

		if (aSkyDirArray.size() > 1)
		{
			sprintf_s(str, "%s%s", (const char*)sSkyDirNext.c_str(), names[i]);
			TexturesNextID[i] = pRS->TextureCreate(str);
		}
	}

	fAngleY = 0.0f;
}

bool SKY::CreateState(ENTITY_STATE_GEN* state_gen)
{
	return true;
}

bool SKY::LoadState(ENTITY_STATE* state)
{
	SetDevice();
	return true;
}

void SKY::Realize(uint32_t Delta_Time)
{
	fAngleY += float(Delta_Time) * 0.001f * fSkySpeedRotate;

	float fFov;
	CVECTOR vPos, vAng;
	D3DXMATRIX pMatWorld, pMatTranslate, pMatRotate;

	pRS->GetCamera(vPos, vAng, fFov);

	D3DXMatrixIdentity(&pMatWorld);

	D3DXMatrixTranslation(&pMatTranslate, vPos.x, vPos.y / 6.0f, vPos.z);
	D3DXMatrixMultiply(&pMatWorld, &pMatWorld, &pMatTranslate);
	D3DXMatrixRotationY(&pMatRotate, fAngleY + fSkyAngle);
	D3DXMatrixMultiply(&pMatWorld, &pMatRotate, &pMatWorld);
	pRS->SetTransform(D3DTS_WORLD, &pMatWorld);

	if (aSkyDirArray.size() > 1)
	{
		UpdateTimeFactor();
    auto fBlendFactor = fTimeFactor - (long)fTimeFactor;
		if (fBlendFactor < 0.f) fBlendFactor = 0.f;
		if (fBlendFactor > 1.f) fBlendFactor = 1.f;
		uint32_t dwColor = (dwSkyColor & 0x00FFFFFF) | (((long)(0xFF000000 * fBlendFactor)) & 0xFF000000);
		pRS->SetRenderState(D3DRS_TEXTUREFACTOR, dwColor);

		if (pRS->TechniqueExecuteStart("skyblend"))
			do
			{
				for (long i = 0; i < SKY_NUM_TEXTURES; i++)
				{
					pRS->TextureSet(0, TexturesID[i]);
					pRS->TextureSet(1, TexturesNextID[i]);
					pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 20, i * 6, 2);
					if (Delta_Time == 0)
						pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 40, 30 + i * 6, 2);
				}
			}
			while (pRS->TechniqueExecuteNext());

		if (Delta_Time != 0)
		{
			entid_t eid;

			if (!pAstronomy)
				if (eid = EntityManager::GetEntityId("Astronomy"))
					pAstronomy = (Entity*)EntityManager::GetEntityPointer(eid);

			if (!pSunGlow)
				if (eid = EntityManager::GetEntityId("SUNGLOW"))
					pSunGlow = (Entity*)EntityManager::GetEntityPointer(eid);

			if (pAstronomy || pSunGlow)
			{
				if (pAstronomy) pAstronomy->ProcessStage(Stage::realize, Delta_Time);
				if (pSunGlow) ((SUNGLOW*)pSunGlow)->DrawSunMoon();

				pRS->SetTransform(D3DTS_WORLD, &pMatWorld);
				if (pRS->TechniqueExecuteStart("skyblend_alpha"))
					do
					{
						for (long i = 0; i < SKY_NUM_TEXTURES; i++)
						{
							pRS->TextureSet(0, TexturesID[i]);
							pRS->TextureSet(1, TexturesNextID[i]);
							pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 20, i * 6, 2);
							if (Delta_Time == 0) //~!~
								pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 40, 30 + i * 6, 2);
						}
					}
					while (pRS->TechniqueExecuteNext());
			}
		}

		std::string sSkyPrev, sSkyNext;
		GetSkyDirStrings(sSkyPrev, sSkyNext);
		// debug pRS->Print( 10,10, "time = %.2f(%.2f), skydir = %s,%s", fTimeFactor,fBlendFactor, (const char*)sSkyPrev,(const char*)sSkyNext);
	}
	else
	{
		pRS->SetRenderState(D3DRS_TEXTUREFACTOR, dwSkyColor);

		if (pRS->TechniqueExecuteStart("sky"))
			do
			{
				for (long i = 0; i < SKY_NUM_TEXTURES; i++)
				{
					pRS->TextureSet(0, TexturesID[i]);
					pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 20, i * 6, 2);
					if (Delta_Time == 0)
						pRS->DrawBuffer(iSkyVertsID, sizeof(SKYVERTEX), iSkyIndexID, 0, 40, 30 + i * 6, 2);
				}
			}
			while (pRS->TechniqueExecuteNext());
	}

	D3DXMatrixIdentity(&pMatWorld);
	D3DXMatrixTranslation(&pMatTranslate, vPos.x, vPos.y / 6.0f, vPos.z);
	D3DXMatrixMultiply(&pMatWorld, &pMatWorld, &pMatTranslate);
	pRS->SetTransform(D3DTS_WORLD, &pMatWorld);
	pRS->DrawBuffer(iFogVertsID, sizeof(FOGVERTEX), iFogIndexID, 0, iFogNumVerts, 0, iFogNumTrgs / 3, "skyfog");
}

uint32_t SKY::AttributeChanged(ATTRIBUTES* pAttribute)
{
	//if (*pAttribute == "dir")			{ sSkyDir = pAttribute->GetThisAttr(); return 0; }
	if (*pAttribute == "dir")
	{
		FillSkyDirArray(pAttribute);
		return 0;
	}
	if (*pAttribute == "color")
	{
		dwSkyColor = pAttribute->GetAttributeAsDword();
		return 0;
	}
	if (*pAttribute == "RotateSpeed")
	{
		fSkySpeedRotate = pAttribute->GetAttributeAsFloat();
		return 0;
	}
	if (*pAttribute == "Size")
	{
		fSkySize = pAttribute->GetAttributeAsFloat();
		return 0;
	}
	if (*pAttribute == "Angle")
	{
		fSkyAngle = pAttribute->GetAttributeAsFloat();
		return 0;
	}

	if (*pAttribute == "isDone")
	{
		GenerateSky();
		return 0;
	}

	if (*pAttribute == "TimeUpdate")
	{
		UpdateFogSphere();
		return 0;
	}

	return 0;
}

uint64_t SKY::ProcessMessage(MESSAGE& message)
{
	if (message.Long() == MSG_SEA_REFLECTION_DRAW) Realize(0);
	return 0;
}

void SKY::FillSkyDirArray(ATTRIBUTES* pAttribute)
{
	aSkyDirArray.clear();
  const long q = pAttribute->GetAttributesNum();
	if (q < 1) return;

	//aSkyDirArray.AddElements( q );
	aSkyDirArray.resize(aSkyDirArray.size() + q);
	if (q > 1)
	{
		for (long n = 0; n < q; n++)
		{
      const auto* const attrName = pAttribute->GetAttributeName(n);
			if (attrName && attrName[0] == 'd' && attrName[1] >= '0' && attrName[1] <= '9')
			{
        const auto i = atol(&attrName[1]);
				if (i < q)
					aSkyDirArray[i] = pAttribute->GetAttribute(n);
			}
		}
	}
	else
	{
		aSkyDirArray[0] = pAttribute->GetAttribute((uint32_t)0);
	}
	fTimeFactor = (float)atof(pAttribute->GetThisAttr());
	if (fTimeFactor < 0.f || fTimeFactor > 24.f) fTimeFactor = 0.f;
	fTimeFactor *= (1.f / 24.f) * aSkyDirArray.size();
}

void SKY::GetSkyDirStrings(std::string& sSkyDir, std::string& sSkyDirNext)
{
  auto n1 = (long)fTimeFactor;
	if (n1 >= 0)
	{
		if (n1 >= (long)aSkyDirArray.size())
			n1 -= aSkyDirArray.size();
    auto n2 = n1 + 1;
		if (n2 >= (long)aSkyDirArray.size())
			n2 -= aSkyDirArray.size();

		sSkyDir = aSkyDirArray[n1];
		sSkyDirNext = aSkyDirArray[n2];
		return;
	}
	sSkyDir = sSkyDirNext = "";
}

void SKY::UpdateTimeFactor()
{
  const auto nPrev = (long)fTimeFactor;

	//fTimeFactor += api->GetDeltaTime() * 0.00005f;
	entid_t eid;
	if (!(eid = EntityManager::GetEntityId("weather"))) return;
	fTimeFactor = ((WEATHER_BASE*)EntityManager::GetEntityPointer(eid))->GetFloat(whf_time_counter);
	fTimeFactor *= (1.f / 24.f) * aSkyDirArray.size();

	if ((long)fTimeFactor >= (long)aSkyDirArray.size()) fTimeFactor -= aSkyDirArray.size();
  const auto nNext = (long)fTimeFactor;
	if (nPrev != nNext)
	{
		// ������������ ��������

		char str[256];
	  const char *names[SKY_NUM_TEXTURES] =
			{"sky_fr.tga", "sky_lf.tga", "sky_bk.tga", "sky_rt.tga", "sky_up.tga"};

		std::string sSkyDir, sSkyDirNext;
		GetSkyDirStrings(sSkyDir, sSkyDirNext);

		for (long i = 0; i < SKY_NUM_TEXTURES; i++)
		{
			if (TexturesID[i] >= 0) pRS->TextureRelease(TexturesID[i]);
			TexturesID[i] = TexturesNextID[i];

			sprintf_s(str, "%s%s", (const char*)sSkyDirNext.c_str(), names[i]);
			TexturesNextID[i] = pRS->TextureCreate(str);
		}
	}
}

float SKY::CalculateAlphaForSun(const CVECTOR& vSunPos, float fSunSize)
{
	// get Sky
	float fFov;
	CVECTOR vPos, vAng;
	pRS->GetCamera(vPos, vAng, fFov);

	CMatrix mtxWorld;
	mtxWorld.BuildRotateY(fAngleY + fSkyAngle);
	mtxWorld.Move(vPos.x, vPos.y / 6.0f, vPos.z);

	CVECTOR vLocalSunPos;
	mtxWorld.MulToInv(vSunPos, vLocalSunPos);

	if (vLocalSunPos.y < 0.f) vLocalSunPos.y = 0.f;

	// ������� ���� �� ��������
	if (fSkySize > 0.f)
	{
		long nTexNum = -1;
		float fu, fv, fk;

		// �������� ��������� � ������� �������
		if (vLocalSunPos.y > 0.f)
		{
			fk = fSkySize / vLocalSunPos.y;
			fu = -vLocalSunPos.x * fk;
			fv = vLocalSunPos.z * fk;

			if (fu <= fSkySize && fu >= -fSkySize &&
				fv <= fSkySize && fv >= -fSkySize)
			{
				nTexNum = 4;
				fu = (fu + fSkySize) * 0.5f / fSkySize;
				fv = (fv + fSkySize) * 0.5f / fSkySize;
			}
		}

		// �������� ��������� � �������� �������
		if (nTexNum == -1)
		{
			if (vLocalSunPos.z < 0.f)
			{
				fk = -fSkySize / vLocalSunPos.z;
				fu = vLocalSunPos.x * fk;
				fv = vLocalSunPos.y * fk;

				if (fu <= fSkySize && fu >= -fSkySize &&
					fv <= fSkySize && fv >= 0.f)
				{
					nTexNum = 0;
					fu = (fu + fSkySize) * 0.5f / fSkySize;
					fv = (fSkySize - fv) / fSkySize;
				}
			}
		}

		// �������� ��������� � ������ �������
		if (nTexNum == -1)
		{
			if (vLocalSunPos.z > 0.f)
			{
				fk = fSkySize / vLocalSunPos.z;
				fu = -vLocalSunPos.x * fk;
				fv = vLocalSunPos.y * fk;

				if (fu <= fSkySize && fu >= -fSkySize &&
					fv <= fSkySize && fv >= 0.f)
				{
					nTexNum = 2;
					fu = (fu + fSkySize) * 0.5f / fSkySize;
					fv = (fSkySize - fv) / fSkySize;
				}
			}
		}

		// �������� ��������� � ����� �������
		if (nTexNum == -1)
		{
			if (vLocalSunPos.x > 0.f)
			{
				fk = fSkySize / vLocalSunPos.x;
				fu = vLocalSunPos.z * fk;
				fv = vLocalSunPos.y * fk;

				if (fu <= fSkySize && fu >= -fSkySize &&
					fv <= fSkySize && fv >= 0.f)
				{
					nTexNum = 1;
					fu = (fSkySize + fu) * 0.5f / fSkySize;
					fv = (fSkySize - fv) / fSkySize;
				}
			}
		}

		// �������� ��������� � ������ �������
		if (nTexNum == -1)
		{
			if (vLocalSunPos.x < 0.f)
			{
				fk = -fSkySize / vLocalSunPos.x;
				fu = -vLocalSunPos.z * fk;
				fv = vLocalSunPos.y * fk;

				if (fu <= fSkySize && fu >= -fSkySize &&
					fv <= fSkySize && fv >= 0.f)
				{
					nTexNum = 3;
					fu = (fSkySize + fu) * 0.5f / fSkySize;
					fv = (fSkySize - fv) / fSkySize;
				}
			}
		}

		// ���� ����� � ��������
		if (nTexNum != -1)
		{
      const auto dwCol1 = GetPixelColor((IDirect3DTexture9*)pRS->GetTextureFromID(TexturesID[nTexNum]), fu, fv);
      const auto dwCol2 = GetPixelColor((IDirect3DTexture9*)pRS->GetTextureFromID(TexturesNextID[nTexNum]), fu, fv);

      const auto fK = fTimeFactor - (long)fTimeFactor;
      const auto fAlpha = (1.f - fK) * (dwCol1 >> 24) / 255.f + fK * (dwCol2 >> 24) / 255.f;
			return fAlpha;
		}
	}


	return 1.f;
}

uint32_t SKY::GetPixelColor(IDirect3DTexture9* pTex, float fu, float fv) const {
	HRESULT hok;
  auto dwCol = 0xFFFFFFFF;

	if (!pTex) return dwCol;

	D3DSURFACE_DESC texdesc;
	pRS->GetLevelDesc(pTex, 0, &texdesc);
  const auto x = (long)(texdesc.Width * fu);
  const auto y = (long)(texdesc.Height * fv);

	D3DLOCKED_RECT lockRect;
	if ((hok = pRS->LockRect(pTex, 0, &lockRect, nullptr, D3DLOCK_READONLY)) == D3D_OK)
	{
		auto* pLine = (uint32_t*)((uint8_t*)lockRect.pBits + y * lockRect.Pitch);
		dwCol = pLine[x];
		pRS->UnlockRect(pTex, 0);
	}

	return dwCol;
}
