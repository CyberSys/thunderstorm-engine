
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	switch(Dialog.CurrentNode)
	{
		case "First time":
			dialog.text = "�� ������������ �� ����. � ��������� �� ���� ������ ������� �������.";
			link.l1 = "������.";
			link.l1.go = "exit";

			NextDiag.TempNode = "First time";
		break;

		case "Second time":
			dialog.text = "�� ������������ �� ����. � ��������� �� ���� ������ ������� �������.";
			link.l1 = "������.";
			link.l1.go = "exit";

			NextDiag.TempNode = "Second time";
		break;

		case "Successful_Travel":
			dialog.text = "������� ��� �� �������� �����������.";
			link.l1 = "�����.";
			link.l1.go = "succesful_travel_exit";
			NextDiag.TempNode = "First time";
		break;

		case "Failed_Travel":
			dialog.text = "�� �������� �� " + FindRussianDaysString(sti(Quests[sti(npchar.CurrentQuest)].l.contribution)) + "! ������� � ������ �������� �������� ����� ������������� ��� ������!";
			link.l1 = "��� ������, �� ����� �� ��� � ������� ������.";
			link.l1.go = "succesful_travel_exit";
			NextDiag.TempNode = "First time";
		break;

		case "Passenger_hear":
			dialog.text = "�������� �� ������������, �� ��� �����������, ��� �� ���������� � ������, � ����� � ���� ���������� ��� ���-���.";
			link.l1 = "�� �����, � ������������� ��� ��������� ���������.";
			link.l1.go = "Passenger_hear_2";
			link.l2 = "�� ��������, � �� ��� ������� ������.";
			link.l2.go = "Passenger_hear_denied";
		break;

		case "Passenger_hear_2":
			dialog.text = "��� ������! ������ �� ��� ���������� ������� �� ������� " + XI_ConvertString("Colony"+Quests[sti(npchar.CurrentQuest)].l.target_colony) + ", ������� ��������� �� ������� " + XI_ConvertString(colonies[FindColony(Quests[sti(npchar.CurrentQuest)].l.target_colony)].island) + ", �� " +FindRussianDaysString(Quests[sti(npchar.CurrentQuest)].l.time) +", � �� ��� � ������� ��� " + FindRussianMoneyString(Quests[sti(npchar.CurrentQuest)].l.money) + ". ��� �������?";
			link.l1 = "�����, �� ������������.";
			link.l1.go = "Passenger_hear_accept";
			link.l2 = "�� �����, ��� ���� ��� ����������.";
			link.l2.go = "Passenger_hear_denied";
		break;

		case "Passenger_hear_accept":
			dialog.text = "���, ��� �� ������������, �������.";
			link.l1 = "��, ��. � ���� ��� ���� ��� ���-����� ����, � ��� � ��������� ������� ����� ��� ����� � ��������� �� ����.";
			link.l1.go = "pass_accept_exit";
		break;

		case "Passenger_hear_denied":
			dialog.text = "�, ����� �������� ����. ��������.";
			link.l1 = "�� ��� �������, � ��� �� ������.";
			link.l1.go = "pass_decline_exit";
		break;

		case "Passenger_delayed":
			dialog.text = "�, ��� � ��, �������. � ����� ��� ���.";
			link.l1 = "����� ������� ��� ��������� �� ���� ����� �������, �� ������������ ��� ������ � ������� ���� ���� � ���� �������.";
			link.l1.go = "pass_accept_exit";
		break;

		case "Convoy_delayed":
			NextDiag.TempNode = "Convoy_delayed";
			dialog.text = "� ��� ��� ��� ��������� ����, � ��� ����� �����������! ����� �� ������ ����������� � ����?";
			if(CheckForConvoyFreeStack() == 1)
			{
				link.l1 = "� ����� ������� ��� ��� �����. ������������ �� ���� ������ �����. ��� ������ � ������� ���� ����, �� ��������� ������.";
				link.l1.go = "convoy_accept_exit";
			}
			else
			{
				link.l2 = "� ���� � ������� ������ ��� ����� ��� ������ �������, �� ��� ������ � ���-������ ��������, � ��� �� � ��� �������.";
				link.l2.go = "convoy_delay_exit";
			}
		break;

		case "Convoy_completed":
			dialog.text = "������� �� �������� �����������.";
			link.l1 = "�� �� ���.";
			link.l1.go = "convoy_successful_exit";
		break;

		case "convoy_accept_exit":
			NextDiag.TempNode = "Second time";
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AcceptEscortQuest(npchar);
		break;

		case "convoy_delay_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			DelayEscortQuest(npchar);
		break;

		case "pass_accept_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AcceptDeliveryPassengerQuest(npchar);
		break;

		case "pass_decline_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			DeletePassengerQuest(npchar);
		break;

		case "succesful_travel_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			CompletePassengerQuest(npchar);
		break;

		case "convoy_successful_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			HalfCompleteEscortTradeShipQuest(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
