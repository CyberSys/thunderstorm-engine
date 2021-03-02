//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Location
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Location_H_
#define _Location_H_

#include <cstdint>
#include "vmodule_api.h"
#include "Matrix.h"

#include "ModelArray.h"
#include "LocatorArray.h"
#include "Supervisor.h"
#include "PtcData.h"

class MODEL;
class Lights;

class Location : public Entity
{
	struct SphVertex
	{
		CVECTOR v;
		uint32_t c;
	};

	struct BarVertex
	{
		CVECTOR p;
		float rhw;
		uint32_t c;
		float u, v;
	};

	struct DmgMessage
	{
		CVECTOR p;
		float alpha;
		float hit, hp;
		uint32_t c;
	};

	struct EnemyBar
	{
		CVECTOR p;
		float hp;
		float energy;
		float alpha;
	};


	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	Location();
	virtual ~Location();


	//�������������
	bool Init() override;
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	//���������
	uint64_t ProcessMessage(MESSAGE& message) override;

	void ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		case Stage::execute:
			Execute(delta);
			break;
		case Stage::realize:
			Realize(delta);
			break;
			/*case Stage::lost_render:
				LostRender(delta); break;
			case Stage::restore_render:
				RestoreRender(delta); break;*/
		}
	}

	//--------------------------------------------------------------------------------------------
	//Location
	//--------------------------------------------------------------------------------------------
public:
	//����� ������ ���������
	LocatorArray* FindLocatorsGroup(const char* gName);

	//�������� ���� ��� ���������
	PtcData& GetPtcData();
	//�������� �������� ����� ��� �������
	MODEL* JmpPatch();

	//��������� ��������� 2-� �����
	bool VisibleTest(const CVECTOR& p1, const CVECTOR& p2);

	//���������� ��� ����� �������
	float Trace(const CVECTOR& src, const CVECTOR& dst);
	bool GetCollideTriangle(TRIANGLE& trg);
	void Clip(PLANE* p, long numPlanes, CVECTOR& cnt, float rad, bool (* fnc)(const CVECTOR* vtx, long num));

	Lights* GetLights();

	VDX9RENDER* GetRS();
	void DrawLine(const CVECTOR& s, uint32_t cs, const CVECTOR& d, uint32_t cd, bool useZ = true);
	//�������� �����
	void Print(const CVECTOR& pos3D, float rad, long line, float alpha, uint32_t color, float scale, const char* format,
	           ...);

	bool IsDebugView();
	bool IsExDebugView();

	bool IsPaused();

	bool IsSwimming();

	//�������� ��������� � �����������
	void AddDamageMessage(const CVECTOR& pos3D, float hit, float curhp, float maxhp);
	//���������� �� ������ ����� ������� ��� ������
	void DrawEnemyBars(const CVECTOR& pos, float hp, float energy, float alpha);

public:
	//������ ����������� �������������� ����������
	Supervisor supervisor;

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	void Update(uint32_t delta_time);
	long LoadStaticModel(const char* modelName, const char* tech, long level, bool useDynamicLights);
	bool LoadCharacterPatch(const char* ptcName);
	void LoadCaustic();
	bool LoadJumpPatch(const char* modelName);
	bool LoadGrass(const char* modelName, const char* texture);
	bool MessageEx(const char* name, MESSAGE& message);
	void UpdateLocators();
	void DrawLocators(LocatorArray* la);
	void CreateSphere();
	void TestLocatorsInPatch(MESSAGE& message);
	//��������� ������� ��� �����������
	void DrawEnemyBars();
	void DrawBar(const MTX_PRJ_VECTOR& vrt, uint32_t color, float hp, float energy);
	void CorrectBar(float v, float start, float end, BarVertex* vrt);


private:
	PtcData ptc;
	long patchJump;

	long lastLoadStaticModel;

	//��� ��������
	std::vector<LocatorArray*> locators;
	long numLocators;
	long maxLocators;

	bool isPause;
	bool isDebugView;

	VDX9RENDER* rs;

	//��� ��������
	ModelArray model;

	//�����
	entid_t grass;
	//���
	entid_t eagle;
	//�������
	entid_t lizards;
	//�����
	entid_t rats;
	//�����
	entid_t blood;

	entid_t lightsid;
	Lights* lights; //��������� ��� �������� �����

	entid_t loceffectsid;

	SphVertex* sphereVertex;
	long sphereNumTrgs;

	float locationTimeUpdate;

	entid_t lighter;
	//entid_t cubeShotMaker;

	DmgMessage message[32];
	long curMessage;

	EnemyBar enemyBar[32];
	long enemyBarsCount;
	long enemyBarsTexture;

	bool bSwimming;
};

//�������� ���� ��� ���������
inline PtcData& Location::GetPtcData()
{
	return ptc;
}

//�������� �������� ����� ��� �������
inline MODEL* Location::JmpPatch()
{
	if (patchJump < 0) return nullptr;
	return model[patchJump];
}

inline VDX9RENDER* Location::GetRS()
{
	return rs;
}

//��������� ��������� 2-� �����
inline bool Location::VisibleTest(const CVECTOR& p1, const CVECTOR& p2)
{
	return model.VisibleTest(p1, p2);
}

inline Lights* Location::GetLights()
{
	return lights;
}

//���������� ��� ����� �������
inline float Location::Trace(const CVECTOR& src, const CVECTOR& dst)
{
	return model.Trace(src, dst);
}

inline bool Location::GetCollideTriangle(TRIANGLE& trg)
{
	return model.GetCollideTriangle(trg);
}

inline void Location::Clip(PLANE* p, long numPlanes, CVECTOR& cnt, float rad,
                           bool (* fnc)(const CVECTOR* vtx, long num))
{
	model.Clip(p, numPlanes, cnt, rad, fnc);
}

inline bool Location::IsPaused()
{
	return isPause;
}


#endif
