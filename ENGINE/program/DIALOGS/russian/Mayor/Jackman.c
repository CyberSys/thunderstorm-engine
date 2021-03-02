// ������� �� ��������
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

// ============================================================================
// ============================= ���� angry ==========>>>>>>>>>>>>>>>>>>>>>>>>>
    if (CheckAttribute(npchar, "angry") && !CheckAttribute(npchar, "angry.ok"))
    {
        npchar.angry.ok = 1;
        if (!CheckAttribute(npchar, "angry.first")) //������� ������� ������������
        {
            NextDiag.TempNode = NextDiag.CurrentNode;
            Dialog.CurrentNode = "AngryExitAgain";
            npchar.angry.first = 1;
        }
        else
        {
            switch (npchar.angry.kind) //���� ����������� ������� �����. � npchar.angry.name ������� �������!!!
            {
                case "repeat":
                    if (npchar.angry.name == "Firsttime") Dialog.CurrentNode = "AngryRepeat_1";
                    if (npchar.angry.name == "I_know_you_good") Dialog.CurrentNode = "AngryRepeat_2";
					if (npchar.angry.name == "BlueBird_1") Dialog.CurrentNode = "AngryRepeat_1";
                break;
            }
        }
    }
// <<<<<<<<<<<<<<<<<<<<<<======= ���� angry ===================================
// ============================================================================

	switch(Dialog.CurrentNode)
	{
		// ----------------------------------- ������ ������ - ������ �������
		case "First time":
            dialog.text = NPCStringReactionRepeat("� ���� ���� �� ���? ���? ����� ���� ������!",
                         "� ������� ���� ���������.", "���� � ��������� � ����, �� �� ����������� ��������� ����!",
                         "��-�-��, ��� ��� ������ �� ��������, ���� ��� �������.", "repeat", 3, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��� �����.",
                                               "�������, �������...",
                                               "������, �������...",
                                               "��...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
			//������ ������ ����� �����
			if (pchar.questTemp.BlueBird == "toBermudes")
			{
				link.l1 = "������, �������, ��� ����� ����... � �����, �� �������� �� � ���� � ����� ������ '����� �����'?";
				link.l1.go = "BlueBird_1";
			}
			if (pchar.questTemp.BlueBird == "weWon")
			{
				link.l1 = "�������������, � ���� ������� ��� ������ '����� �����'!";
				link.l1.go = "BlueBird_6";
			}
			if (pchar.questTemp.Sharp == "begin")
			{
				link.l1 = "��������, ����� ����� � �������� ������ ������� � ��������� ����. �� ������ �� ������ �� ����?";
				link.l1.go = "SharpPearl_1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_toJackman")
			{
				link.l1 = "� ��� ������ ����� ������� ���, ������� ������, ��� �� � ����.";
				link.l1.go = "PL_Q3_1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_toTavernAgain")
			{				
				dialog.text = "��� ����?! � �� � ���������� �������� �� ���� �������!";
				link.l1 = "�������, � ��� ���� � ����� ����� ����. ������ ���.";
				link.l1.go = "PL_Q3_2";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_GoodWork" && !CheckAttribute(npchar, "quest.PQ3"))
			{
				dialog.text = "��������! �� �� � ������� ������!";
				link.l1 = "��� ��������, ��� ������ ��� ���� � ������ � ������ � ����� ������ � ����.";
				link.l1.go = "PL_SEAWOLF";				
			}
			if (pchar.questTemp.piratesLine == "Panama_backToShip")
			{
				dialog.text = "�, ��� ���� ������, " + pchar.name + ". ��, ��� �������?";
				link.l1 = "�� ������ � ���, ��� ������ ������ � ������?";
				link.l1.go = "PL_Q8";				
			}
		break;

 		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
		break;

        case "I_know_you_good":
            dialog.text = NPCStringReactionRepeat(GetFullName(pchar) + ", ��� ���� ������! ����� ��������� �� ���� ���?",
                         "�� ��� ���� ���?", "����� ��� ����� ������������? ���� ���� ������ ������, �� �������� ������!",
                         "�� ������� �����, ������� ���� ����. �� �������� � ������������� � ����� � ������ �� �����.", "repeat", 10, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��, ����������, ������ ��� ��������, ���������. ������ �� ���� ���.",
                                               "������, ������ ���...",
                                               "������, �������, ������...",
                                               "��� ���� ������, ���������!!!", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			NextDiag.TempNode = "I_know_you_good";
			//������ ������ ����� �����
			if (pchar.questTemp.BlueBird == "toBermudes")
			{
				link.l1 = "������, �������, ��� ����� ����... � �����, �� �������� �� � ���� � ����� ������ '����� �����'?";
				link.l1.go = "BlueBird_1";
			}
			if (pchar.questTemp.BlueBird == "weWon")
			{
				link.l1 = "�������������, � ���� ������� ��� ������ '����� �����'!";
				link.l1.go = "BlueBird_6";
			}
			if (pchar.questTemp.Sharp == "begin" && rand(1))
			{
				link.l1 = "��������, ����� ����� � �������� ������ ������� � ��������� ����. �� ������ �� ������ �� ����?";
				link.l1.go = "SharpPearl_1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_toJackman")
			{
				link.l1 = "� ��� ������ ����� ������� ���, ������� ������, ��� �� � ����.";
				link.l1.go = "PL_Q3_1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_toTavernAgain")
			{				
				dialog.text = "��� ����?! � �� � ���������� �������� �� ���� �������!";
				link.l1 = "�������, � ��� ���� � ����� ����� ����. ������ ���.";
				link.l1.go = "PL_Q3_2";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_GoodWork" && !CheckAttribute(npchar, "quest.PQ3"))
			{
				dialog.text = "��������! �� �� � ������� ������!";
				link.l1 = "��� ��������, ��� ������ ��� ���� � ������ � ������ � ����� ������ � ����.";
				link.l1.go = "PL_SEAWOLF";				
			}
			if (pchar.questTemp.piratesLine == "Panama_backToShip")
			{
				dialog.text = "�, ��� ���� ������, " + pchar.name + ". ��, ��� �������?";
				link.l1 = "�� ������ � ���, ��� ������ ������ � ������?";
				link.l1.go = "PL_Q8";				
			}
		break;
		//********************* �������, ����� �3, ������ ��� *********************
		case "PL_Q3_1":
			dialog.text = "�����?! ��! �� ������ �����, ��������. ��, ������! � �� ���� ���!!!";
			link.l1 = "�� �����!..";
			link.l1.go = "PL_Q3_fight";
		break;
		case "PL_Q3_fight":
			LAi_LocationFightDisable(&Locations[FindLocation("Pirates_townhall")], false);
			chrDisableReloadToLocation = true; // ������� ����� �� �������.
            for (i=1; i<=3; i++)
            {
                sld = GetCharacter(NPC_GenerateCharacter("QuestPir_"+i, "pirate_"+i, "man", "man", 100, PIRATE, 0, true));
                FantomMakeCoolFighter(sld, 100, 100, 100, "topor2", "pistol3", 100);
            	LAi_SetWarriorType(sld);
				LAi_SetImmortal(sld, true);
            	sld.Dialog.Filename = "Quest\EngLineNpc_2.c";
                LAi_group_MoveCharacter(sld, "PIRATE_CITIZENS");
               	ChangeCharacterAddressGroup(sld, pchar.location, "reload",  "reload1");
            }
            LAi_group_SetLookRadius("PIRATE_CITIZENS", 100);
            LAi_group_SetRelation("PIRATE_CITIZENS", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
            LAi_group_FightGroups("PIRATE_CITIZENS", LAI_GROUP_PLAYER, false);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "PL_Q3_2":
			dialog.text = "���?! ����� � ���� ���� � ���?";
			link.l1 = "� ��� ���������� �� ��������� �������. ��� ������� �������, � � ��� ���, ���� ������� ��� �����.";
			link.l1.go = "PL_Q3_3";	
		break;
		case "PL_Q3_3":
			dialog.text = "��. �� � ������� ����� ���������. ������ �������, ��� � ������ ��������� ���� ������. �� ���� ����������, ����� ��� ����!";
			link.l1 = "��, � ����, ������ �� ����� ����� ��� ���������.";
			link.l1.go = "PL_Q3_4";
		break;
		case "PL_Q3_4":
			dialog.text = "�� ��, ��� ��� ����! ����� � ���� ��� �������� ����������! ��� � ���� ����������, ��� � ��� ������ ���� ������! �� �� ���� ���������� �� ��� ������ �������� �� ������� ��������. � �� ���������� �� ��������� ����. ������� ��������, �� ��, ��� ���� ���������. ���, ������!\n����� ��� ������� ���������� ����� ������������ �� �����, � ��� ��� � ������: �� �������� '������� ����' - ����� ������ ����� � ������ � ��� �������! ���� ����� ����� ����...";
			link.l1 = "����� �� � ���.";
			link.l1.go = "PL_Q3_5";
		break;
		case "PL_Q3_5":
			dialog.text = "�� ��� � ���, �������, �� ���� � ��� �������, �����, � ������� ���, ������ ��������� �� ��������������. �� � ������ � ������, ������, ��� � ���� ������ �������! � ������� ����� �������� ��������, �����, � ��������� ���, �����.\n"+
				"������, ��������, ����� ������� ��� ������. ��� �� �����, � ���� ����� �����, � ���� ������� ���� �� ��� ������, ���� �� ������ �������� � ���� �������, ���� ���� ��. � ����, ���� ��� ����, �� ��� �������.\n"+
				"���� �� ���� - �� �������� ������������ �� ������! � ���� ��� ��� �����! ���!!! ������� ���� ��� ����� �����, ��� ������ � ���� ������ �� ����, �� �� �������, � �� ����! �� � ���� �����������.";
			link.l1 = "������, ��� �� ������. ������� ������ ��� �� ���� ������.";
			link.l1.go = "PL_Q3_6";
		break;
		case "PL_Q3_6":
			dialog.text = "�����?";
			link.l1 = "������ ���� �� ���������� ��, ��� ������� ��������? � ������ ������, ��� �� ������.";
			link.l1.go = "PL_Q3_7";
		break;
		case "PL_Q3_7":
			dialog.text = "��...  ������... ��� � ������, ���� �������. ��������� ��������, ����������� �����, ���������, �������, �����!  �������� �����, ������? ����� �� ���, ��������� ���� ���� ����� ���, ��� ��������� ����� �����, � �� �������� ����� ��� �� �������.\n"+
				"�� �������� ��� ������, ���-�� �� ��� ������ �����, �� ������� ������������, ���-�� ������� � ����� �����, ���-�� �������. � ���-�� ������������� � ����, � ����� ����������� ����� �� �������� �������\n"+
				"��... � ������, �� �� ��������� � ��� �. ������ �� �� ���������: '������� �������� ����'? ��� ���, ��� �� - ������ ����. ���� ������� ������ ������� �������, ��� ���� ��������.\n"+
				"���� ����� �����, � �������� ����� ������ ������ � ������� �����, ��� ����� ��������� ��� � ����, �����... ����� �� ��������, ������, ��� ������ ���� ����� ����� ������ �������� � ������� �������.";
			link.l1 = "��, ������� �� �����. ��� ������ � �����. � ������ ������, ���� ������, ��� �������� ������, ��� �����-�� ���������.";
			link.l1.go = "PL_Q3_8";
		break;
		case "PL_Q3_8":
			dialog.text = "��-��-��-��, �� ����, ����! � ����������� � �������!";
			link.l1 = "���, ����� ����.";
			link.l1.go = "exit";
			NextDiag.TempNode = "I_know_you_good";
			AddQuestRecord("Pir_Line_3_KillLoy", "12");
			pchar.questTemp.piratesLine = "KillLoy_2toTavernAgain";
		break;
		//����� ������� ���
		case "PL_SEAWOLF":
			NextDiag.TempNode = "I_know_you_good";
			npchar.quest.PQ3 = true;
			int chComp;
			bool bOk = false;
			for (int i=0; i<=COMPANION_MAX; i++)
        	{
				chComp = GetCompanionIndex(pchar, i);
                if(chComp != -1 && RealShips[sti(characters[chComp].ship.type)].Name == "BrigSW1")
        		{	
					bOk = true;
					break;
				}
			}
			if (bOk)
			{
				dialog.text = "������ ���� ���� � � ����. ����, ��� ��� � ��� ���� ����� ����������! �������. �� �������, ��� �� � ����! ������� � ������ ������� ����� ���� '�������� �����', ������ �� �����! ����� �� � ����� ������ ����� ��� ��, � ����� �������!";
				link.l1 = "� - ��������� �������, �������. �� ���� ����� - ����� ��� ����!";
				link.l1.go = "exit";
				AddQuestRecord("Pir_Line_3_KillLoy", "19");	
			}
			else
			{
				dialog.text = "������ ���� ���� � � ����. �� ��� ��, ������� ����, ��������. � ������� �� ����� ��� ���� 20 ����� ��������!";
				link.l1 = "�������, �������! ��� ������ ��� ����������.";
				link.l1.go = "exit";
				AddMoneyToCharacter(pchar, 20000);
				AddQuestRecord("Pir_Line_3_KillLoy", "20");	
			}
		break;
		//********************* �������, ����� �6. ������� *********************
		case "PL_Q6":
			dialog.text = "�-�-�, ���� � ����?! ������ �� ����!";
			link.l1 = "������ ������ � ���� �������, ��� ������� ����� �����.";
			link.l1.go = "PL_Q6_1";
		break;
		case "PL_Q6_1":
			dialog.text = "��� ��� ���, ������... ������� �� �� ��������... � ��� ��� ���������?";
			link.l1 = "� ���� ��� � ���������� �������. ���������, � �� ��� ��������� ������� ����� ������, � ���������, ��� ��� ���������� ���� �������...";
			link.l1.go = "PL_Q6_2";
		break;
		case "PL_Q6_2":
			dialog.text = "������ ���, ���� �����?";
			link.l1 = "���... � �����, � ����� ����� �� ����� ������ � ������� ��������� ��������, ��� ��� �� ������� ���� �� ������. ��� ����� ��������. ��� ��� � � ������ ��� ���� ����������.";
			link.l1.go = "PL_Q6_3";
		break;
		case "PL_Q6_3":
			dialog.text = "��, �� ����! � �� �� � �����, ��������, ��� �� ��� �������� ������ �� ���� �����, ���� �������, � ���� ��� ��������?";
			link.l1 = "�?!!";
			link.l1.go = "PL_Q6_4";
		break;
		case "PL_Q6_4":
			dialog.text = "��, ������ ����� ����� �����. ������� � ������ ������ �� ����, ��� ������ ���� �����.";
			link.l1 = "������!! � ��� �� ��� ������?";
			link.l1.go = "PL_Q6_5";
		break;
		case "PL_Q6_5":
			dialog.text = "����������� � ������ �������� � �����-��������. ��� � ����� �� ������� '���������' ����� ���� ����, � ���� ��� �������� � ��������. ��� �������, ���� ����� ����� ��������� ��� ���������...";
			link.l1 = "������, �������, ��� � ������.";
			link.l1.go = "exit";
			NextDiag.TempNode = "I_know_you_good";
			SetQuestHeader("Pir_Line_6_Jackman");
			AddQuestRecord("Pir_Line_6_Jackman", "2");
			pchar.questTemp.piratesLine = "PL6Brother_toSantaCatalina";
			//������ ����� ����� � ����� ��������
			sld = GetCharacter(NPC_GenerateCharacter("JohnLids", "officer_10", "man", "man", 30, sti(pchar.nation), -1, true));	
			FantomMakeCoolSailor(sld, SHIP_FRIGATE, "���������", CANNON_TYPE_CULVERINE_LBS24, 80, 60, 60);
			sld.name = "����";
			sld.lastname = "����";
			sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
			sld.dialog.currentnode   = "First time";	        
			sld.DeckDialogNode = "JohnLids";		
			sld.Abordage.Enable = false;
			sld.AnalizeShips = true; 
			sld.AlwaysFriend = true;
			sld.ShipEnemyDisable = true;
			sld.Ship.Mode = "Pirate";
			LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
			Group_AddCharacter("LidsGroup", "JohnLids");
			Group_SetGroupCommander("LidsGroup", "JohnLids");			
			Group_SetAddress("LidsGroup", "SantaCatalina", "quest_ships", "quest_ship_7");
			Group_SetTaskNone("LidsGroup");
		break;

		case "PL_Q6_after":
			dialog.text = "���� ��� ��������� ��� � ����� �������� � ���������. �� ����� ����� ��� �����? ����� ���-������ ����������?";
			link.l1 = "���������� �����, �� � ������. ������ �� ��� �� ������...";
			link.l1.go = "PL_Q6_after_1";
		break;
		case "PL_Q6_after_1":
			dialog.text = "��� �����. �� ����� ���� �����-�� �������, �������� �������. ���� ����� �� ����� �������, �� ������� ��� ����� ����������. � ����� ���� �����!";
			link.l1 = "� �������. ��� ������� ���-�� ����, �� �� ������. ���� �������� ��������, ��� ����� �� ���� ����...";
			link.l1.go = "PL_Q6_after_2";
		break;
		case "PL_Q6_after_2":
			dialog.text = "�����. �� ��� ��, ������, ��� �� �� ��� ��������, ��������� ��������� ������ ������, ��� �������. ������ ����������� � �������, ������� ��� � ����� ��������.";
			link.l1 = "��, � ��� � ������. ������� ����, �������, ��� ������� ���.";
			link.l1.go = "PL_Q6_after_3";
		break;
		case "PL_Q6_after_3":
			dialog.text = "� � �� �����, � ������ ����, ��� ���� ����� ���� �� � �����-��������. ���� ������ �������.";
			link.l1 = "�������... ��, ������������� ����� �� ���� �������.";
			link.l1.go = "exit";
			SetQuestHeader("Pir_Line_6_Jackman");
			AddQuestRecord("Pir_Line_6_Jackman", "5");
			QuestSetCurrentNode("Henry Morgan", "PL_Q7_begin");
			sld = characterFromId("Henry Morgan");
			LAi_SetHuberTypeNoGroup(sld);
			ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
		break;


		//********************* ������� ���� *********************
		case "SharpPearl_1":
			dialog.text = "������, ������ ������ �� ���� ������ �� ����. ���������������� �� ���� ��������� ��� ����� ������� � ��������� ������� ����! ��� �� � ��� ������� �������� �������, ��-��-��!";
			link.l1 = "� ��� ��� ����� ���������, ����� �����?";
			link.l1.go = "SharpPearl_2";
			pchar.questTemp.Sharp = "seekSharp";
		break;
		case "SharpPearl_2":
			dialog.text = "�� ����. ������� ���� - ��� ���������� ���. ���� ���� �� �� �����������\n� ������ ������ ��� ����� � ��������.";
			link.l1 = "�������, �������.";
			link.l1.go = "exit";
		break;

        //********************** ������ ����� ����� ***********************
        case "BlueBird_1":
			dialog.text = NPCStringReactionRepeat("�-��, ���������� ����� ������, ��� � ��������... � ����-�� ����� ������������ ��� ������?", 
				"�� ��� �������� �� ���� ������.", 
				"����� ��� �� ��?",
                "���?! �����?! �� � ���� ������...", "repeat", 10, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�� ������ ��, ����� �� ������������ ���� ������ ���������...", 
				"��, �������, �����.",
                "��, ���� �������, ������...", 
				"��-��!..", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("BlueBird_2", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
  		break;
		case "BlueBird_2":
			dialog.text = "��-��, �� �� �������� ��? � �� ������ �, ��� ������� �� ��������� �� ���� '����� �����'.";
			link.l1 = "�������� ������������� ������������ ���� �������. ��, ������, ��� � ������ ��������� �� ����, ��� �� �������� �����, �� �������� ���� ������. ��� ��� �� �������� ��������� �������� �� ���� �����!";
			link.l1.go = "BlueBird_3";
		break;
		case "BlueBird_3":
			dialog.text = "��, ��� ���! �� �� ������, ��� �� ��� ������� �� ���������� ��������, �� ��� ����� ��� �� �������. ��� �� ����, �� ����� ��� ��� '�����' �� ��������� - ������ ���� ��� ��������� ���.";
			link.l1 = "�������, ��� �� ������, ��� ������?";
			link.l1.go = "BlueBird_4";
		break;
		case "BlueBird_4":
			dialog.text = "���, �������. ������ - ����� � ��������� �����, �� � ���� �� ��������.";
			link.l1 = "�����, ���������� �������������, ��� ������?";
			link.l1.go = "BlueBird_5";
		break;
		case "BlueBird_5":
			dialog.text = "���, ����� ��� �� ����. �� ����� ������� �����.";
			link.l1 = "�������. �� ��� �, ������� � �� ����, ��������.";
			link.l1.go = "exit";
		break;

		case "BlueBird_6":
			dialog.text = NPCStringReactionRepeat("��, ����������. ������� �����, ���� ������! �������.", 
				"���������� ��� ���.", 
				"��� � ��� ������...",
                "��, �� �� �������, ��������, ����� ������ ��� �� ������!", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�������, �������. ������ ����� � ����� �������...", 
				"��� ��� �������, �������.",
                "�, �� ��...", 
				"������� ����!", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;
        //********************** ���������� ������� ***********************
 		case "EngLine_quests":
			dialog.text = "��� ������ ����������� ������ �� ���� �������. ��� ���� �����, ������.";
			link.l1 = "� ���� ���� ��� ���� ������. �������� - �������-���������� ������.";
			link.l1.go = "Step_1";
  		break;

 		case "Step_1":
			dialog.text = "����� ���������, ���������.";
			link.l1 = "��������� ����� ����� �� �������, � ��� �� �������� ��������� ����������� ����� ������. � ������� ������ ����� �������������� ������, ������� ���������� ��������� �������������� '��������� �������' � ������� �� ������� � ���������� � ���������. � ��� ����������, ��� ����� ������. ��������� ���������� �������� � ��������.";
			link.l1.go = "Step_2";
  		break;
  		
 		case "Step_2":
			dialog.text = "���... ������ ���������, �� ��������.";
			link.l1 = "� � ��� ������� ������?";
			link.l1.go = "Step_3";
  		break;
  		
 		case "Step_3":
			dialog.text = "� ���, ��� ����� ����������� � �� �������, � ������� ����� ����������� ������ � �� ��������� � ������� � ���������� �����.";
			link.l1 = "��������� ������� ���� �� ������ ������...";
			link.l1.go = "Step_4";
  		break;
  		
 		case "Step_4":
			dialog.text = "���� ������ ������ ��������� ����� ������. ��, ������ �� � ��� ����, �������� ����� ������� - ���� ������, � ������� ����� ��������� - �� ��� � ����� ����� �� ������. ��� ��� ��� ����.";
			link.l1 = "�����, ���������?";
			link.l1.go = "Step_5";
  		break;
  		
 		case "Step_5":
			dialog.text = "���� ����� � ������. � ��������� ��������� �� ���� ������.";
			link.l1 = "��� ������. ����� ��������.";
			link.l1.go = "exit";
			
            AddQuestRecord("Eng_Line_2_Talking", "4");
            pchar.questTemp.Count = makeint(sti(pchar.questTemp.Count) + 1);
            if (pchar.questTemp.Count == "4")   {pchar.questTemp.State = "after_talks_with_pirates";}
            NextDiag.TempNode = "First time";
  		break;

 		case "Lets_go_bussines":
			dialog.text = "��� ���� ������. ������ �������� �� ��������?";
			link.l1 = "� �� ����. �� � �������� ������� �����, �� ������� ������� � ����?";
			link.l1.go = "Step_6";
  		break;

 		case "Step_6":
            if (GetCompanionQuantity(pchar) != 4)
            {
    			dialog.text = "���, ��� ������ ����� �� �����, ������� ��������������, � �����.";
    			link.l1 = "����� ��������� ����������!";
    			link.l1.go = "Step_7";
            }
            else
            {
    			dialog.text = "� ��� � ��� ���� ��������, ��� ��� ��� ���� �� ���� ���.";
    			link.l1 = "�� ��� �, ��� ������, ���� ������.";
    			link.l1.go = "exit";
                NextDiag.TempNode = "First time";
            }
  		break;

 		case "Step_7":
			AddQuestRecord("Eng_Line_3_Morgan", "4");
            LAi_SetActorType(npchar);
            LAi_ActorSetStayMode(npchar);
            ChangeCharacterAddressGroup(npchar, "Pirates_townhall", "goto", "governor1");
            CharacterIntoCompanionAndGoOut(pchar, npchar, "reload", "reload1", 10, false);
            Fantom_SetBalls(npchar, "pirate");
            npchar.CompanionEnemyEnable = false;  // ��� ������ ��� ��������
            pchar.questTemp.CompanionQuantity = makeint(sti(pchar.questTemp.CompanionQuantity) + 1);
            pchar.questTemp.CompanionQuantity.Jackman = true;
            NextDiag.TempNode = "Have_hot_bussines";
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;

  		break;

	 	case "Have_hot_bussines":   // ���� ������� ����� ��������� ��� ����������, �� ����, ������ ��. �� �� ������ ������.
			dialog.text = "���� ������, ����� �������� ������ ���!";
            link.l1 = "��������!";
			link.l1.go = "exit";
            NextDiag.TempNode = "Have_hot_bussines";
  		break;

	 	case "Gold_found_in_fort":    // ���� ��������� � ����� ������
            if (CheckAttribute(pchar, "questTemp.CompanionQuantity.Jackman") && !IsCompanion(characterFromID("Jackman")))
    		{
            	dialog.text = "�� ��� ����� - ��� ������ ��� �������� ���������� ���������! �� ��� �� ����, ������� ����� � ������ ������� ����� ����� �� ���� � �� ���������� ������� ��� �� ����."+
                              "� ������������ �� ����� ��� ����� ������. ���, ������� �������, ���������� �������� ������. �� �� ��� ����, ������ ���� �����. ��� � ������ ����� ������ ����!!!"+
                              "������ ���, � ���� ����. ��� �� ����� ����� �������.";
            }
            else
            {
    			dialog.text = "������� �������, ���������� �������� ������. �� �� ��� ����, ������ ���� �����. ��� � ������ ����� ������ ����!!!"+
                              "������ ���, � ���� ����. ��� �� ����� ����� �������.";
            }
            link.l1 = "� � ��� �� ������!";
			link.l1.go = "Step_8";
        break;

	 	case "Step_8":
			dialog.text = "��-��... �� - ������� ������, " + pchar.name + " " + pchar.lastname + ". � ����� ���� �����, ����.\n"+
                          "��� ���������� ������ ����� ����� ������� ������. ����, ��� �� �������� ������ ���� ���� ������.";
			link.l1 = "��� ��������!";
			link.l1.go = "Step_9";
        break;

	 	case "Step_9":
            LAi_SetActorType(npchar);
            LAi_ActorRunToLocation(npchar, "reload", "reload1_back", "none", "", "", "", 15.0);
            DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
        break;
        //********************** ����������� ������� ***********************
 		case "HolLine8_quest":
			dialog.text = "������ ����, ����� ��������� �� ���?";
			link.l1 = "� ������ ���� �� ����, ��� ������ ������, ������� ������� ������� ��������� ������.";
			link.l1.go = "Step_H8_1";
  		break;
 		case "Step_H8_1":
			dialog.text = "����, ���� ���-��... ��������� �� ����... �� ���-�� ����� ���������� ������ ������, �������, ��� ������������...";
			link.l1 = "� ���?";
			link.l1.go = "Step_H8_2";
  		break;
 		case "Step_H8_2":
			dialog.text = "�� ������. �� �����, �������� ������� ���... �� � �� �� ������ ��� ���� - � ������� ������������ ���-�����!";
			link.l1 = "��, � � ����� �������?";
			link.l1.go = "Step_H8_3";
  		break;
 		case "Step_H8_3":
			dialog.text = "��� � ���� �� ������.";
			link.l1 = "�������... �������, �������.";
			link.l1.go = "exit";
			AddQuestRecord("Hol_Line_8_SeekBible", "3");
			pchar.questTemp.State = "SeekBible_toFFBrothel";
			NextDiag.TempNode = "First time";
  		break;
		//********************* ����������� ������� *********************
        case "FraLine8_talk":
            dialog.text = "���� ��, ������� " + GetFullName(pchar) + " ���������! ������ ���! ���������� � � �� ����, ��� ���� �� �������� �������� ���-�� ����������.";
            link.l1 = "� �����, ������ �������� ����������� � �� ��������. ����� ������ ���������� �� ������� � �������� ����� ����� ������� � ����������.";
            link.l1.go = "Step_F8_1";
        break;
        case "Step_F8_1":
            dialog.text = "�� ��, ������������� ������ �����������. ��� �����, ��� � � ��� �� ��������� ����������� � ��� ����. ��������� ������� - ��������, �� ������� ������ ������ ������ �����. ��� ��� ���� ����������� ��������� ���� ������� ������ ����.";
            link.l1 = "�� ������. �� ������� ����, � ����� � ����� ������������ �� ���� �����, � �� �� ������� ���.";
            link.l1.go = "exit";
			pchar.questTemp.Count = makeint(pchar.questTemp.Count)+1;
			NextDiag.TempNode = "I_know_you_good";
			AddQuestRecord("Fra_Line_8_ThreeCorsairs", "5");
        break;
		//********************* ��������� ������� *********************
		case "PL_Q8_deck":
			dialog.text = "�� ��� ������� ����� � ���� �� ������. ��� ����� ������ ���� �� ����� �����!";
			link.l1 = "��, � �����...";
			link.l1.go = "exit";
			NextDiag.TempNode = "PL_Q8_deck_1";
		break;
		case "PL_Q8_deck_1":
			dialog.text = "��, ����� �� ����� �������, �� ����� � ����...";
			link.l1 = "��� � �������, �� ���������, �������.";
			link.l1.go = "exit";
			NextDiag.TempNode = "PL_Q8_deck_1";
		break;

		case "PL_Q8":
			dialog.text = "����, �������...";
			link.l1 = "��� ������� �� ����� ������?";
			link.l1.go = "PL_Q8_1";
		break;
		case "PL_Q8_1":
			dialog.text = "������ �� �����. �� � ������, � ��������� � ����������. ��� ��� � ����� ������ �� ��� ������ �� ����.";
			link.l1 = "����, ��� �� ���?!";
			link.l1.go = "PL_Q8_2";
		break;
		case "PL_Q8_2":
			dialog.text = "� ��� ���! � �����, � ���� ����� ���� ������!";
			link.l1 = "��, ����...";
			link.l1.go = "exit";
			NextDiag.TempNode = "I_know_you_good";
			pchar.questTemp.piratesLine = "Panama_inEngland";
		break;
// ======================== ���� ��� angry ===============>>>>>>>>>>>>>>>
		case "AngryRepeat_1":
            dialog.text = RandPhraseSimple("����� ��� ������!", "��� �� ����� ����!");
			link.l1 = "��...";
		    link.l1.go = "AngryExitAgainWithOut";
            if (CheckAttribute(npchar, "angry.terms")) //����������� � ��������� ����� 10 ����.
            {
                if (GetNpcQuestPastDayParam(npchar, "angry.terms") > sti(npchar.angry.terms))
                {
         			dialog.text = "��� �� ����� ��������� ���� �� ���� ������� �����������. ������ ��������� ����� ���� ����� ���������...";
        			link.l1 = "� �����, ������.";
        			link.l1.go = NextDiag.TempNode;
        			CharacterDelAngry(npchar);
                }
            }
    	break;
		case "AngryRepeat_2":
            dialog.text = RandPhraseSimple("�� ���� ������� ������, �������� �������.", "� �� ���� � ����� ��������, ��� ��� ���� ����� ���� �� ����������.");
			link.l1 = RandPhraseSimple("�� ��� ������...", "��, �� ��� ��...");
		    link.l1.go = "AngryExitAgain";
            if (CheckAttribute(npchar, "angry.terms")) //����������� � ��������� ����� 10 ����.
            {
                if (GetNpcQuestPastDayParam(npchar, "angry.terms") > sti(npchar.angry.terms))
                {
         			dialog.text = "�������, ������ �� �� ������ �������� ���� ������� �����������, ����� ��� �������� ���� �����. ���������, ��� ��� ����� ����� ���������.";
        			link.l1 = "�������, �� ������ ���� ������ - �� ����...";
        			link.l1.go = NextDiag.TempNode;
        			CharacterDelAngry(npchar);
                }
            }
    	break;
		case "AngryExitAgain":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok");
		break;
		case "AngryExitAgainWithOut":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok");
            DoReloadCharacterToLocation("Pirates_town","reload","reload3");
		break;
// <<<<<<<<<<<<============= ���� ��� angry =============================
	}
}
