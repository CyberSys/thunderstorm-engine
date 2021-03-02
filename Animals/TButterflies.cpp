#include "TButterflies.h"
#include "../Common_h/message.h"
#include "../Common_h/ship_base.h"
#include "../Shared/messages.h"

#pragma warning (disable : 4244)

//--------------------------------------------------------------------
TButterflies::TButterflies()
	:enabled(false)
	,yDefineTime(0)
{
}

//--------------------------------------------------------------------
TButterflies::~TButterflies()
{
	renderService->TextureRelease(texture);

	delete ivManager;
}

//--------------------------------------------------------------------
void TButterflies::LoadSettings()
{
	INIFILE * ini = fio->OpenIniFile(ANIMALS_INI_FILENAME);
	if (!ini)
		return;

	butterfliesCount = ini->GetLong(ANIMALS_BUTTERFLIES_SECTION, "count", BUTTERFLY_COUNT);
	maxDistance = ini->GetFloat(ANIMALS_BUTTERFLIES_SECTION, "distance", BUTTERFLY_DISTANCE);

	delete ini;

	// DEBUG!
	//butterfliesCount = 1;
}

//--------------------------------------------------------------------
void TButterflies::Init()
{
	LoadSettings();

	renderService = (VDX9RENDER *)api->CreateService("dx9render");
	if(!renderService)	
		throw std::exception("!Butterflies: No service 'dx9render'");

	collide = (COLLIDE *)api->CreateService("coll");
	if (!collide)	
		throw std::exception("!Butterflies: No service COLLIDE");

	walker = api->LayerGetWalker("shadow");

	ivManager = new TIVBufferManager(renderService, 
									 BUTTERFLY_VERTEX_TYPE, 
									 sizeof(tButterflyVertex), 
									 3*4, 6, butterfliesCount);

	for (int i=0; i<butterfliesCount; i++)
		butterflies[i].Initialize(CVECTOR(0.0f, 0.0f, 0.0f), maxDistance, ivManager->ReserveElement(), rand() % 4, rand() % 4);

	texture = renderService->TextureCreate("butter.tga");
}

//--------------------------------------------------------------------
uint32_t TButterflies::ProcessMessage(long _code, MESSAGE & message)
{
	uint32_t outValue = 0;
	switch (_code)
	{
	case MSG_ANIMALS_BUTTERFLIES_SHOW:
		enabled = true;
		break;

	case MSG_ANIMALS_BUTTERFLIES_HIDE:
		enabled = false;
		break;

	case MSG_ANIMALS_BUTTERFLIES_XYZ:
		{
			static CVECTOR affectVector(0.f, 0.f, 0.f);
			affectVector.x = message.Float();
			affectVector.z = message.Float();
			for (int i = 0; i<butterfliesCount; i++)
			{
				butterflies[i].Effect(affectVector);
			}
		}
		break;

	}
	return outValue;
}

//--------------------------------------------------------------------
void TButterflies::Execute(uint32_t _dTime)
{
	if (!enabled)
		return;

	// re-set center
	CVECTOR pos, ang;
	float   persp;
	renderService->GetCamera(pos, ang, persp);
	butterflies[0].SetCenter(pos);
	int i;

	// redefine minY
	yDefineTime += _dTime;
	if (yDefineTime > Y_REDEFINE_TIME)
	{
		if (!walker)
			walker = api->LayerGetWalker("shadow");

		for (i = 0; i<butterfliesCount; i++)
		{
			static const float ALL_Y = 1000.0f;
			CVECTOR topVector = butterflies[i].GetPosition();
			CVECTOR bottomVector = butterflies[i].GetPosition();
			topVector.y = ALL_Y;
			bottomVector.y = -ALL_Y;

			float ray = collide->Trace(walker, topVector, bottomVector, nullptr, 0);
			if (ray <= 1.0f)
				butterflies[i].SetMinY(-ALL_Y+(1.f - ray)*2.f*ALL_Y);
			else
				butterflies[i].SetMinY(-ALL_Y);
		}

	}

	// recalculate & redraw
	ivManager->LockBuffers();

	for (i = 0; i<butterfliesCount; i++)
	{
		butterflies[i].Calculate(_dTime, collide, walker);
		butterflies[i].Draw(ivManager);
		//butterflies[i].Draw(renderService);
	}

	ivManager->UnlockBuffers();
}

//--------------------------------------------------------------------
void TButterflies::Realize(uint32_t dTime)
{
	if (!enabled)
		return;

	CMatrix wMatrix;

	renderService->SetTransform(D3DTS_WORLD, (D3DMATRIX *) wMatrix);
	renderService->TextureSet(0, texture);
	//for (int i = 0; i<butterfliesCount; i++)
	//	butterflies[i].Draw(renderService, butterfly);
	ivManager->DrawBuffers("Butterfly");
}

//--------------------------------------------------------------------
