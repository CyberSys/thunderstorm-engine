#include "particles.h"
#include "psystem.h"
#include "../../Common_h/particles.h"
#include "../../Common_h/defines.h"
#include "../../Common_h/filesystem.h"
#include "../../Shared/messages.h"
#include "../../Common_h/EntityManager.h"

PARTICLES::PARTICLES()
{
	bSystemDelete = false;
	pService = nullptr;
	pManager = nullptr;
	CreationCapture = false;
}

PARTICLES::~PARTICLES()
{
	bSystemDelete = true;
	DeleteAll ();
}

bool PARTICLES::Init ()
{
	EntityManager::AddToLayer(REALIZE,GetId(),0xfffff);
	EntityManager::AddToLayer(EXECUTE,GetId(),0);

	pService = (IParticleService*)api->CreateService("ParticleService");
	Assert (pService);
	pManager = pService->DefManager();
	Assert (pManager);
	return true;
}


uint32_t PARTICLES::ProcessMessage(MESSAGE & message)
{
	long code = message.Long();

	static char ps_name[MAX_PATH];
	CVECTOR pos, angles;
	long lifetime;

	switch (code)
	{
	//��������� ��� �������� �� �����
	// ����� �� ��������� ���������...
	case PS_PAUSEALL:
		{
			PauseAllActive (message.Long() != 0);
			break;
		}

	//������� ���������� �������
	case PS_DELETE:
		{
			DeleteSystem (message.Long());
			break;
		}



	case PS_CREATIONCAPTURE_BEG:
		{
			CreationCapture = true;
			break;
		}
	case PS_CREATIONCAPTURE_END:
		{
			CreationCapture = false;
			break;
		}
	case PS_CLEAR_CAPTURED:
		{
			DeleteCaptured ();
			break;
		}



	//������� ���
	case PS_CLEARALL:
		{
			DeleteAll ();
			break;
		}
	//������� ������� (string ���, float x,y,z �������, float rx, ry, rz ��������, float life_time ����� �����)
	case PS_CREATE_RIC:
		{
			message.String(sizeof(ps_name),ps_name);
			pos.x = message.Float();
			pos.y = message.Float();
			pos.z = message.Float();

			angles.x = message.Float();
			angles.y = message.Float();
			angles.z = message.Float();
			lifetime = message.Long();

			PARTICLE_SYSTEM* pSystem = CreateSystem (ps_name, lifetime);
			if (!pSystem) return 0;

			pSystem->SetEmitter(pos,angles);
			pSystem->SetDelay(0);
			pSystem->SetLifeTime(lifetime);
			return (long)pSystem;

			break;
		}
	case PS_CREATEX_RIC:
		{
			throw std::exception("Unsupported particle manager command !!!");
		}

	//������� �������
	case PS_CREATE:
		{
			message.String(sizeof(ps_name),ps_name);
			pos.x = message.Float();
			pos.y = message.Float();
			pos.z = message.Float();

			angles.x = message.Float();
			angles.y = message.Float();
			angles.z = message.Float();
			lifetime = message.Long();

			PARTICLE_SYSTEM* pSystem = CreateSystem (ps_name, lifetime);
			if (!pSystem) return 0;


			pSystem->SetEmitter(pos,angles);
			pSystem->SetDelay(0);
			pSystem->SetLifeTime(lifetime);
			return (long)pSystem;

			break;
		}
	//������� �������
	case PS_CREATEX:
		{
			message.String(sizeof(ps_name),ps_name);
			pos.x = message.Float();
			pos.y = message.Float();
			pos.z = message.Float();

			Vector normal;
			normal.x = message.Float();
			normal.y = message.Float();
			normal.z = message.Float();
			double fLen = (double)normal.Normalize();

			if (fLen)
			{
				angles.y = normal.GetAY();
				double fDiv = -(normal.y / fLen);
				fDiv = Min (Max (fDiv, -1.0), 1.0);
				angles.x = (float)asin (fDiv);
			} else
				{
					angles.x = 0.0f;
					angles.y = 0.0f;
				}

			angles.z = 0.0f;

			lifetime = message.Long();

			PARTICLE_SYSTEM* pSystem = CreateSystem (ps_name, lifetime);
			if (!pSystem) return 0;

			pSystem->SetEmitter(pos, angles);
			pSystem->SetDelay(0);
			pSystem->SetLifeTime(lifetime);
			return (long)pSystem;

			break;
		}
	case PS_ADDTRACKPOINT:
		{
			throw std::exception("Unsupported particle manager command !!!");
		}
	case PS_USESURFACE:
		{
			throw std::exception("Unsupported particle manager command !!!");
		}
	case PS_VALIDATE_PARTICLE:
		{
			long SystemID = message.Long();
			for (uint32_t n = 0; n < CreatedSystems.size(); n++)
				if (CreatedSystems[n].pSystem == (PARTICLE_SYSTEM*)SystemID)
					return 1;
			return 0;
			break;
		}
	}
	return 0;


}


PARTICLE_SYSTEM* PARTICLES::CreateSystem (const char* pFileName, uint32_t LifeTime)
{
	//std::string pFullFileName;
	//pFullFileName = "resource\\particles\\";
	//pFullFileName += pFileName;
	//pFullFileName.AddExtention(".xps");
	//__debugbreak(); //~!~
	fs::path path = fs::path() / "resource" / "particles" / pFileName;
	std::string pathStr = path.extension().string();
	if (_stricmp(pathStr.c_str(), ".xps") != 0)
		path += ".xps";
	pathStr = path.string();
	//MessageBoxA(NULL, (LPCSTR)path.c_str(), "", MB_OK); //~!~

	//api->Trace("K2 Particles Wrapper: Create system '%s'", pFileName);
	IParticleSystem* pSys = pManager->CreateParticleSystemEx(pathStr.c_str(), __FILE__, __LINE__);
	if (!pSys)
	{
		//api->Trace("Can't create particles system '%s'", pFileName);
		return nullptr;
	}

	pSys->AutoDelete(false);

	PARTICLE_SYSTEM* pNewPS = new PARTICLE_SYSTEM(pSys);
	pNewPS->SetManager(this);



	//api->Trace("PSYS Created ok");

	SystemInfo Info;
	Info.pSystem = pNewPS;
	Info.LifeTime = LifeTime;
	Info.FileName = pathStr;
	CreatedSystems.push_back(Info);

	if (CreationCapture)
	{
		CaptureBuffer.push_back((long)pNewPS);
	}


	return pNewPS;
}

void PARTICLES::DeleteSystem (long SystemID)
{
	bSystemDelete = true;
	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		if (CreatedSystems[n].pSystem == (PARTICLE_SYSTEM*)SystemID)
		{
			//api->Trace("Delete particles system with name '%s'", CreatedSystems[n].FileName.c_str());
			delete CreatedSystems[n].pSystem;
			//CreatedSystems.ExtractNoShift(n);
			CreatedSystems[n] = CreatedSystems.back();
			CreatedSystems.pop_back();
			bSystemDelete = false;
			return;
		}
	}

	//api->Trace("Can't delete particle system with GUID %d", SystemID);

	bSystemDelete = false;
}

void PARTICLES::DeleteAll ()
{
	bSystemDelete = true;
	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		delete CreatedSystems[n].pSystem;
	}

	CreatedSystems.clear();
	bSystemDelete = false;
}

void PARTICLES::DeleteResource (PARTICLE_SYSTEM* pResource)
{
	if (bSystemDelete) return;

	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		if (CreatedSystems[n].pSystem == pResource)
		{
			//CreatedSystems.ExtractNoShift(n);
			CreatedSystems[n] = CreatedSystems.back();
			CreatedSystems.pop_back();
			return;
		}
	}
}

void PARTICLES::Realize(uint32_t Delta_Time)
{
	bSystemDelete = true;
	float fDeltaTime = (float)Delta_Time * 0.001f;
	pManager->Execute(fDeltaTime);


	//���� �����, ������ ������������ �� �����
	// ����� ��� �������� ����� ������� ��������� ����...
	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		CreatedSystems[n].PassedTime +=	Delta_Time;

		if (CreatedSystems[n].LifeTime == 0) continue;

		if (CreatedSystems[n].PassedTime > CreatedSystems[n].LifeTime)
		{
			//CreatedSystems[n].pSystem->Pause(true);
			CreatedSystems[n].pSystem->Stop();
		}
	}

	//������� ������� �������...
	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		if (!CreatedSystems[n].pSystem->GetSystem()->IsAlive())
		{
			delete CreatedSystems[n].pSystem;
			//CreatedSystems.ExtractNoShift(n);
			CreatedSystems[n] = CreatedSystems.back();
			CreatedSystems.pop_back();
			n--;
		}
	}


	bSystemDelete = false;
}

void PARTICLES::Execute(uint32_t Delta_Time)
{
}

void PARTICLES::PauseAllActive (bool bPaused)
{

	for (uint32_t n = 0; n < CreatedSystems.size(); n++)
	{
		CreatedSystems[n].pSystem->GetSystem()->Restart(0);
		CreatedSystems[n].pSystem->Pause(bPaused);
	}

}


void PARTICLES::DeleteCaptured ()
{
	for (uint32_t n = 0; n < CaptureBuffer.size(); n++)
	{
		DeleteSystem(CaptureBuffer[n]);
	}

	CaptureBuffer.clear();
}
