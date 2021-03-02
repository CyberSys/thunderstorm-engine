//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Character
//--------------------------------------------------------------------------------------------
//	
//============================================================================================


#include "Character.h"
#include "Location.h"
#include "Lights.h"
#include "CharactersGroups.h"


#include "..\common_h\geometry.h"
#include "..\common_h\sea_base.h"
#include "..\SoundService\VSoundService.h"

//============================================================================================


#define CHARACTER_WAIT_AFTER_DEAD	6.0f	//



#define CHARACTER_COS_STAIR			0.9f	//


#define CHARACTER_NORM_TO_FIGHT		"Normal to fight"
#define CHARACTER_FIGHT_TO_NORM		"Fight to normal"
#define CHARACTER_FIGHT_GUNBELT		"To belt"
#define CHARACTER_FIGHT_GUNHAND		"To hand"
#define CHARACTER_FIGHT_GUNFIRE		"Fire"

#define CHARACTER_FIGHT_FIREDIST	25.0f	//������
#define CHARACTER_FIGHT_FIREANG		20.0f	//��������


#define CHARACTER_DIALOG_ANG		30.0f	//������������ ��� ������������ �������

#define CHARACTER_SEA_NOTRUN		0.2f	//������������� ������ ��� ������� �������� �� ����� ������
#define CHARACTER_SEA_MOVE			0.6f	//������������� ������ ��� ������� �������� ������� ���������
#define CHARACTER_SEA_SWIM			0.8f	//������������� ������ ��� ������� �������� �������� �����


#define CHARACTER_HIDE_DIST			0.5f	//��������� �� ������ ��� ������� ������ ���������


#define CHARACTER_JUMPMAXTIME		5.0f	//����������� ���������� ����� ������� ��� ������
#define CHARACTER_MAXJUMPPOINTS		(sizeof(jumpTrack)/sizeof(CVECTOR))
#define CHARACTER_JUMP_TIMESTEP		(CHARACTER_JUMPMAXTIME/CHARACTER_MAXJUMPPOINTS)

//���� ����
#define FGT_ATTACK_FAST		"fast"
#define FGT_ATTACK_FORCE	"force"
#define FGT_ATTACK_ROUND	"round"
#define FGT_ATTACK_BREAK	"break"
#define FGT_ATTACK_FEINT	"feint"
#define FGT_ATTACK_PARRY	"parry"  // ��� ������ "hit_parry"  boal

//������� ����������� ����� ������ �������� ������
byte Character::fightTbl[fgt_max][fgt_max] =
{
	//Set type
	//nn afs afr ard abk aft aftc fr hatk hft hpr hrd hfr bk bkh bbr pry rcl stl str  //Current type
	{ 1, 1,  1,  1,  1,  1,  1,   1, 1,   1,  1,  1,  1,  1, 1,  1,  1,  1,  1,  1 }, //fgt_none ��� �������� ������� ��������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_fast ������� ����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_force ������� ����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_round �������� ����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_break ����������� ����
	{ 0, 0,  0,  0,  0,  0,  1,   0, 1,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_feint ���� - ������������ �������� ����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_attack_feintc ����������� ����� � ������ ����������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_fire ������� �� ���������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_hit_attack ������� ��������� ����� �� ��������� �������� ��� � stall
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_hit_feint ������� �� ����� �������� ��� � stall
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_hit_parry ������� �� ����������� �������� ��� � stall
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_hit_round ������� ������������ �������� ������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_hit_fire ������� �� �������� �������� ��� � stall
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 1,  1,  0,  0,  0,  0 }, //fgt_block ������ ������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 1,  1,  0,  0,  0,  0 }, //fgt_blockhit ������ ������
	{ 0, 0,  0,  0,  0,  0,  0,   0, 0,   0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0 }, //fgt_blockbreak �������� �����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   1,  1,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_parry �����������, �������� �������� �������� ���������� � stall
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   0,  0,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_recoil ������ �����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   0,  0,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_strafe_l ������ �����
	{ 0, 0,  0,  0,  0,  0,  0,   0, 1,   0,  0,  1,  1,  0, 0,  0,  0,  0,  0,  0 }, //fgt_strafe_r ������ �����
};

char * Character::fightNamesTbl[fgt_max] = 
{
	"fgt_none",				//��� �������� ������� ��������
	"fgt_attack_fast",		//������� ����
	"fgt_attack_force",		//������� ����
	"fgt_attack_round",		//�������� ����
	"fgt_attack_break",		//����������� ����
	"fgt_attack_feint",		//���� - ������������ �������� ����
	"fgt_attack_feintc",	//����������� ����� � ������ ����������
	"fgt_fire",				//������� �� ���������
	"fgt_hit_attack",		//������� ��������� ����� �� ��������� �������� ��� � stall
	"fgt_hit_feint",		//������� �� ����� �������� ��� � stall
	"fgt_hit_parry",		//������� �� ����������� �������� ��� � stall
	"fgt_hit_round",		//������� ������������ �������� ������
	"fgt_hit_fire",			//������� �� �������� �������� ��� � stall
	"fgt_block",			//������ ������
	"fgt_blockhit",			//������ ������
	"fgt_blockbreak",		//�������� �����
	"fgt_parry",			//�����������, �������� �������� �������� ���������� � stall
	"fgt_recoil",			//������ �����
	"fgt_strafe_l",			//������ �����
	"fgt_strafe_r",			//������ �����
};


//============================================================================================

Character::ActionCharacter::ActionCharacter()
{
	name = null;
	tblend = 0.0f;
}

void Character::ActionCharacter::SetName(const char * _name)
{
	if(name) delete name;
	name = null;
	if(_name && _name[0])
	{
		long l = strlen(_name) + 1;
		name = NEW char[l];
		memcpy(name, _name, l);
	}
}

void Character::ActionCharacter::ChangeName(const char * _name)
{
	if(!_name) return;
	SetName(_name);
}


Character::ActionCharacter::~ActionCharacter()
{
	if(name) delete name;
	name = null;
}

Character::ActionMove::ActionMove()
{
	speed = 0.0f;
	turnspd = 0.0f;
}

Character::ActionIdle::ActionIdle()
{
	p = 1.0f;
}

Character::ActionDead::ActionDead()
{
	ang = 0.0f;
}

Character::Blender::Blender()
{
	old = 0.0f;
	dlt = 0.0f;
	kTime = 0.0f;
	time = 1.0f;
}

void Character::Blender::Blend(float _old, float _new, float _time)
{
	old = _old;
	dlt = _new - _old;
	if(_time > 0.0f)
	{
		kTime = 1.0f/_time;
		time = 0.0f;
	}else{
		kTime = 1.0f;
		time = 1.0f;
	}
}

bool Character::Blender::Update(float dltTime)
{
	if(kTime <= 0.0f) return false;
	time += kTime*dltTime;
	if(time > 1.0f)
	{
		time = 1.0f;
		kTime = 0.0f;
	}
	return true;
}

float Character::Blender::Get()
{
	return old + powf(time, 0.1f)*dlt;
}

Character::Detector::Detector(LocatorArray * _la)
{
	la = _la;
	lastLocator = -1;
	timeInLocator = 0.0f;
	lastEventTime = 0.0f;
}

void Character::Detector::Check(float dltTime, Character * ch)
{
	float dist = 0.0f;
	long lIndex = la->FindNearesLocatorCl(ch->curPos.x, ch->curPos.y, ch->curPos.z, 1.0f, dist);
	if(lIndex >= 0)
	{
		if(lastLocator >= 0)
		{
			timeInLocator += dltTime;
			if(lastLocator == lIndex)
			{
				if(lastEventTime > 1.0f)
				{
					lastEventTime = 0.0f;
					_CORE_API->Event("Location_CharacterInLocator", "iissff", ch->location->GetID(), ch->GetID(), la->GetGroupName(), la->Name(lastLocator), dist, timeInLocator);
				}else lastEventTime += dltTime;
				return;
			}else{
				_CORE_API->Event("Location_CharacterExitFromLocator", "iissff", ch->location->GetID(), ch->GetID(), la->GetGroupName(), la->Name(lastLocator), dist, timeInLocator);
			}
		}
		timeInLocator = 0.0f;
		lastEventTime = 0.0f;
		lastLocator = lIndex;
		_CORE_API->Event("Location_CharacterEntryToLocator", "iissf", ch->location->GetID(), ch->GetID(), la->GetGroupName(), la->Name(lastLocator), dist);
	}else{
		if(lastLocator >= 0)
		{
			_CORE_API->Event("Location_CharacterExitFromLocator", "iissf", ch->location->GetID(), ch->GetID(), la->GetGroupName(), la->Name(lastLocator), timeInLocator + dltTime);
			lastLocator = -1;
		}
	}
}

void Character::Detector::Exit(Character * ch)
{
	if(lastLocator >= 0)
	{
		_CORE_API->Event("Location_CharacterExitFromLocator", "iissf", ch->location->GetID(), ch->GetID(), la->GetGroupName(), la->Name(lastLocator), timeInLocator);
	}
	timeInLocator = 0.0f;
	lastEventTime = 0.0f;
	lastLocator = -1;
}

Character::RTuner::RTuner()
{
	alpha = 1.0f;
	chrAlpha = 1.0f;
	camAlpha = 1.0f;
	selected = 0.0f;
	character = null;
	isVisible = true;
}

void Character::RTuner::Set(MODEL * model, VDX8RENDER * rs)
{
	NODE * n = model->GetNode(0);
	if(!n) return;
	if(n->GetTechnique()[0]) return;
	if(alpha < 0.0f) alpha = 0.0f;
	if(alpha > 1.0f) alpha = 1.0f;
	if(chrAlpha < 0.0f) chrAlpha = 0.0f;
	if(chrAlpha > 1.0f) chrAlpha = 1.0f;
	if(camAlpha < 0.0f) camAlpha = 0.0f;
	if(camAlpha > 1.0f) camAlpha = 1.0f;
	float a = camAlpha*chrAlpha*alpha;
	if(!isVisible) a = 0.0f;
	if(a < 0.5f) rs->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	if(a > 1.0f) a = 1.0f;
	if(selected < 0.0f) selected = 0.0f;
	if(selected > 1.0f) selected = 1.0f;
	long r = long(0x40*selected);
	long g = long(0x10*selected);
	long b = long(0x10*selected);
	rs->SetRenderState(D3DRS_TEXTUREFACTOR, (dword(a*255.0f) << 24) | (r << 16) | (g << 8) | b);
	if(selected > 0.0f)
	{
		if(a >= 1.0f)
		{
			n->SetTechnique("AnimationSelected");
		}else{
			n->SetTechnique("AnimationSelectedBlend");
		}
	}else{
		if(a >= 1.0f)
		{
			n->SetTechnique("Animation");
		}else{
			n->SetTechnique("AnimationBlend");
		}
	}
	Lights * ls = character->location->GetLights();
	if(ls) ls->SetCharacterLights(character->curPos);
}

void Character::RTuner::Restore(MODEL * model, VDX8RENDER * rs)
{
	Lights * ls = character->location->GetLights();
	if(ls) ls->DelCharacterLights();
	NODE * n = model->GetNode(0);
	if(!n) return;
	const char * chr = n->GetTechnique();
	if(*((dword *)chr) != 'minA' || *((dword *)(chr + 4)) != 'oita') return;
	n->SetTechnique("");
	rs->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);	
}

float Character::RTuner::GetAlpha()
{
	float a = camAlpha*chrAlpha*alpha;
	if(a > 1.0f) a = 1.0f;
	if(a < 0.0f) a = 0.0f;
	return a;
}

//������� �������
void Character::EventListener::Event(Animation * animation, long index, long eventID, AnimationEvent event)
{
	if(!animation || index != 0) return;
	const char * c = animation->Player(0).GetAction();
	if(!c) return;
	character->ActionEvent(c, animation, index, eventID, event);
}

//������� �������
void Character::EventListener::Event(Animation * animation, long playerIndex, const char * eventName)
{
	if(!animation || !eventName || playerIndex != 0) return;
	character->ActionEvent(animation, playerIndex, eventName);
}

//============================================================================================
//���������������, ������������������
//============================================================================================

Character::Character()
{
	isPlayerEnemy = false;
	enemyBarsAlpha = 0.0f;
	isLookFromEyes = false;
	soundService = null;
	currentNode = -1;
	tuner.character = this;
	location = null;
	//������� ���������
	radiusNrm = 0.7f;
	radiusFgt = 1.5f;
	radius = radiusNrm;
	isDeleted = false;
	curPos = oldPos = grsPos = 0.0f;
	vy = 0.0f;
	ay = 0.0f;
	nay = 0.0f;
	turnDir = 0.0f;
	isTurnLock = false;
	height = 1.8f;
	//��������
	turnSpeed = 1.0f;
	//������ �����
	walk.SetName("walk");
	//������ �����
	backwalk.SetName("back walk");
	//��� �����
	run.SetName("run");
	//��� �����
	backrun.SetName("back run");
	//���� ����� �� ��������
	stsUp.SetName("stairs up");
	//���� ���� �� ��������
	stsDown.SetName("stairs down");
	//���� ����� �� �������� �����
	stsUpBack.SetName("back stairs up");
	//���� ���� �� �������� �����
	stsDownBack.SetName("back stairs down");
	//������ ����� �� ��������
	stsUpRun.SetName("run stairs up");
	//������ ���� �� ��������
	stsDownRun.SetName("run stairs down");
	//������ ����� �� �������� �����
	stsUpRunBack.SetName("back run stairs up");
	//������ ���� �� �������� �����
	stsDownRunBack.SetName("back run stairs down");
	//�����
	swim.SetName("swim");
	//����������
	jump.SetName("jump");
	fall.SetName("fall");
	fall_land.SetName("fall_land");
	fall_water.SetName("fall_water");
	//������� ��� ������ ���
	nfhit.SetName("HitNoFight");
	//�������� �������� �����
	actionTurnL.SetName("turn left");
	//�������� �������� ������
	actionTurnR.SetName("turn right");
	//�������� �������
	actionIdle[0].SetName("idle_1");
	actionIdle[1].SetName("idle_2");
	actionIdle[2].SetName("idle_3");
	actionIdle[3].SetName("idle_4");
	actionIdle[4].SetName("idle_5");
	actionIdle[5].SetName("idle_6");
	actionIdle[6].SetName("idle_7");
	actionIdle[7].SetName("idle_8");
	actionIdle[8].SetName("idle_9");
	actionIdle[9].SetName("idle_10");
	actionIdle[10].SetName("idle_11");
	numActionIdles = 11;
	//����� ���
	//�������� ������� � ������ ���
	actionFightIdle[0].SetName("fight stand_1");
	actionFightIdle[1].SetName("fight stand_2");
	actionFightIdle[2].SetName("fight stand_3");
	actionFightIdle[3].SetName("fight stand_4");
	actionFightIdle[4].SetName("fight stand_5");
	actionFightIdle[5].SetName("fight stand_6");
	actionFightIdle[6].SetName("fight stand_7");
	//actionFightIdle[7].SetName("fight stand_8");
	numFightActionIdles = 7;
	//������ ����� � ������ ���
	fightwalk.SetName("fight walk");
	//������ ����� � ������ ���
	fightbackwalk.SetName("fight back walk");
	//��� ����� � ������ ���
	fightrun.SetName("fight run");
	//��� ����� � ������ ���
	fightbackrun.SetName("fight back run");
	//������
	actionDead[0].SetName("death_citizen_1");
	actionDead[1].SetName("death_citizen_2");
	numActionDead = 2;
	actionFightDead[0].SetName("death_2");
	actionFightDead[1].SetName("death_2");
	actionFightDead[2].SetName("death_2");
	actionFightDead[3].SetName("death_2");
	numActionFightDead =4;
	//����� ����� ����� �����������
	char buf[64];
	for(long i = 0; i < 4; i++)
	{
		_snprintf(buf, sizeof(buf) - 1, "attack_fast_%i", i + 1);
		attackFast[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "attack_force_%i", i + 1);
		attackForce[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "attack_round_%i", i + 1);
		attackRound[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "attack_break_%i", i + 1);
		attackBreak[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "attack_feint_%i", i + 1);
		attackFeint[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "attack_feintc_%i", i + 1);
		attackFeintC[i].SetName(buf);		
		_snprintf(buf, sizeof(buf) - 1, "hit_attack_%i", i + 1);
		hit[i].SetName(buf);
		_snprintf(buf, sizeof(buf) - 1, "parry_%i", i + 1);
		parry[i].SetName(buf);
	}
	numAttackFast = 3;
	numAttackForce = 4;
	numAttackRound = 2;
	numAttackBreak = 4;
	numAttackFeint = 4;
	numParry = 4;
	shot.SetName("Shot");
	numHits = 3;
	hitFeint.SetName("hit_feint");
	hitParry.SetName("hit_parry");
	hitRound.SetName("hit_round");
	hitFire.SetName("hit_fire");
	block.SetName("block");
	blockaxe.SetName("block_axe");
	blockhit.SetName("block_hit");
	blockaxehit.SetName("block_axe_hit");
	blockbreak.SetName("block_break");
	recoil.SetName("recoil");
	strafe_l.SetName("strafeleft");
	strafe_r.SetName("straferight");
	fgtCurType = fgtSetType = fgt_none;
	fgtCurIndex = fgtSetIndex = -1;
	isParryState = false;
	isFeintState = false;
	//
	isMove = false;
	isBack = false;
	isRun = false;
	isRunDisable = false;
	isFight = false;
	isDialog = false;
	isTurn = 0.0f;
	isFightWOWps = false;
	movecs = 1.0f;
	curMove = null;
	speed = 0.0f;
	turnspd = turnSpeed;
	chclass = 0.5f;
	fatigue = 0.0f;
	kSpd = 1.0f;
	kSpdCur = 1.0f;
	kSpdDlt = rand()*(0.1f/RAND_MAX) - 0.05f;
	numDetectors = 0;	
	startColCharacter = 0;
	numColCharacter = 0;
	isSlide = false;
	liveValue = 0.0f;
	eventListener.character = this;
	ayStackPointer = 0;
	isResetAutoAction = true;
	isSetPriorityAction = false;
	deadName = null;
	curIdleIndex = -1;
	isFired = true;
	isLockIdleForCamera = false;
	isSwim = false;
	swimChange = 0.0f;
	priorityActionMoveSpd = 0.0f;
	priorityActionRotSpd = 0.0f;
	isBladeSet = false;
	isGunSet = false;
	isJump = false;
	isJumpSnd = false;
	isEnableJump = false;
	jumpSound = SOUND_INVALID_ID;
	isNFHit = false;
	characterID = null;
	lockMove = false;
	lockRotate = false;
	impulse = 0.0f;
	strafeMove = 0.0f;
	recoilWait = 0.0f;
	strafeWait = 0.0f;
	camRotWait = 0.0f;
	isMale = true;
	recoilSound = SOUND_INVALID_ID;
	soundStep = false;
	soundGrass = false;
	//hScale = 1.0f;//0.9f + rand()*0.2f/RAND_MAX;
	//wScale = 1.2f;//0.95f + rand()*0.1f/RAND_MAX;
	group[0] = 0;
	numTargets = 0;
	groupID = -1;
	lockFightMode = false;
	stepsRate = 0.0f;
	noBlendTime = 1.0f;
	isActiveState = false;
	strafeAngle = 0.0f;
	strafeVel = 0.0f;

	m_pcHandLightLocator = "Saber_hand";
	m_nHandLightID = -1;
}

Character::~Character()
{
	// ������� �������� � ����
	if( m_nHandLightID >= 0 && location && !isDeleted )
		location->GetLights()->DelMovingLight(m_nHandLightID);
	m_nHandLightID = -1;

	//��������� �� �����
	ENTITY_ID grps;
	_CORE_API->FindClass(&grps, "CharactersGroups", 0);
	api->Send_Message(grps, "si", "UnloadCharacter", GetID());

	//����������� ���������
//	for(long i = 0; i < numDetectors; i++) detector[i]->Exit(this);
	//������� ���������
	for(long i = 0; i < numDetectors; i++) delete detector[i];
	//
	if(location && !isDeleted) location->supervisor.DelCharacter(this);
	_CORE_API->DeleteEntity(shadow);
	_CORE_API->DeleteEntity(waterrings);
	_CORE_API->DeleteEntity(mdl);
	_CORE_API->DeleteEntity(blade);
	_CORE_API->DeleteEntity(sign);
	if(characterID) delete characterID;
}

//�������������
bool Character::Init()
{
	//��������� �� �������
	ENTITY_ID loc;
	_CORE_API->FindClass(&loc, "location", 0);
	location = (Location *)_CORE_API->GetEntityPointer(&loc);
	if(!location) return false;
	_CORE_API->FindClass(&effects, "LocationEffects", 0);
	soundService = (VSoundService *)api->CreateService("SoundService");
	//������������ ��� ��������� � �������
	if(location->supervisor.numCharacters >= MAX_CHARACTERS)
	{
		api->Trace("Many characters in location");
		return false;
	}
	location->supervisor.AddCharacter(this);
	//����
	_CORE_API->FindClass(&sea, "sea", 0);
	//�������� �������������
	const char * id = null;
	if(AttributesPointer) id = AttributesPointer->GetAttribute("id");
	if(!id) id = "<none>";
	long len = strlen(id) + 1;
	characterID = NEW char[len];
	strcpy(characterID, id);
	//������� � ������
	ENTITY_ID grps;
	_CORE_API->FindClass(&grps, "CharactersGroups", 0);
	api->Send_Message(grps, "sis", "MoveCharacter", GetID(), group);
	SetSignModel();
	SetSignTechnique();
	return PostInit();
}

//���������
dword _cdecl Character::ProcessMessage(MESSAGE & message)
{
	if(message.GetCurrentFormatType() == 's')
	{
		return zPlaySound(message);
	}
	long messageID = message.Long();
	if(messageID == MSG_CHARACTER_EX_MSG)
	{
		return zExMessage(message);
	}
	if(deadName)
	{
		if(liveValue >= 0)
		{
			if(messageID == MSG_CHARACTER_EXIT_FROM_LOCATION)
			{
				ExitFromLocation();
				DelSavePosition(false);
				return 1;
			}
		}
		return 0;
	}	
	switch(messageID)
	{
	case MSG_CHARACTER_SETMODEL:
		return zLoadModel(message);
	case MSG_CHARACTER_GETMODEL:
		message.ScriptVariablePointer()->Set(mdl);
		return 1;
	case MSG_CHARACTER_GETPOS:
		message.ScriptVariablePointer()->Set(curPos.x);
		message.ScriptVariablePointer()->Set(curPos.y);
		message.ScriptVariablePointer()->Set(curPos.z);
		return 1;
	case MSG_CHARACTER_GETAY:
		message.ScriptVariablePointer()->Set(ay);
		return 1;
	case MSG_CHARACTER_DIST2D:
		return zDistByCharacter(message, true);
	case MSG_CHARACTER_DIST3D:
		return zDistByCharacter(message, false);
	case MSG_CHARACTER_TELEPORT:
		return zTeleport(message, false);
	case MSG_CHARACTER_TELEPORT_AY:
		return zTeleport(message, true);
	case MSG_CHARACTER_TELEPORT_TO_LOCATOR:		
		return zTeleportL(message);
	case MSG_CHARACTER_ADD_DETECTOR:
		return zAddDetector(message);
	case MSG_CHARACTER_DEL_DETECTOR:
		return zDelDetector(message);
	case MSG_CHARACTER_ENTRY_TO_LOCATION:
		return zEntry(message);
	case MSG_CHARACTER_EXIT_FROM_LOCATION:
		ExitFromLocation();
		DelSavePosition(false);
		return 1;
	case MSG_CHARACTER_TURNBYLOC:
		return zTurnByLoc(message);
	case MSG_CHARACTER_TURNBYCHR:
		return zTurnByChr(message);
	case MSG_CHARACTER_TURNBYPOINT:
		return zTurnByPoint(message);
	case MSG_CHARACTER_TURNAY:
		Turn(message.Float());
		return 1;
	case MSG_CHARACTER_TURNPUSH:
		if(ayStackPointer >= sizeof(ayStack)/sizeof(float)) return 0;
		ayStack[ayStackPointer++] = nay;
		return 1;
	case MSG_CHARACTER_TURNPOP:
		if(ayStackPointer <= 0) return 0;
		Turn(ayStack[--ayStackPointer]);
		return 1;
	case MSG_CHARACTER_SETBLADE:
		return zSetBlade(message);
	case MSG_CHARACTER_SETGUN:
		return zSetGun(message);
	case MSG_CHARACTER_ACTIONPLAY:
		return zActionPlay(message);
	case MSG_CHARACTER_FINDNEAR:
		FindNearCharacters(message);
		return 1;
	case MSG_CHARACTER_VISIBLE:
		return CharactersVisibleTest(message);
	case MSG_CHARACTER_BLADEHAND:
		if(message.Long() == 0)	{
			api->Send_Message(blade, "ll", MSG_BLADE_BELT, 0);
			api->Send_Message(blade, "ll", MSG_BLADE_BELT, 1);
		} else {
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
		}
		return 1;	
	case MSG_CHARACTER_VIEWDAMAGE:
		{
			float hit = message.Float();
			float chp = message.Float();
			float mhp = message.Float();
			if(hit < 0.0f) hit = 0.0f;
			if(chp < 0.0f) chp = 0.0f;
			if(mhp < 0.0f) mhp = 0.0f;
			location->AddDamageMessage(curPos + CVECTOR(0.0f, height, 0.0f), hit, chp, mhp);
		}
		return 1;
	default:
		return ChlProcessMessage(messageID, message);
	}
	return 0;
}

//��������� ��������
dword Character::AttributeChanged(ATTRIBUTES * apnt)
{
	if(deadName || liveValue < 0) return 0;
	if(!apnt || !apnt->GetThisName()) return 0;
	if(stricmp(apnt->GetThisName(), "model") == 0)
	{
		SetSignModel();
	}else
	if(stricmp(apnt->GetThisName(), "technique") == 0)
	{
		SetSignTechnique();
	}else
	if(stricmp(apnt->GetThisName(), "id") == 0 && apnt->GetParent() && !apnt->GetParent()->GetParent())
	{
		const char * id = apnt->GetThisAttr();
		if(!id) id = "<none>";
		if(characterID) delete characterID;
		long len = strlen(id) + 1;
		characterID = NEW char[len];
		strcpy(characterID, id);
	}else
	if(stricmp(apnt->GetThisName(), "actions") == 0 && apnt->GetParent() && !apnt->GetParent()->GetParent())
	{
		//������ �������� �����������
		//�������
		walk.ChangeName(apnt->GetAttribute("walk"));
		backwalk.ChangeName(apnt->GetAttribute("backwalk"));
		run.ChangeName(apnt->GetAttribute("run"));
		backrun.ChangeName(apnt->GetAttribute("backrun"));
		stsUp.ChangeName(apnt->GetAttribute("stsUp"));
		stsDown.ChangeName(apnt->GetAttribute("stsDown"));
		stsUpBack.ChangeName(apnt->GetAttribute("stsUpBack"));
		stsDownBack.ChangeName(apnt->GetAttribute("stsDownBack"));
		stsUpRun.ChangeName(apnt->GetAttribute("stsUpRun"));
		stsDownRun.ChangeName(apnt->GetAttribute("stsDownRun"));
		stsUpRunBack.ChangeName(apnt->GetAttribute("stsUpRunBack"));
		stsDownRunBack.ChangeName(apnt->GetAttribute("stsDownRunBack"));
		swim.ChangeName(apnt->GetAttribute("swim"));
		nfhit.ChangeName(apnt->GetAttribute("HitNoFight"));
		//������ �������� ��������		
		actionTurnL.ChangeName(apnt->GetAttribute("turnLeft"));
		actionTurnR.ChangeName(apnt->GetAttribute("turnRight"));		
		//������ �������� ������
		ATTRIBUTES * at = apnt->FindAClass(apnt, "dead");
		if(at)
		{
			long num = at->GetAttributesNum();
			for(long i = 0, j = 0; i < num && j < sizeof(actionDead)/sizeof(ActionDead); i++)
			{
				const char * iname = at->GetAttribute(i);
				if(!iname || !iname[0]) continue;
				if(j == 0) actionDead[j++].ChangeName(iname); else actionDead[j++].SetName(iname);
			}
			if(j < 1) j = 1;
			numActionDead = j;
		}
		//������ �������� �������� �������
		at = apnt->FindAClass(apnt, "idle");
		if(at)
		{
			curIdleIndex = -1;
			long num = at->GetAttributesNum();
			for(long i = 0, j = 0; i < num && j < sizeof(actionIdle)/sizeof(ActionIdle); i++)
			{
				const char * iname = at->GetAttribute(i);
				if(!iname || !iname[0]) continue;
				if(j == 0) actionIdle[j++].ChangeName(iname); else actionIdle[j++].SetName(iname);
			}
			if(j < 1) j = 1;
			numActionIdles = j;
		}		
		//������ �������� � ������ ���
		//�����������
		fightwalk.ChangeName(apnt->GetAttribute("fightwalk"));
		fightbackwalk.ChangeName(apnt->GetAttribute("fightbackwalk"));
		fightrun.ChangeName(apnt->GetAttribute("fightrun"));
		fightbackrun.ChangeName(apnt->GetAttribute("fightbackrun"));
		//������ �����
		ReadFightActions(apnt->FindAClass(apnt, "attack_fast"), attackFast, numAttackFast);
		ReadFightActions(apnt->FindAClass(apnt, "attack_force"), attackForce, numAttackForce);
		ReadFightActions(apnt->FindAClass(apnt, "attack_round"), attackRound, numAttackRound);
		ReadFightActions(apnt->FindAClass(apnt, "attack_break"), attackBreak, numAttackBreak);
		long fnt1, fnt2;
		ReadFightActions(apnt->FindAClass(apnt, "attack_feint"), attackFeint, fnt1);		
		ReadFightActions(apnt->FindAClass(apnt, "attack_feintc"), attackFeintC, fnt2);		
		numAttackFeint = min(fnt1, fnt2);
		ReadFightActions(apnt->FindAClass(apnt, "hit_attack"), hit, numHits);
		ReadFightActions(apnt->FindAClass(apnt, "parry"), parry, numParry);
		//�������
		shot.ChangeName(apnt->GetAttribute("shot"));
		//���������
		hitFeint.ChangeName(apnt->GetAttribute("hit_feint"));
		hitParry.ChangeName(apnt->GetAttribute("hit_parry"));
		hitRound.ChangeName(apnt->GetAttribute("hit_round"));
		hitFire.ChangeName(apnt->GetAttribute("hit_shot"));
		block.ChangeName(apnt->GetAttribute("block"));
		blockaxe.ChangeName(apnt->GetAttribute("block_axe"));
		blockhit.ChangeName(apnt->GetAttribute("blockhit"));
		blockaxehit.ChangeName(apnt->GetAttribute("blockaxehit"));
		blockbreak.ChangeName(apnt->GetAttribute("blockbreak"));
		recoil.ChangeName(apnt->GetAttribute("recoil"));
		strafe_l.ChangeName(apnt->GetAttribute("strafeleft"));
		strafe_r.ChangeName(apnt->GetAttribute("straferight"));
		//������ �������� ������
		at = apnt->FindAClass(apnt, "fightdead");
		if(at)
		{
			long num = at->GetAttributesNum();
			for(long i = 0, j = 0; i < num && j < sizeof(actionFightDead)/sizeof(ActionDead); i++)
			{
				const char * iname = at->GetAttribute(i);
				if(!iname || !iname[0]) continue;
				if(j == 0) actionFightDead[j++].ChangeName(iname); else actionFightDead[j++].SetName(iname);
			}
			if(j < 1) j = 1;
			numActionFightDead = j;
		}
		//�������
		at = apnt->FindAClass(apnt, "fightidle");
		if(at)
		{
			long num = at->GetAttributesNum();
			for(long i = 0, j = 0; i < num && j < sizeof(actionFightIdle)/sizeof(ActionIdle); i++)
			{
				const char * iname = at->GetAttribute(i);
				if(!iname || !iname[0]) continue;
				if(j == 0) actionFightIdle[j++].ChangeName(iname); else actionFightIdle[j++].SetName(iname);
			}
			if(j < 1) j = 1;
			numFightActionIdles = j;
		}
		//��������� ���������
		UpdateActionsData();
		//��������� ������������� ��������
		UpdateAnimation();
	}
	return 0;
}

void Character::SetSignModel()
{
	ATTRIBUTES * apnt = AttributesPointer->FindAClass(AttributesPointer, "quest.questflag.model");
	const char * signModelName = "";
	if(apnt)
	{
		signModelName = apnt->GetThisAttr();
	}	
	if(!signModelName) signModelName = "";
	if(signName == signModelName)
	{
		return;
	}
	signName = signModelName;
	api->DeleteEntity(sign);
	if(!signModelName[0])
	{
		return;
	}
	//���� �� �������
	VGEOMETRY * gs = (VGEOMETRY *)_CORE_API->CreateService("geometry");
	if(gs) gs->SetTexturePath("quest_signs\\");
	//���� �� ��������
	string path = "quest_signs\\";
	path += signModelName;
	//������ � ��������� ��������
	if(!_CORE_API->CreateEntity(&sign, "modelr"))
	{
		if(gs) gs->SetTexturePath("");
		return;
	}
	if(!_CORE_API->Send_Message(sign, 
		"ls", 
		MSG_MODEL_LOAD_GEO, 
		path.GetBuffer()))
	{
		if(gs) gs->SetTexturePath("");
		_CORE_API->Trace("Quest sign model '%s' not loaded", path.GetBuffer());
		return;
	}

	if( !signTechniqueName.IsEmpty() )
	{
		api->Send_Message(sign,"ls",MSG_MODEL_SET_TECHNIQUE,signTechniqueName.GetBuffer());
	}

	if(gs) gs->SetTexturePath("");
	_CORE_API->LayerAdd("realize", sign, 20000);
	_CORE_API->LayerAdd("sun_trace", sign, 10);
}

void Character::SetSignTechnique()
{
	// set technique
	ATTRIBUTES * pATechnique = AttributesPointer->FindAClass(AttributesPointer, "quest.questflag.technique");
	if( !pATechnique ) return;

	char* pcTechniqueName = pATechnique->GetThisAttr();
	if( !pcTechniqueName ) return;

	if( signTechniqueName==pcTechniqueName ) return;
	signTechniqueName = pcTechniqueName;

	api->Send_Message(sign,"ls",MSG_MODEL_SET_TECHNIQUE,pcTechniqueName);
}

void Character::ReadFightActions(ATTRIBUTES * at, ActionCharacter actions[4], long & counter)
{
	if(at)
	{
		long num = at->GetAttributesNum();
		for(long i = 0, j = 0; i < num && j < 4; i++)
		{
			const char * iname = at->GetAttribute(i);
			if(!iname || !iname[0]) continue;
			if(j == 0)
			{
				actions[j++].ChangeName(iname);
			}else{
				actions[j++].SetName(iname);
			}
		}
		if(j < 1) j = 1;
		counter = j;
	}else{
		counter = 1;
	}
}



//============================================================================================
//Character model
//============================================================================================

MODEL * Character::Model()
{
	return (MODEL *)_CORE_API->GetEntityPointer(&mdl);
}

//����������� �������� � ����� x, y, z
bool Character::Teleport(float x, float y, float z)
{
	//������
	isJump = false;
	MODEL * man = Model();
	if(!man) return false;
	//����� ����������
	man->mtx.SetPosition(x, y, z);
	curPos = oldPos = grsPos = CVECTOR(x, y, z);
	vy = 0.0f;
	float bearingY;
	currentNode = location->GetPtcData().FindNode(curPos, bearingY);
	if(curPos.y < bearingY) curPos.y = bearingY;
	CharacterTeleport();
	return true;
}

//����������� �������� � ����� x, y, z � ��������� �� ay
bool Character::Teleport(float x, float y, float z, float ay)
{
	//������
	isJump = false;
	MODEL * man = Model();
	if(!man) return false;
	//����� ����������
	man->mtx.BuildMatrix(0.0f, ay, 0.0f, x, y, z);
	curPos = oldPos = grsPos = CVECTOR(x, y, z);
	vy = 0.0f;
	Turn(ay);
	this->ay = nay;
	float bearingY;
	currentNode = location->GetPtcData().FindNode(curPos, bearingY);
	if(curPos.y < bearingY) curPos.y = bearingY;
	CharacterTeleport();
	return true;
}

//����������� �������� � �������
bool Character::Teleport(const char * group, const char * locator)
{
 if(!group || !group[0] || !locator || !locator[0]) return false;
 LocatorArray * la = location->FindLocatorsGroup(group);
 if(!la) return false;
 long li = la->FindByName(locator);
 if(li < 0) return false;
 CMatrix mtx;
 if(!la->GetLocatorPos(li, mtx)) return false;
 //���� ��� �������� �� Y
 double vz = mtx.Vz().z;
 double vx = mtx.Vz().x;
 double l = vx*vx + vz*vz;
 if(l > 0.0000001)
 {
  //���� ����
  vz = acos(vz/sqrt(l));
 }else vz = l;
 if(vx < 0) vz = -vz;

 // ������� ����� �� �� ��������� � ���� ������� ����
 CVECTOR pos = mtx.Pos();
 if (location->supervisor.CheckPosition(pos.x, pos.y, pos.z, this))
  return Teleport(pos.x, pos.y, pos.z, float(vz)); 

 // ���-�� ����� �����, �������� ����� ����� �� ���������� �������
 float radius = 1.75f;
 for (long i=0; i<10; i++)
 {
  float ang = float(i) / 9.0f * PIm2;
  float x = radius * sinf(ang);
  float z = radius * cosf(ang);

  // ��������� ���� �� ��� ����
  CVECTOR src = CVECTOR(pos.x + x, pos.y + 2.0f, pos.z + z);
  CVECTOR dst = CVECTOR(pos.x + x, pos.y - 2.0f, pos.z + z);
  float k = location->GetPtcData().Trace(src, dst);
  // ���� ���� ����� - �� ��������� ��������
  if (k > 1.0f) continue;
  // ���� ����� �������� - ������������ ����
  CVECTOR pnt = src + (dst - src) * k + CVECTOR(0.0f, 0.01f, 0.0f);
  if (location->supervisor.CheckPosition(pnt.x, pnt.y, pnt.z, this))
   return Teleport(pnt.x, pnt.y, pnt.z, float(vz)); 
 }

 api->Trace("Character Teleport Error: Can't find free place near locator: %s, %s", group, locator);
 return Teleport(pos.x, pos.y, pos.z, float(vz)); 
}

//���������� ������� ��� ��������
void Character::SetSavePosition()
{
	if(!AttributesPointer) return;
	ATTRIBUTES * at = AttributesPointer->CreateSubAClass(AttributesPointer, "saveposition");
	at->SetAttributeUseFloat("x", curPos.x);
	at->SetAttributeUseFloat("y", curPos.y);
	at->SetAttributeUseFloat("z", curPos.z);
	at->SetAttributeUseFloat("ay", ay);
	at = at->CreateSubAClass(at, "savedata");
	SetSaveData(at);
}

//������� ������� ��� ��������
void Character::DelSavePosition(bool isTeleport)
{
	if(!AttributesPointer) return;

	ATTRIBUTES * aPosLocator = AttributesPointer->FindAClass(AttributesPointer, "location");
	if( aPosLocator )
	{
		const char* pcLocGroupName = aPosLocator->GetAttribute("group");
		if( pcLocGroupName && stricmp(pcLocGroupName,"sit")==0 ) isTeleport = false;
	}

	ATTRIBUTES * at = AttributesPointer->FindAClass(AttributesPointer, "saveposition");
	if(at)
	{
		if(isTeleport)
		{
			curPos.x = at->GetAttributeAsFloat("x", curPos.x);
			curPos.y = at->GetAttributeAsFloat("y", curPos.y);
			curPos.z = at->GetAttributeAsFloat("z", curPos.z);
			ay = at->GetAttributeAsFloat("ay", ay);
			Teleport(curPos.x, curPos.y, curPos.z, ay);
			GetSaveData(at->FindAClass(at, "savedata"));
		}
		AttributesPointer->DeleteAttributeClassX(at);
	}
}


//============================================================================================
//Character commands
//============================================================================================

//�����
void Character::StartMove(bool isBack)
{
	if(priorityAction.name) return;
	isMove = true;
	this->isBack = isBack;
}

//������������
void Character::StopMove()
{
	if(priorityAction.name) return;
	isMove = false;
}

//��������� ��������� �� �������
void Character::Turn(float dx, float dz)
{
	//if(priorityAction.name) return;
	double vx = dx;
	double vz = dz;
	double l = vx*vx + vz*vz;
	if(l <= 0.0) return;	
	vz = acos(vz/sqrt(l));
	if(vx < 0) vz = -vz;
	Turn(float(vz));
}

//��������� ��������� �� ����
void Character::Turn(float _ay)
{	
	if(isTurnLock) return;
	const float pi = 3.14159265359f;
	nay = _ay;
	nay -= long(nay/pi)*2.0f*pi;
	if(nay - ay > pi) ay += 2*pi;
	if(nay - ay < -pi) ay -= 2*pi;
	if(fabs(nay - ay) > 0.00001f)
	{
		if(ay < nay) turnDir = 1.0f; else turnDir = -1.0f;
	}
}

//���������� ����� ����
void Character::SetRunMode(bool _isRun)
{
	if(priorityAction.name) return;
	isRun = _isRun;
}

//����������� ����� ���
bool Character::SetFightMode(bool _isFight, bool isPlayAni)
{
	UpdateWeapons();
	if(lockFightMode) return isFight;
	if(!isFightWOWps)
	{
		if(!isBladeSet) return isFight;
	}
	if(isFight == _isFight) return isFight;
	if(priorityAction.name) return isFight;
	if(isSwim) return isFight;
	if(!IsFightEnable())
	{
		if(!isFight) return false;
		_isFight = false;
	}
	isNFHit = false;
	bool old = isFight;
	isFight = _isFight;	
	if(isFight)
	{
		radius = radiusFgt;
		if(isPlayAni)
		{
			if(!SetPriorityAction(CHARACTER_NORM_TO_FIGHT)) {
				api->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
				api->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
			}
		}else{
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
		}
	}else{
		radius = radiusNrm;
		if(isPlayAni)
		{
			if(!SetPriorityAction(CHARACTER_FIGHT_TO_NORM)) {
				api->Send_Message(blade, "ll", MSG_BLADE_BELT, 0);
				api->Send_Message(blade, "ll", MSG_BLADE_BELT, 1);
			}
		}else{
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
			api->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
		}
		fgtCurType = fgtSetType = fgt_none;
		fgtCurIndex = fgtSetIndex = -1;
	}
	api->Event("Character_ChangeFightMode", "ll", isFight, old);
	return old;
}

//���������, ����� �� ���������� � ����� ���
bool Character::IsFightEnable()
{
	//������� � ������� � ����������� ��������
	VDATA * vd = _CORE_API->Event("Location_CharacterIsFight", "i", GetID());
	long res = 0;
	if(vd && vd->Get(res))
	{
		if(!res) return false;
	}else{
//!!!		_CORE_API->Trace("Event \"Location_CharacterIsFight\" -> return type is not int");
		return true;
	}
	return true;
}

//� ������ ������ ��� ����� ���� ��� �������� (�������)
bool Character::IsFireFindTarget()
{
	if(!priorityAction.name || !shot.name) return false;
	if(stricmp(priorityAction.name, shot.name) == 0) return !isFired;
	return false;
}

//���������
void Character::Attack(Character * enemy, FightAction type)
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(!isFight) return;
	if(type < fgt_attack_fast || type > fgt_attack_feintc) return;
	//if(!fightTbl[fgtCurType][type]) return;
	if(enemy)
	{
		enemyAttack = enemy->GetID();
		float dx = enemy->curPos.x - curPos.x;
		float dz = enemy->curPos.z - curPos.z;
		float cs = dx*dx + dz*dz;
		Turn(dx, dz);
		isTurnLock = true;
		/*
		if(cs > 0.0f)
		{
			float cdx = sinf(ay);
			float cdz = cosf(ay);
			cs = (cdx*dx + cdz*dz)/sqrtf(cs);
			static float csang = cosf(60.0f*3.141592654f/180.0f);
			if(cs >= csang)
			{
				Turn(dx, dz);
				isTurnLock = true;
			}
		}*/
	}else memset(&enemyAttack, 0, sizeof(enemyAttack));	
	VDATA * res = null;
	const char * aname = null;
	switch(type)
	{
	case fgt_attack_fast:
		aname = FGT_ATTACK_FAST;
		fgtSetIndex = rand() % numAttackFast;
		break;
	case fgt_attack_force:
		aname = FGT_ATTACK_FORCE;
		ObstacleZone zone[4];
		// ����� 1
		zone[0].use = true; zone[0].x = 0.f; zone[0].z = 1.f; zone[0].dw = 0.001f;
		// ����� 2
		zone[1].use = true; zone[1].x = 0.f; zone[1].z = 1.f; zone[1].dw = 0.001f;
		// ����� 3
		zone[2].use = true; zone[2].x = 0.f; zone[2].z = 1.f; zone[2].dw = 0.001f;
		// ����� 4
		zone[3].use = true; zone[3].x = 0.f; zone[3].z = 1.f; zone[3].dw = 0.001f;
		fgtSetIndex = GetRandomIndexByObstacle(zone, 4);
		//���� ����� �� ������ �� ����, �� ���������� ������� ����, � �� �����
		if( fgtSetIndex<0 )
		{
			type = fgt_attack_fast;
			aname = FGT_ATTACK_FAST;
			fgtSetIndex = rand() % numAttackFast;
		}
		break;
	case fgt_attack_round:
		aname = FGT_ATTACK_ROUND;
		fgtSetIndex = rand() % numAttackRound;
		break;
	case fgt_attack_break:
		aname = FGT_ATTACK_BREAK;
		fgtSetIndex = rand() % numAttackBreak;
		break;
	case fgt_attack_feint:
		aname = FGT_ATTACK_FEINT;
		fgtSetIndex = rand() % numAttackFeint;
		break;
	case fgt_attack_feintc:
        fgtSetType =  fgt_attack_feintc;  // fix boal
        //fgtCurType =
		if(fgtCurIndex >= 0 && fgtCurIndex < numAttackFeint) 
		{
			fgtSetIndex = fgtCurIndex;
		}else{
			fgtSetIndex = rand() % numAttackFeint;
		}
		return; // fix boal
		break;
	default:
		fgtSetType = fgt_none;
		fgtSetIndex = -1;
		return;
	}
	if(aname)
	{
		res = api->Event("ChrAttackAction", "is", GetID(), aname);
		if(res)
		{
			long isEnable = 1;
			if(res->Get(isEnable))
			{
				if(!isEnable)
				{
					res = null;
				}
			}else{
				res = null;
			}
		}
	}
	if(res)
	{
		fgtSetType = type;
	}else{
		fgtSetType = fgt_none;
		fgtSetIndex = -1;
	}	
}

//����
void Character::Block()
{
	// boal ���������� ����� ������ if(priorityAction.name) return;
	if(isSwim) return;
	if(!isFight) return;
	if(fgtCurType != fgt_blockhit && fgtSetType != fgt_blockhit && fgtCurType != fgt_blockbreak && fgtSetType != fgt_blockbreak)
	{
		fgtSetType = fgt_block;
		fgtSetIndex = 0;
		isTurnLock = false;
	}
}

//�����������
void Character::Parry()
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(!isFight) return;
	fgtSetType = fgt_parry;

	//fgtSetIndex = rand() % numParry;
	ObstacleZone zone[4];
	// ���� �����
	zone[0].use = true; zone[0].x = -1.f; zone[0].z = 0.f; zone[0].dw = 0.5f;
	// ���� ������ ���� ������
	zone[1].use = true; zone[1].x = 1.f; zone[1].z = 0.f; zone[1].dw = 0.5f;
	// ���� ������ ���� �����
	zone[2].use = true; zone[2].x = 1.f; zone[2].z = 0.f; zone[2].dw = 0.5f;
	// ������� ������ - �� �����
	zone[3].use = true; zone[3].x = 0.f; zone[3].z = 0.f; zone[3].dw = 0.5f;
	fgtSetIndex = GetRandomIndexByObstacle(zone, 4);
	//api->Trace("��������� ������: \"%i\"", fgtSetIndex);
	if( fgtSetIndex<0 ) fgtSetIndex = 3;

	isTurnLock = false;
}

//������
void Character::Recoil()
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(!isFight) return;
	if(recoilWait > 0.0f) return;
	if(!location->IsSwimming()) return;
	fgtSetType = fgt_recoil;
	fgtSetIndex = 0;
	recoilWait = 0.8f;
	isTurnLock = false;
}

//���� �����
void Character::StrafeLeft()
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(strafeWait > 0.0f) return;
	if(!location->IsSwimming()) return;
	strafeWait = 0.8f;
	fgtSetType = fgt_strafe_l;
	fgtSetIndex = 0;	
	isTurnLock = false;
	//impulse += 15.0f*CVECTOR(-cosf(ay), 0.0f, sinf(ay));
}

//���� ������
void Character::StrafeRight()
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(strafeWait > 0.0f) return;
	if(!location->IsSwimming()) return;
	strafeWait = 0.8f;
	fgtSetType = fgt_strafe_r;
	fgtSetIndex = 0;	
	isTurnLock = false;
	//impulse -= 15.0f*CVECTOR(-cosf(ay), 0.0f, sinf(ay));
}

//����
void Character::Hit(FightAction type)
{
	/*
	//!!!
	ENTITY_ID eid;
	if(stricmp(characterID, "Blaze") == 0)
	{
		api->FindClass(&eid, "ILogAndActions", 0);
	}
	char sbuf[256];
	sprintf(sbuf, "Hit! cur act: %s", fightNamesTbl[fgtCurType]);
	api->Send_Message(eid, "lls", 45020, false, sbuf);
	//!!!
	//*/
	if(priorityAction.name) return;
	if(isSwim) return;
	isNFHit = false;
	//if(!isFight)
	{
		if(!IsDead())
		{
			isNFHit = true;
		}
	}
	impulse = 0.0f;
	bool restBlockBreak = (fgtSetType == fgt_blockbreak);
	switch(type)
	{
	case fgt_hit_attack:
		if(fgtCurType != fgt_block && fgtCurType != fgt_blockhit)
		{
			HitChild(false);
			if(fgtSetType < fgt_hit_attack)
			{
				fgtSetType = fgt_hit_attack;
				fgtSetIndex = rand() % numHits;
			}
			//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_attack");
		}else{
			HitChild(true);
			fgtSetType = fgt_blockhit;
			fgtSetIndex = 0;
			//api->Send_Message(eid, "lls", 45020, false, "fgt_blockhit");
		}
		break;
	case fgt_hit_round:
		if(fgtCurType != fgt_block && fgtCurType != fgt_blockhit)
		{
			HitChild(false);
			fgtSetType = fgt_hit_round;
			fgtSetIndex = 0;
			//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_round");
		}else{
			HitChild(true);
			fgtSetType = fgt_blockhit;
			fgtSetIndex = 0;
			//api->Send_Message(eid, "lls", 45020, false, "fgt_blockhit");
		}
		break;
	case fgt_blockbreak:
		if(fgtCurType != fgt_block && fgtCurType != fgt_blockhit)
		{
			HitChild(false);
			fgtSetType = fgt_hit_attack;
			fgtSetIndex = rand() % numHits;
			//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_attack");
		}else{
			HitChild(true);
			fgtSetType = fgt_blockbreak;
			fgtSetIndex = 0;
			//api->Send_Message(eid, "lls", 45020, false, "fgt_blockbreak");
		}
		break;
	case fgt_hit_feint:
		HitChild(false);
		fgtSetType = fgt_hit_feint;
		fgtSetIndex = 0;
		//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_feint");
		break;
	case fgt_hit_parry:
		HitChild(true);
		fgtSetType = fgt_hit_parry;
		fgtSetIndex = 0;
		//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_parry");
		break;
	case fgt_hit_fire:
		HitChild(false);
		fgtSetType = fgt_hit_fire;
		fgtSetIndex = 0;
		//api->Send_Message(eid, "lls", 45020, false, "fgt_hit_fire");
		break;
	default:
		HitChild(false);
		fgtSetType = fgt_hit_attack;
		fgtSetIndex = rand() % numHits;
		//api->Send_Message(eid, "lls", 45020, false, "default hit!...");
	}
	if(restBlockBreak) fgtSetType = fgt_blockbreak;
}

//�������
void Character::Fire()
{
	if(priorityAction.name) return;
	if(isSwim) return;
	if(!IsGunLoad()) return;
	if(!location->IsSwimming()) return;
	//������ �������� ��������
	fgtSetType = fgt_fire;
	fgtSetIndex = 0;
}

//���������, ������� �� ��������
bool Character::IsGunLoad()
{
	if(!isGunSet) return false;
	if(!isFight || liveValue < 0 || deadName) return false;
	//������� � ������� � ����������� ��������
	VDATA * vd = _CORE_API->Event("Location_CharacterIsFire", "i", GetID());
	long res = 0;
	if(vd && vd->Get(res))
	{
		if(!res) return false;
	}else{
		_CORE_API->Trace("Event \"Location_CharacterIsFire\" -> return type is not int");
		//return false;
		//!!!
		return true;
	}
	return true;
}

//������
void Character::Dead()
{
	if(deadName) return;
	DelSavePosition(false);
	long num = numActionDead;
	ActionDead * dead = actionDead;
	if(isFight)
	{
		num = numActionFightDead;
		dead = actionFightDead;
	}
	Assert(num);
	Assert(dead);
	//������������ ���� � ����������� �� �����������
	float _ay = ay;
	static Supervisor::FindCharacter fnd[MAX_CHARACTERS];
	static long numChr = 0;
	for(long i = 0; i < num; i++)
	{
		ay = _ay + dead[i].ang;
		if(location->supervisor.FindCharacters(fnd, numChr, this, 2.0f, 0.0f, 0.0f)) dead[i].p *= 0.1f;
		float cs = cosf(dead[i].ang);
		float sn = sinf(dead[i].ang);
		CVECTOR p = curPos + CVECTOR(0.0f, 0.5f, 0.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p += CVECTOR(sn*0.5f, 0.0f, -cs*0.5f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p -= CVECTOR(sn*1.0f, 0.0f, -cs*1.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p = curPos + CVECTOR(0.0f, 1.0f, 0.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p += CVECTOR(sn*0.5f, 0.0f, -cs*0.5f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p -= CVECTOR(sn*1.0f, 0.0f, -cs*1.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.5f;
		p = curPos + CVECTOR(0.0f, 1.5f, 0.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.9f;
		p += CVECTOR(sn*0.5f, 0.0f, -cs*0.5f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.9f;
		p -= CVECTOR(sn*1.0f, 0.0f, -cs*1.0f);
		if(!location->VisibleTest(p, p + CVECTOR(cs, 0.0f, sn))) dead[i].p *= 0.9f;		
	}
	ay = _ay;
	//�������� ������������� �������
	float sum = 0.0f;
	for(i = 0; i < num; i++) sum += dead[i].p;
	//�������� ��������
	if(sum > 0.0f)
	{
		float p = rand()*sum/RAND_MAX;
		sum = 0.0f;
		for(i = 0; i < num; i++)
		{
			sum += dead[i].p;
			if(sum >= p) break;
		}
	}else{
		i = rand() % num;
	}
	Assert(i < num);
	//������ ��������
	deadName = dead[i].name;
	api->Event("Event_ChrSnd_Dead", "i", GetID());	
}

//����� � �������
void Character::EntryToLocation()
{
	tuner.alpha = 0.0f;
	liveValue = 1.2f;
}

//����� �� �������
void Character::ExitFromLocation()
{
	liveValue = -1.2f;
}

void Character::StartJump()
{
	if( !isJump && BuildJump(curPos,ay) )
	{
		isJump = true;
		isJumpSnd = true;
		jumpSound = SOUND_INVALID_ID;
		SetPriorityAction(jump.name);
	}
}

//============================================================================================
//Character
//============================================================================================

//�������� ���������
void Character::Reset()
{
	if(fgtCurType == fgt_block && !IsPlayer()) fgtCurType = fgt_none;
}

//���������� ��������� � �������� �������
void Character::Move(float dltTime)
{
	if(lockMove) dltTime = 0.0f;
	if(recoilWait > 0.0f) recoilWait -= dltTime;
	if(strafeWait > 0.0f) strafeWait -= dltTime;
	if(camRotWait > 0.0f) camRotWait -= dltTime;
	if(isTurnLock)
	{
		if(fgtCurType >= fgt_attack_fast && fgtCurType <= fgt_attack_feintc)
		{
			Character * eAttack = (Character *)api->GetEntityPointer(&enemyAttack);
			if(eAttack)
			{
				isTurnLock = false;
				Turn(eAttack->curPos.x - curPos.x, eAttack->curPos.z - curPos.z);
				isTurnLock = true;
			}
		}
	}

	//������ ���������
	if(tuner.selected > 0.0f) tuner.selected -= dltTime*3.0f;
	//���������� ��������
	float k = dltTime*0.5f;
	if(k > 1.0f) k = 1.0f;
	if(kSpd < 0.0f) kSpd = 0.0f;
	if(kSpd > 1.0f) kSpd = 1.0f;
	kSpdCur += (kSpd - kSpdCur)*k;
	k = kSpdCur*0.3f + 0.7f + kSpdDlt;
	MODEL * m = Model();
	if(m)
	{
		Animation * a = m->GetAnimation();
		if(a)
		{
			a->Player(0).SetSpeed(a->Player(0).GetDefSpeed()*k);
			a->Player(1).SetSpeed(a->Player(1).GetDefSpeed()*k);
		}
	}
	//������� �������
	oldPos = curPos;
	if(!isJump)
	{
		//�������� �����������
		if(bspeed.Update(dltTime)) speed = bspeed.Get();
		//�������� ��������
		if(bturn.Update(dltTime)) turnspd = bturn.Get();
		//�����������
		float spd = dltTime*speed*k;
		CVECTOR moveVec(sinf(ay)*spd, 0.0f, cosf(ay)*spd);
		curPos += moveVec;
		if(spd < 0.0f) moveVec = -moveVec;
		curPos += CVECTOR(moveVec.z, 0.0f, -moveVec.x)*(strafeMove*0.8f);
		curPos += impulse*dltTime;
		float kStrafe = dltTime*8.0f;
		bool noStrafe = true;
		bool noStrafeMove = true;
		if(fabsf(speed) > 0.1f)
		{
			float sm = strafeMove;
			if(speed < 0.0f)
			{
				sm = -sm;
			}
			if(sm > 0.1f)
			{
				noStrafe = false;
				strafeAngle += kStrafe;
				if(strafeAngle > 1.0f) strafeAngle = 1.0f;
			}else
			if(sm < -0.1f)
			{
				noStrafe = false;
				strafeAngle -= kStrafe;
				if(strafeAngle < -1.0f) strafeAngle = -1.0f;
			}
		}else{
			if(!IsFight())
			{
				if(strafeMove > 0.1f)
				{
					noStrafeMove = false;
					strafeVel += kStrafe;
					if(strafeVel > 1.0f) strafeVel = 1.0f;
				}else
				if(strafeMove < -0.1f)
				{
					noStrafeMove = false;
					strafeVel -= kStrafe;
					if(strafeVel < -1.0f) strafeVel = -1.0f;
				}
			}
		}
		if(noStrafe)
		{
			if(strafeAngle >= 0.0f)
			{
				strafeAngle -= kStrafe;
				if(strafeAngle < 0.0f) strafeAngle = 0.0f;
			}else{
				strafeAngle += kStrafe;
				if(strafeAngle > 0.0f) strafeAngle = 0.0f;
			}
		}
		if(noStrafeMove)
		{
			if(strafeVel >= 0.0f)
			{
				strafeVel -= kStrafe;
				if(strafeVel < 0.0f) strafeVel = 0.0f;
			}else{
				strafeVel += kStrafe;
				if(strafeVel > 0.0f) strafeVel = 0.0f;
			}
		}
		curPos += CVECTOR(cosf(ay), 0.0f, -sinf(ay))*(strafeVel*dltTime*0.9f);
		//��������
		if(turnDir > 0.0f)
		{
			ay += turnspd*dltTime;
			if(ay > nay)
			{
				ay = nay;
				turnDir = 0.0f;
			}
			isTurn += 0.8f*turnspd*dltTime;
			if(isTurn > 1.0f) isTurn = 1.0f;
		}else
		if(turnDir < 0.0f)
		{
			ay -= turnspd*dltTime;
			if(ay < nay)
			{
				ay = nay;
				turnDir = 0.0f;
			}
			isTurn -= 0.8f*turnspd*dltTime;
			if(isTurn < -1.0f) isTurn = -1.0f;
		}else{
			if(isTurn > 0.0f)
			{
				isTurn -= dltTime*1.7f;
				if(isTurn < 0.0f) isTurn = 0.0f;
			}else
			if(isTurn < 0.0f)
			{
				isTurn += dltTime*1.7f;
				if(isTurn > 0.0f) isTurn = 0.0f;
			}
		}
		//location->Print(curPos, 1000.0f, 4, 1.0f, 0xffffffff, 1.0f, "ay = %f, nay = %f, turnDir = %f, turnspd = %f", ay, nay, turnDir, turnspd);
		kSpd = 1.0f;
	}else{
		//�������� �� ���������� �������
		curJumpFallTime += dltTime;
		if(isJumpSnd && priorityAction.name)
		{
			SEA_BASE * sb = (SEA_BASE *)_CORE_API->GetEntityPointer(&sea);
			if(sb && location->IsSwimming())
			{
				seaY = sb->WaveXZ(curPos.x, curPos.z, null);
				if(seaY > curPos.y + 0.01f && PriorityActionIsJump())
				{
					PlaySound("jump_water");
					isJumpSnd = false;
					api->Send_Message(effects, "sffff", "Splashes", curPos.x, seaY - 0.01f, curPos.z, 1.0f);
					isSwim = true;
				}
			}
		}
		if(curJumpFallTime < jumpFallTime)
		{
			//���� �������� �������
			long s = long(curJumpFallTime/CHARACTER_JUMP_TIMESTEP);
			if(s >= jumpPoints) s = jumpPoints - 1;
			long e = s + 1;
			if(e >= jumpPoints) e = jumpPoints - 1;
			float kBlend = (curJumpFallTime/CHARACTER_JUMP_TIMESTEP) - s;
			if(s + kBlend > e) kBlend = float(e - s);
			if(kBlend < 0.0f) kBlend = 0.0f;
			if(kBlend > 1.0f) kBlend = 1.0f;
			curPos = jumpTrack[s] + (jumpTrack[e] - jumpTrack[s])*kBlend;
		}else{
			//��������� ������
			if(isJumpSnd)
			{
				PlaySound("jump");
				isJumpSnd = false;
			}
			Teleport(osculationPoint.x, osculationPoint.y, osculationPoint.z, ay);

			if(priorityAction.name)
			{
				MODEL * m = Model();
				if(m)
				{
					Animation * a = m->GetAnimation();
					if(a)
					{
						//��������� ������������
						a->Player(0).Play();
					}else priorityAction.SetName(null);
				}else priorityAction.SetName(null);
			}
		}
		if(isJumpSnd)
		{
			//SetSoundPosition(jumpSound);
		}else{
			//ReleaseSound(jumpSound);
			jumpSound = SOUND_INVALID_ID;
		}
	}
	//������� �������
	k = dltTime*5.0f;
	if(k > 1.0f) k = 1.0f;
	impulse -= impulse*k;

	//!!!
	//if(api->Controls->GetDebugAsyncKeyState('7') < 0) api->Send_Message(effects, "sffff", "Splashes", curPos.x, curPos.y, curPos.z, 1.0f);

	//������ ����� � ������ �����
	if(swimChange <= 0.0f && location->IsSwimming())
	{
		bool old = isSwim;
		SEA_BASE * sb = (SEA_BASE *)_CORE_API->GetEntityPointer(&sea);
		isSwim = false;
		isRunDisable = false;
		if(sb)
		{
			CVECTOR n;
			seaY = sb->WaveXZ(curPos.x, curPos.z, &n);
			isRunDisable = seaY - curPos.y > CHARACTER_SEA_NOTRUN;
			if(seaY > curPos.y)
			{
				stepsRate += dltTime;
				if(stepsRate > 0.15f)
				{
					stepsRate = 0.0f;
					api->Send_Message(waterrings, "ffflll", curPos.x, curPos.z, ay, isMove, IsRun(), isSwim);
				}
			}
			if(seaY > curPos.y + CHARACTER_SEA_MOVE*height)
			{
				//������ ������ ��� �� �����
				float dlt = old ? -0.03f : 0.03f;
				if(seaY > curPos.y + CHARACTER_SEA_SWIM*height + dlt)
				{
					//����� �����
					seaY -= CHARACTER_SEA_SWIM*height;
					isSwim = true;
				}
				float k = 5.0f*(seaY - curPos.y - CHARACTER_SEA_MOVE*height)*dltTime;
				if(k > 2.0f) k = 2.0f;
				n.y = 0.0f;
				curPos += n*k;
			}
		}
		if(isSwim != old) swimChange = 0.8f;
	}else swimChange -= dltTime;
}

//�������� ������� ���������
void Character::Update(float dltTime)
{
	//���������� �������
	if(isPlayerEnemy && isFight && !IsDead())
	{
		enemyBarsAlpha += dltTime*2.0f;
		if(enemyBarsAlpha > 1.0f) enemyBarsAlpha = 1.0f;
	}else{
		enemyBarsAlpha -= dltTime*2.0f;
		if(enemyBarsAlpha < 0.0f) enemyBarsAlpha = 0.0f;
	}
	if(enemyBarsAlpha > 1.0f/255.0f)
	{
		float hp = 0.5f;
		float energy = 0.5f;
		//������� ��������� ����� � �������
		VDATA * retVal = api->Event("NpcEvtHP", "i", GetID());
		if(retVal)
		{
			if(!retVal->Get(hp))
			{
				hp = 0.5f;
			}
		}
		retVal = api->Event("NpcEvtEny", "i", GetID());
		if(retVal)
		{
			if(!retVal->Get(energy))
			{
				energy = 0.5f;
			}
		}
		//��������� � ������ ��������
		CVECTOR pos = curPos + CVECTOR(0.0f, 2.0f, 0.0f);
		location->DrawEnemyBars(pos, hp, energy, tuner.GetAlpha()*enemyBarsAlpha);
	}
	//
	if(lockMove) dltTime = 0.0f;
	//��������� ����� ��� ������ �����
	{
		for(long i = 0; i < numTargets; i++)
		{
			grpTargets[i].time += dltTime;
		}
	}
	//����� ��������� ��������
	noBlendTime -= dltTime;
	if(noBlendTime <= 0.0f) noBlendTime = 0.0f;
	//�������� ������ � �����
	{
		float alpha = tuner.GetAlpha()*255.0f;
		dword blendColor = (dword(alpha) << 24) | 0x00ffffff;
		_CORE_API->Send_Message(blade, "ll", MSG_BLADE_ALPHA, blendColor);
		_CORE_API->Send_Message(shadow, "ll", MSG_BLADE_ALPHA, blendColor);
	}
	//
	PtcData & ptc = location->GetPtcData();
	if(deadName)
	{
		if(!priorityAction.name || stricmp(priorityAction.name, deadName) != 0)
		{
			priorityAction.SetName(deadName);
			isSetPriorityAction = false;
		}
	}
	MODEL * m = Model();
	if(!m) return;
	if(!isJump)
	{
		//�������� �������
		vy -= 9.8f*dltTime;
		curPos.y += vy*dltTime;
		//��������� � ������
		currentNode = ptc.Move(currentNode, curPos, oldPos);
		curPos = oldPos;
		if(ptc.isBearing) vy = 0.0f;
		isSlide = ptc.isSlide;
		if(ptc.isSlide && isEnableJump)
		{
			if(fall.name && !isFight && !priorityAction.name)
			{
				if(TestJump(curPos + CVECTOR(0.001f*sinf(ay), -0.001f, 0.001f*cosf(ay))))
				{	
					PlaySound("recoil");
					isJump = true;
					isJumpSnd = true;
					jumpSound = SOUND_INVALID_ID;
					PlaySound("clothes");
					if(fabsf(curPos.y - osculationPoint.y) > 1.5f) SetPriorityAction(fall.name);
					return;
				}
			}
		}
	}else{
		vy = 0.0f;
	}
	//SetSoundPosition(recoilSound);	
	//������ ����� � ������ �����
	CVECTOR chrPos = curPos;
	if(isSwim && location->IsSwimming())
	{
		vy = 0.0f;
		chrPos.y = seaY;	
	}
	//������ ������� ��� ��������
	m->mtx.BuildMatrix(CVECTOR(0.0f, ay + strafeAngle*3.1415f*0.25f*0.8f, 0.0f), chrPos);// - CVECTOR(0.0f, 0.14f, 0.0f));
	//�������� �� ����������
	float sn = 0.0f;
	CVECTOR nodeNorm;
	ptc.GetNodeNormal(currentNode, nodeNorm);
	float k = 10.0f*dltTime;
	if(k > 1.0f) k = 1.0f;	
	movecs += (nodeNorm.y - movecs)*k;
	isUp = nodeNorm.x*m->mtx.Vz().x + nodeNorm.z*m->mtx.Vz().z <= 0;
	//��������
	/*
	if(stricmp(characterID, "Blaze") == 0)
	{
		location->GetRS()->Print(10, 150, "curFgt = %s", fightNamesTbl[fgtCurType]);
		location->GetRS()->Print(10, 170, "setFgt = %s", fightNamesTbl[fgtSetType]);
	}
	*/
	UpdateAnimation();
	//����������� ���������
	for(long i = 0; i < numDetectors; i++) detector[i]->Check(dltTime, this);
	//���� ����� - ����� �������� �������
	if(curPos.y < -1000.0f)
	{
		//Assert(false);
		_CORE_API->Trace("Character [%s] fall to underworld!!!", characterID ? characterID : "Unknow id");
		curPos.y = -500.0f;
		vy = 0.0f;
	}
	//���������, ��������
	if(liveValue != 0.0f)
	{
		tuner.alpha += liveValue*dltTime;
		if(liveValue > 0.0f)
		{
			if(tuner.alpha >= 1.0f)
			{
				tuner.alpha = 1.0f;
				liveValue = 0.0f;
				_CORE_API->Event("Location_CharacterEntryToLocation", "e", GetID());
			}
		}else{
			if(tuner.alpha <= 0.0f)
			{
				tuner.alpha = 0.0f;
				if(deadName)
				{
					_CORE_API->Event("Location_CharacterDead", "e", GetID());
				}else{
					_CORE_API->Event("Location_CharacterExitFromLocation", "e", GetID());
				}
				_CORE_API->DeleteEntity(GetID());
			}
		}
	}
	CVECTOR camPos, camAng;
	float perspective;
	location->GetRS()->GetCamera(camPos, camAng, perspective);
	float dxz = (curPos.x - camPos.x)*(curPos.x - camPos.x) + (curPos.z - camPos.z)*(curPos.z - camPos.z);
	if(dxz < CHARACTER_HIDE_DIST*CHARACTER_HIDE_DIST)
	{
		tuner.chrAlpha -= dltTime*3.0f;
		if(tuner.chrAlpha < 0.1f) tuner.chrAlpha = 0.1f;
	}else{
		tuner.chrAlpha += dltTime*3.0f;
		if(tuner.chrAlpha > 1.0f) tuner.chrAlpha = 1.0f;
	}
	//
	k = dltTime*4.0f;
	if(k > 1.0f) k = 1.0f;
	grsPos += (curPos - grsPos)*k;
	//���� �����
	if(soundStep)
	{
		PlayStep();
		soundStep = false;
	}
	soundGrass = false;
	//
	MODEL * signMdl = (MODEL *)api->GetEntityPointer(&sign);
	if(signMdl)
	{
		CVECTOR dir = camPos - curPos; dir.y = 0.0f;
		float len = sqrtf(~dir);
		if(len > 1e-10f)
		{
			signMdl->mtx.Vz() = dir/len;
			signMdl->mtx.Vy() = CVECTOR(0.0f, 1.0f, 0.0f);
			signMdl->mtx.Vx() = signMdl->mtx.Vz() ^ signMdl->mtx.Vy();
		}
		signMdl->mtx.Pos() = curPos;
	}

	// �������� ����� � ���������
	if( m_nHandLightID >= 0 )
	{
		location->GetLights()->UpdateMovingLight( m_nHandLightID, GetHandLightPos() );
	}
}

//������� ��������� ������������ �������� ��������
void Character::ActionEvent(const char * actionName, Animation * animation, long index, long eventID, AnimationEvent event)
{
	if(index != 0) return;
	if(event == ae_end) curIdleIndex = -1;

	if(fgtCurType != fgt_block) fgtCurType = fgt_none;
	isTurnLock = false;
	if(priorityAction.name && stricmp(actionName, priorityAction.name) == 0)
	{
		if(stricmp(priorityAction.name, CHARACTER_NORM_TO_FIGHT) == 0)
		{
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
			if(event == ae_end)
			{
				priorityAction.SetName(null);
				animation->Player(0).Pause();
				animation->Player(0).SetPosition(1.0f);
			}
		}else
		if(stricmp(priorityAction.name, CHARACTER_FIGHT_TO_NORM) == 0)
		{
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_BELT, 0);
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_BELT, 1);
			if(event == ae_end)
			{
				priorityAction.SetName(null);
				animation->Player(0).Pause();
				animation->Player(0).SetPosition(1.0f);
			}
		}else
		if(shot.name && stricmp(priorityAction.name, shot.name) == 0)
		{
			_CORE_API->Send_Message(blade, "l", MSG_BLADE_GUNBELT);
			if(event == ae_end)
			{
				//���������� �������
				priorityAction.SetName(null);
				animation->Player(0).Pause();
				animation->Player(0).SetPosition(1.0f);
				if(fgtCurType == fgt_fire)
				{
					fgtSetType = fgtCurType = fgt_none;
				}
			}
		}else
		if(recoil.name && stricmp(priorityAction.name, recoil.name) == 0)
		{
			if(rand() % 10 > 7) recoilLook = true; //� ������� ��������� ���������
			priorityAction.SetName(null);
		}else
		if(deadName && stricmp(priorityAction.name, deadName) == 0)
		{
			animation->Player(0).Pause();
			animation->Player(0).SetPosition(1.0f);
			api->Event("Location_Character_Dead", "i", GetID());
		}else
		if( PriorityActionIsJump() )
		{
			priorityAction.SetName(null);
			animation->Player(0).Pause();
			animation->Player(0).SetPosition(1.0f);

			// �������� ���� ����� � ��������� �������� ����� ������� �� ����� � � ����.
			SEA_BASE * sb = (SEA_BASE *)api->GetEntityPointer(&sea);
			if(sb)
			{
				if( sb->WaveXZ(curPos.x, curPos.z, null) - curPos.y > CHARACTER_SEA_SWIM )
					SetPriorityAction(fall_water.name);
				else
					SetPriorityAction(fall_land.name);
			}
			else
				SetPriorityAction(fall_land.name);
		}else
		if( stricmp(priorityAction.name, fall_land.name)==0 || stricmp(priorityAction.name, fall_water.name)==0 )
		{
			priorityAction.SetName(null);
			animation->Player(0).Pause();
			animation->Player(0).SetPosition(1.0f);
		}
	}else
	if(userIdle.name)
	{
		if(stricmp(actionName, userIdle.name) != 0) return;
		_CORE_API->Event("Location_Character_EndAction", "i", GetID());
	}
}

void Character::ActionEvent(Animation * animation, long playerIndex, const char * eventName)
{
	if(playerIndex) return;
	const char * actionName = animation->Player(playerIndex).GetAction();
	const char * alliace = 0;
	if(!actionName) return;
	if(stricmp(eventName, "LStep") == 0)
	{
		soundStep = true;
	}else
	if(stricmp(eventName, "RStep") == 0)
	{
		soundStep = true;
	}else
	if(stricmp(eventName, "swim") == 0)
	{
		PlaySound("swiming");
	}else
	if((alliace = GetValueByPrefix(eventName, "SndAlliace_")) != 0)
	{
		PlaySound(alliace);
	}else
	/*
	if(stricmp(eventName, "Sound_wind_fast") == 0)
	{
		PlaySound("sword_wind_fast");
	}else
	if(stricmp(eventName, "Sound_wind_force") == 0)
	{
		PlaySound("sword_wind_force");
	}else
	if(stricmp(eventName, "Sound_wind_round") == 0)
	{
		PlaySound("sword_wind_round");
	}else
	if(stricmp(eventName, "Sound_wind_break") == 0)
	{
		PlaySound("sword_wind_break");
	}else
	if(stricmp(eventName, "Sound_wind_feint") == 0)
	{
		PlaySound("sword_wind_feint");
	}else
	if(stricmp(eventName, "Sound_wind_feintc") == 0)
	{
		PlaySound("sword_wind_feintc");
	}else
	if(stricmp(eventName, "Sound_wind_feintend") == 0)
	{
		PlaySound("sword_wind_feintend");
	}else*/
	if(stricmp(eventName, "Resact") == 0)
	{
		fgtSetType = fgt_none;
		fgtSetIndex = -1;
	}else
	if(stricmp(eventName, "Attack") == 0)
	{
		CheckAttackHit();
	}else
	if(stricmp(eventName, "Parry start") == 0)
	{
		isParryState = true;
		isFeintState = false;
	}else
	if(stricmp(eventName, "Parry end") == 0)
	{
		isParryState = false;
		isFeintState = false;
	}else
	if(stricmp(eventName, "Feint start") == 0)
	{
		isParryState = false;
		isFeintState = true;
	}else
	if(stricmp(eventName, "Feint end") == 0)
	{
		isParryState = false;
		isFeintState = false;
	}else/*
	if(stricmp(eventName, "sound_pistol") == 0)
	{
		PlaySound("pistol_out");
	}else
	if(stricmp(eventName, "Blade to hand") == 0)
	{
		if(!isFightWOWps)
		{
			float pos = animation->Player(0).GetPosition();
			if(pos < 0.99f) PlaySound("sword_sh");
		}
	}else
	if(stricmp(eventName, "Blade to belt") == 0)
	{
		if(!isFightWOWps)
		{
			float pos = animation->Player(0).GetPosition();
			if(pos < 0.99f) PlaySound("sword_out");
		}
	}else */
	if(stricmp(eventName, "Death sound") == 0)
	{
		api->Event("Event_ChrSnd_Body", "i", GetID());
	}else
	if((alliace=GetValueByPrefix(eventName, "itemaction_")) != 0)
	{
		char* pcActionName = 0;
		long nIdx = -1;
		if( strnicmp(alliace,"set",3)==0 ) {
			pcActionName = "set";
			if( alliace[3]>='0' && alliace[3]<='9' )
				nIdx = atoi(&alliace[3]);
		} else if( strnicmp(alliace,"reset",5)==0 ) {
			pcActionName = "reset";
			if( alliace[5]>='0' && alliace[5]<='9' )
				nIdx = atoi(&alliace[5]);
		}
		if( pcActionName )
			api->Event("Location_CharacterItemAction", "isl", GetID(), pcActionName, nIdx);
	}else
	if(priorityAction.name && stricmp(actionName, priorityAction.name) == 0)
	{
		if(stricmp(priorityAction.name, CHARACTER_NORM_TO_FIGHT) == 0)
		{
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_HAND, 0);
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_HAND, 1);
		}else
		if(stricmp(priorityAction.name, CHARACTER_FIGHT_TO_NORM) == 0)
		{
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_BELT, 0);
			_CORE_API->Send_Message(blade, "ll", MSG_BLADE_BELT, 1);
		}else
		if(shot.name && stricmp(priorityAction.name, shot.name) == 0)
		{
			if(eventName)
			{
				if(stricmp(eventName, CHARACTER_FIGHT_GUNBELT) == 0)
				{
					_CORE_API->Send_Message(blade, "l", MSG_BLADE_GUNBELT);
				}else
				if(stricmp(eventName, CHARACTER_FIGHT_GUNHAND) == 0)
				{
					_CORE_API->Send_Message(blade, "l", MSG_BLADE_GUNHAND);
				}else
				if(stricmp(eventName, CHARACTER_FIGHT_GUNFIRE) == 0)
				{
					_CORE_API->Send_Message(blade, "l", MSG_BLADE_GUNFIRE);
					//PlaySound("pistol_shot");
					isFired = true;
					float kDist;
					Character * chr = FindGunTarget(kDist);
					ENTITY_ID enemy;
					if(chr)
					{
						enemy = chr->GetID();
						chr->Hit(fgt_hit_fire);
					}
					_CORE_API->Event("Location_CharacterFire", "iifl", GetID(), enemy, kDist, chr != null);
				}
			}
		}else
		if(isJump && PriorityActionIsJump())
		{
			if(eventName && stricmp("Jump pause", eventName) == 0)
			{
				animation->Player(playerIndex).Pause();
			}
		}
	} else
	if(userIdle.name)
	{
		if(stricmp(actionName, userIdle.name)==0)
		{
			if(shot.name && stricmp(actionName,shot.name)==0)
			{
				if(eventName)
				{
					if(stricmp(eventName, CHARACTER_FIGHT_GUNBELT) == 0) {
						api->Send_Message(blade, "l", MSG_BLADE_GUNBELT);
					} else if(stricmp(eventName, CHARACTER_FIGHT_GUNHAND) == 0) {
						api->Send_Message(blade, "l", MSG_BLADE_GUNHAND);
					} else if(stricmp(eventName, CHARACTER_FIGHT_GUNFIRE) == 0) {
						api->Send_Message(blade, "l", MSG_BLADE_GUNFIRE);
						api->Event("ActorMakeShot","i",GetID());
					}
				}
			}
		}
	}
}

long Character::PlaySound(const char * soundName, bool isLoop, bool isCached)
{
	if(!soundService) return SOUND_INVALID_ID;
	CVECTOR pos = curPos + CVECTOR(0.0f, 1.0f, 0.0f);
	long sID = soundService->SoundPlay(soundName, PCM_3D, VOLUME_FX, false, false, isCached, 0, &pos);
	return sID;
}

void Character::PlayStep()
{
	if(!soundService) return;
	if(isSwim) return;
	SEA_BASE * sb = (SEA_BASE *)_CORE_API->GetEntityPointer(&sea);
	if(sb && location->IsSwimming())
	{
		//�������� ������ ����
		seaY = sb->WaveXZ(curPos.x, curPos.z, null);
		if(seaY > curPos.y)
		{
			PlaySound("step_water", false, true);
			return;
		}
	}
	//��� �� �����

	if( IsRun() && !isBack ) {
		if(!soundGrass)
		{
			const char * defSnd = "run_grass";
			const char * mtl = location->GetPtcData().GetMaterial(currentNode);
			if(mtl)
			{
				if(stricmp(mtl, "run_grass") == 0)
				{
					defSnd = "run_grass";
				}else
				if(stricmp(mtl, "snd_wood") == 0)
				{
					defSnd = "run_wood";
				}else
				if(stricmp(mtl, "snd_ground") == 0)
				{
					defSnd = "run_ground";
				}else
				if(stricmp(mtl, "snd_sand") == 0)
				{
					defSnd = "run_sand";
				}else
				if(stricmp(mtl, "snd_stone") == 0)
				{
					defSnd = "run_stone";
				}else
				if(stricmp(mtl, "snd_stairway") == 0)
				{
					defSnd = "run_stairway";
				}else
				if(stricmp(mtl, "snd_carpet") == 0)
				{
					defSnd = "run_carpet";
				}else
				if(stricmp(mtl, "snd_church") == 0)
				{
					defSnd = "run_church";
				}else
				if(stricmp(mtl, "snd_echo") == 0)
				{
					defSnd = "run_echo";
				}
				/*else
				if(stricmp(mtl, "snd_iron") == 0)
				{
					defSnd = "run_iron";
				}*/
			}
			PlaySound(defSnd, false, true);
		}else{
			PlaySound("run_grass_noise", false, true);
		}
	} else {
		if(!soundGrass)
		{
			const char * defSnd = "step_grass";
			const char * mtl = location->GetPtcData().GetMaterial(currentNode);
			if(mtl)
			{
				if(stricmp(mtl, "snd_grass") == 0)
				{
					defSnd = "step_grass";
				}else
				if(stricmp(mtl, "snd_wood") == 0)
				{
					defSnd = "step_wood";
				}else
				if(stricmp(mtl, "snd_ground") == 0)
				{
					defSnd = "step_ground";
				}else
				if(stricmp(mtl, "snd_sand") == 0)
				{
					defSnd = "step_sand";
				}else
				if(stricmp(mtl, "snd_stone") == 0)
				{
					defSnd = "step_stone";
				}else
				if(stricmp(mtl, "snd_stairway") == 0)
				{
					defSnd = "step_stairway";
				}else
				if(stricmp(mtl, "snd_carpet") == 0)
				{
					defSnd = "step_carpet";
				}else
				if(stricmp(mtl, "snd_church") == 0)
				{
					defSnd = "step_church";
				}else
				if(stricmp(mtl, "snd_echo") == 0)
				{
					defSnd = "step_echo";
				}else
				if(stricmp(mtl, "snd_iron") == 0)
				{
					defSnd = "step_iron";
				}
			}
			PlaySound(defSnd, false, true);
		}else{
			PlaySound("grass_noise", false, true);
		}
	}
}

void Character::SetSoundPosition(long id)
{
	if(!soundService || id == SOUND_INVALID_ID) return;
	CVECTOR pos = curPos + CVECTOR(0.0f, 1.0f, 0.0f);
	if(location->supervisor.player)
	{
		VDX8RENDER * rs = location->GetRS();
		if(rs)
		{
			static CMatrix view, cur;
			rs->GetTransform(D3DTS_VIEW, view);
			cur.BuildMatrix(CVECTOR(0.0f, ay, 0.0f), location->supervisor.player->curPos);
			cur.MulToInv(CVECTOR(pos), pos);
			view.MulToInv(CVECTOR(pos), pos);
		}
	}
	soundService->SoundSet3DParam(id, SM_POSITION, &pos);
}

void Character::ReleaseSound(long id)
{
	if(!soundService) return;
	if(id != SOUND_INVALID_ID) soundService->SoundRelease(id);
}


//============================================================================================
//������������
//============================================================================================

bool Character::zLoadModel(MESSAGE & message)
{
	char name[256];
	char ani[256];
	char mpath[300];
	_CORE_API->DeleteEntity(shadow);
	_CORE_API->DeleteEntity(waterrings);
	_CORE_API->DeleteEntity(mdl);
	message.String(256, name);
	name[255] = 0;
	message.String(256, ani);
	ani[255] = 0;
	//���� �� �������
	VGEOMETRY * gs = (VGEOMETRY *)_CORE_API->CreateService("geometry");
	if(gs) gs->SetTexturePath("characters\\");
	//���� �� ��������
	strcpy(mpath, "characters\\");
	strcat(mpath, name);
	//������ � ��������� ��������
	if(!_CORE_API->CreateEntity(&mdl, "modelr"))
	{
		if(gs) gs->SetTexturePath("");
		return false;
	}
	if(!_CORE_API->Send_Message(mdl, 
							"ls", 
							MSG_MODEL_LOAD_GEO, 
							mpath))
	{
		if(gs) gs->SetTexturePath("");
		_CORE_API->Trace("Character model '%s' not loaded", mpath);
		return false;
	}
	if(gs) gs->SetTexturePath("");
	if(!_CORE_API->Send_Message(mdl, 
									"ls",
									MSG_MODEL_LOAD_ANI,
									ani) != 0)
	{
		_CORE_API->Trace("Character animation '%s' not loaded", ani);
		_CORE_API->DeleteEntity(mdl);
		return false;
	}
	MODEL * m = (MODEL *)_CORE_API->GetEntityPointer(&mdl);
	if(!m)
	{
		_CORE_API->DeleteEntity(mdl);
		return false;
	}
	Animation * a = m->GetAnimation();
	if(a)
	{
		a->SetEventListener(&eventListener);
		a->SetEvent(ae_end, 0, &eventListener);
	}
	m->SetRenderTuner(&tuner);
	_CORE_API->LayerAdd("realize", mdl, 20);
	_CORE_API->LayerAdd("sun_trace", mdl, 10);
	if(_CORE_API->CreateEntity(&shadow, "shadow"))
	{
		_CORE_API->Send_Message(shadow, "li", 0, mdl);
	}else{
		_CORE_API->Trace("Shadow not created!");
	}
	if(_CORE_API->FindClass(null, "sea", 0))
	{
		_CORE_API->CreateEntity(&waterrings, "waterrings");
	}
	UpdateActionsData();
	return true;
}

bool Character::zTeleport(MESSAGE & message, bool isAy)
{
	float x = message.Float();
	float y = message.Float();
	float z = message.Float();
	if(isAy) return Teleport(x, y, z, message.Float());
	return Teleport(x, y, z);
}

bool Character::zTeleportL(MESSAGE & message)
{
	char group[256];
	char name[256];
	message.String(256, group);
	group[255] = 0;
	message.String(256, name);
	name[255] = 0;
	return Teleport(group, name);
}

bool Character::zAddDetector(MESSAGE & message)
{
	if(numDetectors >= sizeof(detector)/sizeof(Detector *)) return false;
	char group[256];
	message.String(256, group);
	group[255] = 0;
	if(!group[0]) return false;
	//��������� �� �����������
	for(long i = 0; i < numDetectors; i++)
	{
		if(stricmp(detector[i]->la->GetGroupName(), group) == 0) return false;
	}
	//���� ������
	LocatorArray * la = location->FindLocatorsGroup(group);
	if(!la) return false;
	detector[numDetectors++] = NEW Detector(la);
	return true;
}

bool Character::zDelDetector(MESSAGE & message)
{
	char group[256];
	message.String(256, group);
	group[255] = 0;
	for(long i = 0; i < numDetectors; i++)
	{
		if(stricmp(detector[i]->la->GetGroupName(), group) == 0)
		{
			detector[i]->Exit(this);
			delete detector[i];
			detector[i] = detector[--numDetectors];
			return true;
		}
	}
	return false;
}

bool Character::zActionPlay(MESSAGE & message)
{
	char buf[256];
	message.String(sizeof(buf) - 1, buf);
	buf[sizeof(buf) - 1] = 0;
	if(buf[0])
	{
		userIdle.SetName(buf);
		MODEL * m = Model();
		if(!m) return false;
		Animation * a = m->GetAnimation();
		if(!a) return false;
		UpdateActionCharacterData(userIdle, a);
	}else userIdle.SetName(null);
	return true;
}

bool Character::zEntry(MESSAGE & message)
{
	EntryToLocation();
	char grp[256];
	char name[256];
	message.String(sizeof(grp) - 1, grp);
	grp[sizeof(grp) - 1] = 0;
	message.String(sizeof(name) - 1, name);
	name[sizeof(name) - 1] = 0;
	return Teleport(grp, name);
}

bool Character::zSetBlade(MESSAGE & message)
{
	long nBladeIdx = message.Long();

	isBladeSet = false;
	char name[256];
	message.String(sizeof(name) - 1, name);
	name[sizeof(name) - 1] = 0;
	isBladeSet = true;
	if(!name[0])
	{
		isBladeSet = false;
		SetFightMode(false);
	}
	float t = message.Float();
	long s = message.Long();
	long e = message.Long();
	if(!_CORE_API->GetEntityPointer(&blade))
	{
		if(!_CORE_API->CreateEntity(&blade, "blade")) return false;
	}
	_CORE_API->Send_Message(blade, "llisfll", MSG_BLADE_SET, nBladeIdx, mdl, name, t, s, e);
	UpdateWeapons();
	return true;
}

bool Character::zSetGun(MESSAGE & message)
{
	isGunSet = false;
	//if(!isBladeSet) return false;  //eddy. �������� ����� ������, �� �� ������
	char name[256];
	message.String(sizeof(name) - 1, name);
	name[sizeof(name) - 1] = 0;
	isGunSet = true;
	if(!name[0]) isGunSet = false;
	if(!_CORE_API->GetEntityPointer(&blade))
	{
		if(!_CORE_API->CreateEntity(&blade, "blade")) return false;
	}
	_CORE_API->Send_Message(blade, "lis", MSG_BLADE_GUNSET, mdl, name);
	UpdateWeapons();
	return true;
}

bool Character::zTurnByLoc(MESSAGE & message)
{
	char group[256];
	char name[256];
	message.String(sizeof(group) - 1, group);
	group[sizeof(group) - 1] = 0;
	message.String(sizeof(name) - 1, name);
	name[sizeof(name) - 1] = 0;
	LocatorArray * la = location->FindLocatorsGroup(group);
	if(!la) return false;
	long li = la->FindByName(name);
	if(li < 0) return false;
	CMatrix mtx;
	if(!la->GetLocatorPos(li, mtx)) return false;
	bool isTo = message.Long() != 0;
	if(isTo)
	{
		Turn(mtx.Pos().x - curPos.x, mtx.Pos().z - curPos.z);
	}else{
		Turn(mtx.Vz().x, mtx.Vz().z);
	}
	return true;
}

bool Character::zTurnByChr(MESSAGE & message)
{
	ENTITY_ID chr = message.EntityID();
	Character * c = (Character *)_CORE_API->GetEntityPointer(&chr);
	if(!c) return false;
	Turn(c->curPos.x - curPos.x, c->curPos.z - curPos.z);
	return true;
}

bool Character::zTurnByPoint(MESSAGE & message)
{
	float dx = message.Float() - curPos.x;
	message.Float();
	float dz = message.Float() - curPos.z;
	Turn(dx, dz);
	return true;
}

bool Character::zDistByCharacter(MESSAGE & message, bool is2D)
{
	ENTITY_ID chr = message.EntityID();
	Character * c = (Character *)_CORE_API->GetEntityPointer(&chr);
	if(!c) return false;
	float dx = curPos.x - c->curPos.x;
	float dz = curPos.z - c->curPos.z;
	if(is2D)
	{
		message.ScriptVariablePointer()->Set(sqrtf(dx*dx + dz*dz));
	}else{
		float dy = curPos.y - c->curPos.y;
		message.ScriptVariablePointer()->Set(sqrtf(dx*dx + dy*dy + dz*dz));
	}
	return true;
}

dword Character::zExMessage(MESSAGE & message)
{
	char msg[64];
	char grp[32];
	message.String(sizeof(msg), msg);
	msg[sizeof(msg) - 1] = 0;
	LocatorArray * la;
	long i;
	VDATA * v;
	CVECTOR pos;
	if(stricmp(msg, "TieItem") == 0)
	{
		i = message.Long();
		char modelName[MAX_PATH];
		message.String(sizeof(modelName),modelName);
		char locatorName[128];
		message.String(sizeof(locatorName),locatorName);
		if(!api->GetEntityPointer(&blade))
		{
			if(!api->CreateEntity(&blade, "blade")) return 0;
			UpdateWeapons();
		}
		api->Send_Message(blade, "lilss", 1001, mdl,i,modelName,locatorName);
		return 1;
	} else
	if(stricmp(msg, "UntieItem") == 0)
	{
		i = message.Long();
		api->Send_Message(blade, "ll", 1002, i);
		return 1;
	} else
	if(stricmp(msg, "HandLightOn") == 0)
	{
		// ������ ������ ��������
		if( m_nHandLightID >= 0 )
			location->GetLights()->DelMovingLight(m_nHandLightID);
		// �������� �����
		message.String(sizeof(msg),msg);
		m_nHandLightID = location->GetLights()->AddMovingLight( msg, GetHandLightPos() );
		return 1;
	}else
	if(stricmp(msg, "HandLightOff") == 0)
	{
		if( m_nHandLightID >= 0 )
			location->GetLights()->DelMovingLight(m_nHandLightID);
		m_nHandLightID = -1;
		return 1;
	}else
	if(stricmp(msg, "PlaySound") == 0)
	{
		message.String(32, msg);
		msg[31] = 0;
		return PlaySound(msg) != SOUND_INVALID_ID;
	}else
	if(stricmp(msg, "IsFightMode") == 0)
	{
		return IsFight();
	}else
	if(stricmp(msg, "IsSetBalde") == 0)
	{
		return IsSetBlade();
	}else
	if(stricmp(msg, "IsDead") == 0)
	{
		return deadName != 0;
	}else
	if(!deadName)
	{
		if(stricmp(msg, "FindDialogCharacter") == 0)
		{
			Character * chr = FindDialogCharacter();
			if(chr && chr->AttributesPointer)
			{
				return chr->AttributesPointer->GetAttributeAsDword("index", -1);
			}
			return -1;
		}else
		if(stricmp(msg, "SetFightMode") == 0)
		{
			return SetFightMode(message.Long() != 0, false);
		}else
		if(stricmp(msg, "ChangeFightMode") == 0)
		{
			return SetFightMode(message.Long() != 0, true);
		}else
		if(stricmp(msg, "FindForvardLocator") == 0)
		{
			//��� �����
			message.String(sizeof(grp), grp);
			grp[sizeof(grp) - 1] = 0;
			//���������� ��� ����� ��������
			v = message.ScriptVariablePointer();
			//���� �������
			la = location->FindLocatorsGroup(grp);
			i =location->supervisor.FindForvardLocator(la, curPos, CVECTOR(sinf(ay), 0.0f, cosf(ay)));
			if(i < 0)
			{
				v->Set("");
				return 0;
			}
			v->Set((char *)la->LocatorName(i));
			return 1;
		}else
		if(stricmp(msg, "DistToLocator") == 0)
		{
			//��� �����
			message.String(sizeof(grp), grp);
			grp[sizeof(grp) - 1] = 0;
			//��� ��������
			message.String(sizeof(msg), msg);
			msg[sizeof(msg) - 1] = 0;
			v = message.ScriptVariablePointer();
			la = location->FindLocatorsGroup(grp);
			i = -1;
			if(la) i = la->FindByName(msg);
			if(i < 0)
			{
				v->Set(-1.0f);
				return 0;
			}
			la->GetLocatorPos(i, pos.x, pos.y, pos.z);
			v->Set(sqrtf(~(pos - curPos)));
			return 1;
		}else
		if(stricmp(msg, "InDialog") == 0)
		{
			isDialog = message.Long() != 0;
			return 1;
		}else
		if(stricmp(msg, "SetSex") == 0)
		{
			isMale = message.Long() != 0;
			return 1;
		}else
		if(stricmp(msg, "SetFightWOWeapon") == 0)
		{
			isFightWOWps = message.Long() != 0;
			UpdateWeapons();
			return 1;
		}else
		if(stricmp(msg, "LockFightMode") == 0)
		{
			lockFightMode = message.Long() != 0;
			return 1;
		}else
		if(stricmp(msg, "CheckFightMode") == 0)
		{
			return isFight;
		}else
		if(stricmp(msg, "IsActive") == 0)
		{
			return isActiveState;
		}else
		if(stricmp(msg, "CheckID") == 0)
		{
	#ifdef _DEBUG
	#ifndef _XBOX
			message.String(sizeof(msg), msg); msg[sizeof(msg) - 1] = 0;
			if(AttributesPointer)
			{
				const char * id = AttributesPointer->GetAttribute("id");
				if(id)
				{
					if(strcmp(id, characterID) != 0) _asm int 3;
					if(strcmp(id, msg) != 0) _asm int 3;
				}
			}
			if(strcmp(msg, characterID) != 0) _asm int 3;
	#endif
	#endif
			return 1;
		}else
		if(stricmp(msg,"GunBelt") == 0)
		{
			if( message.Long()!=0 )
				api->Send_Message(blade, "l", MSG_BLADE_GUNBELT);
			else
				api->Send_Message(blade, "l", MSG_BLADE_GUNHAND);
		}
	}
	return 0;
}

bool Character::zPlaySound(MESSAGE & message)
{
	char name[64];
	message.String(sizeof(name), name);
	name[sizeof(name) - 1] = 0;
	return PlaySound(name) != SOUND_INVALID_ID;
}

//�������� ����������� �������
bool Character::TestJump(CVECTOR pos)
{	
	MODEL * jpm = location->JmpPatch();
	if(!jpm) return false;
	CVECTOR src(pos.x, pos.y + 1.0f, pos.z);
	CVECTOR dst(pos.x, pos.y - 1.0f, pos.z);
	if(jpm->Trace(src, dst) >= 1.0f) return false;
	//��������� ���������� �������
	const float speed = 3.3f;
	CVECTOR v(speed*sinf(ay), vy, speed*cosf(ay));
	for(long i = 0; i < CHARACTER_MAXJUMPPOINTS; i++)
	{
		//��������� ������� � �����
		jumpTrack[i] = pos;
		//��������� ��������� ������� 
		CVECTOR dlt = v*CHARACTER_JUMP_TIMESTEP;
		v.y -= 9.8f*CHARACTER_JUMP_TIMESTEP;
		CVECTOR src = pos - dlt*0.001f;
		CVECTOR dst = pos + dlt*1.001f;
		//location->DrawLine(src, 0xff00ff00, dst, 0xff00ff00, false);
		float k = location->GetPtcData().Trace(src, dst);
		if(k < 1.0f)
		{
			//����� ����� �����������
			osculationPoint = src + (dst - src)*k + CVECTOR(0.0f, 0.01f, 0.0f);
			jumpFallTime = (i + k)*CHARACTER_JUMP_TIMESTEP;
			curJumpFallTime = 0.0f;
			jumpPoints = i + 1;
			return true;
		}
		//�������� ����� �������
		pos += dlt;
	}
	return false;
}

bool Character::BuildJump(CVECTOR pos,float fAng)
{
	// ��� ����� ��� �������� - ��� � �������
	MODEL * jpm = location->JmpPatch();
	if( !jpm ) return false;

	CVECTOR src(pos.x, pos.y + 1.0f, pos.z);
	CVECTOR dst(pos.x, pos.y - 1.0f, pos.z);
	float fTrace = location->GetPtcData().Trace(src, dst);
	if( fTrace > 1.0f) return false;
	pos.y = src.y + (dst.y-src.y)*(fTrace - 0.1f);

	//��������� ���������� ������
	const float speed = 3.3f;
	CVECTOR v(speed*sinf(fAng), speed, speed*cosf(fAng)); // �������� ������
	//������ ���� ��� ������
	for(long i = 0; i < CHARACTER_MAXJUMPPOINTS; i++)
	{
		//��������� ������� � �����
		jumpTrack[i] = pos;
		//��������� ��������� ������� 
		CVECTOR dlt = v*CHARACTER_JUMP_TIMESTEP;
		v.y -= 9.8f*CHARACTER_JUMP_TIMESTEP;
		CVECTOR src = pos - dlt*0.001f;
		CVECTOR dst = pos + dlt*1.001f;
		//location->DrawLine(src, 0xff00ff00, dst, 0xff00ff00, false);
		//���� ��������� � ����� ���� ���, �� �� ����� �������
		float k = location->GetPtcData().Trace(src, dst);
		if(k < 1.0f)
		{
			//����� ����� �����������
			osculationPoint = src + (dst - src)*k + CVECTOR(0.0f, 0.01f, 0.0f);
			jumpFallTime = (i + k)*CHARACTER_JUMP_TIMESTEP;
			curJumpFallTime = 0.0f;
			jumpPoints = i + 1;

			//�������� ����������� �� ����� ����
			if( jpm ) {
				dst = src = osculationPoint;
				src.y += 10.f;
				dst.y -= 20.f;
				float fT = jpm->Trace(src,dst);
				//�� �� ������������ �� ��������� ����
				if( fT<1.f )
				{
					CVECTOR vdlt = src + (dst-src)*fT - osculationPoint;
					//��������� �������� ���� ��� ���������� ������
					if( vdlt.GetLength() > 0.1f )
					{
						vdlt *= 1.f / jumpPoints;
						for(long i = 1; i < jumpPoints; i++)
						{
							jumpTrack[i] += vdlt;
						}
					}
				}
			}

			return true;
		}
		if( TraceWithObstacle(src,dst) ) return false;
		//�������� ����� �������
		pos += dlt;
	}
	return false;
}

bool Character::TraceWithObstacle(const CVECTOR& src,const CVECTOR& dst)
{
	if( location->Trace( src, dst ) > 1.f ) return false;
	return true;
}

void Character::UpdateActionsData()
{
	MODEL * m = Model();
	if(!m) return;
	Animation * a = m->GetAnimation();
	if(!a) return;
	//�������� �����������
	turnSpeed = 2.0f;
	const char * s = a->GetData("Turn speed");
	if(s && s[0]) turnSpeed = float(atof(s));
	UpdateActionMoveData(walk, a);
	UpdateActionMoveData(backwalk, a);
	UpdateActionMoveData(run, a);
	UpdateActionMoveData(backrun, a);	
	UpdateActionMoveData(stsUp, a);
	UpdateActionMoveData(stsDown, a);
	UpdateActionMoveData(stsUpBack, a);
	UpdateActionMoveData(stsDownBack, a);
	UpdateActionMoveData(stsUpRun, a);
	UpdateActionMoveData(stsDownRun, a);
	UpdateActionMoveData(stsUpRunBack, a);
	UpdateActionMoveData(stsDownRunBack, a);
	UpdateActionMoveData(swim, a);
	UpdateActionMoveData(fall, a);
	UpdateActionMoveData(fall_land, a);
	UpdateActionMoveData(fall_water, a);
	//������
	for(long i = 0; i < numActionDead; i++) UpdateActionDeadData(actionDead[i], a);
	for(i = 0; i < numActionFightDead; i++) UpdateActionDeadData(actionFightDead[i], a);
	//���
	UpdateActionMoveData(fightwalk, a);
	UpdateActionMoveData(fightbackwalk, a);
	UpdateActionMoveData(fightrun, a);
	UpdateActionMoveData(fightbackrun, a); 
	for(i = 0; i < numAttackFast; i++) UpdateActionCharacterData(attackFast[i], a);
	for(i = 0; i < numAttackForce; i++) UpdateActionCharacterData(attackForce[i], a);
	for(i = 0; i < numAttackRound; i++) UpdateActionCharacterData(attackRound[i], a);
	for(i = 0; i < numAttackBreak; i++) UpdateActionCharacterData(attackBreak[i], a);
	for(i = 0; i < numAttackFeint; i++) UpdateActionCharacterData(attackFeint[i], a);
	for(i = 0; i < numAttackFeint; i++) UpdateActionCharacterData(attackFeintC[i], a);
	UpdateActionCharacterData(shot, a);
	for(i = 0; i < numHits; i++) UpdateActionCharacterData(hit[i], a);
	for(i = 0; i < numParry; i++) UpdateActionCharacterData(parry[i], a);  //eddy
	UpdateActionCharacterData(hitFeint, a);
	UpdateActionCharacterData(hitParry, a);
	UpdateActionCharacterData(hitRound, a);
	UpdateActionCharacterData(hitFire, a);
	UpdateActionCharacterData(block, a);
	UpdateActionCharacterData(blockaxe, a);
	UpdateActionCharacterData(blockhit, a);
	UpdateActionCharacterData(blockaxehit, a);
	UpdateActionCharacterData(blockbreak, a);
	UpdateActionCharacterData(recoil, a);
	//�������� �������� �� �����
	UpdateActionCharacterData(actionTurnL, a);
	UpdateActionCharacterData(actionTurnR, a);
	//�������� ������� ��� �������
	for(i = 0; i < numActionIdles; i++) UpdateActionIdleData(actionIdle[i], a);
	//�������� ������� � ������ ���
	for(i = 0; i < numFightActionIdles; i++) UpdateActionIdleData(actionFightIdle[i], a);
	noBlendTime = 1.0f;
	UpdateAnimation();
}

void Character::UpdateActionMoveData(ActionMove & am, Animation * a)
{	
	const char * s;
	if(!a) return;
	//set player action
	if(!a->Player(1).SetAction(am.name)) return;
	//speed
	am.speed = 0.0f;
	s = a->Player(1).GetData("Walk speed");
	if(s && s[0]) am.speed = float(atof(s));
	//Blend time
	am.tblend = 0.2f;
	s = a->Player(1).GetData("Blend time");
	if(s && s[0]) am.tblend = float(atof(s));
	if(am.tblend < 0.0f) am.tblend = 0.0f;
	//Turn speed
	am.turnspd = turnSpeed;
	s = a->Player(1).GetData("Turn speed");
	if(s && s[0]) am.turnspd = float(atof(s));
	//reset player
	a->Player(1).SetAction(null);
}

void Character::UpdateActionCharacterData(ActionCharacter & ac, Animation * a)
{
	const char * s;
	if(!a) return;
	//set player action
	if(!a->Player(2).SetAction(ac.name)) return;
	//Blend time
	ac.tblend = 0.0f;
	s = a->Player(2).GetData("Blend time");
	if(s && s[0]) ac.tblend = float(atof(s));
	if(ac.tblend < 0.0f) ac.tblend = 0.0f;
	//reset player
	a->Player(2).SetAction(null);
}

void Character::UpdateActionIdleData(ActionIdle & ai, Animation * a)
{
	const char * s;
	if(!a) return;
	//set player action
	if(!a->Player(2).SetAction(ai.name)) return;
	//Blend time
	ai.tblend = 0.0f;
	s = a->Player(2).GetData("Blend time");
	if(s && s[0]) ai.tblend = float(atof(s));
	if(ai.tblend < 0.0f) ai.tblend = 0.0f;
	//�����������
	ai.p = 1.0f;
	s = a->Player(2).GetData("Probability");
	if(s && s[0]) ai.p = float(atof(s));
	if(ai.p > 1.0f) ai.p = 1.0f;
	if(ai.p < 0.0f) ai.p = 0.0f;
	//reset player
	a->Player(2).SetAction(null);
}

void Character::UpdateActionDeadData(ActionDead & ai, Animation * a)
{
	const char * s;
	if(!a) return;
	//set player action
	if(!a->Player(2).SetAction(ai.name)) return;
	//Blend time
	ai.tblend = 0.0f;
	s = a->Player(2).GetData("Blend time");
	if(s && s[0]) ai.tblend = float(atof(s));
	if(ai.tblend < 0.0f) ai.tblend = 0.0f;
	//�����������
	ai.p = 1.0f;
	s = a->Player(2).GetData("Probability");
	if(s && s[0]) ai.p = float(atof(s));
	if(ai.p > 1.0f) ai.p = 1.0f;
	if(ai.p < 0.0f) ai.p = 0.0f;
	//s
	ai.ang = 0.0f;
	s = a->Player(2).GetData("Angle");
	if(s && s[0]) ai.ang = float(atof(s));
	if(ai.ang > 360.0f) ai.ang = 360.0f;
	if(ai.ang < 0.0f) ai.ang = 0.0f;
	ai.ang *= 3.141592654f/180.0f;
	//reset player
	a->Player(2).SetAction(null);
}


bool Character::SetAction(const char * actionName, float tblend, float movespeed, float trnspeed, bool forceStart)
{
	MODEL * m = Model();
	if(!m) return false;
	Animation * a = m->GetAnimation();
	if(!a) return false;
	const char * curAction = "";
	if(a->Player(0).IsPlaying() && !a->Player(0).IsPause())
	{
		curAction = a->Player(0).GetAction();
		if(!forceStart && curAction && actionName && stricmp(curAction, actionName) == 0) return true;
	}
	if(noBlendTime > 0.0f) tblend = 0.0f;
	bspeed.Blend(speed, movespeed, tblend);
	bturn.Blend(turnspd, trnspeed, tblend*0.1f);
	if(!actionName || !actionName[0])
	{
		if(tblend <= 0.0f)
		{
			a->Player(0).Stop();
			a->Player(0).SetAction(null);
			a->Player(1).Stop();
			a->Player(1).SetAction(null);
			return true;
		}
		if(a->Player(1).IsPlaying())
		{
			a->Player(1).Stop();
			a->Player(1).SetAction(null);
		}
		if(!a->Player(0).IsPlaying()) return true;
		float start = 0.0f;
		if(a->Timer(0).IsWork())
		{
			start = (1.0f - a->Timer(0).GetCurrent())*tblend;
			Assert(0.0f <= start && start <= tblend);
		}
		if(start < tblend)
		{
			a->Timer(0).ResetTimer();
			a->Timer(0).Start(tblend, start);
			a->Timer(0).SetPlayer(0, true);
			a->Player(0).SetAutoStop(true);			
		}else{
			a->Player(0).Stop();
			a->Player(0).SetAction(null);
		}
		return true;
	}
	//������� ����������� ����
	api->Send_Message(blade, "l", 1003);
	//������ �������� ������
	if(a->Player(0).IsPlaying()) a->CopyPlayerState(0, 1);
	a->Player(0).SetAction(null);
	bool res = a->Player(0).SetAction(actionName) && a->Player(0).Play();
	a->Timer(0).ResetTimer();
	a->Timer(1).ResetTimer();
	if(tblend)
	{	
		a->Timer(0).Start(tblend);
		a->Player(0).SetAutoStop(false);
		a->Player(1).SetAutoStop(true);
		a->Timer(0).SetPlayer(0, false);
		a->Timer(0).SetPlayer(1, true);
	}else{
		a->Player(1).Stop();
		a->Player(1).SetAction(null);
	}
	return res;
}

void Character::UpdateAnimation()
{
	isActiveState = true;
	if(priorityAction.name)
	{
		curIdleIndex = -1;
		//����������� ��������
		if(!isSetPriorityAction)
		{
			isSetPriorityAction = true;
			if(!SetAction(priorityAction.name, priorityAction.tblend, priorityActionMoveSpd, priorityActionRotSpd))
			{
				api->Trace("Character animation: not set priority action: \"%s\"", priorityAction.name);
			}
			curMove = null;
			fgtCurType = fgtSetType = fgt_none;
			fgtSetIndex = -1;
		}
	}else
	if(isSwim)
	{
		if(!SetAction(swim.name, swim.tblend, swim.speed, swim.turnspd))
		{
			api->Trace("Character animation: not set priority action: \"%s\"", swim.name);
		}
		curMove = null;
		fgtCurType = fgtSetType = fgt_none;
		fgtSetIndex = -1;
	}else{
		isSetPriorityAction = false;
		if(!isFight)
		{
			if(isNFHit)
			{				
				isNFHit = false;
				curMove = null;
				if (userIdle.name && (stricmp(userIdle.name, "Ground_SitDown")==0 || stricmp(userIdle.name, "Ground_StandUp")==0))
				{
					api->Trace("Not int: \"%s\"", userIdle.name);
				}
				else
				{
					if(!SetAction(nfhit.name, nfhit.tblend, 0.0f, 0.0f))
					{
						curIdleIndex = -1;
						if(noBlendTime <= 0.0f) api->Trace("Character animation: not set non fight hit action: \"%s\"", nfhit.name);
					}
				}
			}else
			if(isJump)
			{
				if(!SetAction(fall.name, fall.tblend, 0.0f, 0.0f))
				{
					api->Trace("Character animation: not fall action: \"%s\"", fall.name);
				}
			}else
			if(isMove)
			{
				curIdleIndex = -1;
				//_CORE_API->Trace("movesn = %f", movesn);
				if(movecs > CHARACTER_COS_STAIR || IsRun())
				{
					if(!IsRun())
					{
						if(!isBack)
						{
							//������� ������
							if(curMove != &walk)
							{
								curMove = &walk;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set walk action: \"%s\"", curMove->name);
								}
							}
						}else{
							//������ �����
							if(curMove != &backwalk)
							{
								curMove = &backwalk;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set back walk action: \"%s\"", curMove->name);
								}
							}
						}
					}else{
						if(!isBack)
						{
							//���
							if(curMove != &run)
							{
								curMove = &run;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set run action: \"%s\"", curMove->name);
								}
							}
						}else{
							//��� �����
							if(curMove != &backrun)
							{
								curMove = &backrun;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set buck run action: \"%s\"", curMove->name);
								}
							}
						}
					}
				}else{
					if(!IsRun() || true)
					{
						if(!isBack)
						{
							//������ �� ��������
							if(isUp)
							{
								curMove = &stsUp;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set stair up action: \"%s\"", curMove->name);
								}
							}else{
								curMove = &stsDown;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set stair down action: \"%s\"", curMove->name);
								}
							}
						}else{
							//������ ����� �� ��������
							if(isUp)
							{
								curMove = &stsUpBack;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set back stair up action: \"%s\"", curMove->name);
								}
							}else{
								curMove = &stsDownBack;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set back stair down action: \"%s\"", curMove->name);
								}
							}
						}
					}else{
						Assert(false);
						if(!isBack)
						{
							//��� �� �������� �����
							if(isUp)
							{
								curMove = &stsUpRun;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set run stair up action: \"%s\"", curMove->name);
								}
							}else{
								curMove = &stsDownRun;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set run stair down action: \"%s\"", curMove->name);
								}
							}
						}else{
							//��� �� �������� �����
							if(isUp)
							{
								curMove = &stsUpRunBack;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set back run stair up action: \"%s\"", curMove->name);
								}
							}else{
								curMove = &stsDownRunBack;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set back run stair down action: \"%s\"", curMove->name);
								}
							}
						}
					}
				}
			}else{
				float ts;
				const char * an;
				//�������� ������� �� ���������
				if(!curMove)
				{
					if(fabsf(strafeVel) < 0.1f)
					{
						//�������
						an = FindIdleAnimation(ts);
						if(an)
						{
							if(!SetAction(an, ts, 0.0f, turnSpeed))
							{
								curIdleIndex = -1;
								if(noBlendTime <= 0.0f) api->Trace("Character animation: not set idle action: \"%s\"", an);
							}
						}else{
							_CORE_API->Trace("Character: No set idle animation!!!");
							if(noBlendTime <= 0.0f) SetAction(null, 0.3f, 0.0f, turnSpeed);
						}
					}else{
						//������ � ������
						if(strafeVel < 0.0f)
						{
							curIdleIndex = -1;
							if(!SetAction("strafe_left", 0.2f, 0.0f, turnSpeed))
							{
								if(noBlendTime <= 0.0f) api->Trace("Character animation: not set \"strafe_left\" action");
							}
						}else{
							curIdleIndex = -1;
							if(!SetAction("strafe_right", 0.2f, 0.0f, turnSpeed))
							{
								if(noBlendTime <= 0.0f) api->Trace("Character animation: not set \"strafe_right\" action");
							}
						}
					}
				}else{
					//������������
					an = FindIdleAnimation(ts);
					if(an)
					{
						if(!SetAction(an, ts, 0.0f, turnSpeed))
						{
							curIdleIndex = -1;
							if(noBlendTime <= 0.0f) api->Trace("Character animation: not set idle action: \"%s\"", an);
						}
					}else{
						SetAction(null, curMove->tblend, 0.0f, turnSpeed);
						curIdleIndex = -1;
						if(noBlendTime <= 0.0f) _CORE_API->Trace("Character: No set idle animation!!!");
					}
					curMove = null;
				}
			}
		}else{
			//����� ���

			if(fgtSetType == fgt_blockbreak)
			{
				int i = 0;
			}

			if(!fightTbl[fgtCurType][fgtSetType]) return;
			bool isSet = false;
			if(fgtSetType != fgt_none)
			{
				isParryState = false;
				isFeintState = false;
/*
				if(stricmp(characterID, "Blaze") == 0)
				{
					ENTITY_ID eid;
					api->FindClass(&eid, "ILogAndActions", 0);
					char sbuf[256];
					sprintf(sbuf, "Set act: %s", fightNamesTbl[fgtSetType]);
					api->Send_Message(eid, "lls", 45020, false, sbuf);
					if(fgtCurType == fgt_blockhit && fgtSetType == fgt_blockhit)
					{
						int i = 0;
					}
				}
 */
				char* pWeaponID;
				VDATA* pdat = api->Event("eGetWeaponID","s",characterID);
				if( pdat )
				{
					pWeaponID = pdat->GetString();
				}

				switch(fgtSetType)
				{
				case fgt_attack_fast:	//������� ����
					if(!(isSet = SetAction(attackFast[fgtSetIndex].name, attackFast[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set fast attack action: \"%s\"", attackFast[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "fast");
						// boal ������� � ������ ����� api->Event("ChrFgtActApply", "is", GetID(), FGT_ATTACK_FAST);
						camRotWait = camRotMax = 0.3f;
						impulse += 5.0f*GetEnemyDirForImpulse();
					}
					break;
				case fgt_attack_force:	//����� � ����� � ������� ������
					if(stricmp(pWeaponID, "topor") == 0 && fgtSetIndex == 3)
					{	//���� � �������, �� ����� ������ �� ������� ����						
						fgtSetIndex = rand() % 2; 
					}
					if(!(isSet = SetAction(attackForce[fgtSetIndex].name, attackForce[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set force attack action: \"%s\"", attackForce[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "force");
						// boal ������� � ������ ����� api->Event("ChrFgtActApply", "is", GetID(), FGT_ATTACK_FORCE);
						camRotWait = camRotMax = 0.3f;
						impulse += 2.5f*GetEnemyDirForImpulse();
					}
					break;
				case fgt_attack_round:	//�������� ����
					if(!(isSet = SetAction(attackRound[fgtSetIndex].name, attackRound[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set round attack action: \"%s\"", attackRound[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "round");
						// boal ������� � ������ ����� api->Event("ChrFgtActApply", "is", GetID(), FGT_ATTACK_ROUND);
						camRotWait = camRotMax = 0.8f;
					}
					break;
				case fgt_attack_break:	//����������� ����
					if(!(isSet = SetAction(attackBreak[fgtSetIndex].name, attackBreak[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set break attack action: \"%s\"", attackBreak[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "break");
						// boal ������� � ������ ����� api->Event("ChrFgtActApply", "is", GetID(), FGT_ATTACK_BREAK);
						camRotWait = camRotMax = 0.3f;
						impulse += 1.0f*GetEnemyDirForImpulse();
					}
					break;
				case fgt_attack_feint:	//���� - ������������ �������� ����
					if(!(isSet = SetAction(attackFeint[fgtSetIndex].name, attackFeint[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set feint action: \"%s\"", attackFeint[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "feint");
						api->Event("ChrFgtActApply", "is", GetID(), FGT_ATTACK_FEINT);
						camRotWait = camRotMax = 0.3f;
					}
					break;
				case fgt_attack_feintc:	//��������� ����������� �����
					if(!(isSet = SetAction(attackFeintC[fgtSetIndex].name, attackFeintC[fgtSetIndex].tblend, 0.0f, 4.0f)))
					{
						api->Trace("Character animation: not set feint action: \"%s\"", attackFeintC[fgtSetIndex].name);
					}else{
						_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_ON,0);
						api->Event("Event_ChrSnd_Attack", "is", GetID(), "feintc");
						api->Event("ChrFgtActApply", "is", GetID(), "feintc"); // boal ���� ������� �� �����
						camRotWait = camRotMax = 0.3f;
					}
					break;
				case fgt_fire:			//������� �� ���������
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					priorityAction.SetName(shot.name);
					priorityAction.tblend = shot.tblend;
					priorityActionMoveSpd = 0.0f;
					priorityActionRotSpd = 3.0f;
					fgtCurType = fgtSetType = fgt_none;					
					fgtSetIndex = -1;					
					isFired = false;
					break;
				case fgt_hit_attack:	//������� ��������� ����� �� ��������� �������� ��� � stall
                    if(stricmp(characterID, "Blaze") == 0) // boal �� ����� �������, �� � ��� �� ������ ����� ��� ��, �������� �����
                    {
						if (rand() % 100 >= 50) break; // boal �� ������ ����������� � ��������
					}
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(hit[fgtSetIndex].name, hit[fgtSetIndex].tblend, 0.0f, 1.0f, true)))
					{
						api->Trace("Character animation: not set fight attack hit action: \"%s\"", hit[fgtSetIndex].name);
					}
					break;
				case fgt_blockbreak:	//������� ��������� ����� �� ��������� �������� ��� � stall
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(blockbreak.name, blockbreak.tblend, 0.0f, 1.0f, true)))
					{
						api->Trace("Character animation: not set fight blockbreak action: \"%s\"", blockbreak.name);
					}
					break;
				case fgt_hit_feint:		//������� �� ����� �������� ��� � stall
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(hitFeint.name, hitFeint.tblend, 0.0f, 0.0f, true)))
					{
						api->Trace("Character animation: not set fight feint hit action: \"%s\"", hitFeint.name);
					}
					break;
				case fgt_hit_parry:		//������� �� ����������� �������� ��� � stall
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(hitParry.name, hitParry.tblend, 0.0f, 0.0f, true)))
					{
						api->Trace("Character animation: not set fight parry hit action: \"%s\"", hitParry.name);
					}
					api->Event("ChrFgtActApply", "is", GetID(), "hit_parry"); // boal ��� ���� FGT_ATTACK_PARRY);
					break;
				case fgt_hit_round:		//������� ������������ �������� ������
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					impulse.x -= 3.0f*sinf(ay);
					impulse.z -= 3.0f*cosf(ay);
					if(!(isSet = SetAction(hitRound.name, hitRound.tblend, 0.0f, 0.0f, true)))
					{
						api->Trace("Character animation: not set fight round hit action: \"%s\"", hitRound.name);
					}
					break;
				case fgt_hit_fire:		//������� �� �������� �������� ��� � stall
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(hitFire.name, hitFire.tblend, 0.0f, 0.0f, true)))
					{
						api->Trace("Character animation: not set fight fire hit action: \"%s\"", hitFire.name);
					}
					break;
				case fgt_block:			//������ ������		
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(stricmp(pWeaponID, "topor") == 0)
					{
						if(!(isSet = SetAction(blockaxe.name, blockaxe.tblend, 0.0f, 5.0f, true)))
						{
							api->Trace("Character animation: not set block action: \"%s\"", blockaxe.name);
						}
					}
					else
					{
						if(!(isSet = SetAction(block.name, block.tblend, 0.0f, 5.0f, true)))
						{
							api->Trace("Character animation: not set block action: \"%s\"", block.name);
						}
					}
					break;
				case fgt_blockhit:		//������ ������
                    if(stricmp(characterID, "Blaze") == 0) // boal �� ����� �������, �� � ��� �� ������ ����� ��� ��, �������� �����
                    {
						if (rand() % 100 >= 65) break; // boal �� ������ ����������� � ��������
					}
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(stricmp(pWeaponID, "topor") == 0)
					{
						if(!(isSet = SetAction(blockaxehit.name, blockaxehit.tblend, 0.0f, 2.0f, true)))
						{
							api->Trace("Character animation: not set block axe hit action: \"%s\"", blockaxehit.name);
						}
					}
					else
					{
						if(!(isSet = SetAction(blockhit.name, blockhit.tblend, 0.0f, 2.0f, true)))
						{
							api->Trace("Character animation: not set block hit action: \"%s\"", blockhit.name);
						}
					}
					break;
				case fgt_parry:			//�����������, �������� �������� �������� ���������� � stall
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					if(!(isSet = SetAction(parry[fgtSetIndex].name, parry[fgtSetIndex].tblend, 0.0f, 5.0f)))
					{
						api->Trace("Character animation: not set block(parry) action: \"%s\"", parry[fgtSetIndex].name);
					}
					break;
				case fgt_recoil:		//������ �����
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					PlaySound("recoil", true);
					impulse.x -= 2.0f*sinf(ay);
					impulse.z -= 2.0f*cosf(ay);
					priorityAction.SetName(recoil.name); //��� �������� ��������� ��������
					if(!(isSet = SetAction(recoil.name, recoil.tblend, -3.0f, 0.0f)))
					{
						api->Trace("Character animation: not set recoil action: \"%s\"", recoil.name);
					}
					break;
				case fgt_strafe_l:		//������ �����
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					recoilSound = SOUND_INVALID_ID;//PlaySound("recoil", true);
					impulse += 15.0f*CVECTOR(-cosf(ay), 0.0f, sinf(ay));
					if(!(isSet = SetAction(strafe_l.name, strafe_l.tblend, 0.0f, 0.0f)))
					{
						api->Trace("Character animation: not set recoil action: \"%s\"", strafe_l.name);
					}
					break;
				case fgt_strafe_r:		//������ �����
					_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
					recoilSound = SOUND_INVALID_ID;//PlaySound("recoil", true);
					impulse -= 15.0f*CVECTOR(-cosf(ay), 0.0f, sinf(ay));
					if(!(isSet = SetAction(strafe_r.name, strafe_r.tblend, 0.0f, 0.0f)))
					{
						api->Trace("Character animation: not set recoil action: \"%s\"", strafe_l.name);
					}
					break;
				}
				if(isSet)
				{
					fgtCurType = fgtSetType;					
					fgtCurIndex = fgtSetIndex;
				}
				fgtSetType = fgt_none;
				fgtSetIndex = -1;
				curMove = null;
				curIdleIndex = -1;
			}else{
				_CORE_API->Send_Message(blade, "ll", MSG_BLADE_TRACE_OFF,0);
				if(isMove)
				{
					curIdleIndex = -1;
					if(!IsRun())
					{
						if(!isBack)
						{
							//������ �����
							if(curMove != &fightwalk)
							{
								curMove = &fightwalk;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set fight walk action: \"%s\"", curMove->name);
								}
							}
						}else{
							//������ �����
							if(curMove != &fightbackwalk)
							{
								curMove = &fightbackwalk;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set fight back walk action: \"%s\"", curMove->name);
								}
							}
						}
					}else{
						if(!isBack)
						{
							//��� �����
							if(curMove != &fightrun)
							{
								curMove = &fightrun;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set fight run action: \"%s\"", curMove->name);
								}
							}
						}else{
							//��� �����
							if(curMove != &fightbackrun)
							{
								curMove = &fightbackrun;
								if(!SetAction(curMove->name, curMove->tblend, curMove->speed, curMove->turnspd))
								{
									api->Trace("Character animation: not set fight back run action: \"%s\"", curMove->name);
								}
							}
						}
					}
				}else{
					float ts;
					const char * an;
					//�������� ������� �� ���������
					if(!curMove)
					{
						//�������
						an = FindFightIdleAnimation(ts);
						if(an)
						{
							if(!SetAction(an, ts, 0.0f, turnSpeed))
							{
								curIdleIndex = -1;
								if(noBlendTime <= 0.0f) api->Trace("Character animation: not set fight idle \"%s\" action", an);
							}
						}else{
							if(noBlendTime <= 0.0f) _CORE_API->Trace("Character: No set idle animation!!!");
							SetAction(null, 0.3f, 0.0f, turnSpeed);
							curIdleIndex = -1;
						}
					}else{
						//������������
						an = FindFightIdleAnimation(ts);
						if(an)
						{
							if(!SetAction(an, ts, 0.0f, turnSpeed))
							{
								curIdleIndex = -1;
								if(noBlendTime <= 0.0f) api->Trace("Character animation: not set fight idle \"%s\" action", an);
							}
						}else{
							SetAction(null, curMove->tblend, 0.0f, turnSpeed);
							if(noBlendTime <= 0.0f) _CORE_API->Trace("Character: No set idle animation!!!");
							curIdleIndex = -1;
						}
						curMove = null;
					}			
				}
			}
		}
	}

}

const char * Character::FindIdleAnimation(float & tblend)
{
	//���������������� ��������
	if(userIdle.name)
	{
		curIdleIndex = -1;
		tblend = userIdle.tblend;
		return userIdle.name;
	}
	//����������� ��������
	if(fabsf(isTurn) <= (curIdleIndex >= 0 ? 0.8f : 0.1f))
	{
		if(numActionIdles <= 0)
		{
			tblend = 0.0f;
			return null;
		}
		if(!isLockIdleForCamera)
		{		
			if(curIdleIndex < 0)
			{
				//����������� ������� ����������� � �������� �����������
				float allp = 0.0f;
				for(long i = 0; i < numActionIdles; i++) allp += actionIdle[i].p;
				//������� ��������
				float rnd = rand()*allp/RAND_MAX;
				//�������� �� ������ �� ������� ����� �����������
				allp = 0.0f;
				for(i = 0; i < numActionIdles; i++)
				{
					allp += actionIdle[i].p;
					if(rnd < allp) break;
				}
				if(i >= numActionIdles) i = numActionIdles - 1;			
				curIdleIndex = i;
			}
		}else curIdleIndex = 0;
		tblend = actionIdle[curIdleIndex].tblend;
		isActiveState = false;
		return actionIdle[curIdleIndex].name;
	}else{
		curIdleIndex = -1;
		//�������
		if(isTurn < 0)
		{
			tblend = actionTurnL.tblend;
			return actionTurnL.name;
		}else{
			tblend = actionTurnR.tblend;
			return actionTurnR.name;
		}
	}
	return null;
}

const char * Character::FindFightIdleAnimation(float & tblend)
{
	//���������������� ��������
	if(userIdle.name)
	{
		curIdleIndex = -1;
		tblend = userIdle.tblend;
		return userIdle.name;
	}
	if(curIdleIndex < 0 || curIdleIndex >= numFightActionIdles)
	{
		//����������� ������� ����������� � �������� �����������
		float allp = 0.0f;
		for(long i = 0; i < numFightActionIdles; i++) allp += actionFightIdle[i].p;
		//������� ��������
		float rnd = rand()*allp/RAND_MAX;
		//�������� �� ������ �� ������� ����� �����������
		allp = 0.0f;
		for(i = 0; i < numFightActionIdles; i++)
		{
			allp += actionFightIdle[i].p;
			if(rnd < allp) break;
		}
		if(i >= numFightActionIdles) i = numFightActionIdles - 1;
		if(recoilLook) 
		{
			curIdleIndex = 7; //��������� ��������� ����� �������
			recoilLook = false;
		}
		else curIdleIndex = i;
	}
	if(fabsf(isTurn) <= (curIdleIndex >= 0 ? 0.8f : 0.1f)) isActiveState = false;
	tblend = actionFightIdle[curIdleIndex].tblend;
	return actionFightIdle[curIdleIndex].name;
	/*		
	//����������� ��������
	if(fabsf(isTurn) <= 0.0001f)
	{
		tblend = actionFightIdle[curIdleIndex].tblend;
		return actionFightIdle[curIdleIndex].name;
	}else{
		//�������
		if(isTurn < 0)
		{
			tblend = actionFightIdle[curIdleIndex].tblend;
			return actionFightIdle[curIdleIndex].name;
		}else{
			tblend = actionFightIdle[curIdleIndex].tblend;
			return actionFightIdle[curIdleIndex].name;
		}
	}
	return null;
	*/
}

float Character::GetCurrentPlayerPos()
{
	MODEL * m = Model();
	if(!m) return 1.0f;
	Animation * a = m->GetAnimation();
	if(!a) return 1.0f;
	float p = a->Player(0).GetPosition();
	if(!a->Player(0).IsPlaying()) p = 1.0f;
	return p;
}

float Character::GetAniPlayTime()
{
	MODEL * m = Model();
	if(!m) return 1.0f;
	Animation * a = m->GetAnimation();
	if(!a) return 1.0f;
	float p = a->Player(0).GetPosition();
	if(!a->Player(0).IsPlaying()) p = 1.0f;
	p = 1.0f - p;
	p *= a->Player(0).GetFrames()*(1.0f/15.0f);
	return p;
}

Character * Character::FindDialogCharacter()
{
	if(IsFight() || liveValue < 0 || deadName) return null;
	//����� ���������� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	static long num = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, num, this, 3.0f)) return null;
	//�������� �������
	float minDst;
	for(long i = 0, j = -1; i < num; i++)
	{
		//��������
		Supervisor::FindCharacter & fc = fndCharacter[i];
		//���� �����, �� �� ������� � ���
		if(fc.c->IsFight() || fc.c->liveValue < 0 || fc.c->deadName) continue;
		//���������
		if(fabsf(fc.dy) > 2.0f) continue;
		float d = sqrtf(fc.d2);
		//����
		static const float testAng = cosf(0.5f*CHARACTER_DIALOG_ANG*(3.1415926535f/180.0f));		
		float cs = -1.0f;		
		if(fc.d2 > 0.0f) cs = (fc.dx*sinf(ay) + fc.dz*cosf(ay))/d;
		if(cs < testAng) continue;
		//��������� � ������ ����
		d /= cs;
		if(j >= 0)
		{
			if(minDst > d)
			{
				minDst = d;
				j = i;
			}
		}else{
			minDst = d;
			j = i;
		}
	}
	if(j >= 0) return fndCharacter[j].c;
	return null;
}

bool Character::SetPriorityAction(const char * action)
{
	priorityAction.SetName(null);
	if(!action) return false;
	MODEL * m = Model();
	if(!m) return false;
	Animation * a = m->GetAnimation();
	if(!a) return false;
	isResetAutoAction = true;	
	priorityAction.SetName(action);
	priorityAction.tblend = 0.0f;
	priorityActionMoveSpd = 0.0f;
	priorityActionRotSpd = 0.0f;
	if(!a->Player(1).SetAction(action))
	{
		a->Player(1).SetAction(null);
		priorityAction.SetName(null);
		return false;
	}
	UpdateActionCharacterData(priorityAction, a);
	isSetPriorityAction = false;
	return true;
}

inline void Character::CheckAttackHit()
{
	//������� �������� ����� � �������
	bool isBlockBreak = false;
	const char * aname;
	FightAction hitReaction;
	switch(fgtCurType)
	{
	case fgt_attack_fast:
		aname = FGT_ATTACK_FAST;
		hitReaction = fgt_hit_attack;
		break;
	case fgt_attack_force:
		aname = FGT_ATTACK_FORCE;
		hitReaction = fgt_hit_attack;
		break;
	case fgt_attack_round:
		aname = FGT_ATTACK_ROUND;
		hitReaction = fgt_hit_round;
		break;
	case fgt_attack_break:
		aname = FGT_ATTACK_BREAK;
		hitReaction = fgt_blockbreak;
		isBlockBreak = true;
		break;
	case fgt_attack_feintc:
		aname = FGT_ATTACK_FEINT;
		hitReaction = fgt_hit_feint;
		break;
	/*case fgt_attack_feint: //boal ��� ����� ������� � �����
		aname = FGT_ATTACK_FEINT;
		hitReaction = fgt_hit_feint;
		break;*/
	default:
		return;
	}
	float attackDist = 2.4f, attackAng = (fgtCurType != fgt_attack_round ? 60.0f : 0.0f);
	MODEL * m = Model();
	if(m)
	{
		Animation * a = m->GetAnimation();
		if(a)
		{
			const char * s = a->Player(0).GetData("Dist");
			if(s && s[0]) attackDist = float(atof(s));
			s = a->Player(0).GetData("Ang");
			if(s && s[0]) attackAng = float(atof(s));
		}
	}
	//����� ���������� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	long num = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, num, this, attackDist, attackAng, 0.1f, 0.0f, false, true)) return;
	//���������� ���� ������
	bool isParry = false;
	bool isHrrrSound = true;
	bool isUseEnergy = true; // ����� ������� ���� ��� boal
	for(long i = 0; i < num; i++)
	{
		//��������
		Supervisor::FindCharacter & fc = fndCharacter[i];
		if(fc.c->liveValue < 0 || fc.c->deadName || fc.d2 <= 0.0f) continue;

		if(fc.c->fgtCurType == fgt_parry && fc.c->isParryState)
		{
			isParry = true;
			//fc.c->PlaySound("fgt_parry");
		}else
		if(fc.c->fgtCurType == fgt_attack_feint && fc.c->isFeintState)
		{
			// boal ��� ���, ��� ������ ��������, ����� ������ ��������� ����� �����  -->
			//fc.c->StopFightAnimation();
			//StopFightAnimation(); //������ ������ ���
			// boal <--
			fc.c->Attack(this, fgt_attack_feintc);
			fc.c->UpdateAnimation(); // boal
			//fc.c->PlaySound("fgt_feint");
		}else{
			bool isBlocked = (fc.c->fgtCurType == fgt_block || fc.c->fgtCurType == fgt_blockhit);
			if(isBlockBreak) isBlocked = false;
			if(isBlocked)
			{
				//fc.c->PlaySound("fgt_block");
			}else{
				if(fc.c->fgtCurType >= fgt_attack_fast && fc.c->fgtCurType <= fgt_attack_feintc)
				{
					fc.c->PlaySound("fgt_collision");
				}else{
					//fc.c->PlaySound("fgt_blockbreak");
				}
				//fc.c->PlaySound("fgt_inbody");
				if(isHrrrSound && (rand() & 3))
				{
					isHrrrSound = false;
					api->Event("Event_ChrSnd_Hit", "i", fc.c->GetID());
				}
			}
			fc.c->Hit(hitReaction);
			_CORE_API->Event("Location_CharacterAttack", "iisl", GetID(), fc.c->GetID(), aname, (long)isBlocked);
			// boal 12.09.06 ����� ������� �� ����� ����� -->
			if (isUseEnergy && fgtCurType != fgt_attack_feintc)
			{// ��� fgt_attack_feintc ���� ����� � ��������, � ��� ����� "����", � �� ����� 0
				api->Event("ChrFgtActApply", "is", GetID(), aname); 
				isUseEnergy = false;
			}
			// boal <--				 
		}
	}
	if(isParry)
	{
		Hit(fgt_hit_parry);
	}
}

//����� ��������� � �������� ������ �� ���������
Character * Character::FindGunTarget(float & kDist, bool bOnlyEnemyTest)
{
	ENTITY_ID grps;
	CharactersGroups * chrGroup;
	long grp;
	if (bOnlyEnemyTest)
	{
		api->FindClass(&grps, "CharactersGroups", 0);
		chrGroup = (CharactersGroups *)api->GetEntityPointer(&grps);
		grp = chrGroup->FindGroupIndex(group);
	}

	//����� ���������� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	static long num = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, num, this, CHARACTER_FIGHT_FIREDIST, CHARACTER_FIGHT_FIREANG, 0.4f, 30.0f, false)) return null;
	float minDst;
	for(long i = 0, j = -1; i < num; i++)
	{
		Supervisor::FindCharacter & fc = fndCharacter[i];
		if(fc.d2 <= 0.0f || fc.c->radius <= 0.0f) continue;
		//���������� ���������
		if(!VisibleTest(fc.c)) continue;
		if(bOnlyEnemyTest)
		{
			long enemygrpIndex = chrGroup->FindGroupIndex(fc.c->group);
			if (chrGroup->FindRelation(enemygrpIndex, grp).curState != CharactersGroups::rs_enemy) continue;
		}
		//���� ��������� � ����������
		fc.d2 = sqrtf(fc.d2);
		float cs = (fc.dx*sinf(ay) + fc.dz*cosf(ay))/fc.d2;
		cs *= cs*cs;
		if(cs < 0.1f) cs = 0.1f;
		float dist = fc.d2/cs;
		if(j >= 0)
		{
			if(minDst > dist)
			{
				minDst = dist;
				j = i;
			}
		}else{
			minDst = dist;
			j = i;
		}		
	}
	if(j >= 0)
	{
		kDist = 1.0f - fndCharacter[j].d2/CHARACTER_FIGHT_FIREDIST;
		if(kDist < 0.0f) kDist = 0.0f;
		if(kDist > 1.0f) kDist = 1.0f;
		return fndCharacter[j].c;
	}
	return null;
}

#include <stdio.h>

void Character::FindNearCharacters(MESSAGE & message)
{
	VDATA * array = message.ScriptVariablePointer();
	VDATA * num = message.ScriptVariablePointer();
	float rad = message.Float();
	float ax = message.Float();
	float viewAng = message.Float();
	float planeDist = message.Float();
	bool visTest = message.Long() != 0;
	bool isSort = message.Long() != 0;
	//���� ����������
	//����� ���������� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	static long n = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, n, this, rad, viewAng, planeDist, ax, isSort))
	{
		num->Set(0L);
		return;
	}
	num->Set(n);
	if(!n) return;
	if(n > long(array->GetElementsNum())) array->SetElementsNum(n);
	char buf[64];
	for(long i = 0, nn = 0; i < n; i++)
	{
		//����������
		Supervisor::FindCharacter & fc = fndCharacter[i];
		Assert(fc.c);
		if(!fc.c->AttributesPointer) continue;
		//���������� ���������
		if(visTest && !VisibleTest(fc.c)) continue;
		//������� �����������
		nn++;
		//������� �������
		VDATA * e = (VDATA *)array->GetArrayElement(i);
		//������������� ����
		e->Set("index", fc.c->AttributesPointer->GetAttribute("index"));
		sprintf(buf, "%f", sqrtf(fc.d2));
		e->Set("dist", buf);
		sprintf(buf, "%f", fc.dx);
		e->Set("dx", buf);
		sprintf(buf, "%f", fc.dy);
		e->Set("dy", buf);
		sprintf(buf, "%f", fc.dz);
		e->Set("dz", buf);
	}
	num->Set(nn);
}

//��������� ���������
bool Character::CharactersVisibleTest(MESSAGE & message)
{
	Character * chr = (Character *)_CORE_API->GetEntityPointer(&message.EntityID());
	if(!chr) return false;
	return VisibleTest(chr);
}

//�������� ��������� ���� ����������
bool Character::VisibleTest(Character * chr)
{
	if(!chr) return false;
	//���� �� ��� ���������
	if(location->VisibleTest(curPos + CVECTOR(0.0f, height*0.9f, 0.0f), chr->curPos + CVECTOR(0.0f, chr->height*0.97f, 0.0f))) return true;
	if(location->VisibleTest(curPos + CVECTOR(0.0f, height*0.9f, 0.0f), chr->curPos + CVECTOR(0.0f, chr->height*0.67f, 0.0f))) return true;
	if(location->VisibleTest(curPos + CVECTOR(0.0f, height*0.9f, 0.0f), chr->curPos + CVECTOR(0.0f, chr->height*0.37f, 0.0f))) return true;
	//������� ���� ��������� �� ������
	if(location->VisibleTest(curPos + CVECTOR(0.0f, height*0.9f, 0.0f), chr->curPos + CVECTOR(0.3f*cosf(chr->ay), chr->height*0.8f, -0.3f*sinf(chr->ay)))) return true;
	if(location->VisibleTest(curPos + CVECTOR(0.0f, height*0.9f, 0.0f), chr->curPos + CVECTOR(-0.3f*cosf(chr->ay), chr->height*0.8f, 0.3f*sinf(chr->ay)))) return true;
	//���������...
	return false;
}

//�������� ��������� ������
void Character::UpdateWeapons()
{
	if(isFightWOWps)
	{
		_CORE_API->LayerDel("realize", blade);
	}else{
		_CORE_API->LayerAdd("realize", blade, 65550);
	}
}


//�������� ����������� �� ���������� ��� �������� ��� �����
CVECTOR Character::GetEnemyDirForImpulse()
{
	Character * chr = (Character *)api->GetEntityPointer(&enemyAttack);
	if(!chr) return CVECTOR(0.0f);
	CVECTOR dir = chr->curPos - curPos;
	dir.y = 0.0f;
	float l = ~dir;
	if(l > 0.0f)
	{
		dir *= 1.0f/sqrtf(l);
		static const float jAng = cosf(20.0f*3.141592653f/180.0f);
		if((dir.x*sinf(ay) + dir.z*cosf(ay)) >= jAng)
		{
			return dir;
		}
	}
	return CVECTOR(0.0f);
}

//�������� ��������
const char * Character::GetValueByPrefix(const char * str, const char * pref)
{
	if(!str || !str[0]) return 0;
	if(!pref || !pref[0]) return 0;	
	while(true)
	{
		if(*pref == 0) return str;
		if(*str++ != *pref++) break;
	}
	return 0;



}

CVECTOR Character::GetHandLightPos()
{
	MODEL * mdlChr = Model();
	NODE * mdlNode = mdlChr ? mdlChr->GetNode(0) : 0;
	if( mdlNode )
	{
		long sti = -1;
		long idLocator;
		idLocator = mdlNode->geo->FindName(m_pcHandLightLocator);

		if((sti = mdlNode->geo->FindLabelN(sti+1, idLocator))>-1)
		{
			Animation *ani = mdlChr->GetAnimation();
			CMatrix *bones = &ani->GetAnimationMatrix(0);

			GEOS::LABEL lb;
			mdlNode->geo->GetLabel(sti, lb);
			CMatrix mt;
			mt.Vx() = CVECTOR(lb.m[0][0], lb.m[0][1], lb.m[0][2]);
			mt.Vy() = CVECTOR(lb.m[1][0], lb.m[1][1], lb.m[1][2]);
			mt.Vz() = CVECTOR(lb.m[2][0], lb.m[2][1], lb.m[2][2]);
			mt.Pos() = CVECTOR(lb.m[3][0], lb.m[3][1], lb.m[3][2]);

			CMatrix mbn = mt*bones[lb.bones[0]];
			mbn.Pos().x *= -1.0f;
			mbn.Vx().x *= -1.0f;
			mbn.Vy().x *= -1.0f;
			mbn.Vz().x *= -1.0f;

			CMatrix scl;
			scl.Vx().x = -1.0f;
			scl.Vy().y = 1.0f;
			scl.Vz().z = 1.0f;
			mbn.EqMultiply(scl, CMatrix(mbn));

			return (mbn * mdlChr->mtx).Pos();
		}
	}

	return CVECTOR(0.f,0.f,0.f);
}

// boal -->
void Character::StopFightAnimation()
{
	MODEL * m = Model();
	Animation * a;
	if (m) 	a = m->GetAnimation();
	if (a)
	{
		priorityAction.SetName(null);
		a->Player(0).Pause();
		a->Player(0).SetPosition(1.0f);
		fgtSetType = fgtCurType = fgt_none;
	}
}
// boal <--

bool Character::CheckObstacle(float fx,float fz, float fzlen)
{
	float ca = cosf(ay);
	float sa = sinf(ay);
	float fdx = fx*ca + fz*sa;
	float fdz = fz*ca - fx*sa;
	float fk = fzlen / sqrtf(fdx*fdx + fdz*fdz);
	float xmed = curPos.x + fdx;
	float zmed = curPos.z + fdz;
	float xbeg = xmed - fk*fdz;
	float xend = xmed + fk*fdz;
	float zbeg = zmed + fk*fdx;
	float zend = zmed - fk*fdx;
	/*float xbeg = xmed - fzlen*0.5f*sa;
	float xend = xmed + fzlen*0.5f*sa;
	float zbeg = zmed + fzlen*0.5f*ca;
	float zend = zmed - fzlen*0.5f*ca;*/

	float fTrace;
	float fHDist;
	CVECTOR vsrc,vdst;

	// �������� ������� �����
	vsrc = CVECTOR(xmed,50.f,zmed);
	vdst = CVECTOR(xmed,-50.f,zmed);
	fTrace = location->GetPtcData().Trace(vsrc,vdst);
	if(fTrace<0.f || fTrace>1.f) return true;
	fHDist = vsrc.y + fTrace * (vdst.y-vsrc.y) - curPos.y;
	if( fHDist*fHDist > 0.25f ) return true;

	// �������� ��������� �����
	vsrc = CVECTOR(xbeg,50.f,zbeg);
	vdst = CVECTOR(xbeg,-50.f,zbeg);
	fTrace = location->GetPtcData().Trace(vsrc,vdst);
	if(fTrace<0.f || fTrace>1.f) return true;
	fHDist = vsrc.y + fTrace * (vdst.y-vsrc.y) - curPos.y;
	if( fHDist*fHDist > 0.25f ) return true;

	// �������� �������� �����
	vsrc = CVECTOR(xend,50.f,zend);
	vdst = CVECTOR(xend,-50.f,zend);
	fTrace = location->GetPtcData().Trace(vsrc,vdst);
	if(fTrace<0.f || fTrace>1.f) return true;
	fHDist = vsrc.y + fTrace * (vdst.y-vsrc.y) - curPos.y;
	if( fHDist*fHDist > 0.25f ) return true;

	return false;
}

long Character::GetRandomIndexByObstacle(ObstacleZone* pZone, long num)
{
	long n,m,i,q;

	// ������ ���������� ������������ ���
	q = 0;
	for( i=0; i<num; i++ )
		if( pZone[i].use )
			q++;

	while(q>0)
	{
		// ������ ��������� ����� �� ��� ������������ ���
		n = rand() % q;
		m = 0;
		for(i=0; i<num; i++)
		{
			if( !pZone[i].use ) continue;
			if( m==n ) break;
			m++;
		}

		// ������ ��� ���
		if( i==num ) break;

		// �������� ���� �� ������� ���������� � ���
		if( pZone[i].x==0 && pZone[i].z==0 ) return i;
		if( !CheckObstacle(pZone[i].x,pZone[i].z, pZone[i].dw) ) return i;

		// ������� ���� ��� ��������������
		pZone[i].use = false;
		q--;
	}
	return -1;
}
