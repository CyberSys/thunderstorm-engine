
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;
	string sTemp;
	bool bOk;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "First time":
			dialog.text = "����������� ����, ����!";
			link.l1 = "����� � ���� ����? �� �� ���� ����������-������. � ��� �� � ����� ���������?";
			link.l1.go = "Meeting_1";
			NextDiag.TempNode = "First time";
		break;

        case "Meeting_1":
			dialog.text = "��� ������? �� ����� �������, � �����, ��� ��� ���� '����� �������'. ������ - ������ � �������, � ����� ��, ��� ����� �����, �����.";
			link.l1 = "�� ��, �� ������� �����, ������ ��� ���� ���� ��� �� �������� ����������, � ��� ��� � ���. ����� ��� ����� ������ � ���������?";
			link.l1.go = "Meeting_2";
		break;
		
		case "Meeting_2":
			dialog.text = "���� �� �������. � ��� ������ ������� ����� � ����� ������� ������� �������. � ������ ���, ��� � ���� �� ������ ���� � �� ������� � ����.";
			link.l1 = "�������, ��������� ��������, ���� ���. �� �������� ������?";
			link.l1.go = "Meeting_3";
		break;
		
		case "Meeting_3":
			dialog.text = "������ � ��������.";
			link.l1 = "�������� ��� ����������? � ���� ����?";
			link.l1.go = "Meeting_4";
		break;
		
		case "Meeting_4":
			dialog.text = "���, ������ ���, � �����.";
			link.l1 = "������� �� ����, ����� ��������. ������.";
			link.l1.go = "Exit";
			NextDiag.TempNode = "AfterMeet";
			ReOpenQuestHeader("GostShipQuest");
        	AddQuestRecord("GostShipQuest", "4");
		break;
		
		case "AfterMeet":
			dialog.text = "����� ���� ��������. ���� ��� �������?";
			link.l1 = "���, � ������.";
			link.l1.go = "Exit";
			NextDiag.TempNode = "AfterMeet";
		break;
		
		// �������
		case "GostCapt":
			dialog.text = "";
			link.l1 = "";
			link.l1.go = "Exit";
			NextDiag.TempNode = "GostCapt";
		break;
	}
}