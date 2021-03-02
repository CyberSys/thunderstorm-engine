//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WorldMap
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WorldMap_H_
#define _WorldMap_H_

#include "../Common_h/vmodule_api.h"
#include <string>


#define WDMAP_MAXOBJECTS	4096



class WdmRenderObject;
class WdmRenderModel;
class VDX9RENDER;
class WdmCamera;
class WdmEventWindow;
class WdmWaitMenu;

class WorldMap : public ENTITY
{
	struct RObject
	{
		WdmRenderObject * ro;
		long level;
		long next;
		long prev;
	};


//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WorldMap();
	virtual ~WorldMap();

//--------------------------------------------------------------------------------------------
//Entity
//--------------------------------------------------------------------------------------------
public:
	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	//���������
	uint32_t _cdecl ProcessMessage(MESSAGE & message);
	//��������� ��������
	uint32_t AttributeChanged(ATTRIBUTES * apnt);

//--------------------------------------------------------------------------------------------
//���������� ���������
//--------------------------------------------------------------------------------------------
public:
	//�������� ������
	WdmRenderObject * AddObject(WdmRenderObject * obj, long level = 0);
	//�������� ������ � ������ ��������� �� ���������
	void AddPObject(WdmRenderObject * obj, long level = 0);
	//�������� ������ � ������ ��������� ���������
	void AddMObject(WdmRenderObject * obj, long level = 0);
	//�������� ������ � ������ ��������� ����� ���������
	void AddLObject(WdmRenderObject * obj, long level = 0);
	//������� ������
	void DeleteObject(WdmRenderObject * obj);





	//������������������� �������� � ������� � ������ ������ �������
	WdmRenderObject * CreateModel(WdmRenderModel * rm, const char * modelName, bool pr = false, bool mr = true, bool lr = true, long objectLevel = 0, long drawLevel = 0);


//--------------------------------------------------------------------------------------------

	VDX9RENDER * GetRS();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//���������� ���������
	//�������� ������ �� ������� � ������ � ����������� �������
	long GetObject(long & first, long level);
	//��������� ������ �� ������
	void FreeObject(long & first, long i);

	//���������
	//������� �����, ���� ��� ��������
	bool CreateStorm(bool isTornado, float time = -1.0f, ATTRIBUTES * save = nullptr);
	//������� �������� �����
	bool CreateMerchantShip(const char * modelName, const char * locNameStart, const char * locNameEnd, float kSpeed, float time = -1.0f, ATTRIBUTES * save = nullptr);
	// boal ������� �������� ����� � �����������
	bool CreateMerchantShipXZ(const char * modelName, float x1, float z1, float x2, float z2, float kSpeed, float time = -1.0f, ATTRIBUTES * save = nullptr);
	//������� �������� ������������ ���
	bool CreateFollowShip(const char * modelName, float kSpeed, float time = -1.0f, ATTRIBUTES * save = nullptr);
	//������� ���� ������� ���������
	bool CreateWarringShips(const char * modelName1, const char * modelName2, float time = -1.0f, ATTRIBUTES * save1 = nullptr, ATTRIBUTES * save2 = nullptr);
	//������� ��� ����������
	void ReleaseEncounters();
	//������� ������� ��� ���������� ���������� ����������
	ATTRIBUTES * GetEncSaveData(const char * type, const char * retName);


	//����� ���������� � ������ �� ����� ����������
	bool FindIslandPosition(const char * name, float & x, float & z, float & r);

	void ResetScriptInterfaces();

private:
	//������ �������
	VDX9RENDER * rs;
	WdmCamera * camera;

	ATTRIBUTES * aStorm;
	ATTRIBUTES * aEncounter;
	ATTRIBUTES * aInfo;
	ATTRIBUTES * saveData;

	WdmWaitMenu * waitMenu;

	//�������
	float encTime;

	//�������
	long firstFreeObject;
	long firstObject;					//��� ������������ �������
	long firstPrObject;					//�������, ������������ �� �������
	long firstMrObject;					//������� ������������ � �������
	long firstLrObject;					//������� ������������ ����� �������
	
	RObject object[WDMAP_MAXOBJECTS];


	ATTRIBUTES * aDate;
	float timeScale;

	std::string bufForSave;

public:
	uint32_t encCounter;

	float hour;
	long day;
	long mon;
	long year;

	static long month[];
};

inline VDX9RENDER * WorldMap::GetRS()
{
	return rs;
}

#endif
