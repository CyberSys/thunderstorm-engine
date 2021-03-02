#ifndef _POSTEVENTS_H_
#define _POSTEVENTS_H_

#include "../../Common_h/memop.h"
#include "s_eventmsg.h"

class POSTEVENTS_LIST
{
	S_EVENTMSG ** pTable;
	uint32_t		 nClassesNum;
public:
	 POSTEVENTS_LIST(){nClassesNum = 0; pTable = nullptr; };
	~POSTEVENTS_LIST(){Release();};
	void Release()
	{
		uint32_t n;
		if(pTable)
		{
			for(n=0;n<nClassesNum;n++) delete pTable[n];
			delete pTable;
			pTable = nullptr;
		}
		nClassesNum = 0;
	};
	void Add(S_EVENTMSG * pClass)
	{
		uint32_t n;
		n = nClassesNum;
		nClassesNum++;
		pTable = (S_EVENTMSG**)RESIZE(pTable,nClassesNum*sizeof(S_EVENTMSG*));
		pTable[n] = pClass;
	};
	void Del(uint32_t _n)
	{
		uint32_t n;
		if(_n >= nClassesNum) return;
		delete pTable[_n];
		for(n=_n;n<(nClassesNum-1);n++) pTable[n] = pTable[n+1];
		nClassesNum--;
	}
	S_EVENTMSG * Read(uint32_t _n)
	{
		if(_n >= nClassesNum) return nullptr;
		return pTable[_n];
	};
	uint32_t GetClassesNum() {return nClassesNum;}
	void InvalidateAll()
	{
		uint32_t n;
		if(pTable)
		{
			for(n=0;n<nClassesNum;n++) pTable[n]->Invalidate();
		}
	};
	void RemoveInvalidated()
	{
		uint32_t n;
		if(pTable)
		{
			for(n=0;n<nClassesNum;n++) 
			{
				if(pTable[n]->bInvalide)
				{
					Del(n);
					n = 0;
				}
			}	
		}
	}

};


#endif