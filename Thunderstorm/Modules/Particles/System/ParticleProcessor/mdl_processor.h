//****************************************************************
//*
//*  Author : Sergey Makeev aka Joker, 2003
//*
//*
//****************************************************************

#ifndef MODEL_PARTICLE_PROCESSOR
#define MODEL_PARTICLE_PROCESSOR

#include "dx9render.h"
#include "math3d/Matrix.h"

#include "../../ICommon/Particle.h"
#include "../DataSource/FieldList.h"
#include "nameparser.h"

class ParticleSystem;
class ParticleManager;

class ModelProcessor
{
	VDX9RENDER* pRS;
	ParticleManager* pMasterManager;
	GeomNameParser Parser;

	struct MemArrayItem
	{
		MDL_ParticleData pData;
		bool Free;

		MemArrayItem()
		{
			Free = true;
		}
	};

	MemArrayItem* pMemArray;

	std::vector<MDL_ParticleData*> Particles;


	MDL_ParticleData* AllocParticle();
	void FreeParticle(MDL_ParticleData* pItem);


public:

	ModelProcessor(ParticleManager* pManager);
	~ModelProcessor();

	void AddParticle(ParticleSystem* pSystem, const Vector& velocity_dir, const Vector& pos, const Matrix& matWorld,
	                 float EmitterTime, float EmitterLifeTime, FieldList* pFields, uint32_t* pActiveCount,
	                 uint32_t dwGUID);

	void Process(float DeltaTime);
	void Draw();

	uint32_t GetCount();

	void DeleteWithGUID(uint32_t dwGUID, uint32_t GUIDRange = GUIDSTEP);

	void Clear();
};


#endif
