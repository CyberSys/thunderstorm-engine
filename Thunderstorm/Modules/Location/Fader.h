//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Fader
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Fader_H_
#define _Fader_H_

#include "vmodule_api.h"
#include "dx9render.h"

class Fader : public Entity  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Fader();
	virtual ~Fader();

	//�������������
	bool Init();
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

	//������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX9RENDER * rs;
	IDirect3DSurface9 * renderTarget;
	IDirect3DSurface9 * surface;

	bool isWork;
	bool haveFrame;
	bool fadeIn;
	bool isStart;
	bool isAutodelete;
	bool endFade;
	float fadeSpeed;
	float alpha;
	float w, h;

	bool eventStart;
	bool eventEnd;
	long deleteMe;

	long textureID;
	long tipsID;

public:
	static long numberOfTips;
	static long currentTips;
};

#endif

