//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea dogs II
//--------------------------------------------------------------------------------------------
//	ModelArray
//--------------------------------------------------------------------------------------------
//	�������� ���������� � �������
//============================================================================================

#ifndef _ModelArray_H_
#define _ModelArray_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\model.h"

class Animation;

class PathTracer;

#define MA_MAX_NAME_LENGTH	256

class ModelArray  
{
	class UVSlider : public MODEL::RenderTuner
	{
	public:
		virtual void Set(MODEL * model, VDX8RENDER * rs);
		virtual void Restore(MODEL * model, VDX8RENDER * rs);

		float u0, v0;
		float us0, vs0;
		float u1, v1;		
		float us1, vs1;
	};

	class Relection : public MODEL::RenderTuner
	{
	public:
		virtual void Set(MODEL * model, VDX8RENDER * rs);
		virtual void Restore(MODEL * model, VDX8RENDER * rs);
		dword tfactor;
	};

	struct Rotator
	{
		float rx, ry, rz;
	};

	struct LocationModel
	{
		ENTITY_ID modelrealizer;		//���������� ������
		ENTITY_ID id;					//������
		dword hash;						//���� �������� ��� �������� ������
		union
		{
			dword flags;
			struct
			{
				dword isVisible : 1;

			};
		};
		UVSlider * slider;
		Rotator * rotator;
		Relection * reflection;
		char name[MA_MAX_NAME_LENGTH];	//��� ������
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	ModelArray();
	virtual ~ModelArray();
	
	//������� ������
	long CreateModel(const char * modelName, const char * technique, long level, bool isVisible = true, void* pLights=0);
	//������� ������
	void DeleteModel(long modelIndex);
	//���������� ������ ��������
	bool SetAnimation(long modelIndex, const char * modelAni);
	//����� ������ ������ �� �����
	long FindModel(const char * modelName);

	//��������� �� ������������ ������
	bool IsValidateIndex(long index);
	//�������� �������� ������
	const char * GetModelName(long index);

	//���������� �������
	long Models();
	//��������� ID ������ �� �������
	ENTITY_ID & ID(long modelIndex);
	//��������� ������ �� �������
	MODEL * operator [](long modelIndex);
	//��������� �������� �� �������
	Animation * GetAnimation(long modelIndex);
	//��������� ID ����������� �� �������
	ENTITY_ID & RealizerID(long modelIndex);

	//���������� �������� �������� ���������� uv
	void SetUVSlide(long modelIndex, float u0, float v0, float u1, float v1);
	//���������� �������� �������� ��������
	void SetRotation(long modelIndex, float rx, float ry, float rz);
	//���������� �������� ����� ��������� ������� ��� ���������
	void SetReflection(long modelIndex, float scale);

	//�����������
	void Update(float dltTime);

	void UpdateModelsPath();
	void UpdateTexturesPath();
	void UpdateLightPath();
	void UpdateShadowPath();

	//��������� ��������� 2-� �����
	bool VisibleTest(const CVECTOR & p1, const CVECTOR & p2);
	//���������� ��� ����� �������
	float Trace(const CVECTOR & src, const CVECTOR & dst);
	bool GetCollideTriangle(TRIANGLE & trg);
	void Clip(PLANE * p, long numPlanes, CVECTOR & cnt, float rad, bool (* fnc)(const CVECTOR * vtx, long num));

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	dword CalcHashString(const char * str);
	static void UpdatePath(char * path);
private:
	//������ �������
	LocationModel * model;
	long numModels;
	long maxModels;
	TRIANGLE ctrg;
	bool isHavecTrg;

public:
	char modelspath[256];
	char texturespath[256];
	char lightpath[256];
	char shadowpath[256];
	char resPath[512];
};

//��������� �� ������������ ������
inline bool ModelArray::IsValidateIndex(long index)
{
	return index >= 0 && index < numModels;
}

//�������� �������� ������
inline const char * ModelArray::GetModelName(long index)
{
	if(index >= 0 && index < numModels) return model[index].name;
	return null;
}

#endif

