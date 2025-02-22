//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	LocEagle
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _LocEagle_H_
#define _LocEagle_H_

#include "model.h"

class LocEagle : public Entity {
public:
  LocEagle();
  virtual ~LocEagle();

  //�������������
  bool Init() override;
  //����������
  void Execute(uint32_t delta_time);

  void ProcessStage(Stage stage, uint32_t delta) override {
    switch (stage) {
    case Stage::execute:
      Execute(delta);
      break;
      //case Stage::realize:
      //	Realize(delta); break;
      /*case Stage::lost_render:
        LostRender(delta); break;
      case Stage::restore_render:
        RestoreRender(delta); break;*/
    }
  }


  entid_t mdl;
  CVECTOR cnt;
  float kTime, time;
  float y, dltY;
  float kRad, dltRad;
  float ay;
  float kAy, timeAy;
};

#endif
