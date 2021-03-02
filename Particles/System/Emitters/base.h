#ifndef BASE_EMITTER
#define BASE_EMITTER

#include "..\..\..\common_h\math3d.h"
#include "..\..\icommon\iemitter.h"
#include "..\..\icommon\particle.h"
#include "..\..\icommon\types.h"
#include "..\datasource\datasource.h"
#include "..\particlesystem\particlesystem.h"
#include "..\datadesc\data_desc.h"

class DataGraph;

class BaseEmitter : public IEmitter
{




	struct structParticleType
	{
		bool Visible;										//����� ��� ���
		ParticleType Type;							// ��� ��������
		float Remain;										// ������� �������� ������������ � �������� �����
		DWORD ActiveCount;							// ���������� �������� ��������� ������� ����
		DWORD MaxParticlesCount;				// ������������ ���-�� ��������� ����� ����
		DataGraph* EmissionRate;				// ������ �������� �������� ���������� ���������

		FieldList* pFields;


		structParticleType ()
		{
			ActiveCount = 0;
			Remain = 0.0f;
			EmissionRate = NULL;
			pFields = NULL;
			Type = UNKNOWN_PARTICLE;
		}

		
	};

	array<structParticleType> ParticleTypes;


	DataSource::EmitterDesc* pEmitter;
	FieldList* pFields;
	bool IsAttachedFlag;


	Vector Position;
	EmitterType Type;
	float LifeTime;
	float ElapsedTime;
	DataGraph* EmissionDirX;
	DataGraph* EmissionDirY;
	DataGraph* EmissionDirZ;

	bool Looped;
	bool Stoped;
	bool Visible;

	DWORD Unique_GUID;

	Matrix matWorldTransform;
	bool OldMatrixNotInitialized;
	Matrix matWorldTransformOld;
	Matrix matWorldTransformNew;

	void BlendMatrix (Matrix& result, const Matrix& mat1, const Matrix& mat2, float BlendK);


	void IncreaseTime (float DeltaTime);

protected:
  
	string Name;
	ParticleSystem* pMaster;

  

public:
 
	// ����������� / ����������
  BaseEmitter(ParticleSystem* pSystem);
  virtual ~BaseEmitter();
  
	//�������� ������� ��� �������� ����� ���������  
	virtual Vector GetNewParticlePosition (float DeltaTime) = 0;
  

	//������ ����� �������� 
	void BornParticles (float DeltaTime);
 	//���������
  virtual void Execute (float DeltaTime);

	//�������������� � ��������� ������
	virtual void AttachToDataSource (DataSource::EmitterDesc* pEmitter);



	virtual void CreateBillBoardParticle (FieldList &Fields);
	virtual void CreateModelParticle (FieldList &Fields);


	ParticleSystem* GetMaster ();
	ParticleManager* GetManager ();
	void GetEmissionDirection (Matrix &matWorld);


	virtual void SetGUID (DWORD GUID)
	{
		Unique_GUID = GUID;
	}

	virtual DWORD GetGUID ()
	{
		return Unique_GUID;
	}

	virtual void Restart ();


	virtual DWORD GetParticleCount ();
	virtual bool IsStoped ();

	virtual void SetTransform (const Matrix& matWorld);
	virtual void Teleport (const Matrix &matWorld);

	virtual const char* GetName ();

	//���� ���� � true ������� �� ����� �������������� ��������� ��������
	//���, ��� �� ��������
	virtual void SetAttachedFlag (bool Flag);
	virtual bool IsAttached ();

	virtual float GetTime ();
	virtual void SetTime (float Time);


	virtual DWORD GetParticleTypesCount ();
	virtual FieldList* GetParticleTypeDataByIndex (DWORD Index);
	virtual ParticleType GetParticleTypeByIndex  (DWORD Index);


	virtual FieldList* GetData ();


	virtual bool SetEnable (bool bVisible);
	virtual bool GetEnable ();

	//-1 ���� �� �����, ����� ������
	virtual int GetParticleTypeIndex (FieldList* pFields);
	virtual bool SetParticleTypeEnable (bool bVisible, DWORD Index);
	virtual bool GetParticleTypeEnable (DWORD Index);


	virtual void Editor_UpdateCachedData ();

	virtual void SetName (const char* Name);

	virtual void Stop ();

};

#endif