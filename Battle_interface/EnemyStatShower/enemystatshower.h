#ifndef _BI_ENEMY_STAT_SHOWER_H_
#define _BI_ENEMY_STAT_SHOWER_H_

#include "../../common_h/dx9render.h"
#include "../../common_h/defines.h"

class MODEL;
class NODE;

class EnemyStatShower : public Entity
{
public:
	EnemyStatShower();
	~EnemyStatShower();

	bool Init();
	void Realize(uint32_t delta_time);
	uint32_t ProcessMessage(MESSAGE & message);

protected:
	bool ReadAndCreate();
	void SetBeginData();
	void Release();

	entid_t GetModelEIDFromCharacterEID(entid_t& chrEID);

protected: // data
	bool m_bVisible;
	entid_t m_eidChrEID;
	MODEL* m_pModel;
};

#endif
