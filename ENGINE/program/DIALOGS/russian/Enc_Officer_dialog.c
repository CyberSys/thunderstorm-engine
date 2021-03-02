// BOAL ����� ������ ������� � ���������� 21/06/06
void ProcessDialogEvent()
{
	ref NPChar, d;
	aref Link, Diag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);
	
	ref    sld;
	string attr;
	int    iTemp, iTax, iFortValue;
	
	switch(Dialog.CurrentNode)
	{
        case "First time":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = RandPhraseSimple("���, �������, �� ����� ��� �������� ��������� ��������. ������ ���� �� ����� ���� � ���� � �������!", "����������� � ���� ����������� �� �������? ��� ��, � ������ ���, ���...");
				link.l1 = RandPhraseSimple("�� ��� ������...", "�� � � �� ��������� ���� ��������."); 
				link.l1.go = "exit";
				break;
			}
			Diag.TempNode = "OnceAgain";
			dialog.text = "������ �� �����, �������?";
			if (FindFreeRandomOfficer() > 0)
			{
    			Link.l1 = "������? � �� ��� �� �����?";
    			Link.l1.go = "Node_2";
			}
			Link.l2 = "���, � ���� ������ ��������.";
			Link.l2.go = "Exit";			
		break;
		
		case "hired":
			Diag.TempNode = "Hired";
			// ������ ������ -->
			if (IsCompanion(NPChar))
			{
				Dialog.text = "������� �� ����� ������ � ����!";
				link.l2 = "��... � ���� ���.";
				link.l2.go = "Exit";
				break;
			}
			if (Npchar.location.group == "sit")
			{
				dialog.text = "������ � �����, �������, � ���������� �� ����. �� �������� ������������ - � ����� �� ��������.";
				Link.l1 = "������. ��������� - �������� ������ ������!";
				Link.l1.go = "Exit";
				break;
			}
			// ������ ������ <--
			dialog.text = "��� �� ������, �������?";
			if (isOfficer(Npchar)) // �������� ��� �������, ��� �������� ���
			{
	            Link.l2 = "������ ��� ������!";
	            Link.l2.go = "stay_follow";
            }
			Link.l4 = "�� ������. � �������� ��� ����� �����!";
			Link.l4.go = "AsYouWish";

            // boal ����� � �������
            /*if (CheckNPCharQuestAttribute(Npchar, "officertype", XI_ConvertString("treasurer")))
            {
                Link.l8 = "��� ��� ������ �����, " + XI_ConvertString("treasurer") + ".";
                Link.l8.go = "QMASTER_1";
            } */
            // �� ��� ������� ��������� ����� �� ��� ���������  -->
            if (IsEntity(loadedLocation))
            {
                if (CheckAttribute(loadedLocation, "fastreload"))
                {
                    iTemp = FindColony(loadedLocation.fastreload);
                    if (iTemp != -1)
                    {
                        sld = GetColonyByIndex(iTemp);
                        if (sti(sld.HeroOwn) == true && !CheckAttribute(sld, "OfficerIdx"))
                        {
                            NPChar.ColonyIdx = iTemp;
							Link.l7 = "� �������� ���� ����������� ����� ������!";
                            Link.l7.go = "Gover_Hire";
                        }
                    }
                }
            }
            Link.l9 = "������. ������.";
            Link.l9.go = "Exit";
        break;
        
		case "ShowParam_exit":
			Diag.CurrentNode = "OnceAgain";
			NPChar.quest.meeting = true;
			DialogExit();
			
			PChar.SystemInfo.OnlyShowCharacter = true;
            LaunchCharacter(NPChar);
		break;
		
		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = true;
			DialogExit();			
		break;

		case "exit_hire":
			Pchar.questTemp.HiringOfficerIDX = GetCharacterIndex(Npchar.id);
			AddDialogExitQuestFunction("LandEnc_OfficerHired");
			Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = true;
			DialogExit();
		break;

		case "exit_fire":
			//navy -->
			if (CheckAttribute(NPChar, "PGGAi"))
			{
				pchar.questTemp.FiringOfficerIDX = NPChar.index;
				AddDialogExitQuestFunction("PGG_FireOfficer");
				DialogExit();			
				break;
			}
			//navy <--
			Diag.TempNode = "Fired";
			Pchar.questTemp.FiringOfficerIDX = GetCharacterIndex(Npchar.id);
			AddDialogExitQuestFunction("LandEnc_OfficerFired");

			Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = true;

			DialogExit();			
		break;
		
		// boal 29.05.04 ������ ����� ������� -->
        case "WantToGo":
			chrDisableReloadToLocation = false;
			Diag.TempNode = "Hired";
			dialog.text = LinkRandPhrase("�������, ��� ���� ��� ����: ����������, �������, ���� �����, ���� ��, ������ ����������, �� ���� �������. ���� ������ ����� ������� �������. �������, ���� ���������!",
                           "�������, �� ����� �������� � ����, �������, �� �����, �� ���� � � ���� �����-�� ��������. ����� �������, ������� ������. � �� ��� ����� �����. �� �� ��������� ����. ���� ������, ��� ���������� �������.",
                           "�������, ������, �� ���-�� ����� � ��� ����� �� ����� �������. ����� ����� ����������. ���� �������� �� ��������� �����. �� �� ��� ���� �������� ���-������. ��� ����?");
			Link.l1 = LinkRandPhrase("���� � ��� ���� ������� ����������. ��� ����������. ��, ������� ���� � ������ �� ����� - �����.",
			                         "������� �� �������, � � �� ��������. ���, ����� ���� �������.",
			                         "�� ������ �� ����. ����, ����. �����, �����, �� �����: � � �����.");
			Link.l1.go = "WantToGo_free";
			Link.l2 = LinkRandPhrase("���, ����, ������. �� ������ � ����. ����� ����������.",
			                         "������ �� ������, �� � ���� ����� � �����. �� ������� �� ����? ��� ��� � ������ ����� ������.",
			                         "������� ������� �� �������. ������� ������ ��� � � ���� ������ ������ �� �����.");
			Link.l2.go = "WantToGo_Stay";
		break;
		
		case "WantToGo_Stay":
			dialog.text = "� ��� �� �� ���. � ������� ������������� �� ������. ���� �� � ���� ��� ����������. � ������ ������, ����� ��� �������!";
			Link.l1 = "�� ���� ����� �� ����. ��� �� ���� ��������?";
			Link.l1.go = "WantToGo_Stay_2";
			Link.l2 = LinkRandPhrase("���� � ��� ���� ������� ����������. ��� ����������. ��, ������� ���� � ������ �� ����� - �����.",
			                         "������� �� �������, � � �� ��������. ���, ����� ���� �������.",
			                         "�� ������ �� ����. ����, ����. �����, �����, �� �����: � � �����.");
			Link.l2.go = "WantToGo_free";
		break;
		
		case "WantToGo_Stay_2":
			dialog.text = "������, "+sti(NPChar.rank)*500+" ������� ����� ������ �� ���� ���� ������ �������.";
            if (sti(Pchar.money) >= sti(NPChar.rank)*500)
            {
        		Link.l1 = "������������.";
        		Link.l1.go = "WantToGo_Stay_ForMoney";
			}
			Link.l2 = "��� ���! �������? �� � �� ����� ������ ���� �������� �����.";
        	Link.l2.go = "WantToGo_free";

		break;
		case "WantToGo_Stay_ForMoney":
            Diag.TempNode = "Hired";
            NPChar.greeting           = "Gr_Officer";
            AddMoneyToCharacter(Pchar, -(makeint(sti(NPChar.rank)*500)));
            Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = true;
			DialogExit();
			NPChar.location = "none";
			LAi_SetOfficerType(NPChar);
		break;
		
		case "WantToGo_free":
            Diag.TempNode = "WantToGo_free_Yet";
			//navy -->
			if (CheckAttribute(NPChar, "PGGAi"))
			{
				pchar.questTemp.FiringOfficerIDX = NPChar.index;
				AddDialogExitQuestFunction("PGG_FireOfficer");
				Diag.CurrentNode = Diag.TempNode;
				DialogExit();			
				break;
			}
			//navy <--
		    Pchar.questTemp.FiringOfficerIDX = NPChar.index;
			AddDialogExitQuestFunction("LandEnc_OfficerFired");
			
			Diag.CurrentNode = Diag.TempNode;
			NPChar.quest.meeting = true;
			DialogExit();
		break;
		
		case "WantToGo_free_Yet":
            Diag.TempNode = "WantToGo_free_Yet";
			dialog.text = "��.. ��� � ������ ������ ��������� ������...";
			Link.l1 = "��-��, ��������� ���� �������.";
			Link.l1.go = "exit";
		break;
		// boal 29.05.04 ������ ����� ������� <--
		
		// �������� �������� ������� � ��������� -->
		case "WantToRemove":
            chrDisableReloadToLocation = false;
			if (Npchar.alignment == "good")
            {
			    dialog.text = "�������, ��� �������� �������! � ��� � ���� ���������� ������ ������� � '�������' � ���� �����. � ����� �� ���.";
			}
			else
			{
                dialog.text = "�������, ��� ��������, ��� � ����� ���������� �������. � ��? ��� �� ����������? � �� ���� ������ ����� � �����.";
			}
			Link.l1 = "���?!! �� � �������� �� ���� ������!";
			Link.l1.go = "WantToGo_free";
		break;
		// �������� �������� ������� � ��������� <--
		
		case "Node_2":
			dialog.text = NPCharRepPhrase(Npchar, RandSwear() + "�� � ������ "+ XI_ConvertString(Npchar.quest.officertype) + " � ���� �����. " + Npchar.quest.officertype_2 + "����� ������� ��� ������, ����� �� ������.",
                                    "��, �������, ��� � �������� " + XI_ConvertString(Npchar.quest.officertype) + ". " + Npchar.quest.officertype_2 + "�������� ���� �� ���� �����, ������� - ��������� � ���� ����.");
			Link.l1 = "������� �� ������?";
			Link.l1.go = "price";
			Link.l2 = "� ��� �� �� ���� ������������� ��� ����������?";
			Link.l2.go = "ShowParam_exit";
			Link.l3 = "���, � ���� ��� ���� " + XI_ConvertString(Npchar.quest.officertype) + ".";
			Link.l3.go = "Exit";
		break;

		case "OnceAgain":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = RandPhraseSimple("���, �������, �� ����� ��� �������� ��������� ��������. ������ ���� �� ����� ���� � ���� � �������!", "����������� � ���� ����������� �� �������? ��� ��, � ������ ���, ���...");
				link.l1 = RandPhraseSimple("�� ��� ������...", "�� � � �� ��������� ���� ��������."); 
				link.l1.go = "exit";
				break;
			}
			Diag.TempNode = "OnceAgain";
			dialog.text = "������� ����������, �������? ������, ��� ����� " + XI_ConvertString(Npchar.quest.officertype) + " ��� �� ��������?";
			// boal -->
			if (FindFreeRandomOfficer() > 0)
			{
                Link.l1 = "�������, ���. ������� �� ������?";
                Link.l1.go = "price";
			}
			Link.l2 = "����� �� ������ � ����?";
			Link.l2.go = "ShowParam_exit";
			// boal <--
			Link.l3 = "� ���� ������ ��������. ��������� ����������.";
			Link.l3.go = "exit";
		break;

		case "price":
			dialog.text = "�����, �� � ���� �������� �� " + Npchar.quest.OfficerPrice + " ��������.";
			Link.l1 = "�� ������� �� �� ����� ������?";
			Link.l1.go = "trade";
			Link.l2 = "� ��������. ������ ���� ����������� � �������.";
			Link.l2.go = "hire";
			Link.l3 = "�� �� ������ ���� �����. ������.";
			Link.l3.go = "exit";
		break;

		case "trade":
			if (GetSummonSkillFromNameToOld(pchar, SKILL_COMMERCE) >= Rand(12) && (sti(Npchar.quest.OfficerPrice) - GetCharacterSkillToOld(Pchar, "commerce")*100) >= sti(Npchar.quest.OfficerLowPrice))// bug fix
			{
				Npchar.quest.OfficerPrice = makeint(makeint(Npchar.quest.OfficerPrice) - GetCharacterSkillToOld(Pchar, "commerce")*100); // ����� ����� ��������� ����� �����
				dialog.text = "������...�������, � ����� ����������� ���� �� " + Npchar.quest.OfficerPrice + " ��������. �� ���?";
				Link.l1 = "��� ��� ��� �����. �� ������.";
				Link.l1.go = "hire";				
				Link.l2 = "� ��-�������� ������, ��� ��� ��������. ������.";
				Link.l2.go = "exit";
			}
			else
			{
				dialog.text = "���, �������, � ���� ������ �������, ������� ��������. ���� ��� ������� ������ ��� ��� - ������� ����-������ �������.";
				Link.l1 = "�� �����, � ��������. � ���� ����.";
				Link.l1.go = "hire";				
				Link.l2 = "��� � � ��������. ����� ��������.";
				Link.l2.go = "exit";
			}
		break;

		case "hire":
			if(makeint(Pchar.money) >= makeint(Npchar.quest.OfficerPrice))
			{
				AddMoneyToCharacter(Pchar, -(makeint(Npchar.quest.OfficerPrice)));
				Diag.TempNode = "OnboardSoon";
				dialog.text = "��������� ���, �������. �� �� ���������, ��� ������ ��� ��� ������.";
				Link.l1 = "������� � ��� ������.";
				Link.l1.go = "Exit_hire";								
			}
			else
			{
				dialog.text = "��, ������, � ��� �������� � �����������! ��������, �������, �� � �� ������� � ������.";
				Link.l1 = "�� ��, ����!";
				Link.l1.go = "Exit";								
			}
		break;

		case "OnboardSoon":			
			Diag.TempNode = "OnboardSoon";
			dialog.text = "������ � �����, �������, � ���������� �� ����. �� �������� ������������ - � ����� �� ��������.";
			Link.l1 = "������. ��������� - �������� ������ ������!";
			Link.l1.go = "Exit";											
		break;
        
        case "QMASTER_1":
            dialog.text = "������������ ���������� ���� � ����� " +
                     FloatToString(50.0 / (2.0+GetSummonSkillFromNameToOld(PChar, SKILL_REPAIR) + GetSummonSkillFromNameToOld(PChar,SKILL_SNEAK)), 1) +
                     "% �� ���������� �����. �� "+GetCrewQuantity(PChar) + " �������� ����� " + makeint((GetCrewQuantity(PChar)+6) / 10) + " ��������� � ����. ��� ��� ����� ����������� �����.";
            Link.l1 = "�������.";
            Link.l1.go = "Exit";
            Diag.TempNode = "Hired";
        break;
        
        // boal 05.09.03 offecer need to go to abordage -->
        case "stay_follow":
            dialog.text = "����� ����� ����������?";
            Link.l1 = "���� �����!";
            Link.l1.go = "Boal_Stay";
            Link.l2 = "������ �� ���� � �� ��������!";
            Link.l2.go = "Boal_Follow";
            // boal 05.09.03 offecer need to go to abordage <--
		break;
		
		case "AsYouWish":
			// �������� �� ���� -->
			if (bSeaActive)
			{
				attr = Sea_FindNearColony();
				if (attr == "none")
				{
					dialog.text = "��� ���� �����, �� ������ �������� ���� �� ������-������ ������.";
					Link.l1 = "������, ��� � �������. ���� ��������� �� ������.";
					Link.l1.go = "Exit";	
					break;	
				}
			}
			// �������� �� ���� <--
			dialog.text = "��� �������, �������. �� ��������: ���� �� �������� ���� - � ������ ������� �� ������� �� ���� �����. ��� ��� ������ ���.";
			Link.l1 = "�������, � ���������. �� ��� ��� �����.";
			Link.l1.go = "Exit";								
			Link.l2 = "�������. ������ �����������, ���� ��������.";
			Link.l2.go = "Exit_Fire";		
		break;
		
		case "Fired":
		      Diag.TempNode = "Fired";
		      dialog.text = "� �� ������������ ���, �������, ��� �� ������� �� ��� �������. � ���� ���� ���� ��������.";
		      Link.l1 = "�� �� ��� � �� �����!";
		      Link.l1.go = "Exit";
	      break;
        // boal 05.09.03 offecer need to go to abordage -->
	      case "Boal_Stay":
             //SetCharacterTask_Stay(Characters[Npchar.index]); // it's a mistic but here doesn't work :(
             //Log_SetStringToLog(Npchar.id +" "+Npchar.index);
              Pchar.questTemp.HiringOfficerIDX = GetCharacterIndex(Npchar.id);
              AddDialogExitQuestFunction("LandEnc_OfficerStay");
		      Diag.TempNode = "Hired";
		      dialog.text = "���� �������� ����������!";
		      Link.l1 = "������.";
		      Link.l1.go = "Exit";
		      Npchar.chr_ai.tmpl = LAI_TMPL_STAY;
	      break;
	      case "Boal_Follow":
		      SetCharacterTask_FollowCharacter(Npchar, PChar); // it works here!!!
		      Diag.TempNode = "Hired";
		      dialog.text = "���� �������� ����������!";
		      Link.l1 = "������.";
		      Link.l1.go = "Exit";
	      break;
        // boal 05.09.03 offecer need to go to abordage <--

		//////////////////////////////    ������-��������� -->
		case "Gover_Hire":
            dialog.Text = "����� �� �����!";
            Link.l1 = "����� �� �������� � ������������ ������, � ���� ������ ���������� �� ��������� ��������.";
			Link.l1.go = "Exit_Gover_Hire";
            Link.l2 = "� ���������.";
            Link.l2.go = "Exit";
            Diag.TempNode = "Hired";
        break;

        case "Exit_Gover_Hire":
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
            attr = sld.id + "_townhall";

            // ������ ��������� -->
			CheckForReleaseOfficer(sti(NPChar.index));
			RemovePassenger(pchar, NPChar);
			// ������ ��������� <--
		    sld.OfficerIdx = sti(NPChar.index);
            NPChar.location = attr;
            NPChar.location.group = "goto";
			NPChar.location.locator = "governor1";
			LAi_SetHuberStayTypeNoGroup(NPChar);
            SaveCurrentNpcQuestDateParam(NPChar, "StartTaxDate");
            SaveCurrentNpcQuestDateParam(NPChar, "GoverTalkDate");
            LAi_LoginInCaptureTown(NPChar, true);
			//  �� -->
			ReOpenQuestHeader("Gen_CityCapture");
            AddQuestRecord("Gen_CityCapture", "t3_1");
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + sld.id));
			AddQuestUserData("Gen_CityCapture", "sName", GetFullName(NPChar));
			//  �� <--
			Diag.CurrentNode = "Gover_Main";
            DialogExit();
        break;

        case "Gover_Main":
            iTemp = GetNpcQuestPastDayParam(NPChar, "GoverTalkDate");
            if (iTemp > 0)
            {
				dialog.Text = "����� ����� ����������? ��������� ��� ����� ��� " + FindRussianDaysString(iTemp) + " �����.";
			}
			else
			{
			    dialog.Text = "����� ����� ����������?";
			}

            Link.l1 = "����� ����� ������� �� ������ �� ������ ������?";
        	Link.l1.go = "Gover_Tax";
            Link.l8 = "�� ����� ��� �� �������, � ������ ���� � ��������� ������.";
            Link.l8.go = "Gover_Fire";
			Link.l9 = "����� ��������.";
            Link.l9.go = "Exit";
            Diag.TempNode = "Gover_Main";
            SaveCurrentNpcQuestDateParam(NPChar, "GoverTalkDate");
        break;

        case "Gover_Tax":
            iTemp = GetNpcQuestPastDayParam(NPChar, "StartTaxDate");
            iTax  = makeint((GetCharacterSkillSimple(NPChar, SKILL_COMMERCE) + GetCharacterSkillSimple(NPChar, SKILL_LEADERSHIP)) / 2);
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
			if (CheckAttribute(sld, "FortValue"))
			{
			    iFortValue = sti(sld.FortValue);
			}
			else
			{
			    iFortValue = 20;
			}
			NPChar.Gover_Tax_Sum = iFortValue*iTax*iTemp;
			dialog.Text = "��� ������ ��������� ��� �������� " + FindRussianMoneyString(iFortValue*iTax) + " � ����. �� " + FindRussianDaysString(iTemp) + " � ������ " + FindRussianMoneyString(sti(NPChar.Gover_Tax_Sum)) + ".";
			if (sti(NPChar.Gover_Tax_Sum) > 0)
			{
			    Link.l1 = "� ���� ������� ��� ����� �������.";
            	Link.l1.go = "Gover_Tax_Get";
			}
			Link.l2 = "��������� �� ������!";
            Link.l2.go = "Exit";
        break;

        case "Gover_Tax_Get":
            SaveCurrentNpcQuestDateParam(NPChar, "StartTaxDate");
            AddMoneyToCharacter(Pchar, sti(NPChar.Gover_Tax_Sum));

			Diag.CurrentNode = "Gover_Main";
            DialogExit();
        break;

        case "Gover_Fire":
            dialog.Text = "������������! ���� ���������� ������ �� ��� ����.";
            Link.l8 = "��� � ������.";
            Link.l8.go = "exit_hire";
            Diag.TempNode = "Hired";
            sld = GetColonyByIndex(sti(NPChar.ColonyIdx));
            DeleteAttribute(sld, "OfficerIdx");
            //  �� -->
			ReOpenQuestHeader("Gen_CityCapture");
            AddQuestRecord("Gen_CityCapture", "t3_2");
			AddQuestUserData("Gen_CityCapture", "sCity", XI_ConvertString("colony" + sld.id));
			AddQuestUserData("Gen_CityCapture", "sName", GetFullName(NPChar));
			//  �� <--
        break;
        //////////////////////////////    ������-��������� <--
	}
}
