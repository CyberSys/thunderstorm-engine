//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//  Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	Animation
//--------------------------------------------------------------------------------------------
//	������������ ������ ��� ������ � ���������
//============================================================================================

#ifndef _Animation_H_
#define _Animation_H_

//============================================================================================

#include "service.h"
#include "Matrix.h"

//============================================================================================

class AnimationPlayer;			//������������� ��������
class AnimationTimer;			//����� ����������� ���������� ��������
class Animation;				//����� ����������� ���������� ����� ������
class AnimationEventListener;	//����� ����������� ���������� ������� ��������
class AnimationService;			//������ ��������


//============================================================================================
//���������
//============================================================================================

//��������� ������� ������������ �������� ��������
enum AnimationType
{
	at_static,					//�� ���������� �����
	at_normal,					//�� ������ �� �����
	at_reverse,					//�� ����� �� ������
	at_pingpong,				//�� ������ �� �����, �������
	at_rpingpong,				//�� ����� �� ������, �������
};

//������� ��������������� �� ����, ���������� ��������� ���������
enum AnimationEvent
{
	//���������� ������� AnimationPlayer
	ae_setnewaction,			//������ ���������� ����� ��������
	ae_play,					//�������� ������������ ��������
	ae_pauseon,					//��������������� ������������ ��������
	ae_pauseoff,				//���������� ������������ ��������
	ae_stop,					//���������� ������������ ��������
	ae_start,					//��� ������ ��������
	ae_end,						//��� �������� ��������
	ae_change,					//��� ��������� ����������� ������������ (pingpong)
	//AnimationTimer
	ae_timerstop,				//������ �������� ������ ��� ��� ����������


	//����������
	ae_numevents,				//���������� �������
};


#define ANI_MAX_ACTIONS	8		//���������� ActionPlayer � AnimationTimer ��� ����� ������
#define ANI_MAX_EVENTS	8		//���������� ������� �� ���� ��������


//============================================================================================
//����� ������������� ��������
//============================================================================================
//�������� ��� �������� �������� - �������� ������� �������� (������, ���, �����, ...)
//���������� ��� �������� ���������.
//
//Autostop �� ��������� ������� � ������ ���������� ��� ��������� ������ ��������
//============================================================================================

class ActionPlayer
{
public:
	~ActionPlayer(){};
	//���������� ������� ��������
	virtual bool SetAction(const char * actionName) = null;
	virtual const char * GetAction() const = null;
	//���������� �������������
	virtual bool Play() = null;
	virtual void Pause() = null;
	virtual void Stop() = null;
	virtual bool IsPlaying() const = null;
	virtual bool IsPause() const = null;
	//������������� ��� ���������� ������ �������
	virtual bool SetAutoStop(bool isStop) = null;
	virtual bool IsAutoStop() const = null;
	//������� ������� ������������
	virtual float SetPosition(float position) = null;
	virtual float GetPosition() const = null;
	//��� ������������
	virtual void SetType(AnimationType atype) = null;
	virtual AnimationType GetType() const = null;
	//���������� �������� ������������
	virtual float SetSpeed(float kSpeed = 1.0f) = null;
	virtual float GetSpeed() const = null;
	virtual float GetDefSpeed() const = null;
	//�������� ������������ �������� � ������
	virtual long GetFrames() const = null;
	//���������� ���������� ��������� 0..1
	virtual void SetBlend(float k) = null;
	//�������� ���������� ��������� 0..1
	virtual float GetBlend() = null;
	//�������� ���������������� ������ ��� ����� ��������
	virtual const char * GetData(const char * dataName) const = null;
};

//============================================================================================
//����� ����������� ���������� ��������
//============================================================================================
//���� ����� ��������� �������������� ����������
//��������� � ActionPlayer
//���������� �������� ���������� �� 0 �� 1,
//��������� �� 1 �� 0
//============================================================================================

class AnimationTimer
{
public:
	virtual ~AnimationTimer(){};
	//��������� ������ (����� � ��������)
	virtual void Start(float time, float startTime = 0.0f) = null;
	//���������� ������
	virtual void Stop() = null;
	//�������� ��������� �������
	virtual void ResetTimer() = null;
	//������, �������� �� ������
	virtual bool IsWork() = null;
	//�������� ������� �������� 0..1
	virtual float GetCurrent() const = null;
	//�������� �������� ����� ������ �������
	virtual float GetTime() const = null;
	//��������� ActionPlayer ��� ��������� (isInverse == false -> kBlend = [0..1])
	virtual void SetPlayer(long playerIndex, bool isInverse = false) = null;
	virtual void ResetPlayer(long playerIndex) = null;
	//������, ������������ �� ActionPlayer
	virtual bool IsUsedPlayer(long playerIndex, bool * isInverse = null) = null;
	//�������� �������� ��������� ��� ������ (���� �� ������������ �� 1.0f)
	virtual float GetPlayerValue(long playerIndex) = null;
};

//============================================================================================
//����� ����������� ���������� ����� ������
//============================================================================================
//���� ����� �������� ������� ActionPlayer � AnimationTimer � ���������� ANI_MAX_ACTIONS
//ActionPlayer � AnimationTimer ����� ������������ �����������, �� ������ ����������
//============================================================================================

class Animation
{
public:
	virtual ~Animation(){};
	//����������� � ������������� ��������
	virtual ActionPlayer & Player(long index) = null;
	//����������� � ������� ��������
	virtual AnimationTimer & Timer(long index) = null;
	//�������
	//���������� ���������� ����������� �������
	virtual long SetEvent(AnimationEvent event, long index, AnimationEventListener * ael) = null;
	//������� ���������� ����������� �������
	virtual void DelEvent(long eventID) = null;
	//���������� ���������� �������� �������
	virtual void SetEventListener(AnimationEventListener * ael = null) = null;
	//������ � ������
	//�������� ���������� ������ � �������
	virtual long GetNumBones() const = null;
	//�������� ������� �������� ��� �����
	virtual CMatrix & GetAnimationMatrix(long iBone) const = null;
	//������
	//�������� ���������������� ������ ��� ��������
	virtual const char * GetData(const char * dataName) const = null;
	//���������� ��������� ������ ������ � ������
	virtual void CopyPlayerState(long indexSrc, long indexDst, bool copyTimerState = false) = null;
	//�������� �������� ���������� ��������
	virtual float GetFPS() = null;
	//���������� ������ ���������
	//�������������� ������������ ������������ ���������
	virtual bool SetAutoNormalize(bool isNormalize = true) = null;
	virtual bool GetAutoNormalize() = null;
	//��������� ������������� ���������������� ������������ ��������� � ActionPlayer
	virtual bool UserBlend(bool isBlend = true) = null;
	virtual bool IsUserBlend() = null;
};

//============================================================================================
//����� ����������� ���������� ������� ��������
//============================================================================================

class AnimationEventListener
{
public:
	virtual ~AnimationEventListener(){};
	//������� �������
	virtual void Event(Animation * animation, long index, long eventID, AnimationEvent event){};
	//������� �������
	virtual void Event(Animation * animation, long playerIndex, const char * eventName){};
};

//============================================================================================
//������ ��������
//============================================================================================

class AnimationService : public SERVICE
{
public:
	virtual ~AnimationService(){};
	//������� �������� ��� ������, ������� ����� delete
	virtual Animation * CreateAnimation(const char * animationName) = null;

};


//============================================================================================

#endif
