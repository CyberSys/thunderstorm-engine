

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
		case "Drink duel":
			dialog.text = "� ���� ��� ��������! ����� �� ������� ������, � ����� ����� ������ ������ ����.";
			link.l1 = "�����. ������� �� ����� ����� � ������� ������������.";
			link.l1.go = "exit";
			
			NextDiag.TempNode = "First time";

			AddDialogExitQuest("Drink_Duel");
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
