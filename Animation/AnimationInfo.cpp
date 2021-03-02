//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationInfo
//--------------------------------------------------------------------------------------------
//	����� �������� ���������� �� ��������, ����������� 1 ���
//============================================================================================

#include "AnimationInfo.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

AnimationInfo::AnimationInfo(const char * animationName)
{
	Assert(strlen(animationName) < 64);
	strcpy(name, animationName);
	bone = null;
	numBones = 0;
	action = null;
	numActions = 0;
	refCounter = 0;
	downtime = 0;
	fps = 15.0f;
}

AnimationInfo::~AnimationInfo()
{
	if(bone) delete [] bone;
	if(action)
	{
		for(long i = 0; i < numActions; i++)
				if(action[i]) delete action[i];
		delete action;
	}
}

//������� �����
void AnimationInfo::CreateBones(long numbones)
{
	Assert(bone == null || numBones == 0);
	Assert(numbones > 0 && numbones <= 256);
	numBones = numbones;
	bone = NEW Bone[numBones];
}

//������� ��������
ActionInfo * AnimationInfo::AddAction(const char * anctionName, long startframe, long endframe)
{
	Assert(anctionName);
	//���� ����������
	for(long i = 0; i < numActions; i++)
		if(action[i][0] == anctionName) return null;
	//�� ��������� - ����� ��������
	numActions++;
	action = (ActionInfo **)RESIZE(action, numActions*4);	
	action[numActions - 1] = NEW ActionInfo(anctionName, startframe, endframe);
	return action[numActions - 1];
}

//--------------------------------------------------------------------------------------------
//������ � ���������
//--------------------------------------------------------------------------------------------

//�������� � ������� ������
bool AnimationInfo::operator == (const char * animationName)
{
	return stricmp(animationName, name) == 0;
}

//����� �������� �� �����
ActionInfo * AnimationInfo::FindAction(const char * actionName)
{
	for(long i = 0; i < numActions; i++)
		if(action[i][0] == actionName) return action[i];
	return null;
}

