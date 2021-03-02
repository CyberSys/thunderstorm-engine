//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocRats
//--------------------------------------------------------------------------------------------
//	
//============================================================================================
			

#include "LocRats.h"

//============================================================================================

LocRats::LocRats()
{
	num = sizeof(rat)/sizeof(LocRat);
}

LocRats::~LocRats()
{

}


//============================================================================================


//�������������
bool LocRats::Init()
{
	//��������� �� �������
	entid_t loc = api->GetEntityIdWalker("location")();
	Location * location = (Location *)EntityManager::GetEntityPointer(loc);
	if(!location) return false;
	//����������
	//api->LayerCreate("realize", true, false);
	api->LayerSetRealize("realize", true);
	EntityManager::AddToLayer(REALIZE, GetId(), 100000);
	return true;
}

//���������
uint32_t LocRats::ProcessMessage(MESSAGE & message)
{
	long num = message.Long();
	if(num < 1) num = 1;
	if(num > sizeof(rat)/sizeof(LocRat)) num = sizeof(rat)/sizeof(LocRat);
	//��������� �� �������
	entid_t loc = api->GetEntityIdWalker("location")();
	Location * location = (Location *)EntityManager::GetEntityPointer(loc);
	if(!location) return 0;
	//������� ����
	for(long i = 0; i < num; i++) rat[i].Init(location);
	return 1;
}

//����������
void LocRats::Execute(uint32_t delta_time)
{
}

//���������
void LocRats::Realize(uint32_t delta_time)
{
	float dltTime = delta_time*0.001f;
	for(long i = 0; i < num; i++) rat[i].Update(dltTime);

}





