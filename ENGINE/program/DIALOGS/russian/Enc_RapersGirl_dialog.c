void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, Diag;
	int i;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(Diag, NPChar.Dialog);
	
	switch(Dialog.CurrentNode)
	{
		case "exit_1":
			AddDialogExitQuest("LandEnc_RapersTalk");
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();			
		break;
		
		case "exit":
			LAi_SetCitizenTypeNoGroup(npchar);
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
		break;

		case "First time":
			LAi_RemoveCheckMinHP(npchar);
			for(i = 1; i <= 3; i++)
			{
				if (GetCharacterIndex("GangMan_" + i) == -1) continue;
				sld = CharacterFromID("GangMan_" + i);
				LAi_type_actor_Reset(sld);
			}
			dialog.text = LinkRandPhrase("��������! ��������! ��� ���� ���������� ����! �������!",
				"�-�! �� ������, ����������! ������� ���� �� ���� ���������!",
				"������ ����������, �������� ���� �� ���� ��������! ����� ���!");
			Link.l1 = LinkRandPhrase("��� ����������?",
				"� ��� ����?",
				"��� ���������?");
			Link.l1.go = "Node_2";			
		break;        

		case "Node_2":
			dialog.text = LinkRandPhrase("���...��� ����� ����������� ���� ����! ������ ���, �� ���������� �� �����! ������� ����!",
				"��� �����...��� ����� ������� ���-�� �������... �������� ���� �� ���, ����������!",
				"�� ��� ������� ������ - �������� ���� �� ���� ���������� �������! ��� ����� ����������� ����!");
			Link.l1 = "���, ������ ����������.";
			Link.l1.go = "exit_1";			
		break;

		case "ThanksForHelp":
			dialog.text = LinkRandPhrase("�! ��������� ��� �� ������!",
				"�� ������ ����! � ��� ��� ����������!",
				"�� ��������� ������! ������� ���!");
			Link.l1 = "������ ��� � �������� ��������� � ������ ����������!";
			Link.l1.go = "ThanksForHelp_1";			
		break;
		
		case "ThanksForHelp_1":
			Diag.TempNode = "ThanksAgain";
			if(makeint(Pchar.reputation) >= 80)
			{
				dialog.text = "� ���� ���� ������� �����. �������� �� � ���� ���� �������������!";
				Link.l1 = "�������. ������ ���������� � ��������� ���.";
				Link.l1.go = "exit";
			}
			else 
			{
				if(Rand(1) == 0)
				{
					dialog.text = "� ���� ��������, ��� �� ���� ������! ����� ��� �����, ����� �� ������� � ������ �������!";
					Link.l1 = "�������. ������ ��� ����� ������������ �����.";
					Link.l1.go = "exit";
				}
				else
				{
					addMoneyToCharacter(Pchar, makeint(PChar.rank)*100);
					dialog.text = "� ���� ���� ������� �����. �������� �� � ���� ���� �������������!";
					Link.l1 = "�������. ������ ���������� � ��������� ���.";
					Link.l1.go = "exit";
				}
			}
		break;

		case "ThanksAgain":
            Diag.TempNode = "ThanksAgain";
            dialog.text = "� ���� ��������, ��� �� ���� ������! ����� ��� �����, ����� �� ������� � ������ �������!";
			Link.l1 = "�������. ������ ��� ����� ������������ �����.";
			Link.l1.go = "exit";
		break;
	}
}
