#ifndef _TFLOTSAM_H_
#define _TFLOTSAM_H_

#include "sea_base.h"
#include "geometry.h"
#include "model.h"
#include "SinkSplashDefines.h"

enum tFlotsamState
{
	FLOTSAM_RISE,
	FLOTSAM_FLOAT,
	FLOTSAM_SINK
};

///////////////////////////////////////////////////////////////////
// CLASS DEFINITION
///////////////////////////////////////////////////////////////////
class TFlotsam  
{
public:
	TFlotsam();
	virtual ~TFlotsam();

	void Start(float _x, float _z, float _radius);
	void Initialize(SEA_BASE *_sea);
	void Process(uint32_t dTime);
	void Realize(uint32_t dTime);
	bool Enabled();
private:
	SEA_BASE *sea;

	float x, z, y, vX, vZ, vY, turnY, angY;
	tFlotsamState state;
	uint32_t floatTime, maxFloatTime;
	bool enabled;
	//MODEL *model;
	entid_t ModelID;

	static MODEL * models[FLOTSAM_MODELS_COUNT];
	static char modelNames[FLOTSAM_MODELS_COUNT][128];
	static int modelsInitialized;
	static entid_t modelIDs[FLOTSAM_MODELS_COUNT];
};

#endif
