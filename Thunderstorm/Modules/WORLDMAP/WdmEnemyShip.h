//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmEnemyShip
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmEnemyShip_H_
#define _WdmEnemyShip_H_

#include "WdmShip.h"
#include "attributes.h"

class WdmEnemyShip : public WdmShip
{
	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	WdmEnemyShip();
	virtual ~WdmEnemyShip();

	//�������
	void Update(float dltTime) override;

	//��������� �������� ����������
	void LRender(VDX9RENDER* rs) override;

	//����� ������� ��� ������� ������������ ������
	static bool GeneratePosition(float objRadius, float brnDltAng, float& x, float& z);

	//���������� ����� �����
	void SetLiveTime(float time);
	//�������� ����� �����
	float GetLiveTime() const;

	bool isEnableKill;
	bool isEnemy; //���� ����������, �� ������� ���
	bool isEntryPlayer;

	uint32_t type;
	WdmEnemyShip* attack;

	EnemyShipType shipType;

	bool canSkip;

	//�������� ��� ��������
	const char* GetAttributeName() const;

protected:
	//����� ���� ������������� � ������ �����������
	virtual void FindMoveForce();
	//����� ���� ������������� �� ��������
	virtual void FindIslandForce();
	//����� ���� ������������� �� ��������
	virtual void FindShipsForce();
	//��� ������� �� ����������� ������
	virtual void Move(float dltTime);
	//�������� �� ����������
	virtual bool KillTest();
	//���������� ����������� ������
	virtual void UpdateSaveData();
public:
	//��������� ����������
	virtual void SetSaveAttribute(ATTRIBUTES* save);
	void DeleteUpdate();

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------

protected:
	float mx, mz; //������ ����������� ���� ������������
	float ix, iz; //������ ������������ �� ��������
	float sx, sz; //������ ������������ �� ��������
	float dx, dz; //������ ��������������� �����������

	//�������� �� �������� ��� ������������� �� ������
	bool isLookOnPlayer;

	ATTRIBUTES* saveAttribute;

private:
	float brnAlpha;
	float deleteAlpha;
	float liveTime;
	float slowingAlfa;
};

//�������� ����� �����
inline float WdmEnemyShip::GetLiveTime() const {
	if (!isEnableKill) return -1.0f;
	return liveTime >= 0.0f ? liveTime : 0.0f;
}

#endif
