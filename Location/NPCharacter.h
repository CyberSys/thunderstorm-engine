//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	NPCaracter
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _NPCharacter_H_
#define _NPCharacter_H_

#include "AICharacter.h"

class NPCharacter : public AICharacter
{
protected:
	enum NPCTask
	{
		npct_unknow = 0,
		npct_none,				//��� ������, �������� �������������� �����
		npct_stay,				//������ �� �����
		npct_gotopoint,			//���� � �����
		npct_runtopoint,		//������ � �����
		npct_followcharacter,	//���� �� ���c������
		npct_fight,				//��������� � ������ ����������
		npct_escape,			//������� �� ���������
		npct_dead,				//������ ���������
		npct_max
	};

	struct Task
	{
		NPCTask task;
		CVECTOR to;
		ENTITY_ID target;
		union
		{
			dword flags;
			struct
			{
				dword isRun : 1;
				dword isFight : 1;
				dword isFollowInit : 1;
			};
		};
	};

	struct EnemyState
	{
		NPCharacter * chr;	//��������� �� �����
		float look;			//����������� ����� � ��� (cos)
		float dir;			//������������ ����� ������������ ��� (cos)
		float state;		//���������� ��������� �����
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	NPCharacter();
	virtual ~NPCharacter();

	virtual bool PostInit();

	dword ChlProcessMessage(long messageID, MESSAGE & message);
	void Move(float dltTime);
	void Update(float dltTime);

//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------
public:
	//�������� ���������� ���������
	Character * GetAttackedCharacter();

//--------------------------------------------------------------------------------------------
//������
//--------------------------------------------------------------------------------------------

	//���������� ����� ������
	bool SetNewTask(NPCTask tsk, MESSAGE & message);
	
	bool InitFollowChartacter(ENTITY_ID & eid);
	bool InitFightChartacter(ENTITY_ID & eid);


//--------------------------------------------------------------------------------------------
//���������� �����
//--------------------------------------------------------------------------------------------
protected:
	//���������� ������ ���������� �� ����������
	void UpdateFollowCharacter(float dltTime);
	//���������� ������ ��������
	void UpdateEscapeCharacter(float dltTime);
	//���������� ������ ���������� �� ����������
	void UpdateFightCharacter(float dltTime);

	//���
	//��������� � ���
	void DoFightAction(float dltTime, NPCharacter * enemy);
	//��������� � ��� - idle
	void DoFightActionAnalysisNone(float dltTime, NPCharacter * enemy);
	//��������� � ��� - attack
	void DoFightAttack(Character * enemy, long enemyCounter, bool wishDefence);
	//��������� � ��� - block, parry
	void DoFightBlock(bool needParry = false);

	//�������� �������
	float GetEnergy();
	//�������� ������� ��� ��������
	float GetActEnergy(const char * act);

	//�������

	//���������� ���������� ���������� �������
	virtual void FailureCommand();
	//�������� ������ � �����
	virtual void EndGotoCommand();
	//�������� �������� �� ����� �� ����������� ������
	virtual void EndEscapeCommand();
	//� ���������� ������� ����� �����������
	virtual void CollisionThreshold();
	
	virtual void HitChild(bool isInBlock);

	//��������� ������ � �����
	bool PushTask();
	//����������� ������ �� �����
	bool PopTask();


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//���������� ���������� ���������� �������
	void FailureCommand(NPCTask task);
	//�������� �������
	void FightTick();
	//�������� ��� ������ �� �����
	static NPCTask GetTaskID(const char * taskName);
	//�������� ��� ������ �� ����
	static const char * GetTaskName(NPCTask t);
	//��������� �������
	static bool PrTest(float probability, float & testTime);
	static bool PrTest(float probability);

protected:
	Task task;			//������, ������� ���� ���������
	NPCTask lastSetTask;//��������� ������������� ������
private:
	Task taskstack[16];	//���� �����
	long stackPointer;	//��������� �����

	//������ �����
	ENTITY_ID charactersGroups;

	//������� ���
	float fightLevel;	//������� ��������� � ��� 0..1
	
	//�����
	float attackCur;			//�������� ���������� ����������� �����, � ��������
	float attackPrbFast;		//����������� fgt_attack_fast
	float attackPrbForce;		//����������� fgt_attack_force
	float attackPrbRound;		//����������� fgt_attack_round
	float attackPrbBreak;		//����������� fgt_attack_break
	float attackPrbFeint;		//����������� fgt_attack_feint
	
	//������
	float defenceCur;			//�������� ���������� ����������� �����, � ��������
	float blockTime;			//����� �����
	float defencePrbBlock;		//����������� fgt_block
	float defencePrbParry;		//����������� fgt_parry
	bool isRecoilEnable;		//�������� �� ������

	//��������
	float fireCur;				//�������� ���������� ����������� ��������, � ��������
	bool isFireEnable;			//�������� �� �������

	float fightTick;			//����� �� ���������� ���� �������� �������
	bool wantToAttack;			//������� ���������
	bool wantToDefence;			//������� ���������
	bool wantToFire;			//������� ����������

	//������� ��������� ����������
	bool isFgtChanged;
	FightAction enemyFgtType;

	bool	bMusketer;
	float	fMusketerDistance;
	float	fMusketerTime, fMusketerFireTime, fMusketerCheckFireTime;
	bool	bMusketerNoMove;
	bool	bTryAnyTarget;

	void SetEscapeTask(Character * c);
};

//�������� ���������� ���������
inline Character * NPCharacter::GetAttackedCharacter()
{
	if(task.task != npct_fight) return null;
	return (Character *)api->GetEntityPointer(&task.target);
}

//��������� �������
inline bool NPCharacter::PrTest(float probability, float & testTime)
{
	if(testTime < 1.0f/5.0f) return false;
	testTime = rand()*(0.02f/RAND_MAX);
	if(probability <= 0.0f) return false;
	float r = rand()*(1.0f/RAND_MAX);
	return r < probability;
}

//��������� �������
inline bool NPCharacter::PrTest(float probability)
{
	float r = rand()*(1.0f/RAND_MAX);
	return r < probability;
}

#endif

