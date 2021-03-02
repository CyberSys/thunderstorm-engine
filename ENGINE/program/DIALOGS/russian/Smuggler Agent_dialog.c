void ProcessDialogEvent()
{
	ref NPChar, her;
	aref Link, NextDiag;
	bool bOk = false;
    bool bOk2;
    
	int Sum, nRel, nDay;
	ref sld;
	
	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
    string iDay, iMonth;
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;

	if (!CheckAttribute(npchar, "quest.trade_date"))
    {
        npchar.quest.trade_date = "";
    }

	//������ ����� �������� ��� ���������� ���� (navy fix)
	if (CheckAttribute(pchar, "GenQuest.contraTravel.days") && GetQuestPastDayParam("contraTravel") > sti(PChar.GenQuest.contraTravel.days))
	{
		DeleteAttribute(pchar, "GenQuest.contraTravel");
		CloseQuestHeader("Gen_ContrabandTravel");
	}

	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "Smuggling_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			PlaceSmugglersOnShore(Pchar.quest.contraband.CurrentPlace);
			Pchar.quest.Contraband.active = true;
			pchar.GenQuest.Contraband.GuardNation = npchar.nation;

			ReOpenQuestHeader("Gen_Contraband");
            if (GetIslandByCityName(npchar.city) == "Mein")
            {
            	AddQuestRecord("Gen_Contraband", "t1_1");
            }
            else
            {
	            AddQuestRecord("Gen_Contraband", "t1");
				AddQuestUserData("Gen_Contraband", "sIsland", XI_ConvertString(GetIslandByCityName(npchar.city)));
			}
			AddQuestUserData("Gen_Contraband", "sLoc", GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt"));
				
			DialogExit();
		break;
		
		case "First time":			
			if(NPChar.quest.meeting == "0")
			{
				Dialog.Text = "��� ��� ����, �������? � �� ���� ������ �����, � �� ���� ������� ��� ����.";
				Link.l1 = "� ������� " + GetFullName(pchar) + ".";
				Link.l1.go = "meeting";
				NPChar.quest.meeting = "1"; 
			}
			else
			{
				Dialog.Text = "��� �� �������� �� ���� ���, �������?";
				if (LAi_group_GetPlayerAlarm() > 0)
				{
	       			Dialog.Text = RandPhraseSimple("������ �������, ��������, ��� ���� ����? � ��, ������ ��, ������ �� �����!", "�� �����, ������ ������� ���� ����! �� ����� �������� ������, �� �� ������ �����, ������ ��...");
				}
				if(FindFirstContrabandGoods(PChar) != -1)
				{
					Link.l1 = "� ������ ������� ������� �� �������.";
					Link.l1.go = "Meeting_3";
				}

				if (CheckAttribute(pchar, "GenQuest.contraTravel.active") && sti(pchar.GenQuest.contraTravel.active) == true)
					Link.l2 = "�� ���� �������...";
				else
					Link.l2 = "��� ����� ��������� ���-����.";
				Link.l2.go = "Travel";
				
				Link.l3 = "������. �� �������.";
				Link.l3.go = "Exit";				
			}
		break;

		case "Meeting":
			Dialog.Text = "��� ���� ���� ������� ���, �������?";
			if(FindFirstContrabandGoods(PChar) != -1)
			{
				Link.l1 = "� ���� ���-��� �������.";
				Link.l1.go = "Meeting_1";
			}
			Link.l2 = "��� ����� ��������� ���-����.";
			Link.l2.go = "Travel";				
			Link.l3 = "�����. �����!";
			Link.l3.go = "Exit";				
		break;

		case "Meeting_1":
			Dialog.Text = "��... ������ ����� �? ��������, �� �� ���� �����, �������. ����� � ������� - ��� � �������� ����� ��, ��� �� ��������.";
			Link.l1 = "�����, ��� ��� ����� ������ ��.";
			Link.l1.go = "Meeting_2";				
			Link.l2 = "������� �� �����. ����� ��������.";
			Link.l2.go = "exit";				
		break;

		case "Meeting_2":
			Dialog.Text = "����� ��?";
			Link.l1 = "� ���� ������� ��, ��� �� ����� �� ���� �������� �� ���� �������.";
			Link.l1.go = "Meeting_3";				
		break;

		case "Meeting_3":
			bOk  = CheckAttribute(pchar, "GenQuest.contraTravel.active") && (sti(pchar.GenQuest.contraTravel.active) == true);
			bOk2 = CheckAttribute(Pchar, "quest.Contraband.Active") && (sti(Pchar.quest.Contraband.Active) == true);
			if (bOk)
			{
			    if (GetQuestPastDayParam("contraTravel") > sti(PChar.GenQuest.contraTravel.days))
				{  // ���������
					DeleteAttribute(PChar, "GenQuest.contraTravel");
					CloseQuestHeader("Gen_ContrabandTravel");
					bOk = false;
				}
			}

//navy --> PGG
			if (CheckFreeServiceForNPC(NPChar, "Smugglers") != -1)
			{
				Dialog.Text = "������ ������, � ��� ��� ���� ����. ����� ����� ���� ����.";
				Link.l1 = "����...";
				Link.l1.go = "Exit";		
				break;
			}
//navy <--

			if (bOk || bOk2)
			{
				Dialog.Text = "�����, ����� ��������� ���� ���� ������, ��� ������� �� ������?";
				Link.l1 = "�������, �� ����.";
				Link.l1.go = "Exit";				
			}
			else
			{
                if (npchar.quest.trade_date != lastspeak_date)
    			{
                    npchar.quest.trade_date = lastspeak_date;
                    
                    if (ChangeContrabandRelation(pchar, 0) > 5)
                    {
                        Pchar.quest.contraband.CurrentPlace = SelectSmugglingLocation();
                        if (Pchar.quest.contraband.CurrentPlace != "None")//boal fix
                        {
                            if (ChangeContrabandRelation(pchar, 0) >= 70)
                            {
                                Dialog.Text = "� ����, � ����� ����� ����� ����. �� ����� ����� ���� � �����, ������������ " + GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt") + ".";
                            }
                            else
                            {
            				    Dialog.Text = "��... ��������, ���������� � ��������. �� ����� ����� ��� � �����, ������������ " + GetConvertStr(Pchar.quest.contraband.CurrentPlace, "LocLables.txt") + ".";
            				}
            				Link.l1 = "������. �� �������.";
            				Link.l1.go = "Smuggling_exit";
        				}
        				else
        				{   //boal fix
                            Dialog.Text = "������� ������ ������ �� �����. ������� ������.";
            				Link.l1 = "������.";
            				Link.l1.go = "Exit";
        				}
    				}
    				else
    				{
                        Dialog.Text = "� ����� ����� �� �������, ��� ���-�� ������� �������� � �����? �������, ��� �� ��� �� ������� ������� ����� �� ����� �������. ��������!";
        				Link.l1 = "��.. ������ �� ������ ��� ����� ���������������.";
        				Link.l1.go = "Exit";
    				}
				}
				else
				{
                    Dialog.Text = "������� ������ ������ �� �����. ������� ������.";
    				Link.l1 = "������.";
    				Link.l1.go = "Exit";
				}
			}
		break;
////////////////////////////////////////////////////////////////////////////////
//	���������� �����
////////////////////////////////////////////////////////////////////////////////
		case "Travel":
//navy --> PGG
			if (CheckFreeServiceForNPC(NPChar, "Smugglers") != -1)
			{
				Dialog.Text = "������ ������, � ��� ��� ���� ����. ����� ����� ���� ����.";
				Link.l1 = "����...";
				Link.l1.go = "Exit";		
				break;
			}
//navy <--
			//���� ��� ������� � �� � ��� ����������� ��� ��
			if (sti(pchar.ship.type) == SHIP_NOTUSED && GetCompanionQuantity(pchar) == 1 && GetPassengersQuantity(pchar) == 0)
			{
				//������ ���� ��� ���� ����
				if (CheckAttribute(pchar, "GenQuest.contraTravel.active") && sti(pchar.GenQuest.contraTravel.active) == true)
				{
					//������ ���
					if (CheckAttribute(pchar, "GenQuest.contraTravel.payed") && sti(pchar.GenQuest.contraTravel.payed) == true)
					{
						Dialog.Text = "�� ������� ��� ������������?";
						Link.l2 = "��, �����!";
					}
					//�� ������, ������ ����� ���������� ���� �� ����� ����.
					else
					{
						if(GetQuestPastDayParam("contraTravel") == sti(PChar.GenQuest.contraTravel.days))
						{
							Dialog.Text = "������ ������?";
							Link.l1 = "��.";
							Link.l1.go = "Travel_pay";
							Link.l3 = "� ���������....";
							Link.l3.go = "Travel_abort";
							Link.l2 = "��� ����.";
						}
						else
						{
                            if (GetQuestPastDayParam("contraTravel") < sti(PChar.GenQuest.contraTravel.days))
							{
								Dialog.Text = "� ���� ��� ��� ������.";
								Link.l2 = "�����.";
								Link.l1 = "� ���������....";
								Link.l1.go = "Travel_abort";
							}
							else // ���������
							{
							    Dialog.Text = "������� � ����� �� ���� ������. ������� ����� ���� ����, ����� ��� �����.";
								Link.l2 = "����.";
								DeleteAttribute(PChar, "GenQuest.contraTravel");
								CloseQuestHeader("Gen_ContrabandTravel");
							}
						}
					}
				}
				//���� �� ���� ��������, ������������ �������
				else
				{
					nRel = ChangeContrabandRelation(pchar, 0);
					//���� ���������� ��������� � ���������� ������� ������ 20, ��������....
					if (nRel > 0 && Statistic_AddValue(PChar, "contr_TravelKill", 0) < 20)
					{
						//�����...
						pchar.GenQuest.contraTravel.CurrentPlace = SelectSmugglingLocation();
						aref arTmp; makearef(arTmp, pchar.GenQuest.contraTravel);
						SetSmugglersTravelDestination(arTmp);
						//�� ������� �������� 
						pchar.GenQuest.contraTravel.price = (sti(PChar.rank)*250 + (100 - nRel)*10 + rand(30)*20) + sti(arTmp.destination.days)*100;
						
						//���� ����� �������, � ��� ���������� � ��, � ��� ������� �� ��������, ���� ��������� �����, � ���� ...
						bOk = !bPauseContrabandMetro && CheckNPCQuestDate(npchar, "Travel_Talk") && 
							Pchar.GenQuest.contraTravel.CurrentPlace != "None" && rand(50) < nRel;
                        bOk2 = CheckAttribute(Pchar, "quest.Contraband.Active") && (sti(Pchar.quest.Contraband.Active) == true);
						if (bOk && !bOk2)
						{
							nDay = 1 + rand(3);
							SetNPCQuestDate(npchar, "Travel_Talk");
							SaveCurrentQuestDateParam("contraTravel");

							Dialog.Text = "��� �, ����� ��������� ���� �� ����� " + GetConvertStr(locations[FindLocation(pchar.GenQuest.contraTravel.destination.loc)].id, "LocLables.txt") + " ���� " +
								XI_ConvertString("Colony" + pchar.GenQuest.contraTravel.destination + "Gen") + " �� " + pchar.GenQuest.contraTravel.price + " �������. ��������� ������ ����� " +
								FindRussianDaysString(nDay) + ". ������� ����� ����� ���� � ����� ��� ��������� " +
								GetConvertStr(locations[FindLocation(Pchar.GenQuest.contraTravel.CurrentPlace)].id, "LocLables.txt") + " ����� �����.";

							pchar.GenQuest.contraTravel.days = nDay;
							Link.l1 = "������, ���� ����������.";
							Link.l1.go = "Travel_agree";
							Link.l2 = "���, �������.";
						}
						else
						{
							Dialog.Text = "������� � ����� �� ���� ������. ������� ����� ���� ����, ����� ��� �����.";
							Link.l2 = "����.";
						}
					}
					//���, �������� � ���
					else
					{
                        Dialog.Text = "� ����� ����� �� �������, ��� ���-�� ������� �������� � �����? �������, ��� �� ��� �� ������� ������� ����� �� ����� �������. ��������!";
        				Link.l2 = "��.. ������ �� ������.";
					}

				}
			}
			//������� ����, �������� � ���...
			else 
			{
				if(GetPassengersQuantity(pchar) != 0)
				{
					Dialog.Text = "���, �� �� ������� ���. ������ ������.";
					Link.l2 = RandSwear() + " � �� �����!";
				}
				else
				{
					Dialog.Text = "� ���� ������� ���� �����? ���, �� ���� �� ����������.";
					Link.l2 = "����� �� ������...";
				}
			}
			Link.l2.go = "Exit";
			break;

		//������
		case "Travel_abort":
			ChangeContrabandRelation(pchar, -2);
			DeleteAttribute(PChar, "GenQuest.contraTravel");
			CloseQuestHeader("Gen_ContrabandTravel");
			Dialog.Text = "�� ��� ������...";
			Link.l1 = "";
			Link.l1.go = "Exit";
			break;

		//�� �������� �����
		case "Travel_agree":
			ReOpenQuestHeader("Gen_ContrabandTravel");
			AddQuestRecord("Gen_ContrabandTravel", "1");
			AddQuestUserData("Gen_ContrabandTravel", "sLoc", GetConvertStr(pchar.GenQuest.contraTravel.CurrentPlace, "LocLables.txt"));
			AddQuestUserData("Gen_ContrabandTravel", "sLocTo", GetConvertStr(pchar.GenQuest.contraTravel.destination.loc, "LocLables.txt"));
			AddQuestUserData("Gen_ContrabandTravel", "sPlaceTo", XI_ConvertString("Colony" + pchar.GenQuest.contraTravel.destination + "Gen"));
			AddQuestUserData("Gen_ContrabandTravel", "sDays", FindRussianDaysString(sti(pchar.GenQuest.contraTravel.days)));
			AddQuestUserData("Gen_ContrabandTravel", "sPrice", pchar.GenQuest.contraTravel.price);

			//���������� �����
			pchar.GenQuest.contraTravel.active = true;
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			break;

		//�� �������� �������
		case "Travel_pay":
			//����� �������?
			Sum = sti(pchar.GenQuest.contraTravel.price);
			if (sti(pchar.money) >= Sum)
			{
				AddMoneyToCharacter(pchar, -1*Sum);
				//������ ���� ������
				pchar.GenQuest.contraTravel.payed = true;
				Dialog.Text = "������� ����� � ����� ����. ������� ����, �� �������.";
				Link.l1 = "����������.";
				AddQuestRecord("Gen_ContrabandTravel", "2");
				
				//������ ������.
				PlaceSmugglersOnShore(PChar.GenQuest.contraTravel.CurrentPlace);
				//������� �� ������ � �����....
				Sum = sti(pchar.GenQuest.contraTravel.destination.days);
				sld = GetCharacter(NPC_GenerateCharacter("Abracham_Gray", "pirate_6", "man", "man", 5, PIRATE, Sum + 2, true));
				//���������...
				sld.nation = PIRATE;
				SetRandomNameToCharacter(sld);
				SetMerchantShip(sld, rand(GOOD_SILVER));
				SetFantomParamHunter(sld);
				SetCaptanModelByEncType(sld, "pirate");
				SetCharacterShipLocation(sld, PChar.GenQuest.contraTravel.CurrentPlace);

				SetTimerCondition("RemoveTravelSmugglers", 0, 0, 1, false);
			}
			//���, �������� � ���...
			else
			{
				Dialog.Text = "������ � ���� �������� � �����������."
				Link.l1 = "����, ����� �����.";
			}
			Link.l1.go = "Exit";
			break;  
			
////////////////////////////////////////////////////////////////////////////////
//	END OF ���������� �����
////////////////////////////////////////////////////////////////////////////////
	}
}