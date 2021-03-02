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

#include "..\common_h\dx8render.h"
#include "..\common_h\matrix.h"
#include "..\common_h\templates\array.h"
#include "..\common_h\templates\string.h"

#include "WdmObjectsWind.h"

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
		string path;
		dword hash;
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
	VDX8RENDER * rs;
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
	WdmShip ** ships;
	long numShips;
	long maxShips;

	WdmEnemyShip * enemyShip;
	bool enableSkipEnemy;

	//������	
	WdmStorm ** storms;
	long numStorms;	//���������� �������
	long maxStorms;
	bool playarInStorm;

	const char * curIsland;

	bool isPause;
	bool isDebug;

	void DrawCircle(const CVECTOR & pos, float radius, dword color);
	void DrawCircle(CMatrix & mtx, float radius, dword color);
	void DrawVector(const CVECTOR & start, const CVECTOR & end, dword color);
	void DrawLine(const CVECTOR & start, const CVECTOR & end, dword color);
	void DrawBox2D(CMatrix & mtx, float l, float w, dword color);
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

	array<Model> models;			//��������
	long entryModels[1024];			//������� �������� ������ ���������
	string modelPath;

	//����� ����-�������� ������
	static dword CalcHash(const char * str);

	//�������� ����������� � ���� �����
	float GetWind(float x, float z, CVECTOR & dir);
	//�������� ��������� �����
	void UpdateWind(float dltTime);
	//�������� ������ ����������
	const char * GetWindSaveString(string & windData);
	//���������� ������ ����������
	void SetWindSaveString(const char * str);
	//�������� float � c�����
	void AddDataToString(string & str, byte d);
	//�������� float �� ������
	long GetDataFromString(const char * & cur);

	WindField windField;

private:
	struct Vertex
	{
		CVECTOR v;
		dword c;
	};

	static Vertex vertex[1024];
};

extern WdmObjects * wdmObjects;

//����� ����-�������� ������
inline dword WdmObjects::CalcHash(const char * str)
{
	if(!str) return 0;
	unsigned long hval = 0;
	while(*str != '\0')
	{
		char c = *str++;
		if(c >= 'A' && c <= 'Z') c += 'a' - 'A';
		hval = (hval<<4) + (unsigned long int)c;
		unsigned long g = hval & ((unsigned long int) 0xf << (32 - 4));
		if(g != 0)
		{
			hval ^= g >> (32 - 8);
			hval ^= g;
		}		
	}
	return hval;
}

#endif

