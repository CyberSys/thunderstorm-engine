//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Lighter
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Lighter_H_
#define _Lighter_H_

#include "LightProcessor.h"


class Lighter : public Entity  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Lighter();
	virtual ~Lighter();

	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
	//���������
	uint64_t ProcessMessage(MESSAGE & message);
	void ProcessStage(Stage stage, uint32_t delta) override
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

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void MsgAddModel(MESSAGE & message);
	void MsgModelsPath(MESSAGE & message);
	void MsgLightPath(MESSAGE & message);
	void MsgAddLight(MESSAGE & message);
	void PreparingData();

private:

	VDX9RENDER * rs;

	LGeometry geometry;
	OctTree octTree;
	Window window;
	LighterLights lights;
	LightProcessor lightProcessor;

	long initCounter;
	float waitChange;
	bool isInited, autoTrace, autoSmooth;
	
};

#endif

