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
	void Update(float dltTime) override;
	void LRender(VDX9RENDER* rs) override;

	//�������� ��� ������ �� �����
	bool ExitFromMap();
	//�������� �� ���������� ��������� � ������
	long TestInStorm() const;

	float GetAy() const;

	bool canSkip;

protected:
	//����������� ��������
	virtual void Move(float dltTime);

	void Collide() override;

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	bool goForward;
	float actionRadius;
	float stormEventTime;
};

inline float WdmPlayerShip::GetAy() const {
	return ay;
}

#endif
