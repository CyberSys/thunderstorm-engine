void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, Diag;
	int i;
	string sGroup;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(Diag, NPChar.Dialog);
	int iTemp = sti(npchar.EncQty);
	string sTemp = "Gang" + locations[FindLocation(npchar.location)].index + "_";

	switch(Dialog.CurrentNode)
	{
		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();			
		break;

		case "exit_Robbed":
			ChangeCharacterReputation(pchar, -1);
			int iMoney = makeint(makeint(Pchar.money)/20)*10;
			AddMoneyToCharacter(pchar, -iMoney);
			AddMoneyToCharacter(npchar, iMoney);
			npchar.SaveItemsForDead   = true; // ��������� �� ����� ����
			npchar.DontClearDead = true;  // �� ������� ���� ����� 200�
			AddSimpleRumour(LinkRandPhrase("�� �������, � ���������� �������� ��������� �� ����� " + GetFullName(npchar) + " ��������� ��������� ������ - ����� �������. � ���� ��� ������� � ���������. �� ������, �������� " + FindRussianMoneyString(iMoney) + ", ��-��... ", 
				"�� ������, ������� " + GetFullName(pchar) + " ����� " + FindRussianMoneyString(iMoney) + ", ����� ���������� �� ������� �� ����� " + GetFullName(npchar) + " �-�, ��� �� �� ��� �������! �� ��� � ���� ������ - �� �������! ��-��-��.", 
				"�� �� ������� � ���, ������� ��������� ������ �� �������� " + GetMainCharacterNameGen() + "? ��� ���, ��� �������� �� ����� " + FindRussianMoneyString(iMoney) + "!"), sti(npchar.nation), 5, 1);
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_SetWarriorTypeNoGroup(sld);
				sld.Dialog.CurrentNode = "OnceAgain";
				LAi_SetCheckMinHP(sld, LAi_GetCharacterHP(sld)-1, true, "LandEnc_RaidersBeforeDialog");
			}
			DialogExit();			
		break;

		case "exit_fight":
			sGroup = "RaidersGroup_" + locations[FindLocation(npchar.location)].index;
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_SetWarriorTypeNoGroup(sld);
				LAi_group_MoveCharacter(sld, sGroup);
				LAi_RemoveCheckMinHP(sld);
				LAi_SetImmortal(sld, false);
			}
			LAi_group_SetRelation(sGroup, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
			LAi_group_FightGroups(sGroup, LAI_GROUP_PLAYER, true);
			LAi_group_SetCheck(sGroup, "LandEnc_RaidersAfrer");
			DialogExit();	
			AddDialogExitQuest("MainHeroFightModeOn");
		break;

		case "exit_RunFight":
			AddSimpleRumour(LinkRandPhrase("�� �������, �����-�� ������� �������-�� ��������� ������� �� ����� " + GetFullName(npchar) + ". ��� ������ ��� ��������, � � ����� ���� �������� � �����, ��-��!", 
				"�� ������, ������� " + GetFullName(pchar) + " �������� ����� ������� �� ����� " + GetFullName(npchar) + " �-�, ��� �� �� ��� �������! ���������� ������...", 
				"�� �� ������� � ���, ��� ������� ������ �� ����� " + GetFullName(npchar) + " ������� �������� �������� " + GetMainCharacterNameGen() + "? ��� ���, ��� ���������� ��� ��������, �� �� ��� �� ����, ������� - ������� ������. ������� ����������� ��� ����!"), sti(npchar.nation), 5, 1);
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_type_actor_Reset(sld);
				LAi_ActorAfraid(sld, Pchar, true);
				sld.Dialog.CurrentNode = "GetTheHellOut";
			}
			DialogExit();
		break;

		case "exit_noFight":
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_SetCitizenTypeNoGroup(sld);
				sld.Dialog.CurrentNode = "GetTheHellOut";
				LAi_SetCheckMinHP(sld, LAi_GetCharacterHP(sld)-1, true, "LandEnc_RaidersBeforeDialog");
			}		
			DialogExit();			
		break;

		case "First Time":
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_type_actor_Reset(sld);
				LAi_RemoveCheckMinHP(sld);
				LAi_SetImmortal(sld, false);
			}
			dialog.text = LinkRandPhrase("����! ����� ������! ��������������, ����� ������������ � �������!",
				"��, ������! ����������! ���� ������� ���� �������!",
				"����, ��������! ������ ������ ���� ������� - �����!");
			Link.l1 = LinkRandPhrase("��� �� ������?","� ��� ����?", "��� �� ������ � ����?");
			Link.l1.go = "Node_2";
			Link.l2 = LinkRandPhrase("�-�, ����������! ������������ ���� ����������. � ���!",
				"������� �� ���� �������, �� ��� �������. ����������� � ������!",
				"�� �������� �������� � ������ ���� ������ �� �����!");
			Link.l2.go = "CheckSkills";	
		break;
		
		case "Node_2":
			dialog.text = LinkRandPhrase("�, �� �� �� ��������! ���� ��� ������, ��� � ���� ����! ����� �������, ����� ��������� ����!",
				"������ � �������� ���������? ��� ��� ��� ��� � ����! �� ������������ �� ����� ������� - ������������ �� ����� �������!",
				"��������� ��� ����������� ������� - ���������� ��� ������, ��� � ���� ����! � ��� ����� ����� � �������!");
			Link.l1 = "���������! ��������! � ���� � ����� ������ " + makeint(makeint(Pchar.money)/20)*10 + " ��������.";
			Link.l1.go = "CheckMoney";
			Link.l2 = "� �� ����� ���� ������ ��� ���!";
			Link.l2.go = "CheckSkills";			
		break;

		case "CheckSkills":
			if (GetCharacterSkillToOld(Pchar, "Fencing") >= 7 && makeint(Pchar.Rank) >= 8 && Makeint(PChar.reputation) <= 30)
			{
				Diag.TempNode = "GetLost";
				dialog.text = LinkRandPhrase("��, �� ��������� ���, ��������! � �������! ��� ����� �������!",//15
					"�����, � ���������. �� ������ ���������� ���� ����, ����� �� ������ ���� �������.",
					"�� ������ �������! ������� � ���� ������...");
				Link.l1 = LinkRandPhrase("�� ��� ��, ����� �� �������!",
					"������� �� �� ��� ������ �������! ������.",
					"���, ��� �� ������ ������ �������. ��������� ����������!");
				Link.l1.go = "Exit_NoFight";
				Link.l99 = LinkRandPhrase("������ ��������� - ���� ���� ������� � ����������.",
					"�������? �� � ��� �� ���� ��������� ���� ������!",
					"������ � �����, ���� �� ������� ��� ����.");
				Link.l99.go = "Exit_RunFight";
			}
			else 
			{
				dialog.text = LinkRandPhrase("� ���� ��� �� ���� � �����! � ������ ������!",
					"��� �� ��� ������, �������! ����!",
					"�� ���! � ����� ���� �� �����! ������ ���� ������ � ������� �� ��������� ����!");
				Link.l1 = "��������!";
				Link.l1.go = "Exit_Fight";
			}
		break;

		case "CheckMoney":
			if(makeint(makeint(Pchar.money)/20)*10 >= makeint(Pchar.rank)*100)
			{
				Diag.TempNode = "OnceAgain";
				dialog.text = LinkRandPhrase("������! ����� �� ���� ������ � ������ ���������!",
					"�������, �� �����, ��� ������. ������� ����� ���� �� ������������ ������! �� ������ ����.",
					"� ������������� �� ���� ������ ��� ���� ����� - �� ����� ������� �� ��������, ���� �� ����� ������! ���� � �����, � �������, � ��� ���� ��� - ����� ������.");
				Link.l1 = "��������� �� ���� ������!";
				Link.l1.go = "Exit_Robbed";				
			}
			else
			{
				dialog.text = "������� ���, �� ������ ���� ������! �� ������, ������ � ���� ��������. � � ����� � ��������� ������� ���������.";
				Link.l1 = "���������!";
				Link.l1.go = "Exit_Fight";				
			}				
		break;

		case "OnceAgain":
			Diag.TempNode = "OnceAgain";
			dialog.text = LinkRandPhrase("����� ��? �� �� ������ ������ ������ ���, ����� ����� ��������! � �� � ���� � ��������, ��� �������� ����.",
				"������ � ����� �������� ���������� ��� ������, � �� ������ � ���� ����������? ��� ����! ��-��!",
				"��������, �� ��� ������! �������� � ���� ���� �����, ��� � �������� ���� ������!");
			Link.l1 = "��, � ��� �����.";
			Link.l1.go = "Exit";
			Link.l2 = "� �������, � �������� ������� ��� �� ��������!";
			Link.l2.go = "Exit_Fight";
		break;
				
		case "GetLost":
			Diag.TempNode = "GetLost";
			dialog.text = LinkRandPhrase("��� ���� �����? �� �� ������������, ��� �� �������!",
				"������� �� ����, ������. � ���� ���� ����, � ���� ����, ��� �� ����� ���������� ����������!",
				"�����, ��������! ���� ������, ������� �� ���-���� ������ ������ ���� ������?");
			Link.l1 = "��, � ��� �����.";
			Link.l1.go = "Exit";
			Link.l2 = "� ����� ��� �������� �� ���� ���������!";
			Link.l2.go = "Exit_Fight";
		break;

		case "GetTheHellOut":
			Diag.TempNode = "GetTheHellOut";
			dialog.text = LinkRandPhrase("�������! ��������! � ���� ����!",
				"��������! �� ������! �� ������ ����!",
				"������! � ����� ���� �������� ���������! � ������� �����, ����� �������!");
			Link.l1 = "����� �� �� ���� - ��� �� �� ������!";
			Link.l1.go = "Exit";			
		break;
	}
}
