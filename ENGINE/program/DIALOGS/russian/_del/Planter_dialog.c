
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	if(CheckAttribute(npchar, "quest.questflag"))
	{
		if(sti(npchar.quest.questflag) != 1)
		{
			npchar.quest.questflag = CheckSlaveQuest(npchar);
			npchar.quest.questflag.model = SetModelFlag(npchar);
		}
	}
	
	switch(Dialog.CurrentNode)
	{
		case "First time":
			if(CheckAttribute(npchar, "quest.questflag"))
			{
				dialog.text = "��� ��� ������, �������. �� ��������� ��� ������ �� �������� " + FindRussianSlavesString(sti(Quests[sti(npchar.currentquest)].l.slaves), "Acc") + "?";
				if(sti(npchar.quest.questflag) == 1)
				{
					dialog.text = SelectHuntForSlavesDialog(npchar);
					link.l1 = "�������, ��� ��� ��� ����. � ������ �� ���.";
					link.l1.go = "work_accepted";
					link.l2 = "��� ������ �� ��� ����. ����� ����-������ �������.";
					link.l2.go = "work_declined";
				}
				if(sti(npchar.quest.questflag) == 2)
				{
					link.l1 = "��. ����� ���������� � ��������� ������.";
					link.l1.go = "work_completed";
				}
				if(sti(npchar.quest.questflag) == 3)
				{
					link.l1 = "��� ���, �� � ������� ��� ����.";
					link.l1.go = "exit";
				}
				if(sti(npchar.quest.questflag) == -1)
				{
					dialog.text = "��� ������ � ���� ���������.";
					link.l1 = "�� ��� ����������!";
					link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "��� ������ � ���� ���������.";
				link.l1 = "�� ��� ����������!";
				link.l1.go = "exit";
			}
			
			NextDiag.TempNode = "First time";
		break;

		case "work_completed":
			dialog.text = "� ���� ��������� ������� ����� ����! ��� ���� ������ � ������� ��� �������� �������������.";
			link.l1 = "��� ������� �������, ��� �����, ��� ��� ������������ ������� ������� � ����� �������.";
			link.l1.go = "work_completed_exit";
		break;

		case "work_accepted":
			dialog.text = "����� � ���� ����� ���, �������. ���, � ��, � ��� �� ������ ��� ������������.";
			link.l1 = "���������� ��������� ��������� ��� ������, ��� �����.";
			link.l1.go = "exit";
			AcceptSlavesQuest(npchar);
		break;

		case "work_declined":
			dialog.text = "��� ���� ����������, ���� ���� ����������. ��������, �������.";
			link.l1 = "�����.";
			link.l1.go = "work_declined_exit";
		break;

		case "work_completed_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			CompleteSlavesQuest(npchar);
		break;

		case "work_declined_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			DeleteSlavesQuest(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
