// BOAL ������� �� F12 �������� ���� ���. ��� ����� ������� � alt-tab � ����, ��������� ����� �����
//native string GetHello();
//#libriary "b_engine"

void ActiveINSERTControl()
{
    //DoReloadCharacterToLocation(Pchar.location, "goto", "goto2");
    //DoReloadCharacterToLocation("PlutoStoreSmall", "reload", "reload1");
	DoReloadCharacterToLocation("UnderWater", "reload", "reload1");
}

void ActiveF4Control()
{
	Log_Info("Test F4 button");
    SetRandGeraldSail(pchar, rand(4));
}

void ActiveF5Control()
{
	Log_Info("Test F5 button");
}

void ActiveF7Control()
{
	ref sld, npchar;
	int   iTemp, i, ShipType, Rank; // ����� ��� ���������� ����� ����� (�����)
    float locx, locy, locz, fTemp;
    string  attrName, Model, Blade, Gun, sTemp; // ����� ������ ��� ����������

	/*int iTimer = GetPastTime("day", sti(pchar.questTemp.control_year), sti(pchar.questTemp.control_month), sti(pchar.questTemp.control_day), 0,GetDataYear(), GetDataMonth(), GetDataDay(), 0);
    Log_SetStringToLog("��� ������" + pchar.questTemp.control_year + "  ����� ������" + pchar.questTemp.control_month + "  ���� ������" + pchar.questTemp.control_day);
    Log_SetStringToLog("������ ����: " + iTimer);
    Log_SetStringToLog("��������� ������: " + pchar.questTemp.State);
    Log_SetStringToLog("������� �������� ������: " + pchar.questTemp.CurQuestNumber);
    Log_SetStringToLog("���� ���� �� ��������� ���������� ������: " + pchar.questTemp.Waiting_time);    
	Log_SetStringToLog("pchar.questTemp.NationQuest: " + pchar.questTemp.NationQuest);
	Log_SetStringToLog("��������� ������ ��������: " + Pchar.RomanticQuest);
	Log_SetStringToLog("CheckBudgetMoney: " + IsabellaCheckBudgetMoney());*/
    /*Log_SetStringToLog("Pchar.location: " + Pchar.location);
	Log_SetStringToLog("Pchar.locator: " + Pchar.location.locator);
	Log_SetStringToLog("Pchar.location.from_sea: " + Pchar.location.from_sea);	
	Log_SetStringToLog("����� ��: " + pchar.nation);
	Log_SetStringToLog("������� ����� ��: " + GetBaseHeroNation());*/
    /*Log_SetStringToLog("��������� ������ ��������: " + pchar.questTemp.Ascold);
	Log_SetStringToLog("��������� ������ ����: " + pchar.questTemp.Azzy);
    Log_SetStringToLog("��������� ������ ������: " + pchar.questTemp.Ascold.TraderId);
	Log_SetStringToLog("��������� �������(�������): " + pchar.questTemp.Ascold.MerchantColony);
	Log_SetStringToLog("��������� �������: " + pchar.questTemp.Ascold.ShipyarderId);
	Log_SetStringToLog("�������� �������: " + Items[GetItemIndex("Rock_letter")].startLocation);
	Log_SetStringToLog("�������� �������: " + Items[GetItemIndex("Rock_letter")].startLocator);*/

    //SetNationRelation2MainCharacter(SPAIN, RELATION_ENEMY);

            /*sld = GetCharacter(NPC_GenerateCharacter("SoleiRoyalCaptain", "off_fra_2", "man", "man", 35, FRANCE, -1, true));
            FantomMakeCoolSailor(sld, SHIP_MANOWAR, "����� ������", CANNON_TYPE_CANNON_LBS42, 95, 80, 90);
            FantomMakeCoolFighter(sld, 35, 80, 70, BLADE_LONG, "pistol3", 100);*/
	
			//GiveNationLicence(ENGLAND, 30);   
			//WaitDate("", 0, 0, 43, 1, 0);
	
			//Log_SetEternalString("�����: " +  pchar.chr_ai.type); 

			/*sld = &locations[FindLocation(pchar.location)];
			dumpattributes(sld);*/
			/*sld = &Islands[FindIsland("Martinique")];
			dumpattributes(sld);*/

	/*sld = GetCharacter(NPC_GenerateCharacter("SoleiRoyalCaptain1", "officer_8", "man", "man", 35, ENGLAND, -1, false));
	ChangeCharacterAddressGroup(sld, pchar.location, "goto", "goto1");
	LAi_SetStayType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");*/

	/*sld = GetCharacter(NPC_GenerateCharacter("SoleiRoyalCaptain2", "pirate_1N", "man", "man", 35, ENGLAND, -1, false));
	ChangeCharacterAddressGroup(sld, pchar.location, "sit", "sit1");
	LAi_SetHuberType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");*/

	//FreeSitLocator("PortRoyal_tavern", "sit1");

	/*SetCharacterPerk(pchar, "FlagSpa");
	SetCharacterPerk(pchar, "FlagEng");
	SetCharacterPerk(pchar, "FlagHol");
	SetCharacterPerk(pchar, "FlagFra");*/

		//CreateParticleSystem("shipfire", -29.7, -0.12, -0.77, 0, 0, 0, 0);
		//DeleteParticles();
		//PauseParticles(false);
		//DeleteParticleSystem(0);

	//bDisableLandEncounters = true;	

	/*aref	aCurWeather = GetCurrentWeather();
	Log_SetStringToLog("������: " + aCurWeather.id);*/
	//bDisableFastReload = false; //������� �������


}

void ActiveF10Control()
{
	Log_Info("Test F10 button");
    LaunchPsHeroScreen();
    trace("===================RUMOURS=======================");
    for(int i = 0; i < MAX_RUMOURS; i++)
	{
        DumpAttributes(&Rumour[i]);
    	trace("===================");
	}
}

///  ����� ��������� � �����
void ActiveF12Control()
{
    ref mainCh = GetMainCharacter();
    
    //����� ���������� �������� ��������� � �������� �������
    int res = LAi_FindNearestVisCharacter(mainCh, 15);
    if (res != -1)
    {
        ref findCh = GetCharacter(res);
        res = findCh.chr_ai.hp;
        Log_SetStringToLog("" + GetFullName(findCh) +
                           " "+XI_ConvertString("Rank")+" " + findCh.rank + " "+XI_ConvertString("Health")+" "+res + "/" + LAi_GetCharacterMaxHP(findCh));

		if (bBettaTestMode)
        {
            Log_SetStringToLog("Id= " + findCh.id);
            Log_SetStringToLog("Idx= " + findCh.index);
            if (CheckAttribute(findCh, "equip.blade"))
            {
                Log_SetStringToLog("Blade= " + findCh.equip.blade);
            }
            Log_SetStringToLog("model= " + findCh.model);
            Log_SetStringToLog("face= " + findCh.faceId);
            Log_SetStringToLog("chr_ai.type= " + findCh.chr_ai.type);
            Log_SetStringToLog("Group= "+findCh.location.group + " locator= " + findCh.location.locator);
            if (CheckAttribute(findCh, "cirassId"))
            {
                Log_SetStringToLog("cirassId= " + sti(findCh.cirassId));
            }
            else
            {
                Log_SetStringToLog("��� �����");
            }
            if (CheckAttribute(findCh, "chr_ai.FencingType"))
            {
                Log_SetStringToLog("chr_ai.FencingType= " + findCh.chr_ai.FencingType));
            }
            else
            {
                Log_SetStringToLog("��� FencingType - error");
            }
            /*int dchr_index;
            ref deadCh;
            dchr_index = Dead_FindCloseBody();
            if (dchr_index != -1)
            {
                deadCh = &Dead_Characters[dchr_index];
                Log_SetStringToLog("Dead=" + deadCh.id);
            } */
			Log_SetStringToLog("�����: " + findCh.nation);
			Log_SetStringToLog("���: " + findCh.sex);
			Log_SetStringToLog("������: " + findCh.chr_ai.group);
			Log_SetStringToLog("��������: " + findCh.chr_ai.tmpl);
			Log_SetStringToLog("�����: " + findCh.chr_ai.tmpl.state);

            mainCh.SystemInfo.OnlyShowCharacter = true;
			LaunchCharacter(findCh);
        }
        else
        {
            if (MOD_BETTATESTMODE == "Test")
	        {
	            Log_SetStringToLog("Id= " + findCh.id);
	        }
        }
    }
}

///////////////////////////// test //////////////
void TestColonyCommanders()
{
	int iChar;

	string sColony;

	int i;
	float x, y, z;  // boal

	for (i=0; i<MAX_COLONIES; i++)
	{
		if (colonies[i].nation == "none") continue; // ����������
		// ������� ������� ����� �������
		iChar = GetCharacterIndex(colonies[i].id + "_Mayor");
		if (iChar != -1)
		{   // ��� ����
        	Log_Info("M: " + characters[iChar].id + " L:" + characters[iChar].location +
        	" " + characters[iChar].City + "  " + characters[iChar].nation);
        	trace("M: " + characters[iChar].id + " L:" + characters[iChar].location +
        	" " + characters[iChar].City + "  " + characters[iChar].nation);
        }

		// �������� �������� �� �������, � ��� ��� ������, ����� �� ���������?
		if (CheckAttribute(&colonies[i], "HasNoFort"))
		{
			continue;
		}
		iChar = GetCharacterIndex(colonies[i].id + " Fort Commander");
		Log_Info("F: " + characters[iChar].id + " L:" + characters[iChar].location + " g " + characters[iChar].location.group + " r " + characters[iChar].location.locator +
  		" " + characters[iChar].City + "  " + characters[iChar].nation);
  		trace("F: " + characters[iChar].id + " L:" + characters[iChar].location + " g " + characters[iChar].location.group + " r " + characters[iChar].location.locator +
  		" " + characters[iChar].City + "  " + characters[iChar].nation);
	}
}
// �������� ����������� ������ - ����-���, ��� �������� - ������� � ������.��� ������, ������ �������� ���� ������� �� ����� �����
void TestGoodsLiquidity()
{
	int i, j, idx;
	float fMaxCost;
	
	for (i = 0; i< GOODS_QUANTITY; i++)
	{
		SetCharacterGoods(pchar, i, sti(Goods[i].Units));
	}
	trace("======= TestGoodsLiquidity ======== start ");
	for (j = 0; j< GOODS_QUANTITY; j++)
	{
		fMaxCost = 0;
		idx = -1; 
		for (i = 0; i< GOODS_QUANTITY; i++)
		{
			if (GetCargoGoods(pchar, i) > 0)
			{
				if (fMaxCost < stf(Goods[i].Cost)/stf(Goods[i].Weight)) 
				{
					fMaxCost = stf(Goods[i].Cost)/stf(Goods[i].Weight);
					idx = i;
				} 
			}
		}
		if (fMaxCost > 0)
		{
			SetCharacterGoods(pchar, idx, 0);	
			trace(Goods[idx].Name + " = " + fMaxCost);
			Log_info(Goods[idx].Name + " = " + fMaxCost);
		}
	}
	trace("======= TestGoodsLiquidity ======== end ");
}
