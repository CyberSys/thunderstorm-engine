// boal 08/04/06 ����� ������ �����
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;
	string sTemp;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Shipyard\" + NPChar.City + "_Shipyard.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 16/06/06
    bool ok;
    int iTest = FindColony(NPChar.City); // ����� ��������
    ref rColony;
	if (iTest != -1)
	{
		rColony = GetColonyByIndex(iTest);
	}
	switch(Dialog.CurrentNode)
	{
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "ship_tunning_not_now":  // ������ ������, �� ������ ������, ����� ������������.
			DialogExit();
			NextDiag.CurrentNode = "Master_Ship";  // ����� ��� �������, ����� ��� ��-�� LoadSegment
		break;
		
		case "fight":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
			AddDialogExitQuest("MainHeroFightModeOn");
		break;
		
		case "First time":
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
			NextDiag.TempNode = "First time";
			
			//homo ������� �����
            if (Pchar.questTemp.CapBloodLine == true )
            {
                dialog.Text = LinkRandPhrase("��, ������ ����! " + TimeGreeting() + ".",
                                    "��� ������ ���, ����� ����.",
                                    "������, ��� �� ��������� �� ���, " + GetFullName(pchar) + ". ��� �������� ��������� �����?");
                Link.l1 = "���, � ��� �����, " + NPChar.name + ". �� �������.";
				Link.l1.go = "exit";
				break;
            }
			
			if(NPChar.quest.meeting == "0")
			{
				dialog.Text = TimeGreeting() + "! � "+ GetFullName(NPChar) + ", �������� ���� �����.";
				Link.l1 = "��� ����������. � ������� " + GetFullName(Pchar) + ".";
				Link.l1.go = "meeting";
				NPChar.quest.meeting = "1";
			}
			else
			{
				//--->> ����� ������� ������ �� �����
				//���� ������
				if (rand(1) && pchar.questTemp.different == "free" && GetNpcQuestPastDayWOInit(npchar, "questShipyardsMap") > 7 && !CheckAttribute(pchar, "questTemp.different.ShipyardsMap"))
				{
					dialog.text = "����������, � ���� ���� � ��� ���� ������ ����. �������, ������� ��� ������...";					
					link.l1 = "������� ���������, ��� �� ����.";
					link.l1.go = "ShipyardsMap_1";
					SaveCurrentNpcQuestDateParam(npchar, "questShipyardsMap");
					break;
				}
				//<<--- ����� ������� ������ �� �����			
				
				dialog.Text = pcharrepphrase("�, ��� ����� ��? �� �� �����, ������ �� ������.",
                                        TimeGreeting() + ", ��� � ���� ������ ���, " + GetAddress_Form(NPChar) + "?");
				Link.l1 = pcharrepphrase("���-���, � ����� - �� �������.",
                                        "����� ��������������� �������� �����.");
				Link.l1.go = "Shipyard";
				link.l11 = "��� ����� ������ �� �������.";
			    link.l11.go = "Cannons";
				Link.l2 = "� ������ ���� ����������.";
				Link.l2.go = "quests"; //(���������� � ���� ������)
				// -->
    			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
				{
					link.l3 = "� ����� �� ���������� � ���� � ����� ����������.";
					link.l3.go = "LoanForAll";
				}
  				if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") //����� ���� - �� ����� � ����� �������
				{
					link.l7 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
						GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
					link.l7.go = "IntelligenceForAll";
				}
				//--> ����� ����.
	    		if (CheckCharacterItem(pchar, "Azzy_bottle"))
	            {
	                link.l5 = "������, � ���� ���� ���� ����� - ��������. ��� ����� ������������� �� �������. ������� ������� ���? � �������.";
	                if (npchar.id == pchar.questTemp.Ascold.ShipyarderId)
	                {
	                    link.l5.go = "AggryHelp";
						pchar.questTemp.Azzy = "Azzy_IHaveLamp";
	                }
	                else
	                {
	                    link.l5.go = "AzzyTryTalk";
	                }
	            }
	            //<-- �� ����� ����.
				//�������� ���������� ������ ������� ������ �� �����
				if (CheckCharacterItem(pchar, "ShipyardsMap"))
				{				
					link.l6 = "� �������� ���� ���������. ������ �� ����� " + XI_ConvertString("Colony" + pchar.questTemp.different.ShipyardsMap.city + "Gen") + " � ����.";
					link.l6.go = "ShipyardsMapOk_1";
				}
				Link.l9 = "��� ����� ����.";
				Link.l9.go = "exit";
			}
		break;
		
		case "Meeting":
				dialog.Text = "������, � ��� ������� � ����� ��������. ��� ����� � ����� �������.";
				Link.l1 = "�����������, " + GetFullName(NPChar) + ". ������� ��������� ��, ��� �� ������ ��� ����������.";
				Link.l1.go = "Shipyard";
				link.l11 = "��� ����� ������ �� �������.";
				link.l11.go = "Cannons";
				Link.l2 = "� ������ ���� ����������.";
				Link.l2.go = "quests";
				// -->
    			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
				{
					link.l3 = "� ����� �� ���������� � ���� � ����� ����������.";
					link.l3.go = "LoanForAll";//(���������� � ���� ������)
				}
  				if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") //����� ���� - �� ����� � ����� �������
				{
					link.l7 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
						GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
					link.l7.go = "IntelligenceForAll";
				}
				//--> ����� ����.
	    		if (CheckCharacterItem(pchar, "Azzy_bottle"))
	            {
	                link.l5 = "������, � ���� ���� ���� ����� - ��������. ��� ����� ������������� �� �������. ������� ������� ���? � �������.";
	                if (npchar.id == pchar.questTemp.Ascold.ShipyarderId)
	                {
	                    link.l5.go = "AggryHelp";
						pchar.questTemp.Azzy = "Azzy_IHaveLamp";
	                }
	                else
	                {
	                    link.l5.go = "AzzyTryTalk";
	                }
	            }
	            //<-- �� ����� ����.
				Link.l9 = "� ������ ����. ��������� ���.";
				Link.l9.go = "exit";
		break;
		
		case "shipyard":
            ok = (rColony.from_sea == "") || (Pchar.location.from_sea == rColony.from_sea);
		    if (sti(Pchar.Ship.Type) == SHIP_NOTUSED || ok)
			{
			    NextDiag.CurrentNode = NextDiag.TempNode;
				DialogExit();
				LaunchShipyard(npchar);
			}
			else
			{
			    dialog.text = NPCharRepPhrase(npchar, pcharrepphrase("������ ����? � ��� �� ���� �������? � ����� ��� ���!",
				                                                     "������� ��������, ���� �� �������� ����! � ����� ��� ������ �������!"),
													  pcharrepphrase("� �� ���� ������ ������� � �����, ������� " +GetFullName(pchar)+ ". �������, ��� �� '������� ���������'?",
													                 "�������, ������� ������� ����� � �����. ������������ ������� � ���������."));
				link.l1 = NPCharRepPhrase(npchar, pcharrepphrase("" +RandSwear()+"����� ������������!!! �����, ������ �������, ��� ��������!",
				                                                 "� �� ����� ��� �������� " +GetFullName(npchar)+ ", ������� �� ������ ������� �������."),
												  pcharrepphrase("���. ��� ������� ���������� Black Perl. ��� ��� �� �������? ��-��! �����!",
												                 "������� �� �����, ����������� �� ������������."));
			    link.l1.go = "exit";
			}
		break;
		
		case "Cannons":
            ok = (rColony.from_sea == "") || (Pchar.location.from_sea == rColony.from_sea);
		    if (sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok)
			{
			    NextDiag.CurrentNode = NextDiag.TempNode;
				DialogExit();
    			LaunchCannons(sti(rColony.StoreNum));
			}
			else
			{
			    dialog.text = NPCharRepPhrase(npchar, pcharrepphrase("������ ����? � ��� �� ���� �������? � ����� ��� ���!",
				                                                     "������� ��������, ���� �� �������� ����! � ����� ��� ������ �������!"),
													  pcharrepphrase("� �� ���� ������ ������� � �����, ������� " +GetFullName(pchar)+ ". �������, ��� �� '������� ���������'?",
													                 "�������, ������� ������� ����� � �����. ������������ ������� � ���������."));
				link.l1 = NPCharRepPhrase(npchar, pcharrepphrase("" +RandSwear()+"����� ������������!!! �����, ������ �������, ��� ��������!",
				                                                 "� �� ����� ��� �������� " +GetFullName(npchar)+ ", ������� �� ������ ������� �������."),
												  pcharrepphrase("���. ��� ������� ���������� Black Perl. ��� ��� �� �������? ��-��! �����!",
												                 "������� �� �����, ����������� �� ������������."));
			    link.l1.go = "exit";
			}
		break;
		// ����� ������� ������ �� �����
		case "ShipyardsMap_1":
			dialog.text = "��� ����� ��������� ���� ������ ������. ����� ������ ������...";
			link.l1 = "��. ��� �� ������ � ��� ��� �����?";
			link.l1.go = "ShipyardsMap_2";
		break;
		case "ShipyardsMap_2":
			pchar.questTemp.different = "ShipyardsMap";
			pchar.questTemp.different.ShipyardsMap = "toTarget";
			pchar.questTemp.different.ShipyardsMap.city = findShipyardCity(npchar);
			pchar.questTemp.different.ShipyardsMap.what = LinkRandPhrase("�������", "������", "�������");
			pchar.questTemp.different.ShipyardsMap.chance = rand(5); //�������� �������
			if (rand(12) < GetCharacterSPECIAL(pchar, "Charisma"))
			{
				pchar.questTemp.different.ShipyardsMap.skladFight = false; //�� ����� ����� �� ������
			}
			else
			{
				pchar.questTemp.different.ShipyardsMap.skladFight = true; //����� ����� �� ������
			}
			pchar.questTemp.different.ShipyardsMap.sklad = rand(3)+1; //������� ����� �������� �� ������ �� �������� �����
			pchar.questTemp.different.ShipyardsMap.fightQty = 2 + rand(3); //���������� ������ �� �����
			dialog.text = "��� ����� ��������, ��� �� ����� " + XI_ConvertString("Colony" + pchar.questTemp.different.ShipyardsMap.city + "Gen") + " ������� ��������� ��������� " + pchar.questTemp.different.ShipyardsMap.what + ". ��� ����� ������, �� ������� ������� ������.";
			link.l1 = "� ��� � ������ �������?";
			link.l1.go = "ShipyardsMap_3";
		break;
		case "ShipyardsMap_3":
			dialog.text = "�������� ��� ���. �������� ����������� � ���, ��� ������, ����� ��� ��������, ��������� ��������� �� ���� � ������� �����. � ���� ���� ������ - �� ��� �����.";
			link.l1 = "� ��� �� �����������?";
			link.l1.go = "ShipyardsMap_4";
		break;
		case "ShipyardsMap_4":
			dialog.text = "��, � ��������� ��������� ������ ���. ��� �� ��� �������� - ���� ��������. �� ��� ������ � ����� ����� ���������, ���� ������ ����� �������� �� �������������� �������� ���������� � ������� ����������.";
			link.l1 = "��� ��� ������?";
			link.l1.go = "ShipyardsMap_5";
		break;
		case "ShipyardsMap_5":
			dialog.text = "���� ������ ����� ����� ��������������� �������, � ������� ��� ����������, �� ����� ����� ���������� ������. � �������� ������ ������ �������� ����� �������... �� ���, �� �������� ������� �� ��� ����?";
			link.l1 = "���, ��� ��� �����������. �������� '���� � �����' ������� ��� ������������� �������.";
			link.l1.go = "ShipyardsMap_disagree";
			link.l2 = "�����, ��� ����� ���� ��������. ������!";
			link.l2.go = "ShipyardsMap_agree";
		break;

		case "ShipyardsMap_disagree":
			dialog.text = "��, �� ��� ��, ����� ����.";
			link.l1 = "��� ����...";
			link.l1.go = "exit";
			pchar.questTemp.different = "free";
			DeleteAttribute(pchar, "questTemp.different.ShipyardsMap");
		break;
		case "ShipyardsMap_agree":
			dialog.text = "�� ��� ��, ����� ����� ���. � ���� ����� ��� � ������������. � ������ �� ����� �� ����������.";
			link.l1 = "�������...";
			link.l1.go = "exit";
			sld = ItemsFromID("ShipyardsMap");
			sld.shown = true;
			sld.startLocation = pchar.questTemp.different.ShipyardsMap.city + "_Shipyard";
			sld.startLocator = "item1";
			sld.ShipName = pchar.questTemp.different.ShipyardsMap.what;
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ���������� 
			ReOpenQuestHeader("ShipyardsMap");
			AddQuestRecord("ShipyardsMap", "1");
			AddQuestUserData("ShipyardsMap", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData("ShipyardsMap", "sTarget", XI_ConvertString("Colony" + pchar.questTemp.different.ShipyardsMap.city + "Dat"));
			AddQuestUserData("ShipyardsMap", "sShip", pchar.questTemp.different.ShipyardsMap.what);
		break;

		case "Allarm":
			dialog.text = "�� �� ��������! �� ��� �� ��� �������?! ��, �������, ������ ���� ��� �������. � ��, ������, �������� ������!";
			link.l1 = "������?..";
			link.l1.go = "fight";
			LAi_SetPlayerType(pchar);
			AddCharacterExpToSkill(pchar, "FencingLight", 10);
			AddCharacterExpToSkill(pchar, "Fencing", 10);
			AddCharacterExpToSkill(pchar, "FencingHeavy", 10);
			ChangeCharacterReputation(pchar, -1);
			for (i=1; i<=sti(pchar.questTemp.different.ShipyardsMap.fightQty); i++)
			{
				iTemp = 10 + rand(10);
				sld = GetCharacter(NPC_GenerateCharacter("shipowner_"+i, "shipowner_"+(rand(15)+1), "man", "man", iTemp, sti(pchar.nation), 0, true));
				FantomMakeCoolFighter(sld, iTemp, 70, 70, "topor"+(rand(2)+1), "pistol2", 10);
				LAi_SetWarriorType(sld);
				LAi_group_MoveCharacter(sld, npchar.chr_ai.group);
				ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
			}
		break;

		case "Allarm_1":
			dialog.text = "�� ���, �������� ��� �������?! � ��, ������, �������� ������!";
			link.l1 = "������?..";
			link.l1.go = "fight";
			ChangeCharacterReputation(pchar, -1);
			for (i=1; i<=5; i++)
			{
				iTemp = 10 + rand(10);
				sld = GetCharacter(NPC_GenerateCharacter("shipowner_"+i, "shipowner_"+(rand(15)+1), "man", "man", iTemp, sti(pchar.nation), 0, true));
				FantomMakeCoolFighter(sld, iTemp, 70, 70, "topor"+(rand(2)+1), "pistol2", 10);
				LAi_SetWarriorType(sld);
				LAi_group_MoveCharacter(sld, npchar.chr_ai.group);
				ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
			}
		break;

		case "ShipyardsMapOk_1":
			sTemp = pchar.questTemp.different.ShipyardsMap.what;
			dialog.text = "�������! ��, ������� ���������, ��� �� ��� �� �������� " + strcut(sTemp, 0, strlen(sTemp)-2) + "...";
			link.l1 = "��������.";
			link.l1.go = "ShipyardsMapOk_2";
		break;
		case "ShipyardsMapOk_2":
			TakeItemFromCharacter(pchar, "ShipyardsMap");
			if (sti(pchar.questTemp.different.ShipyardsMap.chance) > 0)
			{
				dialog.text = "��, ��� ������ ����� ������������ ��������. � ����� ��������� ��� �� ���� " + FindRussianMoneyString(sti(pchar.questTemp.different.ShipyardsMap.chance)*1000) + ".";
				link.l1 = "���������!";
				link.l1.go = "ShipyardsMapOk_5";
			}
			else
			{
				dialog.text = "���-���... ��, �������� ��� ��������. ������ ��������� � ��� ���� �� ����. ��, �������, ������ �� �� ����������.";
				link.l1 = "�� ���������� ��, ��� ������ �� �����. ������ ��� � ��� � ������. � �� �� ���������� � ���� ��������!";
				link.l1.go = "ShipyardsMapOk_3";
			}
		break;

		case "ShipyardsMapOk_3":
			dialog.text = "���, �� ��� � ������ ������ �� ���� ���������. ��������, �� ��� ������ ��� �� �����.";
			link.l1 = "��, � �� ������ ��� �������� �������, �� ������� ���������� �� ����.";
			link.l1.go = "ShipyardsMapOk_4";
			link.l2 = "������� ��� ������! ����� ���� �����!";
			link.l2.go = "Allarm_1";
		break;
		case "ShipyardsMapOk_4":
			dialog.text = "������� ���� ���������, �� ����� �������� ����� ��� ���-������ �� ������. ���� ������ ������ �� �����.";
			link.l1 = "� ��� �����...";
			link.l1.go = "exit";
			DeleteAttribute(pchar, "questTemp.different.ShipyardsMap");
			AddQuestRecord("ShipyardsMap", "3");
			AddQuestUserData("ShipyardsMap", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			CloseQuestHeader("ShipyardsMap");
		break;

		case "ShipyardsMapOk_5":
			dialog.text = "�������� ��������. ������� ��� �� ����������� ������.";
			link.l1 = "�� �� �� ���...";
			link.l1.go = "exit";
			AddCharacterExpToSkill(pchar, "Commerce", 20);
			ChangeCharacterReputation(pchar, 5);
			AddMoneyToCharacter(pchar, sti(pchar.questTemp.different.ShipyardsMap.chance)*1000);
			AddQuestRecord("ShipyardsMap", "4");
			AddQuestUserData("ShipyardsMap", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData("ShipyardsMap", "iMoney", sti(pchar.questTemp.different.ShipyardsMap.chance)*1000);
			CloseQuestHeader("ShipyardsMap");
			DeleteAttribute(pchar, "questTemp.different.ShipyardsMap");
		break;

	}
}

string findShipyardCity(ref NPChar)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		nation = GetNationRelation(sti(npchar.nation), sti(colonies[n].nation));
		if (sti(colonies[n].nation) != PIRATE && colonies[n].id != "Panama" && colonies[n].nation != "none" && GetIslandByCityName(npchar.city) != colonies[n].islandLable) //�� �� ���� ������
		{
			if (GetCharacterIndex(colonies[n].id + "_shipyarder") != -1)
			{
				storeArray[howStore] = n;
				howStore++;
			}
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}