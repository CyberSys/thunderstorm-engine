#include "DataSource.h"

#include "DataColor.h"
#include "DataFloat.h"
#include "DataGraph.h"
#include "DataString.h"


#include "vmodule_api.h"
#include "../../ICommon/Names.h"

#define HEADER "PSYS"
#define VERSION "v3.5"


BEGIN_DATA_DESC(PointEmitterDesc)
		DATA_STRING(EMITTER_NAME);
		DATA_FLOAT(EMITTER_LIFETIME);
		DATA_POSITION(EMITTER_POSITION);
		DATA_GRAPH(EMISSION_DIR_X);
		DATA_GRAPH(EMISSION_DIR_Y);
		DATA_GRAPH(EMISSION_DIR_Z);
		DATA_BOOL(EMITTER_LOOPING);
END_DATA_DESC(PointEmitterDesc)

BEGIN_DATA_DESC(BillboardParticleDesc)
		DATA_STRING(PARTICLE_NAME);
		DATA_GRAPH(PARTICLE_EMISSION_RATE);
		DATA_FLOAT(PARTICLE_MAX_COUNT);
		DATA_GRAPH(PARTICLE_TRACK_X);
		DATA_GRAPH(PARTICLE_TRACK_Y);
		DATA_GRAPH(PARTICLE_TRACK_Z);
		DATA_GRAPH(PARTICLE_LIFE_TIME);
		DATA_GRAPH(PARTICLE_MASS);
		DATA_GRAPH(PARTICLE_SPIN);
		DATA_GRAPH(PARTICLE_VELOCITY_POWER);
		DATA_GRAPH(PARTICLE_SPIN_DRAG);
		DATA_GRAPH(PARTICLE_SIZE);
		DATA_GRAPH(PARTICLE_ANIMFRAME);
		DATA_COLOR(PARTICLE_COLOR);
		DATA_UV(PARTICLE_FRAMES);
		DATA_GRAPH(PARTICLE_TRANSPARENCY);
		DATA_GRAPH(PARTICLE_DRAG);
		DATA_GRAPH(PARTICLE_PHYSIC_BLEND);
		DATA_STRING(ATTACHEDEMITTER_NAME);
		DATA_GRAPH(PARTICLE_GRAVITATION_K);
		DATA_BOOL(PARTICLE_DIR_ORIENT);
		DATA_GRAPH(PARTICLE_ADDPOWER);
END_DATA_DESC(BillboardParticleDesc)

BEGIN_DATA_DESC(ModelParticleDesc)
		DATA_STRING(PARTICLE_NAME);
		DATA_GRAPH(PARTICLE_EMISSION_RATE);
		DATA_FLOAT(PARTICLE_MAX_COUNT);
		DATA_GRAPH(PARTICLE_TRACK_X);
		DATA_GRAPH(PARTICLE_TRACK_Y);
		DATA_GRAPH(PARTICLE_TRACK_Z);
		DATA_GRAPH(PARTICLE_LIFE_TIME);
		DATA_GRAPH(PARTICLE_MASS);
		DATA_GRAPH(PARTICLE_SPIN_X);
		DATA_GRAPH(PARTICLE_SPIN_Y);
		DATA_GRAPH(PARTICLE_SPIN_Z);
		DATA_GRAPH(PARTICLE_VELOCITY_POWER);
		DATA_GRAPH(PARTICLE_SPIN_DRAGX);
		DATA_GRAPH(PARTICLE_SPIN_DRAGY);
		DATA_GRAPH(PARTICLE_SPIN_DRAGZ);
		DATA_GRAPH(PARTICLE_DRAG);
		DATA_GRAPH(PARTICLE_PHYSIC_BLEND);
		DATA_STRING(ATTACHEDEMITTER_NAME);
		DATA_GRAPH(PARTICLE_GRAVITATION_K);
		DATA_STRING(PARTICLE_GEOM_NAMES);
END_DATA_DESC(ModelParticleDesc)


//---------- ��������/�������� --------------------
DataSource::DataSource(IParticleManager* Master)
{
}

DataSource::~DataSource()
{
	Destroy();
}

bool DataSource::Release()
{
	delete this;
	return true;
}


// ========================= Load & Save =======================================
//���������/������������ �� �����
void DataSource::Write(MemFile* pMemFile)
{
	pMemFile->Write(HEADER, 4);
	pMemFile->Write(VERSION, 4);

	uint32_t dwEmittersCount = Emitters.size();
	pMemFile->WriteType(dwEmittersCount);

	for (uint32_t n = 0; n < dwEmittersCount; n++)
	{
		pMemFile->WriteType(Emitters[n].Type);
		Emitters[n].Fields.Write(pMemFile);
		uint32_t dwParticlesSize = Emitters[n].Particles.size();
		pMemFile->WriteType(dwParticlesSize);
		for (uint32_t i = 0; i < dwParticlesSize; i++)
		{
			pMemFile->WriteType(Emitters[n].Particles[i].Type);
			Emitters[n].Particles[i].Fields.Write(pMemFile);
		}
	}
}

void DataSource::Load(MemFile* pMemFile)
{
	//��������� ID
	char Id[5];
	Id[4] = 0;
	pMemFile->Read(Id, 4);
	if (strcmp(Id, HEADER) != 0)
	{
		api->Trace("Particles: Incorrect file type");
		return;
	}

	//��������� ������
	char Ver[5];
	Ver[4] = 0;
	pMemFile->Read(Ver, 4);

	/* // show warnings
		if (strcmp (Ver, VERSION) != 0)	
	          api->Trace ("Particles: Warning !!! Incorrect file version %s, must be %s", Ver, VERSION);
	*/

	//���-�� ���������...
	uint32_t EmiterCount = 0;
	pMemFile->ReadType(EmiterCount);

	for (uint32_t n = 0; n < EmiterCount; n++)
	{
		EmitterType emType = UNKNOWN_EMITTER;
		pMemFile->ReadType(emType);

		switch (emType)
		{
		case POINT_EMITTER:
			{
				//				api->Trace ("Particles info: Point emitter");
				CreatePointEmitter(pMemFile);
				break;
			}

		default:
			{
				throw std::exception("Particles: Unknown emitter type !");
			}
		} // switch
	} // for all saved emitters...
}


void DataSource::CreatePointEmitter(MemFile* pMemFile)
{
	//api->Trace ("Particles info: Point emitter");
	Emitters.push_back(EmitterDesc{});
	EmitterDesc* PointEmitter = &Emitters.back();
	//EmitterDesc* PointEmitter = &Emitters[Emitters.Add()];
	PointEmitter->Fields.Load(pMemFile);

	PointEmitter->Fields.Convert(&PointEmitterDesc);
	PointEmitter->Type = POINT_EMITTER;

	uint32_t ParticlesCount = 0;
	pMemFile->ReadType(ParticlesCount);

	for (uint32_t n = 0; n < ParticlesCount; n++)
	{
		ParticleType ptType = UNKNOWN_PARTICLE;
		pMemFile->ReadType(ptType);

		switch (ptType)
		{
		case BILLBOARD_PARTICLE:
			{
				//api->Trace ("Particles info: Billboard particle");
				CreateBillBoardParticle(PointEmitter->Particles, pMemFile);
				break;
			}
		case MODEL_PARTICLE:
			{
				//api->Trace ("Particles info: Model particle");
				CreateModelParticle(PointEmitter->Particles, pMemFile);
				break;
			}
		default:
			{
				throw std::exception("Particles: Unknown particle type !!!!");
			}
		} // SWITCH
	} // For all particles
}

//������� BillBoard ������
void DataSource::CreateBillBoardParticle(std::vector<ParticleDesc>& Particles, MemFile* pMemFile)
{
	//ParticleDesc *pDesc = &Particles[Particles.Add()];
	ParticleDesc desc;
	desc.Type = BILLBOARD_PARTICLE;
	desc.Fields.Load(pMemFile);
	desc.Fields.Convert(&BillboardParticleDesc);
	Particles.push_back(desc);
}

//������� Model ������
void DataSource::CreateModelParticle(std::vector<ParticleDesc>& Particles, MemFile* pMemFile)
{
	//ParticleDesc *pDesc = &Particles[Particles.Add()];
	ParticleDesc desc;
	desc.Type = MODEL_PARTICLE;
	desc.Fields.Load(pMemFile);
	desc.Fields.Convert(&ModelParticleDesc);
	Particles.push_back(desc);
}


void DataSource::Destroy()
{
	for (uint32_t n = 0; n < Emitters.size(); n++)
	{
		Emitters[n].Fields.DelAll();
		for (uint32_t i = 0; i < Emitters[n].Particles.size(); i++)
		{
			Emitters[n].Particles[i].Fields.DelAll();
		}
	}

	Emitters.clear();
}


int DataSource::GetEmitterCount()
{
	return Emitters.size();
}

DataSource::EmitterDesc* DataSource::GetEmitterDesc(int Index)
{
	return &Emitters[Index];
}

FieldList* DataSource::CreateEmptyPointEmitter(const char* EmitterName)
{
	Emitters.push_back(EmitterDesc{});
	//EmitterDesc* PointEmitter = &Emitters[Emitters.Add()];
	EmitterDesc* PointEmitter = &Emitters.back();
	PointEmitter->Fields.Convert(&PointEmitterDesc);
	PointEmitter->Type = POINT_EMITTER;

	DataString* pEmitterName = PointEmitter->Fields.FindString(EMITTER_NAME);

	if (pEmitterName)
	{
		pEmitterName->SetValue(EmitterName);
	}

	DataFloat* pEmitterLifeTime = PointEmitter->Fields.FindFloat(EMITTER_LIFETIME);

	if (pEmitterLifeTime)
	{
		pEmitterLifeTime->SetValue(1.0f);
	}

	DataGraph* pAngleX = PointEmitter->Fields.FindGraph(EMISSION_DIR_X);
	if (pAngleX) pAngleX->SetNegative(true);

	DataGraph* pAngleY = PointEmitter->Fields.FindGraph(EMISSION_DIR_Y);
	if (pAngleY) pAngleY->SetNegative(true);

	DataGraph* pAngleZ = PointEmitter->Fields.FindGraph(EMISSION_DIR_Z);
	if (pAngleZ) pAngleZ->SetNegative(true);

	return &PointEmitter->Fields;
}

int DataSource::FindEmitter(const char* Name)
{
	for (uint32_t n = 0; n < Emitters.size(); n++)
	{
		DataString* pString = Emitters[n].Fields.FindString(EMITTER_NAME);
		if (pString)
		{
			if (_stricmp(pString->GetValue(), Name) == 0)
			{
				return n;
			}
		}
	}

	return -1;
}

FieldList* DataSource::CreateBillBoardParticle(const char* ParticleName, const char* EmitterName)
{
	int EmitterIndex = FindEmitter(EmitterName);
	if (EmitterIndex == -1) return nullptr;

	Emitters[EmitterIndex].Particles.push_back(ParticleDesc{});
	//ParticleDesc *pDesc = &Emitters[EmitterIndex].Particles[Emitters[EmitterIndex].Particles.Add()];
	ParticleDesc* pDesc = &Emitters[EmitterIndex].Particles.back();
	pDesc->Type = BILLBOARD_PARTICLE;
	pDesc->Fields.Convert(&BillboardParticleDesc);

	DataString* pParticleName = pDesc->Fields.FindString(PARTICLE_NAME);
	if (pParticleName) pParticleName->SetValue(ParticleName);

	DataString* pAttachedEmitter = pDesc->Fields.FindString(ATTACHEDEMITTER_NAME);
	if (pAttachedEmitter) pAttachedEmitter->SetValue("none");

	DataGraph* pSize = pDesc->Fields.FindGraph(PARTICLE_SIZE);
	if (pSize) pSize->SetDefaultValue(3.0f, 2.0f);

	DataGraph* pEmissionRate = pDesc->Fields.FindGraph(PARTICLE_EMISSION_RATE);
	if (pEmissionRate) pEmissionRate->SetDefaultValue(10.0f, 10.0f);

	DataGraph* pLifeTime = pDesc->Fields.FindGraph(PARTICLE_LIFE_TIME);
	if (pLifeTime) pLifeTime->SetDefaultValue(10.0f, 10.0f);


	DataFloat* pMaxCount = pDesc->Fields.FindFloat(PARTICLE_MAX_COUNT);
	if (pMaxCount) pMaxCount->SetValue(100);

	DataColor* pColorG = pDesc->Fields.FindColor(PARTICLE_COLOR);
	if (pColorG) pColorG->SetDefaultValue(Color(1.0f, 1.0f, 1.0f, 1.0f));

	DataGraph* pGravityK = pDesc->Fields.FindGraph(PARTICLE_GRAVITATION_K);
	if (pGravityK) pGravityK->SetDefaultValue(100.0f, 100.0f);


	DataGraph* pTrackX = pDesc->Fields.FindGraph(PARTICLE_TRACK_X);
	if (pTrackX) pTrackX->SetNegative(true);

	DataGraph* pTrackY = pDesc->Fields.FindGraph(PARTICLE_TRACK_Y);
	if (pTrackY) pTrackY->SetNegative(true);

	DataGraph* pTrackZ = pDesc->Fields.FindGraph(PARTICLE_TRACK_Z);
	if (pTrackZ) pTrackZ->SetNegative(true);

	DataGraph* pVelocity = pDesc->Fields.FindGraph(PARTICLE_VELOCITY_POWER);
	if (pVelocity) pVelocity->SetNegative(true);

	DataGraph* pMass = pDesc->Fields.FindGraph(PARTICLE_MASS);
	if (pMass) pMass->SetNegative(true);

	DataGraph* pSpin = pDesc->Fields.FindGraph(PARTICLE_SPIN);
	if (pSpin) pSpin->SetNegative(true);

	return &pDesc->Fields;
}

FieldList* DataSource::CreateModelParticle(const char* ParticleName, const char* EmitterName)
{
	int EmitterIndex = FindEmitter(EmitterName);
	if (EmitterIndex == -1) return nullptr;

	Emitters[EmitterIndex].Particles.push_back(ParticleDesc{});
	//ParticleDesc *pDesc = &Emitters[EmitterIndex].Particles[Emitters[EmitterIndex].Particles.Add()];
	ParticleDesc* pDesc = &Emitters[EmitterIndex].Particles.back();
	pDesc->Type = MODEL_PARTICLE;
	pDesc->Fields.Convert(&ModelParticleDesc);

	DataString* pParticleName = pDesc->Fields.FindString(PARTICLE_NAME);
	if (pParticleName) pParticleName->SetValue(ParticleName);

	DataString* pAttachedEmitter = pDesc->Fields.FindString(ATTACHEDEMITTER_NAME);
	if (pAttachedEmitter) pAttachedEmitter->SetValue("none");

	DataFloat* pMaxCount = pDesc->Fields.FindFloat(PARTICLE_MAX_COUNT);
	if (pMaxCount) pMaxCount->SetValue(100);

	DataGraph* pEmissionRate = pDesc->Fields.FindGraph(PARTICLE_EMISSION_RATE);
	if (pEmissionRate) pEmissionRate->SetDefaultValue(10.0f, 10.0f);

	DataGraph* pLifeTime = pDesc->Fields.FindGraph(PARTICLE_LIFE_TIME);
	if (pLifeTime) pLifeTime->SetDefaultValue(10.0f, 10.0f);

	DataGraph* pGravityK = pDesc->Fields.FindGraph(PARTICLE_GRAVITATION_K);
	if (pGravityK) pGravityK->SetDefaultValue(100.0f, 100.0f);

	DataGraph* pTrackX = pDesc->Fields.FindGraph(PARTICLE_TRACK_X);
	if (pTrackX) pTrackX->SetNegative(true);

	DataGraph* pTrackY = pDesc->Fields.FindGraph(PARTICLE_TRACK_Y);
	if (pTrackY) pTrackY->SetNegative(true);

	DataGraph* pTrackZ = pDesc->Fields.FindGraph(PARTICLE_TRACK_Z);
	if (pTrackZ) pTrackZ->SetNegative(true);

	DataGraph* pVelocity = pDesc->Fields.FindGraph(PARTICLE_VELOCITY_POWER);
	if (pVelocity) pVelocity->SetNegative(true);

	DataGraph* pMass = pDesc->Fields.FindGraph(PARTICLE_MASS);
	if (pMass) pMass->SetNegative(true);


	DataGraph* pSpinX = pDesc->Fields.FindGraph(PARTICLE_SPIN_X);
	if (pSpinX) pSpinX->SetNegative(true);

	DataGraph* pSpinY = pDesc->Fields.FindGraph(PARTICLE_SPIN_Y);
	if (pSpinY) pSpinY->SetNegative(true);

	DataGraph* pSpinZ = pDesc->Fields.FindGraph(PARTICLE_SPIN_Z);
	if (pSpinZ) pSpinZ->SetNegative(true);

	return &pDesc->Fields;
}


void DataSource::DeletePointEmitter(FieldList* pEmitter)
{
	for (uint32_t n = 0; n < Emitters.size(); n++)
	{
		if (&Emitters[n].Fields == pEmitter)
		{
			Emitters[n].Fields.DelAll();
			//Emitters.ExtractNoShift(n);
			Emitters[n] = Emitters.back();
			Emitters.pop_back();
			break;
		}
	}
}

void DataSource::DeleteBillboard(FieldList* pEmitter, FieldList* pParticles)
{
	for (uint32_t n = 0; n < Emitters.size(); n++)
	{
		if (&Emitters[n].Fields == pEmitter)
		{
			for (uint32_t i = 0; i < Emitters[n].Particles.size(); i++)
			{
				if (&Emitters[n].Particles[i].Fields == pParticles)
				{
					Emitters[n].Particles[i].Fields.DelAll();
					//Emitters[n].Particles.Extract(i);
					Emitters[n].Particles[i] = Emitters[n].Particles.back();
					Emitters[n].Particles.pop_back();
				}
			}
		}
	}
}

void DataSource::DeleteModel(FieldList* pEmitter, FieldList* pParticles)
{
	for (uint32_t n = 0; n < Emitters.size(); n++)
	{
		if (&Emitters[n].Fields == pEmitter)
		{
			for (uint32_t i = 0; i < Emitters[n].Particles.size(); i++)
			{
				if (&Emitters[n].Particles[i].Fields == pParticles)
				{
					Emitters[n].Particles[i].Fields.DelAll();
					//Emitters[n].Particles.Extract(i);
					Emitters[n].Particles[i] = Emitters[n].Particles.back();
					Emitters[n].Particles.pop_back();
				}
			}
		}
	}
}
