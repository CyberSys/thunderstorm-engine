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

class AnimationPlayer; //������������� ��������
class AnimationTimer; //����� ����������� ���������� ��������
class Animation; //����� ����������� ���������� ����� ������
class AnimationEventListener; //����� ����������� ���������� ������� ��������
class AnimationService; //������ ��������


//============================================================================================
//���������
//============================================================================================

//��������� ������� ������������ �������� ��������
enum AnimationType {
  at_static,
  //�� ���������� �����
  at_normal,
  //�� ������ �� �����
  at_reverse,
  //�� ����� �� ������
  at_pingpong,
  //�� ������ �� �����, �������
  at_rpingpong,
  //�� ����� �� ������, �������
};

//������� ��������������� �� ����, ���������� ��������� ���������
enum AnimationEvent {
  //���������� ������� AnimationPlayer
  ae_setnewaction,
  //������ ���������� ����� ��������
  ae_play,
  //�������� ������������ ��������
  ae_pauseon,
  //��������������� ������������ ��������
  ae_pauseoff,
  //���������� ������������ ��������
  ae_stop,
  //���������� ������������ ��������
  ae_start,
  //��� ������ ��������
  ae_end,
  //��� �������� ��������
  ae_change,
  //��� ��������� ����������� ������������ (pingpong)
  //AnimationTimer
  ae_timerstop,
  //������ �������� ������ ��� ��� ����������


  //����������
  ae_numevents,
  //���������� �������
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

class ActionPlayer {
public:
  ~ActionPlayer() {
  }

  //���������� ������� ��������
  virtual bool SetAction(const char* actionName) = 0;
  virtual const char* GetAction() const = 0;
  //���������� �������������
  virtual bool Play() = 0;
  virtual void Pause() = 0;
  virtual void Stop() = 0;
  virtual bool IsPlaying() const = 0;
  virtual bool IsPause() const = 0;
  //������������� ��� ���������� ������ �������
  virtual bool SetAutoStop(bool isStop) = 0;
  virtual bool IsAutoStop() const = 0;
  //������� ������� ������������
  virtual float SetPosition(float position) = 0;
  virtual float GetPosition() const = 0;
  //��� ������������
  virtual void SetType(AnimationType atype) = 0;
  virtual AnimationType GetType() const = 0;
  //���������� �������� ������������
  virtual float SetSpeed(float kSpeed = 1.0f) = 0;
  virtual float GetSpeed() const = 0;
  virtual float GetDefSpeed() const = 0;
  //�������� ������������ �������� � ������
  virtual long GetFrames() const = 0;
  //���������� ���������� ��������� 0..1
  virtual void SetBlend(float k) = 0;
  //�������� ���������� ��������� 0..1
  virtual float GetBlend() = 0;
  //�������� ���������������� ������ ��� ����� ��������
  virtual const char* GetData(const char* dataName) const = 0;
};

//============================================================================================
//����� ����������� ���������� ��������
//============================================================================================
//���� ����� ��������� �������������� ����������
//��������� � ActionPlayer
//���������� �������� ���������� �� 0 �� 1,
//��������� �� 1 �� 0
//============================================================================================

class AnimationTimer {
public:
  virtual ~AnimationTimer() {
  }

  //��������� ������ (����� � ��������)
  virtual void Start(float time, float startTime = 0.0f) = 0;
  //���������� ������
  virtual void Stop() = 0;
  //�������� ��������� �������
  virtual void ResetTimer() = 0;
  //������, �������� �� ������
  virtual bool IsWork() = 0;
  //�������� ������� �������� 0..1
  virtual float GetCurrent() const = 0;
  //�������� �������� ����� ������ �������
  virtual float GetTime() const = 0;
  //��������� ActionPlayer ��� ��������� (isInverse == false -> kBlend = [0..1])
  virtual void SetPlayer(long playerIndex, bool isInverse = false) = 0;
  virtual void ResetPlayer(long playerIndex) = 0;
  //������, ������������ �� ActionPlayer
  virtual bool IsUsedPlayer(long playerIndex, bool* isInverse = nullptr) = 0;
  //�������� �������� ��������� ��� ������ (���� �� ������������ �� 1.0f)
  virtual float GetPlayerValue(long playerIndex) = 0;
};

//============================================================================================
//����� ����������� ���������� ����� ������
//============================================================================================
//���� ����� �������� ������� ActionPlayer � AnimationTimer � ���������� ANI_MAX_ACTIONS
//ActionPlayer � AnimationTimer ����� ������������ �����������, �� ������ ����������
//============================================================================================

class Animation {
public:
  virtual ~Animation() {
  };
  //����������� � ������������� ��������
  virtual ActionPlayer& Player(long index) = 0;
  //����������� � ������� ��������
  virtual AnimationTimer& Timer(long index) = 0;
  //�������
  //���������� ���������� ����������� �������
  virtual long SetEvent(AnimationEvent event, long index, AnimationEventListener* ael) = 0;
  //������� ���������� ����������� �������
  virtual void DelEvent(long eventID) = 0;
  //���������� ���������� �������� �������
  virtual void SetEventListener(AnimationEventListener* ael = nullptr) = 0;
  //������ � ������
  //�������� ���������� ������ � �������
  virtual long GetNumBones() const = 0;
  //�������� ������� �������� ��� �����
  virtual CMatrix& GetAnimationMatrix(long iBone) const = 0;
  //������
  //�������� ���������������� ������ ��� ��������
  virtual const char* GetData(const char* dataName) const = 0;
  //���������� ��������� ������ ������ � ������
  virtual void CopyPlayerState(long indexSrc, long indexDst, bool copyTimerState = false) = 0;
  //�������� �������� ���������� ��������
  virtual float GetFPS() = 0;
  //���������� ������ ���������
  //�������������� ������������ ������������ ���������
  virtual bool SetAutoNormalize(bool isNormalize = true) = 0;
  virtual bool GetAutoNormalize() = 0;
  //��������� ������������� ���������������� ������������ ��������� � ActionPlayer
  virtual bool UserBlend(bool isBlend = true) = 0;
  virtual bool IsUserBlend() = 0;
};

//============================================================================================
//����� ����������� ���������� ������� ��������
//============================================================================================

class AnimationEventListener {
public:
  virtual ~AnimationEventListener() {
  };
  //������� �������
  virtual void Event(Animation* animation, long index, long eventID, AnimationEvent event) {
  }

  //������� �������
  virtual void Event(Animation* animation, long playerIndex, const char* eventName) {
  }
};

//============================================================================================
//������ ��������
//============================================================================================

class AnimationService : public SERVICE {
public:
  virtual ~AnimationService() {
  }

  //������� �������� ��� ������, ������� ����� delete
  virtual Animation* CreateAnimation(const char* animationName) = 0;
};


//============================================================================================

#endif
