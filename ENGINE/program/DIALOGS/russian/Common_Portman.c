#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag, arCapBase, arCapLocal;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	// ��� ������
	if (!CheckAttribute(NPChar, "Portman")) NPChar.Portman = 0;
	if (!CheckAttribute(NPChar, "PortManPrice")) NPChar.PortManPrice = (5000 + rand(4)*1000);

    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\PortMan\" + NPChar.City + "_PortMan.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 25/06/06

    int i, cn;
    ref chref;
    string attrL, sTitle, sCapitainId;
    
    bool ok;
    int iTest = FindColony(NPChar.City); // ����� ��������
    ref rColony;
    string sFrom_sea = "";
	npchar.quest.qty = CheckCapitainsList(npchar); //��� ������ �����

	if (iTest != -1)
	{
		rColony = GetColonyByIndex(iTest);
		sFrom_sea = rColony.from_sea;
	}	
    
	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "fight":
			DialogExit();
            NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
		break;
		
		case "First time":
			NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("� ������ ������� �������, ���� ����� ����! �� ����� ����� � �� �� ���� ����� �������������.", "��� ��������� ������ ����� �� ������ � ������� ����. � �� ����� � ������������� � ����� �� ����!", "���� ��������, ���� ������� �� ������� �� ���� ������..."), 
					LinkRandPhrase("��� ���� �����, �������?! ��������� ������ ��� ����� ���� ����, ������ ���� �� ����, ������� �����!", "������� ������, ��� �� ����� ����! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("������ ���� �����, " + GetWorkTypeOfMan(npchar, "") + ", � �� �� ����� ���� ������� ����!", "���, " + GetWorkTypeOfMan(npchar, "") + ", � ��� ���� �� - ������� ������! ��� ��� � ���� �����, ��������: ���� ���� � ������ ����..."));
				link.l1.go = "fight";
				break;
			}
			
			if(NPChar.quest.meeting == "0")
			{
				NPChar.quest.meeting = "1";
				dialog.text = "����������� ���, " + GetAddress_Form(NPChar) + ". ��, �������, �� �������. � "  + GetFullName(npchar)+ " - ��������� �����.";
				Link.l1 = "������������, " + GetFullName(NPChar) + ". � " + GetFullName(PChar) + ", ������� ������� '" + PChar.ship.name + "'.";
			}
			else
			{
				dialog.text = LinkRandPhrase("����������� ���, " + GetAddress_Form(NPChar) + ". �� �� ��� �� ����?",
                                    "������������, " + GetFullName(Pchar) + ". � �����, ��� ��� ������� ����� � ����, � ��� ������, ��� �� �� ��� �������.",
                                    "�, ������� " + GetFullName(Pchar) + ". ��� ������� ��� �� ���?");
				Link.l1 = "������������, " + GetFullName(NPChar) + ". � ���� � ���� ����������.";
			}
			Link.l1.go = "node_2";
		break;
		
		case "node_2":
			dialog.text = "���������. � � ����� �������, " + GetFullName(PChar) + ".";
			Link.l2 = "����� ����� ��������� ������? ��� ��������?";
			Link.l2.go = "node_4";
			Link.l3 = "���� � �������� ���� �� ����� �������� �� �����?";
			Link.l3.go = "ShipStock_1";
			if (sti(NPChar.Portman) > 0)
			{
                Link.l4 = "� ���� ������� ���� ������� �������.";
    			Link.l4.go = "ShipStockReturn_1";
			}
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
			{
				link.l5 = "� � ��� �� ���������� �����.";
				link.l5.go = "LoanForAll";//(���������� � ��������� ���������)	
			}
  			if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") //����� ���� - �� ����� � ����� �������
			{
				link.l7 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
					GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
				link.l7.go = "IntelligenceForAll";
			}
			link.l6 = "���� ���������� ��������, ������� ���������� � ����� �������� ����������.";
			link.l6.go = "CapitainList";
			link.l8 = "� �� ������� �������.";
			link.l8.go = "quests";
			Link.l9 = "���������. �� ��������.";
			Link.l9.go = "exit";
		break;

		case "node_4":
			//--> �������� ����������� ���������� �����. 
			if (npchar.quest == "PortmansJornal") //���� ����� �� ������� ������
			{
				dialog.text = "�� ������ ���� ����� �������� �� ����� " + npchar.quest.PortmansJornal.capName + " � ������� ��� ������� ������. �� ������� ���?";
				link.l1 = "���, �� ���������� ����...";
				link.l1.go = "PortmanQuest_NF";
				break;
			}
			if (npchar.quest == "PortmansSeekShip" || npchar.quest == "SeekShip_sink") //���� ����� �� ������ ����������� �������
			{
				dialog.text = "�� ����������� ��������� ���������� " + GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName)) + " '" + npchar.quest.PortmansSeekShip.shipName + "'. ���� �� �� ��������� ��� �������, �� � ����� ������ ���������� �� ����� ���� � ����.";
				link.l1 = "� �������� ������, �����.";
				link.l1.go = "exit";
				link.l2 = "���� ���������� �� ���������� ������ ���������.";
				link.l2.go = "SeekShip_break";
				break;
			}
			if (npchar.quest == "SeekShip_success") //���������� ������� ���� �� �������
			{
				dialog.text = "�� ������� ��� ��������� ���������� " + GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName)) + " '" + npchar.quest.PortmansSeekShip.shipName + "'. �� ������� ���?";
				bool bOk = false;
				for (i=0; i<=COMPANION_MAX; i++)
				{
					cn = GetCompanionIndex(pchar, i);
					if(cn != -1)
					{
						sld = &characters[cn];
						if (sld.ship.name == npchar.quest.PortmansSeekShip.shipName && 
							RealShips[sti(sld.ship.type)].BaseName == npchar.quest.PortmansSeekShip.shipTapeName &&
							RealShips[sti(sld.Ship.Type)].basetype == npchar.quest.PortmansSeekShip.shipTape)
						{
							if (i == 0)
							{	//���� ������ ������� - � ��
								sld.Ship.Type = GenerateShip(SHIP_TARTANE, true);
								SetBaseShipData(sld);
								SetCrewQuantityFull(sld);
							}
							else
							{
								RemoveCharacterCompanion(pchar, sld);
								AddPassenger(pchar, sld, false);
							}
							bOk = true;
						}
					}
				}				
				if (bOk)
				{
					link.l1 = "��, � ����� ���, ����� � ����� �� �����. ������ ��������.";
					link.l1.go = "SeekShip_good";
				}
				else
				{
					link.l1 = "� �������� ������, �����.";
					link.l1.go = "exit";
				}
				link.l2 = "���� ���������� �� ���������� ������ ���������.";
				link.l2.go = "SeekShip_break";
				break;
			}
			//<-- �������� ����������� ���������� �����. 

			//--> ���� ����������� ���������� �����. 
			if (rand(2) < 2 && pchar.questTemp.different == "free" && GetNpcQuestPastDayWOInit(npchar, "quest.meeting") > 7)
			{
				dialog.text = LinkRandPhrase("��, ���� ������! � ���� ���� ��� ��� ������!", 
					"��! �� ��� ������ �������! � ���� ���� ���� ��������, ��������� ����������.", 
					"��� ��! �������, � ���� ���� ������! ��������, ������ ��, ������ ��������...");
				link.l1 = "��������� ���� ����, " + GetAddress_Form(NPChar) + ".";
				link.l1.go = "PortmanQuest";
				SaveCurrentNpcQuestDateParam(npchar, "quest.meeting");
				break;
			}			
			//<-- ���� ����������� ���������� �����.

			dialog.text = "���������� ������ �� ������������ ������ - � ���� ������ ���� ������. �����, � ������� ����� ���� ���������������� �����, �� � ������� ������ �������� ���������.";
			Link.l1 = "��� ���� ������.";
			Link.l1.go = "node_2";
			Link.l2 = "���������. �� ��������.";
			Link.l2.go = "exit";
		break;
		case "PortmanQuest_NF":
			dialog.text = "��, ��� �����. � ����� ������, �� � ����� ����� ������ � ���� �� ����� ���� � ����.";
			link.l1 = "�������. ��� ��� ��� ����� �������...";
			link.l1.go = "node_2";
		break;

		//--> ���������� ��������
		case "PortmanQuest":
			if (rand(1))
			{	//����� ������� � �������� ������� ������
				dialog.text = "� ���� ����� ������� ������ ���� �������. ��� �� ���� ���� ����� �������!.. � �����, ����� ��� ������� � ������� �������.";
				link.l1 = "��, ��� �����... �� ��� ������ � ������!";
				link.l1.go = "PortmanQuest_1";
				link.l2 = "���, �� ���� ���� ����������. ��� ��� ��������, ����������...";
				link.l2.go = "node_2";
			}
			else
			{	//����� ��������� ���������� �������
				dialog.text = "� ���� �� ������� �������� ��� ������� �������. � ����, ����� �� ��������� � ������� ��� ���.";
				link.l1 = "��, �� ��� ��, � ����� ������� �� �������.";
				link.l1.go = "SeekShip_1";
				link.l2 = "��������, �� �������� �������� � �� ���������.";
				link.l2.go = "node_2";
			}
		break;
		// -------------------------------- ����� �������� �������� ������� �� ���������� ���� --------------------------
		case "PortmanQuest_1":
			dialog.text = "�������, ��� ��� ������... ��-�, ����� ���� � ���� �����! ������ �������� ������� - ����� ����������, ��������� � �� ����� ���������.";
			link.l1 = "��� �����, ���������� �����! ��, � ������ ���������� ��� ��������� �� ���� ���������� ��������.";
			link.l1.go = "PortmanQuest_2";
			pchar.questTemp.different = "PortmansJornal";
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ����������
			SetJornalCapParam(npchar);
			GiveItem2Character(pchar, "PortmansBook");
		break;
		case "PortmanQuest_2":
			dialog.text = "��, �������! ��� ����� " + npchar.quest.PortmansJornal.capName + ", �� ������� " + GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansJornal.shipTapeName + "Acc")) + " � ������ '" + npchar.quest.PortmansJornal.shipName + "'. ����� �� � ���� �������, � ���������� � " + XI_ConvertString("Colony" + npchar.quest.PortmansJornal.city + "Acc") + ".";
			link.l1 = "�������. �� ��� ��, � ��� ����������� �����. ��� ���� � �������?";
			link.l1.go = "PortmanQuest_3";
		break;
		case "PortmanQuest_3":
			dialog.text = npchar.quest.PortmansJornal.capName + " ��� � ���� ������������, ��� � ��� ���������. ������ ������������ ����� ��� �������, � ��� ����� ���������.";
			link.l1 = "����. ��, � �����...";
			link.l1.go = "exit";
			sTitle = npchar.id + "PortmansBook_Delivery";
			AddQuestRecordEx(sTitle, "PortmansBook_Delivery", "1");
			AddQuestUserDataForTitle(sTitle, "sCapName", npchar.quest.PortmansJornal.capName);
			AddQuestUserDataForTitle(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansJornal.shipTapeName + "Acc")));
			AddQuestUserData(sTitle, "sShipName", npchar.quest.PortmansJornal.shipName);
			AddQuestUserData(sTitle, "sCapName", npchar.quest.PortmansJornal.capName);
			AddQuestUserData(sTitle, "sCapName2", npchar.quest.PortmansJornal.capName);
			AddQuestUserData(sTitle, "sTargetCity", XI_ConvertString("Colony" + npchar.quest.PortmansJornal.city + "Acc"));
			if (GetIslandByCityName(npchar.quest.PortmansJornal.city) != npchar.quest.PortmansJornal.city)
			{
				AddQuestUserData(sTitle, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(npchar.quest.PortmansJornal.city) + "Dat"));
			}			
		break;
		// -------------------------------- ����� ������� ����������� ������� ----------------------------------
		case "SeekShip_1":
			dialog.text = "���������! �� ������, ���������� ������� ����������� �� ���������� ��������, ��� ��� �� ����� ����� ��� ����. � ������� ������� �� ������...";
			link.l1 = "�������. ���������� ���������, ��� �� �������, ��� ����� ��������������� ��� �������.";
			link.l1.go = "SeekShip_2";
			pchar.questTemp.different = "PortmansSeekShip";
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ����������
			SetSeekShipCapParam(npchar);
		break;
		case "SeekShip_2":
			dialog.text = XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName) + " � ������ '" + npchar.quest.PortmansSeekShip.shipName + "'. ������� ��� ����� " + FindRussianDaysString(rand(5)+10) + " �����. ��������� ����.";
			link.l1 = "��, ��� ��� ������ ����, ������, ��������. � �����, � ������� ������ �������� �� ����������...";
			link.l1.go = "SeekShip_3";
		break;
		case "SeekShip_3":
			dialog.text = "��, �����. �� � ������ ��� ����� ������ ��������� �� ����. ������� ���� � �������, �� �������� ������� � �����, � ��� �� ������ ��, ��� ��� �����.";
			link.l1 = "�������. �� ��� ��, ��������� � �������. �������, ��� �������.";
			link.l1.go = "exit";
			sTitle = npchar.id + "Portmans_SeekShip";
			AddQuestRecordEx(sTitle, "Portmans_SeekShip", "1");
			AddQuestUserDataForTitle(sTitle, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName)));
			AddQuestUserDataForTitle(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName)));
			AddQuestUserData(sTitle, "sShipTypeName2", GetStrSmallRegister(XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName)));
			AddQuestUserData(sTitle, "sShipName", npchar.quest.PortmansSeekShip.shipName);
		break;

		case "SeekShip_break":
			dialog.text = "��, �� ��� ��, ����� ����. ����, �� ����� ������ ���� ������������...";
			link.l1 = "��, ������� ����� ������� ������ � ������� �����.";
			link.l1.go = "SeekShip_break_1";
		break;
		case "SeekShip_break_1":
			dialog.text = "�� ��� ��, ������� �� ��������� ������, ���� ��� � �� ���������� �������.";
			link.l1 = "�� �� ���...";
			link.l1.go = "exit";
			sTemp = "SeekShip_checkAbordage" + npchar.index;
			pchar.quest.(sTemp).over = "yes"; //������� ���������� �� �������
			cn = GetCharacterIndex("SeekCap_" + npchar.index);
			//���� ���-��� ��� ��� - ������� ���
			if (cn > 0)
			{
				characters[cn].LifeDay = 0; 
				Map_ReleaseQuestEncounter(characters[cn].id);
				group_DeleteGroup("SeekCapShip_" + characters[cn].index);
			}
			sTitle = npchar.id + "Portmans_SeekShip";
			AddQuestRecordEx(sTitle, "Portmans_SeekShip", "7");
			CloseQuestHeader(sTitle);
			DeleteAttribute(npchar, "quest.PortmansSeekShip");
			npchar.quest = ""; //����������� ������ ���� ������ ��� ��������
			ChangeCharacterReputation(pchar, -7);
		break;	

		case "SeekShip_good":
			if (npchar.quest == "SeekShip_sink")
			{
				dialog.text = "�������! ������, �������, ��� ��� �� ������ ��� �������, ��� ��� �������... ����, ����������, ��� �����! � ���� ��� � ���.";
				link.l1 = "��, �������������...";
				npchar.quest.money = makeint(sti(npchar.quest.money) / 4); //������� ������
				ChangeCharacterReputation(pchar, 5);
				ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 10);
                AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 10);
                AddCharacterExpToSkill(GetMainCharacter(), "Sailing", 10);
                AddCharacterExpToSkill(GetMainCharacter(), "Commerce", 50);
                AddCharacterExpToSkill(GetMainCharacter(), "Cannons", 20);
			}
			else
			{
				dialog.text = "�����������! �� ��� ����� �������. �����������, ��������� ��� ���� ������.";
				link.l1 = "��, �������������...";
				ChangeCharacterReputation(pchar, 10);
				ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 20);
                AddCharacterExpToSkill(GetMainCharacter(), "Leadership", 100);
                AddCharacterExpToSkill(GetMainCharacter(), "Sailing", 150);
                AddCharacterExpToSkill(GetMainCharacter(), "Grappling", 100);
			}
			link.l1.go = "SeekShip_good_1";
		break;
		case "SeekShip_good_1":
			dialog.text = "� ����� ��������� ��� ��������������. ��� ���������� " + FindRussianMoneyString(sti(npchar.quest.money)) + ". ������ � ��������� �� ����, � ���������.";
			link.l1 = "�� ��� ��, ����� ���� ����������. ������� � ����� ��������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, sti(npchar.quest.money));
			sTitle = npchar.id + "Portmans_SeekShip";
			AddQuestRecordEx(sTitle, "Portmans_SeekShip", "6");
			CloseQuestHeader(sTitle);
			DeleteAttribute(npchar, "quest.PortmansSeekShip");
			npchar.quest = ""; //����������� ������ ���� ������ ��� ��������
		break;

		//------------------------------> ���� �� ���� �������� �����
		//��������. � �������� ������ ��������� ������� ������ �� �������   PortmansBook_Delivery  #TEXT   5
		//� ������ �������� ������� �����, ��������� � ����� ������ �� quest_text.txt
		//������: � ����� ������  void SetCapitainFromCityToSea(string qName)
		case "CapitainList":
			if (sti(npchar.quest.qty) > 0)
			{
				dialog.text = "���� ������������ ��������. ��� ��������� ��� ����������?";
				makearef(arCapBase, npchar.quest.capitainsList); 
				for (i=0; i<sti(npchar.quest.qty); i++)
				{
    				arCapLocal = GetAttributeN(arCapBase, i);
					sCapitainId = GetAttributeName(arCapLocal);
					sld = characterFromId(sCapitainId);
					attrL = "l" + i;
					link.(attrL) = GetFullName(sld) + ", ������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "'.";
					link.(attrL).go = "CapList_"+attrL;
				}
			}
			else
			{
				dialog.text = "� ���� � ������� ��� ���������, ��������� ��� ��������������.";
				link.l1 = "�������. ������� �� ����������.";
				link.l1.go = "node_2";
			}
		break;
		case "CapList_l0":
			makearef(arCapBase, npchar.quest.capitainsList);
    		arCapLocal = GetAttributeN(arCapBase,  0);
			sCapitainId = GetAttributeName(arCapLocal);
			sld = characterFromId(sCapitainId);
			dialog.text = LinkRandPhrase("��-�-��, ������� ���������... ���, ����! ", "���-���... ���, �����! ", "������, ���. ") +
				"������� " + GetFullName(sld) + " " + arCapLocal.date + " ���� ���� �� ������ ����� � " + XI_ConvertString("Colony" + arCapLocal + "Acc") + ".";
			link.l1 = "�������. �������� �� ���������� ������ ������...";
			link.l1.go = "CapitainList";
			link.l2 = "���, �������� ���� ����� �� ����������.";
			link.l2.go = "node_2";
			//������� ������ � ��
			AddQuestRecordEx(arCapLocal.QBString1, arCapLocal.QBString2, arCapLocal.QBQty);
			AddQuestUserData(arCapLocal.QBString1, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(arCapLocal.QBString1, "sCapName", GetFullName(sld));
			AddQuestUserData(arCapLocal.QBString1, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Dat")));
			AddQuestUserData(arCapLocal.QBString1, "sShipName", sld.Ship.name);
			AddQuestUserData(arCapLocal.QBString1, "sDate", arCapLocal.date);
			AddQuestUserData(arCapLocal.QBString1, "sTargetCity", XI_ConvertString("Colony" + arCapLocal + "Acc"));
			if (GetIslandByCityName(arCapBase.(sCapitainId)) != arCapBase.(sCapitainId))
			{
				AddQuestUserData(arCapLocal.QBString1, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(arCapBase.(sCapitainId)) + "Dat"));
			}
			//������� �� ������
			DeleteAttribute(arCapBase, sCapitainId); 
		break;
		case "CapList_l1":
			makearef(arCapBase, npchar.quest.capitainsList);
    		arCapLocal = GetAttributeN(arCapBase,  1);
			sCapitainId = GetAttributeName(arCapLocal);
			sld = characterFromId(sCapitainId);
			dialog.text = LinkRandPhrase("��-�-��, ������� ���������... ���, ����! ", "���-���... ���, �����! ", "������, ���. ") +
				"������� " + GetFullName(sld) + " " + arCapLocal.date + " ���� ���� �� ������ ����� � " + XI_ConvertString("Colony" + arCapLocal + "Acc") + ".";
			link.l1 = "�������. �������� �� ���������� ������ ������...";
			link.l1.go = "CapitainList";
			link.l2 = "���, �������� ���� ����� �� ����������.";
			link.l2.go = "node_2";
			//������� ������ � ��
			AddQuestRecordEx(arCapLocal.QBString1, arCapLocal.QBString2, arCapLocal.QBQty);
			AddQuestUserData(arCapLocal.QBString1, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(arCapLocal.QBString1, "sCapName", GetFullName(sld));
			AddQuestUserData(arCapLocal.QBString1, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Dat")));
			AddQuestUserData(arCapLocal.QBString1, "sShipName", sld.Ship.name);
			AddQuestUserData(arCapLocal.QBString1, "sDate", arCapLocal.date);
			AddQuestUserData(arCapLocal.QBString1, "sTargetCity", XI_ConvertString("Colony" + arCapLocal + "Acc"));
			if (GetIslandByCityName(arCapBase.(sCapitainId)) != arCapBase.(sCapitainId))
			{
				AddQuestUserData(arCapLocal.QBString1, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(arCapBase.(sCapitainId)) + "Dat"));
			}
			//������� �� ������
			DeleteAttribute(arCapBase, sCapitainId); 
		break;
		case "CapList_l2":
			makearef(arCapBase, npchar.quest.capitainsList);
    		arCapLocal = GetAttributeN(arCapBase,  2);
			sCapitainId = GetAttributeName(arCapLocal);
			sld = characterFromId(sCapitainId);
			dialog.text = LinkRandPhrase("��-�-��, ������� ���������... ���, ����! ", "���-���... ���, �����! ", "������, ���. ") +
				"������� " + GetFullName(sld) + " " + arCapLocal.date + " ���� ���� �� ������ ����� � " + XI_ConvertString("Colony" + arCapLocal + "Acc") + ".";
			link.l1 = "�������. �������� �� ���������� ������ ������...";
			link.l1.go = "CapitainList";
			link.l2 = "���, �������� ���� ����� �� ����������.";
			link.l2.go = "node_2";
			//������� ������ � ��
			AddQuestRecordEx(arCapLocal.QBString1, arCapLocal.QBString2, arCapLocal.QBQty);
			AddQuestUserData(arCapLocal.QBString1, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(arCapLocal.QBString1, "sCapName", GetFullName(sld));
			AddQuestUserData(arCapLocal.QBString1, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Dat")));
			AddQuestUserData(arCapLocal.QBString1, "sShipName", sld.Ship.name);
			AddQuestUserData(arCapLocal.QBString1, "sDate", arCapLocal.date);
			AddQuestUserData(arCapLocal.QBString1, "sTargetCity", XI_ConvertString("Colony" + arCapLocal + "Acc"));
			if (GetIslandByCityName(arCapBase.(sCapitainId)) != arCapBase.(sCapitainId))
			{
				AddQuestUserData(arCapLocal.QBString1, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(arCapBase.(sCapitainId)) + "Dat"));
			}
			//������� �� ������
			DeleteAttribute(arCapBase, sCapitainId); 
		break;
		case "CapList_l3":
			makearef(arCapBase, npchar.quest.capitainsList);
    		arCapLocal = GetAttributeN(arCapBase,  3);
			sCapitainId = GetAttributeName(arCapLocal);
			sld = characterFromId(sCapitainId);
			dialog.text = LinkRandPhrase("��-�-��, ������� ���������... ���, ����! ", "���-���... ���, �����! ", "������, ���. ") +
				"������� " + GetFullName(sld) + " " + arCapLocal.date + " ���� ���� �� ������ ����� � " + XI_ConvertString("Colony" + arCapLocal + "Acc") + ".";
			link.l1 = "�������. �������� �� ���������� ������ ������...";
			link.l1.go = "CapitainList";
			link.l2 = "���, �������� ���� ����� �� ����������.";
			link.l2.go = "node_2";
			//������� ������ � ��
			AddQuestRecordEx(arCapLocal.QBString1, arCapLocal.QBString2, arCapLocal.QBQty);
			AddQuestUserData(arCapLocal.QBString1, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(arCapLocal.QBString1, "sCapName", GetFullName(sld));
			AddQuestUserData(arCapLocal.QBString1, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Dat")));
			AddQuestUserData(arCapLocal.QBString1, "sShipName", sld.Ship.name);
			AddQuestUserData(arCapLocal.QBString1, "sDate", arCapLocal.date);
			AddQuestUserData(arCapLocal.QBString1, "sTargetCity", XI_ConvertString("Colony" + arCapLocal + "Acc"));
			if (GetIslandByCityName(arCapBase.(sCapitainId)) != arCapBase.(sCapitainId))
			{
				AddQuestUserData(arCapLocal.QBString1, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(arCapBase.(sCapitainId)) + "Dat"));
			}
			//������� �� ������
			DeleteAttribute(arCapBase, sCapitainId); 
		break;
		case "CapList_l4":
			makearef(arCapBase, npchar.quest.capitainsList);
    		arCapLocal = GetAttributeN(arCapBase,  4);
			sCapitainId = GetAttributeName(arCapLocal);
			sld = characterFromId(sCapitainId);
			dialog.text = LinkRandPhrase("��-�-��, ������� ���������... ���, ����! ", "���-���... ���, �����! ", "������, ���. ") +
				"������� " + GetFullName(sld) + " " + arCapLocal.date + " ���� ���� �� ������ ����� � " + XI_ConvertString("Colony" + arCapLocal + "Acc") + ".";
			link.l1 = "�������. �������� �� ���������� ������ ������...";
			link.l1.go = "CapitainList";
			link.l2 = "���, �������� ���� ����� �� ����������.";
			link.l2.go = "node_2";
			//������� ������ � ��
			AddQuestRecordEx(arCapLocal.QBString1, arCapLocal.QBString2, arCapLocal.QBQty);
			AddQuestUserData(arCapLocal.QBString1, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(arCapLocal.QBString1, "sCapName", GetFullName(sld));
			AddQuestUserData(arCapLocal.QBString1, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Dat")));
			AddQuestUserData(arCapLocal.QBString1, "sShipName", sld.Ship.name);
			AddQuestUserData(arCapLocal.QBString1, "sDate", arCapLocal.date);
			AddQuestUserData(arCapLocal.QBString1, "sTargetCity", XI_ConvertString("Colony" + arCapLocal + "Acc"));
			if (GetIslandByCityName(arCapBase.(sCapitainId)) != arCapBase.(sCapitainId))
			{
				AddQuestUserData(arCapLocal.QBString1, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(arCapBase.(sCapitainId)) + "Dat"));
			}
			//������� �� ������
			DeleteAttribute(arCapBase, sCapitainId); 
		break;
		//<--------------------------- ���� �� ���� �������� �����

		case "ShipStock_1":
            if (sti(NPChar.Portman) >= 3)
			{
                dialog.text = "������-��, ��. �� ������ � �� ���� ������� �������. ��� ���� � �����.";
    			Link.l1 = "����� ����.";
    			Link.l1.go = "exit";
			}
            else
            {
    			ok = (sFrom_sea == "") || (Pchar.location.from_sea == sFrom_sea);
			    if (sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok)
				{
					dialog.text = "����� ������ ������� �� ������ ��������?";
	    			for(i=1; i<COMPANION_MAX; i++)
	                {
	        	        cn = GetCompanionIndex(PChar, i);
	        	        if( cn >0 )
	        	        {
	        		        chref = GetCharacter(cn);
	                        if (!GetRemovable(chref)) continue;
	
	        		        attrL = "l"+i;
	        		        Link.(attrL)    = XI_ConvertString(RealShips[sti(chref.Ship.Type)].BaseName) + " " + chref.Ship.Name;
	        		        Link.(attrL).go = "ShipStockMan_" + i;
	        		    }
	        	    }
	    			Link.l9 = "�������, �� �����.";
	    			Link.l9.go = "exit";
    			}
    			else
    			{
					dialog.text = "��. � �� ���� ����� ������� � �����.";
	    			Link.l1 = "��, � ������ ����� ������ � ����������� �������.";
	    			Link.l1.go = "exit";
				}
			}
		break;

		case "ShipStockMan_1":
            NPChar.ShipToStoreIdx = GetCompanionIndex(PChar, 1);
			dialog.text = "���������, ��� ��� �� �������.";
			Link.l1 = "������.";
			Link.l1.go = "ShipStock_2";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStockMan_2":
            NPChar.ShipToStoreIdx = GetCompanionIndex(PChar, 2);
			dialog.text = "��������� ��� ��� �� �������.";
			Link.l1 = "������.";
			Link.l1.go = "ShipStock_2";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStockMan_3":
            NPChar.ShipToStoreIdx = GetCompanionIndex(PChar, 3);
			dialog.text = "��������� ��� ��� �� �������.";
			Link.l1 = "������.";
			Link.l1.go = "ShipStock_2";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;
		
		case "ShipStockMan_4":
            NPChar.ShipToStoreIdx = GetCompanionIndex(PChar, 4);
			dialog.text = "��������� ��� ��� �� �������.";
			Link.l1 = "������.";
			Link.l1.go = "ShipStock_2";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStock_2":
            chref = GetCharacter(sti(NPChar.ShipToStoreIdx));
            NPChar.MoneyForShip =  (8-sti(RealShips[sti(chref.Ship.Type)].Class)) * sti(NPChar.PortManPrice);
			dialog.text = XI_ConvertString(RealShips[sti(chref.Ship.Type)].BaseName) + " " + chref.Ship.Name + ", ����� " + RealShips[sti(chref.Ship.Type)].Class +
                     ", ��������� ������� " + NPChar.MoneyForShip;
			Link.l1 = "��. ��� ���� ����������.";
			if (sti(Pchar.Money) >= sti(NPChar.MoneyForShip))
			{
			    Link.l1.go = "ShipStock_3";
			}
			else
			{
                Link.l1.go = "ShipStock_NoMoney";
			}
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStock_NoMoney":
			dialog.text = "���� ���� ��� �������, ���� � ��� ����� ������ �����.";
			Link.l1 = "���.. ����� �����.";
			Link.l1.go = "exit";
		break;

		case "ShipStock_3":
            AddMoneyToCharacter(pchar, -makeint(NPChar.MoneyForShip));
            chref = GetCharacter(sti(NPChar.ShipToStoreIdx));
            chref.ShipInStockMan = NPChar.id;
            chref.Ship.Crew.Quantity  = 0;
            RemoveCharacterCompanion(pchar, chref);
            chref.location = "";
            chref.location.group = "";
            chref.location.locator = "";

            NPChar.Portman    = sti(NPChar.Portman) + 1;
            pchar.ShipInStock = sti(pchar.ShipInStock) + 1;

			dialog.text = "������. ��������, ����� ����� �����.";
			Link.l1 = "�������.";
			Link.l1.go = "exit";
		break;

		case "ShipStockReturn_1":
            ok = (sFrom_sea == "") || (Pchar.location.from_sea == sFrom_sea);
		    if (sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok)
			{
				if (GetCompanionQuantity(pchar) < COMPANION_MAX)
	            {
	                dialog.text = "����� ������ ������� �� ��������?";
	                cn = 1;
	                for(i=1; i<MAX_CHARACTERS; i++)
	            	{
	            		makeref(chref, Characters[i]);
	            		if (CheckAttribute(chref, "ShipInStockMan"))
	            		{
	                        if (chref.ShipInStockMan == NPChar.id)
	        		        {
	                            attrL = "l"+cn;
	            		        Link.(attrL)    = XI_ConvertString(RealShips[sti(chref.Ship.Type)].BaseName) + " " + chref.Ship.Name;
	            		        Link.(attrL).go = "ShipStockManBack_" + cn;
	            		        attrL = "ShipToStoreId" + cn;
	            		        NPChar.Temp.(attrL) = i;
	            		        cn++;
	        		        }
	            		}
	                }
	
	    			Link.l9 = "���, � ���������.";
	    			Link.l9.go = "exit";
				}
				else
				{
	                dialog.text = "��� � ��� ����� ��� ��� ������ �������.";
	    			Link.l1 = "��, �����. �������.";
	    			Link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "��. � �� ���� ������ �������� � �����. � ������� ���� ������� �� ������ ������ �����.";
    			Link.l1 = "������, � ������� �� ���� �������.";
    			Link.l1.go = "exit";
			}
		break;

        case "ShipStockManBack_1":
            NPChar.ShipToStoreIdx = sti(NPChar.Temp.ShipToStoreId1);
			dialog.text = "���������?";
			Link.l1 = "��.";
			Link.l1.go = "ShipStockManBack";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStockManBack_2":
            NPChar.ShipToStoreIdx = sti(NPChar.Temp.ShipToStoreId2);
            dialog.text = "���������?";
			Link.l1 = "��.";
			Link.l1.go = "ShipStockManBack";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStockManBack_3":
            NPChar.ShipToStoreIdx = sti(NPChar.Temp.ShipToStoreId3);
            dialog.text = "���������?";
			Link.l1 = "��.";
			Link.l1.go = "ShipStockManBack";
			Link.l2 = "���, � ���������.";
			Link.l2.go = "exit";
		break;

		case "ShipStockManBack":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();

			chref = GetCharacter(sti(NPChar.ShipToStoreIdx));
			DeleteAttribute(chref, "ShipInStockMan");
			SetCompanionIndex(pchar, -1, sti(NPChar.ShipToStoreIdx));
			NPChar.Portman    = sti(NPChar.Portman) - 1;
            pchar.ShipInStock = sti(pchar.ShipInStock) - 1;
		break;
	}
}

void SetJornalCapParam(ref npchar)
{
	//������� ����������� ����
	ref sld = GetCharacter(NPC_GenerateCharacter("PortmansCap_" + npchar.index, "", "man", "man", 20, sti(npchar.nation), -1, true));
	SetCaptanModelByEncType(sld, "trade");
	SetShipToFantom(sld, "trade", true);
	sld.Ship.Mode = "trade";
	sld.dialog.filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "PortmansCap";
	sld.DeckDialogNode = "PortmansCap_inDeck";
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "SaveItemsForDead");
	DeleteAttribute(sld, "DontClearDead");
	DeleteAttribute(sld, "AboardToFinalDeck");
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "DontRansackCaptain");
	sld.AlwaysSandbankManeuver = true;
	sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
	sld.DontRansackCaptain = true; //�� ���������
	SetCharacterPerk(sld, "ShipTurnRateUp");
	SetCharacterPerk(sld, "ShipTurnRateUp");
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	//� ������� ������ ����
	string sGroup = "PorpmansShip_" + sld.index;
	Group_FindOrCreateGroup(sGroup);
	Group_SetType(sGroup,"trade");
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 
	//���������� ������ � ��������� �������� � ����
	npchar.quest = "PortmansJornal"; //������ ���� ������ ��� ��������
	npchar.quest.PortmansJornal.capName = GetFullName(sld); //��� ����
	npchar.quest.PortmansJornal.shipName = sld.Ship.name; //��� �������
	npchar.quest.PortmansJornal.shipTapeName = RealShips[sti(sld.Ship.Type)].BaseName; //�������� �������
	npchar.quest.PortmansJornal.city = SelectNotEnemyColony(npchar); //��������� �������, ���� ���� ���
	sld.quest = "InMap"; //������ ���� ����������� ����
	sld.quest.targetCity = npchar.quest.PortmansJornal.city; //������������ �������-���� � ��������� ����
	sld.quest.firstCity = npchar.city; //�������� ����� ������ ����� � ����� ������
	sld.quest.stepsQty = 1; //���������� ������� � ����
	sld.quest.money = ((sti(RealShips[sti(sld.Ship.Type)].basetype)+1) * 1000) + (sti(pchar.rank)*300); //��������������
	//��������� �����, ���� ������� ���������. ����� ����� �� ��������� ����� �� � ������
	string sTemp = GetArealByCityName(npchar.city);
	//�� �����
	sld.mapEnc.type = "trade";
	sld.mapEnc.worldMapShip = "ranger";
	sld.mapEnc.Name = XI_ConvertString(npchar.quest.PortmansJornal.shipTapeName) + " '" + npchar.quest.PortmansJornal.shipName + "'";
	int daysQty = GetMaxDaysFromIsland2Island(sTemp, GetArealByCityName(sld.quest.targetCity))+5; //���� ������� ���� � �������
	Map_CreateTrader(GetIslandRandomShoreId(sTemp), npchar.quest.PortmansJornal.city, sld.id, daysQty);
}
//��������� ������ ������������ ��������� �����
int CheckCapitainsList(ref npchar)
{
	int bResult = 0;
	if (!CheckAttribute(npchar, "quest.capitainsList")) return bResult;
	aref arCapBase, arCapLocal;
    makearef(arCapBase, npchar.quest.capitainsList);
    int	Qty = GetAttributesNum(arCapBase);
	if (Qty < 1) return bResult;
	string sCapitainId; 
	for (int i=0; i<Qty; i++)
    {
    	arCapLocal = GetAttributeN(arCapBase, i);
        sCapitainId = GetAttributeName(arCapLocal);
    	if (GetCharacterIndex(sCapitainId) > 0) //���� ��� ���
    	{
			bResult++;			
    	}
		else
		{
			DeleteAttribute(arCapBase, sCapitainId);
		}
    }
	if (bResult > 5) bResult = 5;
	return bResult;
}

void SetSeekShipCapParam(ref npchar)
{
	//������� ����-����
	int Rank = sti(pchar.rank) + 5;
	if (Rank > 30) Rank = 30;
	ref sld = GetCharacter(NPC_GenerateCharacter("SeekCap_" + npchar.index, "", "man", "man", Rank, PIRATE, -1, true));
	SetCaptanModelByEncType(sld, "pirate");
	SetShipToFantom(sld, "pirate", true);
	sld.Ship.Mode = "pirate";
	sld.dialog.filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "SeekCap";
	sld.DeckDialogNode = "SeekCap_inDeck";
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "SaveItemsForDead");
	DeleteAttribute(sld, "DontClearDead");
	DeleteAttribute(sld, "AboardToFinalDeck");
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "DontRansackCaptain");
	sld.AlwaysSandbankManeuver = true;
	sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
	sld.DontRansackCaptain = true; //�� ���������
	SetCharacterPerk(sld, "FastReload");
	SetCharacterPerk(sld, "HullDamageUp");
	SetCharacterPerk(sld, "SailsDamageUp");
	SetCharacterPerk(sld, "CrewDamageUp");
	SetCharacterPerk(sld, "CriticalShoot");
	SetCharacterPerk(sld, "LongRangeShoot");
	SetCharacterPerk(sld, "CannonProfessional");
	SetCharacterPerk(sld, "ShipDefenseProfessional");
	SetCharacterPerk(sld, "ShipTurnRateUp");
	SetCharacterPerk(sld, "ShipTurnRateUp");
	SetCharacterPerk(sld, "StormProfessional");
	SetCharacterPerk(sld, "SwordplayProfessional");
	SetCharacterPerk(sld, "AdvancedDefense");
	SetCharacterPerk(sld, "CriticalHit");
	SetCharacterPerk(sld, "Sliding");
	SetCharacterPerk(sld, "Tireless");
	SetCharacterPerk(sld, "HardHitter");
	SetCharacterPerk(sld, "GunProfessional");
	//� ������� ������ ����
	string sGroup = "SeekCapShip_" + sld.index;
	Group_FindOrCreateGroup(sGroup);
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 
	//���������� ������ � ��������� �������� � ����
	npchar.quest = "PortmansSeekShip"; //������ ���� ������ ��� ��������
	npchar.quest.PortmansSeekShip.capName = GetFullName(sld); //��� ����-����
	npchar.quest.PortmansSeekShip.shipName = sld.Ship.name; //��� ����������� �������
	npchar.quest.PortmansSeekShip.shipTapeName = RealShips[sti(sld.Ship.Type)].BaseName; //�������� ����������� �������
	npchar.quest.PortmansSeekShip.shipTape = RealShips[sti(sld.Ship.Type)].basetype; //��� ����������� �������
	npchar.quest.money = ((sti(RealShips[sti(sld.Ship.Type)].basetype)+1) * 1500) + (sti(pchar.rank)*500); //��������������
	sld.quest = "InMap"; //������ ���� ����-����
	sld.city = SelectAnyColony(npchar.city); //��������� �������, ������ ���-��� ������
	sld.quest.targetCity = SelectAnyColony2(npchar.city, sld.city); //��������� �������, ���� �� ������
	Log_TestInfo("���-��� " + npchar.quest.PortmansSeekShip.capName + " ����� ��: " + sld.city + " � ���������� �: " + sld.quest.targetCity);
	sld.quest.cribCity = npchar.city; //�����, ������ ���-��� ���� �������
	//�� �����
	sld.mapEnc.type = "trade";
	sld.mapEnc.worldMapShip = "Galleon_red";
	sld.mapEnc.Name = XI_ConvertString(npchar.quest.PortmansSeekShip.shipTapeName) + " '" + npchar.quest.PortmansSeekShip.shipName + "'";
	int daysQty = GetMaxDaysFromIsland2Island(GetArealByCityName(sld.quest.targetCity), GetArealByCityName(sld.city))+3; //���� ������� ���� � �������
	Map_CreateTrader(sld.city, sld.quest.targetCity, sld.id, daysQty);
	//���������� �� �������
	string sTemp = "SeekShip_checkAbordage" + npchar.index;
	pchar.quest.(sTemp).win_condition.l1 = "Character_Capture";
	pchar.quest.(sTemp).win_condition.l1.character = sld.id;
	pchar.quest.(sTemp).function = "SeekShip_checkAbordage";
	pchar.quest.(sTemp).CapId = sld.id;
	//���������� �� ���������� ��� ��������
	sTemp = "SeekShip_checkSink" + npchar.index;
	pchar.quest.(sTemp).win_condition.l1 = "Character_sink";
	pchar.quest.(sTemp).win_condition.l1.character = sld.id;
	pchar.quest.(sTemp).function = "SeekShip_checkSink";
	pchar.quest.(sTemp).CapId = sld.id;
}
