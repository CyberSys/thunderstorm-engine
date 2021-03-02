#pragma once

#include <Windows.h>
#include "vapi.h"

#include "timer.h"
#include "services_list.h"
#include "compiler.h"

#include "EntityManager.h"

#define ENGINE_SCRIPT_VERSION		54128

class CORE : public VAPI
{
public:
	CORE();
	~CORE() = default;
	

	void InitBase();
	void ReleaseBase();

	void CleanUp();
	void Set_Hwnd(HWND _hwnd) {App_Hwnd = _hwnd;};
	bool Initialize();
	void ResetCore();
	bool Run();
	bool LoadClassesTable();

	void ProcessExecute();
	void ProcessRealize();
	void ProcessStateLoading();
	void ProcessRunStart(uint32_t section_code);
	void ProcessRunEnd(uint32_t section_code);

	void CheckAutoExceptions(uint32_t xflag) const;
	void ReleaseServices();
	void ProcessEngineIniFile();

	bool bAppActive{};
	bool Memory_Leak_flag;			// true if core detected memory leak
	bool Root_flag;
	bool Exit_flag;					// true if the program closing 
	bool Initialized;				// initialized flag (false at startup or after Reset())
	bool bEngineIniProcessed;
	HWND App_Hwnd;					// application handle
	char gstring[_MAX_PATH]{};		// general purpose string 
	bool State_loading;
	bool bEnableTimeScale{};
	
	SERVICES_LIST Services_List;	// list for subsequent calls RunStart/RunEnd service functions
	
	HINSTANCE hInstance{};

	char * State_file_name;

	TIMER Timer;

	//INPUT * Input;

	COMPILER Compiler;
	//PROGRAM Program;

	void ProcessControls();


	float fTimeScale;
	long nSplitScreenshotGrid;

	void DumpEntitiesInfo();
	void EraseEntities();
	void ClearEvents();
	void * MakeClass(const char * class_name);
	void AppState(bool state);
	uint32_t MakeHashValue(const char * string);
	VMA * FindVMA(const char * class_name);
	VMA * FindVMA(long hash);
//------------------------------------------------------------------------------------------------
// API functions : (virtual API)

	// common programm control

	// shutdown core, delete all objects and close programm
	void Exit() override;
	// return application handle
	HWND GetAppHWND() override;
	HINSTANCE GetAppInstance() override;
	// set time scale; affect on std entity functions DeltaTime parameter
	void  SetTimeScale(float _scale) override;
	// write message to system log file
	void Trace(const char * Format,...) override;

	// return service object pointer; 
	void * CreateService(const char * service_name) override;

	ATTRIBUTES * Entity_GetAttributeClass(entid_t id_PTR, const char * name) override;
	char *	Entity_GetAttribute(entid_t id_PTR, const char * name) override;
	uint32_t	Entity_GetAttributeAsDword(entid_t id_PTR, const char * name, uint32_t def = 0) override;
	FLOAT	Entity_GetAttributeAsFloat(entid_t id_PTR, const char * name, FLOAT def = 0) override;
	bool	Entity_SetAttribute(entid_t id_PTR, const char * name, const char * attribute) override;
	bool	Entity_SetAttributeUseDword(entid_t id_PTR, const char * name, uint32_t val) override;
	bool	Entity_SetAttributeUseFloat(entid_t id_PTR, const char * name, FLOAT val) override;
	void	Entity_SetAttributePointer(entid_t id_PTR, ATTRIBUTES * pA) override;
	uint32_t	Entity_AttributeChanged(entid_t id_PTR, ATTRIBUTES *);
	ATTRIBUTES * Entity_GetAttributePointer(entid_t id_PTR) override;
	
	// messeges system

	// send message to an object
	uint64_t Send_Message(entid_t Destination, const char * Format,...) override;
	
	// save core state
	bool SaveState(const char * file_name) override;
	// force core to load state file at the start of next game loop, return false if no state file
	bool InitiateStateLoading(const char * file_name) override;

	// return current fps
	uint32_t EngineFps() override;
	// set fixed delta time mode, (-1) - off
	void SetDeltaTime(long delta_time) override;
	uint32_t GetDeltaTime() override;
	uint32_t GetRDeltaTime() override;
	//
	VDATA * Event(const char * Event_name, const char * Format,...) override;
	uint32_t PostEvent(const char * Event_name, uint32_t post_time, const char * Format,...) override;

	void * GetSaveData(const char * file_name, long & data_size) override;
	
	bool SetSaveData(const char * file_name, void * data_ptr, long data_size) override;

	uint32_t SetScriptFunction(IFUNCINFO * pFuncInfo) override;

	char * EngineIniFileName() override;

	void * GetScriptVariable(const char * pVariableName, uint32_t * pdwVarIndex) override;

	bool LoCheck();
};