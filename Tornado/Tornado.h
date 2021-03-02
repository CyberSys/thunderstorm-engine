//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Tornado
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Tornado_H_
#define _Tornado_H_

#include "TornadoParticles.h"
#include "NoiseCloud.h"
#include "Debris.h"

class Tornado : public Entity  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Tornado();
	virtual ~Tornado();

	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	uint32_t ProcessMessage(MESSAGE & message);
	void ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		case Stage::execute:
			Execute(delta); break;
		case Stage::realize:
			Realize(delta); break;
			/*case Stage::LOST_RENDER:
				LostRender(delta); break;
			case Stage::RESTORE_RENDER:
				RestoreRender(delta); break;*/
		}
	}

	void SetAlpha(float a);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX9RENDER * rs;

	VSoundService * soundService;
	long sID;

	Pillar pillar;
	TornadoParticles particles;
	NoiseCloud noiseCloud;
	Debris debris;

	float eventCounter;

	float liveTime;
	float galhpa;

	long ib, vb;
};

#endif

