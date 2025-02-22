//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	ActionInfo
//--------------------------------------------------------------------------------------------
//	���������� �� ����� ��������
//============================================================================================

#ifndef _ActionInfo_H_
#define _ActionInfo_H_

#include "Bone.h"
#include "Animation.h"
#include <unordered_map>
#include <string>

enum ExtAnimationEventType
{
	eae_always,	//������
	eae_normal,	//��� ������ ������������
	eae_reverse	//��� �������� ������������
};

class ActionPlayerImp;

class ActionInfo final  
{
	friend ActionPlayerImp;

	struct Event
	{
		char name[64];					//�������� �������
		float time;						//������������� ����� �������
		ExtAnimationEventType event;	//��� �������
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	ActionInfo(const char * aname, long startframe, long endframe);
	//���������� ���������� �������� ���������������
	void SetRate(float rate);
	//���������� ��� ��������
	void SetAnimationType(AnimationType t);
	//������������� ��������������� ��������
	void SetLoop(bool isloop);
	//�������� �������
	bool AddEvent(const char * ename, float frame, ExtAnimationEventType event);

//--------------------------------------------------------------------------------------------
//������ � ���������
//--------------------------------------------------------------------------------------------
public:
	//�������� � ������� ������
	bool operator == (const char * actionName) const;
	//�������� ��� ��������
	const char * GetName();
	//�������� ���������� �������
	long GetNumEvents();
	//�������� �� ������������ ������� ��������� �������, 
	//direction == true ��� ����������� �����������
	bool CheckEvent(long index, float time, bool direction);
	//�������� ��� ���������
	const char * EventName(long index);
	//�������� ������������ � ������
	long GetFrames();
	//������ � ���������������� ������
	std::unordered_map<std::string, std::string> & GetUserData();

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	char name[64];		//�������� ��������
	
	long startFrame;	//��������� ������� � ����� ��������
	long endFrame;		//�������� ������� � ����� ��������

	float kRate;		//���������� �������� ���������������
	AnimationType type;	//��� ��������
	bool isLoop;		//������������� ��������������� ��������

	uint32_t bonesMask[8];	//����� ������������ ������ � ��������

	Event event[ANI_MAX_EVENTS];	//�������
	long numEvents;					//���������� �������

	std::unordered_map<std::string, std::string> userData;	//���������������� ������
};

//============================================================================================
//inline
//============================================================================================

//���������� ���������� �������� ���������������
inline void ActionInfo::SetRate(float rate)
{
	if(rate < 0.0f) rate = 0.0f;
	if(rate > 10.0f) rate = 10.0f;
	kRate = rate;
}

//���������� ��� ��������
inline void ActionInfo::SetAnimationType(AnimationType t)
{
	type = t;
}

//������������� ��������������� ��������
inline void ActionInfo::SetLoop(bool isloop)
{
	isLoop = isloop;
}

//--------------------------------------------------------------------------------------------
//������ � ���������
//--------------------------------------------------------------------------------------------

//�������� ��� ��������
inline const char * ActionInfo::GetName()
{
	return name;
}

//�������� ���������� �������
inline long ActionInfo::GetNumEvents()
{
	return numEvents;
}

//�������� ������������ � ������
inline long ActionInfo::GetFrames()
{
	return endFrame - startFrame;
}

//������ � ���������������� ������
inline std::unordered_map<std::string, std::string> & ActionInfo::GetUserData()
{
	return userData;
}


#endif

