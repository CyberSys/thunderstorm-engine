//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmObjects
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmObjects_H_
#define _WdmObjects_H_

#include "../Common_h/dx9render.h"
#include "../Common_h/Matrix.h"

#include "WdmObjectsWind.h"

#include <vector>
#include <string>

class WdmIslands;
class WdmShip;
class WdmPlayerShip;
class WorldMap;
class WdmCamera;
class WdmLocations;
class WdmStorm;
class WdmEnemyShip;


//#define WDM_WORLD_SIZE_X	2000.0f
//#define WDM_WORLD_SIZE_Z	2000.0f

enum EnemyShipType
{
	wdmest_unknow,
	wdmest_merchant,
	wdmest_warring,
	wdmest_follow,
};

class GEOS;
class VGEOMETRY;

class WdmObjects  
{
	struct Model
	{
		GEOS * geo;
		std::string path;
		uint32_t hash;
		long next;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmObjects();
	virtual ~WdmObjects();

	void SetWorldSize(float x, float z);
	void Clear();

	void AddShip(WdmShip * ship);
	void DelShip(WdmShip * ship);

	void AddStorm(WdmStorm * storm);
	void DelStorm(WdmStorm * storm);

	//������� ���������
	GEOS * CreateGeometry(const char * path);

	//����������� �������, ���������� ����
	WorldMap * wm;
	//������ �������
	VDX9RENDER * rs;
	//������ ����������
	VGEOMETRY * gs;
	//������
	WdmCamera * camera;
	//�������
	WdmIslands * islands;

	//�������
	//������� ������
	WdmShip * playerShip;
	//��� ������������ �������
	std::vector<WdmShip *> ships;

	WdmEnemyShip * enemyShip;
	bool enableSkipEnemy;

	//������	
	std::vector<WdmStorm *> storms;
	bool playarInStorm;

	const char * curIsland;

	bool isPause;
	bool isDebug;

	void DrawCircle(const CVECTOR & pos, float radius, uint32_t color);
	void DrawCircle(CMatrix & mtx, float radius, uint32_t color);
	void DrawVector(const CVECTOR & start, const CVECTOR & end, uint32_t color);
	void DrawLine(const CVECTOR & start, const CVECTOR & end, uint32_t color);
	void DrawBox2D(CMatrix & mtx, float l, float w, uint32_t color);
	void GetVPSize(float & w, float & h);

	float shipSpeedOppositeWind;	//������������� �������� ������� ������ �����
	float shipSpeedOverWind;		//������������� �������� ������� �� �����

	float enemyshipViewDistMin;		//��������� �� ������� ������� �������� ��������
	float enemyshipViewDistMax;		//��������� �� ������� ������� �������� ���������
	float enemyshipDistKill;		//���������� �� ������� ������� �������
	float enemyshipBrnDistMin;		//����������� ��������� �� ������� �������� �������
	float enemyshipBrnDistMax;		//������������ ��������� �� ������� �������� �������

	float stormViewDistMin;			//��������� �� ������� ����� �������� ��������
	float stormViewDistMax;			//��������� �� ������� ����� �������� ���������
	float stormDistKill;			//���������� �� ������� ������� �����
	float stormBrnDistMin;			//����������� ��������� �� ������� �������� �����
	float stormBrnDistMax;			//������������ ��������� �� ������� �������� �����
	float stormZone;				//����� ������ �������� ������


	char attrSec[256];				//������� �� ������� �����
	char attrMin[256];				//������ �� ������� �����
	char attrHour[256];				//���� �� ������� �����
	char attrDay[256];				//���� �� ������� �����
	char attrMonth[256];			//����� �� ������� �����
	char attrYear[256];				//��� �� ������� �����
	bool isNextDayUpdate;			//���� �������� ������ ���������� ���

	float worldSizeX;				//������ ���� �� X
	float worldSizeZ;				//������ ���� �� Z

	std::vector<Model> models;			//��������
	long entryModels[1024];			//������� �������� ������ ���������
	std::string modelPath;

	//�������� ����������� � ���� �����
	float GetWind(float x, float z, CVECTOR & dir);
	//�������� ��������� �����
	void UpdateWind(float dltTime);
	//�������� ������ ����������
	const char * GetWindSaveString(std::string & windData);
	//���������� ������ ����������
	void SetWindSaveString(const char * str);
	//�������� float � c�����
	void AddDataToString(std::string & str, uint8_t d);
	//�������� float �� ������
	long GetDataFromString(const char * & cur);

	WindField windField;

private:
	struct Vertex
	{
		CVECTOR v;
		uint32_t c;
	};

	static Vertex vertex[1024];
};

extern WdmObjects * wdmObjects;

#endif

