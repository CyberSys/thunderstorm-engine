
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
			dialog.text = "������ ��������� ���������� ��� ���� ��� ����� ��������, �������? ����� �� ���������� �� ������!";
			link.l1 = "������, ��� ��� � ���� ����.";
			link.l1.go = "Trade_exit";
			link.l2 = "� �� ������� ����.";
			link.l2.go = "quests";
			
			NextDiag.TempNode = "First time";
		break;


		case "Trade_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			LaunchItemsTrade(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
