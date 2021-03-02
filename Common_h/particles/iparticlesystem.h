#ifndef PARTICLE_SYSTEM_INTERFACE
#define PARTICLE_SYSTEM_INTERFACE

#include "..\math3d.h"
#include "iparticlemanager.h"

class IGMXEntity;


class IParticleSystem
{

protected:

	virtual ~IParticleSystem() {};

public:

//��������/�������� 
	IParticleSystem() {};
	virtual bool Release () = 0;

//���������� ���� ���������
  virtual DWORD Execute (float DeltaTime) = 0;


//������������� ����������� �������  
  virtual void Restart (DWORD RandomSeed) = 0;
  
//��������� ���������� ���������  
  virtual void PauseEmission (bool bPause) = 0;
  
//������ �� ����� ������� ��� ���  
  virtual bool IsEmissionPaused () = 0;
//���������� ������������� ������� ��� �������...  
  virtual void AutoDelete (bool Enabled) = 0;
//������� ������������� ������� ��� ���  
	virtual bool IsAutoDeleted () = 0;
//���������� ������� ������������� ��� ������� 
	virtual void SetTransform (const Matrix& transform) = 0;
	virtual void GetTransform (Matrix& _matWorld) = 0;

	virtual void Teleport (const Matrix &transform) = 0;

	virtual bool IsAlive () = 0;

	virtual const char* GetName () =0;

	virtual void Stop () = 0;


};

#endif