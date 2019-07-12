//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//
//--------------------------------------------------------------------------------------------
//	Player
//--------------------------------------------------------------------------------------------
//
//============================================================================================

#include "Player.h"


#include "Location.h"
#include "../Common_h/collide.h"
#include "LocationCamera.h"
#include "../Shared/messages.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

Player::Player()
{
	chrGroups = -1;
	lastChange = 10.0f;
	task.task = npct_none;
	activatedDialog = false;
	CmdNone();
	isSpecialMode = false;
	isSetBlock = false;
#ifndef _XBOX
	kSMReload = 1.0f;
	locCam = nullptr;
	shootgunMode = false;
	//shootgunMode = false;
#endif
}

Player::~Player()
{
#ifndef _XBOX
	entid_t peid;
	if(peid = api->GetEntityIdWalker("ShootGunParticles")())
		api->EraseEntity(peid);
#endif
}

bool Player::PostInit()
{
	const auto location = GetLocation();
	if(!location->supervisor.player) location->supervisor.player = this;
	baterfl = api->GetEntityIdWalker("Animals")();
	return NPCharacter::PostInit();
}

void Player::Reset()
{
	if(fgtCurType == fgt_block && !isSetBlock) fgtCurType = fgt_none;
	NPCharacter::Reset();
}

//���������� ��������� � �������� �������
void Player::Move(float dltTime)
{
#ifndef _XBOX

	kSMReload += dltTime*0.7f;
	if(kSMReload > 1.0f) kSMReload = 1.0f;
	if(!locCam)
	{
		entid_t lcam;
		if(lcam = api->GetEntityIdWalker("LocationCamera")())
		{
			locCam = (LocationCamera *)api->GetEntityPointer(lcam);
		}
	}else{
		locCam->LockFPMode(shootgunMode);
	}
	//tuner.isVisible = !shootgunMode;

	bool oldSGMode = shootgunMode;
	shootgunMode = false;
	VDATA * vd = api->Event("EventSGMode", nullptr);
	if(vd)
	{
		long data = 0;
		if(vd->Get(data)) shootgunMode = (data != 0);
	}
	if(oldSGMode != shootgunMode)
	{
		if(shootgunMode)
		{
			api->Send_Message(effects, "s", "SGInited");
			tuner.alpha = 0.0f;
			tuner.camAlpha = 0.0f;
			isFight = true;
		}else{
			api->Send_Message(effects, "s", "SGRelease");
			tuner.alpha = 1.0f;
			tuner.camAlpha = 1.0f;
			isFight = false;
		}
	}

#endif

	/*
	if(GetAsyncKeyState(VK_SPACE) < 0)
	{
		impulse.y += 5.0f;
		impulse.y *= 2;
	}*/

	if(task.task == npct_none)
	{
		isEnableJump = true;
		//------------------------------------
		Rotate(dltTime);
		//------------------------------------
		if(lastChange > 0.3f)
		{
			//lastChange = 0.0f;
			SetRunMode(IsRunMode(dltTime));
			if(GoForward(dltTime))
			{
				StartMove(false);
				StrafeWhenMove(dltTime);
			}else
			if(GoBack(dltTime))
			{
				if(!IsFight())
				{
					StartMove(true);
					StrafeWhenMove(dltTime);
				}else{
					Recoil();
				}
			}else{
				StopMove();
				StrafeWhenStop(dltTime);
			}
		}else lastChange += dltTime;
		//------------------------------------
#ifndef _XBOX
		if(!shootgunMode)
		{
#endif
			if(IsChangeFightMode())
			{
				isSetBlock = false;
				SetFightMode(!IsFight());
			}
			if(IsFight())
			{
				if(IsDoBlock())
				{
					// boal -->
					if (fgtCurType == fgt_attack_fast || fgtCurType == fgt_attack_force || fgtCurType == fgt_attack_round ||
						fgtCurType == fgt_attack_break || fgtCurType == fgt_attack_feint || fgtCurType == fgt_parry)
					{
						StopFightAnimation();
					}
					// boal <--
					Block();
				}else
				if(IsDoParry())
				{
					// boal -->
					if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak ||
						fgtCurType == fgt_hit_attack)
					{
						StopFightAnimation();
					}
					// boal <--
					Parry();
				}else
				if(IsDoAttackForce())
				{
                    // boal -->
					if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak)
					{
						StopFightAnimation();
					}
					// boal <--
					Attack(FindAttackCharacter(), fgt_attack_force);
				}else
				if(IsDoAttackFast())
				{
                    if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak)
					{
						StopFightAnimation();
					}
					Attack(FindAttackCharacter(), fgt_attack_fast);
				}else
				if(IsDoAttackRound())
				{
                    if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak ||
						fgtCurType == fgt_hit_attack)
					{
						StopFightAnimation();
					}
					Attack(FindAttackCharacter(), fgt_attack_round);
				}else
				if(IsDoAttackBreak())
				{
                    if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak)
					{
						StopFightAnimation();
					}
					Attack(FindAttackCharacter(), fgt_attack_break);
				}else
				if(IsDoAttackFeint())
				{
                    if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak ||
						fgtCurType == fgt_hit_attack)
					{
						StopFightAnimation();
					}
					Attack(FindAttackCharacter(), fgt_attack_feint);
				}else
				if(IsFire())
				{
                    if (fgtCurType == fgt_block || fgtCurType == fgt_blockhit || fgtCurType == fgt_blockbreak ||
						fgtCurType == fgt_hit_attack)
					{
						StopFightAnimation();
					}
					Fire();
				}
			}
#ifndef _XBOX
		}else{
			tuner.alpha = 0.0f;
			tuner.camAlpha = 0.0f;
			isFight = true;
			if(kSMReload >= 1.0f) if(IsFire()) FireFromShootgun();
		}
#endif
		//------------------------------------
	}else isEnableJump = false;
	NPCharacter::Move(dltTime);
}

void Player::Update(float dltTime)
{
#ifdef _XBOX
	isSpecialMode = false;
	VDATA * vd = api->Event("EventGetSpecialMode", 0);
	if(vd)
	{
		long isSpec = 0;
		if(vd->Get(isSpec))
		{
			isSpecialMode = (isSpec != 0);
			if(isLookFromEyes) isSpecialMode = false;
		}
	}
#endif
	bool aDialog = false;
	if(task.task == npct_none)
	{
		if(!IsFight())
		{
			CONTROL_STATE cs;
			api->Controls->GetControlState("ChrAction",cs);
			if(cs.state == CST_ACTIVATED) aDialog = true;
			if(activatedDialog)
			{
 				Character * chr = FindDialogCharacter();
				if(chr)
				{
					Assert(AttributesPointer);
					Assert(chr->AttributesPointer);
					long first = AttributesPointer->GetAttributeAsDword("index", -1);
					long next = chr->AttributesPointer->GetAttributeAsDword("index", -1);
					if(first >= 0 && next >= 0)
					{
						api->Event("dlgReady", "ll", next, first);
					}else{
						api->Trace("Incorrect character index! Dialog not activated...");
					}
				}
			}

			api->Controls->GetControlState("ChrJump",cs);
			if(cs.state == CST_ACTIVATED)
			{
				StartJump();
			}
		}else{
			if(IsFireFindTarget())
			{
				float kDist;
				Character * c = FindGunTarget(kDist);
				if(c) c->Select();
			}
		}
	}

	//float kDist;
	//Character * c = FindGunTarget(kDist);
	//if(c) c->Select();
	NPCharacter::Update(dltTime);
	activatedDialog = aDialog;
	api->Send_Message(baterfl, "lff", MSG_ANIMALS_BUTTERFLIES_XYZ, curPos.x, curPos.z);
	//���������� ���������� � ������� ������ � ������
	entid_t eid;
	if(eid = api->GetEntityIdWalker("CharactersGroups")())
	{
		const auto location = GetLocation();
		for(long i = 0; i < location->supervisor.numCharacters; i++)
		{
			Character * chr = location->supervisor.character[i].c;
			if(chr != this && chr)
			{
				chr->isPlayerEnemy = (api->Send_Message(eid, "sii", "IsEnemy", GetId(), chr->GetId()) != 0);
			}
		}
	}
}

//��������� ���������
void Player::SetSaveData(ATTRIBUTES * sdata)
{
	if(!sdata) return;
	sdata->SetAttributeUseDword("isFight", isFight);
}

//����������� ���������
void Player::GetSaveData(ATTRIBUTES * sdata)
{
	if(!sdata) return;
	if(task.task == npct_none)
	{
		SetFightMode(sdata->GetAttributeAsDword("isFight", isFight) != 0, false);
	}
}


void Player::Rotate(float dltTime)
{
	CONTROL_STATE cs;
	if(!isSpecialMode)
	{
		float a = GetRotateH()*0.01f;
		if(a > 0.3f) a = 0.3f;
		if(a < -0.3f) a = -0.3f;
		Turn(GetAY() + a);
	}else{
		if(!lockRotate)
		{
			float dx = GetRotateH()*0.067f;
			api->Controls->GetControlState("ChrTurnV",cs);
			float dz = cs.fValue*0.067f;
			if(api->Controls->GetControlState("ChrTurnV1",cs)) dz += cs.fValue*0.067f;
			if(api->Controls->GetControlState("ChrTurnV2",cs)) dz += cs.fValue*0.067f;
			if(dx*dx + dz*dz > 0.1f)
			{
				//�������� ������ ������������ ������
				CMatrix mtx;
				const auto location = GetLocation();
				location->GetRS()->GetTransform(D3DTS_VIEW, mtx);
				mtx.Transposition3X3();
				mtx.Vy() = CVECTOR(0.0f, 1.0f, 0.0f);
				mtx.Vx().y = 0.0f;
				mtx.Vz().y = 0.0f;
				mtx.Vx() = !CVECTOR(mtx.Vx());
				mtx.Vz()= !CVECTOR(mtx.Vz());
				mtx.Pos() = 0.0f;
				CVECTOR res = mtx*CVECTOR(dx, 0.0f, dz);
				Turn(res.x, res.z);
			}
		}
	}
}

bool Player::GoForward(float dltTime)
{
	CONTROL_STATE cs;
	if(!isSpecialMode)
	{
		bool res = false;
		api->Controls->GetControlState("ChrForward",cs);
		if(cs.lValue != 0) res = true;
		if(api->Controls->GetControlState("ChrForward1",cs) && cs.lValue != 0) res = true;
		if(api->Controls->GetControlState("ChrForward2",cs) && cs.lValue != 0) res = true;
		return res;
	}else{
		float dx = GetRotateH()*0.067f;
		api->Controls->GetControlState("ChrTurnV",cs);
		float dz = cs.fValue*0.067f;
		if(api->Controls->GetControlState("ChrTurnV1",cs)) dz += cs.fValue*0.067f;
		if(api->Controls->GetControlState("ChrTurnV2",cs)) dz += cs.fValue*0.067f;
		if(IsMove())
		{
			return dx*dx + dz*dz > 0.1f;
		}
		return dx*dx + dz*dz > 0.2f;
	}
}

bool Player::GoBack(float dltTime)
{
	if(!isSpecialMode)
	{
		CONTROL_STATE cs;
		bool res = false;
		api->Controls->GetControlState("ChrBackward",cs);
		if(cs.lValue != 0) res = true;
		if(api->Controls->GetControlState("ChrBackward1",cs) && cs.lValue != 0) res = true;
		if(api->Controls->GetControlState("ChrBackward2",cs) && cs.lValue != 0) res = true;
		return res;
	}
	return false;
}

bool Player::IsRunMode(float dltTime)
{
	CONTROL_STATE cs;
	if(!isSpecialMode)
	{
		api->Controls->GetControlState("ChrRun",cs);
		return !(cs.lValue != 0);
	}else{
		float dx = GetRotateH()*0.067f;
		api->Controls->GetControlState("ChrTurnV",cs);
		float dz = cs.fValue*0.067f;
		if(api->Controls->GetControlState("ChrTurnV1",cs)) dz += cs.fValue*0.067f;
		if(api->Controls->GetControlState("ChrTurnV2",cs)) dz += cs.fValue*0.067f;
		if(IsMove() && IsRun())
		{
			return dx*dx + dz*dz > 0.4f;
		}
		return dx*dx + dz*dz > 0.6f;
	}
}

void Player::StrafeWhenMove(float dltTime)
{
	strafeMove = 0.0f;

//----------------------------------------------------------------------------------------
//���� ���� ��������� ������� - ������������� return
//stopstrafe
//----------------------------------------------------------------------------------------

	//return;


	if(!isSpecialMode)
	{
		CONTROL_STATE cs;
		if(api->Controls->GetControlState("ChrStrafeLeft", cs))
		{
			if(cs.lValue != 0)
			{
				strafeMove += -1.0f;
			}
		}
		if(api->Controls->GetControlState("ChrStrafeRight", cs))
		{
			if(cs.lValue != 0)
			{
				strafeMove += 1.0f;
			}
		}
	}
}

void Player::StrafeWhenStop(float dltTime)
{
	StrafeWhenMove(dltTime);
	if(IsFight())
	{
		if(strafeMove > 0.5f)
		{
			StrafeRight();
		}else
		if(strafeMove < -0.5f)
		{
			StrafeLeft();
		}
		strafeMove = 0.0f;
	} else {
		strafeMove = 0.0f;
	}
}

bool Player::IsDoBlock()
{
//	return true;
	CONTROL_STATE cs;
	api->Controls->GetControlState("ChrBlock",cs);
	if(cs.state == CST_ACTIVATED)
	{
		isSetBlock = true;
	}else
	if(cs.state == CST_INACTIVATED)
	{
		isSetBlock = false;
	} else
	if( cs.state == CST_INACTIVE )
	{
		api->Controls->GetControlState("ChrBlock2",cs);
		if(cs.state == CST_ACTIVATED)
		{
			isSetBlock = true;
		}else
		if(cs.state == CST_INACTIVATED)
		{
			isSetBlock = false;
		}
	}
	if(!IsFight())
	{
		isSetBlock = false;
	}
	return isSetBlock;
}

bool Player::IsDoParry()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrParry",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrParry2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsDoAttackForce()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrAttackForce",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrAttackForce2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsDoAttackFast()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrAttackFast",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrAttackFast2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsDoAttackRound()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrAttackRound",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrAttackRound2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsDoAttackBreak()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrAttackBreak",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrAttackBreak2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsDoAttackFeint()
{
	CONTROL_STATE cs;
	bool bPressed = false;
	api->Controls->GetControlState("ChrAttackFient",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	api->Controls->GetControlState("ChrAttackFient2",cs);
	if( cs.state == CST_ACTIVATED ) bPressed = true;
	return bPressed;
}

bool Player::IsFire()
{
	CONTROL_STATE cs;
	api->Controls->GetControlState("ChrFire",cs);
	return (cs.state == CST_ACTIVATED);
}

bool Player::IsChangeFightMode()
{
	CONTROL_STATE cs;
	api->Controls->GetControlState("ChrFightMode",cs);
	return (cs.state == CST_ACTIVATED);
}


//����� ���������� ����������
Player * Player::FindAttackCharacter()
{
	const auto location = GetLocation();
	//����� ���������� ����������
	static Supervisor::FindCharacter fndCharacter[MAX_CHARACTERS];
	static long num = 0;
	if(!location->supervisor.FindCharacters(fndCharacter, num, this, CHARACTER_ATTACK_DIST*1.1f)) return nullptr;
	//�������� �������
	float minDst;
	long task = -1;
	bool isFgt = false;
	bool isEnemy = false;
	float enemyCos = -1.0f;
	float cdx = sinf(ay);
	float cdz = cosf(ay);
	long j = -1;
	for(long i = 0; i < num; i++)
	{
		//��������
		Supervisor::FindCharacter & fc = fndCharacter[i];
		//��������� �� �������
		//if(!fc.c->IsFight()) continue;
		Player * chr = (Player *)fc.c;
		if(chr == this) continue;
		//̸����� ����������
		if(chr->liveValue < 0 || chr->deadName) continue;
		//��������� ������������
		if(isEnemy) //~!~
		{
			if(chr->task.task != npct_fight ||
				api->GetEntityPointer(chr->task.target) != this) continue;
		}else{
			if(isFgt)
			{
				if(!chr->isFight) continue;
			}else{
				if(chr->isFight)
				{
					j = -1;
					isFgt = true;
				}
			}
			/*
			if(chr->task.task == npct_fight &&
				api->GetEntityPointer(chr->task.target) == this)
			{
				j = -1;
				isEnemy = true;
			}
			*/
		}
		//����������� ����������
		if(!isEnemy) //~!~
		{
			entid_t eid;
			if(eid = api->GetEntityIdWalker(nullptr, chrGroups)())
			{
				if(!api->Send_Message(eid, "sii", "IsEnemy", GetId(), chr->GetId())) continue;
			}
		}
		//���� ��� �� ��� �����
		if(j >= 0)
		{
			float cs = -1.0f;
			if(fc.d2 > 0.0f) cs = (fc.dx*cdx + fc.dz*cdz)/sqrtf(fc.d2);
			if(cs > enemyCos)
			{
				j = i;
				minDst = fc.d2;
				enemyCos = cs;
			}
		}else{
			j = i;
			minDst = fc.d2;
			if(fc.d2 > 0.0f) enemyCos = (fc.dx*cdx + fc.dz*cdz)/sqrtf(fc.d2); else enemyCos = -1.0f;
		}
	}
	if(j >= 0) return (Player *)fndCharacter[j].c;
	return nullptr;
}


void Player::FireFromShootgun()
{
#ifndef _XBOX
	kSMReload = 0.0f;
	entid_t peid;
	if(peid = api->GetEntityIdWalker("sound")())
	{
		api->Send_Message(peid, "lsllll", MSG_SOUND_PLAY, "OBJECTS\\sgboom.wav", 4, false, false, false);
	}
	//������� ������� ������ ��������
	float dx = sinf(ay);
	float dz = cosf(ay);
	CMatrix mtx;
	const auto location = GetLocation();
	location->GetRS()->GetTransform(D3DTS_VIEW, mtx);
	mtx.Transposition();
	CVECTOR src = mtx.Pos() + mtx.Vz()*0.7f;
	api->Send_Message(effects, "sffffff", "SGFireParticles", src.x, src.y - 0.35f, src.z, mtx.Vz().x, mtx.Vz().y, mtx.Vz().z);
	walker_t walker = api->LayerGetWalker("sun_trace");
	COLLIDE * collide = (COLLIDE *)api->CreateService("COLL");
	if(!walker) return;
	if(!collide)
	{
		return;
	}
	struct ChrsDmg
	{
		Character * chr;
		float dmg;
	};
	ChrsDmg chrs[16];
	long numChrs = 0;
	for(long i = 0; i < 6; i++)
	{
		//������� ������� ���� ������ ���������
		float r = rand()*3.0f/RAND_MAX;
		float a = rand()*6.283185307f/(RAND_MAX + 1);
		CVECTOR dst = mtx*CVECTOR(r*sinf(a), r*cosf(a), 25.0f);
		if(walker && collide)
		{
			auto id = GetId();
			float dist = collide->Trace(walker, src, dst, &id, 0);
			if(dist <= 1.0f && dist > (0.2f/25.0f))
			{
				CVECTOR dir = !(src - dst);
				dst = src + (dst - src)*dist;
				//���� �� ������
				Entity * e = api->GetEntityPointer(collide->GetObjectID());
				if(e && e != this)
				{
					long n, nm;
					for(n = 0, nm = location->supervisor.numCharacters; n < nm; n++)
					{
						Player * c = (Player *)location->supervisor.character[n].c;
						if(c->Model() == e)
						{
							api->Send_Message(effects, "sffffff", "SGBloodParticles", dst.x, dst.y, dst.z, dir.x, dir.y, dir.z);
							c->impulse -= dir*(1.5f + rand()*(1.0f/RAND_MAX));
							c->impulse.y += 1.5f + rand()*(1.0f/RAND_MAX);
							long j;
							for(j = 0; j < numChrs; j++)
							{
								if(chrs[j].chr == c)
								{
									chrs[j].dmg *= 2.0f;
									break;
								}
							}
							if(j >= numChrs && numChrs < 16)
							{
								chrs[numChrs].chr = c;
								chrs[numChrs++].dmg = 1.0f;
							}
							break;
						}
					}
					if(n >= nm) api->Send_Message(effects, "sffffff", "SGEnvParticles", dst.x, dst.y, dst.z, dir.x, dir.y, dir.z);
				}else api->Send_Message(effects, "sffffff", "SGEnvParticles", dst.x, dst.y, dst.z, dir.x, dir.y, dir.z);
			}
		}
	}
	for(long i = 0; i < numChrs; i++)
	{
		api->Event("Location_CharacterSGFire", "iif", GetId(), chrs[i].chr->GetId(), chrs[i].dmg);
	}
#endif
}

float Player::GetRotateH()
{
	CONTROL_STATE cs;
	api->Controls->GetControlState("ChrCamSpecMode",cs);
	float dx = 0.f;
	if( cs.state != CST_ACTIVE ) {
		api->Controls->GetControlState("ChrTurnH",cs);
		dx = cs.fValue;
	}
	if(api->Controls->GetControlState("ChrTurnH1",cs)) dx += cs.fValue;
	if(api->Controls->GetControlState("ChrTurnHR",cs))
	{
		if(cs.state == CST_ACTIVE) dx += 12.0f;
	}
	if(api->Controls->GetControlState("ChrTurnHL",cs))
	{
		if(cs.state == CST_ACTIVE) dx -= 12.0f;
	}
	return dx;
}