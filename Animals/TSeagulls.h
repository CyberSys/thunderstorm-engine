#ifndef _TSEAGULLS_H_
#define _TSEAGULLS_H_

#include "../Common_h/Matrix.h"
#include "../Common_h/geometry.h"
#include "../Common_h/dx9render.h"
#include "../SoundService/VSoundService.h"
#include "AnimalsDefines.h"

///////////////////////////////////////////////////////////////////
// DEFINES & TYPES
///////////////////////////////////////////////////////////////////
struct tSeagull
{
	CVECTOR center;
	float radius;
	float va;
	float height;
	float deltaA;
	float a;

	long circleTime;
	long circleTimePassed;
	long screamTime;
};

///////////////////////////////////////////////////////////////////
// CLASS DEFINITION
///////////////////////////////////////////////////////////////////
class TSeagulls
{
public:
	TSeagulls();
	virtual ~TSeagulls();

	uint32_t ProcessMessage(long _code, MESSAGE & message);
	void Init();
	void Add(float _x, float _y, float _z);
	void Realize(uint32_t _dTime);
	void Execute(uint32_t _dTime);
	void SetStartY (float _startY) {startY = _startY;}

private:
	void LoadSettings();
	void Frighten();

	entid_t  seagullModel;
	tSeagull seagulls[SEAGULL_COUNT];
	VDX9RENDER *renderService;
	VSoundService *soundService;
	bool enabled;
	long count;
	float maxDistance;
	float maxRadius;
	float maxAngleSpeed;
	float maxHeight;
	long countAdd;
	long maxCircleTime;
	long farChoiceChance;
	long relaxTime;
	bool frightened;
	long frightenTime;
	long screamTime;
	char screamFilename[256];

	CVECTOR cameraPos, cameraAng;
	float startY;
};

#endif // !defined
