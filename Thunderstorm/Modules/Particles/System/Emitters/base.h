#ifndef BASE_EMITTER
#define BASE_EMITTER

#include "math3D.h"
#include "../../ICommon/IEmitter.h"
#include "../../ICommon/Types.h"
#include "../DataSource/DataSource.h"
#include "../ParticleSystem/particlesystem.h"

class DataGraph;

class BaseEmitter : public IEmitter
{
	struct structParticleType
	{
		bool Visible;										//����� ��� ���
		ParticleType Type;							// ��� ��������
		float Remain;										// ������� �������� ������������ � �������� �����
		uint32_t ActiveCount;							// ���������� �������� ��������� ������� ����
		uint32_t MaxParticlesCount;				// ������������ ���-�� ��������� ����� ����
		DataGraph* EmissionRate;				// ������ �������� �������� ���������� ���������

		FieldList* pFields;


		structParticleType ()
		{
			ActiveCount = 0;
			Remain = 0.0f;
			EmissionRate = nullptr;
			pFields = nullptr;
			Type = UNKNOWN_PARTICLE;
		}

		
	};

	std::vector<structParticleType> ParticleTypes;


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

	uint32_t Unique_GUID;

	Matrix matWorldTransform;
	bool OldMatrixNotInitialized;
	Matrix matWorldTransformOld;
	Matrix matWorldTransformNew;

	void BlendMatrix (Matrix& result, const Matrix& mat1, const Matrix& mat2, float BlendK);


	void IncreaseTime (float DeltaTime);

protected:

	std::string Name;
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


	virtual void SetGUID (uint32_t GUID)
	{
		Unique_GUID = GUID;
	}

	virtual uint32_t GetGUID ()
	{
		return Unique_GUID;
	}

	virtual void Restart ();


	virtual uint32_t GetParticleCount ();
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


	virtual uint32_t GetParticleTypesCount ();
	virtual FieldList* GetParticleTypeDataByIndex (uint32_t Index);
	virtual ParticleType GetParticleTypeByIndex  (uint32_t Index);


	virtual FieldList* GetData ();


	virtual bool SetEnable (bool bVisible);
	virtual bool GetEnable ();

	//-1 ���� �� �����, ����� ������
	virtual int GetParticleTypeIndex (FieldList* pFields);
	virtual bool SetParticleTypeEnable (bool bVisible, uint32_t Index);
	virtual bool GetParticleTypeEnable (uint32_t Index);


	virtual void Editor_UpdateCachedData ();

	virtual void SetName (const char* Name);

	virtual void Stop ();

};

#endif