
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
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		case "fight":
            LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
            LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
			DialogExit();
			AddDialogExitQuest("MainHeroFightModeOn");
		break;
		case "quest_fight":
            LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
            LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
            LAi_group_SetCheck("EnemyFight", "DeliverToBander_Dead");
			DialogExit();
			AddDialogExitQuest("MainHeroFightModeOn");
		break;

		case "First time":
			dialog.text = "�� ��� �����?!";
			link.l1 = "����, �����, ������ �� ��������. ���� ������ � ���� ���� �����, ��� �� ���� �������. " + pchar.questTemp.jailCanMove.Deliver.name + ", ������ ������?";
			link.l1.go = "Step_1";			
			NextDiag.TempNode = "Second time";
		break;
		case "Step_1":
			dialog.text = "��� �� ����� ��������?";
			link.l1 = "������ ������ ��������.";
			link.l1.go = "Step_2";
			TakeItemFromCharacter(pchar, "Malyava");
		break;
		case "Step_2":
			if (sti(pchar.questTemp.jailCanMove.Deliver.good))
			{
				dialog.text = "�� ������ �������. ��� ��� � ���� � ��������? �����, ����������, ��� ������ �����.";
				link.l1 = "�� � ����� ���?! ��� �� ��� ����� ������ �� ������!";
				link.l1.go = "Step_fight";
			}
			else
			{
				dialog.text = "����� ������! �� ����� ���� �������� ������� - " + FindRussianMoneyString(sti(pchar.questTemp.jailCanMove.Deliver.price)) + ".";
				link.l1 = "�������, �������.";
				link.l1.go = "Step_good";
			}
		break;
		case "Step_fight":
			dialog.text = "�� ���� ������ �� ������. ������� �� �����, ����� ����� ������ �� �������.";
			link.l1 = "��, ��� �� ��� ��������...";
			link.l1.go = "quest_fight";
		break;

		case "Step_good":
			dialog.text = "������� � ���� ������.";
			link.l1 = "����� ��������.";
			link.l1.go = "Step_good_exit";
		break;
        case "Step_good_exit":
			AddQuestRecord("GivePrisonFree", "11");
			AddQuestUserData("GivePrisonFree", "iMoney", FindRussianMoneyString(sti(pchar.questTemp.jailCanMove.Deliver.price)));
			CloseQuestHeader("GivePrisonFree");
			LAi_SetWarriorTypeNoGroup(npchar);
			chrDisableReloadToLocation = false;
			AddMoneyToCharacter(pchar, sti(pchar.questTemp.jailCanMove.Deliver.price));
			DeleteAttribute(pchar, "questTemp.jailCanMove.Deliver");
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

        case "Man_FackYou":
			dialog.text = "�� ��, �����, ������ ��� �������?!";
			link.l1 = LinkRandPhrase("������!!", "��������!!", "�-���, ����!");
			link.l1.go = "fight";
		break;

		case "Second time":
			dialog.text = "�� � ��� ��������, ���� ������ ��-������.";
			link.l1 = "�����, �����...";
			link.l1.go = "exit";			
			NextDiag.TempNode = "Second time";
		break;
	}
}
