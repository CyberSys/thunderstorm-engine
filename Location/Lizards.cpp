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
	entid_t loc;
	api->FindClass(&loc, "location", 0);
	Location * location = (Location *)api->GetEntityPointer(loc);
	if(!location) return false;
	//������� ������
	for(long i = 0; i < num; i++) lizard[i].Init(location);
	//����������
	api->LayerCreate("realize", true, false);
	api->LayerSetFlags("realize", LRFLAG_REALIZE);
	api->LayerAdd("realize", GetId(), 100000);
	return true;
}

//����������
void Lizards::Execute(uint32_t delta_time)
{
}

//���������
void Lizards::Realize(uint32_t delta_time)
{
	float dltTime = delta_time*0.001f;
	for(long i = 0; i < num; i++) lizard[i].Update(dltTime);

}



