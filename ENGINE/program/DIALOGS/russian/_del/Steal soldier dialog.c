
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
			dialog.text = "��! ���������� �������� ��� �� �����!";
			link.l1 = "�... � ������ �� ��� ��� ����� ���.";
			link.l1.go = "exit";			
			link.l2 = "�� ����� �� �� ������ �����������!";
			link.l2.go = "exit";
			
			NextDiag.TempNode = "First time";
		break;
		
		case "Steal Quest Item":
			dialog.text = "��! �� ������� �� ����� �� ���� ����������?!";
			link.l1 = "������... ������, �� ������, ���� ��� �������?! ��� ����� ���� �������!";
			link.l1.go = "exit3";
			
			NextDiag.TempNode = "First time";
		break;
		
		case "Uuups":
			dialog.text = "�� ���������� �� �����!";
			link.l1 = "������, ������, � ����� ���, ��� ���� ��������.";
			link.l1.go = "steal";
			link.l2 = "��! ������� �����, ������ �������� ��������, ��� �� ������ ��������� �� � �����!";
			link.l2.go = "fight";
		break;
		
		case "steal":
			dialog.text = "����� ��� ��� �������� ��������� ����� � ������� " + (sti(pchar.rank)+100-sti(pchar.reputation))*100 + ".";
			if (sti(pchar.money) >= (sti(pchar.rank)+100-sti(pchar.reputation))*100)
			{
				link.l1 = "����������. ��� ������.";
				link.l1.go = "money";
			}
			link.l2 = "� �� ��������� �������!";
			link.l2.go = "fight";
		break;
		
		case "money":
			StealMoney();
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "fight":
			dialog.text = "�����, ��� �������� ������ ��������� ����.";
			link.l1 = "�����������! ������ ���������, ������ ����� ���� �����!";
			link.l1.go = "exit2";
			pchar.steal.nation = npchar.nation;
			pchar.steal.character = npchar.id;
			AddDialogExitQuest("StealFight");
		break;
		
		case "Exit2":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "Exit3":
			StealQuestItem(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "Exit":
			AddDialogExitQuest("prepare_steal");
			ReturnStealCharacter(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
