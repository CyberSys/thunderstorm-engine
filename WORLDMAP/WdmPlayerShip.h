//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmPlayerShip
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmPlayerShip_H_
#define _WdmPlayerShip_H_

#include "WdmShip.h"

class WdmPlayerShip : public WdmShip  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmPlayerShip();
	virtual ~WdmPlayerShip();

	void PushOutFromIsland();

	void SetActionRadius(float radius);

	//�������
	virtual void Update(float dltTime);
	virtual void LRender(VDX8RENDER * rs);

	//�������� ��� ������ �� �����
	bool ExitFromMap();
	//�������� �� ���������� ��������� � ������
	long TestInStorm();

	float GetAy();

	bool canSkip;

protected:
	//����������� ��������
	virtual void Move(float dltTime);

	virtual void Collide();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	bool goForward;
	float actionRadius;	
	float stormEventTime;
};

inline float WdmPlayerShip::GetAy()
{
	return ay;
}

#endif

