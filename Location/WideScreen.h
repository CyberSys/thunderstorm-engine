//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WideScreen
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WideScreen_H_
#define _WideScreen_H_

#include "../Common_h/vmodule_api.h"
#include "../Common_h/dx9render.h"

class WideScreen : public Entity  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WideScreen();
	virtual ~WideScreen();

	//�������������
	bool Init();
	//���������
	uint32_t ProcessMessage(MESSAGE & message);
	//������
	void Realize(uint32_t delta_time);
	void ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		//case Stage::EXECUTE:
		//	Execute(delta); break;
		case Stage::REALIZE:
			Realize(delta); break;
			/*case Stage::LOST_RENDER:
				LostRender(delta); break;
			case Stage::RESTORE_RENDER:
				RestoreRender(delta); break;*/
		}
	}

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX9RENDER * rs;	//
	float w, h;			//������� ������
	float state;		//��������� ������
	float dlt;			//����������� ��������� ������
};

#endif

