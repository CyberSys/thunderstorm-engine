//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	AnimationTimerImp
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _AnimationTimerImp_H_
#define _AnimationTimerImp_H_

#include "..\common_h\Animation.h"


#define ANITM_PLAYERSMAX	((ANI_MAX_ACTIONS + 31) >> 5)


class AnimationImp;


class AnimationTimerImp : public AnimationTimer  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	AnimationTimerImp();
	virtual ~AnimationTimerImp();

	//���������� ��������
	void SetAnimation(AnimationImp * animation);

//--------------------------------------------------------------------------------------------
//AnimationTimer
//--------------------------------------------------------------------------------------------	
public:
	//��������� ������ (����� � �������������)
	virtual void Start(float time, float startTime = 0.0f);
	//���������� ������
	virtual void Stop();
	//�������� ��������� �������
	virtual void ResetTimer();
	//������, �������� �� ������
	virtual bool IsWork();
	//�������� ������� ��������
	virtual float GetCurrent() const;
	//�������� �������� ����� ������ �������
	virtual float GetTime() const;
	//��������� ActionPlayer ��� ��������� (isInverse == false -> kBlend = [0..1])
	virtual void SetPlayer(long playerIndex, bool isInverse = false);
	virtual void ResetPlayer(long playerIndex);
	//������, ������������ �� ActionPlayer
	virtual bool IsUsedPlayer(long playerIndex, bool * isInverse = null);
	//�������� �������� ��������� ��� ������ (���� �� ������������ �� 1.0f)
	virtual float GetPlayerValue(long playerIndex);

//--------------------------------------------------------------------------------------------
//AnimationTimerImp
//--------------------------------------------------------------------------------------------	
public:
	//���������
	void Execute(long dltTime);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//��������
	AnimationImp * ani;
	//��������� �������
	float curTime;
	float kTime;
	float value;
	//����, ������������ ���� ���� �������������
	dword playersMask[ANITM_PLAYERSMAX];
	dword inverseMask[ANITM_PLAYERSMAX];
};

#endif

