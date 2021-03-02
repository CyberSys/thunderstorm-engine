//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	ActionPlayerImp
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "AnimationImp.h"
#include "AnimationInfo.h"
#include "AnimationServiceImp.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

ActionPlayerImp::ActionPlayerImp()
{
	ani = null;
	playerIndex = -1;
	action = null;
	isPlay = false;
	isPause = false;
	isAutostop = true;
	anitype = at_normal;
	speed = 1.0f;
	kspeed = 1.0f;
	isLoop = false;
	kBlend = 1.0f;
	kBlendTimer = 1.0f;
	ResetEventsMask();
}

ActionPlayerImp::~ActionPlayerImp()
{

}

//���������� ��������� �� �������� ��������
void ActionPlayerImp::SetAnimation(AnimationImp * animation, long index)
{
	Assert(!ani);
	ani = animation;
	playerIndex = index;
}

//--------------------------------------------------------------------------------------------
//ActionPlayer
//--------------------------------------------------------------------------------------------

//���������� ������� ��������
bool ActionPlayerImp::SetAction(const char * actionName)
{
	if(actionName && action && stricmp(action->GetName(), actionName) == null) return true;
	action = null;
	isPlay = isPause = false;
	isAutostop = true;
	kBlendTimer = 1.0f;
	ResetPosition();
	if(!actionName || actionName[0] == 0)
	{
		ani->ApeSetnewaction(playerIndex);
		return false;
	}
	action = ani->GetActionInfo(actionName);
	if(action == null)
	{
		ani->ApeSetnewaction(playerIndex);
		return false;
	}
	//���������� ���������
	if(action->startFrame != action->endFrame) speed = 0.001f*ani->GetFPS()/(action->endFrame - action->startFrame);
		else speed = 0.0f;
	kspeed = action->kRate;
	anitype = action->type;
	isLoop = action->isLoop;
	//���������� �������
	ResetPosition();
	//�������� �� ���������
	ani->ApeSetnewaction(playerIndex);
	return true;
}

const char * ActionPlayerImp::GetAction() const
{
	if(action) return action->GetName();
	return null;
}

//���������� �������������
bool ActionPlayerImp::Play()
{
	if(!action) return false;
	if(isPlay && !isPause) return true;
	isPlay = true;
	bool p = isPause;	
	if(!isPause)
	{
		ani->ApePlay(playerIndex);
		ani->ApeStart(playerIndex);
	}else ani->ApePauseOff(playerIndex);
	isPause = false;
	return true;
}
									
void ActionPlayerImp::Pause()
{
	isPause = true;
	ani->ApePauseOn(playerIndex);
}

void ActionPlayerImp::Stop()
{
	ResetPosition();
	isPlay = false;
	isPause = false;
	ani->ApeStop(playerIndex);
}

void ActionPlayerImp::SysStop()
{
	if(isPause) return;
	isPlay = false;
	isPause = false;
	ani->ApeStop(playerIndex);
	if(position < 0.0f) position = 0.0f;
	if(position > 1.0f) position = 1.0f;	
	ResetEventsMask();
}


bool ActionPlayerImp::IsPlaying() const
{
	return isPlay;
}

bool ActionPlayerImp::IsPause() const
{
	return isPause;
}

//������������� ��� ���������� ������ �������
bool ActionPlayerImp::SetAutoStop(bool isStop)
{
	bool old = isAutostop;
	isAutostop = isStop;
	return old;
}

bool ActionPlayerImp::IsAutoStop() const
{
	return isAutostop;
}

//������� ������� ������������
float ActionPlayerImp::SetPosition(float position)
{
	float pos = this->position;
	if(position < 0.0f) position = 0.0f;
	if(position > 1.0f) position = 1.0f;
	this->position = position;
	return pos;
}

float ActionPlayerImp::GetPosition() const
{
	return position;
}

//��� ������������
void ActionPlayerImp::SetType(AnimationType atype)
{
	anitype = atype;
}

AnimationType ActionPlayerImp::GetType() const
{
	return anitype;
}

//���������� �������� ������������
float ActionPlayerImp::SetSpeed(float kSpeed)
{
	if(kSpeed < 0.0f) kSpeed = 0.0f;
	if(kSpeed > 10.0f) kSpeed = 10.0f;
	float ks = kspeed;
	kspeed = kSpeed;
	return ks;
}

float ActionPlayerImp::GetSpeed() const
{
	return kspeed;
}

float ActionPlayerImp::GetDefSpeed() const
{
	if(action) return action->kRate;
	return 0.0f;
}

//�������� ������������ �������� � �������������
long ActionPlayerImp::GetFrames() const
{
	if(action) return action->GetFrames();
	return 0;
}

//���������� ���������� ��������� 0..1
void ActionPlayerImp::SetBlend(float k)
{
	if(k < 0.0f) k = 0.0f;
	if(k > 1.0f) k = 1.0f;
	kBlend = k;
}

//�������� ���������� ��������� 0..1
float ActionPlayerImp::GetBlend()
{
	return kBlend;
}

//�������� ���������������� ������ ��� ����� ��������
const char * ActionPlayerImp::GetData(const char * dataName) const
{
	if(!action) return null;
	return action->GetUserData().GetData(dataName);
}

//--------------------------------------------------------------------------------------------
//AnimationImp
//--------------------------------------------------------------------------------------------

//������� ��� �� �������
void ActionPlayerImp::Execute(long dltTime)
{
	kBlendTimer = 1.0f;
	if(!action || !isPlay || isPause || anitype == at_static) return;
	float dlt = dltTime*speed*kspeed;
	switch(anitype)
	{
	case at_static: break;
	case at_normal: MoveNormal(dlt); break;
	case at_reverse: MoveReverse(dlt); break;
	case at_pingpong: MovePingpong(dlt); break;
	case at_rpingpong: MoveRPingpong(dlt); break;
	default:
		throw "ActionPlayerImp::Execute -> anknow animation type";
	}
	if(isPlay) CheckEvents(); else ResetPosition();
}

//���������� ������� � ����� ������	
void ActionPlayerImp::ResetPosition()
{
	position = 0.0f;
	dir = true;
	if(action && (anitype == at_reverse || anitype == at_rpingpong))
	{
		position = 1.0f;
		dir = false;
	}
	ResetEventsMask();
}

//�������� ������� �����
float ActionPlayerImp::GetCurrentFrame()
{
	if(!action) return 0;
	return action->startFrame + position*(action->endFrame - action->startFrame);
}

//���������� ��������� ������� ������
void ActionPlayerImp::CopyState(ActionPlayerImp & from)
{
	//���������� � ������� ��������
	action = from.action;
	//��� ������������ ��������
	anitype = from.anitype;
	//������������
	isPlay = from.isPlay;
	isPause = from.isPause;
	//��������
	isAutostop = from.isAutostop;
	//������� ������������
	position = from.position;
	//����������� �������� true - ����������� ����� ��������
	dir = from.dir;
	//�������� ������������
	speed = from.speed;
	//���������� �������� ������������
	kspeed = from.kspeed;
	//����������� ����������
	isLoop = from.isLoop;
	//����� ��� ������������ �������
	for(long i = 0; i < ACTIONPLAYEREVENTFLAGS; i++)
						eventsMask[i] = from.eventsMask[i];
	//���������� ���������
	kBlend = from.kBlend;
	//����������� ���������� ��������� ��� �������
	kBlendTimer = from.kBlendTimer;
}



//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------

//������������ �������
void ActionPlayerImp::MoveNormal(float dlt)
{
	position += dlt;
	if(position >= 1.0f)
	{
		ani->ApeEnd(playerIndex);
		if(isLoop)
		{
			ResetEventsMask();
			position -= float(long(position));
			ani->ApeStart(playerIndex);
		}else SysStop();
	}
}

void ActionPlayerImp::MoveReverse(float dlt)
{
	position -= dlt;
	if(position <= 0.0f)
	{		
		ani->ApeEnd(playerIndex);
		if(isLoop)
		{
			ResetEventsMask();
			position -= long(position) - 1.0f;
			ani->ApeStart(playerIndex);
		}else SysStop();
	}
}

void ActionPlayerImp::MovePingpong(float dlt)
{
	if(dir)
	{
		//������ ��������
		position += dlt;
		if(position >= 1.0f)
		{
			//������ �����������
			position = 1.0f - (position - float(long(position)));
			dir = false;
			ani->ApeChange(playerIndex);
		}
	}else{
		//�������� ��������
		position -= dlt;
		if(position < 0.0f)
		{			
			ani->ApeEnd(playerIndex);
			if(isLoop)
			{
				//������ �����������
				ResetEventsMask();
				position = -(position - long(position));
				dir = true;
				ani->ApeStart(playerIndex);
			}else SysStop();
		}
	}
}

void ActionPlayerImp::MoveRPingpong(float dlt)
{
	if(!dir)
	{
		//�������� ��������
		position -= dlt;
		if(position < 0.0f)
		{
			ResetEventsMask();
			//������ �����������
			position = -(position - long(position));
			dir = true;
			ani->ApeChange(playerIndex);
		}
	}else{
		//������ ��������
		position += dlt;
		if(position >= 1.0f)
		{
			ani->ApeEnd(playerIndex);
			if(isLoop)
			{
				ResetEventsMask();
				//������ �����������
				position = 1.0f - (position - float(long(position)));
				dir = false;
				ani->ApeStart(playerIndex);
			}else SysStop();
		}
	}
}

//��������� ������� � ���� ���� ������������
void ActionPlayerImp::CheckEvents()
{
	if(!action) return;
	long num = action->GetNumEvents();
	for(long i = 0; i < num; i++)
	{
		long mask = 1 << (i & 31);
		if(eventsMask[i >> 5] & mask) continue;
		if(action->CheckEvent(i, position, dir))
		{
			eventsMask[i >> 5] |= mask;
			//��� ���������������� �������
			ani->AteExtern(playerIndex, action->EventName(i));
		}
	}
}

//����� ������� ������������ �������
void ActionPlayerImp::ResetEventsMask()
{
	for(long i = 0; i < ACTIONPLAYEREVENTFLAGS; i++) eventsMask[i] = 0;
}

