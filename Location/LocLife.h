//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocLife
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocLife_h_
#define _LocLife_h_

#include "../Common_h/vmodule_api.h"
#include "../Common_h/Animation.h"

class Location;
class Animation;

class LocLife : public AnimationEventListener
{
public:
	LocLife();
	virtual ~LocLife();

//--------------------------------------------------------------------------------------------
public:
	bool Init(Location * loc);
	void Update(float dltTime);

//--------------------------------------------------------------------------------------------
protected:
	virtual const char * GetModelName() = 0;
	virtual const char * GetAniName() = 0;
	virtual bool PostInit(Animation * ani) = 0;

	virtual void IdleProcess(Animation * ani, float dltTime) = 0;
	virtual void MoveProcess(Animation * ani, float dltTime) = 0;
	virtual void IsStartMove(Animation * ani) = 0;
	virtual void IsStopMove(Animation * ani) = 0;

//--------------------------------------------------------------------------------------------
protected:
	void StartMove();
	void StopMove();
	bool IsNearPlayer(float radius);

private:
	long FindPos();
	long FindRandomPos(CVECTOR & pos);


//--------------------------------------------------------------------------------------------
protected:
	float kSpeed;
	float speed;
private:
	Location * location;
	ENTITY_ID model;
	long node;
	float ay;
	CVECTOR pos;
	CVECTOR npos;

};

#endif

