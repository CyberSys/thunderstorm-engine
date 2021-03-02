
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
			dialog.text = "��� ������ �� � ��� � ���� ��������.";
			link.l1 = "��� �������.";
			link.l1.go = "exit";
			
			NextDiag.TempNode = "First time";
		break;

		case "Smuggler_offer":
			dialog.text = "�������, ��������� ��� ��������, � ��� �� ����� ��������� ������, ������� ������������ ������� ��� �������� ���������������.\n���� �� ������ ���������� �� ��� � �������� �� ��� ������� �����, ����������� � ������ �� ����������� �������� � �������� � ����� ��������� ������ ����������������� �������, �������� ��� �� �����.";
			link.l1 = "��������, � ������� � �������� ������ ������.";
			link.l1.go = "exit";
			NextDiag.TempNode = "Smuggler_offer";
		break;


		case "Smuggler_tutorial":
			dialog.text = "��, �������, �� ������ �� ������ � ����� ��� ���� �������? ��� ����� ���� ������ ������ ��� ����� ����� � ���������, �� ���� ����� �������...";
			link.l1 = "������ �� � ���.";
			link.l1.go = "Smuggler_tutorial1";
			link.l2 = "�� ������.";
			link.l2.go = "exit";
			NextDiag.TempNode = "Smuggler_tutorial";
		break;

		case "Smuggler_tutorial1":
			dialog.text = "����� ��������, � � �������� ��� ��� ������ � �����������.\n������������� ����� ������ ����� ����� �� ����������� �������� - ��������� �������� ���������� ���� ������� ��������. ���� � ��� � ����� ���� ����� �����, ��������� �� ����� �� ����������� ��������, � �� ��������� ��������� ���� �� ����� ��������.\n������ ��������� ���, ����������� �����, � ������ �� �������� ������.";
			link.l1 = "������� �� �����.";
			link.l1.go = "Smuggler_tutorial_exit";			
			NextDiag.TempNode = "First time";
		break;

		case "Smuggler_nothing":
			dialog.text = "�� �����, �������, ��� �� �������� ��������������� ����� ��������... �� � ���� ������ �� ����� ������� �� ����������.";
			link.l1 = "��, �� ��� � ���� ���.";
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
		break;

		case "Smuggler_tutorial_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			CompleteSmugglerTutorial(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
