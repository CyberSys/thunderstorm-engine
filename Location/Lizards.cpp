//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Lizards
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "Lizards.h"
#include "Location.h"


Lizards::Lizards()
{
	num = sizeof(lizard)/sizeof(Lizard);
}

Lizards::~Lizards()
{

}

//�������������
bool Lizards::Init()
{
	//��������� �� �������
	ENTITY_ID loc;
	_CORE_API->FindClass(&loc, "location", 0);
	Location * location = (Location *)_CORE_API->GetEntityPointer(&loc);
	if(!location) return false;
	//������� ������
	for(long i = 0; i < num; i++) lizard[i].Init(location);
	//����������
	_CORE_API->LayerCreate("realize", true, false);
	_CORE_API->LayerSetFlags("realize", LRFLAG_REALIZE);
	_CORE_API->LayerAdd("realize", GetID(), 100000);
	return true;
}

//����������
void Lizards::Execute(dword delta_time)
{
}

//���������
void Lizards::Realize(dword delta_time)
{
	float dltTime = delta_time*0.001f;
	for(long i = 0; i < num; i++) lizard[i].Update(dltTime);

}



