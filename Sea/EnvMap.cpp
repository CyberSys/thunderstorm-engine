#include "sea.h"
#include "../Shared/messages.h"

void SEA::EnvMap_GetSideMatrix(D3DCUBEMAP_FACES Face, CMatrix & mView)
{
	CVECTOR vLookTo, vUp;

    switch (Face)
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            vLookTo = CVECTOR(1.0f, 0.0f, 0.0f);
            vUp = CVECTOR(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            vLookTo = CVECTOR(-1.0f, 0.0f, 0.0f);
            vUp = CVECTOR(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            vLookTo = CVECTOR(0.0f, 1.0f, 0.0f);
            vUp = CVECTOR(0.0f, 0.0f,-1.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            vLookTo = CVECTOR(0.0f,-1.0f, 0.0f);
            vUp = CVECTOR(0.0f, 0.0f, 1.0f);
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            vLookTo = CVECTOR(0.0f, 0.0f, 1.0f);
            vUp = CVECTOR(0.0f, 1.0f, 0.0f);
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            vLookTo = CVECTOR(0.0f, 0.0f, -1.0f);
            vUp = CVECTOR(0.0f, 1.0f, 0.0f);
            break;
    }

	mView.BuildViewMatrix(0.0f, vLookTo, vUp);
}

bool SEA::SunRoad_Render2()
{
	CMatrix mOldView = rs->GetView();
	CMatrix mOldProjection = rs->GetProjection();

	rs->EndScene();
	rs->PushRenderTarget();

	//rs->SetProjection( CMatrix().BuildProjectionMatrix(PI / 2.0f, 256.0f, 256.0f, 1.0f, 4000.0f) );

	float PlaneHeight = 0.5f;

	D3DXPLANE plane;
	D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0, PlaneHeight, 0), &D3DXVECTOR3(0, 1, 0) );

	D3DXMATRIX matReflect;
	D3DXMatrixReflect( &matReflect, &plane );

	CMatrix mView = rs->GetView();

	CMatrix mViewNew = mView;

	CMatrix Invertor;
	memcpy (Invertor.m, matReflect.m, sizeof(float)* 16);
	mViewNew = Invertor * mViewNew;

	rs->SetView(mViewNew);

	CMatrix _mWorld = CMatrix();
	CMatrix _mView = rs->GetView();
	CMatrix _mProj = rs->GetProjection();
	CMatrix _mWorldView = _mWorld * _mView;
	CMatrix _mWorldViewProj = _mWorldView * _mProj;

	D3DXMATRIX mInv;
	memcpy (mInv.m, _mWorldViewProj.m, sizeof(float)* 16);
	D3DXMatrixInverse(&mInv, nullptr, &mInv);
	D3DXMatrixTranspose(&mInv, &mInv);
	D3DXPlaneTransform(&plane, &plane, &mInv);
	rs->SetClipPlane (0, (FLOAT *)&plane);

	uint32_t dwSkyCode = api->Class_Name2Code("sky");
	uint32_t dwIslandCode = api->Class_Name2Code("island");
	uint32_t dwShipCode = api->Class_Name2Code("ship");
	uint32_t dwSailCode = api->Class_Name2Code("sail");

	bool bLayerFrozen = (api->LayerGetFlags("sea_reflection2") & LRFLAG_FROZEN) != 0;

	uint32_t Colors[6] = {0xd934c8, 0x2FFF1F, 0x0000FF, 0xFF00, 0xb28e11, 0x0};
	//for (uint32_t i=0; i<6; i++) 
	{
		//if (i == D3DCUBEMAP_FACE_NEGATIVE_Y) continue;
		//if (i != D3DCUBEMAP_FACE_NEGATIVE_Z) continue;

		IDirect3DSurface9 * pReflectionSurface;
		pReflectionSunroad->GetSurfaceLevel(0, &pReflectionSurface);

		rs->BeginScene();
		bool bCool = rs->SetRenderTarget(pReflectionSurface, pZStencil);
		bCool = D3D_OK != rs->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x0, 1.0f, 0);

		//CMatrix mView;
		//EnvMap_GetSideMatrix((D3DCUBEMAP_FACES)i, mView);

		//mView.m[3][0] = -(mView.m[0][0] * vCamPos.x - mView.m[1][0] * vCamPos.y + mView.m[2][0] * vCamPos.z);
		//mView.m[3][1] = -(mView.m[0][1] * vCamPos.x - mView.m[1][1] * vCamPos.y + mView.m[2][1] * vCamPos.z);
		//mView.m[3][2] = -(mView.m[0][2] * vCamPos.x - mView.m[1][2] * vCamPos.y + mView.m[2][2] * vCamPos.z);

		//rs->SetView(mView);

		entid_t ent_id;
		// Render scene here.
		if (api->SetEntityScanLayer("sea_sunroad") && api->GetEntity(&ent_id))
			do 
			{
				uint32_t dwCCode = ent_id.class_code;
				if (!bLayerFrozen || (dwCCode != dwShipCode && dwCCode != dwSailCode && dwCCode != dwIslandCode)) 
				{
					api->Send_Message(ent_id, "ll", MSG_SEA_SUNROAD_DRAW, long(bSimpleSea));
				}
			} while (api->GetEntityNext(&ent_id));

		rs->EndScene();
		pReflectionSurface->Release();
	}

	rs->PopRenderTarget();
	rs->SetView(mOldView);
	rs->SetProjection(mOldProjection);

	rs->BeginScene();
	mOldView = rs->GetView();

	return true;
}

bool SEA::EnvMap_Render2()
{
	CMatrix mOldView = rs->GetView();
	CMatrix mOldProjection = rs->GetProjection();
	rs->EndScene();
	rs->PushRenderTarget();

	//rs->SetProjection( CMatrix().BuildProjectionMatrix(PI / 2.0f, 256.0f, 256.0f, 1.0f, 4000.0f) );

	float PlaneHeight = 0.5f;

	D3DXPLANE plane;
	D3DXPlaneFromPointNormal( &plane, &D3DXVECTOR3(0, PlaneHeight, 0), &D3DXVECTOR3(0, 1, 0) );

	D3DXMATRIX matReflect;
	D3DXMatrixReflect( &matReflect, &plane );

	CMatrix mView = rs->GetView();

	CMatrix mViewNew = mView;

	CMatrix Invertor;
	memcpy (Invertor.m, matReflect.m, sizeof(float)* 16);
	mViewNew = Invertor * mViewNew;

	rs->SetView(mViewNew);

	CMatrix _mWorld = CMatrix();
	CMatrix _mView = rs->GetView();
	CMatrix _mProj = rs->GetProjection();
	CMatrix _mWorldView = _mWorld * _mView;
	CMatrix _mWorldViewProj = _mWorldView * _mProj;

	D3DXMATRIX mInv;
	memcpy (mInv.m, _mWorldViewProj.m, sizeof(float)* 16);
	D3DXMatrixInverse(&mInv, nullptr, &mInv);
	D3DXMatrixTranspose(&mInv, &mInv);
	D3DXPlaneTransform(&plane, &plane, &mInv);
	rs->SetClipPlane (0, (FLOAT *)&plane);
	//rs->SetEffect("FlatSeaReverseCull");
	//Event("SeaReflection");


	bool bLayerFrozen = (api->LayerGetFlags("sea_reflection2") & LRFLAG_FROZEN) != 0;

	uint32_t Colors[6] = {0xd934c8, 0x2FFF1F, 0x0000FF, 0xFF00, 0xb28e11, 0x0};
	//for (uint32_t i=0; i<6; i++) 
	{
		//if (i == D3DCUBEMAP_FACE_NEGATIVE_Y) continue;
		//if (i != D3DCUBEMAP_FACE_NEGATIVE_Z) continue;

		IDirect3DSurface9 * pReflectionSurface;
		pReflection->GetSurfaceLevel(0, &pReflectionSurface);

		rs->BeginScene();
		//bool bCool = rs->SetRenderTarget(pEnvMap, i, 0, pZStencil);
		//bool bCool = rs->SetRenderTarget(pEnvMap, i, 0, pZStencil);
		bool bCool = rs->SetRenderTarget(pReflectionSurface, pReflectionSurfaceDepth);
		bCool = D3D_OK != rs->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x0, 1.0f, 0);

		//CMatrix mView;
		//EnvMap_GetSideMatrix((D3DCUBEMAP_FACES)i, mView);

		//mView.m[3][0] = -(mView.m[0][0] * vCamPos.x - mView.m[1][0] * vCamPos.y + mView.m[2][0] * vCamPos.z);
		//mView.m[3][1] = -(mView.m[0][1] * vCamPos.x - mView.m[1][1] * vCamPos.y + mView.m[2][1] * vCamPos.z);
		//mView.m[3][2] = -(mView.m[0][2] * vCamPos.x - mView.m[1][2] * vCamPos.y + mView.m[2][2] * vCamPos.z);
		
		//api->Trace("sea: %.3f, %.3f, %.3f", mView.m[3][0], mView.m[3][1], mView.m[3][2]);
		//rs->SetView(mView);

		entid_t ent_id;

		// Render scene here.
		if (api->SetEntityScanLayer("sea_reflection") && api->GetEntity(&ent_id)) do 
		{
			api->Send_Message(ent_id, "ll", MSG_SEA_REFLECTION_DRAW, long(bSimpleSea));
		} while (api->GetEntityNext(&ent_id));

		if (!bLayerFrozen && api->SetEntityScanLayer("sea_reflection2") && api->GetEntity(&ent_id)) do 
		{
			api->Send_Message(ent_id, "ll", MSG_SEA_REFLECTION_DRAW, long(bSimpleSea));
		} while (api->GetEntityNext(&ent_id));

		rs->EndScene();
		pReflectionSurface->Release();
	}

	rs->PopRenderTarget();
	rs->SetView(mOldView);
	rs->SetProjection(mOldProjection);
	
	rs->BeginScene();

	CMatrix mTex;
	mTex.BuildScale(CVECTOR (0.5f, -0.5f, 0.5f));
	mTex.pos = CVECTOR (0.5f, 0.5f, 0.5f);

	CMatrix mProj = rs->GetProjection();
	mProj = mProj * mTex;

	CMatrix mCurWorld = rs->GetWorld();
	CMatrix mCurView = rs->GetView();

	mTexProjection = (mCurWorld * mCurView) * mProj;

	return true;
}

bool SEA::SunRoad_Render()
{
	CMatrix mOldView = rs->GetView();
	CMatrix mOldProjection = rs->GetProjection();

	rs->EndScene();
	rs->PushRenderTarget();

	rs->SetProjection( CMatrix().BuildProjectionMatrix(PI / 2.0f, 256.0f, 256.0f, 1.0f, 4000.0f) );

	uint32_t dwSkyCode = api->Class_Name2Code("sky");
	uint32_t dwIslandCode = api->Class_Name2Code("island");
	uint32_t dwShipCode = api->Class_Name2Code("ship");
	uint32_t dwSailCode = api->Class_Name2Code("sail");

	bool bLayerFrozen = (api->LayerGetFlags("sea_reflection2") & LRFLAG_FROZEN) != 0;

	uint32_t Colors[6] = {0xd934c8, 0x2FFF1F, 0x0000FF, 0xFF00, 0xb28e11, 0x0};
	for (uint32_t i=0; i<6; i++) 
	{
		if (!bUnderSea && i == D3DCUBEMAP_FACE_NEGATIVE_Y) continue;

		rs->BeginScene();
		bool bCool = rs->SetRenderTarget(pSunRoadMap, i, 0, pZStencil);
		bCool = D3D_OK != rs->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, 0x0, 1.0f, 0);

		CMatrix mView;
		EnvMap_GetSideMatrix((D3DCUBEMAP_FACES)i, mView);

		mView.m[3][0] = -(mView.m[0][0] * vCamPos.x - mView.m[1][0] * vCamPos.y + mView.m[2][0] * vCamPos.z);
		mView.m[3][1] = -(mView.m[0][1] * vCamPos.x - mView.m[1][1] * vCamPos.y + mView.m[2][1] * vCamPos.z);
		mView.m[3][2] = -(mView.m[0][2] * vCamPos.x - mView.m[1][2] * vCamPos.y + mView.m[2][2] * vCamPos.z);

		rs->SetView(mView);

		entid_t ent_id;
		// Render scene here.
		if (api->SetEntityScanLayer("sea_sunroad") && api->GetEntity(&ent_id))
			do 
			{
				uint32_t dwCCode = ent_id.class_code;
				if (!bLayerFrozen || (dwCCode != dwShipCode && dwCCode != dwSailCode && dwCCode != dwIslandCode)) 
				{
					api->Send_Message(ent_id, "ll", MSG_SEA_SUNROAD_DRAW, long(bSimpleSea));
				}
			} while (api->GetEntityNext(&ent_id));

		rs->EndScene();
	}

	rs->PopRenderTarget();
	rs->SetView(mOldView);
	rs->SetProjection(mOldProjection);

	rs->BeginScene();
	mOldView = rs->GetView();

	return true;
}

bool SEA::EnvMap_Render()
{
	CMatrix mOldView = rs->GetView();
	CMatrix mOldProjection = rs->GetProjection();
	rs->EndScene();
	rs->PushRenderTarget();

	rs->SetProjection( CMatrix().BuildProjectionMatrix(PI / 2.0f, 256.0f, 256.0f, 1.0f, 4000.0f) );

	bool bLayerFrozen = (api->LayerGetFlags("sea_reflection2") & LRFLAG_FROZEN) != 0;

	uint32_t Colors[6] = {0xd934c8, 0x2FFF1F, 0x0000FF, 0xFF00, 0xb28e11, 0x0};
	for (uint32_t i=0; i<6; i++) 
	{
		if (!bUnderSea && i == D3DCUBEMAP_FACE_NEGATIVE_Y) continue;
		//if (i != D3DCUBEMAP_FACE_NEGATIVE_Z) continue;

		rs->BeginScene();
		bool bCool = rs->SetRenderTarget(pEnvMap, i, 0, pZStencil);
		bCool = D3D_OK != rs->Clear(0, nullptr, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET, Colors[i]/*0x0*/, 1.0f, 0);

		CMatrix mView;
		EnvMap_GetSideMatrix((D3DCUBEMAP_FACES)i, mView);

		mView.m[3][0] = -(mView.m[0][0] * vCamPos.x - mView.m[1][0] * vCamPos.y + mView.m[2][0] * vCamPos.z);
		mView.m[3][1] = -(mView.m[0][1] * vCamPos.x - mView.m[1][1] * vCamPos.y + mView.m[2][1] * vCamPos.z);
		mView.m[3][2] = -(mView.m[0][2] * vCamPos.x - mView.m[1][2] * vCamPos.y + mView.m[2][2] * vCamPos.z);
		
		//api->Trace("sea: %.3f, %.3f, %.3f", mView.m[3][0], mView.m[3][1], mView.m[3][2]);
		rs->SetView(mView);

		entid_t ent_id;

		// Render scene here.
		if (api->SetEntityScanLayer("sea_reflection") && api->GetEntity(&ent_id)) do 
		{
			api->Send_Message(ent_id, "ll", MSG_SEA_REFLECTION_DRAW, long(bSimpleSea));
		} while (api->GetEntityNext(&ent_id));

		if (!bLayerFrozen && api->SetEntityScanLayer("sea_reflection2") && api->GetEntity(&ent_id)) do 
		{
			api->Send_Message(ent_id, "ll", MSG_SEA_REFLECTION_DRAW, long(bSimpleSea));
		} while (api->GetEntityNext(&ent_id));

		rs->EndScene();
	}

	rs->PopRenderTarget();
	rs->SetView(mOldView);
	rs->SetProjection(mOldProjection);
	
	rs->BeginScene();

	return true;
}