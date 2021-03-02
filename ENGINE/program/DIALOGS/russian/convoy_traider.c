//#include "DIALOGS\convoy_traider.h"
void ProcessDialogEvent()
{
	ref NPChar, d;
	aref Link, Diag;
	string NPC_Meeting;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);

	switch(Dialog.CurrentNode)
	{
		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
		break;
		
		case "prepare_convoy_quest":
			if (isBadReputation(pchar, 40)) 
			{
				dialog.text = RandPhraseSimple("�-�-�, ����!! � ����, ��� �� �����! ������ ����������� ������ ����� ������ ���� � ������������� ������ �������. ������!", "�-�, � ���� ��� �� ����! ���� ������ � �������� ���������������?! ��� ������...");
				link.l1 = RandPhraseSimple("���, ��������� �����...", "�������� ����� ����� �����!");
				link.l1.go = "convoy_refused";
			}
			else
			{
				dialog.text = TimeGreeting() + ", "+GetAddress_Form(NPChar) + "! � "+ GetFullName(NPChar) + ", ��������. � ������, ��� �� ����� ������?";
				link.l1 = "���-�� ����� ����. � ��, ��� � ������, ����� ��������, ������� �� ���������� ��� � ���� ����� � ����� ����������?";
				link.l1.go = "prepare_convoy_quest_2";
			}
		break;
		
		case "prepare_convoy_quest_2":
			dialog.text = "���������� �����. ����� ����, �����, ��� �� ��� ��������� � �������� ���������������. ��� �������?";
			link.l1 = "� ����� - ������ ��� ����� �, ��������, �� �����������.";
			link.l1.go = "prepare_convoy_quest_3";
		break;
		
		case "prepare_convoy_quest_3":
			GenerateConvoyQuest(npchar);
			dialog.text = "��� �����, ����� ���� ����������� �� " + XI_ConvertString("Colony" + pchar.quest.destination + "Gen") + ", ��� �� " + XI_ConvertString(GetIslandByCityName(pchar.quest.destination) + "Dat") +
				", �� " + FindRussianDaysString(sti(pchar.ConvoyQuest.iDay)) +", � �� ��� � ������� ��� " + FindRussianMoneyString(sti(pchar.ConvoyQuest.convoymoney)) + ". ��� �������?";
			link.l1 = "� ��������.";
			link.l1.go = "convoy_agreeded";
			link.l2 = "�� �����, ��� ��� ��� ���������.";
			link.l2.go = "convoy_refused";
		break;
		
		case "convoy_refused":
			chrDisableReloadToLocation = false;
			npchar.LifeDay = 0;
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("convoy_refused");
		break;
		
		case "convoy_agreeded":
			chrDisableReloadToLocation = false;
		    pchar.convoy_quest = pchar.quest.destination;
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			AddDialogExitQuest("convoy_agreeded");
		break;
		
		case "complete_convoy_quest":
			dialog.text = "�! ������� ���. ��� ����� ������� � ���������� ���� ��� ������� ��������. ��� ���� �������.";
			Link.l1 = "��������� ���.";
			link.l1.go = "exit";
			//�����
			AddSimpleRumour(LinkRandPhrase("����� �������� ��� �� ����� " + GetFullName(npchar) + " �������, ��� ����� �������� " + GetMainCharacterNameDat() + " � ����� ������������� �������� �����.", 
				"��������� �� ����� " + GetFullName(npchar) + " �������, ��� �������� " + GetMainCharacterNameDat() + " ����� ������. ��� ������� ��� ������� ��������� ������� ��� ������������� �� " + XI_ConvertString("Colony" + pchar.quest.destination + "Gen") + ".", 
				"� ������, ��� �� ������� ����� �� ������������� ��������, ������ �������� ���������. ����� �������� �� ����� " + GetFullName(npchar) + " ����� ������ � ��� ����������."), sti(npchar.nation), 40, 1);
			pchar.quest.generate_convoy_quest_progress = "completed";
			chrDisableReloadToLocation = false;
			npchar.LifeDay = 0;
			AddDialogExitQuest("convoy_refused");
		break;
	}
} 

// boal 03.05.04 ����� ����������� �������� -->
void GenerateConvoyQuest(ref npchar)
{
	int iTradeMoney;

	DeleteAttribute(NPChar, "Ship");
    SetShipToFantom(NPChar, "trade", true);

	iTradeMoney = sti(PChar.rank)*200 + sti(NPChar.rank)*250 + rand(30)*20;

	pchar.ConvoyQuest.convoymoney = iTradeMoney;
	pchar.ConvoyQuest.iDay  = 20 + rand(10);

	SetTimerCondition("generate_convoy_quest_timer", 0, 0, sti(pchar.ConvoyQuest.iDay), false);

	pchar.quest.generate_convoy_quest_progress = "begin";

	pchar.quest.generate_convoy_quest_failed.win_condition.l1 = "NPC_Death";
	pchar.quest.generate_convoy_quest_failed.win_condition.l1.character = "QuestTrader";
	pchar.quest.generate_convoy_quest_failed.win_condition = "generate_convoy_quest_failed";
}
