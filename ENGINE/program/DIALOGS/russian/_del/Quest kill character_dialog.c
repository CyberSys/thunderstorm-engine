
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
			dialog.text = "���� ��� �� ���� �����?!";
			link.l1 = "������ �������������� � ���, ��� ���� ����� " + GetCharacterFullName(npchar.id) + ".";
			link.l1.go = "node_1";

			NextDiag.TempNode = "First time";
		break;

		case "node_1":
			dialog.text = "��� �. ����� � ���� �� ��� ����?";
			link.l1 = "��� ��������� ����� ����. �, �������, � ��� ������.";
			link.l1.go = "fight";
			link.l2 = "������-��, ��� ���� ����� ����, �� �� ������ ����������...";
			link.l2.go = "trade";
		break;

		case "fight":
			dialog.text = "�������! ����� �������� � ����! ���������!";
			link.l1 = "���������?! � ��������� �������. �������� � ������.";
			link.l1.go = "exit";
			AddDialogExitQuest("quest_fight_kill_character");
		break;

		case "trade":
			if (sti(pchar.skill.sneak) < rand(13) && sti(pchar.skill.leadership) < rand(13) &&sti(pchar.skill.commerce) <rand(13))
			{
				dialog.text = "��� ���! ��� ����� ����� ����!";
				link.l1 = "��� �������. �������, �� ��� � ���������� ���� �����.";
				link.l1.go = "exit";
				AddDialogExitQuest("quest_fight_kill_character");
			}
			else
			{
				dialog.text = "���������� �����������, � ������ �����������.";
				link.l1 = "��� ������: �� ������� ��� ���� ������, ������� " + sti(pchar.skill.commerce)*500 + " �����, � � �������� ����.";
				link.l1.go = "trade_2";
			}
		break;

		case "trade_2":
			if (sti(npchar.money) < sti(pchar.skill.commerce)*500)
			{
				dialog.text = "� � ��� �� ���������, �� � ���� ���� ����� " + npchar.money + " ��������. ���� ���� ��� ��������...";
				link.l1 = "��������. ����� �� ����.";
				link.l1.go = "trade_min";
				link.l2 = "���?! ��� ��, ��� ����� ����� ����� ����.";
				link.l2.go = "fight";
			}
			else
			{
				dialog.text = "������������. ��� ���� ������ � ������.";
				link.l1 = "�����. ���� ������� ����� � ����� ����.";
				link.l1.go = "exit";
				AddDialogExitQuest("give_ring");
				AddMoneyToCharacter(pchar, sti(pchar.skill.commerce)*500);
			}
		break;

		case "trade_min":
			dialog.text = "�����. ��� ������, � ��� ������.";
			link.l1 = "����. ���� ������� ����� � ����� ����.";
			link.l1.go = "exit";
			AddDialogExitQuest("give_ring");
			AddMoneyToCharacter(pchar, sti(npchar.money));
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
