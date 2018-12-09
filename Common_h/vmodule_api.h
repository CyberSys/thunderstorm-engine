#ifndef _VMODULE_API_H_
#define _VMODULE_API_H_

#include "vapi.h"
#include "memop.h"
#include "Exs.h"

class VMA;

// dll import func definition
#define DLL_MODULE_API_FUNCTION __declspec( dllexport) VMA *  _cdecl
typedef VMA * (__cdecl * DLLAPIFUNC)(VAPI *, VSYSTEM_API *);


#ifdef _ENGINE_IN_

extern VMA * _pModuleClassRoot;
extern VAPI * _CORE_API; 
extern VSYSTEM_API * _VSYSTEM_API; 
extern VAPI * api; 
extern VFILE_SERVICE * fio;
#else

DLL_MODULE_API_FUNCTION DMAInterface();		// import func declaration for dll module
extern VAPI * _CORE_API; 
extern VAPI * api; 
extern VSYSTEM_API * _VSYSTEM_API; 
extern VFILE_SERVICE * fio;

#endif



class VMA
{
protected:
	VMA * pNext;
	long  nHash;
	long  nReference;
public:
	VMA(): pNext(nullptr)
	{
		nReference = 0;
		nHash = 0;
		//pNext = _pModuleClassRoot;
		//_pModuleClassRoot = this;
	}
	;
	VMA * Next(){return pNext;}
	virtual ~VMA(){};
	long Build_Version() {return ENGINE_VERSION;};
	void SetHash(long _hash) {nHash = _hash;}
	long GetHash() {return nHash;}
	void Set(VMA * _p){pNext = _p;};
	virtual bool Service() {return false;}
	virtual char * GetName() {return nullptr;}
	virtual void * CreateClass() {return nullptr;}
	virtual void RefDec(){nReference--;};
	virtual long GetReference(){return nReference;}
	virtual void Clear(){nReference = 0;};
	virtual bool ScriptLibriary() {return false;}
};

#ifndef _ENGINE_IN_
#define ENGINE_MODULE "engine.exe"
#define INTERFACE_FUNCTION	VMA * _pModuleClassRoot = 0; VAPI * _CORE_API; VAPI * api; VFILE_SERVICE * fio; VSYSTEM_API * _VSYSTEM_API; extern "C" DLL_MODULE_API_FUNCTION _cdecl DMAInterface(VAPI * api_PTR,VSYSTEM_API * sapi_PTR){_CORE_API = api_PTR; api = api_PTR; fio = api_PTR->fio; _VSYSTEM_API = sapi_PTR; return _pModuleClassRoot;} \
	void * __cdecl operator new(std::size_t count) { return malloc(count); } \
	void * __cdecl operator new (std::size_t count, std::align_val_t al) { return malloc(count); } \
	void __cdecl operator delete(void * ptr) noexcept { free(ptr); } \
	void __cdecl operator delete(void* ptr, std::align_val_t al) noexcept { free(ptr); } \
	void __cdecl operator delete(void* ptr, std::size_t sz, std::align_val_t al) noexcept { free(ptr); } \
	void * __cdecl resize(void * ptr, std::size_t count){ return realloc(ptr, count); }
#else
#define INTERFACE_FUNCTION
#endif
#define CREATE_CLASS(a)	class a##vmacd : public VMA { public: a##vmacd(VMA * & _pR){pNext = _pR;_pR = this;}; char * GetName() {return TEXT(#a);} void * CreateClass() {nReference++; return NEW a;}}; a##vmacd a##vmaci(_pModuleClassRoot);
#define CREATE_SERVICE(a)	class a##vmacd : public VMA { public: a * pService; a##vmacd(VMA * & _pR){pNext = _pR;_pR = this; pService = 0; }; char * GetName() {return TEXT(#a);} void * CreateClass() {if(pService == 0) pService = NEW a; nReference++; return pService;} bool Service() {return true;} void Clear(){nReference = 0; if(pService) delete pService; pService = 0;};}; a##vmacd a##vmaci(_pModuleClassRoot);
#define CREATE_SCRIPTLIBRIARY(a)	class a##vmacd : public VMA { public: /*a * pLibraryInitClass;*/ a##vmacd(VMA * & _pR){pNext = _pR;_pR = this; /*pLibraryInitClass = 0;*/ }; char * GetName() {return TEXT(#a);} void * CreateClass() {/*if(pLibraryInitClass == 0) pLibraryInitClass = NEW a; nReference++; return pLibraryInitClass;*/ return NEW a;} bool ScriptLibriary() {return true;} /*void Clear(){nReference = 0; if(pLibraryInitClass) delete pLibraryInitClass; pLibraryInitClass = 0;}*/;}; a##vmacd a##vmaci(_pModuleClassRoot);
#endif