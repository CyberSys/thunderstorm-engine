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

#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"

#include "ModelArray.h"
#include "LocatorArray.h"
#include "Supervisor.h"
#include "PtcData.h"

class MODEL;
class Lights;

class Location : public ENTITY
{
	struct SphVertex
	{
		CVECTOR v;
		dword c;
	};

	struct BarVertex
	{
		CVECTOR p;
		float rhw;
		dword c;
		float u, v;
	};

	struct DmgMessage
	{
		CVECTOR p;
		float alpha;
		float hit, hp;
		dword c;
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
	bool Init();
	//����������
	void Execute(dword delta_time);
	void Realize(dword delta_time);
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);

	
//--------------------------------------------------------------------------------------------
//Location
//--------------------------------------------------------------------------------------------
public:
	//����� ������ ���������
	LocatorArray * FindLocatorsGroup(const char * gName);

	//�������� ���� ��� ���������
	PtcData & GetPtcData();
	//�������� �������� ����� ��� �������
	MODEL * JmpPatch();

	//��������� ��������� 2-� �����
	bool VisibleTest(const CVECTOR & p1, const CVECTOR & p2);

	//���������� ��� ����� �������
	float Trace(const CVECTOR & src, const CVECTOR & dst);
	bool GetCollideTriangle(TRIANGLE & trg);
	void Clip(PLANE * p, long numPlanes, CVECTOR & cnt, float rad, bool (* fnc)(const CVECTOR * vtx, long num));
	
	Lights * GetLights();

	VDX8RENDER * GetRS();
	void DrawLine(const CVECTOR & s, dword cs, const CVECTOR & d, dword cd, bool useZ = true);
	//�������� �����
	void _cdecl Print(const CVECTOR & pos3D, float rad, long line, float alpha, dword color, float scale, const char * format, ...);

	bool IsDebugView();
	bool IsExDebugView();

	bool IsPaused();

	bool IsSwimming();

	//�������� ��������� � �����������
	void AddDamageMessage(const CVECTOR & pos3D, float hit, float curhp, float maxhp);
	//���������� �� ������ ����� ������� ��� ������
	void DrawEnemyBars(const CVECTOR & pos, float hp, float energy, float alpha);

public:
	//������ ����������� �������������� ����������
	Supervisor supervisor;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void Update(dword delta_time);
	long LoadStaticModel(const char * modelName, const char * tech, long level, bool useDynamicLights);
	bool LoadCharacterPatch(const char * ptcName);
	void LoadCaustic();
	bool __declspec(dllexport) __cdecl LoadJumpPatch(const char * modelName);
	bool __declspec(dllexport) __cdecl LoadGrass(const char * modelName, const char * texture);
	bool MessageEx(const char * name, MESSAGE & message);
	void UpdateLocators();
	void DrawLocators(LocatorArray * la);
	void CreateSphere();
	void TestLocatorsInPatch(MESSAGE & message);
	//��������� ������� ��� �����������
	void DrawEnemyBars();
	void DrawBar(const MTX_PRJ_VECTOR & vrt, dword color, float hp, float energy);
	void CorrectBar(float v, float start, float end, BarVertex * vrt);
	

private:
	PtcData ptc;
	long patchJump;

	long lastLoadStaticModel;
	
	//��� ��������
	LocatorArray ** locators;
	long numLocators;
	long maxLocators;

	bool isPause;
	bool isDebugView;

	VDX8RENDER * rs;
	
	//��� ��������
	ModelArray model;

	//�����
	ENTITY_ID grass;
	//���
	ENTITY_ID eagle;
	//�������
	ENTITY_ID lizards;
	//�����
	ENTITY_ID rats;
	//�����
	ENTITY_ID blood;
	
	ENTITY_ID lightsid;
	Lights * lights;	//��������� ��� �������� �����

	ENTITY_ID loceffectsid;

	SphVertex * sphereVertex;
	long sphereNumTrgs;

	float locationTimeUpdate;

	ENTITY_ID lighter;
	ENTITY_ID cubeShotMaker;

	DmgMessage message[32];
	long curMessage;

	EnemyBar enemyBar[32];
	long enemyBarsCount;
	long enemyBarsTexture;

	bool bSwimming;
};

//�������� ���� ��� ���������
inline PtcData & Location::GetPtcData()
{
	return ptc;
}

//�������� �������� ����� ��� �������
inline MODEL * Location::JmpPatch()
{
	if(patchJump < 0) return null;
	return model[patchJump];	
}

inline VDX8RENDER * Location::GetRS()
{
	return rs;
}

//��������� ��������� 2-� �����
inline bool Location::VisibleTest(const CVECTOR & p1, const CVECTOR & p2)
{
	return model.VisibleTest(p1, p2);
}

inline Lights * Location::GetLights()
{
	return lights;
}

//���������� ��� ����� �������
inline float Location::Trace(const CVECTOR & src, const CVECTOR & dst)
{
	return model.Trace(src, dst);
}

inline bool Location::GetCollideTriangle(TRIANGLE & trg)
{
	return model.GetCollideTriangle(trg);
}

inline void Location::Clip(PLANE * p, long numPlanes, CVECTOR & cnt, float rad, bool (* fnc)(const CVECTOR * vtx, long num))
{
	model.Clip(p, numPlanes, cnt, rad, fnc);
}

inline bool Location::IsPaused()
{
	return isPause;
}


#endif


