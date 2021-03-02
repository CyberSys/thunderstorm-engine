//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	NPCaracter
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "NPCharacter.h"
#include "Location.h"
#include "CharactersGroups.h"


//============================================================================================

//Goto
#define NPC_STOP_GOTODIST	0.3f	//��������� � ������, ��� ������� ��������� ��� �������� ����� �� �����

//Follow
#define NPC_STOP_DIST_NPC	1.5f	//��������� �� ���������, ��� ������� ���� ������������ � ��������
#define NPC_START_DIST_NPC	2.5f	//��������� �� ���������, ��� ������� ���� ������ ��������
#define NPC_RUN_DIST_NPC	10.0f	//��������� �� ���������, ��� ������� ���� ������
#define NPC_WALK_DIST_NPC	4.0f	//��������� �� ���������, ��� ������� ��� ��������� � ������

//Escape
#define NPC_STOP_ESCAPE		20.0f	//��������� �� ������� ����������� ��������


//Fight
#define NPC_FIGHT_STOP_DIST	2.0f	//��������� �� ���������, ��� ������� ���� ������������
#define NPC_FIGHT_GO_DIST	2.4f	//��������� �� ���������, ��� ������� ���� ������ �������� � ����
#define NPC_FIGHT_RUN_DIST	4.0f	//��������� �� ���������, ��� ������� ���������� � ����� ����
#define NPC_FIGHT_WALK_DIST	2.5f	//��������� �� ���������, ��� ������� ���������� � ����� ������
#define NPC_FIGHT_FIRE_DIST	25.0f	//��������� �� ���������, ��� ������� ����� ��������




//============================================================================================
//���������������, ������������������
//============================================================================================

NPCharacter::NPCharacter()
{	
	stackPointer = 0;
	task.task = npct_stay;
	lastSetTask = npct_unknow;
	CmdStay();
	//������� ���
	fightLevel = 0.0f;
	//�����
	attackCur = 0.5f;// k3 fix 0.05f;
	attackPrbFast = 0.2f;
	attackPrbForce = 0.5f;
	attackPrbRound = 0.1f;
	attackPrbBreak = 0.1f;
	attackPrbFeint = 0.1f;
	//������
	defenceCur = 0.05f;
	blockTime = 0.0f;
	defencePrbBlock = 0.9f;
	defencePrbParry = 0.1f;
	isRecoilEnable = true;
	//��������
	fireCur = 0.0f;
	isFireEnable = true;
	//������� ��������� ����������
	enemyFgtType = fgt_none;
	isFgtChanged = false;
	fightTick = 0.0f;
	wantToFire = false;
	wantToAttack = false;
	wantToDefence = false;

	//fMusketerTime = 2.0f;
	bMusketer = false;
	fMusketerDistance = 20.0f;
	bMusketerNoMove = false;
	fMusketerFireTime = 0.0f;
	fMusketerCheckFireTime = 5.0f;

	bTryAnyTarget = false;
}

NPCharacter::~NPCharacter()
{
}

bool NPCharacter::PostInit()
{
	api->FindClass(&charactersGroups, "CharactersGroups", 0);
	float tmp;
	long tmpBool;
	VDATA * vd;
	//��������� ������
	vd = api->Event("NPC_Event_GetAttackActive", "i", GetID());
	tmp = attackCur;
	if(vd && vd->Get(tmp)) attackCur = tmp;
	vd = api->Event("NPC_Event_GetAttackWeightFast", "i", GetID());
	tmp = attackPrbFast;
	if(vd && vd->Get(tmp)) attackPrbFast = tmp;	
	vd = api->Event("NPC_Event_GetAttackWeightForce", "i", GetID());
	tmp = attackPrbForce;
	if(vd && vd->Get(tmp)) attackPrbForce = tmp;
	vd = api->Event("NPC_Event_GetAttackWeightRound", "i", GetID());
	tmp = attackPrbRound;
	if(vd && vd->Get(tmp)) attackPrbRound = tmp;
	vd = api->Event("NPC_Event_GetAttackWeightBreak", "i", GetID());
	tmp = attackPrbBreak;
	if(vd && vd->Get(tmp)) attackPrbBreak = tmp;
	vd = api->Event("NPC_Event_GetAttackWeightFeint", "i", GetID());
	tmp = attackPrbFeint;
	if(vd && vd->Get(tmp)) attackPrbFeint = tmp;
	//���������� ������
	vd = api->Event("NPC_Event_GetDefenceActive", "i", GetID());
	tmp = defenceCur;
	if(vd && vd->Get(tmp)) defenceCur = tmp;
	vd = api->Event("NPC_Event_GetDefenceWeightBlock", "i", GetID());
	tmp = defencePrbBlock;
	if(vd && vd->Get(tmp)) defencePrbBlock = tmp;
	vd = api->Event("NPC_Event_GetDefenceWeightParry", "i", GetID());
	tmp = defencePrbParry;
	if(vd && vd->Get(tmp)) defencePrbParry = tmp;
	vd = api->Event("NPC_Event_EnableRecoil", "i", GetID());
	tmpBool = isRecoilEnable;
	if(vd && vd->Get(tmpBool)) isRecoilEnable = tmpBool != 0;
	//��������� ��������
	vd = api->Event("NPC_Event_GetFireActive", "i", GetID());
	tmp = fireCur;
	if(vd && vd->Get(tmp)) fireCur = tmp;
	vd = api->Event("NPC_Event_EnableFire", "i", GetID());
	tmpBool = isFireEnable;
	if(vd && vd->Get(tmpBool)) isFireEnable = tmpBool != 0;
	//������������ ����������
	if(attackCur < 0.0f) attackCur = 0.0f;
	if(attackCur > 1000.0f) attackCur = 1000.0f;
	if(defenceCur < 0.0f) defenceCur = 0.0f;
	if(defenceCur > 1000.0f) defenceCur = 1000.0f;
	float p = attackPrbFast + attackPrbForce + attackPrbRound + attackPrbBreak + attackPrbFeint;
	if(p > 0.0f)
	{
		p = 1.0f/p;
		attackPrbFast *= p;
		attackPrbForce *= p;
		attackPrbRound *= p;
		attackPrbBreak *= p;
		attackPrbFeint *= p;
	}
	p = defencePrbBlock + defencePrbParry;
	if(p > 0.0f)
	{
		p = 1.0f/p;
		defencePrbBlock *= p;
		defencePrbParry *= p;
	}
	return true;
}


dword NPCharacter::ChlProcessMessage(long messageID, MESSAGE & message)
{
	char buf[128];
	switch(messageID)
	{
	case MSG_NPCHARACTER_SETTASK:
		message.String(sizeof(buf) - 1, buf);
		buf[sizeof(buf) - 1] = 0;
		return SetNewTask(GetTaskID(buf), message);
	case MSG_NPCHARACTER_PUSHTASK:
 		return PushTask();
	case MSG_NPCHARACTER_POPTASK:
		return PopTask();
	case MSG_CHARACTER_SETFTGLEVEL:
		fightLevel = message.Float();
		if(fightLevel < 0.0f) fightLevel = 0.0f;
		if(fightLevel > 1.0f) fightLevel = 1.0f;
		return 1;
	case MSG_NPCHARACTER_GETTASK:
		VDATA * v = message.ScriptVariablePointer();
		if(!v) return 0;
		v->Set((char *)GetTaskName(task.task));
		return 1;
	};
	return 0;
}

//���������� ��������� � �������� �������
void NPCharacter::Move(float dltTime)
{
	//����������� ������
	switch(task.task)
	{
	case npct_none:
		break;
	case npct_stay:
		break;
	case npct_gotopoint:		
		break;
	case npct_followcharacter:
		break;
	case npct_fight:
		break;
	case npct_escape:
		break;
	case npct_dead:
		break;
	};
	AICharacter::Move(dltTime);
}

//�������� ������� ���������
void NPCharacter::Update(float dltTime)
{
	bMusketer = AttributesPointer->GetAttributeAsDword("isMusketer", 0) != 0;
	fMusketerDistance = AttributesPointer->GetAttributeAsFloat("MusketerDistance", 20.0f);
	bMusketerNoMove = fMusketerDistance <= 0.0f;

	AICharacter::bMusketer = bMusketer;
	AICharacter::bMusketerNoMove = bMusketerNoMove;

	fMusketerFireTime -= dltTime;
	fMusketerCheckFireTime -= dltTime;
	if (fMusketerFireTime <= 0.0f) fMusketerFireTime = 0.0f;

	switch(task.task)
	{
	case npct_none:
		break;
	case npct_stay:
		SetFightMode(false);
		break;
	case npct_gotopoint:
		break;
	case npct_followcharacter:
		UpdateFollowCharacter(dltTime);
		break;
	case npct_fight:
		UpdateFightCharacter(dltTime);
		break;
	case npct_escape:
		UpdateEscapeCharacter(dltTime);
		break;
	case npct_dead:
		break;
	};
	AICharacter::Update(dltTime);
	//������� ���������� ����������
	if(location->IsDebugView())
	{
		bool isDebugEx = location->IsExDebugView();
		//if(AttributesPointer && AttributesPointer->GetAttributeAsDword("hideInfo", 0)) return;
		const float rad = 25.0f;
		const char * id = null;
		if(AttributesPointer) id = AttributesPointer->GetAttribute("id");
		if(!id) id = "<none>";
		const char * fid = null;
		Character * chr = (Character *)_CORE_API->GetEntityPointer(&task.target);
		if(chr)
		{
			if(chr->AttributesPointer) fid = chr->AttributesPointer->GetAttribute("id");
		}
		if(!fid) fid = "<none>";
		location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, 3, 1.0f, 0xffffff, 0.5f, "id(%s)", id);
		location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, 2, 1.0f, 0xffffff, 0.5f, "task(%s<-%s)", GetTaskName(task.task), GetTaskName(lastSetTask));
		if(isDebugEx) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, 1, 1.0f, 0xffffff, 0.5f, "target(%s)", fid);
		if(isDebugEx) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, 0, 1.0f, 0xffffff, 0.5f, "cmd(%s)", GetCommandName(command.cmd));
		if(isDebugEx) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, -1, 1.0f, 0xffffff, 0.5f, "pos(%.2f, %.2f, %.2f)", curPos.x, curPos.y, curPos.z);
		if(isDebugEx) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, -2, 1.0f, 0xffffff, 0.5f, "isJump = %s", isJump ? "true" : "false");
		long line = 4;
		if(isDebugEx && command.cmd == aicmd_gotopoint) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "goto(%.2f, %.2f, %.2f)", command.pnt.x, command.pnt.y, command.pnt.z);
		if(isDebugEx && AttributesPointer)
		{
			ATTRIBUTES * atr = AttributesPointer->FindAClass(AttributesPointer, "chr_ai.tmpl");
			if(atr)
			{
				for(long i = atr->GetAttributesNum() - 1; i >= 0; i--)
				{
					fid = atr->GetAttributeName(i);
					id = atr->GetAttribute(i);
					if(fid && id) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "tmpl.%s(%s)", fid, id);
				}
				id = atr->GetThisAttr();
				if(id) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "tmpl(%s)", id);
			}
			atr = AttributesPointer->FindAClass(AttributesPointer, "chr_ai.type");
			if(atr)
			{
				for(long i = atr->GetAttributesNum() - 1; i >= 0; i--)
				{
					fid = atr->GetAttributeName(i);
					id = atr->GetAttribute(i);
					if(fid && id) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "type.%s(%s)", fid, id);
				}
				id = atr->GetThisAttr();
				if(id) location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "type(%s)", id);
			}
			atr = AttributesPointer->FindAClass(AttributesPointer, "chr_ai.group");
			if(atr)
			{
				id = atr->GetThisAttr();
				if(!id) id = "";
				location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "group(\"%s\")", id);
			}
		}
		location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "wantToAttack = %s", wantToAttack ? "true" : "false");
		location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "wantToDefence = %s", wantToDefence ? "true" : "false");
		location->Print(curPos + CVECTOR(0.0f, height, 0.0f), rad, line++, 1.0f, 0xffffff, 0.5f, "wantToFire = %s)", wantToFire ? "true" : "false");
	}
}

//============================================================================================
//������
//============================================================================================


void NPCharacter::SetEscapeTask(Character * c)
{
	if (bMusketerNoMove) return;
	if (task.task == npct_runtopoint) return;

	task.task = npct_runtopoint;
	lastSetTask = npct_runtopoint;
	memset(&task.target, 0, sizeof(task.target));
	//task.target = c->GetID();
	/*SetFightMode(false);
	SetRunMode(true);
	task.target = c->GetID();*/

	CVECTOR vOurPos, vPos;
	c->GetPosition(vPos);
	GetPosition(vOurPos);

	float fCurDist = sqrtf(~(vOurPos - vPos));
	CVECTOR vDir = !(vOurPos - vPos);

	SetFightMode(false);
	SetRunMode(true);
	for (long i=0; i<32; i++)
	{
		CMatrix mRot;
		mRot.BuildRotateY(float(i / 32.0f) * PIm2);
		CVECTOR vDir1 = mRot * vDir;

		task.to = vPos + vDir1 * (fMusketerDistance * 0.8f);
		if (CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST*2.0f)) break;
	}
	/*task.to.x = vPos.x;
	task.to.y = vPos.y;
	task.to.z = vPos.z - fMusketerDistance * 0.8f;*/
}

//���������� ����� ������
bool NPCharacter::SetNewTask(NPCTask tsk, MESSAGE & message)
{
	if (bMusketerNoMove)
	{
		if (tsk == npct_gotopoint) return true;
		if (tsk == npct_runtopoint) return true;
		if (tsk == npct_followcharacter) return true;
	}

	task.task = tsk;
	lastSetTask = tsk;
	memset(&task.target, 0, sizeof(task.target));
	switch(tsk)
	{
	case npct_none:
		//SetRunMode(false);
		//SetFightMode(false);
		CmdStay();
		return CmdNone();
	case npct_stay:
		SetFightMode(false);
		return CmdStay();
	case npct_gotopoint:
		SetFightMode(false);
		SetRunMode(false);
		task.to.x = message.Float();
		task.to.y = message.Float();
		task.to.z = message.Float();
		return CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST);
	case npct_runtopoint:
		SetFightMode(false);
		SetRunMode(true);
		task.to.x = message.Float();
		task.to.y = message.Float();
		task.to.z = message.Float();
		return CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST*2.0f);
	case npct_followcharacter:
		SetFightMode(false);
		task.target = message.EntityID();
		return InitFollowChartacter(task.target);
	case npct_fight:
		task.target = message.EntityID();
		return InitFightChartacter(task.target);
	case npct_escape:
		if (bMusketerNoMove) return true;
		SetFightMode(false);
		SetRunMode(true);
		task.target = message.EntityID();
		return (api->GetEntityPointer(&task.target) != null);
	case npct_dead:
		CmdStay();
		Dead();
		return true;
	};
	return false;
}

bool NPCharacter::InitFollowChartacter(ENTITY_ID & eid)
{
	task.target = eid;
	Character * c = (Character *)api->GetEntityPointer(&eid);
	if(c)
	{
		const char * id = c->AttributesPointer->GetAttribute("id");
	}
	task.isFollowInit = 0;
	return true;
}

bool NPCharacter::InitFightChartacter(ENTITY_ID & eid)
{
	enemyFgtType = fgt_none;
	isFgtChanged = false;
	task.target = eid;
	task.isFollowInit = 0;
	return true;
}


//============================================================================================
//���������� �����
//============================================================================================

//���������� ������ ���������� �� ����������
void NPCharacter::UpdateFollowCharacter(float dltTime)
{
	//����
	NPCharacter * c = (NPCharacter *)_CORE_API->GetEntityPointer(&task.target);
	if(!c || c->deadName != null || c->liveValue < 0)
	{
		NPCTask tsk = task.task;
		task.task = npct_none;
		CmdStay();
		FailureCommand(tsk);
		return;
	}
	//�������� ��������� �� ����
	float dst = ~(c->curPos - curPos);
	if(task.isFollowInit == 0)
	{
		task.isFollowInit = 1;
		if(dst <= NPC_START_DIST_NPC*NPC_START_DIST_NPC) EndGotoCommand();		
	}
	if(command.cmd != aicmd_gotopoint)
	{		
		SetRunMode(false);
		if(dst > NPC_START_DIST_NPC*NPC_START_DIST_NPC)
		{
			CmdGotoPoint(c->curPos.x, c->curPos.y, c->curPos.z, NPC_STOP_DIST_NPC, c->currentNode, false);
			_CORE_API->Event("Location_CharacterFollowGo", "si", GetTaskName(npct_followcharacter), GetID());
		}
	}else{
		if(dst > NPC_RUN_DIST_NPC*NPC_RUN_DIST_NPC) SetRunMode(true);
		if(dst < NPC_WALK_DIST_NPC*NPC_WALK_DIST_NPC) SetRunMode(false);
		CmdGotoPoint(c->curPos.x, c->curPos.y, c->curPos.z, NPC_STOP_DIST_NPC, c->currentNode, false);
	}
	SetExCharacter(c);
}

//���������� ������ ��������
void NPCharacter::UpdateEscapeCharacter(float dltTime)
{
	//�������� �� �������� �������
	NPCharacter * c = (NPCharacter *)_CORE_API->GetEntityPointer(&task.target);
	if(!c || c->deadName != null || c->liveValue < 0)
	{
		NPCTask tsk = task.task;
		task.task = npct_none;
		CmdStay();
		FailureCommand(tsk);
		return;
	}
	//������ �� ���������� �����
	float fDist = (bMusketer) ? fMusketerDistance * 0.7f : NPC_STOP_ESCAPE;
	CmdEscape(c->curPos.x, c->curPos.y, c->curPos.z, fDist);
	if(isSlide)
	{
		NPCTask tsk = task.task;
		task.task = npct_none;
		CmdStay();
		SetRunMode(false);
		_CORE_API->Event("Location_CharacterEscapeSlide", "si", GetTaskName(npct_escape), GetID());
	}
}

//���������� ������ ���
void NPCharacter::UpdateFightCharacter(float dltTime)
{
	//fMusketerTime -= dltTime;

	SetFightMode(true);
	//����
	NPCharacter * c = (NPCharacter *)_CORE_API->GetEntityPointer(&task.target);
	if(!c || c->deadName != null || c->liveValue < 0 || c == this)
	{
		NPCTask tsk = task.task;
		task.task = npct_none;
		CmdStay();
		FailureCommand(tsk);
		return;
	}
	//������ ��� ����� ������
	fightTick -= dltTime;
	if(fightTick <= 0.0f)
	{
		fightTick = 0.2f; // k3 fix 0.5f;
		FightTick();
	}
	//�������� ��������� �� ����
	float dst = ~(c->curPos - curPos);
	//������� � ����� ���
	if(dst > NPC_FIGHT_RUN_DIST*NPC_FIGHT_RUN_DIST)
	{
		SetRunMode(true);
	}else
	if(dst < NPC_FIGHT_WALK_DIST*NPC_FIGHT_WALK_DIST)
	{
		SetRunMode(false);
	}
	if(task.isFollowInit == 0)
	{
		task.isFollowInit = 1;
		if(dst < NPC_STOP_DIST_NPC*NPC_STOP_DIST_NPC) EndGotoCommand();
	}
	//���������
	bool isProcessWait = false;

	bool bCurrentActionIsFire = false;
	if(fgtCurType == fgt_none && priorityAction.name && shot.name)
	{
		if(stricmp(priorityAction.name, shot.name) == 0)
		{
			bCurrentActionIsFire = true;
		}
	}

	bool bGunLoaded = IsGunLoad();

	if (fgtSetType == fgt_fire || fMusketerFireTime > 0.0f)
		bCurrentActionIsFire = true;

	bool bTryFire = true;
	if (bMusketer && /*fMusketerTime <= 0.0f &&*/ fMusketerDistance > 0.0f && !bCurrentActionIsFire && !bGunLoaded)
	{
		if (sqrt(dst) < fMusketerDistance * 0.6f)
		{
			bTryFire = false;
			//fMusketerTime = 3.0f;
			SetEscapeTask(c);
		}
		//else
			//CmdStay();
	}

	float kdst;

	bool bVisTarget = VisibleTest(c);
	bool bFarTarget = dst > NPC_FIGHT_FIRE_DIST * NPC_FIGHT_FIRE_DIST;
	Character * target = FindGunTarget(kdst, true);

	if (bVisTarget && !bFarTarget) // ���� ������ �������, �� �������� ���������� ���
		bTryAnyTarget = false;

	// ��� � 5 ������ ���������, ����� �� ����������, ���� ���, �� ��������� �� ����� �� �������
	if (bMusketer && bTryFire && fMusketerCheckFireTime <= 0.0f)
	{
		fMusketerCheckFireTime = 6.0f;

		// ���� ������� ���� �� ����� � ���� ������ ������� ����� - �� �������� ������
		if (!bVisTarget && !target)
		{
			bTryFire = false;

			task.task = npct_runtopoint;
			lastSetTask = npct_runtopoint;
			memset(&task.target, 0, sizeof(task.target));
			//task.target = c->GetID();

			CVECTOR vOurPos, vPos;
			c->GetPosition(vPos);
			GetPosition(vOurPos);

			float fCurDist = sqrtf(~(vOurPos - vPos));
			CVECTOR vDir = !(vOurPos - vPos);

			SetFightMode(false);
			SetRunMode(true);
			for (long i=1; i<9; i++)
			{
				CMatrix mRot;
				mRot.BuildRotateY(float(i / 9.0f) * PIm2);
				CVECTOR vDir1 = mRot * vDir;

				task.to = vPos + vDir1 * fCurDist;
				if (CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST*2.0f)) break;
			}
		}
		else // ���� ������ ����, �� ������ - �� ����� �������� ���������� �����
		{
			bTryAnyTarget = true;
		}
	}

	//if (bTryFire)
	{
		if(command.cmd != aicmd_gotopoint && !bCurrentActionIsFire)
		{
			float fDist = (bMusketer && !bMusketerNoMove) ? fMusketerDistance * 0.9f : NPC_FIGHT_GO_DIST;
			float fDistTo = (bMusketer && !bMusketerNoMove) ? fMusketerDistance * 0.7f : NPC_FIGHT_GO_DIST;
			if(dst > fDist * fDist)
			{
				//���� ������� �����
				CmdGotoPoint(c->curPos.x, c->curPos.y, c->curPos.z, fDistTo, c->currentNode, false);
				_CORE_API->Event("Location_CharacterFightGo", "si", GetTaskName(npct_followcharacter), GetID());
			}//else{
				//�����
			//}
		}//else
		//{
			//������� ����� ��������� ����
			//CmdGotoPoint(c->curPos.x, c->curPos.y, c->curPos.z, NPC_FIGHT_STOP_DIST, c->currentNode, false);
			//�������� ����������� ��������

			double dx = c->curPos.x - curPos.x;
			double dz = c->curPos.z - curPos.z;
			double l = dx*dx + dz*dz;

			// �������� ��������
			/*if (bMusketer && fMusketerTime <= 0.0f && fMusketerDistance >= 0.0f && sqrt(l) < fMusketerDistance * 0.6f)
			{
				bTryFire = false;
				fMusketerTime = 3.0f;
				SetEscapeTask(c);
			}*/
			bool bFired = false;
			if(bTryFire && wantToFire && isFireEnable)
			{
				if(dst < NPC_FIGHT_FIRE_DIST*NPC_FIGHT_FIRE_DIST && 
					(dst > NPC_FIGHT_GO_DIST*NPC_FIGHT_GO_DIST || fightLevel > 0.6f))
				{
					if(IsGunLoad())
					{
						if(l > 0.3)
						{
							l = 1.0/sqrt(l);
							dx *= l; dz *= l;
							float ang = float(acos(dz));
							if(dx < 0) ang = -ang;
							if(dx*sinf(ay) + dz*cosf(ay) > 0.65f)
							{
								//���������� ������� ����
								float _ay = ay;
								ay = ang;
								float kdst;
								Character * target = FindGunTarget(kdst);
								//Character * target = FindGunTarget(kdst, true);
								ay = _ay;
								//if((target == c) || (target && bTryAnyTarget))
								if (target)
								{
									Turn(ang);
									Fire();
									bFired = true;
									fMusketerFireTime = 1.5f;
								}
							}
						}
					}
				}
			}

			if(!bFired && dst <= NPC_FIGHT_GO_DIST * NPC_FIGHT_GO_DIST)
			{
				if (fgtCurType == fgt_none && priorityAction.name && shot.name)
					;
				else
					DoFightAction(dltTime, c);
			}

			if (bMusketer && command.cmd == aicmd_stay)
			{
				Turn(c->curPos.x - curPos.x, c->curPos.z - curPos.z);
			}
		//}
	}
	SetExCharacter(c);
	if(fgtCurType == fgt_none && priorityAction.name && shot.name)
	{
		if(stricmp(priorityAction.name, shot.name) == 0)
		{
			float kdst;
			NPCharacter * target = (NPCharacter *)FindGunTarget(kdst, true);
			if(target)
			{
				if(target != c)
				{
					float tdx = curPos.x - target->curPos.x;
					float tdz = curPos.z - target->curPos.z;
					float edx = curPos.x - c->curPos.x;
					float edz = curPos.z - c->curPos.z;
					if(edz*tdx + edx*tdz < 0.0f)
					{
						Turn(ay - 0.1f);
					}else{
						Turn(ay + 0.1f);
					}
				}else{
					Turn(c->curPos.x - curPos.x, c->curPos.z - curPos.z);
				}
			}else{
				Turn(c->curPos.x - curPos.x, c->curPos.z - curPos.z);
			}
		}
	}
}


//============================================================================================
//Fight AI
//============================================================================================


//��������� � ���
void NPCharacter::DoFightAction(float dltTime, NPCharacter * enemy)
{
	//����������� ����� �������� ����������
	if(enemy->fgtCurType != enemyFgtType)
	{
		enemyFgtType = enemy->fgtCurType;
		isFgtChanged = true;
	}	
	//������� �� ������� ��������
	if(blockTime > 0.0f)
	{
		Block();
		blockTime -= dltTime;
		return;
	}
	switch(fgtCurType)
	{
	case fgt_none:
		//��������
		DoFightActionAnalysisNone(dltTime, enemy);
		break;
	}
	//���������� ��������������� ���������
	Turn(enemy->curPos.x - curPos.x, enemy->curPos.z - curPos.z);
	isFgtChanged = false;
}

//��������� � ��� - idle
void NPCharacter::DoFightActionAnalysisNone(float dltTime, NPCharacter * enemy)
{	
	if(!enemy) return;
	//������� �� ���� �������
	bool wishAttact = wantToAttack;
	bool wishDefence = wantToDefence;
	//���� ������ �� ������ - �������� ������� � ������ ������ �� ������
	if(!(wishAttact | wishDefence)) return;
	//�������� ����� ������ ���� ��� �����
	long isAdaptive = true;
	VDATA * vd = api->Event("NPC_Event_AdaptiveTargetSelect", "i", GetID());
	if(vd) vd->Get(isAdaptive);
	//����������� � ������ ������� �����
	CharactersGroups * chrGroup = (CharactersGroups *)api->GetEntityPointer(&charactersGroups);
	if(!chrGroup) isAdaptive = false;
	//���� ����� ���� � ����� �� ����������, �� ������ ����
	if(wishAttact && !isAdaptive)
	{
		DoFightAttack(enemy, 1, false);
		return;
	}
	//�������� ���� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	static long num = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, num, this, CHARACTER_ATTACK_DIST, 0.0f, 0.01f, 0.0f, false)) return;
	if(!num) return;
	//���� ������
	long grpIndex = chrGroup->FindGroupIndex(group);
	//������� ������
	static EnemyState enemies[MAX_CHARACTERS];
	long enemyCounter = 0;
	//���� �����������
	CVECTOR dir(sinf(ay), 0.0f, cosf(ay));
	//��������� ������
	bool isFreeBack = isRecoilEnable;
	static const float backAng = -cosf(45.0f*(3.1415926535f/180.0f));
	for(long i = 0; i < num; i++)
	{
		//��������
		Supervisor::FindCharacter & fc = fndCharacter[i];
		NPCharacter * chr = (NPCharacter *)fc.c;
		if(chr->liveValue < 0 || chr->deadName || fc.d2 <= 0.0f || chr == this) continue;
		fc.d2 = sqrtf(fc.d2);
		if(isFreeBack)
		{
			float cs = dir.x*fc.dx + dir.z*fc.dz;
			if(cs < 0.0f)
			{
				if(-cs >= backAng*fc.d2)
				{
					isFreeBack = false;
				}
			}
		}
//		if(!chr->isFight) continue;
		//������ ��������
		long grp = chrGroup->FindGroupIndex(chr->group);
		//��������� ��� ������ � �����
		if(chrGroup->FindRelation(grpIndex, grp).curState != CharactersGroups::rs_enemy) continue;
		//��� ����
		enemies[enemyCounter].chr = chr;
		//���� ������� ���� � ������������ ������������ ���
		if(fc.d2 > 0.0f)
		{			
			enemies[enemyCounter].look = -(sinf(chr->ay)*fc.dx + cosf(chr->ay)*fc.dz)/fc.d2;
			enemies[enemyCounter].dir = (dir.x*fc.dx + dir.z*fc.dz)/fc.d2;
		}else{
			enemies[enemyCounter].look = 0.0f;
			enemies[enemyCounter].dir = 0.0f;
		}
		//���� ����, �� ����������� ����
		if(wishAttact)
		{
			float hp = 1.0f;
			vd = api->Event("NpcEvtHP", "i", chr->GetID());
			if(vd) vd->Get(hp);
			if(hp < 0.0f) hp = 0.0f;
			if(hp > 1.0f) hp = 1.0f;
			float energy = 1.0f;
			vd = api->Event("NpcEvtEgy", "i", chr->GetID());
			if(vd) vd->Get(energy);
			if(energy < 0.0f) energy = 0.0f;
			if(energy > 1.0f) energy = 1.0f;
			enemies[enemyCounter].state = 0.1f/(hp + 0.05f) + 0.2f/(energy + 0.5f);
		}
		enemyCounter++;
	}
	//���� ������ ������������ ��� - ������ �� ������
	if(!enemyCounter) return;
	//���� �������, �� �������� ���������� ���� ��� �����
	if(wishAttact)
	{
		float kSel;
		long counter = 0;
		Character * enemy = null;
		for(long i = 0, j = -1; i < enemyCounter; i++)
		{
			EnemyState & es = enemies[i];
			float k = es.state*1.0f + (es.dir + 1.0f)*0.5f;
			if(enemy)
			{
				if(kSel < k)
				{
					kSel = k;
					enemy = es.chr;	
				}
			}else{
				kSel = k;
				enemy = es.chr;
			}
			static const float dangerAng = cosf(0.7f*CHARACTER_ATTACK_ANG*(3.1415926535f/180.0f));
			if(es.look > dangerAng) counter++;
		}
		DoFightAttack(enemy, counter, wishDefence);
		return;
	}
	if(!wishDefence) return;
	//������ ����� �� ���������
	bool isBreakAttack = false;
	long attacked = 0;
	static const float attackAng = cosf(0.5f*CHARACTER_ATTACK_ANG*(3.1415926535f/180.0f));
	for(long i = 0; i < enemyCounter; i++)
	{
		if(enemies[i].look >= attackAng)
		{
			NPCharacter * chr = (NPCharacter *)enemies[i].chr;
			if(chr->fgtCurType >= fgt_attack_fast && chr->fgtCurType <= fgt_attack_feintc)
			{
				if(chr->fgtCurType == fgt_attack_break) isBreakAttack = true;
				attacked++;
			}
		}
	}
	//�������� ������ ����������� �������
	CVECTOR p = curPos;
	if(location->GetPtcData().Move(currentNode, curPos + dir*2.0f, p) >= 0)
	{
		p -= curPos; p.y = 0.0f;
		if(~p < 1.8f)
		{
			isFreeBack = false;
		}
	}
	//���� ������ ���, �� �������� ��� ������ ���� ������� ������� ����
	if(!attacked)
	{
		return;
	}
	//������ ����������� ����, ��� ��������� ����
	if(!wantToDefence)
	{
		return;
	}
	//���� ���� �����������
	if(isBreakAttack)
	{
		//����������� ����������� ����������� �����
		float prbDetect = fightLevel*fightLevel*0.9f;
		if(PrTest(prbDetect))
		{
			if(!isFreeBack)
			{
				DoFightBlock(true);
			}else{
				//���� ����� - ����������� ��� ������
				if(PrTest(0.4f + enemyCounter*(0.1f + fightLevel*0.1f)))
				{
					Recoil();
				}else{
					DoFightBlock(true);
				}
			}
			return;
		}
	}
	//������ ��� ����������
	if(isFreeBack)
	{
		float reciolPrb = attacked*attacked*0.1f*(fightLevel + 0.3f);
		if(reciolPrb > 0.9f) reciolPrb = 0.9f;
		if(PrTest(reciolPrb))
		{
			Recoil();
			return;
		}
	}
	DoFightBlock();
}

//��������� � ��� - attack
void NPCharacter::DoFightAttack(Character * enemy, long enemyCounter, bool wishDefence)
{
	wantToAttack = false;
	//enemy - ���� ����
	//enemyCounter - ���������� �������� ������
	//wishDefence - � ������ ����� ���� ������� ����������
	struct AttackAction
	{
		float prb;
		FightAction action;
	};
	AttackAction attack[5];
	long count = 0;
	if(attackPrbFast > 0.0f)
	{
		attack[count].prb = attackPrbFast;
		attack[count].action = fgt_attack_fast;
		count++;
	}
	if(attackPrbForce > 0.0f)
	{
		attack[count].prb = attackPrbForce;
		attack[count].action = fgt_attack_force;
		count++;
	}
	if(attackPrbRound > 0.0f)
	{
		if(enemyCounter > 3 - (wishDefence ? 1 : 0))
		{
			attack[count].prb = attackPrbRound;
			attack[count].action = fgt_attack_round;
			count++;
		}
	}
	if(attackPrbBreak > 0.0f)
	{
		attack[count].prb = attackPrbBreak;
		attack[count].action = fgt_attack_break;
		count++;
	}
	if(attackPrbFeint > 0.0f)
	{
		attack[count].prb = attackPrbFeint;
		attack[count].action = fgt_attack_feint;
		count++;
	}
	float max = 0.0f;
	for(long i = 0; i < count; i++)
	{
		max += attack[i].prb;
	}
	if(count == 0 || max <= 0.0f) return;
	float r = rand()*(max/RAND_MAX);
	float p = 0.0f;
	for(long i = 0; i < count; i++)
	{
		p += attack[i].prb;
		if(r <= p) break;
	}
	if(i >= count) i = 0;
	Attack(enemy, attack[i].action);
}

//��������� � ��� - block, parry
void NPCharacter::DoFightBlock(bool needParry)
{
	wantToDefence = false;
	float sum = defencePrbBlock + defencePrbParry;
	if(sum <= 0.0f) return;
	float parryEnergy = GetActEnergy("parry");
	float used = GetEnergy() - parryEnergy;
	float minTime = 0.8f + (1.0f - fightLevel)*0.8f;
	float maxTime = minTime + (1.1f - fightLevel)*2.5f;
	blockTime = minTime + rand()*(1.0f/RAND_MAX)*(maxTime - minTime);
	if(used < 0.0f)
	{
		Block();
		return;
	}
	if(needParry)
	{
		Parry();
		return;
	}
	float noParryThreshold = (fightLevel*0.3f + rand()*(0.3f/RAND_MAX));
	if(used < noParryThreshold)
	{
		Block();
		return;
	}
	if(rand()*sum*(1.0f/RAND_MAX) <= defencePrbBlock)
	{
		Block();
	}else{
		Parry();
	}
}

//�������� �������
float NPCharacter::GetEnergy()
{
	float energy = 1.0f;
	VDATA * vd = api->Event("NpcEvtEgy", "i", GetID());
	if(vd) vd->Get(energy);
	if(energy < 0.0f) energy = 0.0f;
	if(energy > 1.0f) energy = 1.0f;
	return energy;
}

//�������� ������� ��� ��������
float NPCharacter::GetActEnergy(const char * act)
{
	VDATA * vd = api->Event("NPC_Event_GetActionEnergy", "is", GetID(), act);
	float energy;
	if(vd && vd->Get(energy)) return energy;
	return 0.1f;
}

void NPCharacter::HitChild(bool isInBlock)
{
	if(isInBlock)
	{
		blockTime -= fightLevel*0.01f;
	}
}

//============================================================================================



//���������� ���������� ���������� �������
void NPCharacter::FailureCommand()
{
	FailureCommand(task.task);
}

//�������� ������ � �����
void NPCharacter::EndGotoCommand()
{
	switch(task.task)
	{
	case npct_gotopoint:
		task.task = npct_none;
		CmdStay();
		SetRunMode(false);
		_CORE_API->Event("Location_CharacterEndTask", "si", GetTaskName(npct_gotopoint), GetID());
		return;
	case npct_runtopoint:
		task.task = npct_none;
		CmdStay();
		SetRunMode(false);
		_CORE_API->Event("Location_CharacterEndTask", "si", GetTaskName(npct_runtopoint), GetID());
		return;
	case npct_followcharacter:
		CmdStay();
		_CORE_API->Event("Location_CharacterFollowStay", "si", GetTaskName(npct_followcharacter), GetID());
		return;
	case npct_fight:
		CmdStay();
		_CORE_API->Event("Location_CharacterFightStay", "si", GetTaskName(npct_followcharacter), GetID());
		return;
	}
}

//�������� �������� �� ����� �� ����������� ������
void NPCharacter::EndEscapeCommand()
{
	task.task = npct_stay;
	SetRunMode(false);
	CmdStay();
	_CORE_API->Event("Location_CharacterEndTask", "si", GetTaskName(npct_escape), GetID());
}

//� ���������� ������� ����� �����������
void NPCharacter::CollisionThreshold()
{
	_CORE_API->Event("Location_CharacterColThreshold", "si", GetTaskName(task.task), GetID());
}

//��������� ������ � �����
bool NPCharacter::PushTask()
{
	if(stackPointer >= sizeof(taskstack)/sizeof(Task)) return false;
	task.isRun = IsRun();
	task.isFight = IsFight();
	taskstack[stackPointer++] = task;
	return true;
}

//����������� ������ �� �����
bool NPCharacter::PopTask()
{
	if(stackPointer <= 0) return false;
	task = taskstack[--stackPointer];
	switch(task.task)
	{
	case npct_none:
		return CmdNone();
	case npct_stay:		
		return CmdStay();
	case npct_gotopoint:		
		SetRunMode(false);
		return CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST);
	case npct_runtopoint:
		SetRunMode(true);
		return CmdGotoPoint(task.to.x, task.to.y, task.to.z, NPC_STOP_GOTODIST);
	case npct_followcharacter:
		return InitFollowChartacter(task.target);
	case npct_fight:
		return InitFightChartacter(task.target);
		break;
	case npct_escape:
		break;
	case npct_dead:
		CmdStay();
		Dead();
		break;
	};
	SetRunMode(task.isRun != 0);
	SetFightMode(task.isFight != 0);
	return true;
}


//============================================================================================
//������������
//============================================================================================

//���������� ���������� ���������� �������
void NPCharacter::FailureCommand(NPCTask task)
{
	_CORE_API->Event("Location_CharacterTaskFailure", "si", GetTaskName(task), GetID());
}

//�������� ��� ������ �� �����
NPCharacter::NPCTask NPCharacter::GetTaskID(const char * taskName)
{
	if(!taskName || !taskName[0]) return npct_unknow;
	for(long i = 0; i < npct_max; i++)
	{
		const char * task = GetTaskName(NPCTask(i));
		if(stricmp(task, taskName) == 0) return NPCTask(i);
	}
	return npct_unknow;
}

//�������� ��� ������ �� ����
const char * NPCharacter::GetTaskName(NPCTask t)
{
	switch(t)
	{
	case npct_none:				//��� ������, �������� �������������� �����
		return "None";
	case npct_stay:				//������ �� �����
		return "Stay";
	case npct_gotopoint:		//���� � �����
		return "Goto point";
	case npct_runtopoint:		//������ � �����
		return "Runto point";
	case npct_followcharacter:	//���� �� ���c������
		return "Follow character";
	case npct_fight:			//��������� � ������ ����������
		return "Fight";
	case npct_escape:			//������� �� ���������
		return "Escape";
	case npct_dead:				//������ ���������
		return "Dead";
	};
	return "Unknow task";
}

//�������� �������
void NPCharacter::FightTick()
{
	wantToFire = PrTest(fireCur);
	wantToAttack = PrTest(attackCur);
	wantToDefence = PrTest(defenceCur);
}
