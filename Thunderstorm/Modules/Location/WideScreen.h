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

#include "vmodule_api.h"
#include "dx9render.h"

class WideScreen : public Entity {
  //--------------------------------------------------------------------------------------------
  //���������������, ������������������
  //--------------------------------------------------------------------------------------------
public:
  WideScreen();
  virtual ~WideScreen();

  //�������������
  bool Init() override;
  //���������
  uint64_t ProcessMessage(MESSAGE& message) override;
  //������
  void Realize(uint32_t delta_time);

  void ProcessStage(Stage stage, uint32_t delta) override {
    switch (stage) {
      //case Stage::execute:
      //	Execute(delta); break;
    case Stage::realize:
      Realize(delta);
      break;
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
  VDX9RENDER* rs; //
  float w, h; //������� ������
  float state; //��������� ������
  float dlt; //����������� ��������� ������
};

#endif
