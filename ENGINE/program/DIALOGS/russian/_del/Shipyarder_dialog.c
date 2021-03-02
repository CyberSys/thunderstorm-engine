
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
			dialog.text = "����� ���������� � ��� �����. ��� ���� ���� �������?";
			link.l1 = "��� ���������� �������� �����, � �����, ���������� �����.";
			link.l1.go = "shipyard";
			link.l2 = "�����, � ��� �����.";
			link.l2.go = "exit";
			
			NextDiag.TempNode = "First time";
		break;
		
		case "shipyard":
			CalculateShipForShipYard(npchar);

			DialogExit();
			LaunchShipyard(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
