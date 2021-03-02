//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationInfo
//--------------------------------------------------------------------------------------------
//	����� �������� ���������� �� ��������, ����������� 1 ���
//============================================================================================

#ifndef _AnimationInfo_H_
#define _AnimationInfo_H_

#include "ActionInfo.h"
#include "Bone.h"
#include "UserData.h"


class AnimationInfo  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	AnimationInfo(const char * animationName);
	virtual ~AnimationInfo();
	//���������� ���������� ������ � ��������
	void SetNumFrames(long _numFrames);
	//������� �����
	void CreateBones(long numbones);
	//������� ��������
	ActionInfo * AddAction(const char * anctionName, long startframe, long endframe);
	//���������� �������� ����������
	void SetFPS(float _fps);

//--------------------------------------------------------------------------------------------
//������ � ���������
//--------------------------------------------------------------------------------------------
public:
	//���������� ������
	long NumBones();
	//����������� �� �����
	Bone & GetBone(long iBone);
	//�������� � ������� ������
	bool operator == (const char * animationName);
	//�������� ������� ������
	void AddRef();
	//�������� ������� ������
	void RelRef();
	//�������� ����� �������
	void AddDowntime(long dltTime);
	//�������� ����� �������
	long GetDowntime();
	//�������� ��� ��������
	const char * GetName();
	//����� �������� �� �����
	ActionInfo * FindAction(const char * actionName);
	//�������� ����� ��������
	long GetAniNumFrames();
	//������ � ���������������� ������
	UserData & GetUserData();
	//�������� �������� ���������� ��������
	float GetFPS();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	char name[64];			//�������� ��������
	
	long numFrames;			//���������� ������ �� ���� ��������
	float fps;				//���������� ������ � �������

	Bone * bone;			//����� � ������� ��������
	long numBones;			//���������� ������ � �������

	ActionInfo ** action;	//��������
	long numActions;		//���������� ��������

	long refCounter;		//������� ������ �� ��� ��������
	long downtime;			//����� �������

	UserData userData;		//���������������� ������
};

//============================================================================================
//inline
//============================================================================================

//���������� ���������� ������ � ��������
inline void AnimationInfo::SetNumFrames(long _numFrames)
{
	if(_numFrames < 0) _numFrames = 0;
	numFrames = _numFrames;
}

//���������� �������� ����������
inline void AnimationInfo::SetFPS(float _fps)
{
	if(fps < 0.0f) fps = 0.0f;
	if(fps > 1000000.0f) fps = 1000000.0f;
	fps = _fps;
}

//���������� ������
inline long AnimationInfo::NumBones()
{
	return numBones;
}

//����������� �� �����
inline Bone & AnimationInfo::GetBone(long iBone)
{
	Assert(iBone >= 0 && iBone < numBones);
	return bone[iBone];
}

//�������� ������� ������
inline void AnimationInfo::AddRef()
{
	refCounter++;
}

//�������� ������� ������
inline void AnimationInfo::RelRef()
{
	refCounter--;
	Assert(refCounter >= 0);
}

//�������� ����� �������
inline void AnimationInfo::AddDowntime(long dltTime)
{
	if(refCounter == 0)
	{
		if(dltTime <= 10) dltTime = 10;
		downtime += dltTime;
	}else downtime = 0;
	if(downtime < 0) downtime = 0;
}

//�������� ����� �������
inline long AnimationInfo::GetDowntime()
{
	return downtime;
}

//�������� ��� ��������
inline const char * AnimationInfo::GetName()
{
	return name;
}

//�������� ����� ��������
inline long AnimationInfo::GetAniNumFrames()
{
	return numFrames;
}

//������ � ���������������� ������
inline UserData & AnimationInfo::GetUserData()
{
	return userData;
}

//�������� �������� ���������� ��������
inline float AnimationInfo::GetFPS()
{
	return fps;
}

#endif

