void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, Diag;
	int i;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(Diag, NPChar.Dialog);
	int iTemp = sti(npchar.EncQty);
	string sTemp = "Patrol" + locations[FindLocation(npchar.location)].index + "_";
	string sGroup = "PatrolGroup_" + locations[FindLocation(npchar.location)].index;

	switch(Dialog.CurrentNode)
	{
		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();			
		break;

		case "exit_fight":
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_SetWarriorType(sld);
				LAi_group_MoveCharacter(sld, sGroup);
			}
			LAi_group_SetRelation(sGroup, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
			LAi_group_FightGroups(sGroup, LAI_GROUP_PLAYER, true);
			LAi_group_SetCheck(sGroup, "LandEnc_PatrolAfrer");
			DialogExit();	
			AddDialogExitQuest("MainHeroFightModeOn");
		break;

		case "exit_noFight":
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				sld.Dialog.CurrentNode = "GoodBye";
				sld.greeting = "soldier_common";
				LAi_SetWarriorType(sld);
				LAi_warrior_DialogEnable(sld, true);
				LAi_group_MoveCharacter(sld, sGroup);				
			}
			LAi_group_SetCheck(sGroup, "LandEnc_PatrolAfrer");
			DialogExit();			
		break;

		case "First Time":
			for(i = 0; i < iTemp; i++)
			{
				sld = CharacterFromID(sTemp + i);
				LAi_type_actor_Reset(sld);
				LAi_RemoveCheckMinHP(sld);
			}
			if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY && sti(NPChar.nation) != PIRATE)
			{
    			if (pchar.nation == npchar.nation)
				{
					dialog.text = RandPhraseSimple("�-�-�, �������, ���� �������� ������ ������� � " + XI_ConvertString("Colony" + npchar.city + "Dat") + "! �������� ���!!", "���, �� ������ �����������! ��������� " + NationNamePeople(sti(pchar.nation))+ " ���������� ���� �� ������ � " + NationNameAblative(sti(npchar.nation)) + "! �������� �������!!");
				}
				else
				{
					dialog.text = RandPhraseSimple("��������� ����� � ���� " + XI_ConvertString("Colony" + npchar.city + "Gen") + "! ����� ���!!", "��������-��, " + NationNamePeople(sti(pchar.nation))+ " ����������� ���� �� �� � " + XI_ConvertString("Colony" + npchar.city + "Dat") + "! ���������� ��������!!");
				}
				link.l1 = RandPhraseSimple("����������, ����� �� � �����������...", "���, ����� ��� ������ ����� ��������.");
				link.l1.go = "exit_fight"; 				
			}
			else
			{				
				dialog.text = LinkRandPhrase("���� ����� �������������! � ��������� ������� �� " + XI_ConvertString("Colony" + npchar.city + "Gen") + ", �� ����������� ������� ����������.",
					"������������, � ��������� ����� �������. �� ����������� ���������� �� " + XI_ConvertString("Colony" + npchar.city + "Gen") + " ����.",
					"����������� ���, " + GetAddress_Form(NPChar) + ". ��� ������������� ������������ �������������� ���������� ���� " + XI_ConvertString("Colony" + npchar.city + "Gen") + ".");
				Link.l1 = LinkRandPhrase("���������. ��� � ���� ��� ������?",
				"����� ������. ���� �� � ���� ��� ���-������ �������, " + GetAddress_Form(NPChar) + "?",
				"�����������. ��� ����� � ���� ��� ��� �������?");
				Link.l1.go = "Node_2";
			}
		break;
		
		case "Node_2":
			dialog.text = RandPhraseSimple("�� �� ������ ������ ��������������� � ������?",
				"���������� �� ��� ���-������, ��������� ����������, " + GetAddress_Form(NPChar) + "?");
			Link.l1 = RandPhraseSimple("���, ������ ������.", "���, ������, ��� ��������.");
			Link.l1.go = "Node_3";		
		break;

		case "Node_3":
			Diag.TempNode = "GoodBye";
			dialog.text = RandPhraseSimple("�� ��� ��, �� ���� ��� �����������. ��������, " + GetAddress_Form(NPChar) + ".",
				"�������. � ����� ������, ��������.");
			Link.l1 = "����� ���.";
			Link.l1.go = "exit_noFight";
		break;

		case "GoodBye":
			Diag.TempNode = "GoodBye";
			dialog.text = LinkRandPhrase("�� ���������� ��� �� ������� ������.",
				"� ����� ��� �� ������ ���!",
				"�, ��� ����� ��... ����� ����� �������, �� ������� ���.");
			Link.l1 = "������.";
			Link.l1.go = "Exit";			
		break;
	}
}
