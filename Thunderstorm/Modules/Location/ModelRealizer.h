#ifndef _ModelWraper_H_
#define _ModelWraper_H_

#include "vmodule_api.h"
#include "geometry.h"
#include "dx9render.h"

class Lights;

class LocModelRealizer : public Entity
{
	entid_t eid_model;
	Lights* lights;
	VDX9RENDER* rs;
	VGEOMETRY* gs;
	bool bShow;

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	LocModelRealizer();
	~LocModelRealizer();

	//�������������
	bool Init();
	//����������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);
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

	//���������
	uint32_t ProcessMessage(MESSAGE & message);
};

#endif
