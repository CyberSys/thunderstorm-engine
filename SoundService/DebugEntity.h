#ifndef _SOUND_VISUALISATION_H_
#define _SOUND_VISUALISATION_H_


#include "../Common_h/dx9render.h"
#include "../Common_h/entity.h"

class SoundService;

class SoundVisualisationEntity : public Entity
{
	SoundService* pSound;

public:

  SoundVisualisationEntity();
  virtual ~SoundVisualisationEntity();

	virtual bool Init();
	virtual void Realize(uint32_t dTime);
  virtual void Execute(uint32_t dTime);
  void ProcessStage(Stage stage, uint32_t delta) override
  {
	  switch (stage)
	  {
	  case Stage::EXECUTE:
		  Execute(delta); break;
	  case Stage::REALIZE:
		  Realize(delta); break;
		  /*case Stage::LOST_RENDER:
			  LostRender(delta); break;
		  case Stage::RESTORE_RENDER:
			  RestoreRender(delta); break;*/
	  }
  }

	void SetMasterSoundService (SoundService* pSoundService);
	void Wakeup();
  
};


#endif