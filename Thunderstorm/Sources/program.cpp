#include "program.h"

extern CORE Core;

PROGRAM::PROGRAM(): ProgramBlock(nullptr)
{
	ProgramDirectory = nullptr;
	ProgramNum = 0;
	DeleteFile(COMPILER_LOG_FILENAME);
	DeleteFile(COMPILER_ERRORLOG_FILENAME);
}

PROGRAM::~PROGRAM()
{
	Release();
}

void PROGRAM::Release()
{
	delete ProgramDirectory;
	ProgramDirectory = nullptr;
	if (ProgramBlock)
	{
		for (uint32_t n = 0; n < ProgramNum; n++)
		{
			ProgramBlock[n]->Release();
			delete ProgramBlock[n];
		}
		free(ProgramBlock);
		ProgramBlock = nullptr;
	}
}

bool PROGRAM::RunProgram(char* program_name)
{
	if (Running(program_name)) return true;
	uint32_t code = ProgramNum;
	ProgramNum++;
	ProgramBlock = (COMPILER **)realloc(ProgramBlock, ProgramNum * sizeof(COMPILER *));

	ProgramBlock[code] = new COMPILER;
	ProgramBlock[code]->SetProgramDirectory(ProgramDirectory);
	if (!ProgramBlock[code]->CreateProgram(program_name))
	{
		delete ProgramBlock[code];
		ProgramNum--;
		ProgramBlock = (COMPILER **)realloc(ProgramBlock, ProgramNum * sizeof(COMPILER *));
		return false;
	}
	return ProgramBlock[code]->Run();
}

bool PROGRAM::Running(char* program_name)
{
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		if (strcmp(ProgramBlock[n]->GetName(), program_name) == 0) return true;
	}
	return false;
}

void PROGRAM::StopProgram(char* program_name)
{
}

void PROGRAM::SetProgramDirectory(char* dir_name)
{
	delete ProgramDirectory;
	ProgramDirectory = nullptr;
	if (dir_name)
	{
		const auto len = strlen(dir_name) + 1;
		ProgramDirectory = new char[len];
		memcpy(ProgramDirectory, dir_name, len);
	}
}

VDATA* PROGRAM::ProcessEvent(char* event_name, MESSAGE message)
{
	VDATA* pVD;
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		message.Move2Start();
		pVD = ProgramBlock[n]->ProcessEvent(event_name, message);
	}
	return pVD; // ??? maybe a should take off programs mechanizm
}

VDATA* PROGRAM::ProcessEvent(char* event_name)
{
	VDATA* pVD;
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		pVD = ProgramBlock[n]->ProcessEvent(event_name);
	}
	return pVD;
}

void PROGRAM::ProcessFrame(uint32_t DeltaTime)
{
	uint32_t old_size = ProgramNum;
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n]->ProcessFrame(DeltaTime);
		if (!ProgramBlock[n]->Completed()) continue;
		ProgramBlock[n]->Release();
		delete ProgramBlock[n];
		for (uint32_t i = n; i < (ProgramNum - 1); i++)
		{
			ProgramBlock[i] = ProgramBlock[i + 1];
		}
		ProgramNum--;
	}
	if (old_size != ProgramNum)
		ProgramBlock = (COMPILER **)realloc(ProgramBlock, ProgramNum * sizeof(COMPILER *));
}

void PROGRAM::ClearEvents()
{
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n]->ClearEvents();
	}
}

bool PROGRAM::SaveState(HANDLE fh)
{
	fio->_WriteFile(fh, &ProgramNum, sizeof(ProgramNum), nullptr);
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n]->SaveState(fh);
	}
	return true;
}

bool PROGRAM::LoadState(HANDLE fh)
{
	Release();
	fio->_ReadFile(fh, &ProgramNum, sizeof(ProgramNum), nullptr);
	ProgramBlock = (COMPILER **)realloc(ProgramBlock, ProgramNum * sizeof(COMPILER *));
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n] = new COMPILER;
		if (!ProgramBlock[n]->LoadState(fh)) return false;
	}
	return true;
}

bool PROGRAM::OnLoad()
{
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n]->OnLoad();
	}
	return true;
}

void PROGRAM::AddPostEvent(S_EVENTMSG* pEM)
{
	for (uint32_t n = 0; n < ProgramNum; n++)
	{
		ProgramBlock[n]->AddPostEvent(pEM);
	}
}
