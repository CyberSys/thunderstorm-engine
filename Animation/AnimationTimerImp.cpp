//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationTimerImp
//--------------------------------------------------------------------------------------------
//	���������� ������� ��������
//============================================================================================

#include "AnimationTimerImp.h"
#include "AnimationImp.h"


//============================================================================================
//���������������, ������������������
//============================================================================================

AnimationTimerImp::AnimationTimerImp()
{
	ani = null;
	for(long i = 0; i < ANITM_PLAYERSMAX; i++)
	{
		playersMask[i] = 0;
		inverseMask[i] = 0;
	}
	curTime = 0.0f;
	kTime = 0.0f;
	value = 1.0f;
}

AnimationTimerImp::~AnimationTimerImp()
{
}

//���������� ��������
void AnimationTimerImp::SetAnimation(AnimationImp * animation)
{
	Assert(!ani && animation);
	ani = animation;
}

//--------------------------------------------------------------------------------------------
//AnimationTimer
//--------------------------------------------------------------------------------------------	

//��������� ������ (����� � �������������)
void AnimationTimerImp::Start(float time, float startTime)
{
	if(startTime < 0) startTime = 0;
	if(startTime >= time || time <= 0)
	{
		Stop();
		return;
	}
	curTime = startTime;
	kTime = 1.0f/time;
	value = 0.0f;
	//if(value < 0.0f) value = 0.0f;
	//if(value > 1.0f) value = 1.0f;
	Execute(0);
}

//���������� ������
void AnimationTimerImp::Stop()
{
	curTime = 0.0f;
	kTime = 0.0f;
	value = 1.0f;
	//������������� �������� �������� � ������������� ��������, ��� ������
	for(long i = 0; i < ANI_MAX_ACTIONS; i++)
	{
		if(playersMask[i >> 5] & (1 << (i & 31)))
		{
			if(inverseMask[i >> 5] & (1 << (i & 31)))
			{
				((ActionPlayerImp *)&ani->Player(i))->TimerBlend() = 0.0f;
				if(ani->Player(i).IsAutoStop()) ani->Player(i).Stop();
			}else{
				((ActionPlayerImp *)&ani->Player(i))->TimerBlend() = 1.0f;
			}
		}
	}
	//�������� �������
	for(i = 0; i < ANI_MAX_ACTIONS; i++)
	{
		if(&ani->Timer(i) == this) break;
	}
	Assert(i < ANI_MAX_ACTIONS);
	ani->AteTimerstop(i);
}

//�������� ��������� �������
void AnimationTimerImp::ResetTimer()
{
	curTime = 0.0f;
	kTime = 0.0f;
	value = 0.0f;
	for(long i = 0; i < ANITM_PLAYERSMAX; i++)
	{
		playersMask[i] = 0;
		inverseMask[i] = 0;
	}
}

//������, �������� �� ������
bool AnimationTimerImp::IsWork()
{
	return kTime != 0.0f;
}

//�������� ������� ��������
float AnimationTimerImp::GetCurrent() const
{
	return value;
}

//�������� �������� ����� ������ �������
float AnimationTimerImp::GetTime() const
{
	return (kTime > 0.0f) ? 1.0f/kTime : 0.0f;
}



//��������� ActionPlayer ��� ��������� (isInverse == false -> kBlend = [0..1])
void AnimationTimerImp::SetPlayer(long playerIndex, bool isInverse)
{
	Assert(playerIndex >= 0 || playerIndex < ANI_MAX_ACTIONS);
	//������������� ����
	playersMask[playerIndex >> 5] |= 1 << (playerIndex & 31);
	//��������� ���� ������������
	if(isInverse)
	{
		inverseMask[playerIndex >> 5] |= 1 << (playerIndex & 31);
	}else{
		inverseMask[playerIndex >> 5] &= ~(1 << (playerIndex & 31));
	}
	((ActionPlayerImp *)&ani->Player(playerIndex))->TimerBlend() = 1.0f;
}

void AnimationTimerImp::ResetPlayer(long playerIndex)
{
	Assert(playerIndex >= 0 || playerIndex < ANI_MAX_ACTIONS);
	//������� ����� �������������
	playersMask[playerIndex >> 5] &= ~(1 << (playerIndex & 31));
	((ActionPlayerImp *)&ani->Player(playerIndex))->TimerBlend() = 1.0f;
}

//������, ������������ �� ActionPlayer
bool AnimationTimerImp::IsUsedPlayer(long playerIndex, bool * isInverse)
{
	Assert(playerIndex >= 0 || playerIndex < ANI_MAX_ACTIONS);
	if(playersMask[playerIndex >> 5] & (1 << (playerIndex & 31)))
	{
		if(isInverse)
		*isInverse = (inverseMask[playerIndex >> 5] & (1 << (playerIndex & 31))) != 0;
		return true;
	}
	return false;
}

//�������� �������� ��������� ��� ������ (���� �� ������������ �� 1.0f)
float AnimationTimerImp::GetPlayerValue(long playerIndex)
{
	Assert(playerIndex >= 0 || playerIndex < ANI_MAX_ACTIONS);
	if(playersMask[playerIndex >> 5] & (1 << (playerIndex & 31)))
	{
		if((inverseMask[playerIndex >> 5] & (1 << (playerIndex & 31))) != 0)
		{
			return kTime != 0 ? 1.0f - value : 0.0f;			
		}else return kTime != 0 ? value : 1.0f;
	}
	return 1.0f;
}

//--------------------------------------------------------------------------------------------
//AnimationTimerImp
//--------------------------------------------------------------------------------------------	

//���������
void AnimationTimerImp::Execute(long dltTime)
{
	if(kTime != 0.0f)
	{
		//���������
		curTime += dltTime*0.001f;
		value = curTime*kTime;
		if(value > 1.0f) value = 1.0f;
		//����������� ����������� ���������
		for(long i = 0; i < ANI_MAX_ACTIONS; i++)
		{
			if(playersMask[i >> 5] & (1 << (i & 31)))
			{
				if(inverseMask[i >> 5] & (1 << (i & 31)))
				{
					((ActionPlayerImp *)&ani->Player(i))->TimerBlend() *= 1.0f - value;
				}else{
					((ActionPlayerImp *)&ani->Player(i))->TimerBlend() *= value;
				}
			}
		}
		//����� ���� ������������		
		if(value == 1.0f) Stop();
	}
}




