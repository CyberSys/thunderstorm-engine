//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocRats
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocRats_h_
#define _LocRats_h_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"
#include "..\common_h\dx9render.h"
#include "..\common_h\collide.h"

#include "LocRat.h"


class LocRats : public ENTITY  
{
public:
	LocRats();
	virtual ~LocRats();

//--------------------------------------------------------------------------------------------
public:
	//�������������
	bool Init();
	//����������
	void Execute(dword delta_time);
	//���������
	void Realize(dword delta_time);

	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);

//--------------------------------------------------------------------------------------------
private:
	LocRat rat[32];
	long num;
};

#endif

