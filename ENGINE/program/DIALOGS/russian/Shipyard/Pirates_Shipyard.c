// ������ �� �������
// ��� ������ �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    if ( sti(Pchar.Ship.Type) != SHIP_NOTUSED)
    {
	    int   cannonQ       = sti(RealShips[sti(Pchar.Ship.Type)].CannonsQuantity);
		int   cannonMax     = sti(RealShips[sti(Pchar.Ship.Type)].MaxCaliber);
		int   shipClass     = sti(RealShips[sti(Pchar.Ship.Type)].Class);
		//int   shipPrice     = sti(RealShips[sti(Pchar.Ship.Type)].Price);
		int   shipCapacity  = sti(RealShips[sti(Pchar.Ship.Type)].Capacity);
		float shipSpeedRate = stf(RealShips[sti(Pchar.Ship.Type)].SpeedRate);
		int   shipMaxCrew   = sti(RealShips[sti(Pchar.Ship.Type)].MaxCrew);
		int   shipHP        = sti(RealShips[sti(Pchar.Ship.Type)].HP);
		float shipTurnRate  = stf(RealShips[sti(Pchar.Ship.Type)].TurnRate);
		
	    int  cannonMatherial1 = makeint(cannonQ * (cannonMax/shipClass));
	    int	 cannonMatherial2 = makeint(cannonQ * (30/shipClass));
	    int  cannonWorkPrice  = makeint(30*(10-shipClass)/(0.5*shipClass) * cannonQ * cannonMax);
	    //int  cannonWorkPrice  = makeint(shipPrice * cannonQ * cannonMax / 2000);
	    
	    int  �apacityMatherial1 = makeint(shipCapacity /(shipClass*2));
	    int	 �apacityMatherial2 = makeint(cannonQ *10 /shipClass);
	    int  �apacityWorkPrice  = makeint(shipCapacity/5 * 200);
	    
	    int  SpeedRateMatherial1 = makeint(shipSpeedRate*18 / shipClass);
	    int	 SpeedRateMatherial2 = makeint(shipSpeedRate*24 / shipClass);
	    int  SpeedRateWorkPrice  = makeint(shipSpeedRate/5 * 10000*6/shipClass);
	    
	    int  MaxCrewMatherial1 = makeint(shipMaxCrew*10 / shipClass);
	    int	 MaxCrewMatherial2 = makeint(shipMaxCrew*12 / shipClass);
	    int  MaxCrewWorkPrice  = makeint(shipMaxCrew/5 * 500*6/shipClass);
	    
	    int  TurnRateMatherial1 = makeint(shipTurnRate*8 / shipClass);
	    int	 TurnRateMatherial2 = makeint(shipTurnRate*10 / shipClass);
	    int  TurnRateWorkPrice  = makeint(shipTurnRate/5 * 3350*6/shipClass);
	    
	    int  HPMatherial1 = makeint(shipHP / (5*shipClass));
	    int	 HPMatherial2 = makeint(shipHP / (7*shipClass));
	    int  HPWorkPrice  = makeint(shipHP/5 * 70*6/shipClass);
	
	}
	int amount;
	ref shTo;
    string sTemp;
    string s1;
    
	switch (Dialog.CurrentNode)
	{
		
		// -----------------------------------������ ������ - ������ �������
		case "First time":  // �������� ��������� �������, ������ ���� ���
			if(NPChar.quest.meeting == "0")
			{
				Dialog.Text = "���... ��������� ������. ���� ����� " + GetFullName(npchar) + ", �, ������ ���� ������, � ������ ������� �� ���� �������� ��������.";
				Link.l1 = pcharrepphrase("������ ����, ������ ������ ����� ��� �� �������. ��� ��� " + GetFullName(pchar) + ", �����, �� ��� ��� ������.",
	            "��������� � ���� �� ����������� - ������ ���������. ���� ����� " + GetFullName(pchar) + ", ����� �� ��� ��� ������.");
				Link.l1.go = "node_1";
				npchar.quest.meeting = "1";
			}
			else // �����, ���� �� ������� ����� ����� ��  "First time"
			{
				dialog.text = "�, ��� ����� ��, ����� �� ������ ����������... ���... ���.";
				link.l1 = pcharrepphrase("�� ����, ��� ����� �, � ��, � ������, ��� ��� �� ��������� ������, �� ����� ��������� � �����.",
	            "� ��� ������� ������, � ������, ���� ���������, ��� �������� �� ������� � ����� ���������� ��������.");
				link.l1.go = "node_3";
				link.l2 = pcharrepphrase("�� ������ ���� ������ �������� �� �����. ������.",
	            "��� ���� ����, ��������, ��� �����������.");
				link.l2.go = "exit";
			}
			NextDiag.TempNode = "Master_Ship";
			Dialog.CurrentNode = "Master_Ship";
		break;
		
		case "Master_Ship":
			dialog.text = "�, ��� ����� ��, ����� �� ������ ����������... ���... ���.";
			link.l1 = pcharrepphrase("�� ����, ��� ����� �, � ��, � ������, ��� ��� �� ��������� ������, �� ����� ��������� � �����.",
            "� ��� ������� ������, � ������, ���� ���������, ��� �������� �� ������� � ����� ���������� ��������.");
			link.l1.go = "node_3";
			link.l2 = pcharrepphrase("�� ������ ���� ������ �������� �� �����. ������.",
            "��� ���� ����, ��������, ��� �����������.");
			link.l2.go = "exit";
			NextDiag.TempNode = "Master_Ship";
		break;

		case "node_1":
			dialog.text = "��������, ��������, �� ��������� �� ��� �������� ��������. ������� ������, ������� ����, ��� ��� ��� ����?";
			link.l1 = pcharrepphrase("��, ������, ������ �� ��, ��� ���� ��������� ����������� ��� �����, ��� �� ����� ��� � �� �������.",
            "��������, �� �� �� ���� ���� �������, ��� �� ������ �������� ���?");
			link.l1.go = "node_2";
		break;

		case "node_2":
			dialog.text = "�� ������ �������, �����!";
			link.l1 = pcharrepphrase("��� �� ���� ������?! ��� ���� ���� ����� � ����� ���� ������, � ���� �� ����������. �������-�� ����� ������, ������ ������� ���������.",
            "����� ������ �� �������� �� ���� �����. � ������, ���� �� ��������� ���� �������, ��� �������� �� ������� � �����.");
			link.l1.go = "node_3";
			link.l2 = pcharrepphrase("���, � ���� ������ ����� �������������������! ������!",
            "� ����, ��� �� � ���� �� ����� ����� ����. ��������.");
			link.l2.go = "exit";
		break;

		case "node_3":
			dialog.text = "���, ���... ��� ��� ��� �� ���� �����, " + GetAddress_Form(NPChar) + "?";
			link.l1 = pcharrepphrase("��� ���� ��������������� �������� ����� �����.","� ���� ��������������� �������� ����� �����.");
			link.l1.go = "shipyard_dlg";
            Link.l2 = "� ���� ������.";
			Link.l2.go = "quests"; //(���������� � ���� ������)
			// -->
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
			{
				link.l3 = "� ����� �� ���������� � ���� � ����� ����������.";
				link.l3.go = "LoanForAll";//(���������� � ���� ������)
			}
			link.l9 = pcharrepphrase("����, � ���� ���� ���� ���������� ���, ��������� ������������ �������.", "��� ��� ���� ����.");
			link.l9.go = "exit";
			NextDiag.TempNode = "Master_Ship";
		break;
		
		case "shipyard_dlg":
			dialog.Text = "���� ���������� ������ �� �������� �������� - ������� �� ��� �������, ������� ����� ������� �� ����!";
			Link.l1 = "������������! ��� ��, ��� ��� �����. � ���� ��� ��� ���� ����� ��������. ����� ���������?";
			Link.l1.go = "ship_tunning_start";
			link.l2 = "���, ��� �� ����� ���������, � ���� ������ ��������������� �������� �����.";
			link.l2.go = "shipyard";
			link.l3 = "��� ����� ������ �� �������.";
			link.l3.go = "Cannons";
			Link.l9 = "�.. ��������. � ��� �����.";
			Link.l9.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_again":
			dialog.Text = RandPhraseSimple("�.. ��� ����� ��... ����� �������� ���� ���������?",
                                           RandPhraseSimple("����������� ���������� �������������. ��� �������� �� ����������?",
                                           "��.. �� ������ ����� ���, �������������� �� ����.. ����� �� �� ������ �����������?"));
			Link.l1 = "��. ���� ����� ����. ����� ���������, ��� ����� ������� ���?";
			Link.l1.go = "ship_tunning_start";
			link.l2 = "���, ��� �� ����� ���������, � ���� ������ ��������������� �������� �����.";
			link.l2.go = "shipyard";
			link.l3 = "��� ����� ������ �� �������.";
			link.l3.go = "Cannons";
            Link.l4 = "� ���� ������.";
			Link.l4.go = "quests"; //(���������� � ���� ������)
			// -->
			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
			{
				link.l5 = "� ����� �� ���������� � ���� � ����� ����������.";
				link.l5.go = "LoanForAll";//(���������� � ���� ������)
			}
			Link.l9 = "���. ����� ���� ������� ��, ��� ����";
			Link.l9.go = "ship_tunning_not_now_2";
		break;
		
        case "ship_tunning_not_now_2":
			dialog.Text = "�� ��, � �������, ��� � �������! ������ �������! �������, ����� �������� ��������� ��������.";
			Link.l1 = RandPhraseSimple("�������! ����� ��������.", "����, ������. ����� � ��� ��������.");
			Link.l1.go = "ship_tunning_not_now";
		break;
        		
        case "ship_tunning_start":
            if ( sti(Pchar.Ship.Type) == SHIP_NOTUSED || Pchar.location.from_sea != "Pirates_town")
            {
                dialog.Text = "�������-�� ���? ��� �� ��� ������ ��� ��� ��������!";
			    Link.l1 = "� �� ������.. ��� ��� �... ���������";
			    Link.l1.go = "ship_tunning_not_now";
            }
            else
            {
			    if (GetHour() == 13)
			    {
                    dialog.Text = "�, �������, ����������, �� � � ���� ���� �������� �����. ������� �����!";
			        Link.l1 = "�������...";
			        Link.l1.go = "ship_tunning_not_now";
			    }
			    else
			    {
                    if (shipClass == 7)
                    {
                        dialog.Text = "�.. ������� � �� ���������. �� ��� ��� ������!";
    			        Link.l1 = "�������...";
    			        Link.l1.go = "ship_tunning_not_now";
    			        break;
                    }

                    s1 = "��� ���.. ���������, ��� ��� � ��� ����... ��� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) + ".";
    			    if (sti(RealShips[sti(Pchar.Ship.Type)].Stolen) == true && !CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning"))
    			    {
    			        s1 = s1 + " ��! �� �������-�� ������� �� � ����� �����. ��-�� �� ����� ��������. ������ ���������� � ���� ����? ����� ����... � ����.";
    			    }
    			    s1 = s1 + " ��� �� �� ����� � ��� �������?";
                    dialog.Text = s1;
                    if (cannonMax < 32 && !CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.Cannon"))
                    {
                        Link.l1 = "��������� ������ ������";
    			        Link.l1.go = "ship_tunning_cannon";
    			    }
    			    if (!CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.Capacity"))
                    {
    			        Link.l2 = "��������� ����";
    			        Link.l2.go = "ship_tunning_Capacity";
    			    }
    			    if (!CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.SpeedRate"))
                    {
    			        Link.l3 = "��������� ��������";
    			        Link.l3.go = "ship_tunning_SpeedRate";
    			    }
    			    if (!CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.TurnRate"))
                    {
    			        Link.l4 = "��������� �������������";
    			        Link.l4.go = "ship_tunning_TurnRate";
    			    }
    			    if (!CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.MaxCrew"))
                    {
    			        Link.l5 = "��������� ����� ��� �������";
    			        Link.l5.go = "ship_tunning_MaxCrew";
    			    }
    			    if (!CheckAttribute(&RealShips[sti(Pchar.Ship.Type)], "Tuning.HP"))
                    {
    			        Link.l6 = "��������� ��������� �������";
    			        Link.l6.go = "ship_tunning_HP";
    			    }
    			    Link.l10 = "��������! � ���������.";
    			    Link.l10.go = "ship_tunning_not_now_2";
			    }
			}
		break;
		
		case "ship_tunning_cannon":
			s1 = "����� ��������, ��� ����� �������. ����� � ���� " + cannonQ + ", ������ ������������ " + cannonMax + "�. ";
			
			s1 = s1 + " ��� ����������� "+ cannonMatherial1 + " ������� ������ ��� �������� �������� � "+ cannonMatherial2+" ���� ��� ������� ������ ������� ������.";
			s1 = s1 + " �� ������ � ������� " + cannonWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_cannon_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_cannon_start":
		    amount = cannonWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
				NPChar.Tuning.Cannon = true;
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = cannonMatherial1; // ������ ������
			    NPChar.Tuning.Matherial2 = cannonMatherial2; // ����
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			    
				NextDiag.TempNode = "ship_tunning_cannon_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText", "��� ���������� ������� ������ �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. ������� ������, "+ NPChar.Tuning.Matherial2+" ��. ���� � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_cannon_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_cannon_again";
			    dialog.Text = "����� ���� ����. ������ " + sti(NPChar.Tuning.Matherial1) + " ������� ������, " + sti(NPChar.Tuning.Matherial2) + " ���� � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_cannon_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_cannon_again_2":
		    amount = makeint(Pchar.money) - sti(NPChar.Tuning.Money);
		    if (amount < 0)
		    {
		       amount = amount + sti(NPChar.Tuning.Money);
		    }
		    else
		    {
		        amount = sti(NPChar.Tuning.Money);
		    }
		    AddMoneyToCharacter(Pchar, -amount);
		    NPChar.Tuning.Money = makeint(sti(NPChar.Tuning.Money) - amount);
		    
		    amount = GetSquadronGoods(Pchar, GOOD_EBONY) - sti(NPChar.Tuning.Matherial1);
		    if (amount < 0)
		    {
		       amount = amount + sti(NPChar.Tuning.Matherial1);
		    }
		    else
		    {
		        amount = sti(NPChar.Tuning.Matherial1);
		    }
		    RemoveCharacterGoods(Pchar, GOOD_EBONY, amount);
		    NPChar.Tuning.Matherial1 = sti(NPChar.Tuning.Matherial1) - amount;
		    
		    amount = GetSquadronGoods(Pchar, GOOD_LEATHER) - sti(NPChar.Tuning.Matherial2);
		    if (amount < 0)
		    {
		       amount = amount + sti(NPChar.Tuning.Matherial2);
		    }
		    else
		    {
		        amount = sti(NPChar.Tuning.Matherial2);
		    }
		    RemoveCharacterGoods(Pchar, GOOD_LEATHER, amount);
		    NPChar.Tuning.Matherial2 = sti(NPChar.Tuning.Matherial2) - amount;
		    
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_cannon_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_cannon_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " ������� ������, " + sti(NPChar.Tuning.Matherial2) + " ���� � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText", "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. ������� ������, "+ sti(NPChar.Tuning.Matherial2)+" ��. ���� � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_cannon_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	    int iCaliber = sti(shTo.MaxCaliber);
        switch(iCaliber)
	    {
		case 12:
			iCaliber = 0;
		break;
		case 16:
			iCaliber = 1;
		break;
		case 24:
			iCaliber = 2;
		break;
		case 32:
			iCaliber = 3;
		break;
	    }
	    iCaliber = iCaliber + 1;
	    if (iCaliber > 3) iCaliber = 3;
	    switch(iCaliber)
	    {
		case 0:
			iCaliber = 12;
		break;
		case 1:
			iCaliber = 16;
		break;
		case 2:
			iCaliber = 24;
		break;
		case 3:
			iCaliber = 32;
		break;
	    }
	    shTo.MaxCaliber = iCaliber;
	    shTo.Tuning.Cannon = true;
	    // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ������ ���� �� ����� � ������ ������ �� ��������";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		////////////////////////////////////////// ���� ////////////////////////////////////////////////////
		case "ship_tunning_Capacity":
			s1 = "����� ��������, ��� ����� �������. ���� � ���� " + shipCapacity;
			
			s1 = s1 + " ��� ����������� "+ �apacityMatherial1 + " �������� ������ ��� ���������� ������ ��������� � "+ �apacityMatherial2+" ����� ��� �������� ����.";
			s1 = s1 + " �� ������ � ������� " + �apacityWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_Capacity_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_Capacity_start":
		    amount = �apacityWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = �apacityMatherial1; // ������� ������ GOOD_MAHOGANY
			    NPChar.Tuning.Matherial2 = �apacityMatherial2; //  GOOD_OIL
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			
				NextDiag.TempNode = "ship_tunning_Capacity_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText", "��� ���������� ����� �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. �������� ������, "+ NPChar.Tuning.Matherial2+" ��. ����� � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_Capacity_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_Capacity_again";
			    dialog.Text = "���� ����. ������ " + sti(NPChar.Tuning.Matherial1) + " �������� ������, " + sti(NPChar.Tuning.Matherial2) + " ����� � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_Capacity_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_Capacity_again_2":
		    checkMatherial(Pchar, NPChar, GOOD_MAHOGANY, GOOD_OIL);
		
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_Capacity_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_Capacity_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " �������� ������, " + sti(NPChar.Tuning.Matherial2) + " ����� � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText", "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. �������� ������, "+ sti(NPChar.Tuning.Matherial2)+" ��. ����� � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_Capacity_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	        shTo.Capacity        = sti(shTo.Capacity) + makeint(sti(shTo.Capacity)/5);
	        shTo.Tuning.Capacity = true;
	        // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ������ ���� � ������ � ��������� �� ������";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		
		////////////////////////////////////////// SpeedRate ////////////////////////////////////////////////////
		case "ship_tunning_SpeedRate":
			s1 = "����� ��������, ��� ����� �������. �������� � ���� " + shipSpeedRate;
			
			s1 = s1 + " ��� ����������� "+ SpeedRateMatherial1 + " �������� � "+ SpeedRateMatherial2+" �����";
			s1 = s1 + " �� ������ � ������� " + SpeedRateWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_SpeedRate_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_SpeedRate_start":
		    amount = SpeedRateWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = SpeedRateMatherial1; // GOOD_SAILCLOTH
			    NPChar.Tuning.Matherial2 = SpeedRateMatherial2; //  GOOD_SILK
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			
				NextDiag.TempNode = "ship_tunning_SpeedRate_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText", "��� ���������� �������� �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. ��������, "+ NPChar.Tuning.Matherial2+" ��. ����� � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_SpeedRate_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_SpeedRate_again";
			    dialog.Text = "������ ����. ������ " + sti(NPChar.Tuning.Matherial1) + " ��������, " + sti(NPChar.Tuning.Matherial2) + " ����� � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_SpeedRate_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_SpeedRate_again_2":
		    checkMatherial(Pchar, NPChar, GOOD_SAILCLOTH, GOOD_SILK);
		    
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_SpeedRate_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_SpeedRate_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " ��������, " + sti(NPChar.Tuning.Matherial2) + " ����� � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. ��������, "+ sti(NPChar.Tuning.Matherial2)+" ��. ����� � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_SpeedRate_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	        shTo.SpeedRate        = (stf(shTo.SpeedRate) + stf(shTo.SpeedRate)/5.0);
	        shTo.Tuning.SpeedRate = true;
	        // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ������ ������ ����� ������� ��������";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		////////////////////////////////////////// MaxCrew ////////////////////////////////////////////////////
		case "ship_tunning_MaxCrew":
			s1 = "����� ��������, ��� ����� �������. ������������ ������ ���� � ������ ��������� " + shipMaxCrew + " ��������";
			
			s1 = s1 + " ��� ����������� "+ MaxCrewMatherial1 + " ������� � "+ MaxCrewMatherial2+" ������";
			s1 = s1 + " �� ������ � ������� " + MaxCrewWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_MaxCrew_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_MaxCrew_start":
		    amount = MaxCrewWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = MaxCrewMatherial1; // GOOD_FRUITS
			    NPChar.Tuning.Matherial2 = MaxCrewMatherial2; //  GOOD_CLOTHES
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			
				NextDiag.TempNode = "ship_tunning_MaxCrew_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� ���������� ������� �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. �������, "+ NPChar.Tuning.Matherial2+" ��. ������ � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_MaxCrew_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_MaxCrew_again";
			    dialog.Text = "������ ����. ������ " + sti(NPChar.Tuning.Matherial1) + " �������, " + sti(NPChar.Tuning.Matherial2) + " ������ � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_MaxCrew_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_MaxCrew_again_2":
		    checkMatherial(Pchar, NPChar, GOOD_FRUITS, GOOD_CLOTHES);
		    
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_MaxCrew_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_MaxCrew_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " �������, " + sti(NPChar.Tuning.Matherial2) + " ������ � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. �������, "+ sti(NPChar.Tuning.Matherial2)+" ��. ������ � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_MaxCrew_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	        shTo.MaxCrew        = sti(shTo.MaxCrew) + makeint(sti(shTo.MaxCrew)/5);
	        shTo.Tuning.MaxCrew = true;
	        // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ������� ����� ������";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		
		////////////////////////////////////////// TurnRate ////////////////////////////////////////////////////
		case "ship_tunning_TurnRate":
			s1 = "����� ��������, ��� ����� �������. ������������� " + shipTurnRate;
			
			s1 = s1 + " ��� ����������� "+ TurnRateMatherial1 + " ����� � "+ TurnRateMatherial2+" �������";
			s1 = s1 + " �� ������ � ������� " + TurnRateWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_TurnRate_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_TurnRate_start":
		    amount = TurnRateWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = TurnRateMatherial1; // GOOD_PLANKS
			    NPChar.Tuning.Matherial2 = TurnRateMatherial2; //  GOOD_LINEN
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			
				NextDiag.TempNode = "ship_tunning_TurnRate_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� ���������� ������������� �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. �����, "+ NPChar.Tuning.Matherial2+" ��. ������� � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_TurnRate_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_TurnRate_again";
			    dialog.Text = "������ ����. ������ " + sti(NPChar.Tuning.Matherial1) + " �����, " + sti(NPChar.Tuning.Matherial2) + " ������� � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_TurnRate_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_TurnRate_again_2":
		    checkMatherial(Pchar, NPChar, GOOD_PLANKS, GOOD_LINEN);
		
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_TurnRate_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_TurnRate_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " �����, " + sti(NPChar.Tuning.Matherial2) + " ������� � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. �����, "+ sti(NPChar.Tuning.Matherial2)+" ��. ������� � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_TurnRate_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	        shTo.TurnRate        = (stf(shTo.TurnRate) + stf(shTo.TurnRate)/5.0);
	        shTo.Tuning.TurnRate = true;
	        // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ������ ������� �������";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		////////////////////////////////////////// HP ////////////////////////////////////////////////////
		case "ship_tunning_HP":
			s1 = "����� ��������, ��� ����� �������. ������ ������ " + shipHP;
			
			s1 = s1 + " ��� ����������� "+ HPMatherial1 + " ������� ������ � "+ HPMatherial2+" �������";
			s1 = s1 + " �� ������ � ������� " + HPWorkPrice + " �������. �������� ������.";
            dialog.Text = s1;
			Link.l1 = "�������. � �������� �������. ��������� � �������";
			Link.l1.go = "ship_tunning_HP_start";
			Link.l2 = "���. ���� ��� �� ����������.";
			Link.l2.go = "ship_tunning_not_now";
		break;
		
		case "ship_tunning_HP_start":
		    amount = HPWorkPrice;
		    if(makeint(Pchar.money) >= makeint(amount / 2 +0.5))
			{
				AddMoneyToCharacter(Pchar, -makeint(amount / 2 +0.5));
			    NPChar.Tuning.Money  = makeint(amount - makeint(amount / 2 +0.5));
			    NPChar.Tuning.Matherial1 = HPMatherial1; // GOOD_EBONY
			    NPChar.Tuning.Matherial2 = HPMatherial2; //  GOOD_SANDAL
			    NPChar.Tuning.ShipType       = Pchar.Ship.Type;
			    NPChar.Tuning.ShipName       = RealShips[sti(Pchar.Ship.Type)].BaseName;
			
				NextDiag.TempNode = "ship_tunning_HP_again";
                dialog.text = "��� � ������. ��� �������� ��� ������";
			    link.l1 = "������� �� ���...";
			    link.l1.go = "Exit";
			    
			    ReOpenQuestHeader("ShipTuning");
			    AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� ���������� ��������� ������� �� ������� " + XI_ConvertString(RealShips[sti(Pchar.Ship.Type)].BaseName) +
                " ����������: "+NPChar.Tuning.Matherial1+ " ��. ������� ������, "+ NPChar.Tuning.Matherial2+" ��. ������� � " + NPChar.Tuning.Money + " �������.");
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_not_now";
                dialog.text = "�� ���� �������...";
				link.l1 = "� ����� �����";
				link.l1.go = "Exit";								
			}
		break;
		
		case "ship_tunning_HP_again":
		    if (sti(NPChar.Tuning.ShipType) == sti(Pchar.Ship.Type) && NPChar.Tuning.ShipName      == RealShips[sti(Pchar.Ship.Type)].BaseName)
		    {
                NextDiag.TempNode = "ship_tunning_HP_again";
			    dialog.Text = "������ ����. ������ " + sti(NPChar.Tuning.Matherial1) + " ������� ������, " + sti(NPChar.Tuning.Matherial2) + " ������� � " + sti(NPChar.Tuning.Money) + " ��������?";
			    Link.l1 = "��. ���-��� ������� ���������";
			    Link.l1.go = "ship_tunning_HP_again_2";
			    Link.l2 = "���. ��� �������";
			    Link.l2.go = "Exit";
			}
			else
			{
			    DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
			    dialog.Text = "������� ���, ������, ��� �� �������� ������� �� ������� ������ �������. ��������� ��� ������ �����������..";
			    Link.l1 = "���� ����. ������, ��� ������� ������...";
			    Link.l1.go = "Exit";
			    
			    AddQuestRecord("ShipTuning", "Lose");
			    CloseQuestHeader("ShipTuning");
			}
		break;
		
		case "ship_tunning_HP_again_2":
		    checkMatherial(Pchar, NPChar, GOOD_EBONY, GOOD_SANDAL);
		
		    if(sti(NPChar.Tuning.Matherial2) < 1 && sti(NPChar.Tuning.Matherial1) < 1 && sti(NPChar.Tuning.Money) < 1)
			{
				DeleteAttribute(NPChar, "Tuning");
                NextDiag.TempNode = "ship_tunning_again";
                dialog.text = "��� ������. �������! ������� ������...";
			    link.l1 = "���";
			    link.l1.go = "ship_tunning_HP_complite";
			}
			else
			{
				NextDiag.TempNode = "ship_tunning_HP_again";
                dialog.Text = "���� �������� " + sti(NPChar.Tuning.Matherial1) + " ������� ������, " + sti(NPChar.Tuning.Matherial2) + " ������� � " + sti(NPChar.Tuning.Money) + " ��������";
				link.l1 = "������.";
				link.l1.go = "Exit";

                AddQuestRecord("ShipTuning", "t1");
				AddQuestUserData("ShipTuning", "sText",  "��� �������� ������� ��� " +  sti(NPChar.Tuning.Matherial1)+ " ��. ������� ������, "+ sti(NPChar.Tuning.Matherial2)+" ��. ������� � "+ sti(NPChar.Tuning.Money) + " �������.");
			}
		break;
		
		case "ship_tunning_HP_complite":
		    AddTimeToCurrent(6, 30);
		    shTo = &RealShips[sti(Pchar.Ship.Type)];
		    DeleteAttribute(NPChar, "Tuning");
		    // �������
	        shTo.HP        = sti(shTo.HP) + makeint(sti(shTo.HP)/5);
	        shTo.Tuning.HP = true;
	        // finish <--
            NextDiag.TempNode = "ship_tunning_again";
			dialog.Text = "... ����� �� ���... ���� ������ ������� ���� �������� �����";
			Link.l1 = "�������! ������� �����������";
			Link.l1.go = "Exit";
			
			AddQuestRecord("ShipTuning", "End");
			CloseQuestHeader("ShipTuning");
		break;
		///////////////////////////////  ��������� �����
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("����� �������?", "��� ��� ������?"), "������ ������� �� �������� ������ ��� ������...", "� ���� �� �����, �� � ������ � ������, � ����� ����������� �������������� �� �����.",
                          "�� ��� �� �������? ��� ���� - ������� �������, ������� ���� � ��������.", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("� ���������...", "������ ��� �� � ��� ��������"), "��, ���-�� � ������� ���� �����...",
                      "��, ������...", "�������...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (!CheckAttribute(npchar, "Step_Door"))
			{
				link.l2 = "��������, � ���� ������, � ����� �������...";
				link.l2.go = "Step_Door_1";
			}
        // ==> �������� �� ������
			if (pchar.questTemp.BlueBird == "toSeekBermudes") //������ ����� �����
			{
				link.l1 = "����������, ��� ����� ����� ���� ���� ���������� ������ ����������...";
				link.l1.go = "BlueBird_1";
			}
            if (CheckAttribute(pchar, "questTemp.State.SeekBible"))// ����� �2, ����� ���������
            {
                if (!CheckCharacterItem(pchar, "Bible"))
                {
                    dialog.text = "��������...";
        			link.l2 = "����������, � ���� ������, ���� �� ����� ����� ���������� ����� �������, ����������� �� �������������?";
        			link.l2.go = "Step_S2_1";
                }
                else
                {
                    dialog.text = "�-�-�, ��� ������ ����. ������, �� ��� � ���� ���� ������� - ��� ����� � ���� �� ��� ����� �������... �� �����, ��� ��� ���?";
        			link.l2 = "����� ���� ����� �� �����, ����� ������ ���������. ��, ������, ��� ��� �������, ��� ������ �����.";
        			link.l2.go = "exit";
                }
            }
			if (pchar.questTemp.piratesLine == "KillLoy_toAlexus")
			{
				link.l1 = "� ��� ������ ��� �������?";
				link.l1.go = "PL_Q3_1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_GoodWork")
			{
				link.l1 = "������� ������� ��� '�������� �����'!";
				link.l1.go = "PL_Q3_SW1";
			}
			if (pchar.questTemp.piratesLine == "KillLoy_LoyIsDied")
			{
				link.l1 = "� ���� �� ����� ������� ������� ������������ ������ �����, ������� ��� ����...";
				link.l1.go = "PL_Q3_SW2";
			}
        // <== �������� �� ������
		break;
//*************************** �������, ����� �3, ������ ��� ***************************
		case "PL_Q3_1":
			dialog.text = "�������... � ��� �� ����� �����?";
			link.l1 = "����... ���� ��������� ������� ������� ���� � ������ �������. � ��� ���, ���� ����� �������!";
			link.l1.go = "PL_Q3_3";
			link.l2 = "� ��� ����, ����� �� ��������, � ��� � ������� �������, ��� �� ������� ����...";
			link.l2.go = "PL_Q3_2";
		break;
		case "PL_Q3_2":
			dialog.text = "���. ����, ������. ��� ������... ��� ������ ������������! ��, ��� ���, ��� � ��������, �����, ��� ����� ������ ����� ������� ���. ������� ���� �������\n����� ����, ���� ���� ��� � ����. ���, ���, ��� � �����������, ����������, ����.";
			link.l1 = "�������! ��� ����!";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_3_KillLoy", "10");
			pchar.questTemp.piratesLine = "KillLoy_toJackman";
		break;
		case "PL_Q3_3":
			dialog.text = "�� ��, �����������, ������ ������! ��, ����, � ���� �������! ������ ������ �� ��������, � �������� ������ '�������� �����' �������� ���� ������...";
			link.l1 = "'�������� �����'?";
			link.l1.go = "PL_Q3_4";
		break;
		case "PL_Q3_4":
			dialog.text = "'������� ����' ��� ����. ��� ����! ����� ������ ��� ��������! ��� ���� � ���� ������. �� ���������, �����������, ����������! �� ������ �� ���� ���������� �������� ������, �� �������, ����� �������� ��������� ������, �� ������� ��������������� ������. � ��� �� ����� �� ������...";
			link.l1 = "��-�-�...  �� �������� ���-�� ��� ���.";
			link.l1.go = "PL_Q3_5";
		break;
		case "PL_Q3_5":
			dialog.text = "��������? �, ��. ������ ���. ����-�� � �������� �����, �������� �����, � ��� ��, ���, � �������. � �����, ����� � ����, � ����� ����� �� �������� � ��������, ���������, ��� ������ ����������. ������� �����������... ������... ����� ������\n����� ���� ������ ��... �-�-�...  ����� � ������ ������, ������� ������: '� ��� ������ ������, �� ���� ���� �������'. ��� � ��� � �������, ��� �� �� ��� ����� ����� ��� �����, ����� � ��� ����, ������ �������?";
			link.l1 = "� �� ������ ��� ������, ������ ��� ��� ������ ����� ��� ������?!";
			link.l1.go = "PL_Q3_6";
		break;
		case "PL_Q3_6":
			dialog.text = "��� �������� ������� �������. ����������, ��� ����� �����, ��� ��� ���� �����. ��������� �����, ���� ������� �����.";
			link.l1 = "��� �, ��������.";
			link.l1.go = "exit";
			AddQuestRecord("Pir_Line_3_KillLoy", "11");
			pchar.questTemp.piratesLine = "KillLoy_toTavernAgain";
			//����� �������� ����� � �������
			sld = GetCharacter(NPC_GenerateCharacter("CapGoodly", "officer_9", "man", "man", 20, PIRATE, -1, true));
			sld.name = "�������";
			sld.lastname = "�����";
			sld.rank = 20;
			sld.city = "Pirates";
			sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
			sld.dialog.currentnode   = "CapGoodly";
			sld.greeting = "pirat_quest";
			LAi_SetSitType(sld);
			LAi_group_MoveCharacter(sld, "PIRATE_CITIZENS");
            FreeSitLocator("Pirates_tavern", "sit2");
         	ChangeCharacterAddressGroup(sld, "Pirates_tavern", "sit", "sit2");
		break;

		case "PL_Q3_SW1":
			dialog.text = "�����, �����, ���� ��������. �� ���������, ����� �� �������� ���������.\n��������� �������. ���������� � ������� ����! ���� �������.";
			link.l1 = "��� �� � ����...";
			link.l1.go = "exit";
		break;
		case "PL_Q3_SW2":
			dialog.text = "����! � ��� � ����... � ���?";
			link.l1 = "�� ������, �������....";
			link.l1.go = "PL_Q3_SW3";
		break;
		case "PL_Q3_SW3":
			dialog.text = "�� ��, ��� �����.";
			link.l1 = "�� �� ��������� ��, ��� ���������...";
			link.l1.go = "exit";
		break;
//*************************** ������ ����� ����� ***************************
		case "BlueBird_1":
			dialog.text = "��� �� ���. ��������, � ������� ���� ������ �� ������� ���� �� �� �����.";
			link.l1 = "�-�, �� �������. ������ ���������� ����������� ��� � ���. �� �������� �� ����� ��� ���������� ������ � �����?";
			link.l1.go = "BlueBird_2";
		break;
		case "BlueBird_2":
			dialog.text = "��-��, ��� �������, ���� ���������� � ��������. �������� ����, ��� ����� ������� ���� ����, ����� �� ���� ������� ����� �� ���� ����, ����� ����� ������ ����� ������� � �������� �������� ��� �� ����.";
			link.l1 = "�� � ���� ���-�� �� �����, ����� �� ���� ������ � ������?";
			link.l1.go = "BlueBird_3";
		break;
		case "BlueBird_3":
			dialog.text = "� �� �������, ��� ���� ���-�� ������ ������� ����� �� ���� ����� � �����, ����� ����� �� �������?";
			link.l1 = "��, ��, �����... ��� ������, ����� ������ �� �������� ����� ���� �����?";
			link.l1.go = "BlueBird_4";
		break;
		case "BlueBird_4":
			dialog.text = "���, ����� ������ �� ��������, ������ ���� ���������.";
			link.l1 = "�� ��� ��, �������� ��� �������!";
			link.l1.go = "exit";
			AddQuestRecord("Xebeca_BlueBird", "5");
			pchar.questTemp.BlueBird = "toCavern";
			pchar.quest.BlueBird_inCavern.win_condition.l1 = "locator";
			pchar.quest.BlueBird_inCavern.win_condition.l1.location = "Bermudes_Cavern";
			pchar.quest.BlueBird_inCavern.win_condition.l1.locator_group = "quest";
			pchar.quest.BlueBird_inCavern.win_condition.l1.locator = "BlueBird";
			pchar.quest.BlueBird_inCavern.function = "BlueBird_inCavern";
			LAi_LocationDisableMonstersGen("Bermudes_Cavern", true); //�������� �� ��������
			LAi_LocationDisableOfficersGen("Bermudes_Cavern", true); //�������� �� �������
		break;	
//*************************** ���������� ����� ***************************
 		case "Step_Door_1":
			dialog.text = "��� �����. ����� ����� ������, ���� ���� ��������� ��� 20000 �����. �� ��������� - �� ������, � ���� ����� �� ��������� ����. �� � ���� ��� ������� �� ������� - �� � �����...";
			if(sti(pchar.money) >= 20000)
			{
    			link.l1 = "��� ���� ������, ��������.";
    			link.l1.go = "Step_Door_2";
            }
            else
            {
    			link.l1 = "��� � ���� ����� �����.";
    			link.l1.go = "Step_Door_3";
            }
            link.l2 = "��� ���! ������� �� �����-�� �����.";
    		link.l2.go = "exit";
		break;
 		case "Step_Door_2":
			npchar.Step_Door = true; // fix
			dialog.text = "�������, ����� �������. � �� ���������!";
			link.l1 = "�� ���� �� ���, ������� ����, " + npchar.name + "...";
			link.l1.go = "exit";
			LocatorReloadEnterDisable("Pirates_Shipyard", "reload2", false);
            AddMoneyToCharacter(pchar, -20000);
			pchar.quest.CloseBermudesDungeonDoor.win_condition.l1 = "MapEnter";
            pchar.quest.CloseBermudesDungeonDoor.win_condition = "CloseBermudesDungeonDoor";
		break;
 		case "Step_Door_3":
			dialog.text = "�� ��� ����� �����, ����� � �������. � ��� � �� ���������� - ����������...";
			link.l1 = "����, �� ���� ������ ������� ���!";
			link.l1.go = "exit";
		break;
//*************************** ����� �2, ������� � ��������� �� ������ �� ��������� ***************************
 		case "Step_S2_1":
			dialog.text = NPCStringReactionRepeat("�-�-�, ������� ������... � ����� �� ��� �����?", "� ��� ��� ������ - ����� �������.", "��� ��� ��������� - ����� �������.", "��������, �� ������� �����, �?", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("� ���� ���� �����, ������� ��� �� �����������. �� ���.", "� �����...", "�������...", "��� ����� � ��������� �����������...", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("Step_S2_2", "none", "none", "none", npchar, Dialog.CurrentNode);
		break;
 		case "Step_S2_2":
			dialog.text = "�������. �� ������, ��� ��������������, �������, ��� ���������� ���� �� ����. ����� �������� �����-�� �������� � ���� ������� ���������. � �� ������ �� ������.";
			link.l1 = "������?";
			link.l1.go = "Step_S2_3";
		break;
 		case "Step_S2_3":
			dialog.text = "�� ������� ����� �� ���� ����� - �� ���� ��, ���� �� ��� ���� �������... ���� ���� �������, ��, �����, �� �� ���������, �� � ������...";
			link.l1 = "���� �� ��� �������?";
			link.l1.go = "Step_S2_4";
		break;
 		case "Step_S2_4":
			dialog.text = "����, ���� ����� ��� �������. � ��� �����, ��� ������ �� ����.";
			link.l1 = "� ���, �� ������? � ������ �� ������ ���� �� ��������?";
			link.l1.go = "Step_S2_5";
		break;
 		case "Step_S2_5":
			dialog.text = "�� ������ ����������, ��� ��� �������� - ��� ��������, ����� � ���� ����� ��������, ��� ��� ����. � �� ������ ������, ��� ������ ���, ����� ������. ������� ���, ������, ��� � ������...\n"+
                          "����� ��� �� ���� ���, ��� ���. �� ����� ����� ������ ����� ���������, ��� ����� � ����� ������...";
			link.l1 = "��� ��� ����... ��, �� ��� ��, ��������.";
			link.l1.go = "Step_S2_6";
		break;
 		case "Step_S2_6":
			dialog.text = "�������, �������... ������, ��� ����� ���� ��� �� ���� �����.";
			link.l1 = "�������, �� ����������.";
			link.l1.go = "exit";
            LocatorReloadEnterDisable("Pirates_Shipyard", "reload2", false);
            LocatorReloadEnterDisable("Shore3", "reload2_back", false);
            Pchar.quest.Inquisition_fightInDangeon.win_condition.l1 = "location";
            Pchar.quest.Inquisition_fightInDangeon.win_condition.l1.location = "Bermudes_Dungeon";
            Pchar.quest.Inquisition_fightInDangeon.win_condition = "Inquisition_fightInDangeon";
         	pchar.GenQuestBox.Terks_Grot = true;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.jewelry5 = 200;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.blade33 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.blade28 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.blade34 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.blade32 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.arm1 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.pistol5 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.pistol4 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.pistol6 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.potion2 = 5;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian18 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.jewelry7 = 5;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.lead1 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.potion2 = 10;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.potionwine = 3;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.jewelry2 = 20;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.jewelry14= 10;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian6 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian11 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian15 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian12 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.indian18 = 1;
            pchar.GenQuestBox.Bermudes_Dungeon.box1.items.Bible = 1;
		break;

	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

void checkMatherial(ref Pchar, ref NPChar, int good1, int good2)
{
    int amount;
    amount = makeint(Pchar.money) - sti(NPChar.Tuning.Money);
    if (amount < 0)
    {
       amount = amount + sti(NPChar.Tuning.Money);
    }
    else
    {
        amount = sti(NPChar.Tuning.Money);
    }
    AddMoneyToCharacter(Pchar, -amount);
    NPChar.Tuning.Money = makeint(sti(NPChar.Tuning.Money) - amount);

    amount = GetSquadronGoods(Pchar, good1) - sti(NPChar.Tuning.Matherial1);
    if (amount < 0)
    {
       amount = amount + sti(NPChar.Tuning.Matherial1);
    }
    else
    {
        amount = sti(NPChar.Tuning.Matherial1);
    }
    RemoveCharacterGoods(Pchar, good1, amount);
    NPChar.Tuning.Matherial1 = sti(NPChar.Tuning.Matherial1) - amount;

    amount = GetSquadronGoods(Pchar, good2) - sti(NPChar.Tuning.Matherial2);
    if (amount < 0)
    {
       amount = amount + sti(NPChar.Tuning.Matherial2);
    }
    else
    {
        amount = sti(NPChar.Tuning.Matherial2);
    }
    RemoveCharacterGoods(Pchar, good2, amount);
    NPChar.Tuning.Matherial2 = sti(NPChar.Tuning.Matherial2) - amount;
}