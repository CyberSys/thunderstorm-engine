#include "TEXT\DIALOGS\Brothel\SantoDomingo_Brothel.h"
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    ref sld; 
	switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple(DLG_TEXT_BR[0], DLG_TEXT_BR[1]), DLG_TEXT_BR[2], DLG_TEXT_BR[3],
                          DLG_TEXT_BR[4], "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple(DLG_TEXT_BR[5], DLG_TEXT_BR[6]), DLG_TEXT_BR[7],
                      DLG_TEXT_BR[8], DLG_TEXT_BR[9], npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (pchar.questTemp.Sharp == "seekSharp" && GetNpcQuestPastDayWOInit(npchar, "quest.SharpTime") > 7 && !LAi_IsDead(&characters[GetCharacterIndex("Sharp")]))
			{
				link.l1 = DLG_TEXT_BR[10] + npchar.name + DLG_TEXT_BR[11];
				link.l1.go = "SharpPearl_1";
				SaveCurrentNpcQuestDateParam(npchar, "quest.SharpTime");
			}
		break;
		// ������� ����, ������ ��������� ���������
		case "SharpPearl_1":
			if (rand(1) || bBettaTestMode)
			{
				if (sti(pchar.questTemp.Sharp.brothelChance) < 9)
				{	//�������� ������� � �����
					pchar.questTemp.Sharp.City = GetSharpCity();
					pchar.questTemp.Sharp.City.rumour = true; //���� ���� �����
					dialog.text = DLG_TEXT_BR[12] + XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc") + ".";
					link.l1 = DLG_TEXT_BR[13] + npchar.name + DLG_TEXT_BR[14];
					link.l1.go = "exit";
					pchar.questTemp.Sharp = "toSharp_going"; //� ������� ������ �� ��������
					ReOpenQuestHeader("SharpPearl");
					AddQuestRecord("SharpPearl", "1");
					AddQuestUserData("SharpPearl", "sCity", XI_ConvertString("Colony" + npchar.city + "Dat"));
					AddQuestUserData("SharpPearl", "sTarget", XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc"));
					if (GetIslandByCityName(pchar.questTemp.Sharp.City) != pchar.questTemp.Sharp.City)
					{
						AddQuestUserData("SharpPearl", "sAreal", DLG_TEXT_BR[15] + XI_ConvertString(GetIslandByCityName(pchar.questTemp.Sharp.City) + "Dat"));
					}
					//��������� ��������� ����� �� �����
					group_DeleteGroup("Sharp_Group");
					sld = characterFromId("Sharp");
					string sGroup = "Sharp_Group";
					Group_FindOrCreateGroup(sGroup);
					Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
					Group_LockTask(sGroup);
					Group_AddCharacter(sGroup, sld.id);
					Group_SetGroupCommander(sGroup, sld.id);
					sld.mapEnc.type = "trade";
					sld.mapEnc.worldMapShip = "quest_ship";
					sld.mapEnc.Name = DLG_TEXT_BR[16];
					string sColony= SelectAnyColony(npchar.city); //�������, ������ ������ ����
					int daysQty = GetMaxDaysFromIsland2Island(GetArealByCityName(sColony), GetArealByCityName(pchar.questTemp.Sharp.City))+3; //���� ������� ���� � �������
					Map_CreateTrader(sColony, pchar.questTemp.Sharp.City, sld.id, daysQty);
					Log_TestInfo(DLG_TEXT_BR[17] + sColony + DLG_TEXT_BR[18] + pchar.questTemp.Sharp.City);
				}
				else
				{	//����, ���� �����, � �������!
					dialog.text = DLG_TEXT_BR[19];
					link.l1 = DLG_TEXT_BR[20] + npchar.name + "...";
					link.l1.go = "SharpPearl_2";
				}
			}
			else
			{
				dialog.text = DLG_TEXT_BR[21];
				link.l1 = DLG_TEXT_BR[22] + npchar.name + ".";
				link.l1.go = "exit";
			}
		break;
		case "SharpPearl_2":
			dialog.text = DLG_TEXT_BR[23];
			link.l1 = DLG_TEXT_BR[24];
			link.l1.go = "SharpPearl_3";
		break;
		case "SharpPearl_3":
			chrDisableReloadToLocation = true;
			pchar.questTemp.Sharp.count = 0; //������� ����� ��� ����. ����
			pchar.questTemp.Sharp.price = 5000+rand(20)*1000; //���� �� �������
			pchar.questTemp.Sharp.price.evil = rand(1); //������ ��� ����, ��� ������
			sld = &characters[GetCharacterIndex("Sharp")];
			sld.dialog.currentnode = "BrothelSharp";
			LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
			ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
			LAi_SetActorTypeNoGroup(sld);
			LAi_ActorDialog(sld, pchar, "", -1, 0);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

