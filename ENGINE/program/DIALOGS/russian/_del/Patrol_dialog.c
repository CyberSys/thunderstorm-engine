
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
			dialog.text = "���������, ���������.";
			link.l1 = "...";
			link.l1.go = "exit";			

			NextDiag.TempNode = "First time";
		break;

		case "Fight_with_enemy":
			dialog.text = "���� � ������! � ������!";
			link.l1 = "�, ������!";
			link.l1.go = "exit";
			AddDialogExitQuest("sneak_to_town_failed");
			NextDiag.TempNode = "First time";
		break;

		case "Fight_with_contaminator":
			dialog.text = "��! �� �� �� ����� �������, ������� �������� ��� �����! � ������!";
			link.l1 = "�, ������!";
			link.l1.go = "exit";
			AddDialogExitQuest("sneak_to_town_failed");
			NextDiag.TempNode = "First time";
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
