#ifndef _BALLSPLASH_H_
#define _BALLSPLASH_H_

#include "geometry.h"
#include "dx9render.h"
#include "sea_base.h"
#include "CVECTOR.h"
#include "BallSplashDefines.h"
#include "TSplash.h"

///////////////////////////////////////////////////////////////////
// CLASS DEFINITION
///////////////////////////////////////////////////////////////////

class BALLSPLASH: public Entity
{
public:
	BALLSPLASH();
	virtual ~BALLSPLASH();

	virtual bool Init();
	virtual uint64_t ProcessMessage(MESSAGE & message);
	virtual void Realize(uint32_t dTime);
	virtual void Execute(uint32_t dTime);
	void BALLSPLASH::ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		case Stage::execute:
			Execute(delta); break;
		case Stage::realize:
			Realize(delta); break;
			/*case Stage::lost_render:
				LostRender(delta); break;
			case Stage::restore_render:
				RestoreRender(delta); break;*/
		}
	}


private:
	void InitializeSplashes();
	TSplash *TryToAddSplash(const CVECTOR &_pos, const CVECTOR &_dir);

	TSplash    splashes[MAX_SPLASHES];
	VDX9RENDER *renderer;
	SEA_BASE   *sea;
};

#endif