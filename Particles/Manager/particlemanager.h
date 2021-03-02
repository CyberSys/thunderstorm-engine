//****************************************************************
//*
//*  Author : Sergey Makeev aka Joker, 2003
//*
//*
//****************************************************************

#ifndef PARTICLES_MANAGER_IMPLEMENTATION
#define PARTICLES_MANAGER_IMPLEMENTATION

#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "..\..\common_h\exs.h"
#include "..\..\common_h\particles\iparticlemanager.h"
#include "..\..\common_h\templates.h"

#include "..\system\ParticleProcessor\mdl_processor.h"
#include "..\system\ParticleProcessor\bb_processor.h"


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
		string Name;
		bool AutoDeleted;
		Matrix matWorld;
	};

	bool ShowStat;
	bool GlobalDelete;

	struct SystemDesc
	{
		ParticleSystem* pSystem;
	};

	array<SystemDesc> Systems;

	//��������� ��� ��������� �������
	ModelProcessor* MDL_Processor;

	//��������� ��� ��������� ������
	BillBoardProcessor* BB_Processor; 

	long pProjectTexture;
	long pProjectTextureNormalMap;
	DataCache* pDataCache;
	GeomCache* pGeomCache;

	string ShortProjectName;
	string TextureName;

	//������ ������� ����� �������
	ParticleService* pService;

  VDX8RENDER* pRS;

	void DeleteAllSystems ();

	float TimeFromLastStatUpdate;
	DWORD nowTickTime;
	DWORD nowUpdateTime;
	DWORD ActiveSystems;
	DWORD ActiveEmitters;
	DWORD ActiveBillboardParticles;
	DWORD ActiveModelParticles;


	array<ParticleSystem*> DeleteQuery;


protected:

 virtual ~ParticleManager ();


 ParticleSystem* CreateParticleSystemFromDataSource (DataSource* pDataSource);

 DWORD IteratorIndex;
 array<string> EnumUsedGeom;

 bool FindInEnumUsedGeom (const char* GeomName);

public:

//��������/�������� 
 ParticleManager (ParticleService* service);
 virtual bool Release ();

//�������� ��������� �� Render/FileService 
 virtual VDX8RENDER* Render ();

//������� ������ 
 virtual bool OpenProject (const char* FileName);
//������� ������ 
 virtual void CloseProject ();
 
//������� �� ������ �������� (���������)
 virtual void RemoveResource (IParticleSystem* pResource);

//��������� �������� 
 virtual void Execute (float DeltaTime);

//������ �������� ������� ��� ��� 
 virtual bool IsSystemAvailable (const char* FileName);

//�������� ���������� �������� ������� 
 virtual long GetProjectTexture ();
//���������� �������� �������
 virtual void SetProjectTexture (const char* FileName = NULL);
 
 

//�������� ��� ������� 
 virtual const char* GetProjectFileName ();
 
 //������� ����������� ������� �� ����� (���� ������ ���� � �������!!!!!)
 virtual IParticleSystem* CreateParticleSystemEx (const char* FileName, const char* File, int Line);

 //������� ������ ����������� �������, ��� ���������...
 virtual IParticleSystem* CreateEmptyParticleSystemEx  (const char* FileName, int Line);

 BillBoardProcessor * GetBBProcessor ();
 ModelProcessor* GetMDLProcessor ();

 virtual bool ValidateSystem (IParticleSystem* pSystem);

 GEOS* GetModel (const char* FileName);

 DWORD GetCreatedSystemCount ();
 ParticleSystem* GetCreatedSystemByIndex (DWORD Index);

 bool ReadyForUse ();

 void DefferedDelete (ParticleSystem* pSys);

 virtual const char* GetProjectTextureName ();

 virtual void Editor_UpdateCachedData ();

 virtual DWORD GetProjectSystemCount ();
 virtual const char* GetProjectSystemName (DWORD Index);

 virtual const char* GetFirstGeomName (const char* FileName);
 virtual const char* GetNextGeomName ();

 virtual void CreateGeomCache ();


 virtual void WriteSystemCache (const char* FileName);
 virtual void WriteSystemCache (const char* FileName, MemFile* pMemFile);
 virtual void WriteSystemCacheAs (const char* FileName, const char* NewName);
 
 virtual void LoadSystemCache (const char* FileName, MemFile* pMemFile);
 virtual void CacheReloaded ();

 virtual FieldList* Editor_CreatePointEmitter (const char* SystemName, const char* EmitterName);
 virtual FieldList* Editor_CreateBillBoardParticle (const char* SystemName, const char*EmitterName, const char* ParticleName);
 virtual FieldList* Editor_CreateModelParticle (const char* SystemName, const char*EmitterName, const char* ParticleName);

 virtual void DeletePointEmitter (const char* SystemName, IEmitter* pEmitter);
 virtual void DeleteBillboard (const char* SystemName, IEmitter* pEmitter, FieldList* pParticles);
 virtual void DeleteModel (const char* SystemName, IEmitter* pEmitter, FieldList* pParticles);


 virtual void OpenDefaultProject ();

 
	
};


#endif