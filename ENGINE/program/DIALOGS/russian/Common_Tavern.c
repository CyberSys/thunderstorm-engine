#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Tavern\" + NPChar.City + "_Tavern.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 25/06/06
	
	int iTest;
    iTest = FindColony(NPChar.City); // ����� 
    ref rColony;
	if (iTest != -1)
	{
		rColony = GetColonyByIndex(iTest);
	}
	bool ok;
	
	switch(Dialog.CurrentNode)
	{
		case "First time":
			NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar,
					LinkRandPhrase("� ������ ������� �������, ���� ����� ����! �� ����� ����� � �� �� ���� ����� �������������.", "��� ��������� ������ ����� �� ������ � ������� ����. � �� ����� � ������������� � ����� �� ����!", "����, ��������, ���� ������� �� ������� �� ���� ������..."),
					LinkRandPhrase("��� ���� �����, �������?! ��������� ������ ��� ����� ���� ����, ������ ���� �� ����, ������� �����!", "������� ������, ��� �� ����� ����! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."),
					RandPhraseSimple("������ ���� �����, " + GetWorkTypeOfMan(npchar, "") + ", � �� �� ����� ���� ������� ����!", "���, " + GetWorkTypeOfMan(npchar, "") + ", � ��� ���� �� - ������� ������! ��� ��� � ���� �����, ��������: ���� ���� � ������ ����..."));
				link.l1.go = "exit";
				break;
			}
			
            // homo ������� �������� �����
            if (Pchar.questTemp.CapBloodLine == true )
            {


                dialog.Text = LinkRandPhrase("��, ������ ����! " + TimeGreeting() + ".",
                                    "��� ������ ���, ����� ����.",
                                    "������, ��� �� ��������� �� ���, " + GetFullName(pchar) + ". ��� �������� ��������� �����?");
                Link.l1 = "���, � ��� �����, " + NPChar.name + ". �� �������.";
				Link.l1.go = "exit";

                if(Pchar.questTemp.CapBloodLine.stat == "WakerOffer")
                {
            		link.l2 = "�� ���������� ��, ������� " + NPChar.name + ", ��� ����� ������� ������?";
            		link.l2.go = "TStep_0";
                }
                
                if(Pchar.questTemp.CapBloodLine.stat == "PrepareToEscape")
                {
            		link.l2 = "� ���� � ��� ���� ���������� ����.";
            		link.l2.go = "TStep_1";
                }
                break;
    		}

			if(NPChar.quest.meeting == "0")
			{
				dialog.Text = "����� ����������, " + GetAddress_Form(NPChar) + ". ���, ������� � ����������� - ��� �� ��������� �����!";
				Link.l1 = "���������... � " + GetFullName(pchar) + ".";
				Link.l1.go = "meeting";
				NPChar.quest.meeting = "1";
			}
			else
			{
				dialog.Text = LinkRandPhrase("��, " + GetAddress_Form(NPChar) + " " + PChar.name + "! " + TimeGreeting() + ".",
                                    "�, ����� � ��� �����! ��� ������ ���, " + GetAddress_Form(NPChar) + " " + PChar.name + ".",
                                    "������, ��� �� ��������� �� ���, " + GetFullName(pchar) + ". ����� ����� ������� ��� �����?");
				Link.l1 = "���� �� ������� �����, ������� ���� � ��������?";
				Link.l1.go = "crew hire";
				Link.l2 = "� �������, �� ������� �������� �� ���� ��������.";
				Link.l2.go = "int_quests";
				if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
				{
					link.l21 = "������, ��� ����� ���������� � ���������� �����.";
					link.l21.go = "LoanForAll";//(���������� � ��������� ���������)
				}
				link.l3 = pcharrepphrase("���� �� � ����� ���������� ��������� ������?", "��� �� � ���� ��������� �������, � ����� �� ������������ ����� �� ��������� �����.");
				link.l3.go = "room";
				Link.l4 = "���, � ��� �����, " + NPChar.name + ". �� �������.";
				Link.l4.go = "exit";
			}

		break;
		
		case "Meeting":
			dialog.Text = "�, �� ���� � ���� ���������, " + GetAddress_Form(NPChar) + ". ���� ����� " + GetFullName(npchar) + ". � �������� ���� �������.";
			Link.l1 = "��� ����� �������, " + NPChar.name + ".";
			Link.l1.go = "crew hire";
			Link.l2 = NPChar.name + ", ����� ���������...";
			Link.l2.go = "int_quests";
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
			{
				link.l21 = "������, ��� ����� ���������� � ���������� �����.";
				link.l21.go = "LoanForAll";//(���������� � ��������� ���������)
			}
			link.l3 = pcharrepphrase("���� �� � ����� ���������� ��������� ������?", "��� �� � ���� ��������� �������, � ����� �� ������������ ����� �� ��������� �����.");
			link.l3.go = "room";
			Link.l4 = "�������, ������� � ���� �������.";
			Link.l4.go = "exit";
		break;
		
		case "crew hire":
			ok = (rColony.from_sea == "") || (Pchar.location.from_sea == rColony.from_sea);
			ok = sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok;
			
			if (!ok)
			{
				Dialog.text = "� �� ��� ���� �������? ���-�� �� ���� ������ ������� � �����.";
				link.l1 = RandPhraseSimple("�����... � ��� ������������ �� � ��� �����.", "����� ����� � ����...");
				link.l1.go = "exit";
				break;
			}
			
            if (makeint(environment.time) > 22.0 || makeint(environment.time) < 7.0)
			{
				Dialog.text = "������ � ���� � ������� ����� �����, �������� ����� ���������, �� ������ ������� ������, � ��� ������ ���������� ������ �����. ����� ����, �� ������ ����� �������, � ��������� ��?";
				link.l1 = "������. � ���� ���� ��������� �������?";
				link.l1.go = "room";
				link.l2 = "���� �� ���������� ������� �� ������ ������. �� �������.";
				link.l2.go = "exit";
			}
			else
			{
                if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY && sti(NPChar.nation) != PIRATE)
                {
                    Dialog.text = "� �� �������, � ���� ���-�� ������, ����� �� � ���� �� ������? ����� �������, ��� ������ �� ����.";
					link.l1 = "�������.";
					link.l1.go = "exit";
                }
                else
                {
					if (CheckAttribute(NPChar, "CrewHired.PGGId") && GetNpcQuestPastDayParam(NPChar, "CrewHired") < 3 && GetCrewQuantity(rColony) == 0)
					{
						Dialog.text = "��������, �� ������ ��� ������. ��� ���� � �������� ����� " + GetFullName(CharacterFromID(NPChar.CrewHired.PGGId)) + ".";
						link.l1 = "��, ����!";
						link.l1.go = "exit";
					}
					else
					{
						DeleteAttribute(NPChar, "CrewHired"); //�� ���� ������.
						NextDiag.CurrentNode =  NextDiag.TempNode;
						DialogExit();
						LaunchHireCrew();
					}
				}
			}
		break;
		
        case "int_quests":
			dialog.text = "� ���� ��������.";
			link.l1 = "� ��� ������. �� �������� ���?";
			link.l1.go = "work";
			//homo 15/06/06 �����
			link.l2 = LinkRandPhrase("�������� ��� ��������� �������?","��� ���������� � ���� �����?",
                                    "��� ����� ����� �� ����?");
			link.l2.go = "rumours_tavern";
			//homo
			link.l3 = "� �� ������� ����.";
			link.l3.go = "quests";//(���������� � ���� ������)
			if (CheckAttribute(pchar, "GenQuest.SeekSpy.City")) //����� ���� ��� ������ ������
			{
				link.l4 = "� �������� �� ��������� �����������. ����� ���, �� �� ������� ������ ��������������� � ��������� ����� � ������?";
				if (pchar.GenQuest.SeekSpy.City == "0")
				{
					link.l4.go = "SeekSpy_Seen";
				}
				else
				{
					link.l4.go = "SeekSpy_NotSeen";
				}
			}
			link.l9 = "�������, ������. �� ��������.";
			link.l9.go = "exit";
		break;
		
		case "work":
			if (sti(NPChar.nation) != PIRATE && GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY)
			{
				dialog.text = RandPhraseSimple("�� ���� ���� ��������, �� ������ ��� ������ " + NationNameGenitive(sti(pchar.nation)) + "!", "���� � � ������� ������ �������, �� ������������ � " + NationNameAblative(sti(pchar.nation)) + " � �� ����.");
				link.l1 = RandPhraseSimple("��, ���� �����... �����, ������.", "��, ��� ������...");
				link.l1.go = "exit";
				break;
			}
			dialog.text = "������, ��������? ���� ���� ������������ ���������� � �������� �������� - ��� ��������� ����� �������� ��� ��������� �����\n��� ������ ���������� ��� �� � ������� ������-������ ����� - ��� ����� ���� ������ ��� ����� �������\n���� �� �� ���������� ���������� ���������, �� ����������� �� ������ - ��������, ��� ����-������ ���� ��������� �� ������ �������\n�� � ���������� ���� ������������ ���� ��������� �� ������ � ������ �����������. �� �� �� ����� �������� ���� ��������� � �������� �������������.";
            if (sti(Pchar.Ship.Type) != SHIP_NOTUSED)
            {
				if (!CheckAttribute(npchar, "work_date") || GetNpcQuestPastDayParam(npchar, "work_date") >= 1 || bBettaTestMode)
    			{					
					SaveCurrentNpcQuestDateParam(npchar, "work_date");
					if (isBadReputation(pchar, 40))
					{
						dialog.text = RandPhraseSimple("�� ������, �����, ������� ����� ������� �� ������ ������, � ���� � ������� ����� ������. ������ ��� ��������� � ����, ��������, ��������... �� ����� ���� ������, ��� ������ �� ���� �� ����� ����������.", "��� �������, � ���� ����� ���������, ������� ����� �������������. �� � ���� ���� �� ������ � �����! �� ������� �������, ��� � ����� ���� ���-������ ���������� ����������� � ��������?");
						link.l1 = RandPhraseSimple("��, ����� ��� ��� ���� ������� ����! ���!", "�� ����, ���� ��� ������! ��-��!");
						link.l1.go = "exit";
						break;
					}
					switch (rand(1))
        			{
                        case 0: //������
                            if (!CheckQuestAttribute("generate_convoy_quest_progress", "begin"))
                			{
								if (GetCompanionQuantity(PChar) == COMPANION_MAX)
								{
									dialog.text = "����� �������� ������ ����������� �� ������� �� ����� ����������. �� ���� � ���� �����, �� ��� ������������ ������� � ������� ������� �������. �� �� ������.";
									link.l1 = "�����, � � �����. �����, ������� �����, ����� ���������� ���� �������� ����������, ��-��.";
									link.l1.go = "exit";
								}
								else
								{
									if (rand(5) > 1)
									{
										dialog.text = "�� ������� �� ��� ��������� ������, ��� ���� �������� ��� ��� ����� ���������� ��������... �-�-�, ��� ��� ��� � ��, ������, ����� � �������? �������� � ���.";
										link.l1 = RandPhraseSimple("��� �� �������, � ��� ����� ����� ����?", "� ��� �� �����? �������, ��� ����� � ��� ����� ����?");
										link.l1.go = "ConvoyAreYouSure";
									}
									else
									{
										dialog.text = "������ � ���� ��������� ��������, ������� ����� �������������, �� ������ ������ ���. ����� ����, ���� ������� � ������ ����.";
										link.l1 = RandPhraseSimple("�����, �� ��� � ���� ���.", "���, �� ������... �� �����, ������.");
										link.l1.go = "exit";
									}
								}
                			}							
            			break;

            			case 1: // ��������
							if (rand(6) > 1)
							{
								dialog.Text = "����� ��� ���� ��� ����� �������, ����� ��������� ���-����. �� ��� � ��!";
								link.l1 = RandPhraseSimple("��� �� �������, ����� �������� ����� ����� �� ����?", "� ��� �� �����? �������, ��� ����� ������� �� ��� ��������?");
								Link.l1.go = "PassangerAreYouSure";
							}
							else
							{
								dialog.text = "���������� ����� � ���� ������, �� ������, ��� �����, ������ ���. ��� ��� �� ��������...";
								link.l1 = RandPhraseSimple("���� ��� ������, �� ���� ��...", "���, �� ������... �� �����, ������.");
								link.l1.go = "exit";
							}
            			break;
        			}
    			}
    			else
    			{
					dialog.text = RandPhraseSimple("�� ��� ��������� ���� ������� � ������.", "�� �� �������� ��� � ������ �������...");
					link.l1 = "���� ������, ��� ����� ����� ������, ������� � ���������!";
                    link.l1.go = "SayAgain";
    			}
			}
			link.l2 = "��������� �� ������. ����� ��������� � ���-������ ������.";
			link.l2.go = "First time";
		break;

        case "SayAgain":
		    dialog.text = "� �� ���� ��� ��� ������! ��� ������ ������ ��������!";
			link.l1 = "�����, �����, ��� �����.";
			link.l1.go = "exit";
        break;

        case "ConvoyAreYouSure":
		    dialog.text = RandPhraseSimple("�������, ��� ������ ���������� ������.", "������ ���, ���� ����������� � ����?");
			Link.l1 = "�������, ������ � ��� � ����������.";
			Link.l1.go = "exit";
			pchar.quest.destination = findTraderCity(npchar);
			AddDialogExitQuest("prepare_for_convoy_quest");
        break;

        case "PassangerAreYouSure":
		    dialog.text = RandPhraseSimple("�������, ��� ������ ���������� ��������.", "������ ���, ���� ����������� � ����?");
			Link.l1 = "�������, ������ � ��� � ����������.";
			Link.l1.go = "exit";
			sGlobalTemp = findPassangerCity(npchar);
			AddDialogExitQuest("prepare_for_passenger_quest");
        break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "room":
   			if (chrDisableReloadToLocation || CheckAttribute(pchar, "questTemp.different.Church_NightGuard")) //���-�� ������ ������� � ��.
			{
				dialog.text = "������ ������� ������, " + GetAddress_Form(NPChar) + ", ����� �� ���� ������.";
				link.l1 = "����, ����� ����...";
				link.l1.go = "exit";
				break;
			}
			if (CheckAttribute(pchar, "questTemp.State.Open") && npchar.id == "Tortuga_tavernkeeper")//���.�������, ����� �6, ���������� ����� � �������
			{
				dialog.text = "�� ��������� ��� �� ������ ������, � �� ���� ����� � ��� ��� ������. ������� ����, ����� � ���������...";
				link.l1 = "������, ��������, �������.";
				link.l1.go = "exit";
				break;
			}
			//-->> ����� ����������
			if (pchar.questTemp.different == "FackWaitress_toRoom")
			{
				dialog.text = "�� ������ ����� �������? �� ��������. � ��� 100 ����� - � ������ ���� ������.";
				if (sti(pchar.money) >= 100)
				{
					link.l1 = "������� ���� ������, �������� �������, ��������...";
					link.l1.go = "exit";
					AddMoneyToCharacter(pchar, -100);
					LocatorReloadEnterDisable(npchar.city + "_tavern", "reload2_back", false);
					pchar.questTemp.different = "FackWaitress_toRoomUp";
				}
				else
				{
					link.l1 = "��, �, �������, ������� �������...";
					link.l1.go = "exit";
				}
				break;
			}
			if (pchar.questTemp.different == "FackWaitress_toRoomUp")//����� ����������
			{
				dialog.text = "������� ����, ����� � ���������...";
				link.l1 = "������, ��������, �������.";
				link.l1.go = "exit";
				break;
			}
			//<<-- ����� ����������
			dialog.text = "�� ����� ���� �� ���������� ������������?";
			if(!isDay())
			{
				link.l1 = "�� ����.";
				link.l1.go = "room_day";
			}
			else
			{
				link.l1 = "�� ����.";
				link.l1.go = "room_night";
				link.l2 = "�� ���������� ����.";
				link.l2.go = "room_day_next";
			}
		break;

		case "room_day":
			dialog.text = "��� ��������� ��� � 5 �������.";
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = "����. ��� ���� ������.";
				link.l1.go = "room_day_wait";
			}
			link.l2 = pcharrepphrase("�� ������ ����������� �����? ��� ��, �� ����� ���� � ����� ������ � ����� ����.",
            "���, ������, ����� �������, ��� ��� �� ������� �� ��� ����� ��������. � ��������� � ����� ����.");
			link.l2.go = "hall_day_wait";
			link.l3 = "�� ������. ����� ��������� � ���-������ ������.";
			link.l3.go = "int_quests";
		break;

		case "room_day_next":
			dialog.text = "��� ��������� ��� � 10 �������.";
			if (makeint(pchar.money) >= 10)
			{
				link.l1 = "����. ��� ���� ������.";
				link.l1.go = "room_day_wait_next";
			}
			link.l2 = pcharrepphrase("�� ������ ����������� �����? ��� ��, �� ����� ���� � ����� ������ � ����� ����.",
            "���, ������, ����� �������, ��� ��� �� ������� �� ��� ����� ��������. � ��������� � ����� ����.");
			link.l2.go = "hall_day_wait";
			link.l3 = "�� ������. ����� ��������� � ���-������ ������.";
			link.l3.go = "int_quests";
		break;

		case "room_night":
			dialog.text = "��� ��������� ��� � 5 �������.";
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = "����. ��� ���� ������.";
				link.l1.go = "room_night_wait";
			}
			link.l2 = pcharrepphrase("�� ������ ����������� �����? ��� ��, �� ����� ���� � ����� ������ � ����� ����.",
            "���, ������, ����� �������, ��� ��� �� ������� �� ��� ����� ��������. � ��������� � ����� ����.");
			link.l2.go = "hall_night_wait";
			link.l3 = "�� ������. ����� ��������� � ���-������ ������.";
			link.l3.go = "int_quests";
		break;

		case "room_night_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate("wait_night");
		break;

		case "hall_night_wait":
			if (rand(2) == 1)
            {
                Dialog.text = "��������, ���������! ������ ��� � ����� ���� ����������� �� ���������.";
				link.l1 = "�����, �����... ��� �����.";
				link.l1.go = "exit";
            }
            else
            {
				DialogExit();
				NextDiag.CurrentNode = NextDiag.TempNode;
				TavernWaitDate("wait_night");
			}
		break;

		case "room_day_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate("wait_day");
		break;

		case "room_day_wait_next":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -10);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate("wait_day");
		break;

		case "hall_day_wait":
			if (rand(1) == 1)
            {
                Dialog.text = "��������, ���������! ������ ��� � ����� ���� ����������� �� ���������.";
				link.l1 = "�����, �����... ��� �����.";
				link.l1.go = "exit";
            }
            else
            {
				DialogExit();
				NextDiag.CurrentNode = NextDiag.TempNode;
				TavernWaitDate("wait_day");
			}
		break;
		
		case "tavern_keeper":
			NextDiag.TempNode = "First time";

			dialog.text = "���������, ����� ��� ���������� ��� ���������� � ���� �������, ��� � ������ ����������!";
			link.l1 = "�����, �����, � ��� �����.";
			link.l1.go = "exit_sit";
		break;

		case "exit_sit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AddDialogExitQuest("exit_sit");
		break;
		//==> eddy. ����� ����, ������� �� ������� �� ������
		case "SeekSpy_Seen":
			dialog.text = NPCStringReactionRepeat("��, ��� ��������... �� ������, ���� ���� ���, ������� ����� ��� ������� �� �������. �� ���� ������� ���������, �� �������� �� ���, �� � ������ �������, ������� � ���� ��������. ���� �� ���� �� �������� - � ��� ����� �����, � ��� � ������ �����.", 
				"�� ��� �������� �� ��� ����, ����� ��� ������� ���� ������.", "����� �� �� �����... � ��� ��� ���� ������.", "������� ����� ���������� ���� � �� ��?!", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�������, ����, ��������. ��� ��, ����� ������ ����� ����.", "��, �� ������...",
                      "��, �� ��...", "������, ��������...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;
		case "SeekSpy_NotSeen":
			dialog.text = NPCStringReactionRepeat("���, ������ �� �����, ����� � ���� ���������� �������.", 
				"�� ��� �������� �� ��� ����, ����� ��� ������� ���� ������.", "����� �� �� �����... � ��� ��� ���� ������.", "������� ����� ���������� ���� � �� ��?!", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�� ��� ��, � �� ���� �������, ��������.", "��, �� ������...",
                      "��, �� ��...", "������, ��������...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;
		
		//==> homo ������� �����
		case "TStep_0":
			dialog.text = "����� �� ��������. � � ����� �� ������������.";
			link.l1 = "���������.";
			link.l1.go = "Exit";
			int n= FindLocation("Bridgetown_tavern");
            locations[n].reload.l2.disable = false;
		break;
		
		case "TStep_1":
			dialog.text = "��� �� ���� �� ��� � ���, ��������� ������?";
			link.l1 = "���� ���������... ��� �� �� ��������, �� ������ ������, ��������� ���������� ������ � ���� ����������, ������� ����. �� ���� ������ ��� �������� ��� ������� �� ��������, �������� ����� �������� ����������? �� �� ������, � ������� ��� ���� ��� ������...";
			link.l1.go = "TStep_2";
		break;
		
		case "TStep_2":
			dialog.text = "��, ����������, ���.";
			link.l1 = "�� ������? ��� ��� ������?";
			link.l1.go = "TStep_3";
            if (makeint(pchar.money) >= 2500)
            {
    			link.l1 = "� ����� �� ��������, ��� � ���� ��� �������� �������� ������� ������ � �������... ��, ������, ������ ��������? �� � ���������, ��� � ����� ���� ����, �� ����� ����� ��� � �������.";
    			link.l1.go = "TStep_4";
            }
		break;
		
		case "TStep_3":
			dialog.text = "����� �� ���� ������, ������.";
			link.l1 = "����� ����������.";
			link.l1.go = "exit";
		break;
		
		case "TStep_4":
			dialog.text = "�������, � ���� ���� ��, ��� ��� �����, ������ ����. ������ �������, �������� �� �� ��� ����� ��������� �������, �� ����� ������ ���������. ��� ��� ��������� ��������� �� �������� ����������� ����� - ������� � ����. ������, �� �� ������� ���.";
			link.l1 = "����� ����������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -1000);
			Pchar.questTemp.CapBloodLine.stat = "PrepareToEscape1";
			AddQuestRecord("WeaponsForEscape", "2");

		break;
	}

}

string findTraderCity(ref NPChar)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		nation = GetNationRelation(sti(npchar.nation), sti(colonies[n].nation));
		if (nation != RELATION_ENEMY && colonies[n].id != "Panama" && colonies[n].nation != "none" && GetIslandByCityName(npchar.city) != colonies[n].islandLable) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}

string findPassangerCity(ref NPChar)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		nation = GetNationRelation(sti(npchar.nation), sti(colonies[n].nation));
		if (colonies[n].nation != "none" && colonies[n].id != "Panama" && GetIslandByCityName(npchar.city) != colonies[n].islandLable) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}
