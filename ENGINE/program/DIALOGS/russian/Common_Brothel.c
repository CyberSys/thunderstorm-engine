void ProcessDialogEvent()
{
	ref NPChar, sld, location;
	aref Link, NextDiag;
	string sTemp, str, str1;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Brothel\" + NPChar.City + "_Brothel.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
	switch(Dialog.CurrentNode)
	{
		// ============= ������� ������� =============
		case "First time":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("�����-�������, ����� �������?! �� ����� �������� ��������� �������, � �� � ������� ���������!", "��� ��������� ������ ����� �� ������ � ������� ����. � �� �������, ��������� ���� � ����� ������...", "������� ��������� �� ����� ���������, ���� ����� ������ ������!"), 
					LinkRandPhrase("�������� ������ ����������� �� ���� ������� - � � ���� ������ ����� �����!", "������� ������, ��� �� ����� ���������! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("��������� �� �����, ����...", "��������, �� �� ���� �����..."));
				link.l1.go = "exit";
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
			//-->> ����� ������������� � �������
			//����������
			if (npchar.questChurch == "baster")
			{
				dialog.text = "�� ���� ������, �� ������������� �� ������! ��������, ����� ������. � �� ���� ���� ����������� � ���� ���������. ����� ���!";
				link.l1 = "���-���, ���������!";
				link.l1.go = "exit";
				link.l2 = "��������, � �������, ��� �� �� ���� �������. � ������������� ������� ����� �����. �� � ���� ������ ���� ������, �, �������, �������� �� ���� � ����� ���������.";
				link.l2.go = "quests";//(���������� � ���� ������)
				break;
			}
			if (npchar.questChurch == "taken")
			{
				dialog.text = "�� ��� �� ����� ������ � �������. ������� ����, ������� � ���� - ������� ����! �� ������!";
				link.l1 = "��, � �����. � ��������� ����.";
				link.l1.go = "exit";
				break;
			}
			if (npchar.questChurch == "yes")
			{
				dialog.text = "�� ������ �������������?";
				link.l1 = "��, � ��� ������. ����� ������ �������� ����, ��� ���� ������ ������� ��������.";
				link.l1.go = "ResultChurch_1";
				break;
			}
			if (npchar.questChurch == "no")
			{
				dialog.text = "�� ������ �������������?";
				link.l1 = "������, " + npchar.name + ", � ������ �� ����� ���� ������. � ��������� ���� �� �������.";
				link.l1.go = "ResultChurch_2";
				break;
			}
			//-->> ���� ������ ������������� � �������
			if (rand(2) == 1 && pchar.questTemp.different == "free" && GetNpcQuestPastDayWOInit(npchar, "questChurch") > 45 && GetHour() < 20)
			{
				dialog.text = "��������, � ���� ��������� �� ����� ���������...";
				link.l5 = "������ �����������.";
				link.l5.go = "toChurch";
				//������ ����� ������� �� ������ ������
				npchar.questChurch = "taken";
				SaveCurrentNpcQuestDateParam(npchar, "questChurch");
				break;
			}
			//<<-- ����� ������������� � �������

			//-->> ���� ������� �� ����
			if (rand(3) == 1 && pchar.questTemp.different == "free" && GetNpcQuestPastDayWOInit(npchar, "questSex") > 180 && !CheckAttribute(npchar, "quest.selected") && !CheckAttribute(npchar, "quest.NotGoneToSex.over"))
			{
				if (!CheckAttribute(npchar, "quest.NotGoneToSex"))
				{
					dialog.text = "��, ��� ������ � ����� ���������. ����� ����, ����� ������ ������� ��� ��, ������� ��� �����?";
					link.l5 = "����������! � �������� �������� ����� ���������� ����.";
					link.l5.go = "toHostessSex";
					link.l8 = "� ���������, ��� �������, " + npchar.name + ". ���-������ � ������ ���...";
					link.l8.go = "exit";
					pchar.questTemp.different = "HostessSex";
					SetTimerFunction("SmallQuests_free", 0, 0, 1); //����������� ���������� �� ���������� 
					SaveCurrentNpcQuestDateParam(npchar, "questSex");
				}
				else
				{
					dialog.text = "� ������� ���, ����� � ����� ���� � ������� �������, �� �� ������... �� ������� ���� ����.";
					link.l1 = "��, ����...";
					link.l1.go = "exit";
					npchar.quest.NotGoneToSex.over = true; //������ ������ �� �������
				}
				break;
			}
			//<<-- ������ �� ����

			if (npchar.quest.meeting == "0")
			{
				dialog.text = RandPhraseSimple(TimeGreeting() + ". ����� ���������� � ��� ����������. ���� ����� " + npchar.name + ", � ����� ������� ����. ��� ���� ���� ��������, " + GetAddress_Form(NPChar) + "?",
				                               TimeGreeting() + ". ����������� ���, ����������, � ���� �������� ���������. ��������� ������������, � "+ NPChar.Name + " - ��������� ����� ������ ��������� ������ ������. ��� � ���� ���� ��� �������?");
				npchar.quest.meeting = "1";
			}
			else
			{
				dialog.text = RandPhraseSimple(TimeGreeting() + ". ��� � ���� ��� ������, " + GetAddress_Form(NPChar) + "?",
				                               TimeGreeting() + ". ��� � ���� ��� ��� �������, " + GetAddress_Form(NPChar) + "?");
			}
			link.l2 = npchar.name + ", � ���� �������� ����� � ����� �� ����� �������.";
			link.l2.go = "Hostess_1";
			link.l3 = "����� � �� ������� ����� ������. ����� ������� ������� ����� �� �������?";
			link.l3.go = "ForCrew";
			link.l4 = "� ���� ���� ������.";
			link.l4.go = "quests";//(���������� � ���� ������)
			//-->> ����� ������ ������ ����
			if (pchar.questTemp.different == "TakeMayorsRing" && pchar.questTemp.different.TakeMayorsRing.city == npchar.city && GetNpcQuestPastDayWOInit(npchar, "TakeMayorsRing") > 7)
			{
				link.l5 = "��������, " + npchar.name + ", � ��� ������ �����������. �� ��� ����� � ���� �������, � ������� ���.";
				link.l5.go = "TakeMayorsRing_H1";
				SaveCurrentNpcQuestDateParam(npchar, "TakeMayorsRing");
			}
			//<<-- ����� ������ ������ ����
			link.l9 = "�����. � ��� �����.";
			link.l9.go = "exit";
			NextDiag.TempNode = "First time";
		break;
        case "Hostess_1":
			if (!CheckAttribute(npchar, "quest.selected"))
			{
				if (CheckNPCQuestDate(npchar, "quest.date"))
				{
					dialog.text = "�� ������ ���� �������. ����� ���, �������, �� ��� ���������� ����-�� ��� ���� ��� �����?";
					Link.l1 = "���, ��� ����� ����� � ����������, � ��� ��� ����� - ���������. � ���� ��� ��� �����������...";
					Link.l1.go = "Hostess_NotChoice";	
					Link.l2 = "��, ���� ����, ��� ��� ������������...";
					Link.l2.go = "Hostess_Choice";
				}
				else
				{
					dialog.text = "������� � ��� �� ���� ���� ���� �������, ����� ���� � ���� ��� ���� �������. ������� ������, �������� ��� �������!";
					Link.l1 = "��, ����, � ������ ����� �� ����... �� �� �����.";
					Link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "��, �� ��� ������� �������. ��� ��� ������� �� ��������, ��� �������, � �� �������� ���� �� ��������.";
				Link.l1 = "������, ��� ���.";
				Link.l1.go = "exit";	
			}
		break;

        case "Hostess_NotChoice":
			sld = GetFreeHorseRef(npchar.city);
			location = &locations[FindLocation(npchar.city + "_Brothel")];
			if (sld.id == "none" || GetNpcQuestPastDayParam(location, "Brothel_date") > 98)
			{
				dialog.text = "������ � ���� ��� ��������� �������, ���� ����� ����� ����� ���� ����� ���� ����.";
				Link.l1 = "������, ��� �������.";
				Link.l1.go = "exit";	
			}
			else
			{
				dialog.text = "�� ���, �������, �����������! ���� ���������� ���������� ������� �� ����� " + GetFullName(sld) + ", ��� ������ ��������. ������ ��� ������������ ����� " + FindRussianMoneyString(sti(sld.quest.price)) + ". ��������?";
				Link.l1 = "���, �������, ��������. ����������...";
				Link.l1.go = "exit";
				if (sti(pchar.money) >= sti(sld.quest.price))
				{
					Link.l2 = "�������, ��������, ����� ����� ���� �������?!";
					Link.l2.go = "Hostess_NotChoice_agree";	
					npchar.quest.choiceIdx = sld.index;
				}
				else
				{
					Link.l1 = "����, ��� � ���� ������ ����� �����...";
					Link.l1.go = "exit";
				}
			}
		break;
        case "Hostess_NotChoice_agree":
			sld = &characters[sti(npchar.quest.choiceIdx)];
			if (sti(pchar.money) >= sti(sld.quest.price))
			{
				dialog.text = "�������, �������. " + sld.name + " ����� ����� ���� � ������� ��� ��������� �� ������ �����.";
				Link.l1 = "���, �� � �����...";
				Link.l1.go = "exit";
				AddMoneyToCharacter(pchar, -sti(sld.quest.price));
				sld.dialog.currentnode = "Horse_ReadyFack";			
				//--> ������ �� �������, ����� �� ����� �����
				str = npchar.city;
				pchar.quest.(str).win_condition.l1            = "Timer";
				pchar.quest.(str).win_condition.l1.date.day   = GetAddingDataDay(0, 0, 1);
				pchar.quest.(str).win_condition.l1.date.month = GetAddingDataMonth(0, 0, 1);
				pchar.quest.(str).win_condition.l1.date.year  = GetAddingDataYear(0, 0, 1);
				pchar.quest.(str).win_condition               = "Brothel_checkVisitTime";	
				pchar.quest.(str).HorseId = sld.id;
				pchar.quest.(str).locator = sld.location.locator;
				//<-- ������ �� �������, ����� �� ����� �����
				ChangeCharacterAddressGroup(sld, npchar.city + "_Brothel_room", "goto", "goto"+(rand(2)+1));
				LocatorReloadEnterDisable(npchar.city + "_Brothel", "reload2_back", false); //��������� �������
				npchar.quest.selected = true; //���� ���� ����� � �������
				SetNPCQuestDate(npchar, "quest.date"); //���� ������ �������
				for(int n=0; n<MAX_CHARACTERS; n++)
				{
					makeref(sld, Characters[n]);
					if (sld.location == npchar.city+"_Brothel" && CheckAttribute(sld, "CityType") && sld.CityType == "horse")
					{
						sld.dialog.currentnode = "Horse_4";
					}
				}
			}
			else
			{
				dialog.text = "��� �� ���� ������, �� ���� � ���, ��� " + sld.name + " - ������� ���������, ����� " + FindRussianMoneyString(sti(sld.quest.price)) + ". � � ����, ��������� � ����, ����� ����� ���. ������� ��� ������������, ������...";
				Link.l1 = "��, ��� ��� ������...";
				Link.l1.go = "exit";
			}
		break;

        case "Hostess_Choice":
			dialog.text = "� ������ ����, ����� ����� ��������� � ��������� ��������� ������ �������... ������ ��� �� ���.";
			Link.l1.edit = 9;
			Link.l1 = "";
			Link.l1.go = "Hostess_Choice_1";	
		break;
        case "Hostess_Choice_1":
			sld = CheckHorsesName(npchar.city, 9);
			if (sld.id == "none")
			{
				dialog.text = "��, �� ����������, � ���� ��� ����� ������� � ���������. ��������, �� ������� ��������� �� ���.";
				Link.l1 = "��, �� � ������ ��� � ��� ������������.";
				Link.l1.go = "Hostess_Choice_2";				
				Link.l2 = "�������, ��� ����� �������� �� ���. � ����� ���������� � ����� �� ��� ����.";
				Link.l2.go = "exit";	
			}
			else
			{
				dialog.text = GetFullName(sld) + ", �� � ��� ��������?";
				Link.l1 = "���, ������ � ���.";
				Link.l1.go = "Hostess_NotChoice_agree";				
				Link.l2 = "���, �� � ���.";
				Link.l2.go = "Hostess_Choice_2";
				npchar.quest.choiceIdx = sld.index;
			}
		break;
        case "Hostess_Choice_2":
			dialog.text = "����� ���� ����� ������� �� ��� ��� ���, �������� � �����, � ��� ���� ����.";
			Link.l1.edit = 9;
			Link.l1 = "";
			Link.l1.go = "Hostess_Choice_1";
			Link.l2 = "�������, ��� ����� �������� �� ���. � ����� ���������� � ����� �� ��� ����.";
			Link.l2.go = "exit";	
		break;
		//==> �������
		case "ForCrew":
			dialog.text = "���.. �� ��� �������? ������, ����� '�������� ���' � �����? ��� �, � ���� ���� ��������� ������ ����������������� �������, ��������� �� �����... � ���� " + FindRussianMoneyString(GetCrewQuantity(pchar)*30) + ".";
			link.l1 = "������, � �������� ��������� ��� ������.";
			link.l1.go = "ForCrew_1";
			link.l2 = "�����, ��� ��������� ���-������...";
			link.l2.go = "exit";
		break;
		
		case "ForCrew_1":
		    if (sti(Pchar.money) >= GetCrewQuantity(pchar)*30 && GetCrewQuantity(pchar)>0)
		    {
		        AddMoneyToCharacter(Pchar, -makeint(GetCrewQuantity(pchar)*30));

	            AddCrewMorale(Pchar, 10);
	            LAi_Fade("", "");
                AddTimeToCurrent(5 + rand(1), rand(30));
			    DialogExit();
		    }
		    else
		    {
		        dialog.text = "���, �������, �� ��� ����, �� ��� �������, ��� ���� ������� ����� ���������� �� �������, � ����� ������ � �� ������.";
			    link.l1 = "�������, ��...";
			    link.l1.go = "exit";
		    }
		break;
		case "Woman_FackYou":
			dialog.text = "�����, �� ���� ��� �����?! ���� ��, � ������� ���������� ���������.\n�� � ������ ��� ������ ���� �� ����, ���������. �������� ���� ��������...";
			link.l1 = "��������, ����...";
			link.l1.go = "exit_setOwner";
			LAi_group_Attack(NPChar, Pchar);
			if (rand(3) != 1) SetNationRelation2MainCharacter(sti(npchar.nation), RELATION_ENEMY);
		break;
		//������� � ������������� � ������
		case "toChurch":
			dialog.text = "���������, � ������� ����� ���������, �������... ��, �� ������ �������, ��� ��.";
			link.l1 = "� �������.";
			link.l1.go = "toChurch_1";
		break;
		case "toChurch_1":
			dialog.text = "������... �� ��� �� ������, ��� � ���� ����� � �� �����. � �����, � ���� ������� ������������� ������. �� �������� ��� ��� ������ - ����� �������, �������������� �������...";
			link.l1 = "����. � ������ �����������, �� ����������.";
			link.l1.go = "toChurch_2";
		break;
		case "toChurch_2":
			pchar.questTemp.different.HostessChurch.money = (rand(4)+1) * 1000;
			dialog.text = "������. � ��� ���� " + FindRussianMoneyString(sti(pchar.questTemp.different.HostessChurch.money)) +  ". ������ ������������� ����� �������!";
			link.l1 = "��� ������ ����� ������, ������ �� ����������.";
			link.l1.go = "toChurch_3";
			link.l2 = "�� ������, � ������ ��������� �����. ����� ������� �� ���������. ������.";
			link.l2.go = "toChurch_4";
		break;
		case "toChurch_3":
			dialog.text = "�������! ������� ����� �� ���, ����������, ��� ��� ������...";
			link.l1 = "������.";
			link.l1.go = "exit";
			pchar.questTemp.different.HostessChurch.city = npchar.city; //�����
			pchar.questTemp.different = "HostessChurch_toChurch";
			SetTimerFunction("HostessChurch_null", 0, 0, 1); //����������� ���������� �� ���������� � ������ ���������
			AddMoneyToCharacter(pchar, sti(pchar.questTemp.different.HostessChurch.money));
			ChangeCharacterReputation(pchar, 0.30);
		break;
		case "toChurch_4":
			dialog.text = "����, ����� ����...";
			link.l1 = "���-������ � ������ ���.";
			link.l1.go = "exit";
			pchar.questTemp.different = "free";
			DeleteAttribute(pchar, "questTemp.different.HostessChurch");
			npchar.questChurch = ""; //����� ������ ���� ������
			ChangeCharacterReputation(pchar, -1);
		break;

		case "ResultChurch_1":
			dialog.text = "����� �������, ��� ���� ��� �� ��� �������� � ���� �����!.. ������� ����.";
			link.l1 = "�� �� �� ���. ���� ����, " + npchar.name + ", � ����� �� ���...";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, 3);
			npchar.questChurch = ""; //����� ������ ���� ������
		break;
		case "ResultChurch_2":
			dialog.text = "��� �� �����?! ��, ��, �� ���� ��������� ������� ����! � � ��� ���������...";
			link.l1 = "������, �� ��� �����...";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, -2);
			npchar.questChurch = ""; //����� ������ ���� ������
			AddMoneyToCharacter(pchar, -sti(pchar.questTemp.different.HostessChurch.money));
		break;
		//������ ������ �����������
		case "TakeMayorsRing_H1":
			dialog.text = "�������� ������ � �� ��������.";
			link.l1 = "� ������� ����?";
			link.l1.go = "TakeMayorsRing_H2";
		break;
		case "TakeMayorsRing_H2":
			dialog.text = "������� ����. ���� ������ ���-�� �������� ��� ������, �� ��� ��� �������� ��� �����������. ����� ������ ����������� ��� �� ��������.";
			link.l1 = "�������... � ����� ���-�� ���-���� ����� ��� ����������?";
			link.l1.go = "TakeMayorsRing_H3";
		break;
		case "TakeMayorsRing_H3":
			dialog.text = "��� ���� ��. �������� ����������� ��������� ���� ������� ��������, � ������.";
			link.l1 = "�������... ������� ����, " + npchar.name + ".";
			link.l1.go = "exit";
		break;
		//------- ������ �� ���� ------------
		case "toHostessSex":
			dialog.text = "�� ����� ���. ��� ��� �� ���� ����������?";
			link.l1 = "���?! ��... � ��� ���! � ���� ����� �������... �-�-�... ���������... ����� �������. ���.";
			link.l1.go = "toHostessSex_1";
			link.l2 = "� ���� ������ ���� � ������� ����, � ���...";
			link.l2.go = "toHostessSex_2";
			link.l3 = "�����, � ������� ������� �� �� ���� �����...";
			link.l3.go = "toHostessSex_3";
		break;
		case "toHostessSex_1":
			dialog.text = "���� '���������' ��� �� ���������. ������...";
			link.l1 = "��� �� ���?..";
			link.l1.go = "exit";
		break;
		case "toHostessSex_2":
			if (rand(1))
			{
				dialog.text = "��������! ����� ���������������, ������ ���� ��� ��������� ���� ��������?! �������� ������������� �������, � ����� ������!";
				link.l1 = "�! �������. ��� �����! ������, ���� ���� ���...";
				link.l1.go = "SpeakHistory";
			}
			else
			{
				dialog.text = "��������! ����� ���������������, ������ ���� ��� ��������� ���� ��������?! �� ���������� �� ������, ��� �������� �������...";
				link.l1 = "��, ���� ����� ���������� ��� ��� ���������.";
				link.l1.go = "SpeakHistoryTwo";
			}
		break;
		case "toHostessSex_3":
			dialog.text = "��������� � ������� �����-������ �������� � ������. � ��� ���������� �����?! �-��, ���� ���������� ��������� �������?..";
			link.l1 = "� �� ��� ���� �����!";
			link.l1.go = "exit";
		break;
		//�������
		case "SpeakHistory":
			dialog.text = "����������� ������...";
			if (rand(1))
			{
				link.l1 = "���� � ���� ���� �������, ��� ���� ���� �� ����� �����. ������ ���������?";
				link.l1.go = "SpeakHistory_1";
			}
			else
			{
				link.l1 = "���-�� ����� �� ������ ��������, ������� ������, ������ � ����, � ���� ��������� � �����! ������ � �������!!! ������� ������, ��� � ����! ��-��! � ��� � ���� �� ����� ���� ������� �������, ����� ������, ����������, �� �� ���������� ��������...";
				link.l1.go = "SpeakHistory_2";
			}
		break;
		case "SpeakHistory_1":
			dialog.text = "�-�! �� ���� �������� �����! ��� �� ���� ������� ��������?!";
			link.l1 = "��, ����� �� �������. ������ �� ���-�� � ������ �����, �� �������, � ���� ���������� �����. ������ �� ����������, ���� ��������� �������. ��� ����� �����������, ������ � ����, �������� ����� ���������� ���� � ����. � ��� � ������� ��������� � ��������, ��� �� ������� ������ ����������: '" + GetAddress_Form(PChar) + ", ����������! �����! �����!!!'. � ��� � �� ������! � ��� ��� ������ � ������� �� ������ ��� �����. �� � ���������!!! � ���� ��� ������ ����! � ���� ������ ��� ��������������� � ���� ����� �� ����!..";
			link.l1.go = "SpeakHistory_3";
		break;
		case "SpeakHistory_2":
			dialog.text = "������! � ���� ��������! �� ������, �������������� ���! �������� � ���� ����!";
			link.l1 = "��, �� ���?..";
			link.l1.go = "exit";
		break;
		case "SpeakHistory_3":
			dialog.text = "�, ����! � ��� ��?!";
			link.l1 = "� �������, ��� ��� �������� � ���� �����, �������������� � �����, ��� ���� ���� � ������, ��� ������ �������� ���������� �������� �����, �� ������ ������� ����� ��������... �� ������ ���������� ����� ���� �����, � �� ���� ������ ��� ��������� �������� �������...";
			link.l1.go = "SpeakHistory_4";
		break;
		case "SpeakHistory_4":
			dialog.text = "� ��?!..";
			link.l1 = "���, �� �. ����. ��� ������� ����. ������, ��� � �� ������� ������� �� ������, � ����� ��� �����.";
			link.l1.go = "SpeakHistory_5";
		break;
		case "SpeakHistory_5":
			dialog.text = "� �� �� �������! ������?! ���� �� ������� ����� �������?";
			link.l1 = "��, ������� �������! ��� ���� �������� ���� ������ ������ - � ������� ������ ��������!";
			link.l1.go = "SpeakHistory_6";
			link.l2 = "���, �� �������. ��� ��, ����� ����� �������� ����� ���������! � ������ ��-�������...";
			link.l2.go = "SpeakHistory_7";
		break;
		case "SpeakHistory_6":
			dialog.text = "��-��... �� ���������� ����! �������� � ����, ������� � ���� ��� ������ ����?.. � ���� �� ����, �� ����� �� �������, ��� � ���! �� ���� �� �������������...";
			link.l1 = "������!! ����, ������...";
			link.l1.go = "exit";
		break;
		case "SpeakHistory_7":
			dialog.text = "� ��� ��?! ��� �� �� ������?";
			link.l1 = "� ��� ����� �������� �����, � �����������, ��� ������ ������������� �� ���� ��������, ������ �������� �����, �������. � ��� ������ �� ����������, ����� ��� ������ ����� ��� ���� � �������, ��� ����� �������� ������ ������ ��������! � �������������, ����� ������� ������, � ���� �� �������, ���������� ��� �������� ��� �������, ��� � ������� �� �����������! � ���� �������, ����� ����� ���������� �� ����� � ��������� ����������� �� ����, � ��� ��� � ������ � ������ ������ �� ��� ���� � �������... �� ������ � ��� �������, � ���� �������� ��������� ������.";
			link.l1.go = "SpeakHistory_8";
		break;
		case "SpeakHistory_8":
			dialog.text = "�-�! �����!.. ������ ���...";
			link.l1 = "������ ������! ������ � ���� ����� ������, �����!..";
			link.l1.go = "SpeakHistory_9";
			link.l2 = "��������? �����?! ��, �, �������, �����, ��������, �� ������������ ��� ����, � �������, ���� ����� ����� �����������... � �� ���� ���� �����������������.";
			link.l2.go = "SpeakHistory_10";
		break;
		case "SpeakHistory_9":
			dialog.text = "��, ���! ������� �� ������, ��� ����� ���� � �����! �� ���� ��������!";
			link.l1 = "��, ������, �� ��� ��?..";
			link.l1.go = "exit";
		break;
		case "SpeakHistory_10":
			dialog.text = "��, ��� �� ����������! � ���� �� ��������� ������, �������� ������ ��������� �������. ��������, �� ������������� ������� ��������� �... ������������, �� � ��� ���� ����� ��������...  � �����, ��� ���-���� ��������� ���������� �� ���� �����, �������� ������, � �������, ��� ��� ����� �� �����������... � ���, ��������, �� ���� �������� ���-������ ����������...";
			link.l1 = "��, ��� ��� �������!..";
			link.l1.go = "exit";
			pchar.questTemp.different.HostessSex = "toRoom";
			pchar.questTemp.different.HostessSex.city = npchar.city;
			AddDialogExitQuestFunction("SexWithHostess_goToRoom");
			pchar.quest.SmallQuests_free.over = "yes"; 
			SetTimerFunction("SexWithHostess_null", 0, 0, 1); //������� ������� �� �����, ���� �� �� ������� � �������
		break;

		case "Hostess_inSexRoom":
			dialog.text = "��, ��� � ��...";
			link.l1 = "��� �����, ��� ��������!";
			link.l1.go = "exit";
			pchar.quest.SexWithHostess_null.over = "yes"; //����� ������ �� �� ������
			NextDiag.TempNode = "First time";
			AddDialogExitQuestFunction("SexWithHostess_fack");
		break;

		case "SpeakHistoryTwo":
			dialog.text = "��������?";
			link.l1 = "��������, ��� ��. ��������, ������������� ����!";
			link.l1.go = "SpeakHistoryTwo_1";
		break;
		case "SpeakHistoryTwo_1":
			dialog.text = "���������, �� ������ �� ����...";
			link.l1 = "� ����� ����! ������ � �����, ��� ��� �����! ��� ������ � ���� ������, ����� �����, �� ������ ����� ����! ����� �������������� ������� � ��� �� ��������! ����� �����, ���� ���!..";
			link.l1.go = "SpeakHistoryTwo_2";
			link.l2 = "� ���� ���������.";
			link.l2.go = "SpeakHistoryTwo_3";
		break;
		case "SpeakHistoryTwo_2":
			dialog.text = "�����, ��� ��� ����� �� ��� ����. �� ������� ���� ����, ���������� �� ���...";
			link.l1 = "������, ��� �� ���?!";
			link.l1.go = "exit";
		break;
		case "SpeakHistoryTwo_3":
			dialog.text = "��, ����� � ���� ��������...";
			link.l1 = "� ������� ������ ������, � ������ ������ �����, ���, ���, �� ��� ���� ����� ���������, ����� �������� ��� ��. � ��� ���� � ����...";
			link.l1.go = "SpeakHistoryTwo_4";
		break;
		case "SpeakHistoryTwo_4":
			dialog.text = "������ �� ��� ������������?";
			link.l1 = "��� ���� ���� ����� ������ � ����!";
			link.l1.go = "SpeakHistoryTwo_5";
		break;
		case "SpeakHistoryTwo_5":
			dialog.text = "��� ���� ��?";
			link.l1 = "����� ���������� ���� ������, ��� ��������!";
			link.l1.go = "SpeakHistoryTwo_6";
			link.l2 = "��� ������������ ���� �������, ���� ������ � ������� ��� ������. ��� ��� ���� ������ � �����, � ��� ����� ��������� ��� �������, ������ ���� �����, ������� �������� ������ � � ����� ��������� ������, ������� ����� �� ��������� �������������� �����...";
			link.l2.go = "SpeakHistoryTwo_7";
		break;
		case "SpeakHistoryTwo_6":
			dialog.text = "��������! �� ������� ����. �� ������� ���� ����.";
			link.l1 = "����, �� ����������...";
			link.l1.go = "exit";
		break;
		case "SpeakHistoryTwo_7":
			dialog.text = "�-�!.. � �� ��������... � ������!";
			link.l1 = "������? ������! � ������ ������. � ������, ���, �������������. �� ��� ������, ��� ��� ����� ����������� ��� ���� ���� ����� ��������������, ���������� ��������...";
			link.l1.go = "SpeakHistoryTwo_8";
		break;
		case "SpeakHistoryTwo_8":
			dialog.text = "���-�-�?!!";
			link.l1 = "...������� ����.";
			link.l1.go = "SpeakHistoryTwo_9";
		break;
		case "SpeakHistoryTwo_9":
			dialog.text = "�! ���-�� � �� � ��� ��������. ���������, ����� ����.";
			link.l1 = "�� ������ ������, � ���� ����� �� ������ � ����� ������...";
			link.l1.go = "SpeakHistoryTwo_10";
			link.l2 = "��� �� ����� �������� � ���-�� ������? �������, � ���� ����� ������ �����.";
			link.l2.go = "SpeakHistoryTwo_15";
		break;
		case "SpeakHistoryTwo_10":
			dialog.text = "��? � � ��� �� ���? ���� ����� ������������.";
			link.l1 = "� ����� ���������� �����, � � ����� ���������� ��������, � � ���� ������������� �������, ��� ������ � �������, ����� ��������� ���� ����!";
			link.l1.go = "SpeakHistoryTwo_11";
		break;
		case "SpeakHistoryTwo_11":
			dialog.text = "������!..";
			link.l1 = "���� �����... � ��, ��������, ��������� ���� �����, ��... ����� �� ���� ���������� ������, � ����������� ����� � ���, ��� ��� ���. ��, ������, � �������...";
			link.l1.go = "SpeakHistoryTwo_12";
		break;
		case "SpeakHistoryTwo_12":
			dialog.text = "������ � ����... �...";
			link.l1 = "���?";
			link.l1.go = "SpeakHistoryTwo_13";
		break;
		case "SpeakHistoryTwo_13":
			dialog.text = "�������������� ����...";
			link.l1 = "��� � ������?";
			link.l1.go = "SpeakHistoryTwo_14";
		break;
		case "SpeakHistoryTwo_14":
			dialog.text = "���� �����... ����� ������� �����, ����� ������� ������, ��� ����������, ��� � ���������... ��������.";
			link.l1 = "� ���������� �������������!";
			link.l1.go = "exit";
			pchar.questTemp.different.HostessSex = "toRoom";
			pchar.questTemp.different.HostessSex.city = npchar.city;
			AddDialogExitQuestFunction("SexWithHostess_goToRoom");
			pchar.quest.SmallQuests_free.over = "yes"; 
			SetTimerFunction("SexWithHostess_null", 0, 0, 1); //������� ������� �� �����, ���� �� �� ������� � �������
		break;

		case "SpeakHistoryTwo_15":
			dialog.text = "��? � � ������, ��� ��������� ���� �������, � �� ���������� ��������. ������ �����! � ���� ���, � ���������� ������ �� �� ���?!";
			link.l1 = "� �� �� ���� � ����...";
			link.l1.go = "SpeakHistoryTwo_16";
		break;
		case "SpeakHistoryTwo_16":
			dialog.text = "����, ��� �� ����� � ����! �������, � ������� �� ������! ��� ���!!! ����������! �� ��� ��������!";
			link.l1 = "�� �� ��� " + npchar.name + ", � � ������ �� ����!..";
			link.l1.go = "exit";
		break;
		// ================================== ������� ������ =======================================
        case "Horse_talk":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("��� ����� ���� �� ���������!", "��� ��������� ������ ����� �� ������! ��� ����� ����...", "����������� - � � ���?! ��� ��, � ������ ���..."), 
					LinkRandPhrase("��������!!", "������� ������, ��� ������! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("���, �� � ���� �� ��...", "��������, �� �� ���� �����..."));
				link.l1.go = "exit";
				break;
			}
			dialog.text = NPCStringReactionRepeat("������������, " + GetAddress_Form(NPChar) + ". ��� ����� ���������� � ������� ���������, ��� ������ �������� ����� ���.", 
				"��, ��� ����� ��. ��������, �� ��� ����� ������� ������� ��� ������� � ��������. ����� ��� ������ � ���.", 
				"����������, " + GetAddress_Form(NPChar) + ", � ����� �������, ��� �� ��� �� ������ � � ������ �����, � �� ������ � ����������... ��� ��� ����� ��� ������ � ������� ���������.", 
				"��, " + GetAddress_Form(NPChar) + ", ����� �� ��... �����������! ��� ����� ������ � ������� ��������� ��� ���������� ������.", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("������, ���������, � �����.", "�-�, ��, �������...",
                      "������ �� �����������, ���������, � ����� � �����, ��� ���!", "��� ������, ���-�� �������... ������, �����.", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			Link.l2 = "� ��� ��� ���������?";
			Link.l2.go = "Horse_1";
			Link.l3 = LinkRandPhrase("����, ���������, � ����� ����� ����� �������...", "������, � ����� �� ����� ����� �������� �������!", "��, �����, �� ���������� ���������.");
			if (!CheckAttribute(npchar, "quest.choice"))
			{
				Link.l3.go = "Horse_2";
			}
			else
			{
				Link.l3.go = "HorseChoice_" + npchar.quest.choice;
			}
			//-->> ����� ������ ������ ����
			if (pchar.questTemp.different == "TakeMayorsRing" && pchar.questTemp.different.TakeMayorsRing.city == npchar.city && GetNpcQuestPastDayWOInit(npchar, "TakeMayorsRing") > 7)
			{
				link.l5 = LinkRandPhrase("������, ���������, �� �� �������� ����� ������������ ������? ������� ��� ��� ���� �������...", 
					"��������, ��, �������, �� �������� � ����� ��������� ������������ ������?", 
					"��������, ���� ���, ������� �� ���������� ���� ���, �����������?");
				link.l5.go = "TakeMayorsRing_S1";
				SaveCurrentNpcQuestDateParam(npchar, "TakeMayorsRing");
			}
			//<<-- ����� ������ ������ ����
            //--> ����. �������, ����� �8. ����� � ������� � �������.
			if (pchar.questTemp.State == "SeekBible_toFFBrothel" || pchar.questTemp.State == "SeekBible_toGirl")
            {
    			if (npchar.id == "HorseGen_"+FindLocation("FortFrance_Brothel")+"_1")
				{
					if (!CheckAttribute(npchar, "quest.HolLineTalk"))
					{
						dialog.text = "���, ��� �������� ����� �������� �����, ����� ���������...";
    					link.l4 = "��������, ��������� ����� ����� � ��� ��� ������� � ��������� ������������...";
    					link.l4.go = "Step_Hol8_1";
					}
					else
					{
						dialog.text = "�������� ������?";
						link.l4 = "��� ���� ������, ������ ���������� ���, ��� ������.";
    					link.l4.go = "Step_Hol8_5";
						AddMoneyToCharacter(pchar, -1000);
					}
				}
            }
			//<-- ����. �������, ����� �8. ����� � ������� � �������.
			NextDiag.TempNode = "Horse_talk";
		break;
        case "Horse_1":
			dialog.text = "��� � ����� ��������, �� ������ ������ � ��� ������ ����� �����, ��������������� ������ �� �����, ���� ����� ����� � ������ ������� ����. �� ����� " + characters[GetCharacterIndex(npchar.city + "_Hostess")].name + ".";
			Link.l1 = "�������, �����, �������.";
			Link.l1.go = "exit";			
		break;
        case "Horse_2":
			if (rand(1))
			{
				dialog.text = LinkRandPhrase("��, ���� ���, �� ���� ������ ������� �����! ������, � ������ ��������, ��� ��� ���� �� �������� ����, �� �� ���������...", "�� ������������� ��� ��������? ��� ����� � ����� �������... ��������, � ������ ��������, ��� ��� �� ������ ������� ����. � ����� ���� ���� ����� � ����� �����...", "��� ���?! ��, �� �����, � ��������, �� ����� ���������� ������� � ���� ����� ����� �����... ������, � �� ������ � ������ ������, ��� ��� � ��������� ���� ������� ����. ������, ��� �� ���������� �������...");
				link.l1 = "���, ������ ���� � �������!";
				Link.l1.go = "Horse_3";		
				Link.l2 = "��-�-��, ��� ��� ������ ���������� ���������� ����.";
				Link.l2.go = "exit";
				npchar.quest.choice = 0; //��� �����, �� �� ����� ���������
			}
			else
			{
				dialog.text = LinkRandPhrase("������ ��� � ���� �����, ����� ���? �� ����� ��� ������ ����������. ������� �����!", "�� ���� �����������, ��� ��� ������� ����� �����? ��, ��� ����� ���... ���� ������ ���� - ����� ������� ���������, � �������� ��� �� �����.", "��� ���� �������� �������������... ����� - � � ���� �����, �� ������ ��� ����� ������� �����!");
				link.l1 = "���, ����� ������!";
				Link.l1.go = "exit";
				npchar.quest.choice = 2; //�� �������
			}
		break;
        case "Horse_3":
			dialog.text = characters[GetCharacterIndex(npchar.city + "_Hostess")].name + " ��� ��������� � ����� ��������. ��� � ���, ��� �����, � ������ ��� ��� - " + npchar.name + ". � ���� ���� �����...";
			Link.l1 = "�������, �����, ����� ����...";
			Link.l1.go = "exit";
			npchar.quest.choice = 1; //��� ���������
			SetNPCQuestDate(npchar, "quest.choice");
		break;
        case "Horse_4": 
			dialog.text = NPCStringReactionRepeat("�� ��� ������� �������, ��� ���� ����, ���������� ������.", 
				"� �� ���� ������� - ���������� � ���.", 
				"������ �����������, ��� ��� �������� - ���������� � ���...", 
				"���������� ��������, �� �������...", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��, � ����.", "� �����.",
                      "������ �� ���������, � ��� � �����.", "��, � ��� ��� ��?..", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			NextDiag.TempNode = "Horse_4";
		break;
		//===>> ������� �� ������� ������������, ���� ����� ��� ���
        case "HorseChoice_0": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				dialog.text = NPCStringReactionRepeat("���-�� � ���� �� �����. �� ����������� �������, �� ����� �� �������� �����. �������� ���...", 
					"����� �� ����� �������������? ��, ��� � �������, � ��� ��� �����.", 
					"��, � �� ������� ����? ������� �� - ��� � ������� ���������.", 
					"��� ������ ����� ���������� ��������, �� �� �����������, �����...", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("��� �����, ������ ��...", "������, � ��� � ������.",
						"��-��, � �����...", "��, ������, ��������.", npchar, Dialog.CurrentNode);
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "������� �� ����������. ���� �� ������ ����� ����, �� ��� � ������� ���������. ��� ��� ������...";
				Link.l1 = "�������.";
				Link.l1.go = "exit";
			}
		break;
        case "HorseChoice_1": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				if (!CheckNPCQuestDate(npchar, "quest.choice"))
				{
					dialog.text = NPCStringReactionRepeat("�����, �� ��� ���������. �� ��������� ���� ����� �����...", 
						"��, ��������, �����, ��� ����� ������� ��� ���� �����, �� ���� ��� � �� ���� �������...", 
						"��������, ����� �� ���-���� ������� ��� � ��������?..", 
						"��, ���� �� ����, ��� � �������...", "block", 1, npchar, Dialog.CurrentNode);
					link.l1 = HeroStringReactionRepeat("�� �� �� ��� �� �����!", "�����������!",
							"��, �������!", "����, ���� � �������...", npchar, Dialog.CurrentNode);
					link.l1.go = "exit";
				}
				else
				{
					dialog.text = "� ���� �����, � �� ��� � �� ���� ����! �� ���� �������, ��� � � �������� �� �����...";
					Link.l1 = "��, ��� ����������...";
					Link.l1.go = "exit";
					npchar.quest.choice = 0; //��� �����, �� �� ����� ���������
				}
			}
			else
			{
				dialog.text = "��-�, ��� ����� ��, ��� ������� ������! ���� ���� ����� ����� �, �� ��� � ������� - �� ���������... ������, � ���� ������, �������, ������ �� ��������� ���������� �� ���� �������� ����������, �� �� ������...";
				Link.l1 = "������� ���� ������ ����-����, ���������, � �� ����� � ����� ���������.";
				Link.l1.go = "exit";
				Link.l2 = "��� ����� �������, ��� �� ���� ���������, �� ������ �������� � ����� ���� �, ���������, �� ����.";
				Link.l2.go = "HorseChoice_1_Add";
			}
		break;
        case "HorseChoice_2": 
			if (!CheckAttribute(npchar, "quest.sexHappend"))
			{
				dialog.text = NPCStringReactionRepeat("��� ���, ����� ����, �������-�� ����� �����. ��� �������� �����, ��� ������ ������.", 
					"��, �������� ��, ��������� ���� � ����...", 
					"��������, ����� ������ ���?!", 
					"��, �� ���� ��, ������ ������, ����� ������ ������� ���������. ������ �� ���� ��������� - ��� � ������� ���������, ��������!", "block", 1, npchar, Dialog.CurrentNode);
				link.l1 = HeroStringReactionRepeat("��, ������...", "��, ��� ��� ��� � �������...",
						"��, ����� ������, � ����� � ���...", "���������� � ����������, ����...", npchar, Dialog.CurrentNode);
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "�-�-�, ��� ����� ��! � ����� � ������� ���������� ������, ������ �� �������� � ���� ����... ���� ������ ��������� �� ���� - ����� ������� ���������, � ����� ���� ��� �����������...";
				Link.l1 = "���, ��� �� ����������...";
				Link.l1.go = "exit";
			}
		break;
        case "HorseChoice_1_Add":
			dialog.text = "��, �� �����, ��� ���� ��� ����������... ����� ����.";
			Link.l1 = "������...";
			Link.l1.go = "exit";
			npchar.quest.choice = 0;
		break;
		//===>> ����
        case "Horse_ReadyFack":
			if (!CheckAttribute(npchar, "quest.choice")) npchar.quest.choice = 0;
			switch(npchar.quest.choice)
			{
				case "0":
					dialog.text = RandPhraseSimple("����� ���� ������ ���� � ������� ���������. � ��� �� ������ ��������?", "�������, �� ���������, �������� ���� ��� ���� �� ��������� ��� ����.");
					Link.l1 = RandPhraseSimple("������� �� �����, �������...", "����� �����������, �����!");
				break;
				case "1":
					dialog.text = "��, ��� � ��, ��� ������� ������! � ���-��� ������� ���� � ������ �������� ���� �����...";
					Link.l1 = "��, ��� ��������� ������...";	
				break;
				case "2":
					dialog.text = "�-�-�, ������, �������-��. ��, �� ����� ������ �������!";
					Link.l1 = "�� �����, �����...";
				break;
			}
			Link.l1.go = "exit";
			//--> ���-�� ���������
			if (!CheckAttribute(npchar, "quest.sexHappend")) npchar.quest.sexHappend = 1;
			else npchar.quest.sexHappend = sti(npchar.quest.sexHappend) + 1;
			//<-- ���-�� ���������
			str = npchar.city;
			pchar.quest.(str).win_condition.l1 = "ExitFromLocation";
			pchar.quest.(str).win_condition.l1.location = pchar.location;
			pchar.quest.(str).win_condition = "Brothel_checkVisitTime";
			pchar.questTemp.HorseQty = sti(pchar.questTemp.HorseQty) + 1; //�������
			NextDiag.TempNode = "Horse_AfterSex";
			AddDialogExitQuest("PlaySex_1");
			
			/// ��� �� �������
			if (CheckNPCQuestDate(pchar, "BrothelSex"))
			{
				AddCharacterHealth(pchar, 5);
				SetNPCQuestDate(pchar, "BrothelSex");
			}
			// ��������� ��������
			ChangeCharacterReputation(pchar, -1);
			AddCharacterExpToSkill(pchar, "Leadership", 15);
			AddCharacterExpToSkill(pchar, "Fencing", -15);
			AddCharacterExpToSkill(pchar, "Pistol", -15);
		break;

        case "Horse_AfterSex":
			switch(npchar.quest.choice)
			{
				case "0":
					dialog.text = LinkRandPhrase("���� �����������?", "��, ��� �������? ��� � �������?", "��� ����, ������, ���������?");
					Link.l1 = RandPhraseSimple("�������, ��� � �������.", "��� � �����, ������.");
					Link.l1.go = "exit";
				break;
				case "1":
					if (sti(npchar.quest.sexHappend) > (rand(4)+5) && sti(pchar.questTemp.HorseLoot) < 3 && !CheckAttribute(npchar, "questTemp.HorseLoot"))
					{
						dialog.text = "��������, �� ������ �� ��� ��� �����, � ��� � ���� �����������...";
						Link.l1 = "�� � � � ���� ����, ���� ��...";	
						Link.l1.go = "HorseQuest";
					}
					else
					{
						dialog.text = LinkRandPhrase("�� ���, � ��������� ��������?", "��, ��� � ����, �����������?", "� �������, �� �������, ������ ��� � �-�-����� ��������...");
						Link.l1 = RandPhraseSimple("��, ��� ��� ����� �����������.", "������� �������������, �� ���� �� ������!");	
						Link.l1.go = "exit";
					}
				break;
				case "2":
					dialog.text = RandPhraseSimple("�� ���, ���� ����.", "����� �����, �������, ���� ���� �� �����.");
					Link.l1 = RandPhraseSimple("���, ����...", "�� �������� � ������� ����...");
					Link.l1.go = "exit";
				break;
			}
			NextDiag.TempNode = "Horse_AfterSex_2";
		break;
        case "Horse_AfterSex_2":
			dialog.text = LinkRandPhrase("������ � ��� ��� ���-������...", "�� �������� � �� ����� ������...", "����� ���� ������ ���� � ��� ���...");
			Link.l1 = "�����...";
			Link.l1.go = "exit";
			NextDiag.TempNode = "Horse_AfterSex_2";
		break;
		//==> ����� �� ���
        case "HorseQuest":
			pchar.questTemp.HorseLoot = sti(pchar.questTemp.HorseLoot) + 1;
			npchar.questTemp.HorseLoot = true; //��� ������
			dialog.text = "������ ���, � ���� ���-��� ����������, ��������, ��� ������� ����...";
			Link.l1 = "��, ������, ��������.";
			Link.l1.go = "HorseQuest_1";
		break;
        case "HorseQuest_1":
			dialog.text = "������� ���� ��� ������ ��� ����� ����. ��� ���, ������ �� ���-�� ��� ���� �� ��������� �����, ������ ���������...";
			Link.l1 = "�����, ������ ������ ��������.";
			Link.l1.go = "HorseQuest_2";
		break;
        case "HorseQuest_2":
			dialog.text = "����� ���� � ���. ���� �� �����, �� ������ �� ������ �����, ��� ��� ���� ���������� ������ ��� ����.";
			Link.l1 = "� � ���������� ��������������� �� ������ �� �������?";
			Link.l1.go = "HorseQuest_3";
		break;
        case "HorseQuest_3":
			sTemp = LinkRandPhrase("Shore55", "Shore9", "Shore_ship2");							
			if (sTemp == "Shore55") 
			{
				str = "������ ���-���� ���� �����";
				str1 = "box" + (rand(1)+1);
			}
			if (sTemp == "Shore9") 
			{
				str = "����� �������";
				str1 = "box1";
			}
			if (sTemp == "Shore_ship2") 
			{
				str = "������ ������, ��� ����� � �������";
				str1 = "box2";
			}
         	pchar.GenQuestBox.(sTemp) = true;
			switch (pchar.questTemp.HorseLoot)
			{
				case "1": 
					pchar.GenQuestBox.(sTemp).(str1).items.spyglass2 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry1 = 4;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry5 = 6;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry10 = 2;
					pchar.GenQuestBox.(sTemp).(str1).items.indian11 = 1;
					pchar.GenQuestBox.(sTemp).(str1).money = 15000;
				break;
				case "2": 
					pchar.GenQuestBox.(sTemp).(str1).items.spyglass3 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry14 = 3;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry16 = 2;
					pchar.GenQuestBox.(sTemp).(str1).items.jewelry18 = 4;
					pchar.GenQuestBox.(sTemp).(str1).items.indian17 = 1;
					pchar.GenQuestBox.(sTemp).(str1).money = 19000;
				break;
				case "3": 
					pchar.GenQuestBox.(sTemp).(str1).items.indian20 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.indian5 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.indian10 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.spyglass4 = 1;
					pchar.GenQuestBox.(sTemp).(str1).items.indian19 = 1;
					pchar.GenQuestBox.(sTemp).(str1).money = 24000;
				break;
			}
			dialog.text = "�� ���-�� ������ � " + str + ".";
			Link.l1 = "��� �, ������� ���� �� ����������. ���� �������� ���-������, �� ������� ��� �����.";
			Link.l1.go = "exit";
		break;

		case "exit_setOwner":
			LAi_SetOwnerTypeNoGroup(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		//������ ������ ����
		case "TakeMayorsRing_S1":
			if (CheckAttribute(pchar, "questTemp.different.TakeMayorsRing.item")) //���� �������� � ������
			{
				dialog.text = LinkRandPhrase("���, �������, �� ����������, � ���������. ���� �� ������, �� �� ����...", 
					"���, ���������, ������ �� ������...", 
					"� ���������, ���. �������� ������ �� ������.");
				link.l1 = "����... �� ��� ��, ������� ����.";
				link.l1.go = "exit";
			}
			else
			{
				if (npchar.id == "HorseGen_"+reload_location_index+"_1")
				{
					dialog.text = "�� �� ������ �� ����������� �� ������ �����?";
					link.l1 = "������, ������!";
					link.l1.go = "TakeMayorsRing_S2";
				}
				else
				{
					dialog.text = LinkRandPhrase("���, �������, �� ����������, � ���������. ���� �� ������, �� �� ����...", 
						"���, ���������, ������ �� ������...", 
						"� ���������, ���. �������� ������ �� ������.");
					link.l1 = "����... �� ��� ��, ������� ����.";
					link.l1.go = "exit";
				}
			}
		break;

		case "TakeMayorsRing_S2":
			dialog.text = "������, �� ������ ��� ��������! ��� ��� ���������� ��� � �� �������.";
			link.l1 = "��������?! ���?";
			link.l1.go = "TakeMayorsRing_S3";
		break;
		case "TakeMayorsRing_S3":
			dialog.text = "����� ������������, �������!";
			link.l1 = "��� �� ��� ������ ��������, ����� ������. �� ������ ������ �� ������.";
			link.l1.go = "TakeMayorsRing_S4";
		break;
		case "TakeMayorsRing_S4":
			dialog.text = "�� � � �� ����� ��� ���? ���� �� ��� �� ��� �� � ����, �� ��� ��� ��������!";
			link.l1 = "��������, ��������, ��� �� ����������. �� ����� ���� � ��� ���������? ������ ���� �����������, ���� ���������... �����, �� ����� ���� ������� ��-�� ����� ������.";
			link.l1.go = "TakeMayorsRing_S5";
		break;
		case "TakeMayorsRing_S5":
			if (rand(1) && sti(pchar.money)>5000)
			{
				dialog.text = "������ �����, ������ �� ��������� ��� 5 ����� ��������.";
				link.l1 = "������, ������� ���� ������ � ���� ������.";
				link.l1.go = "TakeMayorsRing_S6";
			}
			else
			{
				dialog.text = "��, �� �����. ����� ��������!..";
				link.l1 = "�������������, ��������!";
				link.l1.go = "exit";
				GiveItem2Character(pchar, "MayorsRing");
				pchar.questTemp.different.TakeMayorsRing = "HorseTakenRing";
				AddQuestRecord("SeekMayorsRing", "3");
				AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
				AddQuestUserData("SeekMayorsRing", "sName", GetFullName(npchar));
			}
		break;
		case "TakeMayorsRing_S6":
			dialog.text = "����������...";
			link.l1 = "��, �������. ������� �� ������, ��������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -5000);
			GiveItem2Character(pchar, "MayorsRing");
			pchar.questTemp.different.TakeMayorsRing = "HorseTakenRingMoney";
			AddQuestRecord("SeekMayorsRing", "2");
			AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData("SeekMayorsRing", "sName", GetFullName(npchar));
		break;
//********************************* ������� ����� �� ������� �� **********************************
  		case "Step_Hol8_1":
			dialog.text = "� ���?";
			link.l1 = "������ �, ���� �� ������� ����� �����-�� ��������� � �������, ���� ������...";
			link.l1.go = "Step_Hol8_2";
		break;
  		case "Step_Hol8_2":
			dialog.text = "���� �����, �����... ��������, ���������, ����� ���������� ������ � � ������ �� ��� ���� �������. ���� ��� ����� ������ ����� 1000 �����...";
			link.l1 = "��� ����� ������ ������ ���-���...";
			link.l1.go = "Step_Hol8_3";
		break;
  		case "Step_Hol8_3":
			dialog.text = "������, ������ �� ����� ���� ������. �� ������ ������� - ����� �� ����� ��� ����������.";
			link.l1 = "����, � ���� ������� ���...";
			link.l1.go = "Step_Hol8_4";
		break;
  		case "Step_Hol8_4":
			dialog.text = "��, ������� ������� ����� ���������!!!";
            if (sti(pchar.money) >= 1000)
            {
    			link.l1 = "��� ���� ������, �� ������ ��� ������ ����� � ������.";
    			link.l1.go = "Step_Hol8_5";
                AddMoneyToCharacter(pchar, -1000);
            }
            else
            {
    			link.l1 = "������ � �� ���������, ����� ������ ���... ����� � ����, ��� ��������� 1000...";
    			link.l1.go = "exit";
				npchar.quest.HolLineTalk = true;
            }
			NextDiag.TempNode = "Horse_talk";
		break;
		case "Step_Hol8_5":
			dialog.text = "����� �� ���� ������� �����, ��� ����� ������� ���������� �����, ����� �� ������.";
			link.l1 = "��, � ���?";
			link.l1.go = "Step_Hol8_6";
		break;
		case "Step_Hol8_6":
			dialog.text = "��������� ��� �� � �����. �� ������� �������� ������, � ��� ���������� � ������, ��� �� �����. �������� ��� �� ���������� �����, ����� �� ���� ���� �������� �������, ��� �� ��������.";
			link.l1 = "��, ��� ��� ���������, �� � ����� �� �� ����������. ��� � ���� ����� ����� �������?";
			link.l1.go = "Step_Hol8_7";
    	break;
		case "Step_Hol8_7":
			dialog.text = "��� �������� �� �������, ��� � �������. ��� � ��� �������, ����� ��� ����� �� ����, �� ��������� ����������, ���������.";
			link.l1 = "�������, ���������� ����.";
			link.l1.go = "Step_Hol8_8";
    	break;
		case "Step_Hol8_8":
			dialog.text = "������, � ���, ������������� ��� ����� ����� �����?";
			link.l1 = "������ �������, �������� ����. � ��� �� ���� �������? ��� �����, ��� ����� � �����?";
			link.l1.go = "Step_Hol8_9";
    	break;
		case "Step_Hol8_9":
			dialog.text = "�� ������ ����������, ��� � �� ������ ����� ������. �� �� ���� ����, �� �� ������ �� �����...";
			link.l1 = "����. ������� ����, ����� ������ �� �����.";
			link.l1.go = "exit";
			AddQuestRecord("Hol_Line_8_SeekBible", "4");
			pchar.questTemp.State = "SeekBible_toTavern";
			NextDiag.TempNode = "Horse_talk";
    	break;
	}
}


ref GetFreeHorseRef(string City)
{
	ref rCharacter;
    int storeArray[8];
	int howStore = 0;
	for(int n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (rCharacter.location == City+"_Brothel" && CheckAttribute(rCharacter, "CityType") && rCharacter.CityType == "horse")
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return &NullCharacter;
	return &characters[storeArray[rand(howStore-1)]];
}

ref CheckHorsesName(string City, int num)
{
	ref rCharacter;
	string sTemp, sSeeked;
	sSeeked = GetStrSmallRegister(dialogEditStrings[num]);
	for(int n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (rCharacter.location == City+"_Brothel" && CheckAttribute(rCharacter, "CityType") && rCharacter.CityType == "horse")
		{
			sTemp = GetStrSmallRegister(rCharacter.lastname + " " + rCharacter.name);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;			
			sTemp = GetStrSmallRegister(rCharacter.lastname);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;
			sTemp = GetStrSmallRegister(rCharacter.name);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
				return rCharacter;
		}
	}
	return &NullCharacter;
}
