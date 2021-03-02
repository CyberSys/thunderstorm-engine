#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#include "vmodule_api.h"
#include "particles/iparticlesystem.h"
#include "../../ICommon/Types.h"
#include "../../Manager/particlemanager.h"
#include "../DataSource/DataSource.h"


class IEmitter;

class ParticleSystem : public IParticleSystem
{
	struct EmitterDesc
	{
		EmitterType Type;
		IEmitter* pEmitter;
	};

	std::vector<EmitterDesc> Emitters;

	//������������� ������� ��� �������
	bool AutoDeleted;

	// �� ������� ����� �������� !!!
	bool EmissionPause;

	bool DeleteIfNeed();

	std::string SystemName;


protected:

	virtual ~ParticleSystem();


	IEmitter* CreatePointEmitter(DataSource::EmitterDesc* pEmitter);

	void DeleteAllEmitters();

	ParticleManager* pMaster;

	Matrix matWorld;

public:

	//��������/�������� 
	ParticleSystem(ParticleManager* serv);
	bool Release() override;

	//���������� ���� ���������
	uint32_t Execute(float DeltaTime) override;


	//������������� ����������� �������  
	void Restart(uint32_t RandomSeed) override;

	//��������� ���������� ���������  
	void PauseEmission(bool bPause) override;

	void Stop() override;
	//������ �� ����� ������� ��� ���  
	bool IsEmissionPaused() override;


	//���������� ������������� ������� ��� �������...  
	void AutoDelete(bool Enabled) override;
	//������� ������������� ������� ��� ���  
	bool IsAutoDeleted() override;
	//���������� ������� ������������� ��� ������� 
	void SetTransform(const Matrix& transform) override;
	void GetTransform(Matrix& _matWorld) override;


	void CreateFromDataSource(DataSource* pDataSource);


	ParticleManager* GetMaster();

	IEmitter* FindEmitter(const char* name);
	IEmitter* FindEmitterByData(FieldList* Data);


	void Teleport(const Matrix& transform) override;

	bool IsAlive() override;

	const char* GetName() override;
	void SetName(const char* Name);


public:
	uint32_t GetEmittersCount();
	IEmitter* GetEmitterByIndex(uint32_t Index);
	EmitterType GetEmitterTypeByIndex(uint32_t Index);

	void Editor_UpdateCachedData();
};

#endif
