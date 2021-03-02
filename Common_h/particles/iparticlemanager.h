//****************************************************************
//*
//*  Author : Sergey Makeev aka Joker, 2003
//*
//*
//****************************************************************

#ifndef PARTICLES_MANAGER_INTERFACE
#define PARTICLES_MANAGER_INTERFACE

#include <d3d8.h>
#include "..\dx8render.h"

class ParticleService;
class IParticleSystem;


#ifndef NULL
#define NULL 0L
#endif


#define CreateParticleSystem(name) CreateParticleSystemEx(name, __FILE__, __LINE__)

//�������� ����������� ������
class IParticleManager
{


protected:

	virtual ~IParticleManager () {};

public:

//��������/�������� 
	IParticleManager (ParticleService* service) {};
	virtual bool Release () = 0;

//�������� ��������� �� Render/FileService 
	virtual VDX8RENDER* Render () = 0;

//������� ������ 
	virtual bool OpenProject (const char* FileName) = 0;
//������� ������ 
	virtual void CloseProject () = 0;
 
//������� �� ������ �������� (���������)
	virtual void RemoveResource (IParticleSystem* pResource) = 0;

//��������� �������� 
	virtual void Execute (float DeltaTime) = 0;

//������ �������� ������� ��� ��� 
	virtual bool IsSystemAvailable (const char* FileName) = 0;

//�������� ���������� �������� ������� 
	virtual long GetProjectTexture () = 0;
	
	//���������� �������� �������
	virtual void SetProjectTexture (const char* FileName = NULL) = 0;


 


//�������� ��� ������� 
 virtual const char* GetProjectFileName () = 0;
 

 //������� ������ ����������� �������, ��� ���������...
 virtual IParticleSystem* CreateEmptyParticleSystemEx  (const char* FileName, int Line) = 0;


//������� ����������� ������� �� ����� (���� ������ ���� � �������!!!!!)
 virtual IParticleSystem* CreateParticleSystemEx (const char* FileName, const char* File, int Line) = 0;


 //��������� "�������" �� ��������� �� ������� ���������, ����� ��� ��� ���������
 virtual bool ValidateSystem (IParticleSystem* pSystem) = 0;


 //��������� �� ����������� ���������� ��� �������� � ����� �� ��������
 //� �������� �������
 virtual bool ReadyForUse () = 0;


 virtual const char* GetProjectTextureName () = 0;

 virtual void Editor_UpdateCachedData () = 0;

 virtual const char* GetFirstGeomName (const char* FileName) = 0;
 virtual const char* GetNextGeomName () = 0;


 virtual void WriteSystemCache (const char* FileName) = 0;



 virtual void OpenDefaultProject () = 0;


 
	
};


#endif