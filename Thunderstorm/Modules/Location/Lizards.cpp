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
#include "EntityManager.h"


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
	auto loc = EntityManager::GetEntityId("location");
	auto* location = (Location *)EntityManager::GetEntityPointer(loc);
	if(!location) return false;
	//������� ������
	for(long i = 0; i < num; i++) lizard[i].Init(location);
	//����������
	//api->LayerCreate("realize", true, false);
	EntityManager::SetLayerType(REALIZE, EntityManager::Layer::Type::realize);
	EntityManager::AddToLayer(REALIZE, GetId(), 100000);
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



