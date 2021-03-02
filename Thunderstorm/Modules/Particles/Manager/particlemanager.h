//****************************************************************
//*
//*  Author : Sergey Makeev aka Joker, 2003
//*
//*
//****************************************************************

#ifndef PARTICLES_MANAGER_IMPLEMENTATION
#define PARTICLES_MANAGER_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include "../particles/iparticlemanager.h"

#include "../System/ParticleProcessor/mdl_processor.h"
#include "../System/ParticleProcessor/bb_processor.h"
#include <string>
#include <vector>


class ParticleService;
class DataCache;
class GeomCache;
class ParticleSystem;
class DataSource;


//�������� ����������� ������
class ParticleManager : public IParticleManager
{
	struct CacheReloadedInfo
	{
		std::string Name;
		bool AutoDeleted;
		Matrix matWorld;
	};

	bool ShowStat;
	bool GlobalDelete;

	struct SystemDesc
	{
		ParticleSystem* pSystem;
	};

	std::vector<SystemDesc> Systems;

	//��������� ��� ��������� �������
	ModelProcessor* MDL_Processor;

	//��������� ��� ��������� ������
	BillBoardProcessor* BB_Processor;

	long pProjectTexture;
	long pProjectTextureNormalMap;
	DataCache* pDataCache;
	GeomCache* pGeomCache;

	std::string ShortProjectName;
	std::string TextureName;

	//������ ������� ����� �������
	ParticleService* pService;

	VDX9RENDER* pRS;

	void DeleteAllSystems();

	float TimeFromLastStatUpdate;
	uint64_t nowTickTime;
	uint64_t nowUpdateTime;
	uint32_t ActiveSystems;
	uint32_t ActiveEmitters;
	uint32_t ActiveBillboardParticles;
	uint32_t ActiveModelParticles;


	std::vector<ParticleSystem*> DeleteQuery;


protected:

	virtual ~ParticleManager();


	ParticleSystem* CreateParticleSystemFromDataSource(DataSource* pDataSource);

	uint32_t IteratorIndex;
	std::vector<std::string> EnumUsedGeom;

	bool FindInEnumUsedGeom(const char* GeomName);

public:

	//��������/�������� 
	ParticleManager(ParticleService* service);
	bool Release() override;

	//�������� ��������� �� Render/FileService 
	VDX9RENDER* Render() override;

	//������� ������ 
	bool OpenProject(const char* FileName) override;
	//������� ������ 
	void CloseProject() override;

	//������� �� ������ �������� (���������)
	void RemoveResource(IParticleSystem* pResource) override;

	//��������� �������� 
	void Execute(float DeltaTime) override;

	//������ �������� ������� ��� ��� 
	bool IsSystemAvailable(const char* FileName) override;

	//�������� ���������� �������� ������� 
	long GetProjectTexture() override;
	//���������� �������� �������
	void SetProjectTexture(const char* FileName = nullptr) override;


	//�������� ��� ������� 
	const char* GetProjectFileName() override;

	//������� ����������� ������� �� ����� (���� ������ ���� � �������!!!!!)
	IParticleSystem* CreateParticleSystemEx(const char* FileName, const char* File, int Line) override;

	//������� ������ ����������� �������, ��� ���������...
	IParticleSystem* CreateEmptyParticleSystemEx(const char* FileName, int Line) override;

	BillBoardProcessor* GetBBProcessor();
	ModelProcessor* GetMDLProcessor();

	bool ValidateSystem(IParticleSystem* pSystem) override;

	GEOS* GetModel(const char* FileName);

	uint32_t GetCreatedSystemCount();
	ParticleSystem* GetCreatedSystemByIndex(uint32_t Index);

	bool ReadyForUse() override;

	void DefferedDelete(ParticleSystem* pSys);

	const char* GetProjectTextureName() override;

	void Editor_UpdateCachedData() override;

	virtual uint32_t GetProjectSystemCount();
	virtual const char* GetProjectSystemName(uint32_t Index);

	const char* GetFirstGeomName(const char* FileName) override;
	const char* GetNextGeomName() override;

	virtual void CreateGeomCache();


	void WriteSystemCache(const char* FileName) override;
	virtual void WriteSystemCache(const char* FileName, MemFile* pMemFile);
	virtual void WriteSystemCacheAs(const char* FileName, const char* NewName);

	virtual void LoadSystemCache(const char* FileName, MemFile* pMemFile);
	virtual void CacheReloaded();

	virtual FieldList* Editor_CreatePointEmitter(const char* SystemName, const char* EmitterName);
	virtual FieldList* Editor_CreateBillBoardParticle(const char* SystemName, const char* EmitterName,
	                                                  const char* ParticleName);
	virtual FieldList* Editor_CreateModelParticle(const char* SystemName, const char* EmitterName,
	                                              const char* ParticleName);

	virtual void DeletePointEmitter(const char* SystemName, IEmitter* pEmitter);
	virtual void DeleteBillboard(const char* SystemName, IEmitter* pEmitter, FieldList* pParticles);
	virtual void DeleteModel(const char* SystemName, IEmitter* pEmitter, FieldList* pParticles);


	void OpenDefaultProject() override;
};


#endif
