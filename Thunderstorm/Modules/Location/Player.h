//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Player
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Player_H_
#define _Player_H_

#include "NPCharacter.h"


class LocationCamera;


class Player : public NPCharacter
{
	//--------------------------------------------------------------------------------------------
	//���������������, ������������������
	//--------------------------------------------------------------------------------------------
public:
	Player();
	virtual ~Player();

	bool PostInit() override;

	void Reset() override;

	//���������� ��������� � �������� �������
	void Move(float dltTime) override;
	void Update(float dltTime) override;

	//��������� ���������
	void SetSaveData(ATTRIBUTES* sdata) override;
	//����������� ���������
	void GetSaveData(ATTRIBUTES* sdata) override;

	bool IsPlayer() override { return true; }


	void Rotate(float dltTime);
	bool GoForward(float dltTime);
	bool GoBack(float dltTime) const;
	bool IsRunMode(float dltTime);
	void StrafeWhenMove(float dltTime);
	void StrafeWhenStop(float dltTime);

	bool IsDoBlock();
	bool IsDoParry();
	bool IsDoAttackForce();
	bool IsDoAttackFast();
	bool IsDoAttackRound();
	bool IsDoAttackBreak();
	bool IsDoAttackFeint();
	bool IsFire();
	bool IsChangeFightMode();

	//����� ���������� ����������
	Player* FindAttackCharacter();

	//--------------------------------------------------------------------------------------------
	//������������
	//--------------------------------------------------------------------------------------------
private:
	void FireFromShootgun();
	void ShootParticles(const CVECTOR& pos, const CVECTOR& ndir, float size, uint32_t color, long num);

	float GetRotateH();

private:
	float lastChange;
	bool activatedDialog;
	bool isSpecialMode;
	bool isSetBlock;
	entid_t baterfl;
#ifndef _XBOX
	bool shootgunMode;
	float kSMReload;
	LocationCamera* locCam;
#endif
};

#endif
