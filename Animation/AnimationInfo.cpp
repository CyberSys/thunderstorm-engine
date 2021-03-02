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
#include "../Common_h/memop.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

AnimationInfo::AnimationInfo(const char * animationName)
{
	Assert(strlen(animationName) < 64);
	strcpy_s(name, animationName);
	bone = nullptr;
	numBones = 0;
	refCounter = 0;
	downtime = 0;
	fps = 15.0f;
}

AnimationInfo::~AnimationInfo()
{
	delete [] bone;
}

//������� �����
void AnimationInfo::CreateBones(long numbones)
{
	Assert(bone == nullptr || numBones == 0);
	Assert(numbones > 0 && numbones <= 256);
	numBones = numbones;
	bone = NEW Bone[numBones];
}

//������� ��������
ActionInfo * AnimationInfo::AddAction(const char * anctionName, long startframe, long endframe)
{
	Assert(anctionName);
	//���� ����������
	for (const auto &action : actions)
		if (action == anctionName)
			return nullptr;

	//�� ��������� - ����� ��������
	return &actions.emplace_back(anctionName, startframe, endframe);;
}

//--------------------------------------------------------------------------------------------
//������ � ���������
//--------------------------------------------------------------------------------------------

//�������� � ������� ������
bool AnimationInfo::operator == (const char * animationName)
{
	return _stricmp(animationName, name) == 0;
}

//����� �������� �� �����
ActionInfo * AnimationInfo::FindAction(const char * actionName)
{
	for (auto &action : actions)
		if (action == actionName)
			return &action;;
	return nullptr;
}

