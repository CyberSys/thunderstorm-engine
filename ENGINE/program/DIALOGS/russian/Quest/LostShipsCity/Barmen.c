
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	string NodeName = Dialog.CurrentNode;
	string NodePrevName = "";
	if (CheckAttribute(NextDiag, "PrevNode")) NodePrevName = NextDiag.PrevNode;
	
	switch(Dialog.CurrentNode)
	{
		case "First time":
			NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = "� �� ����� � ����� �������������! �� ����� �� ���� ���������!";
				link.l1 = "��� ������..."
				link.l1.go = "exit";
				break;
			}

			if(NPChar.quest.meeting == "0")
			{
				NPChar.quest.meeting = "1";
				if (CheckAttribute(loadedLocation, "storm"))
				{
					dialog.text = "��� ���� ������� �������� ���������? �� ��� ��, ����� ����������! � ����� ������� �������, ����� ���� " + GetFullName(npchar) + ". ���� ������� �������, ��������!!";
					link.l1 = "�� ��, ������� ������ ��������!.. ���� ����� " + GetFullName(pchar) + ". � ��� ���������...";
					link.l1.go = "FS_1";
				}
				else				
				{
					dialog.Text = "�-�-�, ��� ��� ���� ������� �������� ���������? �� ��� ��, ����� ����������! � ����� ������� �������, ����� ���� " + GetFullName(npchar) + ". ��� ��� ������, ����� ���...";
					Link.l1 = "������, " + npchar.name + ". ������� �������������, ��� �������... � ���� ����� " + GetFullName(pchar) + ".";
					Link.l1.go = "First time";
				}
			}
			else
			{
				//����� � ������ �������
				if (pchar.questTemp.LSC == "toWineCellarGone" && npchar.location.locator == "goto10")
				{
					dialog.Text = "�-�, ��� � ��!";
					link.l4 = "������! ������ �������� �� ��������� ����.";
					link.l4.go = "TizerCellar";
					PChar.quest.LSC_returnBarmen.over = "yes"; //������� ������ �� ���������, �������� ���������
					break;
				}			
				//==> ������ ������
				dialog.Text = LinkRandPhrase("��, " + GetAddress_Form(NPChar) + " " + PChar.name + "! " + TimeGreeting() + ".",
                                    "�-�-�, ��� ������ ������ ���������� ������ ������...",
                                    "��� ���� ������ � ���� � �������. ����� ���������?");
				Link.l1 = "� �������, �� ������� �������� �� ���� ��������.";
				Link.l1.go = "int_quests";
				link.l2 = "��� ����� ������������.";
				link.l2.go = "room";
				Link.l3 = "���, � ��� �����, " + NPChar.name + ". �� �������.";
				Link.l3.go = "exit";
				//==>> ������
				if (pchar.questTemp.LSC == "AfterAdmiral" && GetQuestPastDayParam("questTemp.LSC") > 3)
				{
					link.l4 = "�������, �� �������������� � �����, ��� �� ����������� ������� �� ������ ������ ����...";
					link.l4.go = "Tizer";
				}
				if (pchar.questTemp.LSC == "toWineCellarLate")
				{
					link.l4 = "��������, � ��� � �� ����� ���� � ���� ����� ������ �������...";
					link.l4.go = "TizerCellarLate";
				}
				if (pchar.questTemp.LSC == "toDiffindoor" && CheckCharacterItem(pchar, "letter_LSC"))
				{
					link.l4 = "� ���������� '���������' � ����� ��� ������ ������. � ���, ����� �������, ���� ��� � ����� ���������� ������...";
					link.l4.go = "TizerCellarOk_1";
				}
				if (pchar.questTemp.LSC == "afterFightInProtector")
				{
					link.l4 = "��������, ��� ����� ����...";
					link.l4.go = "AfterProtector_1";
				}
				if (pchar.questTemp.LSC == "toAdmNarvalRing" && CheckCharacterItem(pchar, "DOjeronRing"))
				{
					link.l4 = "� ������ ��� �� ������. ������ '��������' � ����.";
					link.l4.go = "NarvalRing_1";
				}
			}
		break;

		case "FS_1":
			dialog.text = "�� �����������, �������, ������ ���� ����� �� ���������. ���� �� �� ������� � ������ ����� ���� ����� - �� �� �� �� ��� �� ����� ��������� �� ����.";
			link.l1 = "� �����, ������ ������...";
			link.l1.go = "FS_2";
		break;
		case "FS_2":
			dialog.text = "��� ������ �� ������, ����� ���� ���� ����� ������ ������! ������ ����� ����� �� �����, � ������ ��� ���� ���� ����.";
			link.l1 = "�������...";
			link.l1.go = "exit";
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		//������ �����
		case "room":
   			if (chrDisableReloadToLocation) //���-�� ������ ������� � ��.
			{
				dialog.text = "� �����, ��� ���� �� ����� ������ �����. ������ ���...";
				link.l1 = "�����.";
				link.l1.go = "exit";
				break;
			}
			dialog.text = "�� ������ ��������� �� �������, ��� ��������� �� ������� �����. ����� ���� ���������?";
			if(!isDay())
			{
				link.l1 = "�����.";
				link.l1.go = "hall_day_wait";
			}
			else
			{
				link.l1 = "� ����.";
				link.l1.go = "hall_night_wait";
				link.l2 = "��������� �����.";
				link.l2.go = "hall_day_wait";
			}
			link.l3 = "���������. �������� ��� ���.";
			link.l3.go = "exit";
		break;
		case "hall_night_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			TavernWaitDate_LSC("wait_night");
		break;
		case "hall_day_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			TavernWaitDate_LSC("wait_day");
		break;
		//�������
		case "int_quests":
			dialog.text = "����������� ���� ������, �������.";
			if (!CheckAttribute(NextDiag, NodeName+".l5"))
			{
				link.l5 = "������� � ���, �������, ������ �����������!!";
				link.l5.go = "ansewer_5";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l1"))
			{
				link.l1 = "��������, ��� ��� �� ������?";
				link.l1.go = "ansewer_1";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l2"))
			{
				link.l2 = "��� ����������, ��� � ����� ����?";
				link.l2.go = "ansewer_2";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l3"))
			{
				link.l3 = "� ���, ������ ������������� ���������� ���������? ������� �������� ������, ������� ������ ��������� ���� �����?";
				link.l3.go = "ansewer_3";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l4"))
			{
				link.l4 = "��� �� ����� ������, ���� ������?..";
				link.l4.go = "ansewer_4";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l6"))
			{
				link.l6 = "��������, �� ���� ������ �������� - ������ ����?";
				link.l6.go = "ansewer_6";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l7") && pchar.questTemp.LSC == "AdmiralIsWaiting")
			{
				link.l7 = "��������, �� �� ������, ��� ����� ���������?";
				link.l7.go = "ansewer_7";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l8"))
			{
				link.l8 = "��� �� ����� ����� � ������?";
				link.l8.go = "ansewer_8";
			}
			link.l10 = "��� ��������. ������, ��������...";
			link.l10.go = "exit";
		break;
		case "ansewer_5":
			dialog.text = "��� ��� ������! ������ � ������. ������ ����� - ������� ����.";
			link.l1 = "�������...";
			link.l1.go = "ansewer_5_1";
			NextDiag.(NodePrevName).l5 = true;
		break;
		case "ansewer_5_1":
			dialog.text = "���� ����� ���������� ������ ����� � ������. � ��� �������� �� �������� ���������� � ��������, ��� ��� ���� �� ��������� ������ �� �� ������� � ����, �� �� ������ ���� ���.";
			link.l1 = "�������?";
			link.l1.go = "ansewer_5_2";
		break;
		case "ansewer_5_2":
			dialog.text = "��� ������, ��������. �� ���� � ���� �����, ��� ���� �����!";
			link.l1 = "����.";
			link.l1.go = "exit";
		break;
		case "ansewer_1":
			dialog.text = "��� ����� ���������� ��������, ��������. ���� ����� ����, �� ������ ��� ����, ��� ��� ������. ������ �� ���������, � ���������...";
			link.l1 = "������!!";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l1 = true;
		break;
		case "ansewer_2":
			dialog.text = "�� ����, ��� ���� ���� ��������, ��� �� ���� ������. �� �� �������, ����� �������� ��� � ��������. ��� � ������ �� ����, ����� ���� � �����, ����� �����, ����� ����� �������... ��� � ���.";
			link.l1 = "� ������ �� �����?";
			link.l1.go = "ansewer_2_1";
			NextDiag.(NodePrevName).l2 = true;
		break;
		case "ansewer_2_1":
			dialog.text = "����� ��� ������, �������, ���� ������� ��������, ��� �������. �� ��� ��� ��� ��������� ���� ������ - ������ �� ����. ����� ��. ������-��, ���� ��� ��������� ����� ��������, ��� ��� ���� �������.";
			link.l1 = "�� ��...";
			link.l1.go = "exit";
		break;
		case "ansewer_3":
			dialog.text = "��!! � ������ �� ����� ����� ������?.. ���� �������, �� ��� ���������. ����������, ��� ���� ����� ��������� � �������� ������ ��. �� ����� ����� �������� ��������, ��� ��� ��� �������� ��� ������������. ���� ����������� �����-������ �������, �� ������ �������� ���������, ��� �������� ��������, ����� ������ �� �������\n� �����, ��� ����� �������. ������� ������ ��� ��������� ����� �������. � ��� �����!";
			link.l1 = "�������...";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l3 = true;
		break;
		case "ansewer_4":
			dialog.text = "�� ������ ���������. ����, ������, ������� ��� �� ������... � ������, ����� ����� �� ��������� ������. � ��� ��������� ���� ����� ������ � �������� �������.";
			link.l1 = "��� ��� �� �������?";
			link.l1.go = "ansewer_4_1";
			NextDiag.(NodePrevName).l4 = true;
		break;
		case "ansewer_4_1":
			dialog.text = "���, ��������, ��� ���������� ����� �������� ��������������, ��� ������ ��� �����. �������, ��� ������ �� ��� �������, ��� ��� �������� ��� �� ����� ��� ����.";
			link.l1 = "����� � �� ����������... � ���, � ��� ������������� �������� ������, � �������� �������� ��������?";
			link.l1.go = "ansewer_4_2";
		break;
		case "ansewer_4_2":
			dialog.text = "��, �����-�� ������ ����, �� ������ ��� ��� �� ���������. ��� ������ ����������� �� ������ �������������, ����� �� ����� ������ ������. ��� ����� �� �����...";
			link.l1 = "��� ������ '���'?";
			link.l1.go = "ansewer_4_3";
		break;
		case "ansewer_4_3":
			dialog.text = "�� '���' ���� ��������, ������� ������ ����� ���������. �� ������ �� ��� � ����.";
			link.l1 = "������ ������, ��� ��?";
			link.l1.go = "ansewer_4_4";
		break;
		case "ansewer_4_4":
			dialog.text = "��, ����� � ��� �������. �� ��� ������...";
			link.l1 = "�������.";
			link.l1.go = "exit";
		break;
		case "ansewer_6":
			dialog.text = "����. �� ��� � ��� ��������� �������, ����� �����. � ����, ������� ��� �����.";
			link.l1 = "� ��� � ���!";
			link.l1.go = "ansewer_6_1";
			NextDiag.(NodePrevName).l6 = true;
		break;
		case "ansewer_6_1":
			dialog.text = "���, ��������� ��� �����?";
			link.l1 = "�� � �� �� ����! ������, ��� �� �����-�� ������� �������� �� ������� �����...";
			link.l1.go = "ansewer_6_2";
		break;
		case "ansewer_6_2":
			dialog.text = "������!!! �� ����� ����!..";
			link.l1 = "�����. ������, ���, � �����, �� �������� ��� �������, �� ���� �����...";
			link.l1.go = "ansewer_6_3";
		break;
		case "ansewer_6_3":
			dialog.text = "����! ����! ����!! ������� ��� ��������?..";
			link.l1 = "�� ��� � ���?";
			link.l1.go = "ansewer_6_4";
		break;
		case "ansewer_6_4":
			dialog.text = "������ ���, ��������, ��� ��� � ���� �����: ������ �� ������ �� ����! ���� ������� ������, ��� ���-�� ������� �����, �� ��� � ���� � ��������... � �����, ����� �������� �� ������������.";
			link.l1 = "��, �� �������...";
			link.l1.go = "ansewer_6_5";
		break;
		case "ansewer_6_5":
			dialog.text = "������� �����. � ����, ���� ������ �� �����, ����� ���, ��� � ������. �� �������� ��� ������! �����?";
			link.l1 = "�����.";
			link.l1.go = "ansewer_6_6";
		break;
		case "ansewer_6_6":
			dialog.text = "������. � � ���� ��������� ������� �� ���� ����� �������...";
			link.l1 = "�� �����, ���������.";
			link.l1.go = "exit";
			AddQuestRecord("ISS_MainLine", "4");
			SaveCurrentQuestDateParam("questTemp.LSC"); //�������� ���� ������
		break;
		case "ansewer_7":
			dialog.text = "��� ����� '�������� ���������', ������ �����.";
			link.l1 = "���� ����, ������...";
			link.l1.go = "ansewer_7_1";
			NextDiag.(NodePrevName).l7 = true;
		break;
		case "ansewer_7_1":
			dialog.text = "����� ���� �� ���� ������ ��� ����� �� ��� - �� ��������� �������� �������. ������, ���-�� ��������� �� ����������, ���������, ������, �� ����. � �����, ������� ���� ��������� �����, ����� ��� �� ����� � ���� �� �����.";
			link.l1 = "������?";
			link.l1.go = "ansewer_7_2";
		break;
		case "ansewer_7_2":
			dialog.text = "����� �������, ����� �������� ������� �� ����, ����������� �� ��������� ������. ��� ��� ���� ���� �����������, ����� ��� ��������� �����.";
			link.l1 = "�������...";
			link.l1.go = "exit";
		break;
		case "ansewer_8":
			dialog.text = "���� ����� � ��� � ������. ������, ������ ����� ��������� ���� �����, ��� ���� �� ����� ��������. ��� ��� ��� � ���� - �����...";
			link.l1 = "��, ��� ������?";
			link.l1.go = "ansewer_8_1";
			NextDiag.(NodePrevName).l8 = true;
		break;
		case "ansewer_8_1":
			dialog.text = "��, � ��� ����� ���������� ����������, ������... � �����, ������������ ��������, ������� ��������.";
			link.l1 = "�� ��� ���, � ���� � ������� ��������? � ���� �����!";
			link.l1.go = "ansewer_8_2";
		break;
		case "ansewer_8_2":
			dialog.text = "���, �� ��������� �����, �� - ��������� �������. ���, � ���� ������ � ������� ���� ���� �������� ���� ������?";
			link.l1 = "��, �������, ������ ������.";
			link.l1.go = "ansewer_8_3";
		break;
		case "ansewer_8_3":
			dialog.text = "��� ��� � � ���� �����... � �����, ������ �� ��� �������, ����� ��� � ���� ��� ���������� � ��������� ��������� �� ������. ������� �� ����� �������� �� ������ � ����, ����� ����� ��� �� ������ ������.";
			link.l1 = "�� � ���, ��������� ������ � ����?";
			link.l1.go = "ansewer_8_4";
		break;
		case "ansewer_8_4":
			dialog.text = "����, �������, �� ������ ��� ����. �� ���� �����, ��� ���� ������ �� �������� �������� �� �����!";
			link.l1 = "�������. �� ��� ��, ������� �� ����������.";
			link.l1.go = "exit";
		break;
		// ------------------ ������ ------------------
		//������ �������� ������ � ������ �������
		case "Tizer":
			dialog.text = "��-��, � ����������. ������ �������� �� ���� ����� �� ����. ����� ���������� � ���� � ������ ������� ����� ��������.";
			link.l1 = "� ��� ��������� ���� ���� ������ ������?";
			link.l1.go = "Tizer_1";
		break;
		case "Tizer_1":
			dialog.text = "��, �������� � ������� ��� �� ��������, � ������ ��� ���� �� ���� �����. ����� ����� �� �������� ������ '�������' � � ��� ����� � ����������.";
			link.l1 = "�������. �� ��� ��, ������������.";
			link.l1.go = "Tizer_2";
		break;
		case "Tizer_2":
			dialog.text = "�� ������������, ����� ���� ����� � �� �����.";
			link.l1 = "�������. ���� �����������.";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "toWineCellar";
			AddQuestRecord("ISS_MainLine", "5");
		break;
		//������ ��������, ��� �� �� ������ � ������
		case "TizerCellarLate":
			dialog.text = "��� ��� ��������, ���� � ���� ���� ��� ����� ���! ���� ������, ������ �� ���� ����� �������������� � ����� ��������� ����!";
			link.l1 = "� �����-����� ���������?";
			link.l1.go = "TizerCellarLate_1";
		break;
		case "TizerCellarLate_1":
			dialog.text = "��� ��� ������, ��� � �� ���� �� ���� ���� ������� � ����� ��������! ��� �������!";
			link.l1 = "�����, �� ��������. ����� ��� ��� ��������� �����������, ��� ��� ��� ����������.";
			link.l1.go = "TizerCellarLate_2";
		break;
		case "TizerCellarLate_2":
			dialog.text = "�� ������. ��� ���� ��� ��, � �������, ������� ����� ��������. �� �� ��� ��� ������ �������.";
			link.l1 = "������, ���� �����������.";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "toWineCellar";
		break;
		//����� � �������� � �������
		case "TizerCellar":
			dialog.text = "�� ������ �� ������� � ������ ���� �� ��� �����?";
			link.l1 = "���, ��� �� � ��������������.";
			link.l1.go = "TizerCellar_1";
			LAi_CharacterDisableDialog(npchar);
			pchar.quest.LSC_returnBarmen_2.win_condition.l1 = "ExitFromLocation";
			pchar.quest.LSC_returnBarmen_2.win_condition.l1.location = pchar.location;
			pchar.quest.LSC_returnBarmen_2.function = "LSC_returnBarmen_2";
		break;
		case "TizerCellar_1":
			dialog.text = "��� ������. ����, ����� � ������ ���� ���������� ����� ����������� �� ����� ������.";
			link.l1 = "� ������������� ��������.";
			link.l1.go = "TizerCellar_2";
		break;
		case "TizerCellar_2":
			dialog.text = "����, ��, ��������, ��� � ����� ����, ��� ����� ������ ��������� ������� ���-����, �� ��� ����� ������ ������. ��� ���, � ������ ��� �� �������, ��� ��� ����� ���� ��������� ������� ����� ������, �� ��� � ������� �� ��� ��������� �� ������� �����.";
			link.l1 = "� ���������, � ����� � ������� ��� ��������� �� ��������, �����, ��� ��������, � ����� �� ��� ����������� ����� ����.";
			link.l1.go = "TizerCellar_3";
		break;
		case "TizerCellar_3":
			dialog.text = "��, ����... ��, ����� �������� �� ��, ��� ������. � ����� �� �������, ������� ����� ��������� ��� ����� �������. ������, ��� ���-�� ������, ���������� � ������� ��������, ��� ����� ���� ��������� ������.";
			link.l1 = "� ������ ���?";
			link.l1.go = "TizerCellar_4";
		break;
		case "TizerCellar_4":
			dialog.text = "���� �������� ������, ��� ����� �������� �� ������� �����, ��, ��� ��������, ������ ������� ����������� ��� �������. ������ ������, ��� ���-�� �������� � ������ ����, ����� ����� �������������� ����� �����������.";
			link.l1 = "�� ���� �...";
			link.l1.go = "TizerCellar_5";
		break;
		case "TizerCellar_5":
			dialog.text = "������ ������ ����� ����, ��������\n� ��� ��� �����. �� ������ ����������� �� ����� '�������� ���������'. ����� ��� ������� ������������ - ������ ������� ������� � ��������� ��� ��������� �� �����������. ������ ������ ��� ����� ��� �������� ����� ����� �������. �������, ��� ������ ��� �� ������ ����� ���� � ����� ������.";
			link.l1 = "��� ���� ���������?";
			link.l1.go = "TizerCellar_6";
		break;
		case "TizerCellar_6":
			dialog.text = "������, ����� �� ��������� � ������ ���������������. ���������� � '����������' ��������� �������� ����� '����� �����' � ���� '��� ��������'. ��� ��, �� ������ ����� ����� ����� �����������, �� ������� ��������� � ����.\n� ���� ��������� �� '��� ��������', �� ������ ������. ���� ���� - ������������� ����� '������'!";
			link.l1 = "����. ��������, �� ������ ���� �������� �������, ��� �� ���� �� '����������'?";
			link.l1.go = "TizerCellar_7";
		break;
		case "TizerCellar_7":
			dialog.text = "���� �� � ����! ������� ����, ��� �� ���� �������������� ����, ��� ���������� ����� ��� ����� ��������� �� ������� �����.";
			if (CheckCharacterItem(pchar, "letter_LSC"))
			{
				link.l1 = "��, �� ������, �� '����������' � ��� �������. � ����� ��� ������ ������. � ���, ����� �������, ���� ��� � ����� ���������� ������...";
				link.l1.go = "TizerCellarOk_1";
			}
			else
			{
				link.l1 = "�� ������, � ���� ���� ����, ������� ���� ������ ����� ��� �� ������� �����. ���� ���� ����� ��� ������� ������� ����, ��� ���������� �� ������ ������� ��������������, �� ���� ������ ���������� ��������.";
				link.l1.go = "TizerCellar_8";
			}
		break;
		case "TizerCellar_8":
			dialog.text = "���������, ����� ���������... ������, ���, ����� ��, ������ �� ����������. ������ ��������� ����, � ����� ����� �� ������ ��������� ������� ��� ����������� �����������. � ����� ��� � ����� ������. ����? �� ������ � ������ �����, ����������� ������ ������ ������ �� ����.";
			link.l1 = "��, ���� �����.";
			link.l1.go = "TizerCellar_9";
		break;
		case "TizerCellar_9":
			dialog.text = "������, ���� - ��� ��� ���. � �������� ������ ������� ��� ����?";
			link.l1 = "��� ��� ���� '���������'.";
			link.l1.go = "TizerCellar_10";
		break;
		case "TizerCellar_10":
			dialog.text = "�����! �� �� ������ ����!\n����������� � '����������'. �������, ��� ��� �� ������� �������, � �������� ������ ���� � ��������. �����, ���� � ������� ��� ��������� � ���������, ��� �� ����� ��� ������ �� �����, ���� �� �����...";
			link.l1 = "�-��, ����� ����. �� ��, ��� �� ��� ��������� ������� - ��� ����.";
			link.l1.go = "TizerCellar_11";
		break;
		case "TizerCellar_11":
			dialog.text = "��������! ��, �� ����� ������ �������. � ��� ���� � ������������ ������������ ������.";
			link.l1 = "����� ����, �������. ������.";
			link.l1.go = "exit";
			AddQuestRecord("ISS_MainLine", "6");
			pchar.questTemp.LSC = "toDiffindoor"; //���� ������� ���
		break;

		case "TizerCellarOk_1":
			dialog.text = "��� ��� �� ������?";
			link.l1 = "������ ���. ���, �����.";
			link.l1.go = "TizerCellarOk_2";
			TakeItemFromCharacter(pchar, "letter_LSC");
		break;
		case "TizerCellarOk_2":
			dialog.text = "���-���, ����� ���������... �� �����: '...��� ��� �� ����� � ����� �������� ����������.' ������, �� ��� �� ����...";
			link.l1 = "��, ���� �� �����, �� ����.";
			link.l1.go = "TizerCellarOk_3";
		break;
		case "TizerCellarOk_3":
			dialog.text = "��-��, ��� ��������! � ������� ���� ������, �� �� �� ��� �� ����� �� ����� �� ��������� � �������� ����� - ���� � ���� �� �� ���� ����� �����. �� ��� �� �����-���� ��������� �� ������� �����, �� ������ ��� ��������, ��� ���� ����, �������� ������������ ����������� ����\n��! �� ���� ��� ��� �������, ����� �����!! ��� ���, �������, ����������� � ����. �������� � ��� ���� � �����, �������� � ������ ����. � �� ���� �� � �� �����, ��� ���� ������� ���-���� �������� �� ������� �����!";
			link.l1 = "� ��� ����������? ���� ������ ������ ��� �������� ��������������� ���� ����.";
			link.l1.go = "TizerCellarOk_4";
		break;
		case "TizerCellarOk_4":
			dialog.text = "��� �����, ��, ��� �� �����, ���������� ���������, ��� ����� ��������� ���������� ����� ����. � �� �����, ��� ��� ����� ��������� ������ ��������� ������, �� ���� ����������� ���������� ������.";
			link.l1 = "������? ��� ������� ������ �� ��������� ��������, ��� ����� � �������� ����. ��� � �����, ��� �� ��� �������.";
			link.l1.go = "TizerCellarOk_5";
		break;
		case "TizerCellarOk_5":
			dialog.text = "�� ��� ��� ������, �������. �� ������ ���� ����� ������� � ����, � �� ����� ������ ����� ��� ��� ��������. � ������� ��������� ������ �� ����� �� ������ ������� �����, � ��������� ������ ������� ��� ����!";
			link.l1 = "��, �������, �� ����...";
			link.l1.go = "TizerCellarOk_6";
		break;
		case "TizerCellarOk_6":
			dialog.text = "� ��� ����� �� ��������, ��� ������� ���� �� ����� ������, ������� ���� � ���� ������ � �������������. ��������� ������ ��������� ������, ������� ������ �������� � ��������� �� �����������, - ������� ����. �� ������ ������� �������� � ����� �������, � ������� ��� ����� ��������, ��� ��� �������.";
			link.l1 = "������ ��������?";
			link.l1.go = "TizerCellarOk_7";
		break;
		case "TizerCellarOk_7":
			dialog.text = "��, ��� ���� �������... ������ � ����� ���� �� �����, ��� ������� �� �����-�� ������ �������� ������.";
			link.l1 = "��, ���������...";
			link.l1.go = "TizerCellarOk_8";
		break;
		case "TizerCellarOk_8":
			dialog.text = "� �����, ��� ��� ��� �������, � ��� �� �������� �������. � ������ ����� ����� ������ � ��������� ������ ��� � ��� ������� � ���� ������.";
			link.l1 = "������, � ��� � ������.";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "toAndreLabor"; //���� ������� ���
			AddQuestRecord("ISS_MainLine", "7");
			pchar.questTemp.LSC.checkBoxes = true; //������� �� �������� �������� ����������
		break;

		case "AfterProtector_1":
			dialog.text = "��� ���������?";
			link.l1 = "��... ��, � �����, ��������� � � ���������, ���� ����� �������, � �� �������� ���� ������ � ����� ������� '���������'. � ���� �����, ��, � ��� ���, ������ ������ �� ����� ����... � ���� ����� ������ ������������ �� �������� �� ����! �������� ���� ��������� �� ��� ����.";
			link.l1.go = "AfterProtector_2";
		break;
		case "AfterProtector_2":
			dialog.text = "������!! ��� ���� ��! ��, ������ ���� �����... ������, ���� ��� ����, �� �����������, ��� ��� � ���� ��������. ���� ��� ��� ����� �����, � � ��� ������ ����...";
			link.l1 = "���� ���� ��� �������! ����� ���� �� �����, ���� ����� � �� �����, ��� ��� ������ ������ �. ��� �� ������ ���������� ��� ��� ��������.";
			link.l1.go = "AfterProtector_3";
		break;
		case "AfterProtector_3":
			dialog.text = "�� � ���� ������?";
			link.l1 = "��� ��� ��� �������.";
			link.l1.go = "AfterProtector_4";
		break;
		case "AfterProtector_4":
			dialog.text = "��� �����, ���� ������������� ������ �� ����. ������� ������ �� �����, �� ���� ��������.";
			link.l1 = "� ��� �� �����������?";
			link.l1.go = "AfterProtector_5";
		break;
		case "AfterProtector_5":
			dialog.text = "��, �����, ��� ���� ��� ������� �� ���� �� ������.";
			link.l1 = "�� �����?";
			link.l1.go = "AfterProtector_6";
		break;
		case "AfterProtector_6":
			dialog.text = "�� ���� '������', ��� �������� �����.";
			link.l1 = "� ��� ��� �������? � �� �� ���� ������ � ���� � �������: '��� �� �, ��� ��� '�������'!'.";
			link.l1.go = "AfterProtector_7";
		break;
		case "AfterProtector_7":
			dialog.text = "� ���� ������ ����� '������' ���� ������ � ��������������� ������������ �������. ������� ������ � ��� � ��� � ��������. ���������� ��� � ���, ��� ����� ��� �� ����� �������� ����������� � ��������.";
			link.l1 = "�� � ��� ��� ����� ��� ������?";
			link.l1.go = "AfterProtector_8";
		break;
		case "AfterProtector_8":
			dialog.text = "� ������� ����� ������� '�������' ���� �����, � ������� '�������' ������ ������ ������. ��� � ��� ���-�� ����� ��������. ��� ����-������ �� ��� ���, � ����� '������' �������� � ���� ���.";
			link.l1 = "�������. �� ��� ��, ��� � ��������.";
			link.l1.go = "exit";
			AddQuestRecord("ISS_MainLine", "10");
			pchar.questTemp.LSC = "toKillOneNarval";
			pchar.quest.LSC_KillOneNarval.win_condition.l1 = "locator";
			pchar.quest.LSC_KillOneNarval.win_condition.l1.location = "ProtectorFisher";
			pchar.quest.LSC_KillOneNarval.win_condition.l1.locator_group = "reload";
			pchar.quest.LSC_KillOneNarval.win_condition.l1.locator = "reload1";
			pchar.quest.LSC_KillOneNarval.function = "LSC_KillOneNarval";
		break;

		case "NarvalRing_1":
			dialog.text = NPCStringReactionRepeat("�������! ������ ��� � �������� � ��������, ��� ����� ��� � ����� '����������'.", 
				"� ��� �����. �� ���, � �������� ��� ���� ����������.", 
				"������, ��� ��� �� ������. ��� � ��������!",
                "�� �� �����, ������...", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�����.", 
				"��, �������...",
                "���, �����������!", 
				"��, ����� ��������. ��� ����-�� ����� ���� � ����. �� ��� ����������� - ���������...", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("exit", "", "", "", npchar, Dialog.CurrentNode);
		break;
	}
	NextDiag.PrevNode = NodeName;
}