#ifndef _BI_ITEM_Entity_H_
#define _BI_ITEM_Entity_H_

#include "../bi_defines.h"
#include "../Animation.h"
#include "../vparticle_system.h"
#include "../Entity.h"
#include <string>

class MODEL;
class NODE;

class ItemEntity : public Entity
{
public:
	ItemEntity();
	~ItemEntity();

	bool Init();
	void Realize(uint32_t delta_time);
	uint32_t ProcessMessage(MESSAGE & message);
	void ProcessStage(Stage stage, uint32_t delta) override
	{
		switch (stage)
		{
		//case Stage::execute:
		//	Execute(delta); break;
		case Stage::realize:
			Realize(delta); break;
			/*case Stage::lost_render:
				LostRender(delta); break;
			case Stage::restore_render:
				RestoreRender(delta); break;*/
		}
	}


protected:
	bool ReadAndCreate();
	void SetBeginData();
	void Release();

	void SetModelToPosition(const CMatrix& mtx);
	void SetTechnique( const char* pcTechnique );
	bool TieToLocator(entid_t mdlEID, const char* pcLocName);
	void UnTieFromLocator();
	void EndEventProcess();

	void DrawIntoLocator();

	void SetEventListener(entid_t mdlEID, entid_t mdlToTieEID,const char* pcLocName, const char* pcStartEvent,const char* pcEndEvent);
	entid_t GetModelEIDFromCharacterEID(entid_t chrEID);

protected: // data
	bool m_bVisible;
	entid_t m_eidModel;
	MODEL* m_pModel;
	CMatrix m_mtxpos;

	bool m_bTieToLocator;
	entid_t m_eidTieModel;
	NODE* m_pMdlNode;
	std::string m_sTieLocName;

	class EventListener : public AnimationEventListener
	{
	public:
		//������� �������
		virtual void Event(Animation * animation, long playerIndex, const char * eventName);

		ItemEntity* item;
		std::string m_sStartEvent;
		std::string m_sEndEvent;
		bool m_bStartWaiting;
		entid_t m_eidListenedModel;
		entid_t m_eidToTieModel;
		std::string m_sToTieLocator;
	};
	friend EventListener;
	EventListener m_eventListener;

	VPARTICLE_SYSTEM* m_pParticle;
	bool CreateParticle();
	void DeleteParticle();
};

#endif
