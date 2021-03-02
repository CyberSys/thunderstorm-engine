#ifndef _TBUTTERFLIES_H_
#define _TBUTTERFLIES_H_

#include "AnimalsDefines.h"
#include "TButterfly.h"
#include "Matrix.h"
#include "Animation.h"
#include "geometry.h"
#include "dx9render.h"
#include "collide.h"
#include "TIVBufferManager.h"

#define Y_REDEFINE_TIME		250

///////////////////////////////////////////////////////////////////
// CLASS DEFINITION
///////////////////////////////////////////////////////////////////
class TButterflies
{
public:
	TButterflies();
	virtual ~TButterflies();

	uint32_t ProcessMessage(long _code, MESSAGE & message);
	void Init();
	void Realize(uint32_t dTime);
	void Execute(uint32_t dTime);

private:
	void LoadSettings();

	VDX9RENDER *renderService;
	COLLIDE *collide;
	TIVBufferManager *ivManager;
	entid_t  butterflyModel;
	TButterfly butterflies[BUTTERFLY_COUNT];
	long butterfliesCount;

	float maxDistance;
	
	bool enabled;
	long yDefineTime;
	long texture;
};

#endif // !defined
