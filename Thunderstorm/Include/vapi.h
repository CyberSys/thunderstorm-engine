#pragma once
#include <Windows.h>
#include "controls.h"
#include "s_import_func.h"
#include "vdata.h"

class VAPI
{
public:
	VAPI()
	{
	};

	virtual ~VAPI()
	{
	};

	CONTROLS* Controls;

	virtual void Exit() = 0;

	virtual HWND GetAppHWND() = 0;
	virtual HINSTANCE GetAppInstance() = 0;

	virtual void SetTimeScale(float _scale) = 0;

	virtual void Trace(const char* Format,...) = 0;

	virtual void*CreateService(const char* service_name) = 0;

	virtual ATTRIBUTES* Entity_GetAttributeClass(entid_t id_PTR, const char* name) =0;
	virtual char* Entity_GetAttribute(entid_t id_PTR, const char* name) =0;
	virtual uint32_t Entity_GetAttributeAsDword(entid_t id_PTR, const char* name, uint32_t def = 0) =0;
	virtual FLOAT Entity_GetAttributeAsFloat(entid_t id_PTR, const char* name, FLOAT def = 0) =0;
	virtual BOOL Entity_SetAttribute(entid_t id_PTR, const char* name, const char* attribute) =0;
	virtual BOOL Entity_SetAttributeUseDword(entid_t id_PTR, const char* name, uint32_t val) =0;
	virtual BOOL Entity_SetAttributeUseFloat(entid_t id_PTR, const char* name, FLOAT val) =0;
	virtual void Entity_SetAttributePointer(entid_t id_PTR, ATTRIBUTES* pA) =0;
	virtual ATTRIBUTES* Entity_GetAttributePointer(entid_t id_PTR) =0;

	virtual uint64_t Send_Message(entid_t Destination, const char* Format,...) = 0;

	virtual bool SaveState(const char* file_name) = 0;
	virtual bool InitiateStateLoading(const char* file_name) = 0;

	virtual uint32_t EngineFps() = 0;
	virtual void SetDeltaTime(long delta_time) = 0;
	virtual uint32_t GetDeltaTime() = 0;
	virtual uint32_t GetRDeltaTime() = 0;

	virtual VDATA* Event(const char* Event_name, const char* Format = nullptr,...) = 0;
	virtual uint32_t PostEvent(const char* Event_name, uint32_t post_time, const char* Format,...) = 0;

	virtual void* GetSaveData(const char* file_name, long& data_size) = 0;
	virtual bool SetSaveData(const char* file_name, void* data_ptr, long data_size) = 0;

	virtual uint32_t SetScriptFunction(IFUNCINFO* pFuncInfo) = 0;

	virtual char* EngineIniFileName() =0;
	virtual void* GetScriptVariable(const char* pVariableName, uint32_t* pdwVarIndex = nullptr) =0;
};
