// BOAL ������ ��������� - � �����
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	ref offref;
    int i, cn;
    int qty;
    string attrLoc;
    
	switch(Dialog.CurrentNode)
	{
		// -----------------------------------������ ������ - ������ �������
		case "First time":
			if (sti(NPChar.nation) == PIRATE)
			{
                dialog.text = RandSwear() + "� ��� �������, " + GetAddress_Form(NPChar) + ". �� ������ �������, ��� " + NationNameNominative(sti(NPChar.nation)) + " ������� �� �������� ����� -  ������ ��� �� ����.";
				link.l2 = "���... �� ���� �� ����������, ������ ���� ����� ���� ��� ����������� � �����-������ ����� �� �����. � ����� ����� ���� � ���� ��������?";
			    link.l2.go = "free_to_officer";
			}
			else
			{
			    dialog.text = RandSwear() + "� ��� �������, " + GetAddress_Form(NPChar) + ". " + NationNameNominative(sti(NPChar.nation)) + " �������� ������� ����� �� ��� �������.";
			}
			attrLoc = Sea_FindNearColony();
			if (attrLoc != "none") 
			{
                link.l1 = "�� ������ ���� ������ " + GetConvertStr(attrLoc +" Town", "LocLables.txt") + ". � ��� �� ���� ��� ��������.";
			    link.l1.go = "free_withoutFee";
			}
			else
			{
                link.l1 = "� ��� ������� � ����� �������� ����������. � �������� ���� �� ��� ������ �������, " + GetFullName(NPChar) + ". ������ ��������� � ����� �������.";
			    link.l1.go = "free_in_sea";
			}
			link.l3 = "��� �� ������ �������� - ��� ����� ������� ����!";
			link.l3.go = "Slave_1";
			link.l4 = "�������, ���� ���� �� ��������!!";
			link.l4.go = "PunishmentAction";
			link.l99 = "� � �����!";
			link.l99.go = "exit";
			NextDiag.TempNode = "first time";
		break;
		
		case "PunishmentAction":
            DialogExit();
            NextDiag.CurrentNode = "first time";
            pchar.GenQuest.PunishmentActionManType = "LAi_SetStayType";
            Pchar.GenQuest.FreeRandomOfficerIdx = NPChar.index;

            AddDialogExitQuest("PunishmentAction");
            AddCharacterExpToSkill(Pchar, "Leadership", -150);
            ChangeCharacterReputation(pchar, -0.2);
        break;
		
        case "Slave_1":
			dialog.text = "�� " + GetAddress_Form(NPChar) + ", � �� ������ �� ���� ������� � ��� � ������� ������ ������.";
            link.l1 = "���!? ��� ��� ���� ������ ���� ������. ���� ���� �����!";
			link.l1.go = "Slave_2";
			link.l99 = "�... �� �����, ���� ���� � �����.";
			link.l99.go = "exit";
		break;
		
		case "Slave_2":
            OfficersReaction("bad");
            if (rand(5) == 1)
            {
    			dialog.text = "���!! ���!! � ����� ������� ����� ����� �����, ��� ����� �����!";
                link.l1 = "���������! � ����������! ��.. ���-������ ������� ��� ��� ���� ������.";
    			link.l1.go = "free_in_sea_4";
			}
            else
            {
                if (rand(1) == 1)
                {
                    if (GetPrisonerQty() > 1 && rand(1) == 1)
                    {
                        dialog.text = "� ��-��, �����, ������� ����� '�������' ��� ��� ������� �� ����� ����!!";
                        link.l1 = "���!? ����!? ���������!";
            			link.l1.go = "free_in_sea_battle_all";
            			break;
                    }
                    dialog.text = "� ��������� ������� � ���!";
                    link.l1 = "� � ������������� ����������� ���� ����� �����������!";
        			link.l1.go = "free_in_sea_battle_1";
    			}
    			else
    			{
                    dialog.text = "������. ��� ������, �� ��� ������ �� �������� - � ��� ���!";
                    link.l1 = "��� � ������. ����� ���������� �� ���� � �������� ������ �����.";
        			link.l1.go = "Slave_3";
    			}
			}
		break;
		
		case "Slave_3":
            ReleasePrisoner(NPChar); // ���������� ��������
        	LAi_SetActorType(NPChar);
        	LAi_ActorRunToLocation(NPChar, "reload", "reload1", "none", "", "", "", 4.0);
        	ChangeCharacterReputation(pchar, -6);
        	DialogExit();
        	AddCharacterGoodsSimple(pchar, GOOD_SLAVES, 1);
        break;
        
        case "free_in_sea":
			dialog.text = "��, " + GetAddress_Form(NPChar) + ", �� �� � �������� ����!!! ���� � �����?";
            link.l1 = "� ���-�� ���! � ������. �� ��������. �������� �����!";
			link.l1.go = "free_in_sea_2";
			link.l99 = "�.. �� �����, ���� ���� � �����.";
			link.l99.go = "exit";
		break;
		
		case "free_in_sea_2":
			dialog.text = "��� �� ����������� ���� �����. �� ������ ������ ������� ���� �� ���� ������?";
            link.l1 = "��� �������, ��������� �� �� ������ ��� ���! ��� � ����� �������!";
			link.l1.go = "free_in_sea_3";
			link.l99 = "�������, �� ����. ��������� ����.";
			link.l99.go = "exit";
		break;

		case "free_in_sea_3":
            OfficersReaction("bad");
            if (rand(5) == 1)
            {
    			dialog.text = "���!! ���!! � ����� ������� ����� ����� �����, ��� ���� ������� ����!";
                link.l1 = "���������! � ����������! ��.. ���-������ ������� ��� ��� ���� ������.";
    			link.l1.go = "free_in_sea_4";
			}
            else
            {
                if (rand(1) == 1)
                {
                    if (GetPrisonerQty() > 1 && rand(1) == 1)
                    {
                        dialog.text = "� ��-��, �����, ������� ����� '�������' ��� ��� ������� �� ����� ����!!";
                        link.l1 = "���!? ����!? ���������!";
            			link.l1.go = "free_in_sea_battle_all";
            			break;
                    }
                    dialog.text = "� ��������� ������� � ���!";
                    link.l1 = "� � ������������� ����������� ���� ����� �����������!";
        			link.l1.go = "free_in_sea_battle_1";
    			}
    			else
    			{
                    dialog.text = "������. � �����. ���� �� ��� �����-������ ���������� � ������!";
                    link.l1 = "��-��. �������� �� ���������� ������?";
        			link.l1.go = "free_in_sea_5";
    			}
			}
		break;
		
		case "free_in_sea_battle_1":
            ReleasePrisoner(NPChar); // ���������� ��������
        	
        	LAi_LocationFightDisable(&Locations[FindLocation("My_Deck")], false);
            LAi_SetFightMode(Pchar, true);
            LAi_SetWarriorType(NPChar);
            LAi_group_MoveCharacter(NPChar, LAI_GROUP_TmpEnemy);
            LAi_group_SetHearRadius(LAI_GROUP_TmpEnemy, 100.0);
            LAi_group_FightGroupsEx(LAI_GROUP_TmpEnemy, LAI_GROUP_PLAYER, true, Pchar, -1, false, false);
            LAi_group_SetRelation(LAI_GROUP_TmpEnemy, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
            LAi_group_SetCheck(LAI_GROUP_TmpEnemy, "My_Deck_Battle_End");
            LAi_group_SetRelation(LAI_GROUP_TmpEnemy, "Prisoner", LAI_GROUP_NEITRAL);
        	DialogExit();
        break;
        
        case "free_in_sea_battle_all":
            for(i=0; i < GetPassengersQuantity(pchar); i++)
            {
                cn = GetPassenger(pchar,i);
                if(cn != -1)
                {
                    offref = GetCharacter(cn);
                    if (CheckAttribute(offref,"prisoned"))
                    {
        	            if(sti(offref.prisoned)==true && GetRemovable(offref)) // ������ ������ ��������
        	            {
                        	ReleasePrisoner(offref); // ���������� ��������
                            LAi_SetWarriorType(offref);
                            LAi_group_MoveCharacter(offref, LAI_GROUP_TmpEnemy);
                        }
                    }
                }
            }
            LAi_LocationFightDisable(&Locations[FindLocation("My_Deck")], false);
            LAi_SetFightMode(Pchar, true);

			LAi_group_SetHearRadius(LAI_GROUP_TmpEnemy, 100.0);
            LAi_group_FightGroupsEx(LAI_GROUP_TmpEnemy, LAI_GROUP_PLAYER, true, Pchar, -1, false, false);
            LAi_group_SetRelation(LAI_GROUP_TmpEnemy, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	            
            LAi_group_SetCheck(LAI_GROUP_TmpEnemy, "My_Deck_Battle_End");

        	DialogExit();
        break;
        
		case "free_in_sea_4":
        	ReleasePrisoner(NPChar); // ���������� ��������
            ChangeCharacterReputation(pchar, -2);
            NPChar.location = "";
	        LAi_KillCharacter(NPChar);
        	DialogExit();
        break;
        
        case "free_in_sea_5":
            ReleasePrisoner(NPChar); // ���������� ��������
        	LAi_SetActorType(NPChar);
        	LAi_ActorRunToLocation(NPChar, "reload", "reload1", "none", "", "", "", 4.0);
        	ChangeCharacterReputation(pchar, -2);
        	DialogExit();
        break;
        
		case "free_withoutFee":
			dialog.text = "�, " + GetAddress_Form(NPChar) + ", �� ����� �����, ��� ���������� ���� ��� ������?";
            link.l1 = "��, ��������. �� ��������, " + GetFullName(NPChar);
			link.l1.go = "free_withoutFee_2";
			link.l99 = "��.. �� �� ����! � �� ����� ����, ��� �� ���������� ����� ������. ������-�� � ����� ���.";
			link.l99.go = "exit";
		break;
		
		case "free_withoutFee_2":
			dialog.text = "�������, "+GetFullName(PChar) + "!!!";
            link.l1 = "�����, ����������, ���� � �� ���������.";
			link.l1.go = "free_withoutFee_3";
			OfficersReaction("good");
		break;
		
		case "free_withoutFee_3":
            ReleasePrisoner(NPChar); // ���������� ��������
        	LAi_SetActorType(NPChar);
        	LAi_ActorRunToLocation(NPChar, "reload", "reload1", "none", "", "", "", 4.0);
        	ChangeCharacterReputation(pchar, 2);
        	DialogExit();
        break;
        
        case "free_to_officer":
            if (rand(12) > GetSummonSkillFromNameToOld(PChar, SKILL_LEADERSHIP) || FindFreeRandomOfficer() < 0)
            {
    			dialog.text = "�����������? �� �������� ������, " + GetAddress_Form(NPChar) + "! ������� � ���? ��� ��, ����� ������� ����!";
                link.l1 = "��-��! ������������, ���� ���� ��� � �������!";
    			link.l1.go = "exit";
			}
			else
			{
    			dialog.text = "��������, " + GetAddress_Form(NPChar) + "? �������, � ��������, �.�. ������� ������ � ���� ������ ���!";
                link.l1 = "������������! ��������� � ����� ������������ ����������.";
    			link.l1.go = "free_to_officer_Hire";
    			link.l2 = "� ����� �� ��� ������? � ���� � ��� ������ ��������.";
    			link.l2.go = "exit";
			}

		break;

        case "free_to_officer_Hire":
            NPChar.Dialog.Filename = "Enc_Officer_dialog.c";
            NPChar.greeting = "Gr_Officer";
            NPChar.loyality = 5 + rand(10);
		    if (sti(NPChar.reputation) > 41)
		    {
		        NPChar.alignment = "good";
		    }
		    else
		    {
		        NPChar.alignment = "bad";
		    }
            ReleasePrisoner(NPChar); // ���������� ��������
            DeleteAttribute(NPChar, "LifeDay") // ����������
			Pchar.questTemp.HiringOfficerIDX = Npchar.index;
			// ��� ���� ������������ ����� � ������� 290704 BOAL -->
			DeleteAttribute(Npchar, "Ship");
			Npchar.Ship.Type = SHIP_NOTUSED;
            DeleteAttribute(Npchar, "Fellows"); // ��� �������
            Npchar.Fellows.Passengers.id0 = Npchar.index; // ���� ��������

			Npchar.Fellows.Passengers.boatswain = "-1";
			Npchar.Fellows.Passengers.navigator = "-1";
			Npchar.Fellows.Passengers.doctor = "-1";
			Npchar.Fellows.Passengers.cannoner = "-1";
			Npchar.Fellows.Passengers.treasurer = "-1";
			Npchar.Fellows.Passengers.carpenter = "-1";
            
			AddDialogExitQuestFunction("LandEnc_OfficerHired");
			DialogExit();
        break;
        	
		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
		break;
	}
}