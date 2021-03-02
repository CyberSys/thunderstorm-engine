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

#include "vmodule_api.h"
#include "model.h"

class Animation;

class PathTracer;

#define MA_MAX_NAME_LENGTH	256

class ModelArray
{
	class UVSlider : public MODEL::RenderTuner
	{
	public:
		void Set(MODEL* model, VDX9RENDER* rs) override;
		void Restore(MODEL* model, VDX9RENDER* rs) override;

		float u0, v0;
		float us0, vs0;
		float u1, v1;
		float us1, vs1;
	};

	class Relection : public MODEL::RenderTuner
	{
	public:
		void Set(MODEL* model, VDX9RENDER* rs) override;
		void Restore(MODEL* model, VDX9RENDER* rs) override;
		uint32_t tfactor;
	};

	struct Rotator
	{
		float rx, ry, rz;
	};

	struct LocationModel
	{
		entid_t modelrealizer; //���������� ������
		entid_t id; //������
		uint32_t hash; //���� �������� ��� �������� ������
		union
		{
			uint32_t flags;

			struct
			{
				uint32_t isVisible : 1;
			};
		};

		UVSlider* slider;
		Rotator* rotator;
		Relection* reflection;
		char name[MA_MAX_NAME_LENGTH]; //��� ������
	};

	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	ModelArray();
	virtual ~ModelArray();

	//������� ������
	long CreateModel(const char* modelName, const char* technique, long level, bool isVisible = true,
	                 void* pLights = nullptr);
	//������� ������
	void DeleteModel(long modelIndex);
	//���������� ������ ��������
	bool SetAnimation(long modelIndex, const char* modelAni);
	//����� ������ ������ �� �����
	long FindModel(const char* modelName);

	//��������� �� ������������ ������
	bool IsValidateIndex(long index);
	//�������� �������� ������
	const char* GetModelName(long index);

	//���������� �������
	long Models();
	//��������� ID ������ �� �������
	entid_t ID(long modelIndex);
	//��������� ������ �� �������
	MODEL* operator [](long modelIndex);
	//��������� �������� �� �������
	Animation* GetAnimation(long modelIndex);
	//��������� ID ����������� �� �������
	entid_t RealizerID(long modelIndex);

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
	bool VisibleTest(const CVECTOR& p1, const CVECTOR& p2);
	//���������� ��� ����� �������
	float Trace(const CVECTOR& src, const CVECTOR& dst);
	bool GetCollideTriangle(TRIANGLE& trg);
	void Clip(PLANE* p, long numPlanes, CVECTOR& cnt, float rad, bool (* fnc)(const CVECTOR* vtx, long num));

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	uint32_t CalcHashString(const char* str);
	static void UpdatePath(char* path);
private:
	//������ �������
	std::vector<LocationModel> model;
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
inline const char* ModelArray::GetModelName(long index)
{
	if (index >= 0 && index < numModels) return model[index].name;
	return nullptr;
}

#endif
