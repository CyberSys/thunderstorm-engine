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
#include "EntityManager.h"

//============================================================================================

LocRats::LocRats()
{
	num = sizeof(rat) / sizeof(LocRat);
}

LocRats::~LocRats()
{
}


//============================================================================================


//�������������
bool LocRats::Init()
{
	//��������� �� �������
	const auto loc = EntityManager::GetEntityId("location");
	auto* location = (Location *)EntityManager::GetEntityPointer(loc);
	if (!location) return false;
	//����������
	//api->LayerCreate("realize", true, false);
	EntityManager::SetLayerType(REALIZE, EntityManager::Layer::Type::realize);
	EntityManager::AddToLayer(REALIZE, GetId(), 100000);
	return true;
}

//���������
uint64_t LocRats::ProcessMessage(MESSAGE& message)
{
  auto num = message.Long();
	if (num < 1) num = 1;
	if (num > sizeof(rat) / sizeof(LocRat)) num = sizeof(rat) / sizeof(LocRat);
	//��������� �� �������
	const auto loc = EntityManager::GetEntityId("location");
	auto* location = (Location *)EntityManager::GetEntityPointer(loc);
	if (!location) return 0;
	//������� ����
	for (long i = 0; i < num; i++) rat[i].Init(location);
	return 1;
}

//����������
void LocRats::Execute(uint32_t delta_time)
{
}

//���������
void LocRats::Realize(uint32_t delta_time)
{
  const auto dltTime = delta_time * 0.001f;
	for (long i = 0; i < num; i++) rat[i].Update(dltTime);
}
