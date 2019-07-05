#ifndef _BATTLE_MANCOMMAND_H_
#define _BATTLE_MANCOMMAND_H_

#include "../battle_command.h"

class BIManCommandList : public BICommandList
{
public:
	BIManCommandList( entid_t& eid, ATTRIBUTES* pA, VDX9RENDER* rs );
	virtual ~BIManCommandList();

	virtual void FillIcons();
	virtual void Init();

protected:
	void Release();

	long CommandAdding();
	long UserIconsAdding();
	long AbilityAdding();
	long AddCancelIcon();
};

#endif
