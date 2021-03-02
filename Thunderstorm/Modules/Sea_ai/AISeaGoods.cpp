#include "AISeaGoods.h"
#include "../Shared/sea_ai/Script_defines.h"

AISeaGoods::AISeaGoods()
{
	pSea = nullptr;
	pGeoService = nullptr;

	bDeleteGoodAnyway = false;

	dtCheckShips.Setup(FRAND(1.0f), 1.0f);
}

AISeaGoods::~AISeaGoods()
{
	for (uint32_t i=0; i<aGoods.size(); i++)
	{
		if (aGoods[i]->pGeo) pGeoService->DeleteGeometry(aGoods[i]->pGeo);
		aGoods[i]->sModel.clear();
		aGoods[i]->aItems.clear();
		STORM_DELETE(aGoods[i]);
	}
	aGoods.clear();
}

bool AISeaGoods::Init()
{
	SetDevice();
	return true;
}

void AISeaGoods::SetDevice()
{
	pGeoService = (VGEOMETRY*)api->CreateService("geometry"); Assert(pGeoService);
}

void AISeaGoods::Execute(uint32_t dwDeltaTime)
{
	float fDeltaTime = float(dwDeltaTime) * 0.001f;

	if (!pSea)
		pSea = (SEA_BASE*)EntityManager::GetEntityPointer(EntityManager::GetEntityId("sea"));
	
	if (!pSea) return;

	for (uint32_t i=0; i<aGoods.size(); i++)
		for (uint32_t j=0; j<aGoods[i]->aItems.size(); j++)
		{
			item_t * pI = &aGoods[i]->aItems[j];
			pI->fTime -= fDeltaTime;
			
			pI->vPos.y = pSea->WaveXZ(pI->vPos.x, pI->vPos.z, &pI->vNormal);

			if (pI->fTime <= 0.0f)
			{
				pI->vPos.y -= fabsf(pI->fTime) * 0.05f;
				if (pI->fTime < -20.0f)
				{
					//aGoods[i]->aItems.ExtractNoShift(j);
					aGoods[i]->aItems[j] = aGoods[i]->aItems.back();
					aGoods[i]->aItems.pop_back();
					j--;
					continue;
				}
			}

		if (dtCheckShips.Update(dwDeltaTime))
		{
			aShips.clear();

			// enumerate ships
			auto& entities = EntityManager::GetEntityIdVector("ship");
			for (auto ent : entities) {
				aShips.push_back((SHIP_BASE*)EntityManager::GetEntityPointer(ent));
			};

			// check ships
			for (uint32_t k=0; k<aShips.size(); k++)
			{
				SHIP_BASE * pS = aShips[k];
				ATTRIBUTES * pACharacter = pS->GetACharacter();
				int iCharacterIndex = GetIndex(pS->GetACharacter());
				float fDistance = sqrtf(~(pS->State.vPos - pI->vPos));
				if (fDistance <= pS->State.vBoxSize.z * fDistanceMultiply)
				{
					VDATA * pVData = api->Event(SHIP_EAT_SWIM_GOOD, "llsl", iCharacterIndex, pI->iCharIndex, pI->sGoodName, pI->iQuantity);
					if (pVData->GetLong() || bDeleteGoodAnyway)
					{
						//aGoods[i]->aItems.ExtractNoShift(j); 
						aGoods[i]->aItems[j] = aGoods[i]->aItems.back();
						aGoods[i]->aItems.pop_back();
						j--; 
						break;
					}
				}
			}
		}
	}
}

void AISeaGoods::Realize(uint32_t dwDeltaTime)
{
	if (!pSea) return;

	AIHelper::pRS->SetRenderState(D3DRS_LIGHTING, true);

	for (uint32_t i=0; i<aGoods.size(); i++) if (aGoods[i]->pGeo)
		for (uint32_t j=0; j<aGoods[i]->aItems.size(); j++)
		{
			item_t * pI = &aGoods[i]->aItems[j];
	
			// set world matrix for item
			CMatrix m; m.BuildPosition(pI->vPos.x, pI->vPos.y, pI->vPos.z);
			
			AIHelper::pRS->SetTransform(D3DTS_WORLD, m);
			aGoods[i]->pGeo->Draw((GEOS::PLANE*)AIHelper::pRS->GetPlanes(), 0, nullptr);
		}

	AIHelper::pRS->SetRenderState(D3DRS_LIGHTING, false);
}

uint32_t AISeaGoods::AttributeChanged(ATTRIBUTES * pAttribute)
{
	ATTRIBUTES * pParent = pAttribute->GetParent();

	if (*pAttribute == "Add")
	{
		for (uint32_t i=0; i<aGoods.size(); i++) if (aGoods[i]->sModel == sTmpModel)
		{
			aGoods[i]->aItems.push_back(TmpItem);
			return 0;
		}
		goods_t * pG = new goods_t;
		aGoods.push_back(pG);
		pG->sModel = sTmpModel;
		pG->aItems.push_back(TmpItem);
		pG->pGeo = pGeoService->CreateGeometry((sModelPath + "\\" + sTmpModel).c_str(), nullptr, 0);
		return 0;
	}

	if (*pAttribute == "CharIndex") { TmpItem.iCharIndex = pAttribute->GetAttributeAsDword(); return 0; }
	if (*pAttribute == "Time")		{ TmpItem.fTime = pAttribute->GetAttributeAsFloat(); return 0; }
	if (*pAttribute == "Quantity")	{ TmpItem.iQuantity = pAttribute->GetAttributeAsDword(); return 0; }
	if (*pAttribute == "Model")		{ sTmpModel = pAttribute->GetThisAttr(); return 0; }
	if (*pAttribute == "Good")		{ strcpy_s(TmpItem.sGoodName, pAttribute->GetThisAttr()); return 0; }
	
	if (*pParent == "Pos")
	{
		if (*pAttribute == "x")		{ TmpItem.vPos.x = pAttribute->GetAttributeAsFloat(); return 0; }
		if (*pAttribute == "z")		{ TmpItem.vPos.z = pAttribute->GetAttributeAsFloat(); return 0; }
	}

	if (*pAttribute == "ModelsPath")		{ sModelPath = pAttribute->GetThisAttr(); return 0; }
	if (*pAttribute == "DeleteGoodAnyway")	{ bDeleteGoodAnyway = pAttribute->GetAttributeAsDword() == 1; return 0; }
	if (*pAttribute == "DistanceMultiply")	{ fDistanceMultiply = pAttribute->GetAttributeAsFloat(); return 0; }

	return 0;
}
