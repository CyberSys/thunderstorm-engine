int iStep = -1;

void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	int iQuestResult;

	switch(Dialog.CurrentNode)
	{
		case "First time":
			if(sStory == "cangetmap2" && sti(pchar.money) >= 1000)
			{
			//1
				dialog.text = "�, ������� ����! ��� ��� ��-�� ��� � �����. �� ��������� ������ ��� ���� ������?";
				link.l1 = "�������.";
				link.l1.go = "2";
			}
			else
			{
				if (npchar.quest.meeting == "0")
				{
					dialog.text = "����� ���������� � ��� �������. ��� ���� ���� �������?";
					npchar.quest.meeting = "1";
				}
				else
				{
					dialog.text = "��� ���� ������?";
				}
				link.l1 = "����� ������ �� ������ ��� ����������?";
				link.l1.go = "interface_exit";
				link.l2 = "� ��� ������. �� �������� ���?";
				link.l2.go = "work";
				link.l3 = "� ���� ������������ ����� �� ��������� �����.";
				link.l3.go = "room";
				link.l4 = "�����. � ��� �����.";
				link.l4.go = "exit";
			}

			NextDiag.TempNode = "First time";
		break;

		case "work":
			dialog.text = "������, ��������... ���� ���� ������������ ���������� � �������� �������� - ��� ��������� ����� �������� ��� ��������� �����\n��� ������ ���������� ��� �� � ������� ������-������ ����� - ��� ����� ���� ������ ��� ����� �������\n���� �� �� ���������� ���������� ���������, �� ����������� �� ������ - ��������, ��� ����-������ ���� ��������� �� ������ �������\n�� � ���������� ���� ������������ ���� ��������� �� ������ � ������ �����������. �� �� �� ����� �������� ���� ��������� � �������� �������������.";
			link.l1 = "�������, �� ��� ����� �����. �� ��������.";
			link.l1.go = "exit";
			link.l2 = "��������� �� ������. ����� ��������� � ���-������ ������.";
			link.l2.go = "First time";
		break;

		case "interface_exit":
			DialogExit();
			NextDiag.CurrentNode =  NextDiag.TempNode;
			LaunchHireCrew();
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "room":
			dialog.text = "�� ����� ���� �� ���������� ������������?";
			if(CheckForNight() == true)
			{
				link.l1 = "�� ����.";
				link.l1.go = "room_day";
			}
			else
			{
				link.l1 = "�� ����.";
				link.l1.go = "room_night";
			}
		break;

		case "room_day":
			dialog.text = "��� ��������� ��� � 5 �������.";
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = "����. ��� ���� ������.";
				link.l1.go = "room_day_wait";
			}
			link.l2 = "�� ����� ������ � ��������� � ����� ����.";
			link.l2.go = "hall_day_wait";
			link.l3 = "�� ������. ����� ��������� � ���-������ ������.";
			link.l3.go = "first time";
		break;

		case "room_night":
			dialog.text = "��� ��������� ��� � 5 �������.";
			if (makeint(pchar.money) >= 5)
			{
				link.l1 = "����. ��� ���� ������.";
				link.l1.go = "room_night_wait";
			}
			link.l2 = "�� ����� ������ � ��������� � ����� ����.";
			link.l2.go = "hall_night_wait";
			link.l3 = "�� ������. ����� ��������� � ���-������ ������.";
			link.l3.go = "first time";
		break;

		case "room_night_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate2("wait_night");
		break;

		case "hall_night_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			TavernWaitDate("wait_night");
		break;

		case "room_day_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			AddMoneyToCharacter(pchar, -5);
			AddDialogExitQuest("sleep_in_tavern");
			TavernWaitDate2("wait_day");
		break;

		case "hall_day_wait":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			TavernWaitDate("wait_day");
		break;

		case "2":
			dialog.text = "���, ���� �� ����� ��������� ��� ������ ������ ����� ������ �� �������������� �����? �������� ��, ��� �� ������ �� ���, ��� ���� ����� ������ - ����� ����� � ����������.";
			link.l1 = "��� ������� �� ����, ��������� �������������� ����� �����. ���� ��� ����� ������� ������, � ���� ������ ���� ������������, �, �������������, ��� �������� ����� �� ������������� ��������� ��������, ���� ������������� ��� ���������� �����...";
			link.l1.go = "3";
		break;

		case "3":
			dialog.text = "...�������. ��-�... �� ���� ����� ��������� ������������, ��� �� ����� �� ����� � 900 �������� ��������������?";
			link.l1 = "�, ��� ������ ��������. ��������, � ������� �� ������� ��� � �� ����� ����������, ����� �� ������ �� ������� ��� ����� �����������. 900 �������� �� ������������ ����� ������ - ���� ��������, �� ��� ������.";
			link.l1.go = "4";
		break;

		case "4":
			dialog.text = "� ���, ���� ���� ����� ����� ������� �� ���, ��� �� ���� ����� ����� �������� ���� ���? ����� ��� �� ��������� �� ��� �������?";
			link.l1 = "����� ������������, �������, ��������� �� ��� ��������, � ��������� �� �������� ������ ���������� ����� ��������� ���������. ������ ��������� ���� �� ����� ��� ��-���������� ������ ������ ����� ���������������, �� ������ ����������� ����������� ��� ��������.";
			link.l1.go = "5";
		break;

		case "5":
			dialog.text = "�-��... �� ������� ����������, �������. 600 ��������, � ��� ��� ��������� �����. ������ �� � �������� � ����� ���� �����-�� �������!";
			link.l1 = "������� ��������� ��������, ���������� ���������� ���� ������ � ������ ������ �������, ���������� ���. �� �����.";
			link.l1.go = "6";
			link.l2 = "��� ����� ������� ������. � ���� �� ������ ����������� ����, ��� ���������, ��������� �� �����, ����� ��� �������, ��� ������ ������� �� ������������.";
			link.l2.go = "7";
		break;

		case "6":
			dialog.text = "� ���� ������� ����� ����, �������. ������, ��� ������������ ��� �� ����������.";
			link.l1 = "����� ������������ � �������������� ���������� ����� ���������. �������, ����� ���� �����, ���� ������ �� ������� ��������.";
			link.l1.go = "SL exit map";
		break;

		case "7":
			dialog.text = "��� ������, �������. �� ������, � ��� ���� �� ����� ������ ����������� ���������� ����� - � ���� ��������� �����, ������� ��� ����� ������ ���������. � ��������� �� ��� �� ���� ������� ���� ���� ������, ��� �� ����� ���� ���� ���. �� ���� ��� ��� �� �����...";
			link.l1 = "�� �����, �����, ���� � �����! �� �����.";
			link.l1.go = "6";
			link.l2 = "���������� �����, �� �����. �� ��������.";
			link.l2.go = "SL exit no map";
		break;

		case "SL exit map":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			//give map, continue SL
			AddDialogExitQuest("get map 2");
		break;

		case "SL exit no map":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			//wrap up SL
			AddDialogExitQuest("refused map");
		break;
	}
}
