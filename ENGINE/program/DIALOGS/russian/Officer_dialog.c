
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
			dialog.text = "��, �������?";
			link.l1 = "���, ������.";
			link.l1.go = "exit";

			
			NextDiag.TempNode = "First time";
		break;

		case "want_fire":
			dialog.text = "� ����� �� ���������� � ����, �������.";
			link.l1 = "��, � ��� ����?";
			link.l1.go = "want_fire_1";
			
			NextDiag.TempNode = "First time";
		break;

		case "want_money":
			dialog.text = "� ����� �� ���������� � ����, �������.";
			link.l1 = "��, � ��� ����?";
			link.l1.go = "want_money_1";
			
			NextDiag.TempNode = "First time";
		break;

		case "want_money_1":
			dialog.text = "� ���� ������������ ������� ������ � ���� ���������. �� ������� ��� ������� ������, �� �����, �� � ���� ������ � �������� � ����� ����� ������ �������.";
			link.l1 = "� ������� �� ������?";
			link.l1.go = "want_money_2";
		break;

		case "want_money_2":
			dialog.text = "�������� ��� ����� ��������, � � �������� � ����. � ��������� ������ ��� �������� ������������ � ����.";
			if(sti(pchar.money) >= 100)
			{
				link.l1 = "������. ������������.";
				link.l1.go = "money_exit";
			}
			link.l2 = "��� ���! ���� �� �������!";
			link.l2.go = "money_decline_exit";
		break;

		case "want_fire_1":
			dialog.text = "������ ��, � ��� �������, ������� � ����� ������� � �����-�� ������. �� �� �������� �� ��������, �� ���� ������ ����� ����, ����� � ���� ��� ���!\n��� ��� � ����� �� �����, � �� ������ ������ ������� �������� �� ��� ���������.";
			link.l1 = "�����, ��� ���� �����.";
			link.l1.go = "fire_exit";
		break;

		case "money_decline_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			OfficerNotGotMoney(npchar);
		break;

		case "money_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			OfficerGotMoney(npchar);
		break;

		case "fire_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			OfficerFired(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
