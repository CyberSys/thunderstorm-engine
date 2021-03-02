//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmShip
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmShip_H_
#define _WdmShip_H_

#include "WdmRenderModel.h"


//#define WDM_SHIP_MODELL			6.0f		//�������� ������ �������
//#define WDM_SHIP_MODELW			1.9f		//�������� ������ �������
//#define WDM_SHIP_RADIUS2		(WDM_SHIP_MODELL*WDM_SHIP_MODELL + WDM_SHIP_MODELW*WDM_SHIP_MODELW)

#define WDM_SHIP_SPEED			7.5f		//�������� �������
#define WDM_SHIP_TSPEED			1.0f		//�������� ��������
#define WDM_SHIP_INER_ST		2.5f		//������� �������T
#define WDM_SHIP_INER_BR		1.25f		//������� ����������

#define WDM_SHIP_MAX_SPEED		20.0f		//������������ �������� �������
#define WDM_SHIP_MAX_TURNSPD	0.65f		//������������ �������� ��������


class WdmShip : public WdmRenderModel  
{
	struct Line
	{
		float x, z;
		float ay, size;
	};

	struct Vertex
	{
		float x, y, z;
		dword color;
		float tu, tv;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmShip();
	virtual ~WdmShip();

	void Teleport(float x, float z, float ay);
	void GetPosition(float & x, float & z, float & ay);
	void SetMaxSpeed(float k);

	virtual bool Load(const char * modelName);
	//�������
	virtual void Update(float dltTime);
	virtual void LRender(VDX8RENDER * rs);

	//true ���� ��������
	static bool CheckPosition(float x, float z, float objRadius);

	bool isLive;
	bool isSelect;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
protected:
	void ShipUpdate(float dltTime);
	void UpdateWaterMark(float dltTime);
	virtual void Collide(){};

public:
	//������� �������� ������������ 0
	float modelL05;			//�������� ������ �������� ������������ 0
	float modelW05;			//�������� ������ �������� ������������ 0
	float modelRadius;		//sqrtf(shipModelL*shipModelL + shipModelW*shipModelW)
	float modelRadius2;		//shipModelL*shipModelL + shipModelW*shipModelW

protected:
	float ax, ay, az;
	float dltAx, dltAz;
	float speed, turnspd;
	float baseV;
	float kMaxSpeed;
	CVECTOR rspeed;
	long collisionCounter;
	float slope;
	long wmtexture;
	bool isWMRender;

	Line lines[8];
};

#endif