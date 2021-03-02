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

	virtual bool PostInit();

	virtual void Reset();

	//���������� ��������� � �������� �������
	virtual void Move(float dltTime);
	virtual void Update(float dltTime);

	//��������� ���������
	virtual void SetSaveData(ATTRIBUTES * sdata);
	//����������� ���������
	virtual void GetSaveData(ATTRIBUTES * sdata);

	virtual bool IsPlayer() {return true;}


	void Rotate(float dltTime);
	bool GoForward(float dltTime);
	bool GoBack(float dltTime);
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
	Player * FindAttackCharacter();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void FireFromShootgun();
	void ShootParticles(const CVECTOR & pos, const CVECTOR & ndir, float size, dword color, long num);

	float GetRotateH();

private:
	float lastChange;
	long chrGroups;
	bool activatedDialog;
	bool isSpecialMode;
	bool isSetBlock;
	ENTITY_ID baterfl;
#ifndef _XBOX
	bool shootgunMode;
	float kSMReload;
	LocationCamera * locCam;
#endif
};

#endif