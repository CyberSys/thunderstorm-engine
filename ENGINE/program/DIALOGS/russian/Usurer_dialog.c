// boal 25/04/04 ����� ������ Usurer
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, chr, sld;
	aref Link, NextDiag;
	string NPC_Area, sTemp, sTitle;
	int LoanSum, LoanInterest, LoanPeriod, LoanResult, iPastMonths, DepositSum, DepositInterest, DepositResult, iNum;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;

	NPC_Area = Npchar.City;
 
	// ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Usurer\" + NPChar.City + "_Usurer.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 16/06/06

	if (!CheckAttribute(npchar, "quest.trade_date"))
    {
        npchar.quest.trade_date = "";
    }
    if (!CheckAttribute(npchar, "quest.FindCitizenNoShip"))
    {
        npchar.quest.FindCitizenNoShip = 0;
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
			if (LAi_group_GetPlayerAlarm() > 0)
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
			//�������, ����� �5, �������� �� ������� ����� -->
			if (CheckAttribute(npchar, "PLQ5Money"))
			{
				dialog.text = "�� �������� ��� ��� 50 �����?";
				link.l1 = "��� ���, � ���� ���������. �����.";
				link.l1.go = "exit";
				link.l2 = "�����, ��� � ���� ������ �� ������ � ���� �����. ��������, ��� � ���������.";
				link.l2.go = "StepPL5End_close";
				if (sti(pchar.money) >= 50000)
				{
					link.l3 = "��, � ������ ���� 50 ����� �� ����� �� ��� �������.";
					link.l3.go = "StepPL5End_1";
				}
				break;
			}
			//<--�������, ����� �5, �������� �� ������� �����
			//-->> ���� ������ ����� ���������� ����������� ������
			if (rand(1) && pchar.questTemp.different == "free" && !CheckAttribute(npchar, "quest.usurersJewel") && GetNpcQuestPastDayWOInit(npchar, "usurersJewel") > 7)
			{
				dialog.text = "������������, �� ��� ������ �������! ���� �������� ��� ���� ����.";
				link.l1 = "����������� ������. ��� �� ����?";
				link.l1.go = "usurersJewel_1";
				npchar.quest.usurersJewel = "inSeek"; //������ ���� ���������� �� ������ �����
				SaveCurrentNpcQuestDateParam(npchar, "usurersJewel");
				break;
			}
			//<<-- ���� ������ ����� ���������� ����������� ������
			dialog.text = NPCStringReactionRepeat("������������, " + GetAddress_Form(npchar) + ". ��� � ���� ��� ������?", 
				"�, ��� ����� ��? ����� ����������, " + GetAddress_Form(npchar) + " ��� �� ������ �� ���� ���?", 
				"��, ����� ��, " + GetAddress_Form(npchar) + "... �� ��� ��, ��� ��� ������ ������� ��� ��� �� ����. ��� �� �������?",
                "��, " + GetAddress_Form(npchar) + ", �� ���� ����������� �� ������, �� ���� ����� ������ ��������� �������... ��� ��� ������?" , "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�����, � ������ ����� ���������� � ����������.", 
				"����� �. � ����� ������ ��� ���������� �����.",
                "���, ����� �. � ����� ��� ����������� ���� ����� ����������.", 
				"��-�-�, ������ ���������� ����������� �����... � ��� �� ��� ������ �� �����, ������ ��������� �����...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			link.l2 = "� ���� �������� ���� ���������� ���������������.";
			link.l2.go = "LoanUsurer";//(�� ��������� ���������)
			link.l3 = "��� ����� ���-����� ����������... � �����, ���� ������.";
			link.l3.go = "quests";//(���������� � ���� ������)
			//������� ���, �������� �������� �� �������
			if (CheckAttribute(pchar, "questTemp.LSC.usurerId") && pchar.questTemp.LSC.usurerId == npchar.id)
			{
				if (pchar.questTemp.LSC == "SignOnPoorMurder")
				{
					link.l4 = "����������, ��� ������ ��� ��������� ������� �� ��� ���.";
					link.l4.go = "ISS_deposit";
				}
				if (pchar.questTemp.LSC == "PoorMurderExecute")
				{
					link.l4 = "��� ���������� ����� ������� �� ������� ��������, ��� ��������� � ���.";
					link.l4.go = "ISS_takeDeposit";
					DeleteAttribute(pchar, "questTemp.LSC.usurerId");
				}
			}
			//������ ����, ������������� �� ���������� �����
  			if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") //����� ���� - �� ����� � ����� �������
			{
				link.l7 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
					GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
				link.l7.go = "IntelligenceForAll";
			}
			//<<<<---------------- �������, ����� �5. --------------------
			if (CheckAttribute(pchar, "questTemp.piratesLine.Q5.city_1") && npchar.city == pchar.questTemp.piratesLine.Q5.city_1)
			{
				if (pchar.questTemp.piratesLine == "PL5Hunter_toFirstUsurer")
				{
					link.l5 = "�������, ��� �� ����� ������ ��������.";
					link.l5.go = "StepPL5First_1";
				}
				if (pchar.questTemp.piratesLine == "PL5Hunter_toFirst")
				{
					link.l5 = "����������, �� �������� �� �� ����� ������?";
					link.l5.go = "StepPL5First_1";
				}
			}
			if (CheckAttribute(pchar, "questTemp.piratesLine.Q5.city_2") && npchar.city == pchar.questTemp.piratesLine.Q5.city_2)
			{
				if (pchar.questTemp.piratesLine == "PL5Hunter_toSecondUsurer")
				{
					link.l5 = "������� � ��� ���� �������� � ������ ������...";
					link.l5.go = "StepPL5Second_0";
				}
				if (pchar.questTemp.piratesLine == "PL5Hunter_toSecond")
				{
					link.l5 = "����������, �� �������� �� �� ����� ������ ��� ���������� ���������?";
					link.l5.go = "StepPL5Second_1";
				}
			}
			if (CheckAttribute(pchar, "questTemp.piratesLine.Q5.city_3") && npchar.city == pchar.questTemp.piratesLine.Q5.city_3)
			{
				if (pchar.questTemp.piratesLine == "PPL5Hunter_toThirdUsurer" || pchar.questTemp.piratesLine == "PPL5Hunter_toThird")
				{
					link.l5 = "� ��� ����� ������, � ��� �� �� ���������?";
					link.l5.go = "StepPL5Third_1";
				}
			}
			if (CheckAttribute(pchar, "questTemp.piratesLine.Q5.city_4") && npchar.city == pchar.questTemp.piratesLine.Q5.city_4)
			{
				if (pchar.questTemp.piratesLine == "PL5Hunter_toFourth")
				{
					link.l5 = "� ������ � ��� ��������� ���������, �������� ��� ����������� ������?";
					link.l5.go = "StepPL5Four_1";
				}
			}
			//-------------------- �������, ����� �5. ---------------->>>>
			//-->> ����� ������ ����� ���������� ����������� ������
			if (CheckCharacterItem(pchar, "UsurersJew") && CheckAttribute(npchar, "quest.usurersJewel.type"))
			{
				link.l6 = "� ����� ��� " + npchar.quest.usurersJewel.type + ". ��� ��.";
				link.l6.go = "usurersJewel_R1";
			}
			//<<-- ����� ������ ����� ���������� ����������� ������
			NextDiag.TempNode = "First time";
		break;

		//<<<<---------------- �������, ����� �5. --------------------
		case "StepPL5First_1":
			dialog.text = "�������, � �� ��������� ��������� � ���� ����������. ������, ��-��, �������, � ������� �� ��� �������, ��� ��������� �������� ��������� � ���� ���������.";
			link.l1 = "� �� �� ������ ���� ����������? �����, ��� ������ ���������� ������ �����?";
			link.l1.go = "StepPL5First_2";
		break;
		case "StepPL5First_2":
			dialog.text = "��� ����� �� ����, ����� �������� ���� �� ��������.";
			link.l1 = "��� �������, ��� �� ��������. �� ����� �������� ���-�� ����?..";
			link.l1.go = "StepPL5First_3";
		break;
		case "StepPL5First_3":
			pchar.questTemp.piratesLine.Q5.city_2 = GetQuestNationsCity(SPAIN); 
			dialog.text = "��, ����� ��, ��������� �� ����� ����� ���� �������, � ��������. � ���� ����� - �� �����... ���� ���������, ���-�� �� ������� � " + XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_2 + "Dat") + ". �� ���� ���������, ��� �� ������������� �� �������.";
			link.l1 = "�����. �������.";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_5_Hunter", "4");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity1", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_1 + "Gen"));
			AddQuestUserData("Pir_Line_5_Hunter", "sCity2", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_2 + "Acc"));
			AddQuestUserData("Pir_Line_5_Hunter", "sIsland", XI_ConvertString(GetIslandByCityName(pchar.questTemp.piratesLine.Q5.city_2) + "Dat"));
			pchar.questTemp.piratesLine = "PL5Hunter_toSecond";
		break;

		case "StepPL5Second_0":
			dialog.text = "�-�-�, �� �� ���� ��������...";
			link.l1 = "��, ��� �� �������� ���?";
			link.l1.go = "StepPL5Second_1";
		break;
		case "StepPL5Second_1":
			dialog.text = "�������. ������ �� ��������, ���� ������ ������. ������ � �������, ���� ������� ������, ���, �����, ����� � �������� �����, ����������� ����������� ����� �����, � ��� ������ ����, ������� �� ����.\n"+
				"����� ���, ���������� �� �������, ��� ����� �����, � ������ ���� �������, ���� ������ �������?! ������� ������, �������, ���� �� � ������, ��� ������� �������� ����� ������...\n"+
				"� �����, � �������� � �����������, ����� ���� ������ ����-�� ����� �� ������ ������. � ������, ������� � ��� �� �����!";
			link.l1 = "����. � ���� �� ����������, ��, �������, �� � �����...";
			link.l1.go = "StepPL5Second_2";
		break;
		case "StepPL5Second_2":
			pchar.questTemp.piratesLine.Q5.city_3 = GetQuestNationsCity(FRANCE);
			dialog.text = "�����, � ��������� ������� ���� ��������. �������� ����� ��� �������� ��� �������� � �������. ���� � ������� �� �����, �� �� ������� ���������� ����� ���������...";
			link.l1 = "���������, ��� ��� �����, ��� �� ��� ������. ������� ��� �� ���������� �� ������.";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_5_Hunter", "6");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_3));
			AddQuestUserData("Pir_Line_5_Hunter", "sIsland", XI_ConvertString(GetIslandByCityName(pchar.questTemp.piratesLine.Q5.city_3) + "Dat"));
			pchar.questTemp.piratesLine = "PL5Hunter_toThird";
		break;

		case "StepPL5Third_1":
			dialog.text = "���� ������������ ��������� ��� ������, ��� ������ ������� �������� ��� ����...";
			link.l1 = "� ��� �� ����� ������ ����������?";
			link.l1.go = "StepPL5Third_2";
		break;
		case "StepPL5Third_2":
			dialog.text = "��� ������ ���� ������ ��������. ���������, ��� ����������� ���� �����, �� ��, ����� �� �����������. ������ � �������, ������ ������� ���� �������, �������� �������. ������� �, ������ ��������, ����� �� �����...";
			link.l1 = "�������... ��, ����� ����, ���� ���� �����-�� �����������, ���� �� ��� ����������?";
			link.l1.go = "StepPL5Third_3";
		break;
		case "StepPL5Third_3":
			pchar.questTemp.piratesLine.Q5.city_4 = GetQuestNationsCity(Holland);
			dialog.text = "���� ��� ���������� ��� ������, �� �����, ��� �� ���������� � " + XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_4) + ".";
			link.l1 = "��... � ������� ������� ������������� � ���� ��������?";
			link.l1.go = "StepPL5Third_4";
		break;
		case "StepPL5Third_4":
			dialog.text = "�������. ������ ��, ���� � ���, ��� � " + XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_4 + "Dat") + " ������� ��� �������� ���������.";
			link.l1 = "�-�-�, � ��� �����! ������� ��������.";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_5_Hunter", "8");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity1", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_3 + "Gen"));
			AddQuestUserData("Pir_Line_5_Hunter", "sCity2", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_4 + "Gen"));
			AddQuestUserData("Pir_Line_5_Hunter", "sIsland", XI_ConvertString(GetIslandByCityName(pchar.questTemp.piratesLine.Q5.city_4) + "Dat"));
			pchar.questTemp.piratesLine = "PL5Hunter_toFourth";
		break;

		case "StepPL5Four_1":
			dialog.text = "���, ���, �������. ������ ��� �� �����, � ������ ����� ����� ����� ������, �� ������ � ���� ����! �, �� ����������, �� ���� ���� �� ������, ������ ������ ����� �� ��� ������ ���... �-�-�, ��������, ��� �� �������, ��� �����?";
			link.l1 = GetFullName(pchar) + ".";
			link.l1.go = "StepPL5Four_2";
		break;
		case "StepPL5Four_2":
			dialog.text = "��, ��, � � ��� ������... �������... ��� � ��� � �����������? � ��, �������������, ��������� ������ ��������, ����������� ��� � �����, � �������� ���� �������! ����!!! ���������� ��������, ������� ���� ���...";
			link.l1 = "��������, ��� ���������, �� �� ���, ������, � ���?";
			link.l1.go = "StepPL5Four_3";
		break;
		case "StepPL5Four_3":
			dialog.text = "...������� ����� �������. ��� � ���?! � ���������, �������! ��, ��� ������! ��, ������, ���� ������ ��� �������! ������ ��� ����! ����!.. �� ���� ��������, ����������, ��� ���, ���� � ���� �������, ��� �� �������� �������...";
			link.l1 = "���? ������?!";
			link.l1.go = "StepPL5Four_4";
		break;
		case "StepPL5Four_4":
			dialog.text = "��, ������ ����� ������, ���������, �������! ��������, ��� �� ���� ��� ����� �����������?!";
			link.l1 = "��������. � ���� �, ������� ��� ������������? ��� ��� �� ������ �������?";
			link.l1.go = "StepPL5Four_5";
		break;
		case "StepPL5Four_5":
			dialog.text = "�-�-�... ��... ���, � ��������, ��� �����, ��� ������ ������, �� � ������ �� ������������ ����. ����, ���� � ��� ����, �� ��������.";
			link.l1 = "������� �������. ���� � ������ ����, ��� �� ��������. ��� � ����� ����� ���� ����?";
			link.l1.go = "StepPL5Four_6";
		break;
		case "StepPL5Four_6":
			dialog.text = "��������� �����! ���� �� ��� ��� ��� ���������, ���� ������� �������� ���� �����.";
			link.l1 = "��� ����� ���������?";
			link.l1.go = "StepPL5Four_7";
		break;
		case "StepPL5Four_7":
			dialog.text = "�� �������, �� ��� ������������?! ��� �� ������ ����� �������� ����������? ����� ������, ����, �������� �������� �� �������.";
			link.l1 = "������. �� �����! ����� ����, � �����!";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_5_Hunter", "9");
			pchar.questTemp.piratesLine = "PL5Hunter_toBermudes";
			npchar.PLQ5Money = true; //����� �� ����������, �������� ������ �����
			//����� ������ ���������� �� ���� � ��� ���� �� ��������
            Pchar.quest.PQ5_inOrryHouse.win_condition.l1 = "location";
            Pchar.quest.PQ5_inOrryHouse.win_condition.l1.location = "Pirates_houseS1";
            Pchar.quest.PQ5_inOrryHouse.function = "PQ5_inOrryHouse";
		break;

		case "StepPL5End_close":
			dialog.text = "�����, � ��� ����� �����������. ��� �������, ��� �� ���-�� ���������������... �� �� �����.";
			link.l1 = "��� ������! ��� ���������, ��������.";
			link.l1.go = "exit";
			DeleteAttribute(npchar, "PLQ5Money");
    		ChangeCharacterReputation(pchar, -50);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), -20);
		break;

		case "StepPL5End_1":
			dialog.text = "���� ������, � ���! �������, � ��� ��������� � ��� ���������, ������� ������� ����?";
			link.l1 = "�� ������� �� ��������, ������ �� ��� �� �����������...";
			link.l1.go = "StepPL5End_2";
		break;
		case "StepPL5End_2":
			dialog.text = "�����������, �� ��� ��, � ����� ��������� ��� ������. ������� ������...";
			link.l1 = "�������� ��������.";
			link.l1.go = "StepPL5End_3";
		break;
		case "StepPL5End_3":
			dialog.text = "�������! ������ �����������! ��� � �������. ��� ���� 5 �����, ��� � ��������������.";
			link.l1 = "�������...";
			link.l1.go = "StepPL5End_4";
			DeleteAttribute(npchar, "PLQ5Money");
			AddMoneyToCharacter(pchar, -45000);
    		ChangeCharacterReputation(pchar, 10);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 5);
		break;
		case "StepPL5End_4":
			dialog.text = "�� ��� ��, � ������ ��� ��� ������ � ����. � ���� ��� �������, ��� ���� �������� � ����� ���������������� � ���������. ������ ������ ��� �� ����, �� ���������� ������...";
			link.l1 = "����� � ������ ��������... �� �����, ��������.";
			link.l1.go = "exit";
		break;
		//-------------------- �������, ����� �5. ---------------->>>>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		��������� ��������� Loan
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "LoanUsurer": 
			dialog.text = "������� �������. ����������� ��� ������.";
			link.l1 = "���� �� � ��� ����, ��������� ��������� ������?";
			link.l1.go = "Loan_GiveWork";
			//����� ��������
			if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && pchar.GenQuest.Loan.FindCitizenNpcharId == NPChar.id)
            {
                link.l2 = "� �� ������ ������� � ��������.";
                link.l2.go = "FindCitizen_1";
            }
			//��������� ������
            if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && pchar.GenQuest.LoanChest.ChestNpcharId == NPChar.id)
            {
                link.l3 = "� �� ������ ������� � �������� �������.";
                link.l3.go = "TakeChest_1";
            }
            if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
            {
                link.l4 = "� ������ ��� ������.";
                link.l4.go = "TakeChest_2";
            }
			//�������
            if(CheckAttribute(Pchar, "Quest.Loans." + (NPC_Area)) && makeint(Pchar.Quest.Loans.(NPC_Area)) == true)
			{
				iPastMonths = GetPastTime("Month", makeint(Pchar.Quest.Loans.(NPC_Area).StartYear),makeint(Pchar.Quest.Loans.(NPC_Area).StartMonth),makeint(Pchar.Quest.Loans.(NPC_Area).StartDay), makefloat(Pchar.Quest.Loans.(NPC_Area).StartTime), getDataYear(),getDataMonth(),GetDataDay(), GetTime());
				Pchar.Quest.Loans.(NPC_Area).Result = makeint(Pchar.Quest.Loans.(NPC_Area).Sum) + ((makeint(Pchar.Quest.Loans.(NPC_Area).Sum)/100)*makeint(Pchar.Quest.Loans.(NPC_Area).Interest))*(iPastMonths+1);// boal 23.01.2004
				if(makeint(PChar.money) >= makeint(Pchar.Quest.Loans.(NPC_Area).Result))
				{
					Link.l5 = "� ������ ��� "+ FindRussianMoneyString(sti(Pchar.Quest.Loans.(NPC_Area).Result)) + LinkRandPhrase(". � ����� ������� ����.", ". � ������ ������ � � ��������� ��������� ��� ����.", ". ������� �����, �������, ������������.");
					Link.l5.go = "loan_return";	
					sTemp = "credit";
				}					
			}
			else
			{
				Link.l6 = LinkRandPhrase("� ���� ������ �����.", "��� ������ ������������ ����-������ ��������.", "��� ������ ���������� �������?");
				Link.l6.go = "loan";	
				if(makeint(Pchar.money) >= 100)
				{
					Link.l7 = LinkRandPhrase("� �� ����� ������ ������ � ����.","������� �� �������� ��������� ������?", "��������� �� ������ ���� �������� �����?");
					Link.l7.go = "deposit";				
				}
			}
			if(CheckAttribute(Pchar, "Quest.Deposits." + (NPC_Area)) && makeint(Pchar.Quest.Deposits.(NPC_Area)) == true)
			{
				iPastMonths = GetPastTime("Month", makeint(Pchar.Quest.Deposits.(NPC_Area).StartYear),makeint(Pchar.Quest.Deposits.(NPC_Area).StartMonth),makeint(Pchar.Quest.Deposits.(NPC_Area).StartDay), makefloat(Pchar.Quest.Deposits.(NPC_Area).StartTime), getDataYear(),getDataMonth(),GetDataDay(), GetTime());
				Pchar.Quest.Deposits.(NPC_Area).Result = makeint(Pchar.Quest.Deposits.(NPC_Area).Sum) + ((makeint(Pchar.Quest.Deposits.(NPC_Area).Sum)/100)*makeint(Pchar.Quest.Deposits.(NPC_Area).Interest))*iPastMonths;
				Link.l8 = LinkRandPhrase("� ���� ������� ���� �����.", "������ ����� ������� ��� ������, ��� � ��� ��������.", "��� ����� ��� ������ � ���������� ��������.");
				Link.l8.go = "Deposit_return";									
			}
			//�����
			if (sTemp == "credit")
			{
				link.l9 = LinkRandPhrase("� �����, ��� ���� � ��� ������, �� ����������� ���� �� �����...", "� ����, ��� ������ ���, �� ������ � ���� ��� �����. ������� �����...", "� �����, ��� ������ ���, �� � ������ ������ �� ���� ����������� ������� ��� ��������� �������. � ������ ��� �����.");
				link.l9.go = "Loan_Remind";
			}
			else
			{
				link.l9 = "� ������, ������� ���������� ��� � ���� � ���� ���. ��������.";
				link.l9.go = "exit";
			}
		break;
		//=============================  ���� ��� �� ���� ������ - ������� � ��������
		case "Loan_GiveWork": 
            // �������� �� ���� ����������� -->
            if (isBadReputation(pchar, 42))
            {
                dialog.text = RandSwear() + "�� ���� " + GetFullName(pchar)+ ", �� ��� ��? � ���� " +
                              RandPhraseSimple("����� ������� �����","������� �� ������ �������") + ", � �� ���� ����� � ����� ����.";
				link.l1 = RandPhraseSimple("��... ����� �������� ����������, � ��� ����� � ���� �������.", RandSwear() + "�� � �� ����!");
				link.l1.go = "exit";
                break;
            }
            // �������� �� ���� ����������� <--
            dialog.text = "���, � �������, � ���� ��� ������.";

			if (npchar.quest.trade_date != lastspeak_date || bBettaTestMode)
			{
                npchar.quest.trade_date = lastspeak_date;
                iNum = rand(1);
                if (sti(Pchar.Ship.Type) == SHIP_NOTUSED)
                {
                    iNum = 0; // ������ ������
                }
                switch (iNum)
                {
                    case 0 : // ����� ��������
                        if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen")) break;
                        
                        if (sti(Pchar.Ship.Type) == SHIP_NOTUSED && sti(npchar.quest.FindCitizenNoShip) < 2)
                        {
                            iNum = findCitizenMan(Npchar, true);
                            npchar.quest.FindCitizenNoShip = sti(npchar.quest.FindCitizenNoShip) + 1; // �������� �� ���� ��� �������
                        }
                        else
                        {
                            iNum = findCitizenMan(Npchar, false);
                        }
                        if (iNum > 0)
                        {
                            chr = &Characters[iNum];
                            pchar.GenQuest.Loan.FindCitizenMoney   = (1 + rand(sti(PChar.rank)))*1500 + rand(10)*100;
                            pchar.GenQuest.Loan.FindCitizenPercent = makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) / (3+rand(2)));
                            pchar.GenQuest.Loan.FindCitizenIdx  =  iNum;
							sTemp = "";
							if (npchar.city != chr.city)
							{
								sTemp = ", ��� �� " + XI_ConvertString(GetIslandByCityName(chr.city)+"Dat");
							}
                            dialog.text = "��, �� �������. ���� � ���� ���������� ������, ��������� �������. "+
                                     GetFullName(chr) + ", ������� � " + XI_ConvertString("Colony"+chr.city+"Dat") + sTemp + ", �������� ��� " + FindRussianMoneyString(sti(pchar.GenQuest.Loan.FindCitizenMoney)) +
                                     ". ���� �� ������� ��� ��� �����, �� ���������� �������� ������ �������� ����. ���, ������, ���������� " + FindRussianMoneyString(sti(pchar.GenQuest.Loan.FindCitizenPercent)) + ".";
                            link.l1 = "� ������!";
                			link.l1.go = "LoanUsurer_GiveWork_1";
            			}
                    break;

                    case 1: // ��������� �������
                        if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest")) break;
                        
                        iNum = findChestMan(Npchar);
                        if (iNum > 0)
                        {
                            chr = &Characters[iNum];
                            pchar.GenQuest.LoanChest.Chest      = 2 + rand(2);
                            pchar.GenQuest.LoanChest.Money      = (1 + rand(sti(pchar.GenQuest.LoanChest.Chest)))*1500 + rand(10)*100;
                            pchar.GenQuest.LoanChest.TargetIdx  =  iNum;
                            pchar.GenQuest.LoanChest.Time       = 20 + rand(15);
							sTemp = "";
							if (npchar.city != chr.city)
							{
								sTemp = ", ��� �� " + XI_ConvertString(GetIslandByCityName(chr.city)+"Dat");
							}
                            dialog.text = "��� ����� ����������� ������� ��� ������ ������. ����� ��������� ������ - " + pchar.GenQuest.LoanChest.Chest + 
									" ������� � ������� � "+ XI_ConvertString("Colony"+chr.city+"Acc") + sTemp + " ��� �������� �� ����� " +
									GetFullName(chr) + ". �� ������� " + GetWorkTypeOfMan(chr, "") +
									". �� ������ �������� " + FindRussianMoneyString(sti(pchar.GenQuest.LoanChest.Money)) + ". ��� ����� ������� �� " + FindRussianDaysString(sti(pchar.GenQuest.LoanChest.Time)) + ".";
                            link.l1 = "� �����!";
                			link.l1.go = "LoanUsurer_ChestWork_1";
            			}
                     break;
    			}
    		}
            else
            {
                dialog.text = "� ��� ��� ��� ������.";
            }
            link.l9 = "��������, ��� ����.";
		    link.l9.go = "exit";
		break;
		//============== boal ���������� �������� ===============
		case "LoanUsurer_GiveWork_1":
            pchar.GenQuest.Loan.FindCitizen = true;
            pchar.GenQuest.Loan.FindCitizenNpcharId = Npchar.id;
            dialog.text = "������������. ��� ��� ��� ����� ������.";
			link.l9 = "�� �������!";
			link.l9.go = "exit";			
            chr = &Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)];     
			sTemp = "";
			if (npchar.city != chr.city)
			{
				sTemp = ", ��� �� " + XI_ConvertString(GetIslandByCityName(chr.city)+"Dat");
			}
			ReOpenQuestHeader("Gen_LoanFindCitizen");
            AddQuestRecord("Gen_LoanFindCitizen", "1");
            AddQuestUserData("Gen_LoanFindCitizen", "sName", GetFullName(chr));
            AddQuestUserData("Gen_LoanFindCitizen", "sCity", XI_ConvertString("Colony"+chr.city+"Dat") + sTemp);
			AddQuestUserData("Gen_LoanFindCitizen", "sFromCity", XI_ConvertString("Colony"+Npchar.city+"Gen"));
			AddQuestUserData("Gen_LoanFindCitizen", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.Loan.FindCitizenMoney)));
			AddQuestUserData("Gen_LoanFindCitizen", "sMyMoney", FindRussianMoneyString(sti(pchar.GenQuest.Loan.FindCitizenPercent)));
		break;

		case "FindCitizen_1": //�������� ���������� ������
            dialog.text = "� ���� ��������.";
            if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone"))
            {
                link.l1 = "���� ��������� ���������!";
                link.l1.go = "finish_work";
                link.l2 = "��� ������� ����� ��������, �� �� ������� ������ �� ���� �����.";
                if (GetCharacterSkillToOld(PChar, SKILL_FORTUNE) >= rand(11))
                {
                    link.l2.go = "finish_work_bad";
                }
                else
                {
                    link.l2.go = "finish_work_lie";
                }
            }

            if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
            {
                link.l1 = "��� ������� ����� ��������, �� �� ������� ������ �� ���� �����.";
                link.l1.go = "finish_work_bad";
            }
            link.l3 = "� ����� ������������ ��� ����� �� ������ �������� - ��� �� ��� ����.";
            link.l3.go = "finish_work_cancel";
            link.l9 = "���. ������.";
			link.l9.go = "exit";
		break; 

		case "finish_work":
            if (sti(pchar.Money) >= sti(pchar.GenQuest.Loan.FindCitizenMoney))
            {
                dialog.text = "��� � ������!";
    			link.l9 = "����������!";
    			link.l9.go = "exit";
    			AddMoneyToCharacter(Pchar, -1*sti(pchar.GenQuest.Loan.FindCitizenMoney));
    			DeleteAttribute(pchar, "GenQuest.Loan");
    			DeleteAttribute(Pchar, "quest.LoanFindCitizenTimer");
    			AddCharacterExpToSkill(Pchar, "Commerce", makeint(Pchar.rank) * 3);
    			AddCharacterExpToSkill(Pchar, "Leadership", makeint(Pchar.rank) * 6);
    			ChangeCharacterReputation(pchar, 1);
    			OfficersReaction("good");    			
    			AddQuestRecord("Gen_LoanFindCitizen", "4");
                CloseQuestHeader("Gen_LoanFindCitizen");
			}
			else
			{
			    dialog.text = "������������! �� ��� ��� ������? �����-�� ����.";
    			link.l9 = "����� �����!";
    			link.l9.go = "exit";
    		}
		break;
		
		case "finish_work_bad":
            dialog.text = "����� � ����� �����! �� �� � ��������� ������ ���� ������!";
			link.l9 = "���� ������� ������ ��� �� �����������, �� �� ����� - ��� ��� ������.";
			link.l9.go = "exit";
			DeleteAttribute(pchar, "GenQuest.Loan");
			DeleteAttribute(Pchar, "quest.LoanFindCitizenTimer");
			ChangeCharacterReputation(pchar, -1);
			AddQuestRecord("Gen_LoanFindCitizen", "5");
            CloseQuestHeader("Gen_LoanFindCitizen");
		break;
		
		case "finish_work_cancel":
            if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") || CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
            {
                dialog.text = "� ��� ������� �� ���� �����������! � ����� ����, ��� �� � ��� ��������. ������� ��� ������!";
                link.l1 = "�� �� �����, ��� ������.";
    			link.l1.go = "finish_work";
    			link.l2 = "�� �� ��������� �������!";
    			if (GetCharacterSkillToOld(PChar, SKILL_FORTUNE) >= rand(11))
                {
    			   link.l2.go = "finish_work_bad";
    			}
    			else
    			{
    			   link.l2.go = "finish_work_bad_2";
    			}
    			link.l9 = "��� ���� �������. � ����� �����.";
    			link.l9.go = "exit";
    			ChangeCharacterReputation(pchar, -1);
			}
            else
            {
                ChangeCharacterReputation(pchar, -1);
                dialog.text = "�����, � ��� ����������� �� ���.";
                link.l1 = "�� �� �����, �� ������ �����.";
    			link.l1.go = "exit";
    			DeleteAttribute(pchar, "GenQuest.Loan");
    			DeleteAttribute(Pchar, "quest.LoanFindCitizenTimer");
    			AddQuestRecord("Gen_LoanFindCitizen", "6");
                CloseQuestHeader("Gen_LoanFindCitizen");
            }
		break;
		
		case "finish_work_bad_2":
            dialog.text = "�� ��� ��������, ��� ������� �� ���. ������� ��� ������!";
            link.l1 = "�� �� �����, ��� ������.";
    		link.l1.go = "finish_work";
		break;
		
		case "finish_work_lie":
            dialog.text = "��, �����?";
            link.l1 = "�������!";
    		link.l1.go = "finish_work_cancel";
		break;

		//============== boal �������� �������� ===============
		case "LoanUsurer_ChestWork_1": 
            pchar.GenQuest.LoanChest.TakeChest = true;
            pchar.GenQuest.LoanChest.ChestNpcharId = Npchar.id;
            dialog.text = "� �� ��� ����� �����������.";
			link.l9 = "�� �������!";
			link.l9.go = "exit";
			TakeNItems(pchar, "Chest", sti(pchar.GenQuest.LoanChest.Chest));
			SetTimerCondition("LoanChestTimer", 0, 0, sti(pchar.GenQuest.LoanChest.Time), false);
			// ������� ������� �����
            TraderHunterOnMap();
            chr  = &Characters[sti(pchar.GenQuest.LoanChest.TargetIdx)];
			sTemp = "";
			if (npchar.city != chr.city)
			{
				sTemp = ", ��� �� " + XI_ConvertString(GetIslandByCityName(chr.city)+"Dat");
			}
			ReOpenQuestHeader("Gen_LoanTakeChest");
            AddQuestRecord("Gen_LoanTakeChest", "1");
            AddQuestUserData("Gen_LoanTakeChest", "sFromCity", XI_ConvertString("Colony"+Npchar.city+"Gen"));
			AddQuestUserData("Gen_LoanTakeChest", "sQty", pchar.GenQuest.LoanChest.Chest);
			AddQuestUserData("Gen_LoanTakeChest", "sCity", XI_ConvertString("Colony"+chr.city+"Acc") + sTemp);
			AddQuestUserData("Gen_LoanTakeChest", "sName", GetFullName(chr));
			AddQuestUserData("Gen_LoanTakeChest", "sType", GetWorkTypeOfMan(chr, ""));
			AddQuestUserData("Gen_LoanTakeChest", "sMoney", FindRussianMoneyString(sti(pchar.GenQuest.LoanChest.Money)));
			AddQuestUserData("Gen_LoanTakeChest", "sDay", FindRussianDaysString(sti(pchar.GenQuest.LoanChest.Time)));
		break;

		case "TakeChest_1":
            dialog.text = "� ���� ��������.";
            link.l1 = "� ����� ������������ ��� ����� �� �������� ������� - ��� �� ��� ����.";
            link.l1.go = "TakeChestFinish_work_cancel";
            link.l9 = "���. ������.";
			link.l9.go = "exit";
		break;

		case "TakeChestFinish_work_cancel":
            if (GetCharacterItem(pchar, "Chest") >= sti(pchar.GenQuest.LoanChest.Chest))
            {
                ChangeCharacterReputation(pchar, -1);
                dialog.text = "�����, � ��� ����������� �� ���.";
                link.l1 = "�� �� �����, �� ������ �����.";
    			link.l1.go = "exit";
    			TakeNItems(pchar, "Chest", -sti(pchar.GenQuest.LoanChest.Chest));
    			DeleteAttribute(pchar, "GenQuest.LoanChest");
    			DeleteAttribute(Pchar, "quest.LoanChestTimer");
                AddQuestRecord("Gen_LoanTakeChest", "5");
                CloseQuestHeader("Gen_LoanTakeChest");
			}
			else
			{
                dialog.text = "��������, � ����� �������� ���������� ����� ������, ���� �� ������� ��� ��� �������.";
                link.l1 = "����� �����.";
    			link.l1.go = "Exit";
			}
		break;
		//============== ������� ===============
		case "loan":
            if (sti(NPChar.nation) == PIRATE)
            {
                dialog.text = "���, ����� �� ���� ������. � ����� ����������� ����� � ���� ���������� ������ ������ �������� �����.";
                if(makeint(Pchar.money) >= 100)
				{
	                Link.l1 = "������, ������� ����������� � ������.";
	                Link.l1.go = "deposit";
				}
                Link.l2 = "��� ��� �� ���������.";
				link.l2.go = "exit";
                break;
            }
			// �������� �� ���� ����������� -->
            if (isBadReputation(pchar, 42))
            {
                dialog.text = RandSwear() + "�� ���� " + GetFullName(pchar)+ ", �� ��� ��? � ���� " +
                              RandPhraseSimple("����� ������� �����","������� �� ������ �������") + ", � �� ���� ����� � ����� ����.";
				link.l1 = RandPhraseSimple("��.. ����� �������� ���������� � ��� ����� � ���� �������.", RandSwear() + "�� � �� ����!");
				link.l1.go = "exit";
                break;
            }
            // �������� �� ���� ����������� <--
			Dialog.snd = "voice\USDI\USDI016";
			dialog.text = "�, ������ ����������. ����� ����� ��� ����������?";
			Link.l1 = "������ ���������.";
			Link.l1.go = "Small";
			Link.l2 = "������ ������������.";
			Link.l2.go = "Medium";
			Link.l3 = "��� ������, ��� �����.";
			Link.l3.go = "Large";
		break;

		case "small":
			Pchar.Quest.Loans.(NPC_Area).Sum = 500*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI017";
			dialog.text = "�������. � ���������� ������� ������ ����� - ������ �����. � ���� ���������� ��� " + FindRussianMoneyString(sti(Pchar.Quest.loans.(NPC_Area).sum)) + "...";
			Link.l1 = "������������. ��� ����� ��������?";
			Link.l1.go = "Interest";
			Link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			Link.l2.go = "Loan";
			Link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			Link.l3.go = "ExitDelLoan1";
			Pchar.Quest.Loans.(NPC_Area).Interest = 11 - makeint(GetCharacterSkillToOld(Pchar, "commerce"));
		break;

		case "Medium":
			Pchar.Quest.Loans.(NPC_Area).Sum = 1500*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI018";
			dialog.text = "�� ��������. �������, ��� " + FindRussianMoneyString(sti(Pchar.Quest.Loans.(NPC_Area).sum)) + " ������� ��� ������ ���� ��������. ��� ������ ������������ �����.";
			Link.l1 = "������������. ��� ����� ��������?";
			Link.l1.go = "Interest";
			Link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			Link.l2.go = "Loan";
			Link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			Link.l3.go = "ExitDelLoan1";
			Pchar.Quest.Loans.(NPC_Area).Interest = 16 - makeint(GetCharacterSkillToOld(Pchar, "commerce"));
		break;

		case "Large":
			Pchar.Quest.Loans.(NPC_Area).Sum = 4000*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI019";
			dialog.text = "�-��... ����������... �� �����, � ����� ������������ ��� ���� �� ����� � " + FindRussianMoneyString(sti(Pchar.Quest.Loans.(NPC_Area).sum)) + ". ��� �� ���������, �������, ��� ������������� ������� ������. ����� ����� ��� �������� ��������� � ����� ����.";
			Link.l1 = "������������. ��� ����� ��������?";
			Link.l1.go = "Interest";
			Link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			Link.l2.go = "Loan";
			Link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			Link.l3.go = "ExitDelLoan1";
			Pchar.Quest.Loans.(NPC_Area).Interest = 26 - makeint(GetCharacterSkillToOld(Pchar, "commerce"));
		break;

		case "Interest":
			//Pchar.Quest.Loans.(NPC_Area).Interest = 16 - makeint(Pchar.skill.commerce);
			Dialog.snd = "voice\USDI\USDI020";
			dialog.text = Pchar.Quest.Loans.(NPC_Area).Interest + "% � �����. ����������� �� ���, ��� � � ��� ����, � �� ���� ���������� ��� ������ ������ �������."; 
			Link.l1 = "���� ��� ������ ����������. �������� ���������� ����.";
			Link.l1.go = "Period";
			Link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			Link.l3.go = "ExitDelLoan1";
		break;

		case "Period":
			Pchar.Quest.Loans.(NPC_Area).Period = makeint(makeint(Pchar.reputation)/20) + 1;			
			Dialog.snd = "voice\USDI\USDI021";
			dialog.text =  "�������� ��, ��� � � ��� ������, " + Pchar.name + ", � ���� ����� �������� ����� ������� " + FindRussianMonthString(sti(Pchar.Quest.Loans.(NPC_Area).Period)) + ". ����� ����, ��� �� ���������, � ����� ����."; 
			Link.l1 = "��� ��, � � ������������� �������� ���� �������... � ���� ������.";
			Link.l1.go = "LoanGranted";
			Link.l3 = "���, �� � ���� �� ������������. �� ��������.";
			Link.l3.go = "ExitDelLoan1";
		break;

		case "LoanGranted":
			Dialog.snd = "voice\USDI\USDI022";
			dialog.text = "�������� ����� ���. �� ��������� ��� ��� ������������. � ����� ��������� ����� ����� � ����, ��� ������� ��������� ��������. ��� ��� ���� � ��� � ���� ����� ����� ������������ ��� ������, ����� �������� ��\n�� ���� ��� ������� - ������ ������������.";
			Link.l1 = "��... ��-��. ����� ��������.";
			Link.l1.go = "LoanGranted_exit";
		break;

		case "Loan_Remind":
			Dialog.snd = "voice\USDI\USDI023";
			dialog.text = "�� ��������... �������� ������. �� � ������� � ��� �������� �� ��� �� �����.";
			Link.l1 = "�� ����������. �� �������.";
			Link.l1.go = "exit";
		break;

		case "loan_return":
			addMoneyToCharacter(Pchar, -(makeint(Pchar.Quest.Loans.(NPC_Area).Result)));
			// boal 27.01.2004 -->
            if (npchar.quest.trade_date != lastspeak_date) // ���� � ��������� ����
            {
    			AddCharacterExpToSkill(Pchar, "Commerce", makeint(Pchar.Quest.Loans.(NPC_Area).Result) / 350);
    			AddCharacterExpToSkill(Pchar, "Leadership", makeint(Pchar.rank) * 5);
			}
			// boal 27.01.2004 <--
			DeleteAttribute(PChar, "quest.Loans_" + NPC_Area); // bug fix
			DeleteAttribute(PChar, "quest.Loans." + NPC_Area);
			Dialog.snd1 = "voice\USDI\USDI024";
			Dialog.snd2 = "voice\USDI\USDI025";
			Dialog.snd3 = "voice\USDI\USDI026";
			dialog.text = LinkRandPhrase("�, ��� ������ �������! � ����� �����, ��� ��� ����������� ������ - � � ����� �������.", "���������. ����� ������� ����� ���� � ������� ���������, ������� ������� ������� ����. � �� ������, ������ ������...",  "�, �� ��� � ��� � ��� ����� ������, " + GetAddress_Form(NPChar) + "! �������, ��� �� ��������� ���, ����� �� ���������� � ���� �������!");
			Link.l1 = LinkRandPhrase("� ���� ������ �����.", "��� ������ ������������ ����-������ ��������.", "��� ������ ���������� �������?");
			Link.l1.go = "loan";
			if(makeint(PChar.money) >= 100)
			{
				Link.l2 = LinkRandPhrase("� �� ����� ������ ������ � ����.","������� �� �������� ��������� ������?", "��������� �� ������ ���� �������� �����?");
				Link.l2.go = "deposit";				
			}
			Link.l3 = "�� ��������, " + NPchar.name + ".";
			Link.l3.go = "ExitDelLoan1";
		break;

		case "deposit":
			Dialog.snd1 = "voice\USDI\USDI027";
			Dialog.snd2 = "voice\USDI\USDI028";
			Dialog.snd3 = "voice\USDI\USDI029";
			dialog.text = LinkRandPhrase("� ����� ��������� ��������! ��� ������� ����� �� ������ ������ � ����?", "������. �������� ���, �� �����, ��� �� ��� �������, ����� ����� ��� � ������� � �����������, � ���� � ����������...�, ������, ������ ��� �����?", "� ������, �� ���������, ��� � ���� � ���� �����. ������� �� ������ ������ ��� �� ��������?");
			Link.l1 = "�������� - " + FindRussianMoneyString(makeint(makeint(PChar.money)/40)*10) + ".";
			Link.l1.go = "quarter";
			Link.l2 = "�������� - " + FindRussianMoneyString(makeint(makeint(PChar.money)/20)*10) + ".";
			Link.l2.go = "half";
			Link.l3 = "���, ��� � ���� ���� - " + FindRussianMoneyString(makeint(makeint(PChar.money)/10)*10) + ".";
			Link.l3.go = "All";
		break;

		case "quarter":
			Pchar.QuestTemp.Deposits.(NPC_Area).Interest = makeint(GetCharacterSkillToOld(Pchar, "commerce")/2.0 + 0.33) + 1;
			Pchar.QuestTemp.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/40)*10;
			Dialog.snd = "voice\USDI\USDI030";
			dialog.text = "������. ��������� ��� ��� ����� � ����� ��� ����������...��...������, " + Pchar.QuestTemp.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			Link.l1 = "���� ��� ����������.";
			Link.l1.go = "Deposit_placed";
			Link.l2 = "����� � ������ �����.";
			Link.l2.go = "Deposit";
			Link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			Link.l3.go = "Exit";
		break;

		case "half":
			Pchar.QuestTemp.Deposits.(NPC_Area).Interest = makeint(GetCharacterSkillToOld(Pchar, "commerce")/2.0 + 0.33) + 1;
			Pchar.QuestTemp.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/20)*10;
			Dialog.snd = "voice\USDI\USDI031";
			dialog.text = "������. ��������� ��� ��� ����� � ����� ��� ����������...��...������, " + Pchar.QuestTemp.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			Link.l1 = "���� ��� ����������.";
			Link.l1.go = "Deposit_placed";
			Link.l2 = "����� � ������ �����.";
			Link.l2.go = "Deposit";
			Link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			Link.l3.go = "Exit";
		break;

		case "All":
			Pchar.QuestTemp.Deposits.(NPC_Area).Interest = makeint(GetCharacterSkillToOld(Pchar, "commerce")/2.0 + 0.33) + 1;
			Pchar.QuestTemp.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/10)*10;
			Dialog.snd = "voice\USDI\USDI032";
			dialog.text = "������. ��������� ��� ��� ����� � ����� ��� ����������...��...������, " + Pchar.QuestTemp.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			Link.l1 = "���� ��� ����������.";
			Link.l1.go = "Deposit_placed";
			Link.l2 = "����� � ������ �����.";
			Link.l2.go = "Deposit";
			Link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			Link.l3.go = "Exit";
		break;

		case "Deposit_placed":
			Dialog.snd = "voice\USDI\USDI033";
			if ( !CheckAttribute(Pchar, "Quest.Deposits." + (NPC_Area)+".Result"))
			{
			    Pchar.Quest.Deposits.(NPC_Area).Result = 0;//fix
			}
			// boal limit
			if ( (sti(Pchar.Quest.Deposits.(NPC_Area).Result) + sti(Pchar.QuestTemp.Deposits.(NPC_Area).Sum)) > sti(NPChar.UsurerDeposit)*1000)
			{
			    dialog.text = "� ���������, � �� ���� ���������� ����������� ����� ������� �����. �� ��� ������� � ���� " +
				        FindRussianMoneyString(MakeMoneyShow(sti(Pchar.Quest.Deposits.(NPC_Area).Result), MONEY_SIGN,MONEY_DELIVER)) +
				        ", � � ���� ������� �� ��� � ����� �� ����� " +
						FindRussianMoneyString(MakeMoneyShow(sti(NPChar.UsurerDeposit)*1000, MONEY_SIGN,MONEY_DELIVER)) + ".";
				Link.l1 = "����� ����, �������� ���������� � ������� ����������.";
				Link.l1.go = "Exit";
			}
			else
			{
				dialog.text = "������ �� ������ ���� �������, ��� ���� ����� �������� ���� �� ������� ��� ��� ������� � �������������. ���� �� �� ������ ������� �����, �� ��������� � ����� �����.";
				Link.l1 = "������. ����� ��������.";
				Link.l1.go = "Deposit_Exit";
			}
		break;
		
		case "Deposit_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			Pchar.Quest.Deposits.(NPC_Area).Interest = sti(Pchar.QuestTemp.Deposits.(NPC_Area).Interest);
			Pchar.Quest.Deposits.(NPC_Area).Sum      = sti(Pchar.QuestTemp.Deposits.(NPC_Area).Sum);

			AddMoneyToCharacter(Pchar, -(makeint(Pchar.Quest.Deposits.(NPC_Area).Sum)));
			// ����� ����
			Pchar.Quest.Deposits.(NPC_Area).Sum = sti(Pchar.Quest.Deposits.(NPC_Area).Result) + sti(Pchar.Quest.Deposits.(NPC_Area).Sum);
			Pchar.Quest.Deposits.(NPC_Area) = true;
			Pchar.Quest.Deposits.(NPC_Area).StartDay = getDataDay();
			Pchar.Quest.Deposits.(NPC_Area).StartMonth = getDataMonth();
			Pchar.Quest.Deposits.(NPC_Area).StartYear = getDataYear();
			Pchar.Quest.Deposits.(NPC_Area).StartTime = getTime();
			DialogExit();
		break;

		case "Deposit_return":
			Dialog.snd = "voice\USDI\USDI034";
			dialog.text = "������ �� ���������, ������� �� ��������� � ������� ���, � �������� ��������� �����, � ���������, ��� ������ ��� " + FindRussianMoneyString(sti(Pchar.Quest.Deposits.(NPC_Area).Result)) + "... �� �������, ��� ������ ������� ��� ������";
			Link.l1 = "���������. ������� �� ����.";			
			Link.l1.go = "Deposit_return_1";		
			Link.l2 = "������, �� �����. ����� ��� ��� ������� �������. ����� ��������.";			
			Link.l2.go = "Exit";		
		break;

		case "Deposit_return_1":
			addMoneyToCharacter(Pchar, makeint(Pchar.Quest.Deposits.(NPC_Area).Result));
			Dialog.snd = "voice\USDI\USDI035";
			dialog.text = "��... ���� ���� � ���� ������������. � � ��� ��� ���-�� ������. ��� �� - ��������� �� ��� ���.";			
			Link.l1 = "���� ����������� - �����. ��������� ����������.";			
			Link.l1.go = "Exit";
			DeleteAttribute(Pchar, "quest.Deposits." + (NPC_Area));
		break;

		case "DeadMotherfucker":
			Dialog.TempNode = "DeadMotherFucker_1";
			Dialog.snd = "voice\USDI\USDI035";
			dialog.text = "�� ��� ����? ��������, ������ ������� ��� ������? � �����! ��-�� ��� � ������� �������, ������� �� ��� � ����� �� ����������! � �� ���������, ���� �� ����� ��� ����!";
            		Link.l1 = "� ��� ��� ���� ������� ���� ���� �����.";
			Link.l1.go = "LoanRestore_1";
			Link.l2 = LinkRandPhrase("�� �����, ���������! ������ ����, ��� �� � ��� ����� ������ ����� ������!", "��������� ���?! �� ������, ���������, ��� �� �����������...", "������ - �� ������� � �����. ��� ��� �� ����� ���� ���� ��� ��� ��������� ������ � ������� ������.");
			Link.l2.go = "ExitDelLoan2";
		break;

		case "DeadMotherFucker_1":			
			Dialog.snd = "voice\USDI\USDI035";
			dialog.text = "���������� �� ����� ����! ����� ��� ��� �� �� ������!";			
			Link.l1 = "...";			
			Link.l1.go = "ExitDelLoan2";
            Dialog.TempNode = "DeadMotherFucker_1";
		break;
		// boal ����������� ��������� -->
		case "LoanRestore_1":
            iPastMonths = GetPastTime("Month", makeint(Pchar.Quest.Loans.(NPC_Area).StartYear),makeint(Pchar.Quest.Loans.(NPC_Area).StartMonth),makeint(Pchar.Quest.Loans.(NPC_Area).StartDay), makefloat(Pchar.Quest.Loans.(NPC_Area).StartTime), getDataYear(),getDataMonth(),GetDataDay(), GetTime());
			Pchar.Quest.Loans.(NPC_Area).Result = makeint(Pchar.Quest.Loans.(NPC_Area).Sum) + ((makeint(Pchar.Quest.Loans.(NPC_Area).Sum)/100)*makeint(Pchar.Quest.Loans.(NPC_Area).Interest))*(iPastMonths+1);
			dialog.text = "��? � ��� �� �����������?";
			Link.l1 = "�� ������� � ������ ��� ����� � " + FindRussianMoneyString(sti(Pchar.Quest.Loans.(NPC_Area).Result)) + ". ��������� �� ���.";
			Link.l1.go = "DeadMotherFucker_1";
            if (sti(Pchar.Money) >= 2*sti(Pchar.Quest.Loans.(NPC_Area).Result))
            {
    			Link.l2 = "����� �������� ���������, ��������� ������� ��� ����� ����� � ������� " + FindRussianMoneyString(2*sti(Pchar.Quest.Loans.(NPC_Area).Result)) + ".";
    			Link.l2.go = "LoanRestore_2";
			}
		break;
		
		case "LoanRestore_2":
			dialog.text = "��� ������ ����! � ���� ��� �� ����� ����� ����.";
			Link.l1 = "�������. ������ �� �������.";
			Link.l1.go = "ExitDelLoan2";
			NextDiag.TempNode = "First time";			
			addMoneyToCharacter(Pchar, -2*sti(Pchar.Quest.Loans.(NPC_Area).Result));
			DeleteAttribute(PChar, "quest.Loans_" + NPC_Area);
			DeleteAttribute(PChar, "quest.Loans." + NPC_Area);
			ChangeCharacterHunterScore(PChar, NationShortName(sti(NPChar.nation)) + "hunter", -30);
			ChangeCharacterReputation(pchar, 2);
		break;

		case "ExitDelLoan1":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			DeleteAttribute(Pchar, "quest.Loans." + (NPC_Area));
		break;

		case "ExitDelLoan2":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			DeleteAttribute(Pchar, "quest.Loans." + (NPC_Area));
			DeleteAttribute(Pchar, "quest.Deposits." + (NPC_Area));
		break;
		
        case "LoanGranted_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;

			AddMoneyToCharacter(Pchar, makeint(Pchar.Quest.Loans.(NPC_Area).Sum));
			Pchar.Quest.Loans.(NPC_Area) = true;
			Pchar.Quest.Loans.(NPC_Area).StartDay = getDataDay();
			Pchar.Quest.Loans.(NPC_Area).StartMonth = getDataMonth();
			Pchar.Quest.Loans.(NPC_Area).StartYear = getDataYear();
			Pchar.Quest.Loans.(NPC_Area).StartTime = getTime();

			sTemp = "Loans_" + NPC_Area;			
			SetTimerCondition(sTemp, 0, makeint(Pchar.Quest.Loans.(NPC_Area).Period), 0, false);
			pchar.quest.(sTemp).CityId = NPC_Area;
			pchar.quest.(sTemp).win_condition = "LoansForAll";
			
			DialogExit();
			npchar.quest.trade_date = lastspeak_date; // ���� � ��������� ����
		break;
		//������ ������������ ����� �� ������
		case "usurersJewel_1":
			sld = ItemsFromID("UsurersJew");
			switch (rand(3))
			{
				case 0: 
					sld.JevelType = "���������";  
					sld.model = "Diamond";
					sld.picIndex = 2;
				break;
				case 1: 
					sld.JevelType = "�����";  
					sld.model = "Ruby";
					sld.picIndex = 3;
				break;
				case 2: 
					sld.JevelType = "�������";  
					sld.model = "Emerald";
					sld.picIndex = 4;
				break;
				case 3: 
					sld.JevelType = "������";  
					sld.model = "Sapphire";
					sld.picIndex = 1;
				break;
			}			
			dialog.text = "�� ������, �� ���� ��������� �������� ���������. � ������� " + sld.JevelType + " ���������� ������. �� ����� ���������� ���� �� ���. ����� ������, ��� ������ - �� ���!";
			link.l1 = "�� � ���? ������ ������ " + sld.JevelType + " � ���� � ������.";
			link.l1.go = "usurersJewel_2";
			npchar.quest.usurersJewel.type = sld.JevelType; //��� �����
			npchar.quest.usurersJewel.money = (rand(6)+2) * 500; //��������������
		break;
		case "usurersJewel_2":
			dialog.text = "��, ���� �� ��� ���� ��� ������! ���� � ���, ��� ���� " + npchar.quest.usurersJewel.type + " �������� � ����� ����. ���� ������ ������� ��� �������� ��� ������������ �������. ��������� ���-������ �������� �� ������� � ����� � �� ���� �����������...";
			link.l1 = "�-��, ��� ������������� ��������. ��, � ��� �� � ���� ��� ������?";
			link.l1.go = "usurersJewel_3";
		break;
		case "usurersJewel_3":
			dialog.text = "������� " + npchar.quest.usurersJewel.type + ", ������ � �����!";
			link.l1 = "� ��� �� ��� ��������?";
			link.l1.go = "usurersJewel_4";
		break;
		case "usurersJewel_4":
			dialog.text = "� ��������, ����� �� �������. � ������ ����� ����������� ��� � ������. ��� �����������...";
			link.l1 = "���������, � ������� ��������� ������ ���������. � �� ����� � �� �����. ������ ���������� ���� ����� ��� �� ���������.";
			link.l1.go = "usurersJewel_out";
			link.l2 = "��, �� ��� ��, � ����� ������� �� ������ ����������� �����. ������ �������������� �� ������?";
			link.l2.go = "usurersJewel_5";
		break;

		case "usurersJewel_out":
			dialog.text = "�-��, ����� ����. ������, � � ��� ������. ����� ���� ��������...";
			link.l1 = "�� �� ��������.";
			link.l1.go = "exit";
			DeleteAttribute(npchar, "quest.usurersJewel");
			ChangeCharacterReputation(pchar, -1);
		break;
		case "usurersJewel_5":
			dialog.text = "���� �� ������� ��������� " + npchar.quest.usurersJewel.type + ", �� � ������ �������. ���� ��������, ��� ����� ������.";
			link.l1 = "��, ��� ������. �� ��� ��, � ����������� �� ������. �������� ����������.";
			link.l1.go = "exit";
			pchar.questTemp.different = "TakeMayorsRing";
			//����� ������
			sld = ItemsFromID("UsurersJew");
			sld.shown = true;
			sld.startLocation = npchar.city + "_ExitTown";
			sld.startLocator = "item" + (rand(14)+1);
			sld.CityName = XI_ConvertString("Colony" + npchar.city + "Gen");
			Log_QuestInfo("������ ��������� � �������� " + sld.startLocator);
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ���������� 
			sTitle = npchar.city + "UrurersJewel";
			AddQuestRecordEx(sTitle, "SeekUrurersJewel", "1");
			AddQuestUserDataForTitle(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserDataForTitle(sTitle, "sJewType", npchar.quest.usurersJewel.type);
			AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sName", GetFullName(npchar));
			AddQuestUserData(sTitle, "sJewType", npchar.quest.usurersJewel.type);
		break;

		case "usurersJewel_R1":
			dialog.text = "���������! ��-��, ��� ��, ��� ����� ���������� " + npchar.quest.usurersJewel.type + "... �� ��� ��, � �������, ����� �������. � ����� ��������� ��� ������� �� ������.";
			link.l1 = "����� ������.";
			link.l1.go = "usurersJewel_R2";
			TakeItemFromCharacter(pchar, "UsurersJew");
		break;
		case "usurersJewel_R2":
			dialog.text = "������� ��� ��� ���. �� ������ � ����� ����� ���������� �� ���, � ������ �������� ���.";
			link.l1 = "�������, ���� ��...";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, 2);
			AddCharacterExpToSkill(pchar, "Leadership", 15);
			AddCharacterExpToSkill(pchar, "Fortune", 25);
			AddMoneyToCharacter(pchar, sti(npchar.quest.usurersJewel.money));
			sTitle = npchar.city + "UrurersJewel";
			CloseQuestHeader(sTitle);
			DeleteAttribute(npchar, "quest.usurersJewel");
		break;
	}
}

int findCitizenMan(ref NPChar, bool bCity)
{
    ref ch;
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	if (bCity && sti(Pchar.Ship.Type) == SHIP_NOTUSED)
	{
		ch = GetCharacter(NPC_GenerateCharacter("LoanFindingMan", "citiz_"+(rand(11)+1), "man", "man", 10, sti(npchar.nation), -1, false));
		ch.dialog.filename = "Common_citizen.c";
		ch.city = npchar.city;
		ch.RebirthPhantom = true; 
		LAi_NoRebirthDisable(ch);
		LAi_SetCitizenType(ch);
		LAi_group_MoveCharacter(ch, GetNationNameByType(sti(npchar.nation)) + "_citizens");
		PlaceCharacter(ch, "goto", npchar.city + "_town");
	}
	else
	{		
		for(n=0; n<MAX_COLONIES; n++)
		{			
			nation = GetNationRelation(sti(npchar.nation), sti(colonies[n].nation));	
			if (nation != RELATION_ENEMY && colonies[n].nation != "none")
			{           
				storeArray[howStore] = n;
				howStore++;
			}
		}
		if (howStore == 0) return -1; 
		nation = storeArray[rand(howStore-1)];
		ch = GetCharacter(NPC_GenerateCharacter("LoanFindingMan", "citiz_"+(rand(11)+1), "man", "man", 10, sti(colonies[nation].nation), -1, false));
		ch.dialog.filename = "Common_citizen.c";
		ch.city = colonies[nation].id;
		ch.RebirthPhantom = true; 
		LAi_NoRebirthDisable(ch);
		LAi_SetCitizenType(ch);
		LAi_group_MoveCharacter(ch, GetNationNameByType(sti(colonies[nation].nation)) + "_citizens");
		PlaceCharacter(ch, "goto", colonies[nation].id + "_town");
	}
	return sti(ch.index);
}

int findChestMan(ref NPChar)
{
    ref ch;
	int n;
    int storeArray[TOTAL_CHARACTERS];
    int howStore = 0;
	string sTemp, sCity;

	for(n=2; n<MAX_CHARACTERS; n++)
	{
		makeref(ch,Characters[n]);
		sTemp = ch.id;
		if (CheckAttribute(ch, "City") && ch.id != "Jackman") sCity = ch.City;
		else continue;
		sTemp = strcut(sTemp, strlen(sCity)+1, strlen(sTemp)-1);
		// ��������
		if (sTemp == "trader")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}
        // ����
		if (sTemp == "Mayor")
		{
            if (NPChar.city == ch.city) continue;
            if (sti(ch.nation) == PIRATE) continue; // ������ �� ����� ������
            if (ch.location == "none") continue;
			if (ch.location != ch.Default) continue; //����������� ����� �� ����
            storeArray[howStore] = n;
            howStore++;
		}
        // ��������
		if (sTemp == "shipyarder")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}
		// ����������
		if (sTemp == "tavernkeeper")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}
	    // ����������
		if (sTemp == "Priest")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}		
		// ����������
		if (sTemp == "usurer")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}
		// ���������� ������
		if (sTemp == "PortMan")
		{
            if (NPChar.city == ch.city) continue;
            if (ch.location == "none") continue;
            storeArray[howStore] = n;
            howStore++;
		}
    }
    if (howStore == 0)
    {
        return -1;
    }
    else
    {
        return storeArray[rand(howStore-1)];
    }
}
