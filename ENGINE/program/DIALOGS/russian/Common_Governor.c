// boal 25/04/04 ����� ������ Governor
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Governor\" + NationShortName(sti(NPChar.nation)) + "_Governor.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
    
    ref offref, sld;
    int i, cn;
    int qty;
    string attrLoc;

    /// ����� �������
    ref CaptGovenor, FortColony;
    int f, colony_money;

    int k = 1000;
    if (CheckAttribute(Nations[sti(NPChar.nation)], "Fort"))
    {
        k = (300 - sti(Nations[sti(NPChar.nation)].Fort)*10);
    }
	bool ok;
	
	attrLoc = Dialog.CurrentNode;
	if (findsubstr(attrLoc, "CityGive_" , 0) != -1)
 	{
        i = findsubstr(attrLoc, "_" , 0);
	 	NPChar.quest.CityIdx = strcut(attrLoc, i+1, strlen(attrLoc)-1); // ����� � �����
 	    Dialog.CurrentNode = "ColonyGive";
 	}
 	
	switch (Dialog.CurrentNode)
	{
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			AddDialogExitQuest("MainHeroFightModeOff");
		break;
		
		case "fight":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
		break;
		
		case "First time":
            NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("�� ������� ������� � �������-�����������?! ������ ���...", "��� ��� ����������� ���������, ��� �� ��� �������� ����?! ��� �����������...", "�� ��, ��� ������ �� ����� �����, ��� �����-�� ���������� ������ � ���� � ����������..."), 
					LinkRandPhrase("��� ���� �����, �������?! ��� ������� ��� ����� ���� ����, ������ ���� �� ����, ������� �����!", "������� ������, ��� �� ���� ����������! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("�������� ���� ���� �����...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("������ ���� �����, ��������, � �� �� ����� ���� ������� ����!", "��� ��� � ���� �����, ��������: ���� ���� � ������ ����..."));
				link.l1.go = "fight";
				break;
			} 
			if (npchar.quest.meeting == "0")
			{
				dialog.text = "� ������, ��� �� ����� ���������� ������� ���������. ���� ����� " + GetFullName(npchar) +
                              ". � �������-���������� ������� " + NationNameGenitive(sti(NPChar.nation))+ ", ��������� ������ " + NationKingsName(npchar)+
                              " � ���� �����. � ������ ������������� �������� ���� ������ ������, " + GetAddress_Form(NPChar) + ".";
				link.l1 = "��� ��� " + GetFullName(pchar) + ".";
				link.l1.go = "node_1";
				npchar.quest.meeting = "1";
			}
			else
			{
				dialog.text = RandPhraseSimple("�, ��� ����� ��? �� ������ ������� ��� ����������� �������-���������� " + NationNameGenitive(sti(NPChar.nation))+ " �� ���� ���?",
                              "����� ��������� ���� �� ������ ��������������� ���? ��� ��� ������, " +GetAddress_Form(NPChar)+"?");
				link.l1 = "� ���� ���������� � ������ �� ����� ������ " + NationNameGenitive(sti(NPChar.nation));
				link.l1.go = "work";
				link.l2 = "����� ���������� � ���� �� ����� ����.";
				link.l2.go = "quests"; // ���� �����
				link.l10 = "����� �������� ����, �� ���� ���� ����.";
				link.l10.go = "exit";
			}
		break;
		
		case "node_1":
			dialog.text = "��� ����� ������� ��������� ��� ������ ���� � ��������� ���� �� ������ ��������������� ���?";
			link.l1 = "� ���� ���������� � ������ �� ����� ������ " + NationNameGenitive(sti(NPChar.nation));
			link.l1.go = "work";
			link.l2 = "����� ���������� � ���� �� ����� ����.";
			link.l2.go = "quests";
			link.l10 = "��� ������ ����� ���������� � ������ �����, "+GetAddress_FormToNPC(NPChar);
			link.l10.go = "node_2";
		break;

		case "node_2":
			dialog.text = "� ����� ������, � ������� ��� �������� ��� �������, � ����� �� ������ ��� ��������.";
			link.l1 = "��, ��, �������, ��������, ��� ���������� ���.";
			link.l1.go = "exit";
		break;
		
		case "work":
            if (!CheckCharacterItem(Pchar, "patent_" + NationShortName(sti(NPChar.nation))) || !CheckAttribute(pchar, "EquipedPatentId"))
			{
    			dialog.text = "� ��� ���������� �� ����, " + GetAddress_Form(NPChar) + ". �������� ��� �������. � ���� ���� ������ � ������, ���������� �� ������ " + NationKingsName(npchar) + ".";
    			link.l1 = "�������� �� ������������. ����� � ���� ����� ������ "+NationNameGenitive(sti(NPChar.nation))+", � �������.";
    			link.l1.go = "exit";
			}
			else
			{// ���� �������
                // ������ ��������
                if (isReadyToNextTitle(sti(NPChar.nation)))
                {
                    Items[sti(pchar.EquipedPatentId)].TitulCurNext = 0;
                    Items[sti(pchar.EquipedPatentId)].TitulCur = sti(Items[sti(pchar.EquipedPatentId)].TitulCur) + 1;

                    dialog.text = "� ������ ���������� ���"+ ", " + GetAddress_Form(NPChar)+". �������� � ����� ��������� �� ������ ��������� � ���������� " + NationKingsName(npchar) +
                                ". ��� ��������� ������ " + GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur)) +
                                " ����� " + NationNameGenitive(sti(NPChar.nation)) +".";
        			link.l1 = "�������! ������ � ������ � ������ ������� "+NationKingsCrown(npchar) + ".";
        			link.l1.go = "exit";
        			break
                }
                // ���� �������
                dialog.text = LinkRandPhrase("�� ������ ������� �� ���, "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+"?",
                                             "� ������ ���, " + GetAddress_Form(NPChar)+".",
                                             "��������, � ���� - ��������.");

                //GetAddress_Form(NPChar)+", � ���, ��� ����� ������ "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+" �� ������ " + NationKingsName(npchar) + ".";
				if (pchar.questTemp.NationQuest == "")
                {
	    			if (pchar.questTemp.piratesLine == "begin" || pchar.questTemp.piratesLine == "over" || pchar.questTemp.piratesLine == "waiting_Q8" || pchar.questTemp.LSC == "over")
					{
						link.l1 = "���� �� ����� ������ ��� �������������� �������?";
	    				link.l1.go = "work_1";
						if (!bWorldAlivePause && pchar.questTemp.State == "") //eddy. ������, ���� �� �������� �������.
						{
							link.l1.go = "onlyGenWork_Alter";
						}
					}
					else
					{
						link.l1 = "���� �� ����� ������ ��� �������������� �������?";
	    				link.l1.go = "onlyGenWork_2";
					}
    			}
    			else
    			{
    			    if (pchar.questTemp.NationQuest == NPChar.nation)
	                {
		    			link.l1 = RandPhraseSimple("� ������ �� ����, �������� ���������� � ������ �����������.", "�� ������ ����� �������...");
		    			link.l1.go = "work_1";
	    			}
	    			else
	    			{// �������� - ���
	    			    link.l1 = "���� �� ����� ������ ��� �������������� �������?";
	    				link.l1.go = "onlyGenWork_1";
	    			}
    			}

                // ����� ������ -->
				if (isHeroOwnCity(false))
				{
				    Link.l2 = "� ���� ��������� ����������� ���� ��������� ��� ���� "+ XI_ConvertString(GetNationNameByType(sti(NPChar.nation)) + "Gen") +".";
					Link.l2.go = "colony_capture";
				}
                // ����� ������ <--
                link.l9 = RandPhraseSimple("��������, � ����� �����.",
                                           "������ ����������, "+GetAddress_FormToNPC(NPChar)+". ������ ����� ����������.");
    			link.l9.go = "exit";
			}
		break;
		
		case "onlyGenWork_1":
			dialog.text = "��... � ������, ��� �� ��������� ������ ������� ��� "+ NationNameGenitive(sti(pchar.questTemp.NationQuest)) +", ������� ������ ���������� � ��� �������� ��� �� �����.";
			link.l1 = "����� ����.";
			link.l1.go = "exit";
		break;

		case "onlyGenWork_2":
			dialog.text = "����� �����, ��� �� ����� ����� ������� � �������������� ���������� ��������. �� �����, ��� � ���� �������� �� ������� ���������� �������� �� ����� " + NationNameGenitive(sti(npchar.nation)) + ".";
			link.l1 = "��... �� ��� ��, ����� ����.";
			link.l1.go = "exit";
		break;

		case "onlyGenWork_Alter":
   			dialog.text = "� ��������� ��� ��� � � ������� ��� ���� ����������� � ������������������ ������� ��� ������� � ���� ������ ���.";
			link.l1 = "��� ��, ����� �����.";
			link.l1.go = "exit";
		break;

		/////������� �� ����������� �������.
		case "colony_capture":
   			dialog.text = "� ����� ������ ���� ����?";
   			for (i=0; i<MAX_COLONIES; i++)
			{
				if (sti(colonies[i].HeroOwn) == true)
				{
					sld = GetFortCommander(colonies[i].id);
					attrLoc = "l" + i;
					Link.(attrLoc) = GetCityName(colonies[i].id);
					Link.(attrLoc).go = "CityGive_" + i;
				}
			}
			link.l99 = "� ������, ���� ��� � ������.";
			link.l99.go = "exit";
		break;
		
        case "ColonyGive":
            i = sti(NPChar.quest.CityIdx);
            sld = GetFortCommander(colonies[i].id);
            colony_money = TWN_CityCost(colonies[i].id) / 50;  // �� *1000 � �����
            TWN_RealeseForMoney(colonies[i].id, false);
            dialog.text = "�������� �������, "+GetAddress_FormTitle(sti(NPChar.nation), sti(Items[sti(pchar.EquipedPatentId)].TitulCur))+
                          "! ���� ������� �� ��������� ������������ � ���������� ��� ���� �������. ��� ��� " + colony_money +
                          " ������� � �������� �������������� �� ���� ������� ����� "+NationNameAblative(sti(NPChar.nation))+
                          ". � ������ ����� ���� ��������, � ���� ����� ����� ���.";
            link.l1 = "� ����� ������, " + GetAddress_FormToNPC(NPChar) + ", �� ���� ��� ������ �����������. ����� ����������.";
            link.l1.go = "exit";

            ReOpenQuestHeader("Gen_CityCapture");
		    AddQuestRecord("Gen_CityCapture", "t8");
			AddQuestUserData("Gen_CityCapture", "sCity", GetCityName(colonies[i].id));
			AddQuestUserData("Gen_CityCapture", "sNation", XI_ConvertString(GetNationNameByType(sti(NPChar.nation)) + "Gen"));
		
            AddMoneyToCharacter(PChar, colony_money);
            AddTitleNextRate(sti(NPChar.nation), 1);  // ������� ������
            ChangeCharacterNationReputation(Pchar, sti(NPChar.nation), 10);
            PChar.questTemp.DontNullDeposit = true;    // ���� �� ������ ����������
            SetCaptureTownByNation(colonies[i].id, sti(NPChar.nation));
        break;
	}
}
