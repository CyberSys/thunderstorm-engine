//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	ActionPlayerImp
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _AnimationImp_H_
#define _AnimationImp_H_

#include "..\COMMON_H\Animation.h"

class AnimationImp;
class ActionInfo;

#define ACTIONPLAYEREVENTFLAGS	((ANI_MAX_EVENTS + 31) >> 5)


class ActionPlayerImp : public ActionPlayer
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	ActionPlayerImp();
	virtual ~ActionPlayerImp();

	//���������� ��������� �� ��������
	void SetAnimation(AnimationImp * animation, long index);

//--------------------------------------------------------------------------------------------
//ActionPlayer
//--------------------------------------------------------------------------------------------
public:
	//���������� ������� ��������
	virtual bool SetAction(const char * actionName);
	virtual const char * GetAction() const;
	//���������� �������������
	virtual bool Play();
	virtual void Pause();
	virtual void Stop();
	virtual bool IsPlaying() const;
	virtual bool IsPause() const;
	//������������� ��� ���������� ������ �������
	virtual bool SetAutoStop(bool isStop);
	virtual bool IsAutoStop() const;
	//������� ������� ������������
	virtual float SetPosition(float position);
	virtual float GetPosition() const;
	//��� ������������
	virtual void SetType(AnimationType atype);
	virtual AnimationType GetType() const;
	//���������� �������� ������������
	virtual float SetSpeed(float kSpeed = 1.0f);
	virtual float GetSpeed() const;
	virtual float GetDefSpeed() const;
	//�������� ������������ �������� � ������
	virtual long GetFrames() const;
	//���������� ���������� ��������� 0..1
	virtual void SetBlend(float k);
	//�������� ���������� ��������� 0..1
	virtual float GetBlend();
	//�������� ���������������� ������ ��� ����� ��������
	virtual const char * GetData(const char * dataName) const;

//--------------------------------------------------------------------------------------------
//ActionPlayerImp
//--------------------------------------------------------------------------------------------
public:
	//������� ��� �� �������
	void Execute(long dltTime);
	//���������� ������� � ����� ������	
	void ResetPosition();
	//�������� ������� �����
	float GetCurrentFrame();
	//������� ������ �� ������������ ���������
	float & Blend();
	float & TimerBlend();
	//���������� ��������� ������� ������
	void CopyState(ActionPlayerImp & from);

public:
	//������� ���������� ���������
	float kBlendCurrent;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//������������ �������
	void MoveNormal(float dlt);
	void MoveReverse(float dlt);
	void MovePingpong(float dlt);
	void MoveRPingpong(float dlt);
	//��������� ������� � ���� ���� ������������
	void CheckEvents();
	//����� ������� ������������ �������
	void ResetEventsMask();
	//��������� ���������
	void SysStop();

private:
	//�������� ��� ����� ��������
	AnimationImp * ani;
	//������ ����� ������
	long playerIndex;
	//���������� � ������� ��������
	ActionInfo * action;
	//��� ������������ ��������
	AnimationType anitype;
	//������������
	bool isPlay, isPause;
	//��������
	bool isAutostop;
	//������� ������������
	float position;
	//����������� �������� true - ����������� ����� ��������
	bool dir;
	//�������� ������������
	float speed;
	//���������� �������� ������������
	float kspeed;
	//����������� ����������
	bool isLoop;
	//����� ��� ������������ �������
	dword eventsMask[ACTIONPLAYEREVENTFLAGS];
	//���������� ���������
	float kBlend;
	//����������� ���������� ��������� ��� �������
	float kBlendTimer;
};

//������� ������ �� ����������� ���������
inline float & ActionPlayerImp::Blend()
{
	return kBlend;
}

inline float & ActionPlayerImp::TimerBlend()
{
	return kBlendTimer;
}

#endif

