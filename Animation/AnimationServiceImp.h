//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationServiceImp
//--------------------------------------------------------------------------------------------
//	������ ��������, ����������� ��������� ������� AnimationManager
//============================================================================================

#ifndef _AnimationServiceImp_H_
#define _AnimationServiceImp_H_

#include "../Common_h/Animation.h"
#include "../Common_h/vmodule_api.h"
#include "AnimationInfo.h"

//============================================================================================

class AnimationImp;

class AnimationServiceImp : public AnimationService
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	AnimationServiceImp();
	virtual ~AnimationServiceImp();
	
	//����� ����������
	virtual uint32_t RunSection();
	//������� ����������
	virtual void RunStart();
	virtual void RunEnd();
	//������� �������� ��� ������, ������� ����� delete
	virtual Animation * CreateAnimation(const char * animationName);

//--------------------------------------------------------------------------------------------
//������� ��� Animation
//--------------------------------------------------------------------------------------------	
	//������� �������� (���������� �� �����������)
	void DeleteAnimation(AnimationImp * ani);
	//�������
	void Event(const char * eventName);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//��������� ��������
	long LoadAnimation(const char * animationName);
	//��������� �� ������� ������ ���������������� ������
	void LoadUserData(INIFILE * ani, const char * sectionName, std::unordered_map<std::string, std::string> & data, const char * animationName);
	//��������� AN
	bool LoadAN(const char * fname, AnimationInfo * info);

	
	AnimationInfo ** ainfo;
	long numInfos;

	AnimationImp ** animation;
	long numAnimations;

	static char key[1024];
};

//============================================================================================

#endif

