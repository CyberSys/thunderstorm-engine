#include "TFishSchools.h"
#include "rands.h"
#include "../../Shared/messages.h"
#include "defines.h"
#include "EntityManager.h"

//--------------------------------------------------------------------
TFishSchools::TFishSchools()
	: enabled(false)
{
	memset(fishSchools, 0, sizeof(fishSchools));
}

//--------------------------------------------------------------------
TFishSchools::~TFishSchools()
{
	for (int i = 0; i < fishSchoolsCount; i++)
	{
		if (fishSchools[i])
			delete fishSchools[i];
	}

	EntityManager::EraseEntity(fishSchoolModel);
}

//--------------------------------------------------------------------
void TFishSchools::LoadSettings()
{
	INIFILE* ini = fio->OpenIniFile(ANIMALS_INI_FILENAME);
	if (!ini)
		return;

	fishSchoolsCount = ini->GetLong(ANIMALS_FISHSCHOOLS_SECTION, "count", FISHSCHOOL_COUNT);
	maxDistance = ini->GetFloat(ANIMALS_FISHSCHOOLS_SECTION, "distance", FISHSCHOOL_DISTANCE);

	delete ini;
}

//--------------------------------------------------------------------
void TFishSchools::Init()
{
	LoadSettings();

	renderService = (VDX9RENDER *)api->CreateService("dx9render");
	if (!renderService)
		throw std::exception("!FishSchools: No service 'dx9render'");

	sea = (SEA_BASE*)EntityManager::GetEntityPointer(EntityManager::GetEntityId("sea"));
	if (!sea)
	{
		enabled = false;
		return;
	}

	for (int i = 0; i < fishSchoolsCount; i++)
	{
		fishSchools[i] = new TFishSchool();
		fishSchools[i]->Initialize(CVECTOR(0.0f, 0.0f, 0.0f), maxDistance);
		fishSchools[i]->depth = 0.05f;
		fishSchools[i]->time = 0;
		fishSchools[i]->timeDivider = randUpper(100.0f);
		fishSchools[i]->amplitude = rand(0.01f);
		AddDeflector(fishSchools[i]);
	}

	AddAttractor(&cameraObject);

	fishSchoolModel = EntityManager::CreateEntity("MODELR");
	api->Send_Message(fishSchoolModel, "ls",MSG_MODEL_LOAD_GEO, ANIMALS_FISHSCHOOL_FILENAME);
}

//--------------------------------------------------------------------
uint64_t TFishSchools::ProcessMessage(long _code, MESSAGE& message)
{
	uint32_t outValue = 0;

	switch (_code)
	{
	case MSG_ANIMALS_FISHSCHOOLS_SHOW:
		enabled = true;
		break;

	case MSG_ANIMALS_FISHSCHOOLS_HIDE:
		enabled = false;
		break;
	}

	return outValue;
}

//--------------------------------------------------------------------
void TFishSchools::Execute(uint32_t _dTime)
{
	if (!enabled)
		return;

	CVECTOR pos, ang, fishPos;
	float persp;
	renderService->GetCamera(pos, ang, persp);

	cameraObject.SetXYZ(pos);
	float speedK = ((float)_dTime) / 1000.0f;
	for (int i = 0; i < fishSchoolsCount; i++)
	{
		//respawn near camera if needed
		fishPos = fishSchools[i]->GetXYZ();
		if ((fabs(fishPos.x - pos.x) + fabs(fishPos.x - pos.x)) > RESPAWN_DISTANCE)
			fishSchools[i]->Initialize(pos, maxDistance);

		//recalculate all fishes
		fishSchools[i]->Calculate(attractors, MAX_DYNAMIC_OBJECTS, deflectors, MAX_DYNAMIC_OBJECTS, speedK);
		fishSchools[i]->time += _dTime;
	}
}

//--------------------------------------------------------------------
void TFishSchools::Realize(uint32_t _dTime)
{
	if (!enabled)
		return;
	/*
		CVECTOR cameraPos, cameraAng;
		float   cameraPersp;
		renderService->GetCamera(cameraPos, cameraAng, cameraPersp);
	*/
	sea = (SEA_BASE*)EntityManager::GetEntityPointer(EntityManager::GetEntityId("sea"));
	if (!sea)
	{
		enabled = false;
		return;
	}

	auto* fishSchool = (MODEL*)EntityManager::GetEntityPointer(fishSchoolModel);
	if (!fishSchool)
		return;

	for (int i = 0; i < fishSchoolsCount; i++)
	{
		static const float OSC_AMPLITUDE = 0.1f;
		float fishSchoolAngle = fishSchools[i]->GetAngle();
		CVECTOR fishSchoolPos = fishSchools[i]->GetXYZ();
		float fishSchoolTime = fishSchools[i]->time / fishSchools[i]->timeDivider;
		CVECTOR ang(
			0.0f, PId2 - fishSchoolAngle - (fishSchools[i]->amplitude * (PId2 / 10.0f) / 0.1f) * cosf(fishSchoolTime),
			0.0f);
		CVECTOR pos;
		pos.x = fishSchoolPos.x + fishSchools[i]->amplitude * cosf(fishSchoolAngle + PId2) * sinf(fishSchoolTime);
		pos.z = fishSchoolPos.z + fishSchools[i]->amplitude * sinf(fishSchoolAngle + PId2) * sinf(fishSchoolTime);
		pos.y = sea->WaveXZ(pos.x, pos.z) - fishSchools[i]->depth;
		fishSchool->mtx.BuildMatrix(ang, pos);
		fishSchool->ProcessStage(Entity::Stage::realize, _dTime);
	}
}

//--------------------------------------------------------------------
