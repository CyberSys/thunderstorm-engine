// boal 25/04/04 ����� ������ �������
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Church\" + NPChar.City + "_Church.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 16/06/06
    
    string iDay, iMonth, lastspeak_date;
    string sTemp, sTitle;

	iDay           = environment.date.day;
	iMonth         = environment.date.month;
	lastspeak_date = iday + " " + iMonth;

	if (!CheckAttribute(npchar, "quest.HealthMonth"))
    {
        npchar.quest.HealthMonth = "";
    }
    if (!CheckAttribute(npchar, "quest.BadMeeting"))
    {
        npchar.quest.BadMeeting = "";
    }
    if (!CheckAttribute(npchar, "quest.GhostShipMonth"))
    {
        npchar.quest.GhostShipMonth = "";
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
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("��� ���, � ���������, � �� ���� ���� ���� �������. ����!", "��� ��������� ������ ����� �� ������ � ������� ����, ��� ���. ������� ������� ��� ����������, �� � �� ���� ������������ ���� �������...", "��� ���, ���� ���� ������! ����� ����, ����������!"), 
					LinkRandPhrase("��� �� ������� � ������, ������ ����? � ������, ����� �� �������� ����������, ���� ������� �� ��������� � �� �������� ����� �����!", "���������� ������ ����, ���������! � �� ���������� �� ������, ��� ��!", "���������� ������ ���� �����, ������! ��� �� ����� �������� �����!!"));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("�� ����� ����, �����...", "���, ��� �� ����� ��� ���� ������..."), 
					RandPhraseSimple("���, ������ ����, ����� ��������...", "����, �� �����, �����..."));
				link.l1.go = "fight";
				break;
			} 
            if (npchar.quest.BadMeeting != lastspeak_date)
			{
				Dialog.Text = "����� ���������� � ���� ������, ��� ���.";
				link.l1 = "������������, ������ ����.";
				link.l1.go = "node_3";
				Link.l2 = "�� ��� �� ����, � �� ���� ��� ������ ����������.";
				Link.l2.go = "node_2";
			}
			else
			{
				Dialog.Text = "�������� ���� �������. ��� �� � ��� ������� ����� ��������.";
				Link.l1 = "�� � �� �����!";
				Link.l1.go = "exit";
			}
			NextDiag.TempNode = "First time";
		break;
		
		case "node_2":
			dialog.text = "�� ��, �����������! � �� ����� ������, ���� �� ���������� ����� ������������ ��� ����!";
			link.l1 = "�� � � ��� �� ��������� ����� �������������.";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, -0.25);
			npchar.quest.BadMeeting = lastspeak_date;
		break;

		case "node_3":
			dialog.text = "�� ����������� ������� ��� � ���� ����... �� ������ �� ��� � �����-�� �����?";
   			//homo ������� �����
            if (Pchar.questTemp.CapBloodLine != true )
            {
    			link.l1 = RandPhraseSimple("� ���� ������ �������������.", "���� ������������ �� ������ ����.");
    			link.l1.go = "donation";
    			link.l2 = RandPhraseSimple("�����, ������� ����� ��������.","��� ����� ���������, "+RandPhraseSimple("�����.", "������ ����."));
    			link.l2.go = "ispoved";
    			link.l3 = RandPhraseSimple("� ���� � ��� ����, " + RandPhraseSimple("�����.", "������ ����."),
                                           "� ������ �� ����, " + RandPhraseSimple("�����.", "������ ����."));
    			link.l3.go = "work";//"quest lines";
            }

			link.l6 = "��������, ��� ���� ����.";
			link.l6.go = "exit";
		break;

		case "donation":
			dialog.Text = "�������, ��� ���. ������� �� ������� ������������ ������ ������?";
			Link.l1 = "��������, ������ ����, �� ������������� �� �����.";
			Link.l1.go = "No donation";
			if(makeint(PChar.money)>=100)
			{
				Link.l2 = "� ���������, �������. 100 ��������.";
				Link.l2.go = "donation paid_100";
			}
			if(makeint(PChar.money)>=1000)
			{
				Link.l3 = "1000 ��������. � �����, ����� ������.";
				Link.l3.go = "donation paid_1000";
			}
			if(makeint(PChar.money)>=5000)
			{
				Link.l4 = "��� ����� � ��������, ������� � ��������� 5000 ��������.";
				Link.l4.go = "donation paid_5000";
			}
			//-->> ����� ������������� ������� �������
			if(pchar.questTemp.different == "HostessChurch_toChurch" && pchar.questTemp.different.HostessChurch.city == npchar.city && sti(pchar.money) >= sti(pchar.questTemp.different.HostessChurch.money))
			{
				Link.l5 = "������ ����, � ���� ������� ������������� �� �� ����. � ����� ��� �� ���������.";
				Link.l5.go = "HostessChurch";
			}
			//<<-- ����� ������������� ������� �������
		break;

		case "No donation":
			dialog.Text = "��... ��� ���� �������, � �� ������ ��� ��������. �� �� ��������� � ���, ��� ��� ��� �������� ��� ����� ������.";
			Link.l1 = "��� ��� ������ �� ������! ������� ����� ������ ����.";
			Link.l1.go = "node_3";
			Link.l2 = "��������, ������ ����, ��� ����.";
			Link.l2.go = "exit";
		break;

		case "donation paid_100":
			AddMoneyToCharacter(pchar, -100);
			pchar.questTemp.donate = makeint(pchar.questTemp.donate) + 100;
			dialog.Text = "�� ���� ������ ������ ��������� ���, ��� ���, �� ��� ���.";
			Link.l1 = "��� ���� ���������� � ����, ������ ����.";
			Link.l1.go = "node_3";
			Link.l2 = "��������, ������ ����, ��� ����.";
			Link.l2.go = "exit";
			AddDialogExitQuest("donation");
		break;

		case "donation paid_1000":
			AddMoneyToCharacter(pchar, -1000);
			pchar.questTemp.donate = makeint(pchar.questTemp.donate) + 1000;
			dialog.Text = "�� ���� ������ ������ ��������� ���, ��� ���, �� ��� ���.";
            Link.l1 = "��� ���� ���������� � ����, ������ ����.";
			Link.l1.go = "node_3";
			Link.l2 = "��������, ������ ����, ��� ����.";
			Link.l2.go = "exit";
			AddDialogExitQuest("donation");
		break;

		case "donation paid_5000":
			AddMoneyToCharacter(pchar, -5000);
			pchar.questTemp.donate = makeint(pchar.questTemp.donate) + 5000;
			dialog.Text = "�� ���� ������ ������ ��������� ���, ��� ���, �� ��� ���.";
            Link.l1 = "��� ���� ���������� � ����, ������ ����.";
			Link.l1.go = "node_3";
			Link.l2 = "��������, ������ ����, ��� ����.";
			Link.l2.go = "exit";
			AddDialogExitQuest("donation");
		break;

		case "ispoved":
			dialog.text = "������� ��, ��� ���, � ���� ������.";
			link.l1 = "� ���������. ����� ���� � ������ ���.";
			link.l1.go = "exit";
			link.l2 = "� ��� ������, ������ ����...";
			link.l2.go = "ispoved_1";
		break;

		case "ispoved_1":
			dialog.text = "��� �� ������.";
			link.l1 = "��, �� �... � ����� � ������ �������, � ������...";
			link.l1.go = "ispoved_2";
		break;

		case "ispoved_2":
			dialog.text = "�� ���� ����� ��� �� ���� ���, ��� ������ ��� ������ " + NationNameGenitive(sti(NPChar.nation)) + "?";
			link.l1 = "���, �������, ������ ����.";
			link.l1.go = "ispoved_3";
			link.l2 = "���� � �����, ������ ����.";
			link.l2.go = "ispoved_4";
		break;

		case "ispoved_3":
			dialog.text = "����� ������� ������� ���� ���� �����.";
			link.l1 = "�������, ������ ����, � �����.";
			link.l1.go = "exit";
		break;

		case "ispoved_4":
			dialog.text = "������� ������� �� ������� ���� ���� ����!";
			link.l1 = "��������, ������ ����.";
			link.l1.go = "exit";
			npchar.quest.BadMeeting = lastspeak_date;
		break;

        case "work":
			dialog.text = "����� ���� ������� ����, ��� ���.";
			link.l1 = "� ����� �� ������ ��� ���� ������, �����...";
			link.l1.go = "quests"; // ������ � ���
            link.l2 = RandPhraseSimple("� ���� ���������� � ������ �� ����� ������ " + NationNameGenitive(sti(NPChar.nation)) + ".",
                                        "��� ���� ���� � �������, �� ����� �� ����� ������?");
			link.l2.go = "prihod";
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
			{
	            link.l3 = RandPhraseSimple("������ ����", "�����") + ", � ����� �� ���������� � ���� � ���������� ��������."; //(��������� � �������)
	            link.l3.go = "LoanForAll";
            }
			// -->
            if (stf(pchar.Health.HP) < 60.0)
            {
                link.l4 = "� �������� � ���������.";
                link.l4.go = "healthAdd_1";
            }
            //==> ������� ����, ����� ����, ����������� ����� �����.
            if (CheckCharacterItem(pchar, "Azzy_bottle"))
            {
                link.l6 = "������ ����, � ���� ���� ����, ������� � ���� ������ ���. ��� �������� �������� �����, ����������.";
                link.l6.go = "GiveLamp";
            }
			//<== ������� ����, ����� ���� ����������� ����� �����.
			//����� ���� - �� ����� � ����� �������
			if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "")
			{
	            link.l7 = RandPhraseSimple("������ ����", "�����") + ", � ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".";
	            link.l7.go = "IntelligenceForAll";
            }
			//==> ��������� ������� � �������
			if (CheckAttribute(npchar, "DestroyGhost") && npchar.DestroyGhost == "GoodResult")
			{
				switch (npchar.quest.DestroyGhost.label)
				{
					case "������": sTemp = "������ �������"; break;
					case "����": sTemp = "���� ������"; break;
					case "����������": sTemp = "���������� �������"; break;
				}
				link.l5 = "������ ����, " + sTemp + " �� �������. ���� �������, ������ ������� ����� �� �����������.";
                link.l5.go = "GhostShip_S1";
			}
			//==> ������ ������ � ������
			if (CheckAttribute(pchar, "questTemp.different.Church_NightGuard") && pchar.questTemp.different.Church_NightGuard == npchar.location)
			{
	            link.l8 = RandPhraseSimple("������ ����", "�����") + ", � ���� ���������� � ��������� ���������� � �������� ������� �������.";
	            link.l8.go = "NightGuard_result";
            }
			//<== ������ ������ � ������
			link.l99 = "� ���������, ���� ���� ����.";
			link.l99.go = "exit";
		break;

		case "prihod":
			//��������� �������
			if (rand(2)==1 && pchar.questTemp.different == "free" && !CheckAttribute(npchar, "quest.DestroyGhost") && GetNpcQuestPastDayWOInit(npchar, "Ghost") > 7)
			{
                dialog.text = "��� �� ������ ������ �� ���� ��������, �� � ���� ��� ������� ������. � ����� ��� ��������� �����. � �� ���� ����� ���� ����������. ����� ����, �� � ���� �� ������ ��������, ������� ���������� ��� ��� ����������� �������, ������������ ���� �����.";
        		link.l1 = RandPhraseSimple("��� �� ������ �������, ������ ����, ��� ��� ��� ��������? ��� ������ ���� �����?",
                                            "� ����� ���, ����������� ������, ������ ����.");
        		link.l1.go = "DestroyGhost_1";
				SaveCurrentNpcQuestDateParam(npchar, "Ghost");
				break;
			}
			//��������� ������� �����
			if (rand(4)==1 && pchar.questTemp.different == "free" && !CheckAttribute(npchar, "quest.GuardNightChurch") && !CheckAttribute(pchar, "questTemp.different.Church_NightGuard") && GetNpcQuestPastDayWOInit(npchar, "NightChurch") > 60 && GetHour() < 23)
			{
                dialog.text = "��� ���, � ���� ���� ��� ���� ��������� ���������. ��� ����� ������ ������, �� �� ������ � ���������� � �� ����������� ����������. ��� ���� � ���, ��� � ��������� ����� '������' ������� ����������. �����-�� �������� ���� ����� �������� � �������, �� �� �������, � ������ ������� �� ��������...";
        		link.l1 = "� ��� ��� ���������, ������ ����?";
				link.l1.go = "NightChurch_1";
				SaveCurrentNpcQuestDateParam(npchar, "NightChurch");
				break;
			}
            dialog.text = "� ��� � ������� ��� ��������, ��� ���. ������� �� �����������.";
    		link.l1 = "��, ��� ��� ������, � ����� �� ����� �����.";
    		link.l1.go = "exit";
		break;
		//--->> ��������� �������
		case "DestroyGhost_1":
			npchar.quest.DestroyGhost = "toCave"; //������ ���� ���������� �� ������ �����
			//������� ������ ��� ������
			sTemp = GetArealByCityName(npchar.city);
			if (sTemp == "Cuba2") sTemp = "Cuba1";
			aref aPlace, aPlace_2;
			makearef(aPlace, NullCharacter.TravelMap.Islands.(sTemp).Treasure);
			int iQty = GetAttributesNum(aPlace)-1;
			aPlace_2 = GetAttributeN(aPlace, rand(iQty));
			npchar.quest.DestroyGhost.locationId = GetAttributeName(aPlace_2); //Id ������� ������
			sld = &locations[FindLocation(npchar.quest.DestroyGhost.locationId)];
			npchar.quest.DestroyGhost.label = GetConvertStr(sld.id.label, "LocLables.txt"); //��� ���������� ��� ��������
			switch (npchar.quest.DestroyGhost.label)
			{
				case "������": sTemp = "���� � ���, ��� ������, ����������� ����� ������ ������, ����� ���������� �������!"; break;
				case "����": sTemp = "���� � ���, ��� ����, ����������� ����� ������ ������, ���� ���������� �������!"; break;
				case "����������": sTemp = "���� � ���, ��� ����������, ����������� ����� ������ ������, ����� ���������� �������!"; break;
			}			
			dialog.text = "��� ������ � ������ ����� ������������ � �������� �������������. " + sTemp + " ������ �� ���� ����� ����� ����� �����, ��� � � �������� ��� �� ���������. ���� ��������, ����� ��������� � �� ������...";
			link.l1 = "��� � ���� ��� ������, ������ ����?";
			link.l1.go = "DestroyGhost_2";
		break;
		case "DestroyGhost_2":
			dialog.text = "�� - ������ �������. ������� ������� ������� � ���� �����! ���������� �������! � �������� ��� ����� ��������� ������ �����...";
			link.l1 = "� �����, �����!";
			link.l1.go = "DestroyGhost_3";
			link.l2 = "������ ����, � �� ���� ������� ���� �����������. ���-������ � ������ ���.";
			link.l2.go = "DestroyGhost_out";
		break;
		case "DestroyGhost_out":
			dialog.text = "�����, ��� ���, ����� �����. � �������� �� ����!";
			link.l1 = "� �����, ��� �������� �� ��������...";
			link.l1.go = "exit";
			DeleteAttribute(npchar, "quest.DestroyGhost");
			ChangeCharacterReputation(pchar, -3);
		break;
		case "DestroyGhost_3":
			dialog.text = "����� ������, ��� ���. ����������� ����, � �� �������� � ����� ����!";
			link.l1 = "��� ����� � �������, ������ ����.";
			link.l1.go = "exit";
			pchar.questTemp.different = "DestroyGhost";
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ���������� 
			//���� ������ �� �������, ���� �� ����� ������ ������� ����������
			sld = &locations[FindLocation(npchar.quest.DestroyGhost.locationId)];
			sld.DestroyGhost = npchar.index; 
			sTitle = sld.id + "Church_DestroyGhost";
			AddQuestRecordEx(sTitle, "Church_DestroyGhost", "1");
			AddQuestUserDataForTitle(sTitle, "sLocation", GetStrSmallRegister(npchar.quest.DestroyGhost.label));
			AddQuestUserDataForTitle(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sLocation", npchar.quest.DestroyGhost.label);
		break;

		case "GhostShip_S1":
			dialog.text = "�������, ��� ���! �� ������ ����������� ����. ����, ��� ��������� � ���� " + NationNameGenitive(sti(NPChar.nation)) + " ������ �����.";
			link.l1 = "� ��������, �����...";
			link.l1.go = "GhostShip_S2";
		break;
		case "GhostShip_S2":
			dialog.text = "� ���� �������� �� ����. ��� ��� ���, � ���� ������ ����� ����� �������� � �������� �����.";
			link.l1 = "������, ������ ����.";
			link.l1.go = "exit";
			switch (npchar.quest.DestroyGhost.label)
			{
				case "������": sTemp = "������"; break;
				case "����": sTemp = "����"; break;
				case "����������": sTemp = "����������"; break;
			}
			AddSimpleRumourCity(LinkRandPhrase("��� ������ ����, " + GetFullName(npchar) + ", ��������� � ���, ��� �� �������� �� ������� " + sTemp + " ���� ������ ������. ����������� ����, ����� � ���...", 
				"�� �������, ����� ������� �� ������� ������ ������� ���� ������� �� ������� " + sTemp + " ����� � ����� �������. ����� �������!", 
				"�������, ��� �� �������� " + sTemp + " ����� � ����� ������� �� �������? ������, ��� �� ��� ��� ����� ����������!"), npchar.city, 10, 1, "");
			ChangeCharacterReputation(pchar, 5);
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 15);
			AddCharacterExpToSkill(pchar, "Leadership", 30);
			sld = &locations[FindLocation(npchar.quest.DestroyGhost.locationId)];
			sTitle = sld.id + "Church_DestroyGhost";
			AddQuestRecordEx(sTitle, "Church_DestroyGhost", "3");
			CloseQuestHeader(sTitle);
			DeleteAttribute(npchar, "quest.DestroyGhost");
		break;
        //<<--- ��������� �������
		//--->> ������ ������
		case "NightChurch_1":
			dialog.text = "������ ���. � ������ ���, ��� ��� ���������. ����� ���������� ����� ����, �� ��� �������� ������ ��������� ��������� ����������. ���� �� �����, ����-�� ������������ ��������� ������\n�� ���, �� �������� �������� ��������� ����� � ������?";
			link.l1 = "���, �������, ��������, ������ ����. ����� ���� ��������.";
			link.l1.go = "exit";
			link.l2 = "� ��������, � ��������. �� ������� ����� ��� ���� ����� �������� �������? �����, ����� � �� ��������, � ���� �������� ���� - ������� ����������, ������� ������ ���������� �� ����� ��������� ���� ����������� �������.";
			link.l2.go = "NightChurch_2";
		break;
		case "NightChurch_2":
			dialog.text = "��, ����� � ���, �� ��� ������������. � �����, ���� ����� ����� ���������� ����� ��� ����. ���� ����� ��� � �� ��������, �� ����� �������, ��� ���� ������ ��������� �������.";
			link.l1 = "��� ��, ��� ���� ����������. � ��������!";
			link.l1.go = "NightChurch_3";
		break;
		case "NightChurch_3":
			dialog.text = "����� ��������� � ����������� ����. � �����, ��� �� ������ ��������� � ������ ���������� � �������� �� ����� ����.";
			link.l1 = "����, ������ ����. ��� ���� ������ ���� �������� �� ���� ������.";
			link.l1.go = "exit";
			pchar.questTemp.different = "Church_NightGuard";
			SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ���������� 
			pchar.questTemp.different.Church_NightGuard = npchar.location; //���� ������������ ���� �����
			pchar.questTemp.different.Church_NightGuard.NightQty = 0; //���-��� ������������ �����
			pchar.questTemp.different.Church_NightGuard.chance = rand(3); //������ �� ��������� ����������
			pchar.questTemp.different.Church_NightGuard.fight = false; //���� ������
			SaveCurrentQuestDateParam("questTemp.different.Church_NightGuard"); //������� ����
			sTitle = npchar.location + "Church_NightGuard";
			AddQuestRecordEx(sTitle, "Church_NightGuard", "1");
			AddQuestUserDataForTitle(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
		break;

		case "NightGuard_result":
			if (sti(pchar.questTemp.different.Church_NightGuard.NightQty) == 0)
			{
				dialog.text = "��� ���, �� ��� �� �������� �� ����� ����. ���, �������� ���� ������, �� ����� ���...";
				link.l1 = "������, ������ ����.";
				link.l1.go = "exit";
			}
			if (sti(pchar.questTemp.different.Church_NightGuard.NightQty) > 0 && sti(pchar.questTemp.different.Church_NightGuard.NightQty) < 3)
			{
				dialog.text = "��� ���, �� �� �������� ������ �� �����, �� �������������� � ���� �����...";
				link.l1 = "��, � �����... �������� ���������.";
				link.l1.go = "exit";
			}
			if (sti(pchar.questTemp.different.Church_NightGuard.NightQty) >= 3)
			{
				if (sti(pchar.questTemp.different.Church_NightGuard.fight))
				{
					dialog.text = "��� ���, ��� ��� �������� � ���, ��� �� ����� ������� ��������� �����. � ���� �������� � ������ ����� �������� - ����, �� ������� �������� ���� ���.";
					link.l1 = "�������, ������ ����...";
					link.l1.go = "NightGuard_fight";
				}
				else
				{
					dialog.text = "��� ���, �� ������ ���������� � ������ �� �����. ������ ��������� �� �����?";
					link.l1 = "���, ������ ����, ��� ���� ��������. ������ �� ����� ������.";
					link.l1.go = "NightGuard_noLook";
				}
			}
		break;

		case "NightGuard_noLook":
			dialog.text = "��, ������, �� ���������... �� ��� ��, ���������� ���������! � �������� ������� � �� ��������� ���� �����, �� ���� �������, ��� ����� ���� ����� �������� ����� � " + NationNameAblative(sti(npchar.nation)) + " ����� ���������.";
			link.l1 = "�������, ������ ����.";
			link.l1.go = "exit";
			sTitle = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
			CloseQuestHeader(sTitle);
			DeleteAttribute(pchar, "questTemp.different.Church_NightGuard");
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 5);
			AddSimpleRumourCity(LinkRandPhrase("�������, ��� �� �������� � ����� ������ �� �����...", 
				"������ ��������� � ������ - ���� �����������. ������� ���.", 
				"��� ������ ����, " + GetFullName(npchar) + ", ��������, ��� �� ����� ������ �� ����� � ����� ������."), npchar.city, 10, 1, "");
		break;
		case "NightGuard_fight":
			dialog.text = "������ �����, � � ����� ��������� ���� ����� ����� " + NationNameAblative(sti(npchar.nation)) + ", ��� � ������. � ���� ��, � ���� �������� �� ���� ��������, �, �������, ����� �� ������������ ���������� ���� ������� ������� ������!";
			link.l1 = "�������� �������, ������ ����!";
			link.l1.go = "exit";
			sTitle = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
			CloseQuestHeader(sTitle);
			DeleteAttribute(pchar, "questTemp.different.Church_NightGuard");
			ChangeCharacterNationReputation(pchar, sti(NPChar.nation), 5);
			AddCharacterMaxHealth(pchar, 2);
			AddSimpleRumourCity(LinkRandPhrase("�������, ��� �� ����� �� ������� �������� � ����� ������! �������� ��� �������!", 
				"�� ������, ��� ������ ������ ���������� ��� �� ��, ��� �� �� ��������� ��������� ����� ������. ������� ���!", 
				"��� ������ ����, " + GetFullName(npchar) + ", ������ � ��� � �������, ��� �� ������ ���� ��������� ������ ��� �����..."), npchar.city, 10, 1, "");
		break;

		//<<--- ������ ������

        case "healthAdd_1":
            if (GetHealthMaxNum(pchar) == 6) // �������
            {
                dialog.text = "�������� ���� " + GetHealthNameMaxSmall(pchar) + ", � ���� ������� ����. ����� ������ �������� �������������� ������.";
    			link.l1 = "����������. ���������� �������� ������� ��������� �����.";
    			link.l1.go = "exit";
            }
            else
            {
    			dialog.text = "�������� ���� " + GetHealthNameMaxSmall(pchar) + ", � ���� ���� ������� ����, ���� �� �������� ��� ���������.";
    			link.l1 = "��� ��� ����� �����?";
    			link.l1.go = "healthAdd_2";
			}
			link.l2 = "� ������ ������ ��� �������� ��������?";
    		link.l2.go = "healthAdd_info";
		break;

		case "healthAdd_info":
			dialog.text = "����� ������, ��� ���, �� ��������� ����, ����� ���� �����. ��� �� �������� ���������, �������� ���� ����������� ������ �����, � � ���������� �� ����������� ���� � ��������� ��� �� � �����.\n ������� ����, ���� �������� ������, ������ � ���� ��������. �� ���� �� ��������� ����� ���������� � ������ ������ �������� �������������. ������������� ����� � ����������� ������� ������� ���������� ������ ������ ���.";
			link.l1 = "�������, �� ���� ����������.";
			link.l1.go = "exit";
		break;

		case "healthAdd_2":
			dialog.text = "������� � ����� �� �������, ��� ���. ����� ������� ������ ���� ��������� �����.";
			link.l1 = "� �� �� ����� �� ������� �� ����?";
			link.l1.go = "healthAdd_3";
			link.l2 = "�� ���� �����? ��... ����������, �������.";
			link.l2.go = "exit";
		break;

		case "healthAdd_3":
            if (npchar.quest.HealthMonth != iMonth)
            {
    			dialog.text = "�? �������, ��� ��������. �� ���� ���������� ������ ������������� ������ ������.";
    			link.l1 = pcharrepphrase("�������?", "� ��������! ����� ��� ������?");
    			link.l1.go = "healthAdd_4";
    			link.l2 = "� �� ���-������ ���...";
    			link.l2.go = "exit";
			}
			else
			{
                dialog.text = "��� ���, � ��� ������ � ����� ������������. ������� ������ ��� ��� ����� �� ����� ������.";
    			link.l1    = "�������, "+RandPhraseSimple("�����.", "������ ����.");
    			link.l1.go = "exit";
			}
		break;

		case "healthAdd_4":
			dialog.text = "��� ����� ������� ����� ������ ����������� ������ �� ���� ���������.";
			link.l1 = pcharrepphrase("�������?!! �� �� �����������. � ������� ����� �����, ����� ��������� ���� �������.", "��� ���� ��� ������� �����. �����, ������ ������� - �������� �� ������ �� ������.");
			link.l1.go = "exit";
            if (sti(pchar.Money) >= 100000)
            {
    			link.l2 = "� ��������!";
    			link.l2.go = "healthAdd_5";
			}
		break;

		case "healthAdd_5":
            AddMoneyToCharacter(pchar, -100000);
            AddCharacterMaxHealth(pchar, 4); // ����� �� ���������, �� ���� ������ 51
            npchar.quest.HealthMonth = iMonth;
			dialog.text = "������. ���� �������� ������� ���������. �� ���� ����� �������� ������ � ������ ����, ����� ���� ������� ������ �� ���. ��� ����� ����� ����� �������� � �������� � ����� ������� ����.";
			link.l1 = "�������. ���� ���, � ����� ����� ����� �����.";
			link.l1.go = "exit";
		break;
		// ������ ��� �������� �� ������
		case "Prison_Temp":
            NextDiag.TempNode = "Prison_Temp";
			dialog.text = "��� �� � ��� ����� ��������.";
			link.l1 = "�� � �� �����.";
			link.l1.go = "Exit";
		break;

		case "Prison_Cell":
            NextDiag.TempNode = "Prison_Temp";
			dialog.text = "��������� ����, �� ������ �� �� ��������� � ������ �����, �������� ���� � ��������� ������ ����?";
			link.l1 = "� ��, ����������, ���?";
			link.l1.go = "Prison_Cell_Info";
			link.l2 = "� �������� � ���������, �����.";
			link.l2.go = "Prison_Cell_Talk";
			link.l3 = "�������� �����, �������.";
			link.l3.go = "Prison_Cell_None";
		break;

		case "Prison_Cell_Info":
			dialog.text = "� ������� ��������, �������� ������ �������, ��������� �� � ������� ����� ���������� ��������� ��������� ��� ���������� ����� ������ ������� �� �������.";
			link.l1 = "� ������ �����, ����� ������� � ���������, ������?";
			link.l1.go = "Prison_Cell_Info_2";
			link.l2 = "� ����� ���������, �����.";
			link.l2.go = "Prison_Cell_Talk";
			link.l3 = "� ���������� �� �������. ��� ��� ��� �� �����.";
			link.l3.go = "Prison_Cell_None";
		break;

		case "Prison_Cell_Info_2":
			dialog.text = "�������, ��� ���, �� ��� �������� �� ��������� ���������, ������ �������� �������� ������ �������. ����� �� �� �������� � ����� ������?";
			link.l2 = "� ����� ���������, �����.";
			link.l2.go = "Prison_Cell_Talk";
			link.l3 = "� ���������� �� �������. ��� ��� ��� �� �����.";
			link.l3.go = "Prison_Cell_None";
		break;

		case "Prison_Cell_None":
			dialog.text = "�������� � ����� � �������������� ������� �� �������� ���� � �������� �����. ��������� � �������!";
			link.l1 = "� ���������, ��� ��� �� �����. ������ �����!";
			link.l1.go = "Prison_Cell_death";//"Exit";
			//AddDialogExitQuest("MoveToPrison_Cell");
		break;

		case "Prison_Cell_Talk":
			dialog.text = "� ������ ����, ��� ���.";
			link.l2 = "� ������, �����, ������ ��������� �� ���� �����, � ������ � ������ ��������, �� ���������� � �� ����. ��� �������� ������ ���������. �� ������ �������� �� �� ���� ��������?";
			link.l2.go = "Prison_Cell_Talk_2";
			link.l3 = "� ���������. �������� ����.";
			link.l3.go = "Prison_Cell_None";
		break;

		case "Prison_Cell_Talk_2":
			dialog.text = "��� �� ��� � ������� ������������? � ������� ���� ����������� ����� �������� ������? �� ������ �����! � ������� ���� ���������� �� ����� ���������.";
			link.l1 = "�� � ����� �� ��, � ��� ���� �������!";
			link.l1.go = "Prison_Cell_death";
		break;

		case "Prison_Cell_death":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
   			sld = &Characters[sti(PChar.GenQuest.ToPrisonCmdIdx)];
			// to_do DeathExecutionStart(Pchar, sti(sld.nation));
		break;
		//����� ������������� ������� �������
		case "HostessChurch":
			dialog.text = "� ��� �� ��� ������ ����, ��� ��������?";
			link.l1 = "��... ��� ������� �������� �������.";
			link.l1.go = "HostessChurch_call";
			link.l2 = "� �� ��������� �� �������� ����. ���� ����� �� ���� ��������� �������������!";
			link.l2.go = "HostessChurch_notCall";
			pchar.questTemp.different = "HostessChurch_return"; //���� �� �����������
		break;
		case "HostessChurch_call":
			if (isBadReputation(pchar, 5)) 
			{
				if (rand(100) < GetCharacterSkill(pchar, "Fortune")) //������ ������ �� �����
				{
					dialog.text = "��� �������, ��� �� �������� � �������? � ����� ���� ������� �� �����, ���� ��� �� ����� �������� ����������!";
					link.l1 = "��� �����. �������, ������� ����� �������� �� ���... ��� ��� ������ �������������?";
					link.l1.go = "HostessChurch_ok";
				}
				else
				{
					dialog.text = "�����, ������!!! ������� ���� ���������� ������ � �������� ������!";
					link.l1 = "������ ����, ��� �� ���?! �� � ��� �� ���� �����, � ��...";
					link.l1.go = "HostessChurch_bad";
				}
			}
			else
			{
				if (rand(10) < GetCharacterSPECIAL(pchar, "Charisma")) //������ ������ �� �������
				{
					dialog.text = "� ���, ��� �� ��������� ��������� ������� ���� � ������� ������. �� ��� ���� ��������� �� �������!";
					link.l1 = "��� ���� �� �������! ��� ��� ������ �������������?";
					link.l1.go = "HostessChurch_ok";
				}
				else
				{
					dialog.text = "� �� ���� ������� ������, ������� �� �����. ����� �� ���� ��������, ��� ���. ����� ������ ���� � ������� �� ��������.";
					link.l1 = "����� ����, ��� �� ���������� ��. ����� ����...";
					link.l1.go = "HostessChurch_bad_1";
				}
			}
		break;
		case "HostessChurch_notCall":
			if (rand(10) < GetCharacterSPECIAL(pchar, "Charisma"))
			{
				dialog.text = "����� ��� ������, ���� �� ����, �� ���� ���...";
				link.l1 = "��, �����, ������ ���. ������� ������������� �� ������� ������!";
				link.l1.go = "HostessChurch_ok_1";
			}
			else
			{
				dialog.text = "����� ��� ������, ���� �� ����, �� ���� ���? �� � ��� �����?! � �����, �� ���� ������� - �����!!";
				link.l1 = "������ ����, �� ���� ������� - �����...";
				link.l1.go = "HostessChurch_bad_2";
			}
		break;

		case "HostessChurch_ok":
			dialog.text = "� �������� ���, ��� ���. �����, ������ �������� ��� ��������.";
			link.l1 = "������, �����. �������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -sti(pchar.questTemp.different.HostessChurch.money));
			AddCharacterExpToSkill(pchar, "Fortune", 20);
			//���� ������� ��� ��������� ������� ����
			characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "yes";
		break;
		case "HostessChurch_ok_1":
			dialog.text = "� �������� ���, ��� ���. ������ �������� ���������.";
			link.l1 = "������, �����. �������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -sti(pchar.questTemp.different.HostessChurch.money));
			AddCharacterExpToSkill(pchar, "Fortune", 20);
			//���� ������� ��� ��������� ������� ����
			characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "yes";
		break;
		case "HostessChurch_bad":
			dialog.text = "��������, ������� ���!!! � �� ���� ������������ ��� �� �����!";
			link.l1 = "���, �� ������.";
			link.l1.go = "exit";
			AddCharacterExpToSkill(pchar, "Leadership", 50);
			npchar.quest.BadMeeting = lastspeak_date;
			//���� ������� ��� ��������� ������� ����
			characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "no";
		break;
		case "HostessChurch_bad_1":
			dialog.text = "������ ��� �������. ��� � �����, ��� ���.";
			link.l1 = "��������, �����.";
			link.l1.go = "exit";
			AddCharacterExpToSkill(pchar, "Leadership", 30);
			//���� ������� ��� ��������� ������� ����
			characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "no";
		break;
		case "HostessChurch_bad_2":
			dialog.text = "��� �� ��� �������, ����������! �������� �� ����� � �� ����������� ��� �� �����!";
			link.l1 = "���? �� ����������, ����...";
			link.l1.go = "exit";
			AddCharacterExpToSkill(pchar, "Leadership", 40);
			npchar.quest.BadMeeting = lastspeak_date;
			//���� ������� ��� ��������� ������� ����
			characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "no";
		break;

	}
}
