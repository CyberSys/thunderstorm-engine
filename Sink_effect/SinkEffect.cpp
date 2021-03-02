#include <stdio.h>
#include <stdlib.h>
#include "../Shared/messages.h"
#include "../Common_h/Cvector.h"
#include "../SoundService/VSoundService.h"
#include "SinkEffect.H"
#include "../Common_h/ship_base.h"

INTERFACE_FUNCTION
CREATE_CLASS(SINKEFFECT)

//--------------------------------------------------------------------
SINKEFFECT::SINKEFFECT()
	:sea(nullptr)
	,renderer(nullptr)
{
}

//--------------------------------------------------------------------
SINKEFFECT::~SINKEFFECT()
{
	//GUARD(SINKEFFECT::~SINKEFFECT)

	//UNGUARD
}

//--------------------------------------------------------------------
bool SINKEFFECT::Init()
{
	//GUARD(SINKEFFECT::Init)

	entid_t seaID = api->GetEntityIdWalker("sea")();
	sea = (SEA_BASE*) EntityManager::GetEntityPointer(seaID);

	renderer = (VDX9RENDER *) api->CreateService("dx9render");

	InitializeSinks();

	return true;
	//UNGUARD
}

//--------------------------------------------------------------------
uint32_t SINKEFFECT::ProcessMessage(MESSAGE & message)
{
	//GUARD(SINKEFFECT::ProcessMessage)

	entid_t shipID;
	SHIP_BASE *shipBase;
	long code = message.Long();
	uint32_t outValue = 0;

	switch (code)
	{
	case MSG_SHIP_DELETE:
		{
			ATTRIBUTES *attrs = message.AttributePointer();
			if (attrs)
			{
				const auto walker = api->GetEntityIdWalker("ship");
				if(shipID = walker())
				{
					/*
					shipBase = (SHIP_BASE *) EntityManager::GetEntityPointer(shipID);
					if (shipBase->GetACharacter() == attrs)
					{
						TryToAddSink(shipBase->GetPos(), shipBase->GetBoxsize().z / 2.0f);
						return outValue;
					}*/

					do 
					{
						shipBase = (SHIP_BASE *) EntityManager::GetEntityPointer(shipID);
						if (shipBase->GetACharacter() == attrs)
						{
							TryToAddSink(shipBase->GetPos(), shipBase->GetBoxsize().z / 2.0f);
							return outValue;
						}
					} while (shipID = walker());
				}//if (FindClass)
			}//if (attrs)
		}//case
		break;
	}

	return outValue;
	//UNGUARD
}

//--------------------------------------------------------------------
void SINKEFFECT::Realize(uint32_t _dTime)
{
	//GUARD(SINKEFFECT::Realize)

	for (int i = 0; i < MAX_SINKS; ++i)
		sinks[i].Realize(_dTime);

	//UNGUARD
}

//--------------------------------------------------------------------
void SINKEFFECT::Execute(uint32_t _dTime)
{
	//GUARD(SINKEFFECT::Execute)
/*
	if (GetAsyncKeyState('X'))
	{
		if (renderer && sea)
		{
			static CVECTOR pos, ang, nose, head;
			static CMatrix view;
			//CVECTOR dir(randCentered(2.0f), -1.0f, randCentered(2.0f));
			CVECTOR dir(0.0f, -1.0f, 0.0f);

			renderer->GetTransform(D3DTS_VIEW, view);
			view.Transposition();
			nose = view.Vz();
			//head = view.Vy();
			pos = view.Pos();

			pos += 10.0f * !nose;
			pos.y = sea->WaveXZ(pos.x, pos.y);
			TSink *sinks= TryToAddSink(pos, 10.0f);
		}
	}
*/
	for (int i = 0; i < MAX_SINKS; ++i)
		sinks[i].Process(_dTime);

	//UNGUARD
}

//--------------------------------------------------------------------
void SINKEFFECT::InitializeSinks()
{
	INIFILE *psIni = fio->OpenIniFile("resource\\ini\\particles.ini");

	for (int i = 0; i < MAX_SINKS; ++i)
	{
		sinks[i].Release();
		sinks[i].Initialize(psIni, nullptr, sea, renderer);
	}

	delete psIni;
}

//--------------------------------------------------------------------
TSink *SINKEFFECT::TryToAddSink(const CVECTOR &_pos, float _r)
{
	for (int i = 0; i < MAX_SINKS; ++i)
	{
		if (!sinks[i].Enabled())
		{
			sinks[i].Start(_pos, _r);
			return &sinks[i];
		}
	}

	return nullptr;
}

//--------------------------------------------------------------------
