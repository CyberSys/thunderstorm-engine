//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationImp
//--------------------------------------------------------------------------------------------
//	���������� ���������� Animation
//============================================================================================

#ifndef _AnimationManagerImp_H_
#define _AnimationManagerImp_H_

#include "..\COMMON_H\Animation.h"
#include "ActionPlayerImp.h"
#include "AnimationTimerImp.h"
#include "AnimationInfo.h"


#define ANIIMP_MAXLISTENERS	8

class AnimationServiceImp;

class AnimationImp : public Animation
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	AnimationImp(long id, AnimationInfo * animationInfo);
	virtual ~AnimationImp();

	//���������� ��������� �� ������ ��������
	static void SetAnimationService(AnimationServiceImp * animationService);

//--------------------------------------------------------------------------------------------
//Animation
//--------------------------------------------------------------------------------------------
public:
	//����������� � ������������� ��������
	virtual ActionPlayer & Player(long index);
	//����������� � ������� ��������
	virtual AnimationTimer & Timer(long index);
	//�������
	//���������� ���������� �������
	virtual long SetEvent(AnimationEvent event, long index, AnimationEventListener * ael);
	//������� ���������� �������
	virtual void DelEvent(long eventID);
	//���������� ���������� �������� �������
	virtual void SetEventListener(AnimationEventListener * ael);
	//������ � ������
	//�������� ���������� ������ � �������
	virtual long GetNumBones() const;
	//�������� ������� �������� ��� �����
	virtual CMatrix & GetAnimationMatrix(long iBone) const;
	//������
	//�������� ���������������� ������ ��� ��������
	virtual const char * GetData(const char * dataName) const;
	//���������� ��������� ������ ������ � ������
	virtual void CopyPlayerState(long indexSrc, long indexDst, bool copyTimerState = false);
	//�������� �������� ���������� ��������
	virtual float GetFPS();
	//���������� ������ ���������
	//�������������� ������������ ������������ ���������
	virtual bool SetAutoNormalize(bool isNormalize = true);
	virtual bool GetAutoNormalize();
	//��������� ������������� ���������������� ������������ ��������� � ActionPlayer
	virtual bool UserBlend(bool isBlend = true);
	virtual bool IsUserBlend();

//--------------------------------------------------------------------------------------------
//AnimationImp
//--------------------------------------------------------------------------------------------
public:
	//�������� thisID
	long GetThisID();
	//�������� ��������� �� AnimationInfo
	AnimationInfo * GetAnimationInfo();
	//����� �������� �� �����
	ActionInfo * GetActionInfo(const char * actionName);
	//������� ��� �� �������
	void Execute(long dltTime);
	//��������� ������� ��������
	void BuildAnimationMatrices();
	//�������� ��������� �� ����� ��������
	static AnimationServiceImp * GetAniService();
	//������� AnimationPlayer
	//������ ���������� ����� ��������
	void ApeSetnewaction(long index);
	//�������� ������������ ��������
	void ApePlay(long index);
	//��������������� ������������ ��������
	void ApePauseOn(long index);
	//���������� ������������ ��������
	void ApePauseOff(long index);
	//���������� ������������ ��������
	void ApeStop(long index);
	//C������ �������� �� ������ �����
	void ApeAlways(long index);
	//��� ������ ��������
	void ApeStart(long index);
	//��� �������� ��������
	void ApeEnd(long index);
	//��� ��������� ����������� ������������ (pingpong)
	void ApeChange(long index);
	//������ �������� ������ ��� ��� ����������
	void AteTimerstop(long index);

	//������� �������
	void AteExtern(long plIndex, const char * evt);

private:
	//��������� �������
	void SendEvent(AnimationEvent event, long index);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//����������� �������������
	long thisID;
	//��������� �� ��������
	AnimationInfo * aniInfo;
	//��������
	ActionPlayerImp action[ANI_MAX_ACTIONS];
	//�������
	AnimationTimerImp timer[ANI_MAX_ACTIONS];
	//����������������
	bool isAutoNormalize;
	//���������������� ��������
	bool isUserBlend;
	//������� �������
	CMatrix * matrix;
	//���������� �� ���������� �������
	AnimationEventListener * ae_listeners[ae_numevents][ANIIMP_MAXLISTENERS];
	//���������� �� ������� �������
	AnimationEventListener * ae_listenersExt;
	//��������� �� ������ ��������
	static AnimationServiceImp * aniService;
};

//============================================================================================
//inline
//============================================================================================

//���������� ��������� �� ������ ��������
inline void AnimationImp::SetAnimationService(AnimationServiceImp * animationService)
{
	Assert(aniService == null);
	aniService = animationService;
}

//--------------------------------------------------------------------------------------------
//AnimationImp
//--------------------------------------------------------------------------------------------

//�������� thisID
inline long AnimationImp::GetThisID()
{
	return thisID;
}

//�������� ��������� �� AnimationInfo
inline AnimationInfo * AnimationImp::GetAnimationInfo()
{
	return aniInfo;
}

//����� �������� �� �����
inline ActionInfo * AnimationImp::GetActionInfo(const char * actionName)
{
	return aniInfo->FindAction(actionName);
}

//�������� ��������� �� ����� ��������
inline AnimationServiceImp * AnimationImp::GetAniService()
{
	return aniService;
}

//������� AnimationPlayer
//������ ���������� ����� ��������
inline void AnimationImp::ApeSetnewaction(long index)
{
	SendEvent(ae_setnewaction, index);
}

//�������� ������������ ��������
inline void AnimationImp::ApePlay(long index)
{
	SendEvent(ae_play, index);
}

//��������������� ������������ ��������
inline void AnimationImp::ApePauseOn(long index)
{
	SendEvent(ae_pauseon, index);
}

//���������� ������������ ��������
inline void AnimationImp::ApePauseOff(long index)
{
	SendEvent(ae_pauseoff, index);
}

//���������� ������������ ��������
inline void AnimationImp::ApeStop(long index)
{
	SendEvent(ae_stop, index);
}

//��� ������ ��������
inline void AnimationImp::ApeStart(long index)
{
	SendEvent(ae_start, index);
}

//��� �������� ��������
inline void AnimationImp::ApeEnd(long index)
{
	SendEvent(ae_end, index);
}

//��� ��������� ����������� ������������ (pingpong)
inline void AnimationImp::ApeChange(long index)
{
	SendEvent(ae_change, index);
}

//������ �������� ������ ��� ��� ����������
inline void AnimationImp::AteTimerstop(long index)
{
	SendEvent(ae_timerstop, index);
}

//������� �������
inline void AnimationImp::AteExtern(long plIndex, const char * evt)
{
	if(ae_listenersExt) ae_listenersExt->Event(this, plIndex, evt);
}

#endif
