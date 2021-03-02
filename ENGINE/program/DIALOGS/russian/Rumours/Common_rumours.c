// ����� - ������ �� �������
void ProcessCommonDialogRumors(ref NPChar, aref Link, aref NextDiag);
{

    string strum;
    string srum;
	switch(Dialog.CurrentNode)
	{
	case "rumours_citizen":	
		NextDiag.CurrentNode = "rumours";
		//------------- ��������� ����� ������� --------->>>>>>>>
		//��������� ��������� �����
		if (pchar.questTemp.Sharp.City == npchar.city && sti(pchar.questTemp.Sharp.City.rumour))
		{
			pchar.questTemp.Sharp.City.rumour = false; //���� ���
			Dialog.Text = Sharp_choiceAction();
			link.l1 = "��, �������... ��� ��� ������?";
			link.l1.go = "new question";
			link.l2 = RandPhraseSimple("�������, ������ �����������.",
									"����� ��������.");
			link.l2.go = "exit";
			break;
		}
		//����� ������ ����� �����
		if (pchar.questTemp.BlueBird.City == npchar.city && pchar.questTemp.BlueBird == "returnMoney")
		{
			Dialog.Text = BlueBurd_setTradeShip();
			link.l1 = "���, �������... ��� ��� ������ � ������?";
			link.l1.go = "new question";
			link.l2 = RandPhraseSimple("�������, ������ �����������.",
									"����� ��������.");
			link.l2.go = "exit";
			//����� ���� � ������ ������
			pchar.questTemp.BlueBird.City = GetSharpCity();
			break;
		}
		//<<<<<-------- ��������� ����� ������� -----------------
		if (!CheckAttribute(NPChar, "quest.repeat.rumours_citizen") || NPChar.quest.repeat.rumours_citizen != 3 )
        srum = SelectRumourEx("citizen", NPChar);
        else srum = NO_RUMOUR_TEXT[rand(4)]; // fix
        string posrep1, posrep2, answ1, answ2, answ3, answ4;
        if (RumourHasInformation(srum))
        {
            posrep1 = RandPhraseSimple(" ����� ��� ����...", " ��������, ��� ��� ���������.");
            posrep2 = " � � ���, �������, ���� �����-������ �������� ��� ����� �������?";
            answ1 = RandPhraseSimple(RandSwear() + "���������� ���� �� �������������, "+GetFullName(NPChar)+".",
                                 "����� �������� ��� ���...");
            answ2 = RandPhraseSimple(RandSwear() + "��� ����� ���������! ��� ��� ��� ����� ��������...",
                                 "��� ������.");
            answ3 = RandPhraseSimple("���� ������ �� ��������� �� �����. ��� ����, �� ������� �������� � ���� ���. ",
                                 "� ������� �����, ��� ��� ���-������ � ������ ���.");
            answ4 = "";
        
        }
        else
        {
            posrep1 = " � ��� ������ ����� ���-���� ����������. �� ���� �� ���������� ���-�� ���������, �� ��� �� ���� ������ � �������.";
            posrep2 = " ��� ��� �������� �������. �� ����� " + GetAddress_Form(NPChar) + " ������� ���-�� �����?";
            answ1 = RandPhraseSimple("����� ��������-�� ��� �����...",
                                 RandSwear() + "�� ������ �� ������! �����, ��� ��� ��� ����� ��������...");
            answ2 = RandPhraseSimple("��������, �� ������, �����, ����� ��� �����...",
                                 "��, �� ������ �� ��� �����"+NPCharSexPhrase(NPChar, " ", "�� ")+", ��, �����, �� ������ ���-�� ���?");
            answ3 = RandPhraseSimple("�� �����, ��� ���� ���-�� ���, ����� �� ��� ��������������.",
                                 "� � ������������� ���-������ ��������, �� ���-������ � ������ ���.");
            answ4 = "";
        
        }
		Dialog.Text = NPCStringReactionRepeat(srum,
                                                srum+posrep1,
                                                srum+posrep2,
                                                RandPhraseSimple("� ���������, ������ � ������ �� ����, ��������� ��� ����.",
                                                                    "�� ������� ���� ������ �����������, ����� ���� ��������, �� ���� ���� ����."),
                                                "block", 1, npchar, Dialog.CurrentNode);
		
		//link.l1 = RandPhraseSimple(RandSwear() + "��� ����� ���������! ��� ��� ��� ����� ��������...",
        //                         "��� ������.");
        link.l1 = HeroStringReactionRepeat(answ1,
                                            answ2,
                                            answ3,
                                            answ4,
                                            npchar, Dialog.CurrentNode);
		link.l1.go = "new question";
		link.l2 = RandPhraseSimple("�������, ������ �����������.",
                                 "����� ��������.");
		link.l2.go = "exit";
	break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	case "rumours_habitue":
		Dialog.Text = LinkRandPhrase("��... �-��! ","��, ����� ���! ","���... ")+SelectRumourEx("habitue", NPChar);
		link.l1 = RandPhraseSimple(RandSwear() + "�� ��� ���� ������ ��� ��������...",
                                 "������. ����� ��� �� �����.");
		link.l1.go = "sit_3";
		link.l2 = RandPhraseSimple("�������, ������ ����������.",
                                 "��, ���������� ���. �����, ��� ��� ����, ������ ��������.");
		link.l2.go = "exit_sit";
		if (pchar.questTemp.piratesLine == "PL5Hunter_toFirst" && npchar.location == pchar.questTemp.piratesLine.Q5.city_1 + "_tavern")
		{
			dialog.text = "�������������, ���� ������ �� ������, �����, ��������� ���� ������ � �� �������� � ����� ��� � �����! ��, �������, ������ ����� ������?! � ����� ������� - ���������. �� ���������, � � ��� ���� �������! �! ������?! �, �� ��������, ������ �� ������, � �� ��� ��� �����. ����� ����! �����, �� ������ ���?..";
			link.l1 = "��, ��� �����...";
			link.l1.go = "sit_3";
			AddQuestRecord("Pir_Line_5_Hunter", "3");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_1 + "Gen"));
			pchar.questTemp.piratesLine = "PL5Hunter_toFirstUsurer";
		}
		if (pchar.questTemp.piratesLine == "PL5Hunter_toSecond" && npchar.location == pchar.questTemp.piratesLine.Q5.city_2 + "_tavern")
		{
			dialog.text = "������ �� ���, ���� ���� �����, �����, ��� ���� ��� �� ������. ����� ���, ��� ��� ������... ���� ������. ���������, ��� �� ����� �������� ����������, ��-��-��. ��� ��� � ����, ��������� ����������, ����� �� ������� � ������, � ��������� �������� � ��������-�� ������...";
			link.l1 = "�� ��, ��� ���������� - ����� �������.";
			link.l1.go = "sit_3";
			AddQuestRecord("Pir_Line_5_Hunter", "5");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_2 + "Gen"));
			pchar.questTemp.piratesLine = "PL5Hunter_toSecondUsurer";
		}
		if (pchar.questTemp.piratesLine == "PL5Hunter_toThird" && npchar.location == pchar.questTemp.piratesLine.Q5.city_3 + "_tavern")
		{
			dialog.text = "��� ��� �����, ��� ����� ����� ����! ���� ������ ������ ��������. ������, ���� �������, ������ ��������� �����. ����, ����! � ���� ������. ���� ��������, ���. �� ����, ����� ����������, ���� � ������� ������� ��������. ������!";
			link.l1 = "� ���! �����...";
			link.l1.go = "sit_3";
			AddQuestRecord("Pir_Line_5_Hunter", "7");
			AddQuestUserData("Pir_Line_5_Hunter", "sCity", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_3 + "Gen"));
			pchar.questTemp.piratesLine = "PPL5Hunter_toThirdUsurer";
		}
	break;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "rumours_trader":
            if (!CheckAttribute(NPChar, "quest.repeat.rumours_trader") || NPChar.quest.repeat.rumours_trader!= 3 )
            strum = SelectRumourEx("trader", NPChar);
            else strum = NO_RUMOUR_TEXT[rand(4)]; // fix
			Dialog.Text =  NPCStringReactionRepeat(strum, strum+" ��. ������ ������ �� ��������",
                                                   strum+" � ������ ��� ����� ��������� � �����.",
                                                    RandPhraseSimple("� �� ��� ������, ��� � ���� ��� ������� �� ������ ��������!",
                                                                        "� ������� ������� � ��� ������� ��������� ������ �������!"),
                                                                         "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(pcharrepphrase(RandSwear() + "��� ����� ���������! �� ����� � ����","���������, ����� ��� ��� ����� ����"),
                                                pcharrepphrase("���� � ����! ������� �����. �� � ���� ���� ��� ������.",
                                                                "��� �� ����� ������ ������ ����������� ����� � �������� � �����"),
                                                pcharrepphrase(RandSwear() +" ��� ���� ������ � � ����.","�� �����, ����� - ������."),
                                                pcharrepphrase("������ � �������, ��� � ����� ������. ����� �� ��������.",
                                                                "����� �������� ����. �������� � ����."),
                                                npchar, Dialog.CurrentNode);

			link.l1.go = "business";
			link.l2 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("����, ������ ��������.",
													               "����� �������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"."),
													pcharrepphrase("��������! ������ �� �����. � ��� �����.",
													               "������� ���� ��� ������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"! �� ��������!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("������ ��������!",
													               "�� ��������, ��������."),
													pcharrepphrase("��! �� ��� ��� �������? ���? ���!?! �� �����, ����� ����� �������.",
													               "����� ���� ��������, � ���� ������� ���� � ������.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("���! ��� ��������!",
													               "��� ���� ����. ��������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!"),
													pcharrepphrase("� �� ����  ��� ��������! ������� ��� ��� � ����� �������!",
													               "�������, ����� ��������� ��� ���� �� �������. ��������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("��� � �����, �������? ��� �� � - " +GetFullName(pchar)+ "! ��������, � ����� �����.",
													               "�����, ���� ������ ��������� ��� ������, ��� � ��� ��� ����� ����."),
													pcharrepphrase("������ ���� �������, ��������! � ��� �����.",
													               "��� ��� ��� ������, " + GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											  npchar, Dialog.CurrentNode);
			link.l2.go = "Exit";
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "rumours_capitan":
			Dialog.Text = SelectRumourEx("capitan", NPChar);
			Link.l1 = RandPhraseSimple(RandSwear() + "��� ����� ���������! ��� ��� ��� ����� ��������...",
                                     "��� ������.");
			Link.l1.go = "quests";
			Link.l2 = RandPhraseSimple("�������, ������ ����������.",
                                     "����� ��������");
			Link.l2.go = "exit";
			NextDiag.TempNode = "quests";
            NPChar.talk_date =  lastspeakdate();
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "rumours_tavern":  // homo 03/08/06

                 Dialog.Text = NPCStringReactionRepeat(LinkRandPhrase("�������� ����� �����. �� �������� 1000 ������� � �������� ��� ��� ����.",
                "���-�� � �����������. �� �������� 1000 ����� ������� ��� ������.",
                "� ����� ���������� ���, ��� ����. �� �� ��������� - 1000 �������, �� ������� � �� ��������."),
                "������ ��� ������ ��������. � ������ ����� ���� ��������, �� ���� �� ����.",
                "", "", "block", 1, npchar, Dialog.CurrentNode);
             
    			link.l1 = HeroStringReactionRepeat(RandPhraseSimple(RandSwear() + "������� �� ������ ��������? ��� ��, ����� ����-������ �������.",
                                         "�� ����� ������ � ��� ��� �����."), "����� ��������� � ���-������ ������.", "����� ��������� � ���-������ ������.", "����� ��������� � ���-������ ������.", npchar, Dialog.CurrentNode);
                link.l1.go = HeroStringReactionRepeat("rumours_tavern_no", "first time", "first time", "first time", npchar, Dialog.CurrentNode);//"rumours_tavern_no";
                NextDiag.CurrentNode = NextDiag.TempNode;
    			if (makeint(pchar.money) >= 1000 )
    			{
    				link.l2 = HeroStringReactionRepeat(RandPhraseSimple("�� �� ��������, ���� � ������ ������������� ������ ��������.", "� �������, �� ������ ��� �������, ��� �� ��� ���������� ������������� ���-�� ������."),
                     "", "", "" , npchar, Dialog.CurrentNode);
    				link.l2.go = HeroStringReactionRepeat("rumours_tavern_yes", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
    			}
    			else
    			{
                    link.l2 =  HeroStringReactionRepeat("� ���� ��� ����� �����.", "", "", "", npchar, Dialog.CurrentNode);
    				link.l2.go = HeroStringReactionRepeat("rumours_tavern_no", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
    			}

		break;
		
		case "rumours_tavern_yes":

			//------------- ��������� ����� ���������� --------->>>>>>>>
			//�������, ����� �3
			if (pchar.questTemp.piratesLine == "KillLoy_toCumana")
			{
				Dialog.Text =  "��������� �����, ����� ����� ��������� �������, ������, ��� �������, ��������� ������ ���������� ������� � �����. ������������ � ��������� � ����� ������ ������-�� �����... ��� ��� ������ ��������!";
				link.l1 = "��������� ������ ���������� �������, ��������? ����� ���������... � ������ ������ ��������������� �� ������?";
				link.l1.go = "rumours_tavern";
				AddQuestRecord("Pir_Line_3_KillLoy", "14");		
				pchar.questTemp.piratesLine = "KillLoy_toCumana_2";
				break;
			}
			//<<<<<-------- ��������� ����� ���������� -----------------
		
            string RumText = SelectRumourEx("tavern", NPChar); //fix
            //SelectRumour();


		    if ( RumText == NO_RUMOUR_TEXT[0] || RumText == NO_RUMOUR_TEXT[1]
                || RumText == NO_RUMOUR_TEXT[2] || RumText == NO_RUMOUR_TEXT[3] || RumText == NO_RUMOUR_TEXT[4] )//fix
            {

                Dialog.Text = NO_RUMOUR_TEXT[rand(3)]; //fix
                link.l1 = RandPhraseSimple("� �� ����� ������� �� ������ �������, ������.", "���� �������� �� ����� � �������� ����� - �� ������ �� ��������.");

			    link.l1.go = "Exit";
			    NextDiag.CurrentNode = NextDiag.TempNode;
			  //  link.l2 = LinkRandPhrase("�������, ������ ����������.",
              //                       "����� ��������","�������, ��� ����. �� ��������.");
			  //  link.l2.go = "Exit";
            }
            else
            {

                AddMoneyToCharacter(pchar, -1000);

                Dialog.Text = RumText;

    			link.l1 = RandPhraseSimple("���������, ��� ���� ������. � ������ ������ �� ����������?", "������, �� �������� ���� ������ - �����. ���� ���-������ ���?");
                //RandPhraseSimple(RandSwear() + "��� ����� ���������! ��� ��� ��� ����� ��������...",
                //                         "��� ������.");

    			link.l1.go = "rumours_tavern";
    			NextDiag.CurrentNode = NextDiag.TempNode;
    			//link.l2 = LinkRandPhrase("�������, ������ ����������.",
              //                           "����� ��������","�������, �� ��� ����� �����. �� ��������.");
    		//	link.l2.go = "Exit";
            }

		break;

		case "rumours_tavern_no":
			Dialog.Text = "������� ���� ���������, ��� ����� ������� �����.";
            link.l1 = "����� ��������� � ���-������ ������.";
			link.l1.go = "first time";
			NextDiag.CurrentNode = NextDiag.TempNode;
			link.l2 = LinkRandPhrase("�������, ������ ����������.",
                                     "����� ��������","�� ��������.");
			link.l2.go = "Exit";
		break;	
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "rumours_poor":
			if (!CheckAttribute(NPChar, "quest.repeat.rumours_poor") || NPChar.quest.repeat.rumours_poor <= 3)
			{
				Dialog.Text = SelectRumourEx("poor", NPChar);
				Link.l1 = RandPhraseSimple(RandSwear() + "��, ��� ���������. ������� ��� ������...",
										"��, ��� ��� ��� ����� ��������...");
				Link.l1.go = "rumours_poor_2";
				Link.l2 = RandPhraseSimple("��, ������, ������.",
										"����� ��������, ��������.");
				Link.l2.go = "exit";
				NPChar.quest.repeat.rumours_poor = sti(NPChar.quest.repeat.rumours_poor) + 1;
			}
			else 
			{
				Dialog.Text = RandPhraseSimple("�� � ��� ��������, ��� ��� ��������, ��� ����.", 
					"������ � ������ �� ������, ��������...");
				Link.l1 = RandPhraseSimple("�������. �� ��� ��, ����� ��������.",
										"����. ��, �����, ��������...");
				Link.l1.go = "exit";			
			}
            NPChar.talk_date = lastspeakdate();
		break;
        case "rumours_poor_2":
			if (NPChar.quest.repeat.rumours_poor <= 3)
			{
				Dialog.Text = SelectRumourEx("poor", NPChar);
				Link.l1 = RandPhraseSimple(RandSwear() + "��, ��� ���������. ������� ��� ������...",
										"��, ��� ��� ��� ����� ��������...");
				Link.l1.go = "rumours_poor";
				Link.l2 = RandPhraseSimple("��, ������, ������.",
										"����� ��������, ��������.");
				Link.l2.go = "exit";
				NPChar.quest.repeat.rumours_poor = sti(NPChar.quest.repeat.rumours_poor) + 1;
			}
			else 
			{
				Dialog.Text = RandPhraseSimple("�� � ��� ��������, ��� ��� ��������, ��� ����.", 
					"������ � ������ �� ������, ��������...");
				Link.l1 = RandPhraseSimple("�������. �� ��� ��, ����� ��������.",
										"����. ��, �����, ��������...");
				Link.l1.go = "exit";			
			}
			NPChar.talk_date = lastspeakdate();
		break;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		��������� ��������� Loan
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "LoanForAll": //��������� � ������� ��� ����
			dialog.text = LinkRandPhrase("�������? ��, ��������, ��� ������.", "���������� ������� - ��� ������ ���������. ��������, � ����� ���.", "��� ��, ������� ��������� � ��������.");
            if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
            {
                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
                {
                    dialog.text = LinkRandPhrase("� �� �������, ��� ���������� �� ������, " + GetAddress_Form(NPChar) + "? ��� ��� " + GetFullName(npchar) + ". ��� ���� �������?",
                             "�� �������� �� ���, " + GetAddress_Form(NPChar) + "?! � " + GetFullName(npchar) + ". � ��� �� ���� � ���?",
                             "� ��� ������, " + GetAddress_Form(NPChar) + ". ��� �� ������? ������, ���� ����� " + GetFullName(npchar) + ".");
                    link.l1 = NPCharRepPhrase(pchar, "� �� ����������, �������! ������ �� �����!", "� ���� � ��� ���������� ������ �� ������ �����, " + GetAddress_FormToNPC(NPChar) + ".");
                    link.l1.go = "LoanFindCitizen_1";
				    NPChar.quest.LoanFindCitizenType = rand(2); // ��� ����
                    break;
                }
            }
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
            {
                link.l2 = "� ������ ��� ������.";
                link.l2.go = "TakeChest_2";
            }
			link.l9 = RandPhraseSimple("�-�-�, � ,������, ������. ������� ��������� ��� � � ���� �� ����.", "��������, ������� ���������� �������� � ���� � ��� ���.");
			link.l9.go = "exit";
		break;
		// ������� ���� ����������
		case "LoanFindCitizen_1":
            switch (sti(NPChar.quest.LoanFindCitizenType))
            {
                case 0 :
                    dialog.text = "��, �����. � ��� ��� ��������� ��� ��� ������.";
        			Link.l1 = "�� ��� ��, ����� ������! ����� � ���� " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ".";
        			Link.l1.go = "LoanFindCitizenType_0";
        			Link.l2 = "������������, ��� � ������ ��� ���!";
        			Link.l2.go = "exit";
                break;
                
                case 1 :
                    dialog.text = "���! ������ � �� �����! � ������ ����������, ������ ������.";
                    Link.l1 = "������ �� ����, � ���� " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". � �� ����� ���������� � �� �������.";
        			Link.l1.go = "LoanFindCitizenType_1";
        			Link.l2 = "�� � �� �����!";
        			Link.l2.go = "LoanFindCitizenType_1_1";
                break;
                
                case 2 :
                    dialog.text = "� ��������� � ���� ��� ���� �����.";
                    Link.l1 = "������ �� ����, � ���� " + FindRussianMoneyString(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent))) + ". � �� ����� ���������� � �� �������.";
        			Link.l1.go = "LoanFindCitizen_1";
        			Link.l2 = "� ������� ����?";
        			Link.l2.go = "LoanFindCitizenType_2";
        			NPChar.quest.LoanFindCitizenType = rand(1);// ������ 2 ����
                break;
			}
		break;

		case "LoanFindCitizenType_0":
            dialog.text = "��� ��� �����.";
			Link.l1 = "������� �� ��������������!";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenDone = true;
        	addMoneyToCharacter(Pchar, (sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent)));
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("� ��� ���� 30 ����, ����� �������� ������������ � ������� ��� ������.");
        	AddCharacterExpToSkill(pchar, "COMMERCE", 20);        	
        	AddQuestRecord("Gen_LoanFindCitizen", "3");
			npchar.LifeDay = 0;
		break;
		
		case "LoanFindCitizenType_1_1":
            dialog.text = "���, ��� �� � ���� ������������.";
			Link.l1 = "�� �� ����!";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenFalied = true;
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("� ��� ���� 30 ����, ����� �������� ������������ � ������� ������.");
        	AddQuestRecord("Gen_LoanFindCitizen", "2");
			npchar.LifeDay = 0;
		break;
		
		case "LoanFindCitizenType_1":
            dialog.text = "������!! �������� ������ ������!";
			Link.l1 = "�������!";
			Link.l1.go = "fight";
			pchar.GenQuest.Loan.FindCitizenFalied = true;
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("� ��� ���� 30 ����, ����� �������� ������������ � ������� ������.");
        	AddQuestRecord("Gen_LoanFindCitizen", "2");
			npchar.LifeDay = 0;
		break;
		
		case "LoanFindCitizenType_2":
            pchar.GenQuest.Loan.FindCitizenTempMoney = 10 + rand(makeint(sti(pchar.GenQuest.Loan.FindCitizenMoney) + sti(pchar.GenQuest.Loan.FindCitizenPercent) - 1000));
            dialog.text = "������ " + pchar.GenQuest.Loan.FindCitizenTempMoney + " �������.";
			Link.l1 = "���. ����� ��� �����!";
			Link.l1.go = "LoanFindCitizen_1";
			Link.l2 = "�����, ��� ����. ��� �������. ����� � ������� ������.";
			Link.l2.go = "LoanFindCitizenType_2_1";
			NPChar.quest.LoanFindCitizenType = rand(1);// ������ 2 ����
		break;
		
		case "LoanFindCitizenType_2_1":
            dialog.text = "�� ����� �����! ������� �������!";
			Link.l1 = "��! ����� ��� �� ��� ����.";
			Link.l1.go = "exit";
			pchar.GenQuest.Loan.FindCitizenDone = true;
        	addMoneyToCharacter(Pchar, sti(pchar.GenQuest.Loan.FindCitizenTempMoney));
        	SetTimerCondition("LoanFindCitizenTimer", 0, 0, 30, false);
        	Log_SetStringToLog("� ��� ���� 30 ����, ����� �������� ������������ � ������� ��� ������.");
        	OfficersReaction("good");
        	ChangeCharacterReputation(pchar, 3);
        	AddCharacterExpToSkill(pchar, "Leadership", 20);
        	AddQuestRecord("Gen_LoanFindCitizen", "3");
			npchar.LifeDay = 0;
		break;
		//�������� ��������
        case "TakeChest_2":
            if (GetCharacterItem(pchar, "Chest") >= sti(pchar.GenQuest.LoanChest.Chest))
            {
                if (CheckAttribute(pchar, "GenQuest.LoanChest.Money.Discount"))
				{
					Dialog.Text = "��� ��� ������ � � �������� � ��� ������. ������, �� �� ��������� � ���������� ��� ��� ����� ��������. ������� � �������� ��������� ��� ������ ������ �������� �� " + pchar.GenQuest.LoanChest.Money.Discount + "%. ����� ��������, �� ��� ���������� �� ��������. �������� ��������...";
					link.l1 = "�������!";
    				link.l1.go = "exit";
					AddMoneyToCharacter(pchar, sti(pchar.GenQuest.LoanChest.Money)/100*sti(pchar.GenQuest.LoanChest.Money.Discount));
				}
				else
				{
					Dialog.Text = "������������! � ����� ��� ���. ��� ���� �������.";
					link.l1 = "�������!";
    				link.l1.go = "exit";
					AddMoneyToCharacter(pchar, sti(pchar.GenQuest.LoanChest.Money));
				}
				ChangeCharacterReputation(pchar, 1);
                AddCharacterExpToSkill(pchar, "Sailing", 50);
				AddCharacterExpToSkill(pchar, "Leadership", 20);
				AddCharacterExpToSkill(pchar, "COMMERCE", 5);
                OfficersReaction("good");
    			TakeNItems(pchar, "Chest", -sti(pchar.GenQuest.LoanChest.Chest));    			
    			DeleteAttribute(pchar, "GenQuest.LoanChest");
    			DeleteAttribute(Pchar, "quest.LoanChestTimer");
    			AddQuestRecord("Gen_LoanTakeChest", "3");
                CloseQuestHeader("Gen_LoanTakeChest");
			}
			else
			{
                Dialog.Text = "������������! �� ��� �� ��� ������� � �������?";
                link.l1 = "� �� ������ ������� ������ ��� ��������?";
    			link.l1.go = "TakeChest_3";
                link.l2 = "����� �����.";
    			link.l2.go = "exit";
			}
		break;

		case "TakeChest_3":
            Dialog.Text = "���! ������ � ��������!";
            link.l1 = "�����. ����� � ���� �� ���������.";
			link.l1.go = "exit";
			link.l2 = "�� �, ������, ����� � " + FindRussianMoneyString(sti(pchar.GenQuest.LoanChest.Chest)*30000) + " ������� ��������� ��������?";
			link.l2.go = "TakeChest_4";
		break;

		case "TakeChest_4":
            Dialog.Text = "��, ��� ���������.";
            if (sti(pchar.Money) >= (sti(pchar.GenQuest.LoanChest.Chest)*30000))
            {
                link.l1 = "��� � ������������!";
    			link.l1.go = "TakeChest_5";
			}
			link.l2 = "������ ��� ����� ��� ����.";
			link.l2.go = "exit";
		break;

		case "TakeChest_5":
            ChangeCharacterReputation(pchar, 1);
            AddCharacterExpToSkill(pchar, "Sailing", 30);
			AddCharacterExpToSkill(pchar, "Leadership", 10);
			AddCharacterExpToSkill(pchar, "COMMERCE", 5);
            OfficersReaction("good");
            Dialog.Text = "� ���� ����� ����� ����!";
            link.l1 = "�������!";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -1*(sti(pchar.GenQuest.LoanChest.Chest)*30000));
			DeleteAttribute(pchar, "GenQuest.LoanChest");
			DeleteAttribute(Pchar, "quest.LoanChestTimer");
			AddQuestRecord("Gen_LoanTakeChest", "4");
            CloseQuestHeader("Gen_LoanTakeChest");
		break; 

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		������ ����, ������������� �� ���������� �����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "IntelligenceForAll":
            Dialog.Text = RandPhraseSimple("�� ��� ��, ����� ������. � ���� ����� ��� ��� �����, ������� ��� �������� ��������� ���������� ���� ����� � ����. �������� ��������.\n�� ��������� �� ��� ������ � �� ������������� �� ����, ��� ����� ���� ��������������. ���������� �����������...", 
				"��, ���������. � ������ ��� �����, � �������, ��� �� ������� ���� ��������� ���������� ���� ���� ����� � ����.\n������ �������� � �� ��������� �� ��� ����� - ��� ������.");
            link.l1 = RandPhraseSimple("� �����. ��������...", "�������� � �����.");
			link.l1.go = "exit";
			pchar.GenQuest.Intelligence = "Execute";
			GiveItem2Character(pchar, "sicretLetter");
			ref rItem = ItemsFromID("sicretLetter");
			rItem.City = XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.Intelligence.MayorId)].city+"Gen")); 
			rItem.Mayor = GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)); 
			AddQuestRecord("MayorsQuestsList", "7");
			AddQuestUserData("MayorsQuestsList", "ColonyName", XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.Intelligence.MayorId)].city+"Gen"));
			AddQuestUserData("MayorsQuestsList", "MayorName", GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)));
			AddQuestUserData("MayorsQuestsList", "sCity", XI_ConvertString("Colony"+pchar.GenQuest.Intelligence.City+"Dat"));
			AddQuestUserData("MayorsQuestsList", "sWho", GetWorkTypeOfMan(&characters[GetCharacterIndex(pchar.GenQuest.Intelligence.SpyId)], "Gen"));
			AddQuestUserData("MayorsQuestsList", "SpyName", GetFullName(&characters[GetCharacterIndex(pchar.GenQuest.Intelligence.SpyId)]));			
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		����� ���������, ��������� ������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "AscoldTrader": //��������� ������ ������
            dialog.Text = "��������, �� �������� ��� ������� �����, ������� � ������ ���� ���������?";
            if (GetQuestPastDayParam("questTemp.Ascold") > 3)
			{
				Link.l1 = "���, ���� �� ��������... ��������, � ��������� �� ������ ��, �������, ����, �� ���-�� ��� ������...";
            	Link.l1.go = "AscoldTrader_6";
			}
			else
			{
				Link.l1 = "���, ���� �� ��������... �� � �������� ������.";
            	Link.l1.go = "quests";
			}
		break;
        case "AscoldTrader_WasSeek":
            dialog.Text = "��������, �� �������� ��� ������� �����, ������� � ������ ���� ���������?";
            Link.l2 = "��, �����. � ������� �� ��� �� ���� ���������?";
            Link.l2.go = "AscoldTrader_1";
        break;		
		case "AscoldTrader_1":
        	dialog.text = "� - ���������. �� � ����, ��� ����� � ���� ���.";
    		link.l1 = "��, � ���c������� �� ��������� ����� ����� ������.";
    		link.l1.go = "AscoldTrader_2";
		break;
 		case "AscoldTrader_2":
        	dialog.text = "�������, �� �� ���������� � �������. ������ �����������.\n"+
                          "�� ��������� � ������ ��� ��� ����� ���� �������� ��� - �������.";
            if (Characters[GetCharacterIndex("Ascold")].quest.meeting == "0")
            {
                link.l1 = "�������� ���...";
                link.l1.go = "AscoldTrader_4";
            }
            else
            {
                link.l1 = "���� � ���, �������� ����������. ������, ����� ��������...";
                link.l1.go = "AscoldTrader_3";
            }
		break;
 		case "AscoldTrader_3":
        	dialog.text = "� ��� ���������? ������� �� ���� ��� ������ ����?";
    		link.l1 = "�� ��, �� ������ �������� ���� �� ����� ��� ���������!";
    		link.l1.go = "AscoldTrader_4";
		break;
 		case "AscoldTrader_4":
        	dialog.text = "������� ������� ��������, �� ������� �� �������, �������. �� ������ - ��������, ���� ����� �����, ������� ����� �� ������ ������� �����, ��� ������� ��� ����, � ������ ���� ���� �� �������. ������ � �������� �������, � ���� � ����� ������ �� �������� ����������� ��������.\n"+
                          "��� � ����� �� �������� �����, � ����� �� ����, ��� � �� ����, ��� �� ����������.\n"+
                          "�� ��� �� ��������, ��� ��� � ���� � ��� �������� ����.\n"+
                          "��� ��� ������� � ����, ������������� ��� �� ���� � ��� ����� � �������. ����� �� � ���� ��� �����, ��� ��� ��� �����.";
    		link.l1 = "� ���� � ����� ����� �����?";
    		link.l1.go = "AscoldTrader_5";
		break;
 		case "AscoldTrader_5":
        	dialog.text = "�� ���� �� ���������, � ������ ������ ����� ������������ �� ����� � ��� ������� ��������.";
    		link.l1 = "�������. ��� �, �������, ������� � ������ �������� ��� ������.";
    		link.l1.go = "exit";
    	    pchar.questTemp.Ascold = "GoToAscold";
    	    QuestSetCurrentNode("Ascold", "HaveRecomendation");
            AddQuestRecord("Ascold", "2");
		break;
 		case "AscoldTrader_6":
        	dialog.text = "�������, � ����������� ��������?";
    		link.l1 = "�����, ��������...";
    		link.l1.go = "AscoldTrader_7";
		break;
 		case "AscoldTrader_7":
        	dialog.text = "�� � ��� �� �����?.. ��� ��� � ���� �����, �������� - ������� ���� � ���. �� � ����, �������, �� � ����-�� ����� �� �������. ������� �� ���� ����������� � ���� - � � ����� ��������� ������.";
    		link.l1 = "���, �� ��� ��, ������� �� ������.";
    		link.l1.go = "quests";
			SaveCurrentQuestDateParam("questTemp.Ascold");
		break;
        //==> ����� ������ ��� ������ �����, ����� ����.
		case "AzzyTryTalk":
			dialog.Text = NPCStringReactionRepeat("����� ��������� �� ���� ��������. �-�-�, ���. �� �������� �� ���, ��� ������ ������� ��������� ����������, �� ����� ��� ����������. ������� ���� ������� ��� ���� � �������, ��� �����������, ��� � ��� ������.", "none", "none", "none", "block", 0, npchar, Dialog.CurrentNode);
  			Link.l1 = HeroStringReactionRepeat("������� �� ������ �����.", "none", "none", "none", npchar, Dialog.CurrentNode);
			Link.l1.go = "Exit";
		break;
		case "AggryHelp":
			dialog.Text = NPCStringReactionRepeat("��� ��� �� ����� ����� ����������: �������� �����, ������� �� �������, ������� ��� ����� � ������. ���� � �������� ���, � ����� ��� �� ����.", "none", "none", "none", "block", 0, npchar, Dialog.CurrentNode);
			Link.l10 = HeroStringReactionRepeat("�������, " + npchar.name + ".", "none", "none", "none", npchar, Dialog.CurrentNode);
			Link.l10.go = "exit";
		break;
		//==> ����� ������ ��� ������, ����� ����.
        case "GiveLamp":
			dialog.text = "�� ��������� ��������, ��� ���. ����������, ��� ��� ������, �� �� ����� �����, ��� ������ ������� ����� ����� �������� ������.\n"+
                          "� �� ���� ����������, ��� �� ��� ����. ������ ��������� ������ ���� ����� ����! ��� � �����.";
			link.l1 = "�������, ������ ����.";
			link.l1.go = "exit";
            ChangeCharacterReputation(pchar, 100);
            TakeItemFromCharacter(pchar, "Azzy_bottle");
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		������ ����� ���� ���, ������� ������� � �������
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        case "Man_FackYou":
			dialog.text = LinkRandPhrase("������ ����� ���� ���!!! ��� ��� �� ����� ��������?! �� ������, ��������...", "��, �� ���� ��� ��� ���������?! �����, ������� �������� ����? ��, ����� ���� �����...", "������, �� ���� ��� �����? �� �� ���, �����������! ��, ������, ��� �� �������, ������...");
			link.l1 = LinkRandPhrase("������!!", "��������!!", "�-���, ����!");
			link.l1.go = "fight_owner";
		break;
		case "fight_owner":
			LAi_SetOwnerTypeNoGroup(NPChar);
			LAi_group_Attack(NPChar, Pchar);
			if (rand(3) != 1) SetNationRelation2MainCharacter(sti(npchar.nation), RELATION_ENEMY);
			NextDiag.CurrentNode = "First time";
			DialogExit();
		break;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		����� ��������� ����� �����
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "RBlueBird":
			dialog.text = "��, �������, �� ���������, � ��������� ��� �����. ��, �������� ���������� ����, �� ����� ���������. � �� ���� �������� ��������� '����� �����'...";
			link.l1 = "'����� �����'?";
			link.l1.go = "RBlueBird_1";
		break;
		case "RBlueBird_1":
			dialog.text = "��, ��� ������� ������, ������������ ������� ������ ���� �����, ���� �� ���� � ���� ����������. ������ ������ ������� ���������. ������� ��������, ��������� �������� ���������, �� �������.";
			link.l1 = "� ������ �� �������, ��� �������� ����������? ���� ������ ��� ����� ��������� ���� � ����� ����������.";
			link.l1.go = "RBlueBird_2";
		break;
		case "RBlueBird_2":
			dialog.text = "���������, �����. �� ��� ���� � ���, ��� ������ ��� ����� �������� ���������� ����������. ����� ����� ����� ���� ���� ��� �����, ����� �������� ������ � ������ ����� � ����� ����. ��� ��� - ��������!\n� ����� �� �������� ������ � ������������ - ��� ��� �� �� ��������, ���� ���� � ������� ���������.";
			link.l1 = "�������. � ��� ��� ���� ������� ����� �� ������� ������...";
			link.l1.go = "RBlueBird_3";
		break;
		case "RBlueBird_3":
			dialog.text = "������ ���. ��� ����� ��� �������� �������� ����������� �������, ���� �� ����, ��� '����� �����'!";
			link.l1 = "�-��, �������� ��� ���... ��, ��� ��� �� �� ������ �� ����?";
			link.l1.go = "RBlueBird_4";
		break;
		case "RBlueBird_4":
			dialog.text = "�� �����, ����� �� ���������� ��� ���������� ��������, ���� �� ��������!!!";
			link.l1 = "� ��� � ���� � ����� �����?";
			link.l1.go = "RBlueBird_5";
		break;
		case "RBlueBird_5":
			dialog.text = "� ������ �� ���� ������ ��������� ������ ��� ��� �� ��� ������. ��������� ����� �������� � ���� ��� ������ �������. ������� ������ ������ ����� �� ���!";
			link.l1 = "�� ����� ������, ��������, � � ������� �� ��������. ��� ������!";
			link.l1.go = "RBlueBird_close";
			link.l2 = "��, ��� ������ ���� ����������. � ����� ������� �� ��� ����. ������ ����� ����������?";
			link.l2.go = "RBlueBird_6";
		break;
		case "RBlueBird_6":
			dialog.text = "�� �� ����������� �� �������. � ��������� �������, ��� ������� ��� ����� �������, ���� ������! ������� �� ������, �� ��� ���������� ������� ����� � ����� ����, ������ ������ ��������. ��������������...";
			link.l1 = "��� ���? �� ������ ��������� �� ���� ����������?";
			link.l1.go = "RBlueBird_7";
		break;
		case "RBlueBird_7":
			dialog.text = "��, ��������� �� � ������ ���-����, ����� �� �������� ���� ���� ������ ����� �������. ������ �� �����, ��� ������ � ������� ������ �� �������, �� ������ �� �����. ���� ��� �����, ��� ��� ���������� � ���� ������� ������������...";
			link.l1 = "��-�-�-��, ��������, ��� ��� ���-��. � ������ �������� ��� ������� ������.";
			link.l1.go = "RBlueBird_8";
		break;
		case "RBlueBird_8":
			dialog.text = "����������� ������, " + GetAddress_Form(NPChar) + "." ;
			link.l1 = "��� ������ ����� ����� � ����, ������ ������� ��� ������, �� ������ �� ������, ��� � ������ �� ��� ����. � ������. �� ������, ��� �������� ���� ������� ����� ��� ��������. ��� ������ '�����'?";
			link.l1.go = "RBlueBird_9";
		break;
		case "RBlueBird_9":
			dialog.text = "� ������ �������� ������� �����. �� �� �����������, �� ���� �� ������� �� ��� ������. �� - ���� ������������ ������� ���������� �����, ��� ��� ���� ��� ������. ���� �� �� �� ����������� ��� � ������� ������ - ���� ����� ������ �� �����. �� ��� ������ �� ��������� � �������� ���� ����� � ����!";
			link.l1 = "�������. ����� ������! � ��� �� ��� �����?";
			link.l1.go = "RBlueBird_10";
		break;
		case "RBlueBird_10":
			dialog.text = "������� ������. �� �������� ����� �� �������� � ��������� ���������. ��� �� �������� �� ����?";
			link.l1 = "�� ���������� ��� ������� � ��������?";
			link.l1.go = "RBlueBird_11";
		break;
		case "RBlueBird_11":
			dialog.text = "����������!";
			link.l1 = "����� ������. � ������� ������ � ����� ���, ������� ������ ���������.";
			link.l1.go = "RBlueBird_12";
		break;
		case "RBlueBird_12":
			dialog.text = "��, ������ �� ������, �� ��� ����! �� ��� ��, �����������. ����� �������, ��� ��� ����� ��������� ���, ��� �� �������.";
			link.l1 = "�����, ������ �� �����������!";
			link.l1.go = "exit";
			pchar.questTemp.BlueBird = "toBermudes";
			pchar.questTemp.BlueBird.traiderId = npchar.id; //�������� Id ��������
			SetQuestHeader("Xebeca_BlueBird");
			AddQuestRecord("Xebeca_BlueBird", "1");
			AddQuestUserData("Xebeca_BlueBird", "sCity", XI_ConvertString("Colony" + npchar.city + "Gen"));
			AddQuestUserData("Xebeca_BlueBird", "sName", GetFullName(npchar));
		break;


		case "RBlueBird_close":
			dialog.text = "�� ��� ��, ����� ����. �������, �� ������ '������' � ������ �����...";
			link.l1 = "��...";
			link.l1.go = "exit";
			pchar.questTemp.BlueBird = "over";
		break;
		case "RBlueBirdWon":
			dialog.text = "���������� �������, ����� � ���! �������-�� �� ������ �������� ��������� ��-����������... �� ������ �������� ������ ����� ������?";
			link.l1 = "��, ����������...";
			link.l1.go = "RBlueBirdWon_1";
			int chComp;
			for (int i=0; i<=COMPANION_MAX; i++)
        	{
                chComp = GetCompanionIndex(pchar, i);
                if(chComp != -1 && RealShips[sti(characters[chComp].ship.type)].Name == "XebekVML1")
        		{	
					dialog.text = "��, ���������... ��������� ��� ��������, �� ������ ���������� ������, �� �� �������� ��! ������ ��� ��������� � ����� ��������.";
					link.l1 = "��, ��� ���. �� � � ��� ��������?";
					link.l1.go = "RBlueBirdWon_10";
					break;
                }
            }
		break;

		case "RBlueBirdWon_1":
			dialog.text = "��� �������, �������� ��������.";
			link.l1 = "�������, �� � ��������. ����� ����� ����������... ������, � �� �� ������ ������, ��� ����� �� ���� ����?";
			link.l1.go = "RBlueBirdWon_2";
			AddMoneyToCharacter(pchar, 50000);
		break;
		case "RBlueBirdWon_2":
			dialog.text = "��, ����� ����� ��������� ������...";
			link.l1 = "��� ����� '������' �������� � ��������� ���� - ������� ������.";
			link.l1.go = "RBlueBirdWon_3";
		break;
		case "RBlueBirdWon_3":
			dialog.text = "�� ����� ����!..";
			link.l1 = "�����. �������������, � ���������, � ���� ���, �� �� ������ �����, ��� ������� ��� ���� �� ������.";
			link.l1.go = "RBlueBirdWon_4";
		break;
		case "RBlueBirdWon_4":
			dialog.text = "��, ��� ��� ����� ���������... �� ��� ��, ������� ��� �� ������ � �� ����������.";
			link.l1 = "�� �� �� ���, ��� ���� ��������. ��������.";
			link.l1.go = "exit";
			CloseQuestHeader("Xebeca_BlueBird");
			pchar.questTemp.BlueBird = "over";
		break;

		case "RBlueBirdWon_10":
			dialog.text = "�������� � ���, ��� ���� ��������� ������� ������ � ��� � �����. � ��, ������, ���� �����! � ��� ��� �������� ������� ��������� ����� �����, ��� ��� ����� ���������� ������?";
			link.l1 = "��, ������, �������, ����������... ������, � �������� ������!";
			link.l1.go = "RBlueBirdWon_11";
		break;
		case "RBlueBirdWon_11":
			dialog.text = "���, �� �� �� ���������. �� �������������� � ���������� '����� �����', � �� �� ��������.";
			link.l1 = "��, ��� ������ ���! �� ��� ��, � ��� �����... ����� ����������, ����������� ��� ��������� ������ ����� ��������.";
			link.l1.go = "RBlueBirdWon_12";
		break;
		case "RBlueBirdWon_12":
			dialog.text = "������� �������������! ��������, � ������ ������� ���� ����� '��' � '��'.";
			link.l1 = "�����������. �� � �� ��������...";
			link.l1.go = "exit";
			AddQuestRecord("Xebeca_BlueBird", "9");
			pchar.questTemp.BlueBird = "returnMoney";
			pchar.questTemp.BlueBird.City = GetSharpCity();
		break;

		case "RBlueBirdDieHard":
			dialog.text = "����� � ����� ����. �� ��� ��, �� �� ������, ��� ������� �� ��� ����, � � ���� ������ �� �������. �����, �������� ��� � ����� ���������...";
			link.l1 = "�������, ��� ������ � ����� �� ��������...";
			link.l1.go = "RBlueBirdDieHard_1";
		break;
		case "RBlueBirdDieHard_1":
			dialog.text = "��� �� �����. ��� ����� ��� � �����, � ������ ��������, ��� ������ ������ ��� ����� ��������. �� ���, �� ����� �����, � ����� � �����\n���, ���-�� ���� �� ������ ��������.";
			link.l1 = "������... �� ��� ��, ��������.";
			link.l1.go = "exit";
			CloseQuestHeader("Xebeca_BlueBird");
			pchar.questTemp.BlueBird = "over";
		break;

		case "RBlueBird_retMoney_1":
			dialog.text = "�� �� ��������! ������, ���� �������� ��������� �������!!";
			link.l1 = "� �� ��� �� ������, ���� �� �� �� ������ ����������!";
			link.l1.go = "RBlueBird_retMoney_2";
		break;
		case "RBlueBird_retMoney_2":
			dialog.text = "�-�-���, ����! ������, ������ ���!! ������ ���� �� ����, ��������...";
			link.l1 = "����� �� ������� ��� ������, �������. ����� ������ ��� ������...";
			link.l1.go = "fight";
		break;

		case "RBlueBird_retMoney_3":
			dialog.text = "���� �� �������, ��������! ��� � �����, ��� �������� � �����!! �� ���� '����� �����' ������� �� ���������� ��� ������� �������!..";
			link.l1 = "������ ���, �� ���� ��������, ����� ���� ��������� ��������.";
			link.l1.go = "RBlueBird_retMoney_4";
		break;
		case "RBlueBird_retMoney_4":
			dialog.text = "������ ��������, ������ � ���? �� ��� �������������� ��������������! ��� �� ��� ��������, �� ������� ������!";
			link.l1 = "��� ���, �� ���� ��� �������, ��� ������� ���������� �� ��������� �������. � �����, �� ������� ����� �������������? � �����, � ����� �������� � ���� ������ � ��������� �����...";
			link.l1.go = "RBlueBird_retMoney_5";
		break;
		case "RBlueBird_retMoney_5":
			dialog.text = "�� ������, ���� ��� ����� �� ��� ���. ��� �� �������� ������� ������, � �� �� - ��� ���� �������!";
			link.l1 = "���! ��, ��������, ������� ������� ������ ������� � ���������. ���� � ���� ������. ���-�� ������� �� ��������, � ����� ������ ����� ��������.";
			link.l1.go = "RBlueBird_retMoney_6";
		break;
		case "RBlueBird_retMoney_6":
			dialog.text = "��� ��� ����� �������? � ��� � ���� ���� ������ ��������������.";
			link.l1 = "��, ��� � ������� ���������. ����� � ���, ��� �� ���������� �� ����������� ������ ���� �� �����, � ������� ���. � � �����, ��� ������� ����������� � ������� ������� ����� �������.";
			link.l1.go = "RBlueBird_retMoney_7";
		break;
		case "RBlueBird_retMoney_7":
			dialog.text = "����� �������?";
			link.l1 = "������� ����� ���������������. ��� ������ '����� �����' - � ����� ��� ��������. ���� �� �� ������ ��� ��� ������, �� � ��� � ������� �� �� ������. ��� ��� ��� ���� �������� �� ����� ��������, ��� �������...";
			link.l1.go = "RBlueBird_retMoney_8";
		break;
		case "RBlueBird_retMoney_8":
			dialog.text = "�� �����, ��������� ���� 50 ����� � ���������� ������!";
			link.l1 = "�� ��� ������, ��� ���. ��� ���� � ���, ��� � ����� �������������� ���������� ��������, ������� ��������� ���. �� ������ ��� � ��������� ������. ������� ����� �������.";
			link.l1.go = "RBlueBird_retMoney_9";
		break;
		case "RBlueBird_retMoney_9":
			dialog.text = "� ��� ��� �� ����� �����? �������� ��.";
			link.l1 = "��������� �� � ������ ����. 200 �����, � � ������� ��� � �����. ����� - ������. � ����� ����. ������� ���� �����, � ����� ���...";
			link.l1.go = "RBlueBird_retMoney_10";
		break;
		case "RBlueBird_retMoney_10":
			dialog.text = "�� �� - ������!!";
			link.l1 = "� �����, ����� ���� ������...";
			link.l1.go = "RBlueBird_retMoney_11";
		break;
		case "RBlueBird_retMoney_11":
			dialog.text = "������. ��� ���� ������. � ������ ��������! � ������ ��� � �����!";
			link.l1 = "�������������. ������, ����� �� ������������ �� ����, '����� �����' ��� ����� �� ��������. ��������, ����� � ����� ������������...";
			link.l1.go = "exit";
			DeleteAttribute(pchar, "questTemp.BlueBird");
			pchar.questTemp.BlueBird = "over"; //��������� �����
			AddMoneyToCharacter(pchar, 200000);
			AddQuestRecord("Xebeca_BlueBird", "14");
			CloseQuestHeader("Xebeca_BlueBird");
		break;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////		������� ������� �������� �������� ISS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		case "ISS_deposit":
			dialog.text = NPCStringReactionRepeat("���������� �����. ���� ������� �������� �� ������ �������� ����� ������ ����� ��������� ���� �������������.", 
				"� ��� ������� ���, " + GetAddress_Form(NPChar) + ", ��� ������� � ����. � ��� ������������� �� ��� ������.", 
				"��� �� ����� �����������. ������� ��������� � ����, � ��� ������ ������������� �� ��� ������.",
				"��, ��� �������, �� ���������� ����������� ���� ���������. ��� ��������� ����������, ������� � ���� ������ ��������� ����� ���������. ������� �� ���� ��� ��������� � ����, �������� �� ��� ������� ����� ������������� �� ������.", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�������. �� ��� ��, ���������. � ����� ��, ��� �����.", 
				"���, �������...",
				"� � �� ���������� �����.", 
				"�������... ������� ���, " + GetAddress_Form(NPChar) + ".", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;
		case "ISS_takeDeposit":
			dialog.text = "��-��, � ������� ���������� �������� ��� ���������� ��������. �������� ��������, " + GetAddress_Form(NPChar) + ".";
			link.l1 = "��-��, ����� ������...";
			link.l1.go = "ISS_takeDeposit_1";
			AddMoneyToCharacter(pchar, 1000000);
			CloseQuestHeader("ISS_PoorsMurder");
		break;
		case "ISS_takeDeposit_1":
			dialog.text = "���� �����, �� ������ ������� ��� ������ � ����? � ������ � ����� �������, ��� ���������...";
			link.l1 = "� ����� ���, �������. � �������.";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "over"; //����� ������� ���
			CloseQuestHeader("ISS_PoorsMurder");
		break;

	}
}
