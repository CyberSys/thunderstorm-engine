#include "layer.h"

LAYER::LAYER(char * name,bool ordered,bool system,uint32_t system_flags)
{
	ls.Flags = 0;
	Base_Link = nullptr;
	ls.Ordered = ordered;
	ls.System = system;
	ls.Deleted = false;
	Count = 0;
	ls.System_flags = system_flags;
	strcpy_s(Name,name);
	walkers_num = 0;
} 

LAYER::~LAYER()
{
	while(Base_Link) {Del(Base_Link->id);}
	for(uint32_t n=0;n<walkers_num;n++)
	{
		if(walkers[n]) 
		{
			VIDWALKER* pInterface = walkers[n]->pInterface;
			if(pInterface) pInterface->Invalidate();
			delete walkers[n];
		}
	}
}

bool LAYER::Add(entid_t eid)
{
	LAYER_NODE* ln_PTR = (LAYER_NODE *)new LAYER_NODE;
	if(ln_PTR == nullptr) return false;
	ln_PTR->id = eid;
	ln_PTR->link_R = nullptr;
	ln_PTR->link_L = Base_Link;
	ln_PTR->priority = 0;
	if(Base_Link) Base_Link->link_R = ln_PTR;
	Base_Link = ln_PTR;
	Count++;
	return true;
}

bool LAYER::Add(entid_t eid, uint32_t priority)
{
	if(!ls.Ordered) return Add(eid);


	LAYER_NODE* ln_PTR = (LAYER_NODE *)new LAYER_NODE;
	if(ln_PTR == nullptr) return false;
	ln_PTR->id = eid;
	ln_PTR->priority = priority;

	if(Base_Link == nullptr) 
	{
		ln_PTR->link_R = nullptr;
		ln_PTR->link_L = Base_Link;
		if(Base_Link) Base_Link->link_R = ln_PTR;
		Base_Link = ln_PTR;
		Count++;
		return true;
	}
	
	LAYER_NODE* scanln_PTR = Base_Link;
	while(scanln_PTR)
	{
		if(scanln_PTR->priority >= priority)
		{
			ln_PTR->link_L = scanln_PTR;
			ln_PTR->link_R = scanln_PTR->link_R;
			scanln_PTR->link_R = ln_PTR;
			if(ln_PTR->link_R) (ln_PTR->link_R)->link_L = ln_PTR;
			if(scanln_PTR == Base_Link) Base_Link = ln_PTR;
			Count++;
			return true;
		}
		if(scanln_PTR->link_L == nullptr)
		{
			scanln_PTR->link_L = ln_PTR;
			ln_PTR->link_R = scanln_PTR;
			ln_PTR->link_L = nullptr;
			Count++;
			return true;
		}
		scanln_PTR = scanln_PTR->link_L;
	} 
	throw std::exception("cant add to layer");
	//UNGUARD
	return false;
}

bool LAYER::Del(entid_t eid)
{
	LAYER_NODE* ln_PTR = Base_Link;
	do
	{
		if(ln_PTR) 
		{
			if(memcmp(&ln_PTR->id,&eid,sizeof(entid_t)) == 0)
			{
				LAYER_NODE* link_L = ln_PTR->link_L;
				LAYER_NODE* link_R = ln_PTR->link_R;

				if(link_L) link_L->link_R = link_R;
				if(link_R) link_R->link_L = link_L;
				if(ln_PTR == Base_Link) Base_Link = link_L;
				
				delete ln_PTR;
				Count--;
				if(Search_Link == ln_PTR) Search_Link = link_L;
				
				for(uint32_t n=0;n<walkers_num;n++)
				{
					if(walkers[n]) 
					{
						if(walkers[n]->Search_Link == (LPBYTE)ln_PTR) walkers[n]->Search_Link = (LPBYTE)link_L;
					}
				}
				return true;
			}
			ln_PTR = ln_PTR->link_L;
		}
	} while(ln_PTR);

	return false;
}

uint32_t LAYER::GetPriority(entid_t eid)
{
	LAYER_NODE* ln_PTR = Base_Link;
	do
	{
		if(ln_PTR)
		{
			if(memcmp(&ln_PTR->id,&eid,sizeof(entid_t)) == 0) return ln_PTR->priority;
			ln_PTR = ln_PTR->link_L;
		}
	} while(ln_PTR);
	return 0;
}

entid_t * LAYER::WalkerGetID(LPBYTE& sl)
{
	if(ls.Deleted) return nullptr;
	if(Base_Link)
	{
		sl = (LPBYTE)Base_Link;
		return &Base_Link->id;
	}
	sl = nullptr;
	return nullptr;
}

entid_t * LAYER::WalkerGetNextID(LPBYTE& sl)
{
	if(ls.Deleted) return nullptr;
	if(sl)
	{
		LAYER_NODE* ln_PTR = (LAYER_NODE *)sl;
		sl = (LPBYTE)ln_PTR->link_L;
		ln_PTR = (LAYER_NODE *)sl;
		if(sl == nullptr) return nullptr;
		return &ln_PTR->id;
	}
	return nullptr;
}



entid_t * LAYER::GetID()
{
	if(ls.Deleted) return nullptr;
	if(Base_Link)
	{
		Search_Link = Base_Link;
		return &Base_Link->id;
	}
	Search_Link = nullptr;
	return nullptr;
}

entid_t * LAYER::GetNextID()
{
	if(ls.Deleted) return nullptr;
	if(Search_Link)
	{
		Search_Link = Search_Link->link_L;
		if(Search_Link == nullptr) return nullptr;
		return &Search_Link->id;
	}
	return nullptr;
}

entid_t * LAYER::GetID(uint32_t * priority_PTR)
{
	if(ls.Deleted) return nullptr;
	if(Base_Link)
	{
		Search_Link = Base_Link;
		if(priority_PTR) *priority_PTR = Base_Link->priority;
		return &Base_Link->id;
	}
	Search_Link = nullptr;
	return nullptr;
}

entid_t * LAYER::GetNextID(uint32_t * priority_PTR)
{
	if(ls.Deleted) return nullptr;
	if(Search_Link)
	{
		Search_Link = Search_Link->link_L;
		if(Search_Link == nullptr) return nullptr;
		if(priority_PTR) *priority_PTR = Search_Link->priority;
		return &Search_Link->id;
	}
	return nullptr;
}

void LAYER::SetFlag(uint32_t flag)
{
	ls.Flags |= flag;
}

void LAYER::ClrFlag(uint32_t flag)
{
	ls.Flags &= (~flag);
}


VIDWALKER * LAYER::GetWalker()
{
	walkers.resize(walkers_num + 1); 

	walkers[walkers_num] = new IDWALKER;

	TIDWALKER* tidw = (TIDWALKER *)new TIDWALKER(walkers[walkers_num]);
	walkers[walkers_num]->SetLayer((void *)this);
	walkers[walkers_num]->SetInterface((void *)tidw);
	walkers_num++;

	return tidw;
}

void LAYER::DelWalker(void * pw)
{
	for(uint32_t n = 0;n<walkers_num;n++)
	{
		if((void *)walkers[n] == pw)
		{
			if(walkers_num == 1)
			{
				delete walkers[0];
				walkers_num = 0;
				return;
			}
			delete walkers[n];
			for(;n<(walkers_num - 1);n++)
			{
				walkers[n] = walkers[n + 1];
			}
			walkers_num--;
			walkers.resize(walkers_num);
			return;
		}
	}
}

void IDWALKER::Invalidate()
{
	if(pLayer) ((LAYER *)pLayer)->DelWalker((void *)this);
}

entid_t * IDWALKER::GetID()
{
	if(pLayer) return ((LAYER *)pLayer)->WalkerGetID(Search_Link);
	return nullptr;
}

entid_t * IDWALKER::GetIDNext()
{
	if(pLayer) return ((LAYER *)pLayer)->WalkerGetNextID(Search_Link);
	return nullptr;
}
