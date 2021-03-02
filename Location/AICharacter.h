//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	AICharacter
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _AICharacter_H_
#define _AICharacter_H_

#include "Character.h"

class AICharacter : public Character  
{
protected:
	enum AICommand
	{
		aicmd_unknow = 0,
		aicmd_none,				//��� �������, �������� �������������� �����
		aicmd_stay,				//������ �� �����
		aicmd_gotopoint,		//���� � �����
		aicmd_escape,			//������� �� �����
		aicmd_max
	};

	struct PathNode
	{
		CVECTOR pos;			//����� ��������
		CVECTOR nrm;			//����������� ����
		float dst;				//��������� �� ���������
		long node;				//���, �������� ������ ����������
	};

	struct Command
	{
		AICommand cmd;		//�������
		CVECTOR pnt;		//����� ��� �������
		long node;			//���, � ������� ����
		CVECTOR tpnt;		//����� � ������� ��� � ������ ������
		long tnode;			//���, �� ������� ���
		float radius;		//������ ��� ���������� ������
		float waitTime;		//����� �������� (��������)
		AICharacter * exch;	//� ���� ���������� ����������������
		//�����
		struct
		{
			dword cnt : 8;		//������� ����������
			dword isWait : 1;	//��������� � ��������
			dword isBusy : 1;	//��������� �� ��������� �����
		};
	};


//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	AICharacter();
	virtual ~AICharacter();

//--------------------------------------------------------------------------------------------
//Character
//--------------------------------------------------------------------------------------------
public:
	//���������� ��������� � �������� �������
	void Move(float dltTime);
	//�������� �������������� �������
	void Calculate(float dltTime);
	//�������� ������� ���������
	void Update(float dltTime);
	//�������� ����������� ���������
	void CharacterTeleport();

//--------------------------------------------------------------------------------------------
//AICharacter
//--------------------------------------------------------------------------------------------
public:
	//������ �� ������
	bool CmdNone();
	//������
	bool CmdStay();
	//���� � �����
	bool CmdGotoPoint(float x, float y, float z, float rad, long node = -1, bool isCheckBusyPos = true);
	//������� �� �����
	bool CmdEscape(float x, float y, float z, float rad);
	//���������� ��������� � ������� ����������������
	void SetExCharacter(AICharacter * chr);

	//�������

	//���������� ���������� ���������� �������
	virtual void FailureCommand(){};
	//�������� ������ � �����
	virtual void EndGotoCommand(){};
	//�������� �������� �� ����� �� ����������� ������
	virtual void EndEscapeCommand(){};
	//� ���������� ������� ����� �����������
	virtual void CollisionThreshold(){};

	static const char * GetCommandName(AICommand cmd);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
//private:
protected:
	//���� � �����
	void CmdProcessGotoPoint(float dltTime);
	void CmdUpdateGotoPoint(float dltTime);
	//������� �� �����
	void CmdProcessEscape(float dltTime);
	void CmdUpdateEscape(float dltTime);

	//����� ������ ���� ��� ������ ����������
	long FindNodeIndex(const CVECTOR & pos, float * hy = null);
	//����� ����������� ���� ���� (���������� �� ���������)
	bool FindDirectional();
	//����� �������������� ����
	void CalcRepulsionForces();
	//��������� ����� ���������� ������������ � ������� �� �����
	static bool FindIntersection(const CVECTOR & s, const CVECTOR & e, const CVECTOR & cur, const CVECTOR & to, CVECTOR & res);
	//��������� ���� �� ������� �����������
	static float Angle(double vx, double vz, float defAy);


protected:
	//������� �������
	Command command;
	PathNode path[2];
	//���������������� ���������� ��������
	float likeKSpd;
	float collisionValue;

	//����������� ����
	CVECTOR force;		//���� �������������
	CVECTOR goForce;	//���� ������������ ����������� �����������
	CVECTOR separation;	//���� �������������
	CVECTOR alignment;	//���� ������������ �����������
	CVECTOR around;		//���� ������ ����������

	bool	bMusketer;
	bool	bMusketerNoMove;

};

#endif

