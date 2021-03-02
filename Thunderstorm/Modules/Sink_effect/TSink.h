#pragma once

#include "dx9render.h"
#include "sea_base.h"
#include "Cvector.h"
#include "IVBufferManager.h"
#include "TSinkSplash.h"
#include "TFlotsam.h"

///////////////////////////////////////////////////////////////////
// CLASS DEFINITION
///////////////////////////////////////////////////////////////////
class TSink  
{
public:
	TSink();
	virtual ~TSink();

	void Initialize(INIFILE * _ini, IDirect3DDevice9 *_device, SEA_BASE *sea, VDX9RENDER *_renderer);
	void Release();
	void Start(const CVECTOR &_pos, float _radius);
	void Process(uint32_t dTime);
	void Realize(uint32_t dTime);
	bool Enabled();

private:

	bool enabled;

	VDX9RENDER *renderer;
	SEA_BASE *sea;
	long texture;
	IVBufferManager *ivManager;
	uint32_t time;

	TSinkSplash splashes[MAX_SPLASHES];
	long    times[MAX_SPLASHES];
	long	ivIndexes[MAX_SPLASHES];
	long    flotsamTimes[MAX_FLOTSAMS];
	TFlotsam flotsams[MAX_FLOTSAMS];
	float	distortDivider;
	float	radius;
	CVECTOR center;
};