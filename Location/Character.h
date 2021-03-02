//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Character
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Character_H_
#define _Character_H_


#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"
#include "..\common_h\model.h"
#include "..\common_h\templates.h"
#include "PtcData.h"

class Location;
class MODEL;
class Animation;
class LocatorArray;
class Supervisor;
class VSoundService;


#define CHARACTER_ATTACK_DIST		2.4f	//������������ ��������� ������������ ����� (�)
#define CHARACTER_ATTACK_ANG		60.0f	//������������ ��� ������������ ����� (�������)
#define CHARACTER_BLOCK_ANG			50.0f	//������������ ��� ������������ ����� (�������)
#define CHARACTER_MAX_JMP_STEPS		50

class Character : public ENTITY  
{
	float hScale;
	float wScale;

	friend Supervisor;

	struct ActionCharacter
	{
		ActionCharacter();
		~ActionCharacter();
		void SetName(const char * _name);
		void ChangeName(const char * _name);

		char * name;	//�������� ��������
		float tblend;	//����� ��������� ������ ��������
	};

	//�������� �����������
	struct ActionMove : public ActionCharacter
	{
		ActionMove();
		float speed;	//�������� ����������� ��� ������� �������� �/���
		float turnspd;	//�������� �������� ���/���		
	};

	//�������� �����������
	struct ActionIdle : public ActionCharacter
	{
		ActionIdle();
		float p;		//����� ����������� ������ ����� ��������
	};

	//�������� ���
	
	//�����
	struct ActionFightAttack : ActionCharacter
	{
		float hitTime;	//����� ������������ �����
	};

	//�������� ������
	struct ActionDead : public ActionIdle
	{
		ActionDead();
		float ang;			//����������� �������
	};

	struct Blender
	{
		Blender();
		void Blend(float _old, float _new, float _time);
		bool Update(float dltTime);
		float Get();

		float old;
		float dlt;
		float kTime;
		float time;
	};

	struct Detector
	{
		Detector(LocatorArray * _la);
		void Check(float dltTime, Character * ch);
		void Exit(Character * ch);

		LocatorArray * la;
		long lastLocator;
		float timeInLocator;
		float lastEventTime;
	};

	friend Detector;

	class RTuner : public MODEL::RenderTuner
	{
		virtual void Set(MODEL * model, VDX8RENDER * rs);
		virtual void Restore(MODEL * model, VDX8RENDER * rs);
	public:
		RTuner();
		float alpha;
		float chrAlpha;
		float camAlpha;
		float selected;
		Character * character;
		bool isVisible;

		float GetAlpha();
	};


	class EventListener : public AnimationEventListener
	{
	public:
		Character * character;
		//������� �������
		virtual void Event(Animation * animation, long index, long eventID, AnimationEvent event);
		//������� �������
		virtual void Event(Animation * animation, long playerIndex, const char * eventName);
	};

	friend RTuner;
	friend EventListener;

	struct ObstacleZone
	{
		float x,z;
		float dw;
		bool use;
	};

protected:
	enum FightAction
	{
		fgt_none = 0,		//��� �������� ������� ��������
		fgt_attack_fast,	//������� ����
		fgt_attack_force,	//������� ����
		fgt_attack_round,	//�������� ����
		fgt_attack_break,	//����������� ����
		fgt_attack_feint,	//���� - ������������ �������� ����
		fgt_attack_feintc,	//����������� ����� � ������ ����������
		fgt_fire,			//������� �� ���������
		fgt_hit_attack,		//������� ��������� ����� �� ��������� �������� ��� � stall
		fgt_hit_feint,		//������� �� ����� �������� ��� � stall
		fgt_hit_parry,		//������� �� ����������� �������� ��� � stall
		fgt_hit_round,		//������� ������������ �������� ������
		fgt_hit_fire,		//������� �� �������� �������� ��� � stall
		fgt_block,			//������ ������
		fgt_blockhit,		//������ ������
		fgt_blockbreak,		//�������� �����
		fgt_parry,			//�����������, �������� �������� �������� ���������� � stall
		fgt_recoil,			//������ �����
		fgt_strafe_l,		//������ �����
		fgt_strafe_r,		//������ �����
		fgt_max,
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Character();
	virtual ~Character();

	//�������������
	bool Init();
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);
	//��������� ��������
	dword AttributeChanged(ATTRIBUTES * apnt);
	void SetSignModel();
	void SetSignTechnique();
	void ReadFightActions(ATTRIBUTES * at, ActionCharacter actions[4], long & counter);

	virtual bool PostInit(){ return true; };
	virtual dword ChlProcessMessage(long messageID, MESSAGE & message){ return 0; };

	void AlreadyDelete();

//--------------------------------------------------------------------------------------------
//Character model
//--------------------------------------------------------------------------------------------
public:
	//�������� �������� ���������
	MODEL * Model();
	
	//����������� �������� � ����� x, y, z
	bool Teleport(float x, float y, float z);
	//����������� �������� � ����� x, y, z � ��������� �� ay
	bool Teleport(float x, float y, float z, float ay);
	//����������� �������� � �������
	bool Teleport(const char * group, const char * locator);

	//���������� ������� ��� ��������
	void SetSavePosition();
	//������� ������� ��� ��������
	void DelSavePosition(bool isTeleport);
	//��������� ���������
	virtual void SetSaveData(ATTRIBUTES * sdata){};
	//����������� ���������
	virtual void GetSaveData(ATTRIBUTES * sdata){};
	void StopFightAnimation(); // boal

protected:
	virtual void CharacterTeleport(){};
	virtual void HitChild(bool isInBlock){};

//--------------------------------------------------------------------------------------------
//Character animation
//--------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------
//Character commands
//--------------------------------------------------------------------------------------------
public:
	//�����
	void StartMove(bool isBack = false);
	//������������
	void StopMove();
	//��������� ��������� �� �������
	void Turn(float dx, float dz);
	//��������� ��������� �� ����
	void Turn(float _ay);
	//������ �����������
	float GetAY();
	//���������� ����� ����
	void SetRunMode(bool _isRun = true);
	//��������� ��
	bool IsMove();
	//������ ����� ����
	bool IsRun();
	//����� ��
	bool IsSwim();
	//���������� ����� ���
	bool SetFightMode(bool _isFight, bool isPlayAni = true);
	//���������, ����� �� ���������� � ����� ���
	bool IsFightEnable();
	//������ ����� ���
	bool IsFight();
	//� ������ ������ ��� ����� ���� ��� �������� (�������)
	bool IsFireFindTarget();
	//� ��������
	bool IsDialog();
	//�������� ������ ���������
	float GetHeight();
	//�������� ������� ���������
	void GetPosition(CVECTOR & pos);
	//�������� ������� ���������
	void GetGrassPosition(CVECTOR & pos, CVECTOR & gpos);
	//���������� ����� �������� �� �����
	void SetGrassSound();
	//�������� ������������ ������ ���������
	float GetRadius();
	//���������
	void Attack(Character * enemy, FightAction type);
	//����
	void Block();
	//�����������
	void Parry();
	//������
	void Recoil();
	//���� �����
	void StrafeLeft();
	//���� ������
	void StrafeRight();
	//���������
	void Hit(FightAction type);
	//�������
	void Fire();
	//���������, ������� �� ��������
	bool IsGunLoad();
	//������
	void Dead();
	//����� � �������
	void EntryToLocation();
	//����� �� �������
	void ExitFromLocation();
	//����� ��� ����
	bool IsDead();
	//������
	void StartJump();

	//���� �� ������
	bool IsSetBlade();

	//���������� ������������ ���������
	void SetCameraAlpha(float alpha);
	//������������� ����� idle ��������
	void LockIdle(bool isLock);
	//�������� ���������
	void Select();

	//������������� ����������� ���������
	void LockMove(bool isLock);	
	//������������� �������� ��������� �� �������
	void LockRotate(bool isLock);
	//������������� �������� �������� ������
	float CameraTurnSpeed();
	//�������� �� ����
	void LookFromEyes(bool isLook);


	


//--------------------------------------------------------------------------------------------
//Character
//--------------------------------------------------------------------------------------------
public:
	//�������� ���������
	virtual void Reset();
	//���������� ��������� � �������� �������
	virtual void Move(float dltTime);
	//�������� �������������� �������
	virtual void Calculate(float dltTime){};
	//�������� ������� ���������
	virtual void Update(float dltTime);
	
	//������� ��������� ������������ �������� ��������
	virtual void ActionEvent(const char * actionName, Animation * animation, long index, long eventID, AnimationEvent event);
	virtual void ActionEvent(Animation * animation, long playerIndex, const char * eventName);

	//�������� �������, ���������������� ���� �������� ������
	//virtual void CharacterNowIsAttacked(){};

	long PlaySound(const char * soundName, bool isLoop = false, bool isCached = false);
	void PlayStep();
	void SetSoundPosition(long id);
	void ReleaseSound(long id);

	virtual bool IsPlayer() {return false;}

protected:
	//����� ��������� ��� �������
	Character * FindDialogCharacter();

	long startColCharacter;
	long numColCharacter;
	bool isSlide;
	CVECTOR slideDir;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	bool zLoadModel(MESSAGE & message);
	bool zTeleport(MESSAGE & message, bool isAy);
	bool zTeleportL(MESSAGE & message);
	bool zAddDetector(MESSAGE & message);
	bool zDelDetector(MESSAGE & message);
	bool zActionPlay(MESSAGE & message);
	bool zEntry(MESSAGE & message);
	bool zSetBlade(MESSAGE & message);
	bool zSetGun(MESSAGE & message);
	bool zTurnByLoc(MESSAGE & message);	
	bool zTurnByChr(MESSAGE & message);	
	bool zTurnByPoint(MESSAGE & message);
	bool zDistByCharacter(MESSAGE & message, bool is2D);
	dword zExMessage(MESSAGE & message);
	bool zPlaySound(MESSAGE & message);
	bool TestJump(CVECTOR pos);
	bool BuildJump(CVECTOR pos,float fAng);
	bool TraceWithObstacle(const CVECTOR& src,const CVECTOR& dst);
	void UpdateActionsData();
	void UpdateActionMoveData(ActionMove & am, Animation * a);
	void UpdateActionCharacterData(ActionCharacter & ac, Animation * a);
	void UpdateActionIdleData(ActionIdle & ai, Animation * a);
	void UpdateActionDeadData(ActionDead & ai, Animation * a);
	bool SetAction(const char * actionName, float tstart = 0.0f, float movespeed = 0.0f, float trnspeed = 0.0f, bool forceStart = false);
	void UpdateAnimation();
	const char * FindIdleAnimation(float & tstart);
	const char * FindFightIdleAnimation(float & tstart);
	float GetCurrentPlayerPos();
	float GetAniPlayTime();
	bool SetPriorityAction(const char * action);
	//��������� ������� ��������� �� ��������� ����
	void CheckAttackHit();
	//�������� ��������� ������
	void UpdateWeapons();
	//�������� ����������� �� ���������� ��� �������� ��� �����
	CVECTOR GetEnemyDirForImpulse();

	bool PriorityActionIsJump();

protected:
	//����� ��������� � �������� ������ �� ��������� kDist = 1..0
	Character * FindGunTarget(float & kDist, bool bOnlyEnemyTest = false);
	//����� ���������� "eeffffll"
	void FindNearCharacters(MESSAGE & message);
	//��������� ���������
	bool CharactersVisibleTest(MESSAGE & message);
public:
	//�������� ��������� ���� ����������
	bool VisibleTest(Character * chr);
	//�������� ��������
	static const char * GetValueByPrefix(const char * str, const char * pref);

protected:
	//������� � ������� ��������� ��������
	Location * location;
	//
	VSoundService * soundService;
	//������� ���������
	float radius, radiusNrm, radiusFgt;
	//������� � ���������� �������
	CVECTOR curPos, oldPos, colMove, grsPos, impulse;
	float strafeMove;
	float seaY;

	long currentNode;	//������� ���

	float speed;		//������� �������� �����������
	float turnspd;		//�������� ��������
	Blender bspeed;		//������� �������� ��������
	Blender bturn;		//������� �������� ��������
	float ay;			//����������� ���������	
	float nay;			//�������� �����������
	float turnDir;		//����������� �������� +1..0..-1
	float vy;			//�������� �������	
	float height;		//������ ���������

	float turnSpeed;	//�������� �������� � ��������

	//��������� ���������
	float chclass;		//����� ��������� 0..1, ������������ ���������
	float fatigue;		//��������� 0..1
	float kSpd;			//�������� ���������� ��������
	float kSpdCur;		//������� ���������� ��������
	float kSpdDlt;		//������� �� �������� � ������ ����������
	bool isMove;		//����� ��� ��������
	bool isBack;		//��� �����
	bool isRun;			//���������� �� ����� ����
	bool isRunDisable;	//�������� �� ����� ����
	bool isFight;		//����� ���
	bool lockFightMode;	//������������� ����� ���
	bool isSwim;		//�����
	bool isJump;		//����������
	bool isJumpSnd;		//���� ��������� ���� ������
	bool isEnableJump;	//��������� �� �����������
	bool isUp;			//����������� ��������� �����������
	bool isCollision;	//��� �������� � ������ ����������
	bool isTurnLock;	//������������ ���������� ���������
	bool lockMove;		//��������� �����������
	bool lockRotate;	//��������� ��������
	bool isDialog;		//�������� � �������
	bool isFightWOWps;	//��������� ���������� � ����� ��� ��� ������
	bool isActiveState;	//���� true �� �������� �������
	bool recoilLook;	//��� ������������ ��������� ����� �������
	float swimChange;	//����� ������� �� ��������� ������
	float isTurn;		//��������������	
	bool isNFHit;		//������ ��� ������ ���
	float movecs;		//������� ���� ������� ���������
	const char * deadName;//��� �������� ������
	long jumpSound;		//���� ������������� ������ ��� ������
	long recoilSound;	//���� ��������

	float noBlendTime;	

	bool isLookFromEyes;

	ActionMove * curMove;		//������� �������� �����������

	ActionMove walk;			//������ �����
	ActionMove backwalk;		//������ �����
	ActionMove run;				//��� �����
	ActionMove backrun;			//��� �����
	ActionMove stsUp;			//���� ����� �� ��������
	ActionMove stsDown;			//���� ���� �� ��������
	ActionMove stsUpBack;		//���� ����� �� �������� �����
	ActionMove stsDownBack;		//���� ���� �� �������� �����
	ActionMove stsUpRun;		//������ ����� �� ��������
	ActionMove stsDownRun;		//������ ���� �� ��������
	ActionMove stsUpRunBack;	//������ ����� �� �������� �����
	ActionMove stsDownRunBack;	//������ ���� �� �������� �����
	ActionMove swim;			//�����
	ActionMove fall;			//�����������
	ActionMove fall_land;		//�����������, ��������� �� �����
	ActionMove fall_water;		//�����������, �������� � ����
	ActionMove jump;			//��������
	
	
	
	ActionIdle nfhit;			//������� ��� ������ ���


	//Idle ��������
	long curIdleIndex;
	ActionIdle actionIdle[32];
	long numActionIdles;
	ActionIdle actionFightIdle[32];
	long numFightActionIdles;

	//������
	ActionDead actionDead[8];
	long numActionDead;
	ActionDead actionFightDead[8];
	long numActionFightDead;

	ActionCharacter actionTurnL;//�������� �������� �����
	ActionCharacter actionTurnR;//�������� �������� ������
	ActionCharacter userIdle;	//�������� ������������� �� ���

	//���
	//������� �����������
	ActionMove fightwalk;		//������ ����� � ������ ���
	ActionMove fightbackwalk;	//������ ����� � ������ ���
	ActionMove fightrun;		//��� ����� � ������ ���
	ActionMove fightbackrun;	//��� ����� � ������ ���
	//�����
	ActionCharacter attackFast[4];	//������� �����
	long numAttackFast;				//���������� ������� ����
	ActionCharacter attackForce[4];	//������� �����
	long numAttackForce;			//���������� ������� ����
	ActionCharacter attackRound[4];	//�������� �����
	long numAttackRound;			//���������� �������� ����
	ActionCharacter attackBreak[4];	//����������� �����
	long numAttackBreak;			//���������� ����������� ����
	ActionCharacter attackFeint[4];	//����
	ActionCharacter attackFeintC[4];//��������� ����������� �����
	long numAttackFeint;			//���������� ������
	ActionCharacter parry[4];	    //�����������  eddy
	long numParry;					//���������� �����������
	//������������� �����
	ActionCharacter shot;			//������� �� ���������
	//�������
	ActionCharacter hit[4];			//��������� �� ���������
	long numHits;					//���������� ��������� ���������
	ActionCharacter hitFeint;		//������� �� ����
	ActionCharacter hitParry;		//������� �� �����������
	ActionCharacter hitRound;		//������� ������������ �������� ������
	ActionCharacter hitFire;		//������� �� �������
	//������
	ActionCharacter block;			//����
	ActionCharacter blockaxe;		//���� �������
	ActionCharacter blockhit;		//��������� �� ��������� � �����
	ActionCharacter blockaxehit;	//��������� �� ��������� � ����� � �������
	ActionCharacter blockbreak;		//���������� �����
	ActionCharacter recoil;			//������ �����
	ActionCharacter strafe_l;		//������ �����
	ActionCharacter strafe_r;		//������ ������
	//���������� ���������
	FightAction fgtCurType;			//��� �������� ��������	
	long fgtCurIndex;				//������ �������� ��������
	FightAction fgtSetType;			//��� �������������� ��������
	long fgtSetIndex;				//������ �������������� ��������
	bool isParryState;				//��������� �����������
	bool isFeintState;				//��������� �����������
	bool isFired;					//��������� �� �������
	bool isLockIdleForCamera;		//
	float recoilWait;				//�������� ����� ������
	float strafeWait;				//�������� ����� �������
	float camRotWait;				//������� ���������� �������� ��������
	float camRotMax;				//������������ ����� ��������
	float strafeAngle;				//���� �������
	float strafeVel;				//������ �� �����
	ENTITY_ID enemyAttack;			//�� ���� ������������� �� ����� �����

	//������� ����������� ����� ������ �������� ������
	static byte fightTbl[fgt_max][fgt_max];
	static char * fightNamesTbl[fgt_max];

	//��������, ������� ���������� �����������
	ActionCharacter priorityAction;
	float priorityActionMoveSpd;
	float priorityActionRotSpd;
	bool isResetAutoAction;
	bool isSetPriorityAction;

	//���������
	Detector * detector[64];
	long numDetectors;


	//�������� ���������
	ENTITY_ID mdl;
	//����
	ENTITY_ID shadow;
	//�����
	ENTITY_ID blade;
	bool isBladeSet;
	bool isGunSet;
	//����
	ENTITY_ID sea;

	ENTITY_ID effects;
	//�������� ������������ �����
	ENTITY_ID sign;
	string signName;
	string signTechniqueName;


	//����� �� ����
	ENTITY_ID waterrings;
	float stepsRate;

	//���� ����������, �� �� ��������� �� supervisor
	bool isDeleted;

	//��� ���������
	bool isMale;
	//���� ����
	bool soundStep;
	//���� �������� � �����
	bool soundGrass;

	//������������ ��������� � ��������
	RTuner tuner;
	float liveValue;	//>0 �����������, <0 ��������, 0 �������������

	//������� ��������
	EventListener eventListener;

	float ayStack[16];
	long ayStackPointer;

	CVECTOR jumpTrack[50];
	CVECTOR osculationPoint;
	float jumpFallTime;
	float curJumpFallTime;
	long jumpPoints;

	char * characterID;	//ID ���������

	//
	long m_nHandLightID;
	char* m_pcHandLightLocator;
	CVECTOR GetHandLightPos();

	bool CheckObstacle(float fx,float fz, float fzlen);
	long GetRandomIndexByObstacle(ObstacleZone* pZone, long num);

public:	

	bool isPlayerEnemy;
	float enemyBarsAlpha;

	//���������� ��� �����
	struct GrpTarget
	{
		ENTITY_ID chr;		//������������� ����
		float time;			//����� ������ ���� �� ����
		float timemax;		//������������ ����� ��������� ����
	};

	GrpTarget grpTargets[32];	//������ �����
	long numTargets;			//���������� �����
	long groupID;				//������ ������ ��� ��������� ������
	char group[128];			//��� ������� ������
};

inline void Character::AlreadyDelete()
{
	isDeleted = true;
}

//������ �����������
inline float Character::GetAY()
{
	return ay;
}

//������ ����� ����
inline bool Character::IsRun()
{
	return isRun && !isRunDisable;
}

//����� ��
inline bool Character::IsSwim()
{
	return isSwim;
}

//������ ����� ���
inline bool Character::IsFight()
{
	return isFight;
}

//� ��������
inline bool Character::IsDialog()
{
	return isDialog;
}

//�������� ������ ���������
inline float Character::GetHeight()
{
	return height;
}

//�������� ������� ���������
inline void Character::GetPosition(CVECTOR & pos)
{
	pos = curPos;
	if(isSwim) pos.y = seaY;
}

//�������� ������� ���������
inline void Character::GetGrassPosition(CVECTOR & pos, CVECTOR & gpos)
{
	pos = curPos;
	gpos = grsPos;
}

//���������� ����� �������� �� �����
inline void Character::SetGrassSound()
{
	soundGrass = true;
}

//�������� ������������ ������ ���������
inline float Character::GetRadius()
{
	return radius;
}

//���������� ������������ ���������
inline void Character::SetCameraAlpha(float alpha)
{
	if(alpha < 0.0f) alpha = 0.0f;
	if(alpha > 1.0f) alpha = 1.0f;
	tuner.camAlpha = alpha;
}

//��������� ��
inline bool Character::IsMove()
{
	return isMove;
}

//������������� ����� idle ��������
inline void Character::LockIdle(bool isLock)
{
	isLockIdleForCamera = isLock;
}

//�������� ���������
inline void Character::Select()
{
	tuner.selected = 1.0f;
}

//������������� ����������� ���������
inline void Character::LockMove(bool isLock)
{
	lockMove = isLock;
}

//������������� �������� ��������� �� �������
inline void Character::LockRotate(bool isLock)
{
	lockRotate = isLock;
}

//������������� �������� �������� ������
inline float Character::CameraTurnSpeed()
{
	if(camRotWait <= 0.0f || camRotMax <= 0.0f) return 1.0f;
	float k = camRotWait/camRotMax;
	if(k > 1.0f) k = 1.0f;
	return powf(1.0f - k, 1.8f);
}

//�������� �� ����
inline void Character::LookFromEyes(bool isLook)
{
	isLookFromEyes = isLook;
}

//����� ��� ����
inline bool Character::IsDead()
{
	return (liveValue < 0 || deadName);
}

//���� �� ������
inline bool Character::IsSetBlade()
{
	return isBladeSet | isFightWOWps;
}

inline bool Character::PriorityActionIsJump()
{
	return (priorityAction.name && (stricmp(priorityAction.name,jump.name)==0 || stricmp(priorityAction.name,fall.name)==0));
}

#endif
