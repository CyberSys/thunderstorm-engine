
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	int iResult;

	
	switch(Dialog.CurrentNode)
	{
		case "First time":	
			if(0)//CheckForQuestFree(npchar) == 1)
			{
				//iResult = GenerateEscortQuest(npchar, 0, "", 0, rand(3), 0, 0);
				trace("iResult = " + iResult);
				if (iResult == 0)
				{
					dialog.text = "� ��� ����-������, ��� ���������� �� ������������ ���� � ��� ����� �� ������� " + XI_ConvertString("colony"+Quests[sti(npchar.currentquest)].l.target_colony) + ", ����������� �� ������� " + XI_ConvertString(colonies[FindColony(Quests[sti(npchar.currentquest)].l.target_colony)].island) +  ".\n�� ��� ������ �� �������� ����� " + FindRussianMoneyString(Quests[sti(npchar.CurrentQuest)].l.money)  + ". ��� �������?";
					link.l1 = "������� �����������. �����, � �������� �� ��� ������.";
					link.l1.go = "escort_quest_accept";
					link.l2 = "�� �����, ��� ��� ��� ��������.";
					link.l2.go = "exit";
				}
				if(iResult == -1)
				{
					dialog.text = "��������, �� � �� � ���������� �������������.";
					link.l1 = "������.";
					link.l1.go = "exit";
					//EraseQuestFreeState(npchar);
				}
				if(iResult == -2)
				{
					dialog.text = "���, �������, ���� ��������� ������� ������ ���� ��������, ��� ��� � �� ���� ����� � ����� ������� ���.";
					link.l1 = "�� ���� ����.";
					link.l1.go = "exit";
				}
				if(iResult == -3)
				{
					dialog.text = "�������, � ������ � ��� ������ �������. � ����... ��� � ���� ���� ��� ���� ���� ������.";
					link.l1 = "� ����� ������ ��������� �����.";
					link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "��������, �� � �� � ���������� �������������.";
				link.l1 = "������.";
				link.l1.go = "exit";
			}
			
			NextDiag.TempNode = "First time";
		break;

		case "escort_quest_accept":
			dialog.text = "������ �� �����, �������? ������ ���� ��������� ������� - ��, �� ����������� ������ ������ ��������� ���� � ������� " + FindRussianDaysString(Quests[sti(npchar.CurrentQuest)].l.time) + ".";
			link.l1 = "��-��, � ����, ���� ������� � �� ������ �� �������� �������������.";
			link.l1.go = "exit";
			AcceptEscortQuest(npchar);
		break;

		case "SelectOffer":
			dialog.text = SelectEscortShipDialog(npchar);
			link.l1 = "�� �����.";
			link.l1.go = "accept_offer_exit";
			link.l2 = "��� ��� �� ���������.";
			link.l2.go = "decline_offer_exit";
		break;

		case "accept_offer_exit":
			AcceptEscortQuest(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "decline_offer_exit":
			DeleteEscortQuest(npchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
 
