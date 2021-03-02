
ref		sld, rCharacter;
int     iTemp, i; // ����� ��� ���������� ����� ����� (�����)
float   locx, locy, locz;
string  sTemp; // ����� ������ ��� ����������
bool    bOk;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������ � �����������   ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//==> ���������
void Birglars_fight(string qName)
{
	if (GetCharacterIndex("Berglar" + pchar.quest.(qName).city) != -1)
	{
		LAi_SetFightMode(pchar, false);
		LAi_LockFightMode(pchar, true);		
		sTemp = "berglar" + pchar.quest.(qName).city;
		pchar.questTemp.tugs.(sTemp) = "over";
		sld = characterFromId("Berglar" + pchar.quest.(qName).city);
		LAi_SetActorType(sld);
		LAi_ActorDialog(sld, pchar, "", 2.0, 0);
	}
}
//���������� ���� �� ���� �����������
void SmallQuests_free(string qName)
{
	pchar.questTemp.different = "free";
}
//������� ��� ������������ ������� � sit ����� ������
void MayorSitBack(string qName) 
{
	iTemp = GetCharacterIndex(Pchar.quest.MayorSitBack.mayorId)
	if (iTemp > 0)
	{
		sld = &characters[iTemp];
		sld.location = Pchar.quest.MayorSitBack.locationName;
		sld.location.group = "sit";
		sld.location.locator = Pchar.quest.MayorSitBack.locatorName;
		LAi_SetHuberTypeNoGroup(sld);
		RemoveCharacterEquip(sld, BLADE_ITEM_TYPE);
		RemoveCharacterEquip(sld, GUN_ITEM_TYPE);
	}
}

void Ascold_LeifIsDead(string qName)
{
	LocatorReloadEnterDisable("Guadeloupe_Cave", "reload3_back", false);
}
void DelivLettTortuga_Digress(string qName)
{
	LAi_ActorAnimation(characterFromId("AntonioDeBarras"), "attack_fast_1", "DelivLettTortuga_2KickPhar", 0.8);
}

void SeekIsabella_exitTown(string qName)
{
    DeleteAttribute(&locations[FindLocation("Beliz_ExitTown")], "DisableEncounters"); //��������� ������
	DoQuestCheckDelay("TalkSelf_Quest", 0.1); //������ ���-��-���
}

void Fr2Letter_shore22(string qName)
{
	if (characters[GetCharacterIndex("Bandit_1")].location == "Shore22")
	{
		for (i=1; i<=2; i++)
		{
			sld = characterFromId("Bandit_"+i);
			LAi_SetActorType(sld);
			LAi_ActorTurnToCharacter(sld, pchar);
		}
		LAi_SetActorType(pchar);
		SetActorDialogAny2Pchar(sld.id, "", 0.0, 0.0);
		LAi_ActorFollow(pchar, sld, "ActorDialog_Any2Pchar", 0.0);
	}
}

void Isabella_widding(string qName)
{
	pchar.quest.Romantic_Widding_Cancel.over = "yes"; //������� ������ �� ������� �� �������
	pchar.RomanticQuest = "Widding";
	sld = characterFromID("Isabella");
	LAi_SetActorType(pchar);
    SetActorDialogAny2Pchar(sld.id, "", 0.0, 0.0);
	LAi_ActorFollow(pchar, sld, "ActorDialog_Any2Pchar", 0.0);	
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������ � �����������   �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������ ����� �����   ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void BlueBird_inCavern(string qName)
{
	pchar.questTemp.BlueBird = "seenCarriers";
	DoQuestCheckDelay("TalkSelf_Quest", 0.2); //������ ���-��-���
}

void BlueBird_endCaveDialog()
{
	LAi_SetActorType(pchar);
	LAi_ActorTurnByLocator(pchar, "goto", "goto1");
	string sQuest = "";
	string CarrierName[10];
	CarrierName[0] = "GenresBag1";
	CarrierName[1] = "GenresBag2";
	CarrierName[2] = "GenresBarrel1";
	CarrierName[3] = "GenresBarrel2";
	CarrierName[4] = "GenresBarrelTop1";
	CarrierName[5] = "GenresBarrelTop2";
	CarrierName[6] = "GenresBottle1";
	CarrierName[7] = "GenresBottle2";
	CarrierName[8] = "GenresChest1";
	CarrierName[9] = "GenresChest2";
	for (i=0; i<10; i++)
	{
		sld = GetCharacter(NPC_GenerateCharacter("Carrier_" + i, CarrierName[i], "man", "genres", 35, PIRATE, 0, true));
		sld.gotoGroup = "reload";
		sld.gotoLocator = "reload1_back";
		LAi_SetCarrierType(sld);
		ChangeCharacterAddressGroup(sld, "Bermudes_Cavern", "reload", "reload2");
	}
	DoQuestFunctionDelay("BlueBird_endCaveScript", 33.0);
}

void BlueBird_endCaveScript(string qName)
{
	LAi_LocationDisableMonstersGen("Bermudes_Cavern", false); //�������� ��������
	LAi_LocationDisableOfficersGen("Bermudes_Cavern", false); //�������� �������
	for (i=0; i<10; i++)
	{
		sld = characterFromId("Carrier_" + i);
		ChangeCharacterAddress(sld, "none", "");
	}
	AddQuestRecord("Xebeca_BlueBird", "6");
	LAi_SetPlayerType(pchar);
	pchar.questTemp.BlueBird = "toSeaBattle";
	pchar.quest.BlueBird_seaBattle.win_condition.l1 = "location";
	pchar.quest.BlueBird_seaBattle.win_condition.l1.location = "Bermudes";
	pchar.quest.BlueBird_seaBattle.function = "BlueBird_seaBattle";	
}

void BlueBird_seaBattle(string qName)
{
	LAi_group_Delete("EnemyFight");
	group_DeleteGroup("BlueBird_Group");
	sld = GetCharacter(NPC_GenerateCharacter("BlueBirdCapitain", "", "man", "man", 20, PIRATE, 30, true));							
	SetCaptanModelByEncType(sld, "pirate");
	sld.dialog.filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "BlueBirdCapitain";
	sld.greeting = "CapSinkShip";
	FantomMakeCoolFighter(sld, 20, 80, 70, "blade32", "pistol3", 20);
	FantomMakeCoolSailor(sld, SHIP_XebekVML, "����� �����", CANNON_TYPE_CULVERINE_LBS24, 70, 70, 70);
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "SaveItemsForDead");
	DeleteAttribute(sld, "DontClearDead");
	DeleteAttribute(sld, "AboardToFinalDeck");
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "DontRansackCaptain");
	sld.AlwaysSandbankManeuver = true;
	sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
	sld.DontRansackCaptain = true;
	SetCharacterPerk(sld, "FastReload");
	SetCharacterPerk(sld, "HullDamageUp");
	SetCharacterPerk(sld, "SailsDamageUp");
	SetCharacterPerk(sld, "CrewDamageUp");
	SetCharacterPerk(sld, "CriticalShoot");
	SetCharacterPerk(sld, "LongRangeShoot");
	SetCharacterPerk(sld, "CannonProfessional");
	SetCharacterPerk(sld, "ShipDefenseProfessional");
	SetCharacterPerk(sld, "SwordplayProfessional");
	SetCharacterPerk(sld, "AdvancedDefense");
	SetCharacterPerk(sld, "CriticalHit");
	SetCharacterPerk(sld, "Sliding");
	LAi_group_MoveCharacter(sld, "EnemyFight");

	Group_AddCharacter("BlueBird_Group", "BlueBirdCapitain");			
	Group_SetType("BlueBird_Group", "pirate");
	Group_SetGroupCommander("BlueBird_Group", "BlueBirdCapitain");
	sTemp = "quest_ship_10";
	if (pchar.location.from_sea == "Shore_ship1") sTemp = "quest_ship_8";
	Group_SetAddress("BlueBird_Group", "Bermudes", "quest_ships", sTemp);
	Group_SetTaskAttack("BlueBird_Group", PLAYER_GROUP);

	pchar.quest.BlueBird_over.win_condition.l1 = "NPC_Death";
	pchar.quest.BlueBird_over.win_condition.l1.character = "BlueBirdCapitain";
	pchar.quest.BlueBird_over.function = "BlueBird_over";

	pchar.quest.BlueBird_DieHard1.win_condition.l1 = "ExitFromLocation";
	pchar.quest.BlueBird_DieHard1.win_condition.l1.location = "Bermudes";
	pchar.quest.BlueBird_DieHard1.function = "BlueBird_DieHard";
	Pchar.quest.BlueBird_DieHard2.win_condition.l1 = "MapEnter";
	pchar.quest.BlueBird_DieHard2.function = "BlueBird_DieHard";
}

void BlueBird_over(string qName)
{
	pchar.quest.BlueBird_DieHard1.over = "yes";
	pchar.quest.BlueBird_DieHard2.over = "yes";
	AddQuestRecord("Xebeca_BlueBird", "7");
	pchar.questTemp.BlueBird = "weWon";
}

void BlueBird_DieHard(string qName)
{
	sld = characterFromId("BlueBirdCapitain");
	sld.lifeDay = 0;
	group_DeleteGroup("BlueBird_Group");
	pchar.quest.BlueBird_over.over = "yes";
	pchar.quest.BlueBird_DieHard1.over = "yes";
	pchar.quest.BlueBird_DieHard2.over = "yes";
	AddQuestRecord("Xebeca_BlueBird", "8");
	pchar.questTemp.BlueBird = "DieHard";
}

//��������� ��������. ��������� �� ������ ����� ��������
void BlueBird_loginFleut(string qName)
{
	if (GetQuestPastMinutesParam("questTemp.BlueBird") < 10) 
	{
		LAi_group_Delete("EnemyFight");
		group_DeleteGroup("BlueBird_Group");
		sld = GetCharacter(NPC_GenerateCharacter("BlueBirdTrader", "", "man", "man", 20, sti(pchar.questTemp.BlueBird.nation), 30, true));							
		sld.dialog.filename = "Quest\ForAll_dialog.c";
		sld.dialog.currentnode = "BlueBirdTrader";
		sld.greeting = "CapSinkShip";		
		SetCaptanModelByEncType(sld, "trade");
		FantomMakeCoolSailor(sld, SHIP_FLEUT, pchar.questTemp.BlueBird.Ship.Name, CANNON_TYPE_CULVERINE_LBS24, 70, 70, 70);
		sld.Ship.Mode = "trade"; //��������
		sld.DontRansackCaptain = true;
		LAi_group_MoveCharacter(sld, "EnemyFight");
		Group_AddCharacter("BlueBird_Group", "BlueBirdTrader");			
		Group_SetGroupCommander("BlueBird_Group", "BlueBirdTrader");
		Group_SetAddress("BlueBird_Group", pchar.questTemp.BlueBird.Island, "quest_ships", "quest_ship_"+(rand(5)+1));
		Group_SetTaskRunaway("BlueBird_Group", PLAYER_GROUP);
		pchar.questTemp.BlueBird = "attackFleut"; //���� ������
		//���������� �� ���� �� �������
		pchar.Quest.BlueBirdFleut_over1.win_condition.l1 = "Location_Type";
		pchar.Quest.BlueBirdFleut_over1.win_condition.l1.location_type = "town";
		pchar.Quest.BlueBirdFleut_over1.function = "BlueBirdFleut_over";
		pchar.Quest.BlueBirdFleut_over2.win_condition.l1 = "Location_Type";
		pchar.Quest.BlueBirdFleut_over2.win_condition.l1.location_type = "seashore";
		pchar.Quest.BlueBirdFleut_over2.function = "BlueBirdFleut_over";
		Pchar.quest.BlueBirdFleut_over3.win_condition.l1 = "MapEnter";
		pchar.Quest.BlueBirdFleut_over3.function = "BlueBirdFleut_over";
		//���������� �� �������
    	pchar.quest.BlueBirdFleut_board.win_condition.l1 = "NPC_death";
    	pchar.quest.BlueBirdFleut_board.win_condition.l1.character = "BlueBirdTrader";
    	pchar.quest.BlueBirdFleut_board.function = "BlueBirdFleut_board";
	}
	else
	{
		AddQuestRecord("Xebeca_BlueBird", "11");
		AddQuestUserData("Xebeca_BlueBird", "sIsland", XI_ConvertString("Colony" + pchar.questTemp.BlueBird.Island + "Gen"));
	}
}

void BlueBirdFleut_over(string qName)
{
	LAi_group_Delete("EnemyFight");
	group_DeleteGroup("BlueBird_Group");
	pchar.Quest.BlueBirdFleut_over1.over = "yes";
	pchar.Quest.BlueBirdFleut_over2.over = "yes";
	pchar.Quest.BlueBirdFleut_over3.over = "yes";
	pchar.Quest.BlueBirdFleut_board.over = "yes";
	AddQuestRecord("Xebeca_BlueBird", "12");
	AddQuestUserData("Xebeca_BlueBird", "sIsland", XI_ConvertString(pchar.questTemp.BlueBird.Island + "Gen"));
	pchar.questTemp.BlueBird = "returnMoney"; //����� ����� �� ����� �� ���������
}
//����� �����������, ������� ���������
void BlueBirdFleut_board(string qName)
{
	pchar.Quest.BlueBirdFleut_over1.over = "yes";
	pchar.Quest.BlueBirdFleut_over2.over = "yes";
	pchar.Quest.BlueBirdFleut_over3.over = "yes";
	pchar.questTemp.BlueBird.count = sti(pchar.questTemp.BlueBird.count) + 1; //������� ����������� �������
	AddQuestRecord("Xebeca_BlueBird", "13");
	AddQuestUserData("Xebeca_BlueBird", "sIsland", XI_ConvertString(pchar.questTemp.BlueBird.Island + "Gen"));
	pchar.questTemp.BlueBird = "returnMoney"; //����� ����� �� ����� �� ���������
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������ ����� �����    �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ��������� ���������� �������� Sharp    ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sharp_loginNearIsland(string qName)
{	
	if (GetQuestPastMinutesParam("questTemp.Sharp") < 10) 
	{
		sld = &characters[GetCharacterIndex("Sharp")];
		LAi_SetCurHPMax(sld);
		sld.nation = pchar.nation; //����� ����� ������ ��� � ��
		SetCrewQuantityFull(sld);
		DeleteAttribute(sld, "ship.sails");// ������ ���� �� �������
		DeleteAttribute(sld, "ship.blots");
		DeleteAttribute(sld, "ship.masts");// ������� ������ �����
		SetBaseShipData(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
		Group_AddCharacter("Sharp_Group", "Sharp");			
		Group_SetGroupCommander("Sharp_Group", "Sharp");
		Group_SetAddress("Sharp_Group", pchar.questTemp.Sharp.Island, "quest_ships", "quest_ship_"+(rand(1)+1));
		Group_SetTaskRunaway("Sharp_Group", PLAYER_GROUP);
		pchar.Quest.Sharp_landOver1.win_condition.l1 = "Location_Type";
		pchar.Quest.Sharp_landOver1.win_condition.l1.location_type = "town";
		pchar.Quest.Sharp_landOver1.function = "Sharp_Over";
		pchar.Quest.Sharp_landOver2.win_condition.l1 = "Location_Type";
		pchar.Quest.Sharp_landOver2.win_condition.l1.location_type = "seashore";
		pchar.quest.Sharp_landOver2.function = "Sharp_Over";
	    Pchar.quest.Sharp_mapOver.win_condition.l1 = "MapEnter";
		pchar.quest.Sharp_mapOver.function = "Sharp_Over";
	}
	else
	{
		AddQuestRecord("SharpPearl", "12");
		CloseQuestHeader("SharpPearl");
		pchar.questTemp.Sharp = "seekSharp"; //����� �������� � ��������
		pchar.questTemp.Sharp.brothelChance = rand(10);
	}
}

void Sharp_Over(string qName)
{
	group_DeleteGroup("Sharp_Group");
	pchar.quest.Sharp_landOver1.over = "yes";
	pchar.quest.Sharp_landOver2.over = "yes";
	pchar.quest.Sharp_mapOver.over = "yes";
	//���� ������ � ��, ���� �� ����� �����
	if (pchar.questTemp.Sharp == "toSharp_going")
	{
		AddQuestRecord("SharpPearl", "4");
		CloseQuestHeader("SharpPearl");
		pchar.questTemp.Sharp = "seekSharp"; //����� �������� � ��������
		pchar.questTemp.Sharp.brothelChance = rand(10);
	}
}

//------------ ��������� �������� ----------------
void SeaPearl_login(string qName)
{
	characters[GetCharacterIndex("Sharp")].DeckDialogNode = "NewDeckSharp"; //����� ���� �� ������ �����
	pchar.quest.SeaPearl_Late.over = "yes";
	LAi_group_Delete("EnemyFight");
	group_DeleteGroup("PearlGroup");
	iTemp = sti(pchar.questTemp.Sharp.PearlAreal.qtyShips);
	for (i = 1; i <= iTemp; i++)
	{                
		sTemp = "PearlCapitain_" + i;
		sld = GetCharacter(NPC_GenerateCharacter(sTemp, "trader_"+(rand(15)+1), "man", "man", 10, PIRATE, 30, true));	
		sld.Ship.Type = GenerateShip(SHIP_TARTANE, false);
		SetRandomNameToShip(sld);
		SetBaseShipData(sld);
		SetCrewQuantityFull(sld);
		Fantom_SetBalls(sld, "pirate");
		sld.ShipTaskLock = true;
		sld.Abordage.Enable = false;
		sld.SinkTenPercent = true; //�������� � �� �����
		sld.PearlTartane = true; //������� ������ �������, ����� �������
		LAi_group_MoveCharacter(sld, "EnemyFight");
		Group_AddCharacter("PearlGroup", sTemp);
	}
    // ==> ������������
	Group_SetGroupCommander("PearlGroup", "PearlCapitain_1");			
	Group_SetAddress("PearlGroup", pchar.questTemp.Sharp.PearlAreal.Island, "quest_ships", pchar.questTemp.Sharp.PearlAreal.locator);
	Group_SetTaskRunaway("PearlGroup", PLAYER_GROUP);
	Group_LockTask("PearlGroup");
	pchar.quest.SeaPearl_DieHard1.win_condition.l1 = "ExitFromLocation";
	pchar.quest.SeaPearl_DieHard1.win_condition.l1.location = pchar.questTemp.Sharp.PearlAreal.Island;
	pchar.quest.SeaPearl_DieHard1.function = "SeaPearl_DieHard";
	Pchar.quest.SeaPearl_DieHard2.win_condition.l1 = "MapEnter";
	pchar.quest.SeaPearl_DieHard2.function = "SeaPearl_DieHard";
}

void SeaPearl_Late(string qName)
{	
	pchar.quest.SeaPearl_login.over = "yes";
	AddQuestRecord("SharpPearl", "9");
	AddQuestUserData("SharpPearl", "iDay", FindRussianDaysString(sti(pchar.questTemp.Sharp.PearlAreal.terms)));
	AddQuestUserData("SharpPearl", "sTarget", GetConvertStr(pchar.questTemp.Sharp.PearlAreal.Shore, "LocLables.txt"));
	pchar.questTemp.Sharp = "seekSharp"; //����� �������� � ��������
	pchar.questTemp.Sharp.brothelChance = rand(10);
	DeleteAttribute(pchar, "questTemp.Sharp.PearlAreal");
}

void SeaPearl_DieHard(string qName)
{
	group_DeleteGroup("PearlGroup");
	pchar.quest.SeaPearl_DieHard1.over = "yes";
	pchar.quest.SeaPearl_DieHard2.over = "yes";
	if (pchar.questTemp.Sharp == "SeaPearl_success")
	{	//�������� ���������
		AddQuestRecord("SharpPearl", "7");
		AddQuestUserData("SharpPearl", "iSmall", pchar.questTemp.Sharp.PearlAreal.SmallPearlQty);
		AddQuestUserData("SharpPearl", "iBig", pchar.questTemp.Sharp.PearlAreal.BigPearlQty);
	}
	else
	{	//��������� ���������
		AddQuestRecord("SharpPearl", "8");
	}
	pchar.questTemp.Sharp = "seekSharp"; //����� �������� � ��������
	pchar.questTemp.Sharp.brothelChance = rand(10);
	DeleteAttribute(pchar, "questTemp.Sharp.PearlAreal");
	CloseQuestHeader("SharpPearl");
}

void SharpSeekSpy_loginSpy(string qName)
{
	if (rand(1) && !IsDay())
	{
		LAi_group_Delete("EnemyFight");
		AddQuestRecord("SharpPearl_SeekSpy", "3");
		sld = GetCharacter(NPC_GenerateCharacter("Emillio", "pirate_5", "man", "man", 20, SPAIN, 0, true));
		sld.name = "������";
		sld.lastname = "������";
		sld.Dialog.Filename = "Quest\ForAll_dialog.c";
		sld.dialog.currentnode = "SharpSeekSpy_Emi";
		LAi_SetActorType(sld);
		LAi_group_MoveCharacter(sld, "EnemyFight");
		ChangeCharacterAddressGroup(sld, pchar.location, "goto", "goto5");
		rCharacter = GetCharacter(NPC_GenerateCharacter("QuestMan_5", "pirate_6", "man", "man", 20, SPAIN, 0, true));
		rCharacter.Dialog.Filename = "Quest\ForAll_dialog.c";
		rCharacter.dialog.currentnode = "SharpSeekSpy_fri";
		LAi_SetActorType(rCharacter);
		LAi_group_MoveCharacter(rCharacter, "EnemyFight");
		ChangeCharacterAddressGroup(rCharacter, pchar.location, "goto", "goto6");
		DoQuestCheckDelay("CannotFightCurLocation", 1.0);
		pchar.questTemp.Sharp.SeekSpy = "inDelNorte";
		pchar.quest.SharpSeekSpy_script.win_condition.l1 = "locator";
		pchar.quest.SharpSeekSpy_script.win_condition.l1.location = "Shore55";
		pchar.quest.SharpSeekSpy_script.win_condition.l1.locator_group = "quest";
		pchar.quest.SharpSeekSpy_script.win_condition.l1.locator = "SharpSeekSpy";
		pchar.quest.SharpSeekSpy_script.function = "SharpSeekSpy_script";
	}
	else
	{
		SetTimerFunction("SharpSeekSpy_again", 0, 0, 3);
	}
}

void SharpSeekSpy_script(string qName)
{
	if (GetCharacterIndex("Emillio") != -1)
	{
		LAi_ActorTurnToCharacter(characterFromId("QuestMan_5"), pchar);
		sld = characterFromId("Emillio");
		sld.dialog.currentnode = "SharpSeekSpy_Emi";
		LAi_ActorDialog(sld, pchar, "", 0, 0);
	}
	else
	{
		pchar.questTemp.Sharp.SeekSpy = "over";
	}
}

void SharpSeekSpy_again(string qName)
{
	pchar.quest.SharpSeekSpy_loginSpy.win_condition.l1 = "location";
	pchar.quest.SharpSeekSpy_loginSpy.win_condition.l1.location = "Shore55";
	pchar.quest.SharpSeekSpy_loginSpy.function = "SharpSeekSpy_loginSpy";	
}

void SharpSeekSpy_caveDialog()
{
	sld = characterFromId("QuestMan_5");
	sld.Dialog.Filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "SharpSeekSpy_fri";
	LAi_ActorDialog(sld, pchar, "", 0, 0);
}

void SharpSeekSpy_caveDialog_2()
{
	sld = characterFromId("Emillio");
	sld.dialog.currentnode = "SharpSeekSpy_Emi_1";
	LAi_ActorDialog(sld, pchar, "", 0, 0);
}

void SharpSeekSpy_caveDialog_3()
{
	DoQuestCheckDelay("CanFightCurLocation", 0.0);
	sld = characterFromId("Emillio");
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	sld = characterFromId("QuestMan_5");
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
    pchar.quest.SharpSeekSpy_afterFight.win_condition.l1 = "NPC_Death";
	pchar.quest.SharpSeekSpy_afterFight.win_condition.l1.character = "Emillio";
    pchar.quest.SharpSeekSpy_afterFight.win_condition.l2 = "NPC_Death";
	pchar.quest.SharpSeekSpy_afterFight.win_condition.l2.character = "QuestMan_5";
	pchar.quest.SharpSeekSpy_afterFight.function = "SharpSeekSpy_afterFight";
}

void SharpSeekSpy_afterFight(string qName)
{
	AddQuestRecord("SharpPearl_SeekSpy", "4");
	pchar.questTemp.Sharp.SeekSpy = "alwaysKnow";
	pchar.questTemp.Sharp = "seekSharp"; //����� ����� �������� �����
	CloseQuestHeader("SharpPearl");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ��������� ���������� �������� Sharp   �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ��������� �������     ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void PiratesLine_q1_tavernEnd()
{
	chrDisableReloadToLocation = false; 
	bDisableFastReload = false; 
	LAI_SetPlayerType(pchar);           
    DoReloadCharacterToLocation("PuertoPrincipe_tavern", "tables", "stay3");
    sld = CharacterFromID("QuestPirate1");
    LAI_SetSitType(sld);
}

void PiratesLine_q1_arrest(string qName)
{
	chrDisableReloadToLocation = true; 
	bDisableFastReload = true; 
	//LocatorReloadEnterDisable("Portroyal_town", "houseS1", true); //������� ��� �������
	GetCharacterPos(pchar, &locx, &locy, &locz);
    for (i=1; i<=3; i++)
    {
		sld = GetCharacter(NPC_GenerateCharacter("QuestSold_"+i, "sold_eng_"+i, "man", "man", 20, ENGLAND, 0, true));
		sld.City = "PortRoyal";
		//sld.CityType = "soldier";
		sld.BreakTmplAndFightGroup = true; //����������� �� �����
		sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
		sld.dialog.currentnode = "ArrestInPR";
		LAi_SetActorType(sld);
		LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
		ChangeCharacterAddressGroup(sld, pchar.location, "patrol", "patrol16");
		if (i == 1) LAi_ActorDialog(sld, pchar, "", -1, 0);
		else LAi_ActorFollow(sld, characterFromId("QuestSold_1"), "", -1);
	}	
	sld = characterFromId("QuestPirate2");
	ChangeCharacterAddressGroup(sld, pchar.location, "goto", LAi_FindNearestFreeLocator("goto", locx, locy, locz));	
	LAi_SetActorTypeNoGroup(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	LAi_ActorFollow(sld, pchar, "", -1);
}

void PiratesLine_q1_toPrison()
{
	sld = characterFromId("QuestPirate2");
	sld.lifeDay = 0;
	ChangeCharacterAddress(sld, "none", "");
	RemovePassenger(pchar, sld);
	DoReloadCharacterToLocation("PortRoyal_prison",  "goto", "goto9");
	RemoveCharacterEquip(pchar, BLADE_ITEM_TYPE);
	RemoveCharacterEquip(pchar, GUN_ITEM_TYPE);
	LAi_LocationFightDisable(&Locations[FindLocation("PortRoyal_prison")], true);
    DoQuestFunctionDelay("PiratesLine_q1_MorganInPrison", 10.0);
    sld = characterFromId("Henry Morgan");
    LAi_SetStayTypeNoGroup(sld);
    ChangeCharacterAddressGroup(sld, "PortRoyal_prison", "goto", "goto13");
	locations[FindLocation("PortRoyal_fort")].loginNpc = true;
}

void PiratesLine_q1_MorganInPrison(string qName)
{
    sld = characterFromID("Henry Morgan");
    LAi_SetActorTypeNoGroup(sld);
    LAi_ActorGoToLocator(sld, "goto", "goto23", "PiratesLine_q1_MorganGoTo", -1);
}

void PiratesLine_q1_MorganEnd()
{
	chrDisableReloadToLocation = false; 
	bDisableFastReload = false; 
	LocatorReloadEnterDisable("Portroyal_town", "houseS1", false); //������� ��� �������
	LAi_LocationFightDisable(&Locations[FindLocation("PortRoyal_prison")], false);
	pchar.questTemp.jailCanMove = true; //����� �������� ����� �� ������
	AddQuestRecord("Pir_Line_1_JohnBolton", "4");
	DoQuestReloadToLocation("PortRoyal_prison", "goto", "goto12", "");
    sld = characterFromId("Henry Morgan");
    LAi_SetHuberTypeNoGroup(sld);
    ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
}
//�������, ����� �3, ������ ���
void PiratesLine_q3_LoyHouse(string qName)
{
	AddQuestRecord("Pir_Line_3_KillLoy", "2");
	pchar.questTemp.piratesLine = "KillLoy_toSeek";
}

void PiratesLine_q3_loginSeaWolf(string qName)
{
	if (GetCompanionQuantity(pchar) <= 1 && sti(RealShips[sti(pchar.ship.type)].basetype) < 4)
	{
		sld = characterFromId("EdwardLoy");
		sld.dialog.currentnode   = "SeaWolf";
		sld.greeting = "CapSinkShip";
		pchar.questTemp.piratesLine = "KillLoy_SeaWolfBattle";
		AddQuestRecord("Pir_Line_3_KillLoy", "15");
		Group_AddCharacter("SW_Group", "EdwardLoy");			
		Group_SetType("SW_Group", "pirate");
		Group_SetGroupCommander("SW_Group", "EdwardLoy");
		sTemp = "quest_ship_" + (rand(4)+5);
		Group_SetAddress("SW_Group", "Cumana", "quest_ships", sTemp);
		Group_SetTaskAttack("SW_Group", PLAYER_GROUP);
		pchar.Quest.PiratesLine_q3_over1.win_condition.l1 = "Location_Type";
		pchar.Quest.PiratesLine_q3_over1.win_condition.l1.location_type = "town";
		pchar.Quest.PiratesLine_q3_over1.function = "PiratesLine_q3_over";
		pchar.Quest.PiratesLine_q3_over2.win_condition.l1 = "Location_Type";
		pchar.Quest.PiratesLine_q3_over2.win_condition.l1.location_type = "seashore";
		pchar.quest.PiratesLine_q3_over2.function = "PiratesLine_q3_over";
	}
	else
	{
		pchar.quest.PiratesLine_q3_SeaWolfAgain.win_condition.l1 = "ExitFromLocation";
		pchar.quest.PiratesLine_q3_SeaWolfAgain.win_condition.l1.location = "Cumana";
		pchar.quest.PiratesLine_q3_SeaWolfAgain.function = "PiratesLine_q3_SeaWolfAgain";	
	}
}

void PiratesLine_q3_SeaWolfAgain(string qName)
{
	pchar.quest.PiratesLine_q3_loginSeaWolf.win_condition.l1 = "location";
	pchar.quest.PiratesLine_q3_loginSeaWolf.win_condition.l1.location = "Cumana";
	pchar.quest.PiratesLine_q3_loginSeaWolf.function = "PiratesLine_q3_loginSeaWolf";	
}

void PiratesLine_q3_over(string qName)
{
	pchar.quest.PiratesLine_q3_over1.over = "yes";
	pchar.quest.PiratesLine_q3_over2.over = "yes";
	if (GetCharacterIndex("EdwardLoy") == -1)
	{
		QuestSetCurrentNode("Henry Morgan", "PL_Q3_GoodWork");
		if (sti(RealShips[sti(pchar.ship.type)].basetype) == SHIP_BRIGSW)
		{
			AddQuestRecord("Pir_Line_3_KillLoy", "17");
			pchar.questTemp.piratesLine = "KillLoy_GoodWork"; //��������� ���, ���� ��������			
		}
		else
		{
			AddQuestRecord("Pir_Line_3_KillLoy", "18");
			pchar.questTemp.piratesLine = "KillLoy_LoyIsDied"; //��� ����, ���� �� ��������
		}
	}
	else
	{
		AddQuestRecord("Pir_Line_3_KillLoy", "16");
		pchar.questTemp.piratesLine = "KillLoy_loose"; //��� ������� ���, ������ ������
		sld = characterFromId("EdwardLoy");
		sld.lifeDay = 0;
		QuestSetCurrentNode("Henry Morgan", "PL_Q3_notFound");
	}
	group_DeleteGroup("SW_Group");
}
//�������, ����� �4, ������� ��������� ������
void PQ4_SeaPearl_login(string qName)
{
	pchar.quest.PQ4_SeaPearl_Late.over = "yes";
	LAi_group_Delete("EnemyFight");
	group_DeleteGroup("PearlGroup");
	iTemp = sti(pchar.questTemp.Sharp.PearlAreal.qtyShips);
	for (i = 1; i <= iTemp; i++)
	{                
		sTemp = "PearlCapitain_" + i;
		sld = GetCharacter(NPC_GenerateCharacter(sTemp, "trader_"+(rand(15)+1), "man", "man", 10, PIRATE, 30, true));	
		sld.Ship.Type = GenerateShip(SHIP_TARTANE, false);
		SetRandomNameToShip(sld);
		SetBaseShipData(sld);
		SetCrewQuantityFull(sld);
		Fantom_SetBalls(sld, "pirate");
		sld.ShipTaskLock = true;
		sld.Abordage.Enable = false;
		sld.SinkTenPercent = true; //�������� � �� �����
		sld.PearlTartane = true; //������� ������ �������, ����� �������
		LAi_group_MoveCharacter(sld, "EnemyFight");
		Group_AddCharacter("PearlGroup", sTemp);
	}
    // ==> ������������
	Group_SetGroupCommander("PearlGroup", "PearlCapitain_1");			
	Group_SetAddress("PearlGroup", pchar.questTemp.Sharp.PearlAreal.Island, "quest_ships", pchar.questTemp.Sharp.PearlAreal.locator);
	Group_SetTaskRunaway("PearlGroup", PLAYER_GROUP);
	Group_LockTask("PearlGroup");
	pchar.quest.PQ4_SeaPearl_DieHard1.win_condition.l1 = "ExitFromLocation";
	pchar.quest.PQ4_SeaPearl_DieHard1.win_condition.l1.location = pchar.questTemp.Sharp.PearlAreal.Island;
	pchar.quest.PQ4_SeaPearl_DieHard1.function = "PQ4_SeaPearl_DieHard";
	Pchar.quest.PQ4_SeaPearl_DieHard2.win_condition.l1 = "MapEnter";
	pchar.quest.PQ4_SeaPearl_DieHard2.function = "PQ4_SeaPearl_DieHard";
}

void PQ4_SeaPearl_Late(string qName)
{	
	pchar.quest.PQ4_SeaPearl_login.over = "yes";
	AddQuestRecord("Pir_Line_4_Pearl", "2");
	pchar.questTemp.piratesLine = "PearlQ4_late";
	sld = characterFromId("Henry Morgan");
    LAi_SetHuberTypeNoGroup(sld);
	ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
	QuestSetCurrentNode("Henry Morgan", "PL_Q4_late");
}

void PQ4_SeaPearl_DieHard(string qName)
{
	group_DeleteGroup("PearlGroup");
	pchar.quest.PQ4_SeaPearl_DieHard1.over = "yes";
	pchar.quest.PQ4_SeaPearl_DieHard2.over = "yes";
	if (pchar.questTemp.Sharp == "SeaPearl_success")
	{	//�������� ���������
		AddQuestRecord("Pir_Line_4_Pearl", "3");
		AddQuestUserData("Pir_Line_4_Pearl", "iSmall", pchar.questTemp.Sharp.PearlAreal.SmallPearlQty);
		AddQuestUserData("Pir_Line_4_Pearl", "iBig", pchar.questTemp.Sharp.PearlAreal.BigPearlQty);
		pchar.questTemp.piratesLine = "PearlQ4_goodWork";
		QuestSetCurrentNode("Henry Morgan", "PL_Q4_goodWork");
		pchar.questTemp.piratesLine.Qty.SmallPearl = pchar.questTemp.Sharp.PearlAreal.SmallPearlQty;
		pchar.questTemp.piratesLine.Qty.BigPearl = pchar.questTemp.Sharp.PearlAreal.BigPearlQty;
	}
	else
	{	//��������� ���������
		AddQuestRecord("Pir_Line_4_Pearl", "4");
		pchar.questTemp.piratesLine = "PearlQ4_badWork";
		QuestSetCurrentNode("Henry Morgan", "PL_Q4_badWork");
	}
	pchar.questTemp.Sharp = "seekSharp"; //����� �������� � ��������
	pchar.questTemp.Sharp.brothelChance = rand(10);
	DeleteAttribute(pchar, "questTemp.Sharp.PearlAreal");
	CloseQuestHeader("SharpPearl");
}
//�������, ����� �5, ������ ����
void PQ5_meetInPortRoyal(string qName)
{
	chrDisableReloadToLocation = true;
	sld = GetCharacter(NPC_GenerateCharacter("CapGoodly", "officer_9", "man", "man", 20, PIRATE, -1, true));
	sld.name = "�������";
	sld.lastname = "�����";
	sld.rank = 20;
	sld.city = "PortRoyal";
	sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode   = "PQ5CapGoodly";
	sld.greeting = "pirat_quest";
    ChangeCharacterAddressGroup(sld, "PortRoyal_town", "patrol", "patrol16");
	LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	LAi_ActorDialog(sld, pchar, "", 2.0, 0);
}

void PQ5_inOrryHouse(string qName)
{
	pchar.questTemp.piratesLine = "PL5Hunter_exitReload";
	LAi_LocationFightDisable(&locations[FindLocation("Pirates_houseS1")], true); //������� ������
	chrDisableReloadToLocation = true; 
	LAi_group_Delete("EnemyFight");

	sld = GetCharacter(NPC_GenerateCharacter("Aivory", "officer_7", "man", "man", 30, PIRATE, -1, true));
	FantomMakeCoolFighter(sld, 30, 100, 100, "blade28", "pistol3", 80);
	sld.name = "����";
	sld.lastname = "������";
	sld.city = "Pirates";
	sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode   = "PQ5Hero";
	sld.greeting = "pirat_quest";
	LAi_SetStayType(sld);
    ChangeCharacterAddressGroup(sld, "Pirates_houseS1", "goto", "goto1");

	sld = GetCharacter(NPC_GenerateCharacter("Orry", "officer_15", "man", "man", 27, PIRATE, -1, true));
	FantomMakeCoolFighter(sld, 27, 100, 100, "topor2", "pistol3", 50);
	sld.name = "����";
	sld.lastname = "����";
	sld.city = "Pirates";
	sld.money = 51600;
	sld.SaveItemsForDead   = true; // ��������� �� ����� ����
	sld.DontClearDead = true;  // �� ������� ���� ����� 200�
	sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode   = "PQ5Orry";
	sld.greeting = "pirat_quest";
	LAi_SetStayType(sld);
    ChangeCharacterAddressGroup(sld, "Pirates_houseS1", "goto", "goto2");

	pchar.quest.PQ5_afterFight.win_condition.l1 = "NPC_Death";
	pchar.quest.PQ5_afterFight.win_condition.l1.character = "Aivory";
	pchar.quest.PQ5_afterFight.win_condition.l2 = "NPC_Death";
	pchar.quest.PQ5_afterFight.win_condition.l2.character = "Orry";
	pchar.quest.PQ5_afterFight.function = "PQ5_afterFight";
}

void PQ5_afterChangeMainHero()
{
	LAi_LocationFightDisable(&locations[FindLocation("Pirates_houseS1")], false);
    SetMainCharacterIndex(1);
    PChar = GetMainCharacter();
    LAi_SetPlayerType(PChar);
	locCameraTarget(PChar)
    locCameraFollow();	
	rCharacter = characterFromID("Orry");
	LAi_SetActorType(rCharacter);
	LAi_ActorTurnToCharacter(rCharacter, pchar);
	sld = characterFromID("Aivory");
	sld.dialog.currentnode = "PQ5Aivory";
	LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", -1, 0);
	EndQuestMovie();
}

void PQ5_afterChangeMainHero2()
{	
	rCharacter = characterFromID("Orry");
	LAi_group_MoveCharacter(rCharacter, "EnemyFight");
	sld = characterFromID("Aivory");			
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
	LAi_SetFightMode(pchar, true);
}

void PQ5_afterFight(string qName)
{
	AddQuestRecord("Pir_Line_5_Hunter", "11");
	AddQuestUserData("Pir_Line_5_Hunter", "sCity", XI_ConvertString("Colony" + pchar.questTemp.piratesLine.Q5.city_4 + "Gen"));
	pchar.questTemp.piratesLine = "PL5Hunter_toMorgan";
	chrDisableReloadToLocation = false; 
	Pchar.quest.PQ5_inPortPoyalTavern.win_condition.l1 = "location";
	Pchar.quest.PQ5_inPortPoyalTavern.win_condition.l1.location = "PortRoyal_tavern";
	Pchar.quest.PQ5_inPortPoyalTavern.function = "PQ5_inPortPoyalTavern";
}

void PQ5_inPortPoyalTavern(string qName)
{
	FreeSitLocator("PortRoyal_tavern", "sit3");
	sld = characterFromID("CapGoodly");
	sld.dialog.currentnode = "PQ5CapGoodlyEnd";
	LAi_SetSitTypeNoGroup(sld);
    ChangeCharacterAddressGroup(sld, "PortRoyal_tavern", "sit", "sit3");
}

void PQ5_Morgan_1()
{
	LAi_SetActorType(pchar);
	LAi_ActorGoToLocator(pchar, "goto", "goto2", "PQ5_Morgan_2", -1.0);
}

void PQ5_Morgan_4()
{
    SetMainCharacterIndex(1);
    PChar = GetMainCharacter();
    LAi_SetPlayerType(PChar);
	locCameraTarget(PChar)
    locCameraFollow();	
	rCharacter = characterFromID("CapGoodly");
	LAi_SetActorType(rCharacter);
	LAi_ActorTurnToCharacter(rCharacter, pchar);
	sld = characterFromID("Henry Morgan");
	LAi_SetActorType(pchar);
	LAi_ActorDialog(pchar, sld, "", 0, 0);
}

void PQ5_Morgan_5()
{
	LAi_group_Delete("EnemyFight");
	LAi_SetPlayerType(pchar);
	sld = characterFromID("CapGoodly");
	LAi_group_MoveCharacter(sld, "EnemyFight");
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
	pchar.quest.PQ5_Morgan_afterFight.win_condition.l1 = "NPC_Death";
	pchar.quest.PQ5_Morgan_afterFight.win_condition.l1.character = "CapGoodly";
	pchar.quest.PQ5_Morgan_afterFight.function = "PQ5_Morgan_afterFight";
	LAi_SetFightMode(pchar, true);
}

void PQ5_Morgan_afterFight(string qName)
{
	QuestSetCurrentNode("Henry Morgan", "PL_Q5_afterFight");
	AddQuestRecord("Pir_Line_5_Hunter", "13");
}
//�������, ����� �6, �������
void PQ6_ShoreBattle(string qName)
{
    LAi_group_Delete("EnemyFight"); 
	LocatorReloadEnterDisable("Shore53", "boat", true); //������� ����� � ����
	GetCharacterPos(pchar, &locx, &locy, &locz);
	//����
	string sOurModel;
	string sTemp = NationShortName(sti(pchar.nation));
	if (sti(pchar.nation) == PIRATE)
	{
		sOurModel = "pirate_" + (rand(9)+1);
	}
	else
	{
		sOurModel = "sold_" + sTemp + "_" + (rand(7)+1);
	}
    for (i=1; i<=5; i++)
    {
        iTemp = 10 + rand(10);
		sld = GetCharacter(NPC_GenerateCharacter("OwnPirate_"+i, sOurModel, "man", "man", iTemp, sti(pchar.nation), 0, true));
        LAi_SetWarriorType(sld);
		LAi_warrior_DialogEnable(sld, false);
        LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		//sTemp = LAi_FindNearestFreeLocator("goto", locx, locy, locz);
		//if (sTemp == "") sTemp = LAi_FindNearestLocator("goto", locx, locy, locz);
        ChangeCharacterAddressGroup(sld, "Shore53", "goto", "goto5");
    }
	//�����
	for (i=1; i<=(MOD_SKILL_ENEMY_RATE + 5); i++)
    {
        iTemp = 15 + rand(10);
		sld = GetCharacter(NPC_GenerateCharacter("QuestPirate_"+i, "officer_"+(rand(19)+1), "man", "man", iTemp, PIRATE, -1, true));
        FantomMakeCoolFighter(sld, iTemp, 80, 80, BLADE_LONG, "pistol3", 50);
        LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        PlaceCharacter(sld, "goto", PChar.location);
    }
    LAi_group_SetLookRadius("EnemyFight", 100);
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
    Pchar.quest.PQ6_JungleDialog.win_condition.l1 = "location";
    Pchar.quest.PQ6_JungleDialog.win_condition.l1.location = "Common_jungle_01";
    Pchar.quest.PQ6_JungleDialog.function = "PQ6_JungleDialog";
}

void PQ6_JungleDialog(string qName)
{
	LAi_LocationFightDisable(&Locations[FindLocation("Common_jungle_01")], true);
	chrDisableReloadToLocation = true;
	string sModel = pchar.model;
	//������� ������ � ��������
	switch (sModel)
	{
		case "Hero1_Cirass": sModel = "Hero1"; break;
		case "Hero1_Cirass1": sModel = "Hero1"; break;
		case "Hero1_Cirass2": sModel = "Hero1"; break;
		case "Hero1_Cirass3": sModel = "Hero1"; break;
		case "Espinosa_Cirass": sModel = "Espinosa"; break;
		case "Espinosa_Cirass1": sModel = "Espinosa"; break;
		case "Espinosa_Cirass2": sModel = "Espinosa"; break;
		case "Espinosa_Cirass3": sModel = "Espinosa"; break;
		case "Isterling_Cirass": sModel = "Isterling"; break;
		case "Isterling_Cirass1": sModel = "Isterling"; break;
		case "Isterling_Cirass2": sModel = "Isterling"; break;
		case "Isterling_Cirass3": sModel = "Isterling"; break;
	}
	sld = GetCharacter(NPC_GenerateCharacter("QuestPirateHead", sModel, "man", pchar.model.animation, 30, PIRATE, -1, true));
	FantomMakeCoolFighter(sld, 30, 90, 90, "topor2", "pistol6", 100);
	sld.name = pchar.name;
	sld.lastname = pchar.lastname;
    sld.dialog.Filename = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode = "QuestPirateHead";
	ChangeCharacterAddressGroup(sld, "Common_jungle_01", "goto", "goto1");
    LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	LAi_ActorDialog(sld, pchar, "CanFightCurLocation", 2.0, 0);
}

void PQ6_JungleBattle()
{
	LAi_SetFightMode(pchar, true);
	//����
	string sOurModel;
	string sTemp = NationShortName(sti(pchar.nation));
	if (sti(pchar.nation) == PIRATE)
	{
		sOurModel = "pirate_" + (rand(9)+1);
	}
	else
	{
		sOurModel = "sold_" + sTemp + "_" + (rand(7)+1);
	}
    for (i=1; i<=5; i++)
    {
        iTemp = 10 + rand(10);
		sld = GetCharacter(NPC_GenerateCharacter("OwnPirate2_"+i, sOurModel, "man", "man", iTemp, sti(pchar.nation), 0, true));
        LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
        ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
    }
	//�����
	for (i=1; i<=(MOD_SKILL_ENEMY_RATE + 7); i++)
    {
        iTemp = 17 + rand(10);
		sld = GetCharacter(NPC_GenerateCharacter("QuestPirate2_"+i, "officer_"+(rand(19)+1), "man", "man", iTemp, PIRATE, -1, true));
        FantomMakeCoolFighter(sld, iTemp, 90, 90, "topor2", "pistol6", 100);
        LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, "Common_jungle_01", "goto", "goto2");
    }
    LAi_group_SetLookRadius("EnemyFight", 100);
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
	LAi_group_SetCheck("EnemyFight", "PQ6_afterBattle");
}
//�������, ����� �7, ����� ��������
void PQ7_FriendLinney(string qName)
{
	if (GetQuestPastMinutesParam("questTemp.piratesLine") < 20)
	{
		sld = GetCharacter(NPC_GenerateCharacter("FriendLinney", "", "man", "man", 20, sti(pchar.nation), 5, true));							
		SetCaptanModelByEncType(sld, "pirate");
		sld.dialog.filename = "Quest\PiratesLine_dialog.c";
		sld.DeckDialogNode = "FriendLinney";
		FantomMakeCoolFighter(sld, 20, 80, 70, "blade32", "pistol3", 20);
		FantomMakeCoolSailor(sld, SHIP_LUGGER, "", CANNON_TYPE_CULVERINE_LBS16, 70, 70, 70);
		sld.AlwaysSandbankManeuver = true;
		sld.DontRansackCaptain = true;
		sld.Ship.Mode = "Pirate";
		DeleteAttribute(sld, "SinkTenPercent");
		DeleteAttribute(sld, "SaveItemsForDead");
		DeleteAttribute(sld, "DontClearDead");
		DeleteAttribute(sld, "AboardToFinalDeck");
		DeleteAttribute(sld, "SinkTenPercent");
		DeleteAttribute(sld, "DontRansackCaptain");		
		sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
		Group_AddCharacter("FriendLinney_Group", "FriendLinney");			
		Group_SetType("FriendLinney_Group", "pirate");
		Group_SetGroupCommander("FriendLinney_Group", "FriendLinney");
		Group_SetAddress("FriendLinney_Group", "Hispaniola1", "quest_ships", "quest_ship_4");
		Group_SetTaskNone("FriendLinney_Group");
	}
	else
	{
		AddQuestRecord("Pir_Line_7_Soukins", "3");
		pchar.questTemp.piratesLine = "Soukins_LaVegaLate";
		QuestSetCurrentNode("Henry Morgan", "PL_Q7_LaVegaLate");
	}
}

void PQ7_LoginLeon()
{
	LAi_group_Delete("EnemyFight");
	group_DeleteGroup("FriendLinney_Group"); //������ ����� ����� ������
	sld = GetCharacter(NPC_GenerateCharacter("LeonCapitain", "", "man", "man", 25, PIRATE, 30, true));							
	SetCaptanModelByEncType(sld, "pirate");
	sld.dialog.filename = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode = "LeonCapitain";
	sld.greeting = "CapSinkShip";
	FantomMakeCoolFighter(sld, 25, 80, 80, "blade32", "pistol6", 50);
	FantomMakeCoolSailor(sld, SHIP_FRIGATE, "����", CANNON_TYPE_CULVERINE_LBS24, 80, 80, 80);
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "SaveItemsForDead");
	DeleteAttribute(sld, "DontClearDead");
	DeleteAttribute(sld, "AboardToFinalDeck");
	DeleteAttribute(sld, "SinkTenPercent");
	DeleteAttribute(sld, "DontRansackCaptain");
	sld.Ship.Mode = "Pirate";
	sld.AlwaysSandbankManeuver = true;
	sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
	sld.DontRansackCaptain = true;
	SetCharacterPerk(sld, "FastReload");
	SetCharacterPerk(sld, "HullDamageUp");
	SetCharacterPerk(sld, "SailsDamageUp");
	SetCharacterPerk(sld, "CrewDamageUp");
	SetCharacterPerk(sld, "CriticalShoot");
	SetCharacterPerk(sld, "LongRangeShoot");
	SetCharacterPerk(sld, "CannonProfessional");
	SetCharacterPerk(sld, "ShipDefenseProfessional");
	SetCharacterPerk(sld, "SwordplayProfessional");
	SetCharacterPerk(sld, "AdvancedDefense");
	SetCharacterPerk(sld, "CriticalHit");
	SetCharacterPerk(sld, "Sliding");
	LAi_group_MoveCharacter(sld, "EnemyFight");

	string sGroup = "Leon_Group";
	Group_FindOrCreateGroup(sGroup);
	Group_SetType(sGroup,"trade");
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 

	sld.mapEnc.type = "warrior";
	sld.mapEnc.worldMapShip = "quest_ship";
	sld.mapEnc.Name = "������ '����'";
	Map_CreateWarrior("shore33", "LeonCapitain", 30);
	Log_TestInfo("������ ���� ����������.");
}

void PQ7_inSoukinsResidence(string qName)
{
	QuestSetCurrentNode("Henry Morgan", "PL_Q7_BadWork"); //����, ���� ������ �� ������
	if (IsLoginTime())
	{
		DoQuestFunctionDelay("PQ7_inSoukinsResidence_2", 1.5);
	}
}

void PQ7_inSoukinsResidence_2(string qName)
{
	LAi_SetActorType(pchar);
	sld = characterFromId("Richard_Soukins");
	QuestSetCurrentNode("Richard_Soukins", "PL_Q7_5");
	LAi_ActorDialog(pchar, sld, "", 0, 0);
}

void PQ7_loadToRecidence(string qName)
{
	DoQuestReloadToLocation("PuertoPrincipe_townhall", "reload", "reload2", "");
}

void PQ7_setBattleShip(string qName)
{	
	sld = &locations[FindLocation("PuertoPrincipe_townhall")];
	DeleteAttribute(sld, "reload.l2"); //������ ����������� ���������� � ��������
	if (GetQuestPastMinutesParam("questTemp.piratesLine") > 100)
	{
		Log_TestInfo("������� �������� �� ����� ����������. �� ����� ������� ����� � ����.");
		pchar.questTemp.piratesLine = "Soukins_battlshipLate";
		AddQuestRecord("Pir_Line_7_Soukins", "10");
	}
	else
	{
		sld = GetCharacter(NPC_GenerateCharacter("QuestCap_PL7", "SpaOfficer1", "man", "man", 40, SPAIN, -1, true));							
		sld.dialog.filename = "Quest\PiratesLine_dialog.c";
		sld.dialog.currentnode = "QuestCap_PL7";
		sld.DeckDialogNode = "QuestCap_PL7_inDeck";
		sld.greeting = "CapSinkShip";
		FantomMakeCoolFighter(sld, 40, 90, 90, "blade32", "pistol4", 200);
		FantomMakeCoolSailor(sld, SHIP_BATTLESHIP, "����������", CANNON_TYPE_CULVERINE_LBS32, 90, 90, 90);
		DeleteAttribute(sld, "SinkTenPercent");
		DeleteAttribute(sld, "SaveItemsForDead");
		DeleteAttribute(sld, "DontClearDead");
		DeleteAttribute(sld, "AboardToFinalDeck");
		DeleteAttribute(sld, "SinkTenPercent");
		DeleteAttribute(sld, "DontRansackCaptain");
		sld.AlwaysSandbankManeuver = true;
		sld.AnalizeShips = true;  //������������� ��������� ������� ��� ������ �����
		sld.DontRansackCaptain = true;
		SetCharacterPerk(sld, "FastReload");
		SetCharacterPerk(sld, "HullDamageUp");
		SetCharacterPerk(sld, "SailsDamageUp");
		SetCharacterPerk(sld, "CrewDamageUp");
		SetCharacterPerk(sld, "CriticalShoot");
		SetCharacterPerk(sld, "LongRangeShoot");
		SetCharacterPerk(sld, "CannonProfessional");
		SetCharacterPerk(sld, "ShipDefenseProfessional");
		SetCharacterPerk(sld, "ShipTurnRateUp");
		SetCharacterPerk(sld, "ShipTurnRateUp");
		SetCharacterPerk(sld, "StormProfessional");
		SetCharacterPerk(sld, "SwordplayProfessional");
		SetCharacterPerk(sld, "AdvancedDefense");
		SetCharacterPerk(sld, "CriticalHit");
		SetCharacterPerk(sld, "Sliding");
		SetCharacterPerk(sld, "Tireless");
		SetCharacterPerk(sld, "HardHitter");
		SetCharacterPerk(sld, "GunProfessional");

		string sGroup = "Quest_Ship";
		Group_FindOrCreateGroup(sGroup);
		Group_SetType(sGroup,"trade");
		Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
		Group_LockTask(sGroup);
		Group_AddCharacter(sGroup, sld.id);
		Group_SetGroupCommander(sGroup, sld.id);
		SetRandGeraldSail(sld, sti(sld.Nation)); 

		sld.mapEnc.type = "trade";
		sld.mapEnc.worldMapShip = "quest_ship";
		sld.mapEnc.Name = "��������";
		Map_CreateTraderXZ(1009.22, -992.708, 988.514, 515.407, "QuestCap_PL7", 15);
		Log_TestInfo("������� �������� ����� ��������.");	
	}
}

void PQ7_winBattleShip(string qName)
{
	pchar.questTemp.piratesLine = "Soukins_battlshipWin";
	QuestSetCurrentNode("Henry Morgan", "PL_Q7_GoodWork");
	AddQuestRecord("Pir_Line_7_Soukins", "13");
	pchar.quest.PQ7_BattleShip_dieHard.over = "yes";
}

void PQ8_timeOver(string qName) //������� �������
{
	pchar.questTemp.piratesLine = "over";
	sld = characterFromId("Henry Morgan");
	ChangeCharacterAddress(sld, "none", "");
	AddQuestRecord("Pir_Line_8_Panama", "2");
	CloseQuestHeader("Pir_Line_8_Panama");
	SetTimerFunction("PQ8_openPanama", 0, 0, 40);
	QuestSetCurrentNode("Oliver_Green", "PL_Q8_over"); //���� ���������
}

void PQ8_openPanama(string qName) //������� ������
{
	LocatorReloadEnterDisable("Shore48", "reload1_back", false); 
	LocatorReloadEnterDisable("PortoBello_Jungle_01", "reload1_back", false);
	sld = characterFromId("Henry Morgan");
	sld.dialog.currentnode = "First time";
    ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
    LAi_SetHuberType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
}

void PQ8_openPanama_2(string qName) //������� ������
{
	sld = characterFromId("Henry Morgan");
	if (pchar.questTemp.piratesLine == "Panama_backToShip") 
	{
		SetTimerFunction("PQ8_openPanama_3", 0, 0, 180);
	}
	else 
	{
		sld.dialog.currentnode = "I_know_you_good";
		ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
		LAi_SetImmortal(sld, true);
		LAi_LoginInCaptureTown(sld, false);
		LAi_SetHuberType(sld);
		LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	}
	//���������� ��������
	iTemp = GetCharacterIndex("Jackman");
	if (iTemp != -1)
	{
		if(IsCompanion(&characters[iTemp]))  
		{
			RemoveCharacterCompanion(pchar, &characters[iTemp]);
		}               
		LAi_SetHuberType(&characters[iTemp]);
		LAi_group_MoveCharacter(&characters[iTemp], "PIRATE_CITIZENS");
        ChangeCharacterAddressGroup(&characters[iTemp], "Pirates_townhall", "sit", "sit1");
        characters[iTemp].dialog.currentnode = "I_know_you_good";
    }
    //���������� �� ����� ��������
	iTemp = GetCharacterIndex("Richard_Soukins");
	if (iTemp != -1)
	{
		if(IsCompanion(&characters[iTemp]))  
		{
			RemoveCharacterCompanion(pchar, &characters[iTemp]);
		}                   
		LAi_SetHuberType(&characters[iTemp]);
		LAi_group_MoveCharacter(&characters[iTemp], "PIRATE_CITIZENS");
        ChangeCharacterAddressGroup(&characters[iTemp], "PuertoPrincipe_townhall", "sit", "sit1");
        characters[iTemp].dialog.currentnode = "First time";
    }
	LocatorReloadEnterDisable("Panama_ExitTown", "reload4", false); 
	SetLocationCapturedState("Panama_town", false);
	bDisableLandEncounters = false;
	pchar.quest.PL_Q8_backFight_1.over = "yes"; //�� ������ ������
}

void PQ8_openPanama_3(string qName) //������ ����� ������� � ������
{
	sld = characterFromId("Henry Morgan");
	sld.dialog.currentnode = "PL_Q8_afterPanama";
	ChangeCharacterAddressGroup(sld, "PortRoyal_houseS1", "sit", "sit2");
	LAi_SetImmortal(sld, true);
	LAi_LoginInCaptureTown(sld, false);
	LAi_SetHuberType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
}

void PQ8_PanamaTimerOver(string qName)
{
    if (IsEntity(worldMap))
    {
        if (!bQuestCheckProcessFreeze) // ����� ��������
		{
	        pchar.quest.PQ8_PanamaTimerOver_01.over = "yes"; // �����, ���� ��� �����������
	        pchar.quest.PQ8_PanamaTimerOver_02.over = "yes";
			pchar.quest.PQ8_EnterPortoBello.over = "yes"; //������� ���������� �� ����
            
            aref arOldMapPos;
			makearef(arOldMapPos, worldMap.old);
            WdmPrepareMapForAbordage(arOldMapPos);
            MakeCloneShipDeck(pchar, true); // ������� ������
            DoReloadFromWorldMapToLocation("Ship_deck", "goto", "goto5");
	        ReloadFromWMtoL_complete();
	        
            sld = characterFromID("Henry Morgan");
            ChangeCharacterAddressGroup(sld, "Ship_deck", "goto", "goto6");
            LAi_SetWarriorType(sld);
	        LAi_warrior_DialogEnable(sld, true);
	        sld.dialog.currentnode = "PL_Q8_timeOver";
   	        // ==> �������
            for (i=1; i<5; i++)
        	{
                sTemp = "pirate_" + i;
                sld = GetCharacter(NPC_GenerateCharacter(sTemp, sTemp, "man", "man", 15, PIRATE, 1, true));
                sld.dialog.Filename = "Quest\sailors_dialog.c";
	            sld.dialog.currentnode = "Morgan_wait_you";
                LAi_SetWarriorType(sld);
                LAi_warrior_DialogEnable(sld, true);
                LAi_SetLoginTime(sld, 0.0, 24.0);
                LAi_SetHP(sld, 100.0, 100.0);
                ChangeCharacterAddressGroup(sld, "Ship_deck", "goto", "goto"+i);
            }
            pchar.quest.Munity = "";  // ������� ������ � ������ (�������, � �� ����� �� ��������)
            LAi_LockFightMode(pchar, true);
		}
		else
		{   // ������� �� ���������� (������ ��������)
      		Pchar.quest.PQ8_PanamaTimerOver_01.win_condition.l1 = "MapEnter";
            Pchar.quest.PQ8_PanamaTimerOver_01.function = "PQ8_PanamaTimerOver";
		}
	}
	else
	{   // �������� ������ �� �����, ��� ����� ������, �� �� �� �� ���
		Pchar.quest.PQ8_PanamaTimerOver_02.win_condition.l1 = "MapEnter";
        Pchar.quest.PQ8_PanamaTimerOver_02.function = "PQ8_PanamaTimerOver";
	}
}

void PQ8_EnterPortoBello()
{
	pchar.quest.PQ8_PanamaTimerOver.over = "yes"; //������� ������ 
	//bQuestDisableMapEnter = true;
}

void PQ8_MorganInPortoBello()
{
	chrDisableReloadToLocation = true; 
	DoQuestFunctionDelay("PQ8_MorganInPortoBello_2", 3.0);	
}

void PQ8_MorganInPortoBello_2(string qName)
{	
	sld = characterFromId("Henry Morgan");
	LAi_LoginInCaptureTown(sld, true); 
	LAi_SetCurHPMax(sld);
	sld.dialog.currentNode = "PL_Q8_PBResidence";
	ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
    LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", -1, 0);
}

void PQ8_PBExitResidence(string qName)
{
	iTemp = GetCharacterIndex("Henry Morgan");
	if (iTemp != -1)
	{
		if(IsCompanion(&characters[iTemp]))  
		{
			RemoveCharacterCompanion(pchar, &characters[iTemp]);
		}   				
		ChangeCharacterAddress(&characters[iTemp], "none", "");
	}
	iTemp = GetCharacterIndex("Jackman");
	if (iTemp != -1)
	{
		if(IsCompanion(&characters[iTemp]))  
		{
			RemoveCharacterCompanion(pchar, &characters[iTemp]);
		}   				
		ChangeCharacterAddress(&characters[iTemp], "none", "");
	}
	iTemp = GetCharacterIndex("John Morris");
	if (iTemp != -1)
	{
		if(IsCompanion(&characters[iTemp]))  
		{
			RemoveCharacterCompanion(pchar, &characters[iTemp]);
		}   				
		ChangeCharacterAddress(&characters[iTemp], "none", "");
	}
	sld = characterFromId("Richard_Soukins");
	sld.DeckDialogNode = "PL_Q8_deck_2";
	AddQuestRecord("Pir_Line_8_Panama", "5");
	pchar.Quest.PQ8_fromPBport.win_condition.l1 = "EnterToSea";
	pchar.Quest.PQ8_fromPBport.function = "PQ8_fromPBport";
	SetTimerFunction("PQ8_controlShore48", 0, 0, 2);
}

void PQ8_fromPBport(string qName)
{
	pchar.Quest.PQ8_controlShore48_1.win_condition.l1 = "Location_Type";
	pchar.Quest.PQ8_controlShore48_1.win_condition.l1.location_type = "town";
	pchar.Quest.PQ8_controlShore48_1.function = "PQ8_controlShore48";
	pchar.Quest.PQ8_controlShore48_2.win_condition.l1 = "Location_Type";
	pchar.Quest.PQ8_controlShore48_2.win_condition.l1.location_type = "seashore";
	pchar.Quest.PQ8_controlShore48_2.function = "PQ8_controlShore48";
}

void PQ8_controlShore48(string qName)
{
	pchar.Quest.PQ8_controlShore48.over = "yes";
	pchar.Quest.PQ8_controlShore48_1.over = "yes";
	pchar.Quest.PQ8_controlShore48_2.over = "yes";
	if (pchar.location == "Shore48")
	{	//�� ������� � �� ����� -->
		LAi_LocationFightDisable(&Locations[FindLocation("Shore48")], true); //��������� ������� ��� ������
		LocatorReloadEnterDisable("Shore48", "boat", true); //�� ���� �������� � ����
		chrDisableReloadToLocation = true; 
		bDisableLandEncounters = true; //���������� �� �������
		pchar.questTemp.piratesLine = "Panama_inShore";
		pchar.questTemp.piratesLine.crewHero = makeint(GetCrewQuantity(pchar) / 100 / 2);	//�������� ���-�� ������� �� ��
		if (sti(pchar.questTemp.piratesLine.crewHero) > 5) pchar.questTemp.piratesLine.crewHero = 5;
		pchar.questTemp.piratesLine.crewRichard = 3;   //�������� ���-�� ������� �� ��������
		//����� ��. ���������������� �� ���� 3 �����������.
		SetCrewQuantity(pchar, GetMinCrewQuantity(pchar)); //����������� ������� �� ���
		iTemp = sti(pchar.questTemp.piratesLine.crewHero) ; //����� ������� ������ ��
        for (i=1; i<=iTemp; i++)
        {
            sld = GetCharacter(NPC_GenerateCharacter("MHpirate_"+i, "pirate_"+(rand(9)+1), "man", "man", sti(pchar.rank), PIRATE, -1, true));
            LAi_SetWarriorType(sld);
			TakeNItems(sld,"potion1", rand(2)+1);
			LAi_warrior_DialogEnable(sld, false);
            LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
            ChangeCharacterAddressGroup(sld, "Shore48", "goto",  "goto12");
        }	
        for (i=1; i<=iTemp; i++) //��������� ��
        {
            sld = GetCharacter(NPC_GenerateCharacter("MHmush_"+i, "mushketer_"+(rand(4)+1), "man", "mushketer", sti(pchar.rank), PIRATE, -1, false));
            LAi_SetWarriorType(sld);
			TakeNItems(sld,"potion1", rand(2)+1);
			LAi_warrior_DialogEnable(sld, false);
            LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
            ChangeCharacterAddressGroup(sld, "Shore48", "goto",  "goto12");
        }
		//����� ��������
		iTemp = sti(pchar.questTemp.piratesLine.crewRichard) ; //����� ������� ������ ��������
        for (i=1; i<=iTemp; i++)
        {
            sld = GetCharacter(NPC_GenerateCharacter("RSpirate_"+i, "pirate_"+(rand(9)+1), "man", "man", 25, PIRATE, -1, true));
            LAi_SetWarriorType(sld);
			TakeNItems(sld,"potion1", rand(2)+1);
			LAi_warrior_DialogEnable(sld, false);
            LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
            ChangeCharacterAddressGroup(sld, "Shore48", "goto",  "goto2");
		}
        for (i=1; i<=iTemp; i++) //��������� ��������
        {
            sld = GetCharacter(NPC_GenerateCharacter("RSmush_"+i, "mushketer_"+(rand(4)+1), "man", "mushketer", 25, PIRATE, -1, false));
            LAi_SetWarriorType(sld);
			LAi_warrior_DialogEnable(sld, false);
			TakeNItems(sld,"potion1", rand(2)+1);
            LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
            ChangeCharacterAddressGroup(sld, "Shore48", "goto",  "goto2");
		}
		//������� ��������
        for (i=1; i<=3; i++)
        {
            sld = GetCharacter(NPC_GenerateCharacter("RSofficer_"+i, "officer_"+(rand(9)+1), "man", "man", 30, PIRATE, -1, true));
            FantomMakeCoolFighter(sld, 30, 90, 90, "blade28", "pistol4", 100);
			LAi_SetWarriorType(sld);
			LAi_warrior_DialogEnable(sld, false);
            LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
            ChangeCharacterAddressGroup(sld, "Shore48", "goto",  "goto4");
		}
		//��� �������
		sld = characterFromId("Richard_Soukins");
		sld.dialog.currentNode = "Shore48";
		LAi_SetImmortal(sld, false);
		LAi_NoRebirthEnable(sld);
		GiveItem2Character(sld, "blade23");
		EquipCharacterbyItem(sld, "blade23");
		GiveItem2Character(sld, "pistol4");
		EquipCharacterbyItem(sld, "pistol4");
		TakeNItems(sld,"potion1", 5);
		RemoveCharacterCompanion(pchar, sld); //������� �������� �� �����������
		ChangeCharacterAddressGroup(sld, "Shore48", "goto", "goto11");	
		LAi_SetActorType(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
		LAi_ActorDialog(sld, pchar, "", -1.0, 0);
		//���������� �� ���� � �������
		pchar.Quest.PQ8_jungle_05.win_condition.l1 = "location";
		pchar.Quest.PQ8_jungle_05.win_condition.l1.location = "Panama_jungle_05";
		pchar.Quest.PQ8_jungle_05.function = "PQ8_jungle_05";
	}
	else
	{	//�� �� ����� ��� �� ���� ����� -->
		iTemp = GetCharacterIndex("Richard_Soukins");
		if (iTemp != -1)
		{
			if(IsCompanion(&characters[iTemp]))  
			{
				RemoveCharacterCompanion(pchar, &characters[iTemp]);
			}                   
			LAi_SetHuberType(&characters[iTemp]);
			LAi_group_MoveCharacter(&characters[iTemp], "PIRATE_CITIZENS");
            ChangeCharacterAddressGroup(&characters[iTemp], "PuertoPrincipe_townhall", "sit", "sit1");
            characters[iTemp].dialog.currentnode = "First time";
        }
        //���������� �� ����� ��������
		iTemp = GetCharacterIndex("Jackman");
		if (iTemp != -1)
		{              
			LAi_SetHuberType(&characters[iTemp]);
			LAi_group_MoveCharacter(&characters[iTemp], "PIRATE_CITIZENS");
            ChangeCharacterAddressGroup(&characters[iTemp], "Pirates_townhall", "sit", "sit1");
            characters[iTemp].dialog.currentnode = "First time";
        }
		AddQuestRecord("Pir_Line_8_Panama", "6");
		CloseQuestHeader("Pir_Line_8_Panama");
		SetTimerFunction("PQ8_openPanama", 0, 0, 40);
		LocatorReloadEnterDisable("Shore48", "reload1_back", true); //�������� ������� � ������
		LocatorReloadEnterDisable("PortoBello_Jungle_01", "reload1_back", true);
		pchar.questTemp.piratesLine = "over";
	}
}

void PQ8_jungle_05(string qName)
{ 
	chrDisableReloadToLocation = true;
	pchar.questTemp.piratesLine = "Panama_inJungle";
	//--------------------------- ���� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc5");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc6");
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc7");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc8");
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc"+i);
		}
	}
	//��� �������
	iTemp = GetCharacterIndex("Richard_Soukins");
	if(iTemp != -1 && !LAi_IsDead(&characters[iTemp]))
	{
		sld = &characters[iTemp];
		LAi_SetCurHPMax(sld);
		LAi_SetStayType(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		ChangeCharacterAddressGroup(sld, pchar.location, "rld", "loc0");
	}
    //--------------------------- ������� ------------------------------
    int Rank = sti(pchar.rank) - 5 + MOD_SKILL_ENEMY_RATE;
	if (Rank < 1) Rank = 1;
	for (i=12; i<=18; i++) //max - 21
    {
        sTemp = "sold_spa_"+(rand(7)+1);
		if (i==16) sTemp = "off_spa_2";
 		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, sTemp, "man", "man", Rank, SPAIN, -1, true));       
		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, "sold_spa_"+(rand(7)+1), "man", "man", sti(pchar.rank), SPAIN, -1, true));
		FantomMakeCoolFighter(sld, sti(pchar.rank), 80, 80, "blade34", "pistol6", 80);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
    }
    for (i=1; i<=8; i++) //max - 11
    {
        sld = GetCharacter(NPC_GenerateCharacter("SpaMush_"+i, "spa_mush_"+(rand(2)+1), "man", "mushketer", Rank, SPAIN, -1, false));
		LAi_SetWarriorType(sld);
		if (MOD_SKILL_ENEMY_RATE > 5) TakeNItems(sld,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
	}
	//�����������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
	LAi_group_SetCheck("EnemyFight", "PQ8_jungle_afterFight");
	pchar.Quest.PQ8_jungle_04.win_condition.l1 = "location";
	pchar.Quest.PQ8_jungle_04.win_condition.l1.location = "Panama_jungle_04";
	pchar.Quest.PQ8_jungle_04.function = "PQ8_jungle_04";
}


void PQ8_jungle_04(string qName)
{ 
	chrDisableReloadToLocation = true;
	pchar.questTemp.piratesLine = "Panama_inJungle";
	//--------------------------- ���� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc5");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc6");
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc7");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc8");
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc"+i);
		}
	}
	//��� �������
	iTemp = GetCharacterIndex("Richard_Soukins");
	if(iTemp != -1 && !LAi_IsDead(&characters[iTemp]))
	{
		sld = &characters[iTemp];
		LAi_SetCurHPMax(sld);
		LAi_SetStayType(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		ChangeCharacterAddressGroup(sld, pchar.location, "rld", "loc0");
	}
    //--------------------------- ������� ------------------------------
    int Rank = sti(pchar.rank) - 5 + MOD_SKILL_ENEMY_RATE;
	if (Rank < 1) Rank = 1;
	for (i=11; i<=13; i++)
    {
        sTemp = "sold_spa_"+(rand(7)+1);
		if (i==14) sTemp = "off_spa_1";
 		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, sTemp, "man", "man", Rank, SPAIN, -1, true));      
		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, "sold_spa_"+(rand(7)+1), "man", "man", Rank, SPAIN, -1, true));
        FantomMakeCoolFighter(sld, sti(pchar.rank), 90, 90, "blade26", "pistol4", 100);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
    }
	for (i=14; i<=17; i++) //�������
    {
        if (i==14 || i==16) sTemp = "indsair2";
		else sTemp = "indsar1";
		sld = GetCharacter(NPC_GenerateCharacter("IndEnemy_"+i, "indsair2", "man", "man", Rank, SPAIN, -1, true)); //to_do ��������
		FantomMakeCoolFighter(sld, Rank, 90, 90, "topor2", "", 5);
        LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
    }
    for (i=1; i<=10; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("SpaMush_"+i, "spa_mush_"+(rand(2)+1), "man", "mushketer", Rank, SPAIN, -1, false));
		if (MOD_SKILL_ENEMY_RATE > 5) TakeNItems(sld,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
	}
	//�����������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
	LAi_group_SetCheck("EnemyFight", "PQ8_jungle_afterFight");
	pchar.Quest.PQ8_jungle_03.win_condition.l1 = "location";
	pchar.Quest.PQ8_jungle_03.win_condition.l1.location = "Panama_jungle_03";
	pchar.Quest.PQ8_jungle_03.function = "PQ8_jungle_03";
}

void PQ8_jungle_03(string qName)
{ 
	chrDisableReloadToLocation = true;
	pchar.questTemp.piratesLine = "Panama_inJungle";
	//--------------------------- ���� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc5");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc6");
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc7");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc8");
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc"+i);
		}
	}
	//��� �������
	iTemp = GetCharacterIndex("Richard_Soukins");
	if(iTemp != -1 && !LAi_IsDead(&characters[iTemp]))
	{
		sld = &characters[iTemp];
		LAi_SetCurHPMax(sld);
		LAi_SetStayType(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		ChangeCharacterAddressGroup(sld, pchar.location, "rld", "loc0");
	}
    //--------------------------- ������� ------------------------------
    int Rank = sti(pchar.rank) - 5 + MOD_SKILL_ENEMY_RATE;
	if (Rank < 1) Rank = 1;
	for (i=11; i<=16; i++)
    {
        sTemp = "sold_spa_"+(rand(7)+1);
		if (i==13) sTemp = "off_spa_2";
 		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, sTemp, "man", "man", Rank, SPAIN, -1, true));
		FantomMakeCoolFighter(sld, sti(pchar.rank), 90, 90, "blade26", "pistol4", 120);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
    }
    for (i=1; i<=10; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("SpaMush_"+i, "spa_mush_"+(rand(2)+1), "man", "mushketer", Rank, SPAIN, -1, false));
		if (MOD_SKILL_ENEMY_RATE > 5) TakeNItems(sld,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
	}
	//�����������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
	LAi_group_SetCheck("EnemyFight", "PQ8_jungle_afterFight");
	pchar.Quest.PQ8_ExitTown.win_condition.l1 = "location";
	pchar.Quest.PQ8_ExitTown.win_condition.l1.location = "Panama_ExitTown";
	pchar.Quest.PQ8_ExitTown.function = "PQ8_ExitTown";
}

void PQ8_ExitTown(string qName)
{ 
	iTemp = GetCharacterIndex("Richard_Soukins");
	if(LAi_IsDead(&characters[iTemp]))
	{	//������� �����, ������� ���������
		AddQuestRecord("Pir_Line_8_Panama", "7");	
		pchar.questTemp.piratesLine = "Panama_RichardGoodWork";
	}
	else
	{	//������� ���, ������� ���������
		AddQuestRecord("Pir_Line_8_Panama", "8");	
		pchar.questTemp.piratesLine = "Panama_RichardBadWork";
		sld = &characters[iTemp];
		LAi_SetCurHPMax(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		ChangeCharacterAddressGroup(sld, pchar.location, "rld", "loc2");
	}
	chrDisableReloadToLocation = true;
	pchar.questTemp.piratesLine = "Panama_inExitTown";
	//--------------------------- ���� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].lifeDay = 10;
			SaveCurrentNpcQuestDateParam(&characters[iTemp], "LifeTimeCreate");
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc15");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].lifeDay = 10;
			SaveCurrentNpcQuestDateParam(&characters[iTemp], "LifeTimeCreate");
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc16");
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].lifeDay = 10;
			SaveCurrentNpcQuestDateParam(&characters[iTemp], "LifeTimeCreate");
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc17");
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].lifeDay = 10;
			SaveCurrentNpcQuestDateParam(&characters[iTemp], "LifeTimeCreate");
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc18");
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].lifeDay = 10;
			SaveCurrentNpcQuestDateParam(&characters[iTemp], "LifeTimeCreate");
			LAi_group_MoveCharacter(&characters[iTemp], LAI_GROUP_PLAYER);
			ChangeCharacterAddressGroup(&characters[iTemp], pchar.location, "rld",  "loc"+(i+10));
		}
	}
    //--------------------------- ���� ������� ------------------------------
    for (i=1; i<=5; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("HMpirate_"+i, "pirate_"+(rand(9)+1), "man", "man", 25, PIRATE, 10, true));
        LAi_SetWarriorType(sld);
		LAi_warrior_DialogEnable(sld, false);
        LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "loc20");
	}
    for (i=1; i<=5; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("HMmush_"+i, "mushketer_"+(rand(4)+1), "man", "mushketer", 25, PIRATE, 10, false));
        LAi_SetWarriorType(sld);
		LAi_warrior_DialogEnable(sld, false);
        LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "loc19");
	}
    //--------------------------- ������� ------------------------------
    int Rank = sti(pchar.rank) - 5 + MOD_SKILL_ENEMY_RATE;
	if (Rank < 1) Rank = 1;
	for (i=11; i<=15; i++)
    {
        sTemp = "sold_spa_"+(rand(7)+1);
		if (i==12) sTemp = "off_spa_1";
 		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, sTemp, "man", "man", Rank, SPAIN, -1, true));
        FantomMakeCoolFighter(sld, sti(pchar.rank), 90, 90, "blade26", "pistol4", 120);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
    }
    for (i=1; i<=10; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("SpaMush_"+i, "spa_mush_"+(rand(2)+1), "man", "mushketer", Rank, SPAIN, -1, false));
		if (MOD_SKILL_ENEMY_RATE > 5) TakeNItems(sld,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);		
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "aloc"+i);
	}
	//�����������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
	LAi_group_SetCheck("EnemyFight", "PQ8_ExitTown_afterFight");
}

void PQ8_piratesInPanama(string qName)
{
	DeleteAttribute(&colonies[FindColony("PortoBello")], "notCaptured"); //����� ����� ����� ����
	//������� ���� ������ - ����������
	rCharacter = characterFromId("Panama_Mayor");
	sld = GetCharacter(NPC_GenerateCharacter("FantomMayor", rCharacter.model, "man", "man", sti(rCharacter.rank), sti(rCharacter.nation), -1, false));
	sld.quest.type = "hovernor";
	sld.City = "Panama";
	sld.location = "Panama_TownhallRoom";
	sld.greeting = "spa_gov_common";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
    sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
	sld.Dialog.CurrentNode = "PQ8_Mayor";
	LAi_SetStayType(sld);
	LAi_LoginInCaptureTown(sld, true);
	//--------------------------- ���� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_PirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_PirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_RsPirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_RsPirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_RsPirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
	}
    //--------------------------- ���� ������� ------------------------------
    for (i=1; i<=5; i++)
    {
        iTemp = GetCharacterIndex("HMpirate_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_MPirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
	}
    for (i=1; i<=5; i++)
    {
        iTemp = GetCharacterIndex("HMmush_"+i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			LAi_warrior_DialogEnable(sld, true);
			LAi_LoginInCaptureTown(sld, true);
			sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
			sld.Dialog.CurrentNode = "PQ8_MPirInPan";
			PlaceCharacter(sld, "goto", pchar.location);
		}
	}
}	

void PQ8_MorganInPanama_1(string qName)
{
	chrDisableReloadToLocation = true;
	//������ ������� ����
	sld = ItemsFromID("keyPanama");
	sld.shown = true; 
	//������� � ����� � �� ������
	GetCharacterPos(pchar, &locx, &locy, &locz);
	sld = characterFromId("Henry Morgan");
	sld.dialog.currentNode = "PL_Q8_Panama1";
	LAi_LoginInCaptureTown(sld, true);
	ChangeCharacterAddressGroup(sld, pchar.location, "goto", "goto6");
	LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
	LAi_ActorDialog(sld, pchar, "", -1, 0);
}

void PL_Q8_ResEnterBox(string qName)
{
	if (CheckCharacterItem(pchar, "keyPanama"))
    {
		TakeItemFromCharacter(pchar, "keyPanama");
		chrDisableReloadToLocation = true;
		sld = characterFromId("Henry Morgan");
		sld.dialog.currentNode = "PL_Q8_Panama2";		
		ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
		LAi_SetActorType(sld);
		LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
		LAi_ActorDialog(sld, pchar, "", -1, 0);
	}
	else
	{
		Pchar.quest.PL_Q8_ResEnterBox_2.win_condition.l1 = "ExitFromLocation";
        Pchar.quest.PL_Q8_ResEnterBox_2.win_condition.l1.location = pchar.location;
        Pchar.quest.PL_Q8_ResEnterBox_2.function = "PL_Q8_ResEnterBox_2";
	}
}

void PL_Q8_ResEnterBox_2(string qName)
{
	pchar.quest.PL_Q8_ResEnterBox.win_condition.l1 = "locator";
	pchar.quest.PL_Q8_ResEnterBox.win_condition.l1.location = "Panama_TownhallRoom";
	pchar.quest.PL_Q8_ResEnterBox.win_condition.l1.locator_group = "box";
	pchar.quest.PL_Q8_ResEnterBox.win_condition.l1.locator = "private1";
	pchar.quest.PL_Q8_ResEnterBox.function = "PL_Q8_ResEnterBox";
}

void PL_Q8_inPanamaFort2(string qName)
{
	Pchar.quest.PL_Q8_inPanamaPirates_1.win_condition.l1 = "location";
	Pchar.quest.PL_Q8_inPanamaPirates_1.win_condition.l1.location = "Panama_town";
	Pchar.quest.PL_Q8_inPanamaPirates_1.function = "PL_Q8_inPanamaPirates_1";
}

void PL_Q8_inPanamaPirates_1(string qName)
{
	chrDisableReloadToLocation = true;	
	//--------------------------- ������ ���� � ���������� ������� ------------------------------
	//����������� ������ ��
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHpirate_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewHero); i++)
    {
        iTemp = GetCharacterIndex("MHmush_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
    }
    //����������� ������ ��������
	for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSpirate_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
    }
    for (i=1; i<=sti(pchar.questTemp.piratesLine.crewRichard); i++)
    {
        iTemp = GetCharacterIndex("RSmush_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
    }
    //������� ��������
	for (i=1; i<=3; i++)
    {
        iTemp = GetCharacterIndex("RSofficer_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
	}
    //--------------------------- ���� ������� ------------------------------
    for (i=1; i<=5; i++)
    {
        iTemp = GetCharacterIndex("HMpirate_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
	}
    for (i=1; i<=5; i++)
    {
        iTemp = GetCharacterIndex("HMmush_"+i);
		if(iTemp != -1)
		{
			characters[iTemp].Dialog.CurrentNode = "PQ8_FackMorgan";
		}
	}	
	//����������� �����
	GetCharacterPos(pchar, &locx, &locy, &locz);
	sld = GetCharacter(NPC_GenerateCharacter("QuestPirate", "pirate_"+(rand(9)+1), "man", "man", 25, PIRATE, 0, true));
	sld.Dialog.Filename = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentNode = "PL_Q8_QFackMorgan";
	LAi_LoginInCaptureTown(sld, true);
	ChangeCharacterAddressGroup(sld, pchar.location, "goto", "goto10");
	LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER);
	LAi_ActorDialog(sld, pchar, "", -1, 0);
}

void PL_Q8_backFight_1(string qName) 
{
    int Rank = sti(pchar.rank) - 5 + MOD_SKILL_ENEMY_RATE;
	if (Rank < 1) Rank = 1;
	for (i=1; i<=8; i++)
    {
        sTemp = "sold_spa_"+(rand(7)+1);
		if (i==2) sTemp = "off_spa_2";
 		sld = GetCharacter(NPC_GenerateCharacter("SpaEnemy_"+i, sTemp, "man", "man", Rank, SPAIN, 12, true));
		FantomMakeCoolFighter(sld, sti(pchar.rank), 90, 90, "blade26", "pistol4", 120);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "loc1");
    }
    for (i=1; i<=4; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("SpaMush_"+i, "spa_mush_"+(rand(2)+1), "man", "mushketer", Rank, SPAIN, 12, false));
		if (MOD_SKILL_ENEMY_RATE > 5) TakeNItems(sld,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
		LAi_SetWarriorType(sld);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "rld",  "loc3");
	}
	//�����������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, false);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ��������� �������     �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////   ����������        ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ----------------- ��������� � ����������� ---------------------
void WaitressFack_null(string qName) //����� ����� ����������
{
	pchar.questTemp.different = "free";
	pchar.quest.WaitressFack_inRoom.over = "yes"; 
	DeleteAttribute(pchar, "questTemp.different.FackWaitress");
}

void WaitressFack_inRoom(string qName)
{
	chrDisableReloadToLocation = true;
	LocatorReloadEnterDisable(locations[reload_location_index].fastreload + "_tavern", "reload2_back", true);
	DoQuestFunctionDelay("WaitressFack_Enter", frand(20.0));
}

void WaitressFack_Enter(string qName)
{	
	LAi_group_Delete("EnemyFight"); 
	LAi_SetFightMode(pchar, false);
	LAi_LockFightMode(pchar, true);
	if (pchar.questTemp.different.FackWaitress.Kick == "0")
	{	//��������
		iTemp = 5 + sti(pchar.rank) + rand(MOD_SKILL_ENEMY_RATE);
		sld = GetCharacter(NPC_GenerateCharacter("BerglarWairessQuest", "officer_"+(rand(19)+1), "man", "man", iTemp, PIRATE, -1, true));
		FantomMakeCoolFighter(sld, iTemp, 80, 80, "topor2", "pistol6", 100);
		sld.dialog.Filename = "Quest\ForAll_dialog.c";
		sld.dialog.currentnode = "WaitressBerglar";		
 		//������ ������� ����������
		rCharacter = characterFromId(locations[reload_location_index].fastreload + "_waitress");
		rCharacter.model = "girl_" + (rand(7)+1);
		SetRandomNameToCharacter(rCharacter);
		FaceMaker(rCharacter);  
	}
	else
	{
        sld = characterFromId("WairessQuest");
	}
	ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload1");
	LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	LAi_ActorDialog(sld, pchar, "", 2.0, 0);
}

void WaitressFack_outRoom()
{
	// ==> �������� ������, ������� � ������.
	RemoveCharacterEquip(pchar, BLADE_ITEM_TYPE);
	RemoveCharacterEquip(pchar, GUN_ITEM_TYPE);
    while (FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE) != "")
    {
        TakeItemFromCharacter(pchar, FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE));
    }
    while (FindCharacterItemByGroup(pchar, GUN_ITEM_TYPE) != "")
    {             
        TakeItemFromCharacter(pchar, FindCharacterItemByGroup(pchar, GUN_ITEM_TYPE));
    }
    pchar.money = 0;
    // <== �������� ������, ������� � ������.
	AddCharacterExpToSkill(pchar, "Pistol", 100);
	LAi_SetActorType(sld);
	LAi_ActorGoToLocation(sld, "reload", "reload1_back", "none", "", "", "", -1.0);
	DoQuestCheckDelay("OpenTheDoors", 10.0);
	pchar.questTemp.different = "FackWaitress_noMoney"; //���� ��������
}

void WaitressFack_fight()
{
	AddCharacterExpToSkill(pchar, "FencingLight", 30);
	AddCharacterExpToSkill(pchar, "Fencing", 30);
	AddCharacterExpToSkill(pchar, "FencingHeavy", 30);
	LAi_SetFightMode(pchar, true);
	pchar.questTemp.different = "FackWaitress_fighted"; //���� �������� ���������
	LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups("EnemyFight", LAI_GROUP_PLAYER, true);
	pchar.quest.WaitressFack_afterFight.win_condition.l1 = "NPC_Death";
	pchar.quest.WaitressFack_afterFight.win_condition.l1.character = "BerglarWairessQuest";
	pchar.quest.WaitressFack_afterFight.win_condition = "OpenTheDoors";
}

void WaitressFack_fack()
{
	pchar.questTemp.HorseQty = sti(pchar.questTemp.HorseQty) + 1;
	DoQuestCheckDelay("PlaySex_1", 1.0);
	pchar.questTemp.different = "FackWaitress_facking"; 
}

// ----------------- ������������� ������� ������� ---------------------
void HostessChurch_null(string qName) //����� ����� 
{
	//���� ����� ��� ����, �� ������ ������ ������������
	if (characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch == "taken") 
	{
		characters[GetCharacterIndex(pchar.questTemp.different.HostessChurch.city + "_Hostess")].questChurch = "baster";
	}
	pchar.questTemp.different = "free";
	DeleteAttribute(pchar, "questTemp.different.HostessChurch");
}

// ----------------- ������� ������� �� ����� ---------------------
void ShipyardsMap_returnCFN(string qName) 
{
	string  sName;
	aref	arDis, arRld;
	int location_index = FindLocation(pchar.questTemp.different.ShipyardsMap.city + "_town");
    makearef(arRld, Locations[location_index].reload);
    int		Qty = GetAttributesNum(arRld);
    for (int a=0; a<Qty; a++)
    {
    	arDis = GetAttributeN(arRld, a);
		sName = arDis.go;
    	if (findsubstr(sName, "_shipyard" , 0) != -1)
    	{
			arDis.close_for_night = true;
			break;
    	}
    }
	LocatorReloadEnterDisable(pchar.questTemp.different.ShipyardsMap.city + "_shipyard", "reload2", false);
}

// ----------------- ����� ������ ���� � ������� ---------------------
void TakeMayorsRing_null(string qName) //����� ����� 
{
	pchar.questTemp.different = "free";
	if (CheckCharacterItem(pchar, "MayorsRing"))
    {
		TakeItemFromCharacter(pchar, "MayorsRing");
		AddQuestRecord("SeekMayorsRing", "4");
		AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + pchar.questTemp.different.TakeMayorsRing.city + "Gen"));
		CloseQuestHeader("SeekMayorsRing");
		ChangeCharacterReputation(pchar, -3);
	}
	else
	{
		if (pchar.questTemp.different.TakeMayorsRing == "toBrothel")
		{
			AddQuestRecord("SeekMayorsRing", "5");
			AddQuestUserData("SeekMayorsRing", "sCity", XI_ConvertString("Colony" + pchar.questTemp.different.TakeMayorsRing.city + "Gen"));
			CloseQuestHeader("SeekMayorsRing");
			ChangeCharacterReputation(pchar, -5);
		}
	}
	DeleteAttribute(pchar, "questTemp.different.TakeMayorsRing");
}

// ----------------- ������ �� ���� ������� ������� ---------------------
void SexWithHostess_null(string qName) //����� ����� 
{
	if (pchar.questTemp.different.HostessSex == "toRoom")
	{
		sld = characterFromId(pchar.questTemp.different.HostessSex.city + "_Hostess");
		ChangeCharacterAddressGroup(sld, pchar.questTemp.different.HostessSex.city + "_SecBrRoom", "goto", "goto8");
		LocatorReloadEnterDisable(pchar.questTemp.different.HostessSex.city + "_Brothel", "reload2_back", true);
	}
	LAi_SetOwnerTypeNoGroup(sld);
	sld.dialog.currentnode = "First time";
	sld.quest.NotGoneToSex = true; //�� ������. ����� ������ �� �����
	DeleteAttribute(pchar, "questTemp.different.HostessSex");
	pchar.questTemp.different = "free";
}

void SexWithHostess_goToRoom()  
{
	chrDisableReloadToLocation = true;
	sld = characterFromId(pchar.questTemp.different.HostessSex.city + "_Hostess");
	LAi_SetActorTypeNoGroup(sld);
	LAi_ActorGoToLocation(sld, "reload", "reload1", pchar.questTemp.different.HostessSex.city + "_Brothel_room", "goto", "goto3", "OpenTheDoors", -1);
	pchar.quest.SexWithHostess_inRoom.win_condition.l1 = "location";
	pchar.quest.SexWithHostess_inRoom.win_condition.l1.location = pchar.questTemp.different.HostessSex.city + "_Brothel_room";
	pchar.quest.SexWithHostess_inRoom.function = "SexWithHostess_inRoom";
	LocatorReloadEnterDisable(pchar.questTemp.different.HostessSex.city + "_Brothel", "reload2_back", false);
}

void SexWithHostess_inRoom(string qName)
{
	DoQuestFunctionDelay("SexWithHostess_inRoom_2", 1.5);
	LocatorReloadEnterDisable(pchar.questTemp.different.HostessSex.city + "_Brothel", "reload2_back", true);
}

void SexWithHostess_inRoom_2(string qName)
{
	sld = characterFromId(pchar.questTemp.different.HostessSex.city + "_Hostess");
	sld.dialog.currentnode = "Hostess_inSexRoom";
	LAi_SetActorTypeNoGroup(sld);
	LAi_ActorDialog(sld, pchar, "", 0, 0);
}

void SexWithHostess_fack()
{
	pchar.questTemp.HorseQty = sti(pchar.questTemp.HorseQty) + 1;
	DoQuestCheckDelay("PlaySex_1", 1.0);
	pchar.questTemp.different = "HostessSex";
}

// ----------------- ���������� ��������  ---------------------
void SetCapitainFromCityToSea(string qName) //�������� � ���� ����, ������� ���� ��������� � ������
{
	sld = characterFromId(pchar.quest.(qName).CapId);
	sld.location	= "none";
	sld.location.group = "";
	sld.location.locator = "";
	//� ������� ������ ����
	string sGroup = "PorpmansShip_" + sld.index;
	group_DeleteGroup(sGroup);
	Group_FindOrCreateGroup(sGroup);
	Group_SetType(sGroup,"trade");
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 
	//���������� ������ � ��������� ����
	sld.quest = "InMap"; //������ ���� ����������� ����
	sld.quest.targetCity = SelectNotEnemyColony(sld); //��������� �������, ���� ���������� ���
	sld.quest.stepsQty = sti(sld.quest.stepsQty) + 1; //���������� ������� � ����
	//��������� �����, ���� ������� ���������. ����� ����� �� ��������� ����� �� � ������
	sTemp = GetArealByCityName(sld.city);
	//�� �����
	iTemp = GetMaxDaysFromIsland2Island(sTemp, GetArealByCityName(sld.quest.targetCity))+5; //���� ������� ���� � �������
	Map_CreateTrader(GetIslandRandomShoreId(sTemp), sld.quest.targetCity, sld.id, iTemp);
	//���� ����� ����, ��� ��� ���� � ������ �����
	AddSimpleRumourEx(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �������� ����� " + GetFullName(sld) + ", ����� ���� � ����. �� ������, �� �������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "'? ��� ���, �� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"���� ��� ����� ������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ��� �������� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ". " + GetFullName(sld) + " ���� ������ ����."), 
		sld.city, iTemp, 1, "PortmansBook_DeliveryToCap", sld.id);
	//--> ������ ���� �� ���� � ���� �������� ��������
	sTemp = sld.id; //Id ����, ������� ������� �������
	rCharacter = &characters[GetCharacterIndex(sld.City + "_PortMan")];
	rCharacter.quest.capitainsList.(sTemp) = sld.quest.targetCity; //���� ����������
	rCharacter.quest.capitainsList.(sTemp).date = GetDateString(); //������� ����, ����� ����������
	//��������. � �������� ������ ��������� ������� ������ �� �������   PortmansBook_Delivery    #TEXT   5
	//� ������ �������� ������� �����, ��������� � ����� ������ �� quest_text.txt
	rCharacter.quest.capitainsList.(sTemp).QBString1 = characters[GetCharacterIndex(sld.quest.firstCity + "_PortMan")].id + "PortmansBook_Delivery";
	rCharacter.quest.capitainsList.(sTemp).QBString2 = "PortmansBook_Delivery";
	rCharacter.quest.capitainsList.(sTemp).QBQty = 5;
}

void SetRobberFromSeaToMap(string qName) //�������� � ���� ����-����, ������� ���� ����� �� ����� � �����
{
	sld = characterFromId(pchar.quest.(qName).CapId);
	//� ������� ������ ����
	string sGroup = "SeekCapShip_" + sld.index;
	group_DeleteGroup(sGroup);
	Group_FindOrCreateGroup(sGroup);
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 
	//���������� ������ � ��������� ����
	sld.quest = "InMap"; //������ ���� ����������� ����
	sld.quest.targetCity = SelectAnyColony2(sld.city, sld.quest.cribCity); //��������� �������, ���� ���������� ���
	Log_TestInfo("���-��� " + GetFullName(sld) + " ����� ��: " + sld.city + " � ���������� �: " + sld.quest.targetCity);
	//�� �����
	iTemp = GetMaxDaysFromIsland2Island(GetArealByCityName(sld.quest.targetCity), GetArealByCityName(sld.city))+5; //���� ������� ���� � �������
	Map_CreateTrader(sld.city, sld.quest.targetCity, sld.id, iTemp);
	//���� ����� ����, ��� ��� ���� � ������ �����
	AddSimpleRumourEx(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �������� ����� " + GetFullName(sld) + ", ����� ���� � ����. �� ������, �� �������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "'? ��� ���, �� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"���� ��� ����� ������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ��� �������� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ". " + GetFullName(sld) + " ���� ������ ����."), 
		sld.city, iTemp, 1, "Portmans_SeekShip_rum", sld.id);
	//--> ������ ���� �� ���� � ���� �������� ��������
	sTemp = sld.id; //Id ����, ������� ������� �������
	rCharacter = &characters[GetCharacterIndex(sld.City + "_PortMan")];
	rCharacter.quest.capitainsList.(sTemp) = sld.quest.targetCity; //���� ����������
	rCharacter.quest.capitainsList.(sTemp).date = GetDateString(); //������� ����, ����� ����������
	//��������. � �������� ������ ��������� ������� ������ �� �������   PortmansBook_Delivery    #TEXT   5
	//� ������ �������� ������� �����, ��������� � ����� ������ �� quest_text.txt
	rCharacter.quest.capitainsList.(sTemp).QBString1 = characters[GetCharacterIndex(sld.quest.cribCity + "_PortMan")].id + "Portmans_SeekShip";
	rCharacter.quest.capitainsList.(sTemp).QBString2 = "Portmans_SeekShip";
	rCharacter.quest.capitainsList.(sTemp).QBQty = 2;
}

void SeekShip_checkAbordage(string qName) //���-��� ������� �����������
{	
	//�������, ���� ������� ���� ��� ��������
	sld = characterFromId(pchar.quest.(qName).CapId);
	rCharacter = &characters[GetCharacterIndex(sld.quest.cribCity + "_PortMan")];
	bOk = false;
	for (i=0; i<=COMPANION_MAX; i++)
	{
		iTemp = GetCompanionIndex(pchar, i);
		if(iTemp != -1)
		{
			sld = &characters[iTemp];
			if (sld.ship.name == rCharacter.quest.PortmansSeekShip.shipName && 
				RealShips[sti(sld.ship.type)].BaseName == rCharacter.quest.PortmansSeekShip.shipTapeName &&
				RealShips[sti(sld.Ship.Type)].basetype == rCharacter.quest.PortmansSeekShip.shipTape)
			{
				bOk = true;
			}
		}
	}	
	//������� �� ����������� ��������
	sld = characterFromId(pchar.quest.(qName).CapId);
	sTemp = "SeekShip_checkSink" + rCharacter.index;
	pchar.quest.(sTemp).over = "yes"; //������� ���������� �� ������
	if (bOk)
	{		
		rCharacter.quest = "SeekShip_success"; //���� ��������� ������ �� �������
		sTemp = rCharacter.id + "Portmans_SeekShip";
		AddQuestRecordEx(sTemp, "Portmans_SeekShip", "5");
	}
	else
	{
		rCharacter.quest = "SeekShip_sink"; //���� ��������� �����, ������ ������
		sTemp = rCharacter.id + "Portmans_SeekShip";
		AddQuestRecordEx(sTemp, "Portmans_SeekShip", "9");
	}
}

void SeekShip_checkSink(string qName)
{
	sld = characterFromId(pchar.quest.(qName).CapId);
	rCharacter = &characters[GetCharacterIndex(sld.quest.cribCity + "_PortMan")];
	rCharacter.quest = "SeekShip_sink"; //���� ��������� �����, ������ ������
	sTemp = rCharacter.id + "Portmans_SeekShip";
	AddQuestRecordEx(sTemp, "Portmans_SeekShip", "8");
	sTemp = "SeekShip_checkAbordage" + rCharacter.index;
	pchar.quest.(sTemp).over = "yes"; //������� ���������� �� �������
}

// ----------------- ������ ������ � ������ ---------------------
void ChurchNightGuard_exitFromLoc(string qName)
{
	pchar.quest.ChurchNightGuard_timerInChurch.over = "yes"; //������� ������ �� ��������� ��������
	if (sti(pchar.questTemp.different.Church_NightGuard.NightQty) == 0) pchar.questTemp.different.Church_NightGuard.NightQty = 1;
	else pchar.questTemp.different.Church_NightGuard.NightQty = sti(pchar.questTemp.different.Church_NightGuard.NightQty) + GetQuestPastDayParam("questTemp.different.Church_NightGuard");
	Log_TestInfo("���-�� ����� � ������: " + pchar.questTemp.different.Church_NightGuard.NightQty);
	if (sti(pchar.questTemp.different.Church_NightGuard.NightQty) >= 3)
	{
		sTemp = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
		AddQuestRecordEx(sTemp, "Church_NightGuard", "4");
	}
	else
	{
		if (GetTime() > 6.0 && GetTime() < 23.99)
		{
			SaveCurrentQuestDateParam("questTemp.different.Church_NightGuard"); //����������� ����
		}
		else
		{
			sTemp = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
			AddQuestRecordEx(sTemp, "Church_NightGuard", "3");
			CloseQuestHeader(sTemp);
			DeleteAttribute(pchar, "questTemp.different.Church_NightGuard"); //������� ����
			ChangeCharacterReputation(pchar, -20);
		}
	}
}

void ChurchNightGuard_timerInChurch(string qName)
{
	int iRank, num;
	if (pchar.questTemp.different.Church_NightGuard.chance == pchar.questTemp.different.Church_NightGuard.NightQty)
	{	//��������� �����
		pchar.quest.ChurchNightGuard_exitFromLoc.over = "yes"; //������� ���������� �� ����� �� �������
		pchar.questTemp.different.Church_NightGuard.NightQty = 3; //�������� �������� ���� ���������
		pchar.questTemp.different.Church_NightGuard.fight = true; //���� ������
        chrDisableReloadToLocation = true; // ������� ����� �� �������.
		sTemp = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
		AddQuestRecordEx(sTemp, "Church_NightGuard", "5");		
        LAi_group_Delete("EnemyFight");
		//--> ������� ���� 
		num = rand(2)+1;
		if (sti(pchar.rank) > 6) 
		{
			if (sti(pchar.rank) > 20) iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*2.5/num);
			else iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE*1.6/num);
		}
		else  
		{	//�������� ������� ���� �� ������ ����
			if (sti(pchar.rank) > 3) iRank = sti(pchar.rank);
			else 
			{
				iRank = 1;
				num = 1;
			}
		}	
		//<-- ������� ���� 		
		for (i=1; i<=num; i++)
        {
            sld = GetCharacter(NPC_GenerateCharacter("Bandit"+i, "officer_"+(i+10), "man", "man", iRank, PIRATE, 0, true));
			SetFantomParamFromRank(sld, iRank, true);         
			LAi_SetWarriorType(sld); 
			LAi_warrior_SetStay(sld, true);
			LAi_warrior_DialogEnable(sld, false);
            LAi_group_MoveCharacter(sld, "EnemyFight");
            ChangeCharacterAddressGroup(sld, pchar.location, "reload",  "reload1");
        }
		sld.Dialog.Filename = "Quest\ForAll_dialog.c";
		sld.dialog.currentnode = "ChurchBerglar";
        LAi_SetActorTypeNoGroup(sld);
		LAi_ActorDialog(sld, pchar, "", 1.0, 0);
	}
	else
	{	//���� ������
        SetLaunchFrameFormParam("������� ����...", "ChurchNightGuard_timerInChurch_2", 0, 3);
        LaunchFrameForm();
        RecalculateJumpTable();
	}
}
// ----------------- ����� �����, ���� ������ ����������� ---------------------
void SCQ_seekCapIsDeath(string qName)
{	
	string sTitle;
	sld = characterFromId(pchar.quest.(qName).CapId); //�������
	rCharacter = &characters[GetCharacterIndex("QuestCitiz_" + sld.quest.cribCity)]; //���������-������������
	if (rCharacter.quest.SeekCap == "manSlave")
	{
		sTitle = sld.quest.cribCity + "SCQ_" + rCharacter.quest.SeekCap;
		AddQuestRecordEx(sTitle, "SCQ_" + rCharacter.quest.SeekCap, "4");
		AddQuestUserData(sTitle, "sCapName", GetFullName(sld));
		AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + sld.city + "Acc"));
		rCharacter.quest.SeekCap = rCharacter.quest.SeekCap + "over"; //��������� ���� ����� �� '��������'
	}
	if (sld.quest.SeekCap == "womanRevengeFight") //�������� ���� ��� �� �����, ��� ����� ������
	{
		sTitle = sld.quest.cribCity + "SCQ_" + rCharacter.quest.SeekCap;
		AddQuestRecordEx(sTitle, "SCQ_" + rCharacter.quest.SeekCap, "5");
		AddQuestUserData(sTitle, "sCapName", GetFullName(sld));
		AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + rCharacter.city + "Acc"));
		rCharacter.quest.SeekCap = rCharacter.quest.SeekCap + "over"; //��������� ���� ����� �� '��������'
	}
	if (sld.quest.SeekCap == "womanHasband" || rCharacter.quest.SeekCap == "manRapeWife" || sld.quest.SeekCap == "manFriend" || sld.quest.SeekCap == "womanRevenge" || rCharacter.quest.SeekCap == "womanPirates")
	{
		sTitle = sld.quest.cribCity + "SCQ_" + sld.quest.SeekCap;
		AddQuestRecordEx(sTitle, "SCQ_" + sld.quest.SeekCap, "4");
		sld.lifeDay = 0;
		Map_ReleaseQuestEncounter(sld.id);
		string sGroup = "SeekCapShip_" + sld.index;
		group_DeleteGroup(sGroup);
		sTemp = "SecondTimer_" + sld.id;
		pchar.quest.(sTemp).over = "yes"; //������� �������� ������������� ������
		sTemp = "SCQ_" + rCharacter.index;
		pchar.quest.(sTemp).over = "yes"; //������� ���������� ������ ������������
		sTitle = sld.quest.cribCity + "SCQ_" + sld.quest.SeekCap;
		CloseQuestHeader(sTitle);
	}
}
//������ ������������ ��������� � ������ �����
void SCQ_CitizenIsDeath(string qName)
{
	sld = characterFromId(pchar.quest.(qName).CapId); //�������	
	sld.lifeDay = 0;
	rCharacter = characterFromId(pchar.quest.(qName).CitizenId); //���������-������������
	Map_ReleaseQuestEncounter(sld.id);
	string sGroup = "SeekCapShip_" + sld.index;
	group_DeleteGroup(sGroup);
	sTemp = "SecondTimer_" + sld.id;
	pchar.quest.(sTemp).over = "yes"; //������� �������� ������������� ������
	sTemp = "SCQ_" + sld.index;
	pchar.quest.(sTemp).over = "yes"; //������� ���������� ������ ����
	string sTitle = sld.quest.cribCity + "SCQ_" + rCharacter.quest.SeekCap;
	CloseQuestHeader(sTitle);
}

void CitizCapFromSeaToMap(string qName) //�������� �� ����� ����, �������������� ����������
{
	sld = characterFromId(pchar.quest.(qName).CapId);
	sld.nation = sld.quest.nation;
	//� ������� ������ ����
	string sGroup = "SeekCapShip_" + sld.index;
	group_DeleteGroup(sGroup);
	Group_FindOrCreateGroup(sGroup);
	Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
	Group_LockTask(sGroup);
	Group_AddCharacter(sGroup, sld.id);
	Group_SetGroupCommander(sGroup, sld.id);
	SetRandGeraldSail(sld, sti(sld.Nation)); 
	//���������� ������ � ��������� ����
	sld.quest = "InMap"; //������ ���� �������� ����
	sld.quest.targetCity = SelectAnyColony2(sld.city, sld.quest.cribCity); //��������� �������, ���� ���������� ���
	Log_TestInfo("������� ��� " + GetFullName(sld) + " ����� ��: " + sld.city + " � ���������� �: " + sld.quest.targetCity);
	//�� �����
	iTemp = GetMaxDaysFromIsland2Island(GetArealByCityName(sld.quest.targetCity), GetArealByCityName(sld.city))+5; //���� ������� ���� � �������
	Map_CreateTrader(sld.city, sld.quest.targetCity, sld.id, iTemp);
	//���� ����� ����, ��� ��� ���� � ������ �����
	AddSimpleRumourEx(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �������� ����� " + GetFullName(sld) + ", ����� ���� � ����. �� ������, �� �������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "'? ��� ���, �� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ".", 
		"���� ��� ����� ������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ��� �������� ���������� � " + XI_ConvertString("Colony"+sld.quest.targetCity+"Acc") + ". " + GetFullName(sld) + " ���� ������ ����."), 
		sld.city, iTemp, 1, "Citiz_SeekCap_rum", sld.id);
	//--> ������ ���� �� ���� � ���� �������� ��������
	sTemp = sld.id; //Id ����, ������� ������� �������
	rCharacter = &characters[GetCharacterIndex(sld.City + "_PortMan")];
	rCharacter.quest.capitainsList.(sTemp) = sld.quest.targetCity; //���� ����������
	rCharacter.quest.capitainsList.(sTemp).date = GetDateString(); //������� ����, ����� ����������
	//��������. � �������� ������ ��������� ������� ������ �� �������   PortmansBook_Delivery    #TEXT   5
	//� ������ �������� ������� �����, ��������� � ����� ������ �� quest_text.txt
	rCharacter.quest.capitainsList.(sTemp).QBString1 = sld.quest.cribCity + "SCQ_" + characters[GetCharacterIndex("QuestCitiz_"+sld.quest.cribCity)].quest.SeekCap;
	rCharacter.quest.capitainsList.(sTemp).QBString2 = "SCQ_" + characters[GetCharacterIndex("QuestCitiz_"+sld.quest.cribCity)].quest.SeekCap;
	rCharacter.quest.capitainsList.(sTemp).QBQty = 2;
}

//����� ������ � �����. ��� ���� ������� �� ������� ����� ����������
void CitizSeekCap_afterCabinFight(string qName)
{	
	sld = GetCharacter(NPC_GenerateCharacter(pchar.quest.(qName).label + "_" + pchar.quest.(qName).WifeCity, pchar.quest.(qName).model, "woman", "towngirl", 5, pchar.quest.(qName).nation, -1, false));
	sld.name = pchar.quest.(qName).WifeName;
	sld.lastname = pchar.quest.(qName).WifeLastname;
	sld.dialog.filename   = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = pchar.quest.(qName).label + "_Board";
	sld.quest.SeekCap = pchar.quest.(qName).label;
	sld.quest.cribCity = pchar.quest.(qName).WifeCity;
	LAi_SetStayType(sld);
	GetCharacterPos(pchar, &locx, &locy, &locz);
	ChangeCharacterAddressGroup(sld, pchar.location, "rld", LAi_FindFarLocator("rld", locx, locy, locz));
	LAi_SetActorType(pchar);
    LAi_SetActorType(sld);
    SetActorDialogAny2Pchar(sld.id, "", 0.0, 0.0);
	LAi_ActorFollow(pchar, sld, "ActorDialog_Any2Pchar", -1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////   ����������        �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������� ���           ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void ISS_MurderSignExecute(string qName)
{
	pchar.questTemp.LSC.killCost = sti(pchar.questTemp.LSC.killCost)-1;
	pchar.questTemp.LSC = "PoorMurderBegin";
	if (sti(pchar.questTemp.LSC.killCost) <= 0)
	{
		pchar.questTemp.LSC = "PoorMurderExecute";
		DeleteAttribute(pchar, "questTemp.LSC.killCost");
		SetQuestHeader("ISS_PoorsMurder");
		AddQuestRecord("ISS_PoorsMurder", "2");
	}
}

void LSC_enterStuvesantBox(string qName)
{
	AddQuestRecord("ISS_PoorsMurder", "5");
	pchar.questTemp.LSC = "toVillemstadTavern";
	//������� ������� � �������
	FreeSitLocator("Villemstad_tavern", "sit1");
	sld = GetCharacter(NPC_GenerateCharacter("GiveKeyMan", "shipowner_10", "man", "man", 10, HOLLAND, -1, false));
	sld.name = "����";
	sld.lastname = "������";
	sld.city = "Villemstad";
	sld.location	= "Villemstad_tavern";
	sld.location.group = "sit";
	sld.location.locator = "sit1";
	sld.dialog.filename   = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode   = "GiveKeyMan";
	LAi_SetSitType(sld);
	LAi_RemoveLoginTime(sld);
	LAi_group_MoveCharacter(sld, "HOLLAND_CITIZENS");
}

void LSC_openMasterHouse(string qName)
{	
	LocatorReloadEnterDisable("Villemstad_town", "houseSp5", false);
	//������ ������� ����
	sld = ItemsFromID("keyQuestLSC");
	sld.shown = true; 
	pchar.quest.LSC_enterMasterHouse.win_condition.l1 = "location";
	pchar.quest.LSC_enterMasterHouse.win_condition.l1.location = "Villemstad_houseSp5";
	pchar.quest.LSC_enterMasterHouse.function = "LSC_enterMasterHouse";	
}

void LSC_enterMasterHouse(string qName)
{
	chrDisableReloadToLocation = true;
	LAi_LocationFightDisable(&Locations[FindLocation("Villemstad_houseSp5")], true);
	//������ ������� ������
	sld = characterFromId("GiveKeyMan");
	sld.dialog.currentnode = "GiveKeyManInside";
	LAi_SetActorType(sld);
	ChangeCharacterAddressGroup(sld, "Villemstad_houseSp5", "barmen", "bar1");
	LAi_ActorDialog(sld, pchar, "", 1.5, 0);
}

void LSC_enterSoldiers()
{	
	LAi_group_Delete("EnemyFight"); 
	pchar.questTemp.LSC = "CanFoundStuvesantKey";
	sld = characterFromId("GiveKeyMan"); 
	LAi_CharacterDisableDialog(sld);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_TmpEnemy);
	LAi_group_SetRelation(LAI_GROUP_TmpEnemy, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	iTemp = sti(pchar.rank) + MOD_SKILL_ENEMY_RATE;
	for (i=1; i<=1+MOD_SKILL_ENEMY_RATE; i++)
    {
        sld = GetCharacter(NPC_GenerateCharacter("MKSold"+i, "sold_hol_"+(rand(7)+1), "man", "man", iTemp, HOLLAND, 0, true));
		SetFantomParamFromRank(sld, iTemp, true);         
		LAi_SetWarriorType(sld); 
		//LAi_warrior_SetStay(sld, true);
		LAi_warrior_DialogEnable(sld, false);
        LAi_group_MoveCharacter(sld, "EnemyFight");
        ChangeCharacterAddressGroup(sld, pchar.location, "goto",  "goto3");
    }
    sld = GetCharacter(NPC_GenerateCharacter("MKOfficer", "off_hol_2", "man", "man", iTemp, HOLLAND, 0, true));
	SetFantomParamFromRank(sld, iTemp, true);
	sld.Dialog.Filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "MK_HollOfficer";
    LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	ChangeCharacterAddressGroup(sld, pchar.location, "reload",  "reload1");
	LAi_ActorDialog(sld, pchar, "", 1.0, 0);
}

void FirstLoginLostShipsCity(string qName) //�������������� ��������� ��� � ���
{
	//��������� ���������
	pchar.questTemp.LSC = "AdmiralIsWaiting";
	bDisableFastReload = true; //������� �������
	//�� ������ ������ �������� ������ ���� -->
	TakeItemFromCharacter(pchar, "letter_LSC");
	ChangeItemName("letter_LSC", "itmname_letter_LSC_2");
	ChangeItemDescribe("letter_LSC", "itmdescr_letter_LSC_2");
	i = FindLocation("LostShipsCity_town");
	locations[i].private11.items.letter_LSC = 1;
	//<-- �� ������ ������ �������� ������ ����
	CloseQuestHeader("ISS_PoorsMurder");
	sld = characterFromId("hol_guber"); //����� ��� ����������
	DeleteAttribute(sld, "notMoveAble");
	DeleteAttribute(&colonies[FindColony("Villemstad")], "notCaptured"); //���������� ����� ����
	//������ ������ -->
	LAi_group_Delete("PearlGroup_1"); //��� ������ ������� ���� ������ �����������
	LAi_group_Register("PearlGroup_1");
	LAi_group_SetRelation("PearlGroup_1", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_SetLookRadius("PearlGroup_1", 50);
	LAi_group_SetHearRadius("PearlGroup_1", 20);
	LAi_group_SetSayRadius("PearlGroup_1", 20);
	LAi_group_SetPriority(LAI_GROUP_PLAYER, 0);
	LAi_group_Delete("PearlGroup_2");
	LAi_group_Register("PearlGroup_2");
	LAi_group_SetRelation("PearlGroup_2", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
	LAi_group_SetLookRadius("PearlGroup_2", 50);
	LAi_group_SetHearRadius("PearlGroup_2", 20);
	LAi_group_SetSayRadius("PearlGroup_2", 20);
	LAi_group_SetPriority(LAI_GROUP_PLAYER, 0);
	//<-- ������ ������
	//����� ����� � ������� ���
	reload_cur_island_index = FindIsland("LostShipsCity"); //to_do, � ����� ������
	DeleteAttribute(&Islands[reload_cur_island_index], "alwaysStorm");
	DeleteAttribute(&Islands[reload_cur_island_index], "storm");
	DeleteAttribute(&Islands[reload_cur_island_index], "tornado");
	DeleteAttribute(&Islands[reload_cur_island_index], "QuestlockWeather");
	DeleteAttribute(&Islands[reload_cur_island_index], "MaxSeaHeight");
	//������ ��� ������� � �����������
	int cn;
	for (i=0; i<=COMPANION_MAX; i++)
	{
		cn = GetCompanionIndex(pchar, i);
		if(cn != -1)
		{
			sld = &characters[cn];
			if (i == 0)
			{	//������� ������� ��
				DeleteAttribute(pchar, "ship");
				pchar.ship.name = "";
				pchar.ship.type = SHIP_NOTUSED;
				SetCharacterShipLocation(pchar, "");
			}
			else
			{
				RemoveCharacterCompanion(pchar, sld);
				sld.lifeDay = 0;
			}
		}
	}
	//������ ���� ���������� � ��������
	for (i=0; i<=PASSENGERS_MAX; i++)
	{
		cn = GetPassenger(pchar, i);
		if(cn != -1)
		{
			sld = &characters[cn];
			RemovePassenger(pchar, sld);
			sld.lifeDay = 0;
		}
	}
	//�������� �� �����, ����� �� ����� �� �����
	PChar.quest.LSC_stormTimer_1.win_condition.l1            = "Timer";
	PChar.quest.LSC_stormTimer_1.win_condition.l1.date.hour  = 23;
    PChar.quest.LSC_stormTimer_1.win_condition.l1.date.day   = GetAddingDataDay(0, 0, 1);
    PChar.quest.LSC_stormTimer_1.win_condition.l1.date.month = GetAddingDataMonth(0, 0, 0);
    PChar.quest.LSC_stormTimer_1.win_condition.l1.date.year  = GetAddingDataYear(0, 0, 0);
    PChar.quest.LSC_stormTimer_1.function = "LSC_stormTimer_1";

	//���-�������
	sld = GetCharacter(NPC_GenerateCharacter("LSCMayor", "capitan_2", "man", "man", 40, ENGLAND, -1, true));
	FantomMakeCoolFighter(sld, 90, 90, 90, "blade33", "pistol4", 200);
	sld.name = "���";
	sld.lastname = "������";
	sld.rank = 40;
	sld.city = "LostShipsCity";
	sld.location = "SanAugustineResidence";
	sld.location.group = "sit";
	sld.location.locator = "sit1";
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_0";
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������	
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mayor.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "mayor"; //��� ���
	sld.greeting = "eng_gov_common";
	sld.standUp = true; //�������� � �������� �� �����
	//sld.talker = true; //������ ������
	LAi_SetHuberType(sld); //to_do �������� �� ����
	LAi_RemoveLoginTime(sld);
	LAi_SetImmortal(sld, true);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);	
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �1
	sld = GetCharacter(NPC_GenerateCharacter("Ment_1", "sold_eng_3", "man", "man", 30, ENGLAND, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade21", "pistol4", 100);
	sld.name = "�����";
	sld.lastname = "�������";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto9"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_1"; 
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.greeting = "pirat_common";
	sld.talker = true; //������ ������
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �2
	sld = GetCharacter(NPC_GenerateCharacter("Ment_2", "sold_fra_4", "man", "man", 30, FRANCE, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade25", "pistol4", 100);
	sld.name = "����";
	sld.lastname = "�����";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto8"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_2"; 
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������	
	sld.location.hours = 2; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �3
	sld = GetCharacter(NPC_GenerateCharacter("Ment_3", "sold_hol_1", "man", "man", 30, HOLLAND, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade22", "pistol4", 100);
	sld.name = "����";
	sld.lastname = "�������";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto11"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_3"; 
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������
	sld.location.hours = 6; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.greeting = "pirat_common";
	sld.talker = true; //������ ������
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �4
	sld = GetCharacter(NPC_GenerateCharacter("Ment_4", "off_spa_2", "man", "man", 30, HOLLAND, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade24", "pistol4", 100);
	sld.name = "�����";
	sld.lastname = "����";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto4"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_4";  
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������
	sld.location.hours = 3; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �5
	sld = GetCharacter(NPC_GenerateCharacter("Ment_5", "officer_8", "man", "man", 30, PIRATE, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade33", "pistol4", 100);
	sld.name = "����";
	sld.lastname = "�������";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto3"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_5"; 
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ����������� �6
	sld = GetCharacter(NPC_GenerateCharacter("Ment_6", "officer_13", "man", "man", 30, PIRATE, -1, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "topor2", "pistol4", 100);
	sld.name = "�����";
	sld.lastname = "����";
	sld.rank = 30;
	sld.city = "LostShipsCity";
	sld.location	= "SanAugustineResidence";
	sld.location.group = "goto";
	sld.location.locator = "goto1"; 
	sld.location.baseShip = "11"; //������� ������� � ���
	sld.location.baseLocator = "goto11_6"; 
	sld.location.baseShip.going = false; //������������ ����� ��������� � ������	
	sld.location.hours = 7; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\Mentos.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "ment"; //��� ���
	sld.talker = true; //������ ������
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ------------------------ ������� ------------------------ 
	//���������
	sld = GetCharacter(NPC_GenerateCharacter("LSCBarmen", "barmen_8", "man", "man", 20, HOLLAND, -1, false));
	sld.name = "����";
	sld.lastname = "�������";
	sld.rank = 20;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "barmen";
	sld.location.locator = "stay";
	sld.dialog.filename   = "Quest\LostShipsCity\Barmen.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "tavern"; //��� ���
	sld.greeting = "Gr_Barmen";
	LAi_SetBarmanType(sld);
	LAi_RemoveLoginTime(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	//����������
	sld = GetCharacter(NPC_GenerateCharacter("LSCwaitress", "girl_1", "woman", "towngirl", 5, FRANCE, -1, false));
	sld.name = "����";
	sld.lastname = "�����";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "waitress";
	sld.location.locator = "barmen";
	sld.dialog.filename   = "Quest\LostShipsCity\Waitress.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "tavern"; //��� ���
	sld.greeting = "Gr_officiant";
	LAi_SetWaitressType(sld);
	LAi_RemoveLoginTime(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ------------------------ ������� �������� ------------------------	
	// ������� ��������� ����� �������
	sld = GetCharacter(NPC_GenerateCharacter("PedroHurtado", "usurer_2", "man", "man", 10, SPAIN, -1, false));
	sld.name = "�����";
	sld.lastname = "�������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto06_1"; 
	sld.location.baseShip = "06"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 2; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\PedroHurtado.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetLSCoutType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ������� ��������� ���� ����
	sld = GetCharacter(NPC_GenerateCharacter("ZhilBaru", "trader_16", "man", "man", 10, FRANCE, -1, true));
	sld.name = "����";
	sld.lastname = "����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto07_1"; 
	sld.location.baseShip = "07"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 1; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\ZhilBaru.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetLSCoutType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ������� ��������� ������� ������
	sld = GetCharacter(NPC_GenerateCharacter("TristanBorcher", "shipowner_13", "man", "man", 10, ENGLAND, -1, false));
	sld.name = "�������";
	sld.lastname = "������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto09_1"; 
	sld.location.baseShip = "09"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 3; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\TristanBorcher.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetLSCoutType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ������� ��������� ����� ����
	sld = GetCharacter(NPC_GenerateCharacter("RoyelHaag", "trader_11", "man", "man", 10, HOLLAND, -1, false));
	sld.name = "�����";
	sld.lastname = "����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto14_1"; 
	sld.location.baseShip = "14"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\RoyelHaag.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetLSCoutType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ������� ��������� ������� �����
	sld = GetCharacter(NPC_GenerateCharacter("DominicLegro", "trader_1", "man", "man", 10, FRANCE, -1, false));
	sld.name = "�������";
	sld.lastname = "�����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto15_1"; 
	sld.location.baseShip = "15"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\DiminicLegro.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetLSCoutType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ������� ��������� ����� ������
	sld = GetCharacter(NPC_GenerateCharacter("JorgeStouks", "barmen_5", "man", "man", 10, ENGLAND, -1, true));
	sld.name = "�����";
	sld.lastname = "������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "LostShipsCity_town";
	sld.location.group = "goto";
	sld.location.locator = "goto08_1"; 
	sld.location.baseShip = "08"; //������� ������� � ���
	sld.location.baseLocator = sld.location.locator; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\JorgeStouks.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetStayType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");
	ChangeCharacterAddress(sld, "LostShipsCity_town", "goto16_3");

	// ------------------------ ������� �������� � ������� ------------------------
	// � ������� ����� ������
	sld = GetCharacter(NPC_GenerateCharacter("HenryFolder", "barmen_14", "man", "man", 10, ENGLAND, -1, false));
	sld.name = "�����";
	sld.lastname = "������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit2"; 
	sld.location.baseShip = "03"; //������� ������� � ���
	sld.location.baseLocator = "goto03_2"; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 6; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\HenryFolder.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ������ �������
	sld = GetCharacter(NPC_GenerateCharacter("FelixMendoza", "shipowner_1", "man", "man", 10, SPAIN, -1, true));
	sld.name = "������";
	sld.lastname = "������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit3"; 
	sld.location.baseShip = "10"; //������� ������� � ���
	sld.location.baseLocator = "goto10_2"; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\FelixMendoza.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ������ ������
	sld = GetCharacter(NPC_GenerateCharacter("RodgerWoyler", "usurer_5", "man", "man", 10, ENGLAND, -1, false));
	sld.name = "������";
	sld.lastname = "������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit5"; 
	sld.location.baseShip = "12"; //������� ������� � ���
	sld.location.baseLocator = "goto12_2"; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 7; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\RodgerWoyler.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ����� �����
	sld = GetCharacter(NPC_GenerateCharacter("AndreLabor", "shipowner_15", "man", "man", 15, FRANCE, -1, true));
	FantomMakeCoolFighter(sld, 60, 60, 60, "blade20", "pistol2", 10);
	sld.name = "�����";
	sld.lastname = "�����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit9"; 
	sld.location.baseShip = "15"; //������� ������� � ���
	sld.location.baseLocator = "goto15_2"; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\AndreLabor.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ������ �����
	sld = GetCharacter(NPC_GenerateCharacter("StefanKleyn", "shipowner_6", "man", "man", 10, HOLLAND, -1, true));
	sld.name = "������";
	sld.lastname = "�����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit8"; 
	sld.location.baseShip = "14"; //������� ������� � ���
	sld.location.baseLocator = "goto14_2";  
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 3; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\StefanKleyn.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ����� �����
	sld = GetCharacter(NPC_GenerateCharacter("ThomasFlann", "citiz_1", "man", "man", 10, ENGLAND, -1, false));
	sld.name = "�����";
	sld.lastname = "�����";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit6"; 
	sld.location.baseShip = "02"; //������� ������� � ���
	sld.location.baseLocator = "goto02_2";  
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 3; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\ThomasFlann.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	sld.talker = true; //������ ������
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������� ����� �������
	sld = GetCharacter(NPC_GenerateCharacter("PabloBarades", "officer_20", "man", "man", 10, SPAIN, -1, true));
	sld.name = "�����";
	sld.lastname = "�������";
	sld.rank = 10;
	sld.city = "LostShipsCity";
	sld.location	= "FleuronTavern";
	sld.location.group = "sit";
	sld.location.locator = "sit14"; 
	sld.location.baseShip = "09"; //������� ������� � ���
	sld.location.baseLocator = "goto09_2";  
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\PabloBarades.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "cit_common";
	LAi_SetSitType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ------------------------ ������� �������� � ������ ------------------------
	// �����
	sld = GetCharacter(NPC_GenerateCharacter("LSC_Priest", "priest_3", "man", "man", 5, HOLLAND, -1, false));
	sld.name = "�����";
	sld.lastname = "������";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "GloriaChurch";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.dialog.filename   = "Quest\LostShipsCity\Priest.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "priest"; //��� ���
	sld.greeting = "Gr_Church";
	LAi_SetPriestType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);		
	
	// � ������ ������ �����
	sld = GetCharacter(NPC_GenerateCharacter("SandraGomes", "girl_4", "woman", "towngirl", 5, SPAIN, -1, false));
	sld.name = "������";
	sld.lastname = "�����";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "GloriaChurch";
	sld.location.group = "goto";
	sld.location.locator = "goto1"; 
	sld.location.baseShip = "03"; //������� ������� � ���
	sld.location.baseLocator = "goto03_3"; 
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\SandraGomes.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizen"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// � ������ ������ ����
	sld = GetCharacter(NPC_GenerateCharacter("MonikaRight", "girl_5", "woman", "towngirl", 5, ENGLAND, -1, false));
	sld.name = "������";
	sld.lastname = "����";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "GloriaChurch";
	sld.location.group = "goto";
	sld.location.locator = "goto2"; 
	sld.location.baseShip = "12"; //������� ������� � ���
	sld.location.baseLocator = "goto12_3"; 
	sld.location.baseLocation = "VelascoShipInside1"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\MonikaRight.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ------------------------ ������� ------------------------
	// �����
	sld = GetCharacter(NPC_GenerateCharacter("LSC_Trader", "trader_5", "man", "man", 5, HOLLAND, -1, false));
	sld.name = "����";
	sld.lastname = "����";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "EsmeraldaStoreBig";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.dialog.filename   = "Quest\LostShipsCity\Trader.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "trader"; //��� ���
	sld.greeting = "Gr_Church";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);	

	// ------------------------ ���������� �������� ------------------------
	// ��������� ��������� ���� ������
	sld = GetCharacter(NPC_GenerateCharacter("ElisTaylor", "girl_3", "woman", "towngirl", 5, ENGLAND, -1, false));
	sld.name = "����";
	sld.lastname = "������";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "CeresSmithy";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.location.baseShip = "03"; //������� ������� � ���
	sld.location.baseLocator = "goto03_0"; 
	sld.location.baseLocation = "CeresSmithy"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 4; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\ElisTaylor.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ��������� ��������� ����� ������
	sld = GetCharacter(NPC_GenerateCharacter("OreliBertin", "girl_2", "woman", "towngirl", 5, FRANCE, -1, false));
	sld.name = "�����";
	sld.lastname = "������";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "FurieShipInside2";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.location.baseShip = "06"; //������� ������� � ���
	sld.location.baseLocator = "goto06_0"; 
	sld.location.baseLocation = "FurieShipInside2"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 6; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\OreliBertin.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ��������� ��������� ��� �����
	sld = GetCharacter(NPC_GenerateCharacter("LeaToors", "girl_7", "woman", "towngirl", 5, HOLLAND, -1, false));
	sld.name = "���";
	sld.lastname = "�����";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "PlutoStoreSmall";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.location.baseShip = "09"; //������� ������� � ���
	sld.location.baseLocator = "goto09_0"; 
	sld.location.baseLocation = "PlutoStoreSmall"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\LeaToors.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ��������� ��������� ����� ������
	sld = GetCharacter(NPC_GenerateCharacter("ElizaCalvo", "girl_8", "woman", "towngirl", 5, SPAIN, -1, false));
	sld.name = "�����";
	sld.lastname = "������";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "SantaFlorentinaShipInside4";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.location.baseShip = "14"; //������� ������� � ���
	sld.location.baseLocator = "goto14_0"; 
	sld.location.baseLocation = "SantaFlorentinaShipInside4"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 7; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\ElizaCalvo.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	// ��������� ��������� ������ ������
	sld = GetCharacter(NPC_GenerateCharacter("SesilGalard", "girl_6", "woman", "towngirl", 5, FRANCE, -1, false));
	sld.name = "������";
	sld.lastname = "������";
	sld.rank = 5;
	sld.city = "LostShipsCity";
	sld.location	= "AvaShipInside3";
	sld.location.group = "barmen";
	sld.location.locator = "stay"; 
	sld.location.baseShip = "15"; //������� ������� � ���
	sld.location.baseLocator = "goto15_0"; 
	sld.location.baseLocation = "AvaShipInside3"; //������� ��������� �������
	sld.location.baseShip.going = true; //������������ ����� ��������� � ������	
	sld.location.hours = 5; //������������ ����� .. �����
	sld.dialog.filename   = "Quest\LostShipsCity\SesilGalard.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "citizenHome"; //��� ���
	sld.greeting = "Gr_Woman_Citizen";
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetOwnerType(sld);
	LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
	SaveCurrentNpcQuestDateParam(sld, "location");

	//------------------- ���� ������ -------------------
	// ������ �������
	sld = GetCharacter(NPC_GenerateCharacter("Narval_head", "capitan_3", "man", "man", 30+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 30+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 100, 100, 100, "blade28", "pistol4", 200);
	sld.name = "�����";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto1"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_SetImmortal(sld, true);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	SaveCurrentNpcQuestDateParam(sld, "location"); //����� ����� ������� �����������
	// ������ �1
	sld = GetCharacter(NPC_GenerateCharacter("Narval_1", "officer_18", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade21", "pistol6", 100);
	sld.name = "����";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto2"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �2
	sld = GetCharacter(NPC_GenerateCharacter("Narval_2", "officer_12", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;	
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade34", "pistol3", 150);
	sld.name = "������";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto3"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �3
	sld = GetCharacter(NPC_GenerateCharacter("Narval_3", "trader_2", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 70, 70, 70, "blade25", "pistol5", 50);
	sld.name = "���";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto4"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �4
	sld = GetCharacter(NPC_GenerateCharacter("Narval_4", "pirate_7", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 85, 85, 85, "blade33", "pistol6", 150);
	sld.name = "����";
	sld.lastname = "������";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto5"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �5
	sld = GetCharacter(NPC_GenerateCharacter("Narval_5", "pirate_3", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "topor2", "pistol3", 100);
	sld.name = "�����";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto6"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �6
	sld = GetCharacter(NPC_GenerateCharacter("Narval_6", "pirate_4", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade22", "pistol6", 120);
	sld.name = "�����";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto7"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");
	// ������ �7
	sld = GetCharacter(NPC_GenerateCharacter("Narval_7", "officer_14", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 85, 85, 85, "blade23", "pistol4", 150);
	sld.name = "�����";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "VelascoShipInside1";
	sld.location.group = "goto";
	sld.location.locator = "goto8"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Narval.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "narval"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_1");

	//------------------- ���� ������ -------------------
	// ������ �������
	sld = GetCharacter(NPC_GenerateCharacter("Casper_head", "shipowner_7", "man", "man", 30+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 30+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 100, 100, 100, "blade28", "pistol5", 200);
	sld.name = "������";
	sld.lastname = "�������";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto1"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_SetImmortal(sld, true);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	SaveCurrentNpcQuestDateParam(sld, "location"); //����� ����� ������� �����������
	//������ �1
	sld = GetCharacter(NPC_GenerateCharacter("Casper_1", "pirate_9", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "topor2", "pistol3", 100);
	sld.name = "�����";
	sld.lastname = "������";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto2"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �2
	sld = GetCharacter(NPC_GenerateCharacter("Casper_2", "pirate_8", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade26", "pistol6", 120);
	sld.name = "�����";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto3"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �3
	sld = GetCharacter(NPC_GenerateCharacter("Casper_3", "pirate_10", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade34", "pistol4", 150);
	sld.name = "����";
	sld.lastname = "������";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto4"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �4
	sld = GetCharacter(NPC_GenerateCharacter("Casper_4", "pirate_2", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade23", "pistol3", 100);
	sld.name = "������";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto5"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �5
	sld = GetCharacter(NPC_GenerateCharacter("Casper_5", "officer_10", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "topor2", "pistol3", 130);
	sld.name = "������";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto6"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �6
	sld = GetCharacter(NPC_GenerateCharacter("Casper_6", "officer_1", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 20+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 70, 70, 70, "blade25", "pistol4", 100);
	sld.name = "����";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto7"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �7
	sld = GetCharacter(NPC_GenerateCharacter("Casper_7", "officer_2", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade34", "pistol6", 150);
	sld.name = "������";
	sld.lastname = "���";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto8"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �8
	sld = GetCharacter(NPC_GenerateCharacter("Casper_8", "officer_17", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade26", "pistol4", 150);
	sld.name = "�����";
	sld.lastname = "������";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto9"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �9
	sld = GetCharacter(NPC_GenerateCharacter("Casper_9", "officer_5", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade28", "pistol6", 150);
	sld.name = "������";
	sld.lastname = "�����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto10"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");
	//������ �10
	sld = GetCharacter(NPC_GenerateCharacter("Casper_10", "officer_6", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, -1, true));
	sld.rank = 25+MOD_SKILL_ENEMY_RATE;
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade28", "pistol4", 150);
	sld.name = "������";
	sld.lastname = "����";
	sld.city = "LostShipsCity";
	sld.location	= "SanGabrielMechanic";
	sld.location.group = "goto";
	sld.location.locator = "goto11"; 	
	sld.dialog.filename   = "Quest\LostShipsCity\Casper.c";
	sld.dialog.currentnode   = "First time";
	sld.cityTape = "casper"; //��� ���
	sld.greeting = "pirat_common";
	LAi_SetWarriorType(sld);
	LAi_group_MoveCharacter(sld, "PearlGroup_2");



}

//����� �����
void LSC_stormTimer_1(string qName)
{
	for(i=0; i<MAX_LOCATIONS; i++)
	{				
		if (CheckAttribute(&locations[i], "fastreload") && locations[i].fastreload == "LostShipsCity")
		{			
			DeleteAttribute(&locations[i], "alwaysStorm");
			DeleteAttribute(&locations[i], "storm");
			DeleteAttribute(&locations[i], "tornado");	
			DeleteAttribute(&locations[i], "QuestlockWeather");
			DeleteAttribute(&locations[i], "MaxWaveHeigh");
		}
	}
	//������� ����� ��, ���� ��� ��� �� ������ � ����
	if (pchar.questTemp.LSC == "AdmiralIsWaiting")
	{
		pchar.quest.LSC_admiralOwnFind.win_condition.l1 = "ExitFromLocation";
		pchar.quest.LSC_admiralOwnFind.win_condition.l1.location = pchar.location;
		pchar.quest.LSC_admiralOwnFind.function = "LSC_admiralOwnFind";
	}
}

void LSC_admiralOwnFind(string qName)
{
	sld = characterFromId("LSCMayor");
	sld.dialog.currentnode = "FoundHero";
	PlaceCharacter(sld, "goto", "random_must_be_near");
	LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", 2.0, 0);	
}

void LSC_enterToProtector(string qName)
{
    chrDisableReloadToLocation = true;
	LAi_LocationFightDisable(loadedLocation, true); 
    DoQuestFunctionDelay("LSC_enterToProtector_1", 7.0);
}

void LSC_enterToProtector_1(string qName)
{
	LAi_group_Delete("EnemyFight"); 
    sld = GetCharacter(NPC_GenerateCharacter("LSCQuestMent", "off_hol_2", "man", "man", 20+MOD_SKILL_ENEMY_RATE, PIRATE, 0, true));
	SetFantomParamFromRank(sld, iTemp, true);
	sld.Dialog.Filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "LSCQuestMent";
    LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	ChangeCharacterAddressGroup(sld, pchar.location, "reload",  "reload1");
	LAi_ActorDialog(sld, pchar, "", 1.0, 0);
}

void LSC_KillOneNarval(string qName)
{
    chrDisableReloadToLocation = true;
	LAi_LocationFightDisable(loadedLocation, true); 
    DoQuestFunctionDelay("LSC_KillOneNarval_1", 3.0 + frand(4.0));
}

void LSC_KillOneNarval_1(string qName)
{
	LAi_group_Delete("EnemyFight"); 
    sld = GetCharacter(NPC_GenerateCharacter("LSCQuestNarval", "officer_16", "man", "man", 25+MOD_SKILL_ENEMY_RATE, PIRATE, 0, true));
	FantomMakeCoolFighter(sld, 80, 80, 80, "blade28", "pistol4", 150);
	sld.Dialog.Filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "LSCQuestNarval";
	ChangeItemName("DOjeronRing", "itmname_DOjeronRingNarval");
	ChangeItemDescribe("DOjeronRing", "itmdescr_DOjeronRingNarval");
	GiveItem2Character(sld, "DOjeronRing");
	sld.SaveItemsForDead   = true; // ��������� �� ����� ����
	sld.DontClearDead = true;  // �� ������� ���� ����� 200�
    LAi_SetActorType(sld);
	LAi_group_MoveCharacter(sld, "EnemyFight");
	ChangeCharacterAddressGroup(sld, pchar.location, "reload",  "reload1");
	LAi_ActorDialog(sld, pchar, "", 1.0, 0);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
////  ������� ���    eddy       �����
/////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////
////   -- ������� �������� ����� --     ������
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void Blood_StartGame(string qName)
{
    ref sld;
    Pchar.questTemp.CapBloodLine.stat = "Begining";

    RemoveCharacterEquip(pchar, BLADE_ITEM_TYPE);
    RemoveCharacterEquip(pchar, GUN_ITEM_TYPE);
    RemoveCharacterEquip(pchar, SPYGLASS_ITEM_TYPE);
    RemoveCharacterEquip(pchar, PATENT_ITEM_TYPE);
	RemoveCharacterEquip(pchar, CIRASS_ITEM_TYPE);
	RemoveCharacterEquip(pchar, MAPS_ITEM_TYPE);
    DeleteAttribute(Pchar, "items");
//    DeleteAttribute(Pchar, "equip");
    DeleteAttribute(Pchar, "ship");
    DeleteAttribute(Pchar, "ShipSails.gerald_name");
    Pchar.ship.type = SHIP_NOTUSED;
    GiveItem2Character(Pchar, "unarmed");
    EquipCharacterByItem(Pchar, "unarmed");
    TakeNItems(Pchar, "potion3", 1);
    TakeNItems(Pchar, "potion2", 3);
    TakeNItems(Pchar, "potion1", 4);

	sld = ItemsFromID("key3");
	sld.shown = true;
	sld.startLocation = "Estate";
	sld.startLocator = "item1";
	
	//����� �����
    sld = GetCharacter(NPC_GenerateCharacter("Beyns", "trader_8", "man", "man", 7, ENGLAND, 1, false));
    sld.name = "�����";
    sld.lastname = "�����";
	sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
	sld.dialog.currentnode = "First time";
	sld.greeting = "cit_common";
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
	LAi_group_MoveCharacter(sld, sTemp);
    LAi_SetStayTypeNoGroup(sld);
   	LAi_SetImmortal(sld, true);
	sld.talker = 10;
	ChangeCharacterAddressGroup(sld, "Estate", "goto", "goto12");
	
    //������� ����
   	sld = GetCharacter(NPC_GenerateCharacter("Pitt", "Youngman", "man", "man3", 1, ENGLAND, -1, false)); //TO_DO �������� ������� �� ���������� ��������
    sld.name 	= "�������";
    sld.lastname 	= "����";
    sld.Dialog.CurrentNode = "First time";
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    sld.greeting = "Gr_Officer";

    SetSPECIAL(sld, 9,8,10,3,6,10,4);
    InitStartParam(sld);
    SetEnergyToCharacter(sld);
   	LAi_SetImmortal(sld, true);
    LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", 2.0, 0);
	ChangeCharacterAddressGroup(sld, "Estate", "goto", "goto1");
	
	pchar.quest.CureLordMovie.win_condition.l1 = "locator";
	pchar.quest.CureLordMovie.win_condition.l1.location = "EstateBadroom1";
	pchar.quest.CureLordMovie.win_condition.l1.locator_group = "quest";
	pchar.quest.CureLordMovie.win_condition.l1.locator = "quest1";
	pchar.quest.CureLordMovie.function = "CureLord";

}

void CureLord(string qName)
{
    ref sld;
    sld = &characters[GetCharacterIndex("Beyns")];
    ChangeCharacterAddressGroup(sld, "EstateBadroom1", "reload", "reload1");
    sld.dialog.currentnode = "EBStep_1";
    LAi_SetActorTypeNoGroup(sld);
	LAi_ActorDialog(sld, pchar, "",  -1.0, 0);

}

void CureLordMovie()
{
    InterfaceStates.Buttons.Save.enable = false;
	StartQuestMovie(true, true, true);
	ChangeShowIntarface();
    LAi_SetActorTypeNoGroup(Pchar);
    LAi_ActorGoToLocator(Pchar, "goto", "goto6", "CureLordMovie_Con1", 10.0);
}

void CureLordMovie_Con1()
{
    LAi_ActorTurnToLocator(Pchar, "goto", "goto7");
    LAi_ActorAnimation(Pchar, "Barman_idle", "CureLordMovie_Con2", 5);
}

void CureLordMovie_Con2()
{
    LAi_Fade("CureLordMovie_Con3", "");
}

void CureLordMovie_Con3()
{

    ref sld;
    sld = &characters[GetCharacterIndex("Beyns")];
    sld.dialog.currentnode = "EBStep_2";
//   	sld.talker = 10;
//    LAi_SetActorTypeNoGroup(sld);
//	LAi_ActorDialog(sld, pchar, "",  2, 0);
    ChangeCharacterAddressGroup(sld, "EstateOffice", "goto", "goto6");
    LAi_SetStayTypeNoGroup(sld);
    DoReloadCharacterToLocation("EstateOffice", "goto", "goto3");
    EndQuestMovie();
   	ChangeShowIntarface();
    LAi_SetPlayerType(Pchar);
    InterfaceStates.Buttons.Save.enable = true;

}

void BloodGetBlade(string qName)
{
    LAi_SetCheckMinHP(Pchar, 1, true, "CapBlood_CheckMinHP");
   	LAi_SetImmortal(pchar, false);
}

void DragunInvansion()
{
    ref sld;
    sld = &characters[GetCharacterIndex("Pitt")];
    sld.dialog.currentnode = "PStep_5";
    ChangeCharacterAddressGroup(sld, "EstateOffice", "reload", "reload1");
    LAi_SetActorTypeNoGroup(sld);
    LAi_ActorDialog(sld, pchar, "",  -1, 0);
}

void DragunInvansion2()
{
    pchar.quest.BloodGetBlade.win_condition.l1 = "item";
    pchar.quest.BloodGetBlade.win_condition.l1.item= "blade2";
    pchar.quest.BloodGetBlade.function = "BloodGetBlade";
    pchar.quest.DragunInvansion.win_condition.l1          = "location";
    pchar.quest.DragunInvansion.win_condition.l1.location = "Estate";
    pchar.quest.DragunInvansion.function                  = "_DragunInvansion3";

}

void _DragunInvansion3(string qName)
{
    chrDisableReloadToLocation = true; // ������� ����� �� �������
    DoQuestFunctionDelay("DragunInvansion3", 60.0);
}

void DragunInvansion3(string qName)
{
    ref ch, sld;
    LAi_LocationFightDisable(loadedLocation, true);
    LAi_SetFightMode(Pchar, false);
    LAi_LockFightMode(pchar, true);
    LAi_group_SetRelation("EnemyFight", LAI_GROUP_PLAYER, LAI_GROUP_NEITRAL);

	ch = GetCharacter(NPC_GenerateCharacter("CapGobart", "off_eng_1", "man", "man", 10, ENGLAND, 0, false));
    ch.name 	= "�������";
    ch.lastname 	= "������";
    ch.Dialog.CurrentNode = "First time";
    ch.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    ch.greeting = "soldier_arest";
    ch.location = "none";
    FantomMakeCoolFighter(ch, 25, 70, 70, "blade22", "pistol2", 20);
    LAi_SetImmortal(ch, true);
    ch.BreakTmplAndFight = true;
    //LAi_group_MoveCharacter(ch, "TmpEnemy");
    LAi_group_MoveCharacter(ch, "EnemyFight");
    ChangeCharacterAddressGroup(ch, "Estate", "goto", "goto1");
    LAi_SetActorTypeNoGroup(ch);
	LAi_ActorDialog(ch, pchar, "", 7, 0);

    for (int i=0 ; i<=4; i++)
    {
    	// ������� c �������������
        sld = GetCharacter(NPC_GenerateCharacter(("Dragun_"+i), "sold_eng_"+(rand(7)+1), "man", "man", 2, ENGLAND, 0, false));
        sld.location = "none";
        sld.BreakTmplAndFight = true;
        SetSPECIAL(sld, 8,10,9,3,6,10,4);
        InitStartParam(sld);
        SetEnergyToCharacter(sld);
        LAi_SetImmortal(sld, true); // ����� �������
        //LAi_SetCheckMinHP(sld, 1, true, "Tut_StartGame_CheckMinHP_2");
        GiveItem2Character(sld, "blade4");
    	EquipCharacterByItem(sld, "blade4");
        //LAi_group_MoveCharacter(sld, "TmpEnemy");
        LAi_group_MoveCharacter(sld, "EnemyFight");
    	ChangeCharacterAddressGroup(sld, "Estate", "reload", "reload1");
        LAi_SetActorTypeNoGroup(sld);
    	if(i < 3)
    	{
        	if( i > 1 )
        	{
            	GiveItem2Character(sld, "pistol2");
            	EquipCharacterByItem(sld, "pistol2");
            	TakeNItems(sld, "bullet", 30);
            }
            LAi_ActorFollow(sld, ch, "", -1);
        }
        if(i == 3)LAi_ActorRunToLocation(sld, "reload", "reload3", "none", "", "", "", 20);
        if(i == 4)LAi_ActorRunToLocation(sld, "reload", "reload5", "none", "", "", "", 20);
     }

}

void DragunInvansion4()
{
    ref ch, sld;
    LAi_LockFightMode(pchar, false);
    LAi_LocationFightDisable(loadedLocation, false);
    ch = &characters[GetCharacterIndex("Pitt")];
    ChangeCharacterAddressGroup(ch, "Estate", "reload", "reload5");
    LAi_SetActorType(ch);
    LAi_ActorGoToLocator(ch, "goto", "goto5", "", -1);
    
    sld = &characters[GetCharacterIndex("Beyns")];
    ChangeCharacterAddressGroup(sld, "Estate", "reload", "reload5");
    LAi_SetActorType(sld);
    LAi_ActorFollow(sld, ch, "", -1);

    for (i=1; i<5; i++)
    {
        sld = &characters[GetCharacterIndex("Dragun_"+i)];
        //LAi_group_MoveCharacter(sld, "TmpEnemy");
        if(i == 3 || i == 4)
        {
            ChangeCharacterAddressGroup(sld, "Estate", "reload", "reload5");
            LAi_SetActorTypeNoGroup(sld);
            LAi_ActorFollow(sld, ch, "", -1);
        }
        else
        {
            //LAi_SetStayTypeNoGroup(sld);
            //LAi_SetWarriorType(sld);
            //LAi_SetGuardianType(sld);
        }
    }

}

void CapGobartAttack(string qName)
{
    ref sld;
    int numdead = 0;
    for (i=1; i<=2; i++)
    {
        sld = characterFromID("Dragun_"+i);
        if (CharacterIsDead(sld))
        {
            numdead++;
        }
    }
    if(numdead == 2)
    {
        sld = characterFromID("CapGobart");
        LAi_group_MoveCharacter(sld, "TmpEnemy");
        LAi_SetWarriorType(sld);
        LAi_group_Attack(sld, Pchar);
    }
}

void Blood_StartGame_End(string qName)
{
    ref sld;
    chrDisableReloadToLocation = true;
    InterfaceStates.Launched = false;
    InterfaceStates.Buttons.Save.enable = true;
    sld = characterFromID("Dragun_0");
    LAi_RemoveCheckMinHP(sld);
    pchar.quest.CapGobartAttack_1.over = "yes";
    pchar.quest.CapGobartAttack_2.over = "yes";
    LAi_Fade("CapBloodLineInit", "");
    Pchar.model="blood_bomj";
}


void CapBloodLineInit(string qName)
{
    //���������� ��� ����� �����
    LAi_SetCurHPMax(Pchar);
    LAi_SetPlayerType(Pchar);
    //�������
    LAi_SetImmortal(Pchar, false);
    DeleteAttribute(Pchar, "items");
    DeleteAttribute(Pchar, "equip");
    DeleteAttribute(Pchar, "ship");
    DeleteAttribute(Pchar, "ShipSails.gerald_name");
    Pchar.ship.type = SHIP_NOTUSED;
    GiveItem2Character(Pchar, "unarmed");
    EquipCharacterByItem(Pchar, "unarmed");
    // C���
    sld = &characters[GetCharacterIndex("Bridgetown_Mayor")];
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    // ������ �����
	Environment.date.month = 4;
    SetCurrentTime(23, 1);
    pchar.quest.onPlantation.win_condition.l1          = "location";
    pchar.quest.onPlantation.win_condition.l1.location = "BridgeTown_Plantation";
    pchar.quest.onPlantation.function                  = "CapBloodLine_q1";
    //DoReloadCharacterToLocation("BridgeTown_Plantation", "patrol", "patrol12");

    //����� ������, ������� ��� ���               //to_do
    int n= FindLocation("Bridgetown_town");  // �� ������ �������
    locations[n].reload.l3.close_for_night = false;
    //��� ����������� � ��������
    bDisableLandEncounters = true;

    sld = characterFromID("Pitt");
    sld.Dialog.CurrentNode = "PStep_7";
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
	LAi_group_MoveCharacter(sld, sTemp);
//    LAi_SetStayTypeNoGroup(sld);
    ChangeCharacterAddressGroup(sld, "Plantation_S1", "reload", "reload1");
    LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", 3, 0);
	
    DoReloadCharacterToLocation("Plantation_S1", "goto", "goto1");
	StartPictureAsVideo( "loading\sea.tga", 1 );

}


void CapBloodLine_q1(string qName)
{

    ref sld;
    LAi_LocationFightDisable(loadedLocation, true);
    LAi_SetFightMode(Pchar, false);
    LAi_LockFightMode(pchar, true);
    chrDisableReloadToLocation = true; // ������� ����� �� �������
    bDisableFastReload = true; // ������� ��������.
    Pchar.questTemp.CapBUnarmed = true; //�� ����� �� ��������

    //����� �� ����������                              //off_eng_1
   	sld = GetCharacter(NPC_GenerateCharacter("Bishop", "trader_2", "man", "man", 1, ENGLAND, 0, false));
    sld.name 	= "���������";
    sld.lastname 	= "�����";
    sld.Dialog.CurrentNode = "First Bishop";
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    sld.greeting = "soldier_arest";

    SetSPECIAL(sld, 9,8,10,3,6,10,4);
    InitStartParam(sld);
    SetEnergyToCharacter(sld);

    GiveItem2Character(sld, "blade22");
	EquipCharacterByItem(sld, "blade22");
	GiveItem2Character(sld, "pistol2");
	EquipCharacterByItem(sld, "pistol2");
	LAi_SetImmortal(sld, true);

    ChangeCharacterAddressGroup(sld, "BridgeTown_Plantation", "reload", "houseSp1");
    LAi_SetActorType(sld);
	LAi_ActorDialog(sld, pchar, "", -1, 0);

    //������� ����
    sld = characterFromID("Pitt");
    sld.Dialog.CurrentNode = "First time";
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    LAi_SetCitizenType(sld);
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
	LAi_group_MoveCharacter(sld, sTemp);
    ChangeCharacterAddressGroup(sld, "BridgeTown_Plantation", "officers", "houseS1_3");
    //���������
	sld = GetCharacter(NPC_GenerateCharacter("Griffin", "shipowner_11", "man", "man", 10, ENGLAND, 3, false));
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
	sld.name = "��";
	sld.lastname = "�������";
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
    LAi_group_MoveCharacter(sld, sTemp);
	LAi_SetStayTypeNoGroup(sld);
	ChangeCharacterAddressGroup(sld, "CommonFlamHouse", "goto","goto1");
	LAi_SetImmortal(sld, true);
    //�����
	sld = GetCharacter(NPC_GenerateCharacter("Hells", "shipowner_9", "man", "man", 10, ENGLAND, 3, false));
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
	sld.name = "������";
	sld.lastname = "������";
    sld.SaveItemsForDead = true; // ��������� �� ����� ����
    sld.DontClearDead = true;
	AddMoneyToCharacter(sld, 2000);
	FantomMakeCoolFighter(sld, 7, 25, 50, "topor1", "", 10);
    //GiveItem2Character(sld, "topor1");
	//EquipCharacterByItem(sld, "topor1");
	//sTemp = GetNationNameByType(ENGLAND) + "_citizens";
    //LAi_group_MoveCharacter(sld, sTemp);
	LAi_SetStayTypeNoGroup(sld);
	ChangeCharacterAddressGroup(sld, "CommonRoom_MH2", "goto","goto4");
	//������
	sld = GetCharacter(NPC_GenerateCharacter("Quest_Smuggler", "Pirate_4", "man", "man", 10, ENGLAND, 3, false));
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
	LAi_SetSitTypeNoGroup(sld);
	ChangeCharacterAddressGroup(sld, "Bridgetown_tavern", "sit","sit2");
	

    //��������� ��������� ��� �� ��������
	SetQuestsCharacters();

    // ������ ����� � �����
    pchar.quest.CapBloodLine_q1_1.win_condition.l1          = "location";
    pchar.quest.CapBloodLine_q1_1.win_condition.l1.location = "Bridgetown_town";
    pchar.quest.CapBloodLine_q1_1.function                  = "_PrepareBridgeTown";

}

void _PrepareBridgeTown(string qName)
{
    DoQuestFunctionDelay("PrepareBridgeTown", 1.0);
}
void PrepareBridgeTown(string qName)
{
    trace("PrepareBridgeTown");
    //������ ����� ��� �������
    ChangeBridgeTownDialogs(false);
    //������ �� ������ � ����������
    ref sld = &characters[GetCharacterbyLocation("Bridgetown_town", "soldiers", "soldier2")];
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    sld.Dialog.CurrentNode = "First Guard";
    sld.protector = true;
    sld.protector.CheckAlways = 1 ;
    LAi_RemoveLoginTime(sld);
    LoginCharacter(sld, "BridgeTown_town");
    //�������
    LoginNettl("");
    //������� ���������� �� ����
    LocatorReloadEnterDisable("BridgeTown_town", "reload1_back", true);
    //LocatorReloadEnterDisable("BridgeTown_town", "reload3_back", true);
    //������ ����� �� �������
    //int n = FindLocation("Bridgetown_tavern");
    //locations[n].habitues = 0;
    //������� �����
    sld = &characters[GetCharacterIndex("Bridgetown_Smuggler")]
    LoginCharacter(sld, "Bridgetown_tavern");

}

void DetectBlades()
{
    if (Pchar.questTemp.CapBloodLine == true && Pchar.questTemp.CapBUnarmed == true)
    {
        string sweapon = FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE);
        //Log_Info("������:"+sweapon);
        bool bblades = ( sweapon != "unarmed") && (sweapon != "blade5" ) && (sweapon!= "");
        bool bguns = (FindCharacterItemByGroup(pchar, GUN_ITEM_TYPE) != "");
        if(bblades || bguns)
        {
            pchar.quest.CapBloodDetectBlades1.win_condition.l1          = "location";
            pchar.quest.CapBloodDetectBlades1.win_condition.l1.location = "Bridgetown_town";
            pchar.quest.CapBloodDetectBlades1.function                  = "TakeBlades";
            
            pchar.quest.CapBloodDetectBlades2.win_condition.l1          = "location";
            pchar.quest.CapBloodDetectBlades2.win_condition.l1.location = "BridgeTown_Plantation";
            pchar.quest.CapBloodDetectBlades2.function                  = "TakeBlades";
        }
        if(sweapon == "")
        {
            sweapon = "unarmed";
            GiveItem2Character(Pchar, "unarmed");
            EquipCharacterByItem(Pchar, sweapon);
        }
        else
        {
            EquipCharacterByItem(Pchar,GetCharacterEquipByGroup(Pchar, BLADE_ITEM_TYPE));
        }
    }
}

void TakeBlades(string qName)
{
    sld = GetCharacter(NPC_GenerateCharacter("SolderTakeBlades", "sold_eng_"+(rand(7)+1), "man", "man", 10, ENGLAND, 0, false));
	sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
	sld.dialog.currentnode = "First time";
	sld.city = "Bridgetown_town";
    sld.greeting = "Gr_HUNTER";
    sld.location = "none";
    sld.BreakTmplAndFightGroup = true;
    SetFantomParamHunter(sld); //������ �����
	LAi_SetCitizenType(sld);
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
	LAi_group_MoveCharacter(sld, sTemp);
    PlaceCharacter(sld, "goto", "random_must_be_near"))
    LAi_SetActorTypeNoGroup(sld);
	LAi_ActorDialog(sld, pchar, "",  -1.0, 0);
	chrDisableReloadToLocation = true;
	//Log_Info("������ � �� ������� �����!");

}


//������ ���� ������
void ChangeBridgeTownDialogs(bool restore)
{
    ref sld;
    int num, cur;
    num = 0;
    cur = rand(3)+1;
    for(int i=0;i<MAX_CHARACTERS;i++)
	{
        sld = &characters[i];
        //�������
        if (CheckAttribute(sld, "city") && sld.city == "Bridgetown")
        {
            if(CheckAttribute(sld, "CityType") && sld.CityType == "soldier")
            {
                if (!restore)
                {
                    sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
                }
                else
                {
                    sld.dialog.filename = "Common_Soldier.c";
                }

            }
            //��������
            if(CheckAttribute(sld, "CityType") && sld.CityType == "citizen")
            {
                if (!restore)
                {
                    sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
                    if(sld.location.group == "merchant")
                    {   //��������
                        num++;
                        if(num == cur) // ��� ������ �� ������ ������ ����� ������ ��������
                        {
                            sld.quest.bGoodMerch = true;
                        }

                    }
                }
                else
                {
                    if(sld.location.group == "merchant")
                    {   //��������
                        sld.dialog.filename = "Common_ItemTrader.c";
                    }
                    else
                    {   //�������
                        sld.dialog.filename = "Common_citizen.c";
                    }
                }

            }
            //�����
            if(sld.id == "Bridgetown_Poorman")
            {
                if (!restore)
                {
                    sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
                }
                else
                {
                    sld.dialog.filename = "Common_poorman.c";
                }
            }

            //���������
            if(sld.id == "QuestCitiz_Bridgetown")
            {
                if (!restore)
                {
                    sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
                    sld.Dialog.CurrentNode = "First time";
                }
                else
                {
                    sld.dialog.filename = "Quest\ForAll_dialog.c";
                }
            }
        }

	}
}

void LoginNettl(string qName)
{

        Pchar.questTemp.CapBloodLine.TalkWithNettl = false;          //10
		sld = GetCharacter(NPC_GenerateCharacter("Nettl", "shipowner_3", "man", "man", 7, ENGLAND, 3, false));
        sld.name = "";
        sld.lastname = "�������";
		sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
		sld.dialog.currentnode = "First time";
		sld.city = "Bridgetown_town";
		sld.greeting = "Gr_Tavern_Mate";
//		LAi_SetLoginTime(sld, 0.0, 23.99);
		//SetFantomParamFromRank(sld, Rank, true);
		LAi_SetCitizenType(sld);
		sTemp = GetNationNameByType(ENGLAND) + "_citizens";
		LAi_group_MoveCharacter(sld, sTemp);
		sld.talker = 10;
		PlaceCharacter(sld, "goto", "random");
}

void CapBloodLine_q1_Late(string qName)
{
    InterfaceStates.Buttons.Save.enable = false;
  //  SetLaunchFrameFormParam("���������� ���� �� ������� ������ �����"+ NewStr() +
  //  " ��� ������������� � �� ����."+ NewStr() +
  //  "��������� ���������� �������������� �������, "+ NewStr() +
  //  "����� ��� ����� ������� �� ������ � �������� ��� ����� �� ��������� ������."+ NewStr() +
  //  "������ ��� ���� ��������� ����� � ����� ��������� ������ �����."+ NewStr() +
  //  "������� �����������, �� ���������...", "", 0.1, 7.0);
//	LaunchFrameForm();
 //   PostEvent("LAi_event_GameOver", 15000, "s", "land");
    //PostEvent(GAME_OVER_EVENT, 15000);
    GameOver("land");
}

void CapBloodLine_q1_End(string qName)
{

   // pchar.quest.CapBloodLine_q1_2.win_condition.l1          = "location";
   // pchar.quest.CapBloodLine_q1_2.win_condition.l1.location = "CommonBedroom";
   // pchar.quest.CapBloodLine_q1_2.function                  = "CapBloodLine_q1_Complited";
   AddQuestRecord("CapBloodLine_q1", "6");
   Pchar.questTemp.CapBloodLine.stat = "CureMisStid_Complite";
}

void CapBloodLine_q1_Complited(string qName)
{
    ref sld
    trace("CapBloodLine_q1_Complited!!!");
    pchar.quest.CapBloodLineTimer_1.over = "yes";
    sld = &characters[GetCharacterIndex("MisStid")];
//    sld.lifeDay = 0;
    sld.location = "none";
    sld = &characters[GetCharacterIndex("Bridgetown_Mayor")];
    sld.Dialog.CurrentNode = "First time";
    
    int n= FindLocation("Bridgetown_town");
    locations[n].reload.l3.close_for_night = true;//��������� ����������
    AddQuestRecord("CapBloodLine_q1", "7");
    CloseQuestHeader("CapBloodLine_q1");
    //LAi_Fade("", "");
    bQuestCheckProcessFreeze = true;
    WaitDate("",0,0,0, 12, 0); // ������ �����
    SetCurrentTime(7, 0); // ������ ���� 2-� ����� 7 �����
    bQuestCheckProcessFreeze = false;
    SetLaunchFrameFormParam("�� ��������� ����...", "Reload_To_Location", 0.1, 2.0);
    SetLaunchFrameReloadLocationParam("Bridgetown_tavern", "reload","reload2", "");
	LaunchFrameForm();
	CapBloodLine_q2();
}

void CapBloodLine_q2()
{
    ref sld;
    sld = &characters[GetCharacterIndex("Waker")];
    sld.dialog.currentnode = "First time";
    Pchar.questTemp.CapBloodLine.stat = "WakerOffer";
    ChangeCharacterAddressGroup(sld, "Bridgetown_tavern_upstairs", "goto","goto1");
    LAi_SetActorTypeNoGroup(sld);
    LAi_ActorDialog(sld, pchar, "", 2.0, 0);
    AddQuestRecord("CapBloodLine_q2", "2");
    AddQuestRecord("CapBloodLine_q2", "3");

}

void Spain_spyDie(string qName)
{
    ref sld;
    sld = &characters[GetCharacterIndex("Spain_spy")];
    LAi_KillCharacter(sld);
}

void LoginSpain_spy(string qName)
{
    ref sld;
    sld = GetCharacter(NPC_GenerateCharacter("Spain_spy", "shipowner_13", "man", "man", 7, ENGLAND, 2, false));
    sld.dialog.filename = "Quest\CapBloodLine\Bishop.c";
    sld.name = "������";
    sld.lastname = "������";
	sld.dialog.currentnode = "First time";
	sld.city = "Bridgetown_town";
    sld.greeting = "Gr_HUNTER";
    sld.location = "none";
    //SetFantomParamHunter(sld); //������ �����
    FantomMakeCoolFighter(sld, 7, 30, 50, "blade10", "", 20);
	LAi_SetCitizenType(sld);
	sTemp = GetNationNameByType(ENGLAND) + "_citizens";
	LAi_group_MoveCharacter(sld, sTemp);
    PlaceCharacter(sld, "goto", "random_must_be_near"))
    LAi_SetActorTypeNoGroup(sld);
	LAi_ActorDialog(sld, pchar, "",  -1.0, 0);
	chrDisableReloadToLocation = true;
}

void SpainSpyAttack(string qName)
{
    LAi_Fade("", "");
    bQuestCheckProcessFreeze = true;
    WaitDate("",0,0,0, 0, 10); // ������ �����
    bQuestCheckProcessFreeze = false;
    AddQuestRecord("WeaponsForEscape", "8");
    LocatorReloadEnterDisable("BridgeTown_town", "houseSp2", false);
}

void _DeadSolder(string qName)
{
    PlaySound("People Fight\Death_NPC_04.WAV");
    DoQuestFunctionDelay("DeadSolder", 1.0);
}

void DeadSolder(string qName)
{
    ref sld;
    sld = &characters[GetCharacterIndex("CPBQuest_Solder")];
    LAi_KillCharacter(sld);
}

void SpyTalk()
{
    ref sld;
    //sld = &characters[GetCharacterIndex("Spain_spy")];
    //sld.dialog.currentnode = "SSStep_3";
    //LAi_SetActorType(sld);
    //LAi_ActorDialog(sld, pchar, "", -1, 0);

}
////////////////////////////////////////////////////////////////////////////////////////////
void RestoreBridgetown()
{
        Pchar.questTemp.CapBloodLine = false;
        Pchar.model="Blood_5";
        //����������
        ref sld = &characters[GetCharacterIndex("Bridgetown_Mayor")];
        sld.Dialog.Filename = "Common_Mayor.c";
        sld.Dialog.CurrentNode = "First time";
        //�����
        int n= FindLocation("Bridgetown_town");  // �� ������ �������
        locations[n].reload.l3.close_for_night = true;
        locations[n].questSeekCap = 6;
        locations[n].houseEnc = true;
        //������� �����
        n = FindLocation("Bridgetown_tavern");
        locations[n].habitues = 1;
        DeleteAttribute(&locations[n], "notCloseCommonHouse");
        //������� �������
        ChangeBridgeTownDialogs(true);
        bDisableLandEncounters = false;
        chrDisableReloadToLocation = false;
        bDisableFastReload = false;
        Pchar.questTemp.CapBUnarmed = false;
        //���������� �����
        if (CheckAttribute(PChar, "quest.CureLordMovie")) Pchar.quest.CureLordMovie.over = "yes";
        if (CheckAttribute(PChar, "quest.DragunInvansion")) Pchar.quest.DragunInvansion.over = "yes";
        if (CheckAttribute(PChar, "quest.CapBloodLine_q1_1")) Pchar.quest.CapBloodLine_q1_1.over = "yes";
        if (CheckAttribute(PChar, "quest.onPlantation")) Pchar.quest.onPlantation.over = "yes";
        if (CheckAttribute(PChar, "quest.CapBloodDetectBlades1")) Pchar.quest.CapBloodDetectBlades1.over = "yes";
        if (CheckAttribute(PChar, "quest.CapBloodDetectBlades2")) Pchar.quest.CapBloodDetectBlades2.over = "yes";
        if (CheckAttribute(PChar, "quest.CapBloodLineTimer_1")) Pchar.quest.CapBloodLineTimer_1.over = "yes";
        if (CheckAttribute(PChar, "quest.CureMisStid")) Pchar.quest.CureMisStid.over = "yes";
        if (CheckAttribute(PChar, "quest.PrepareToEscape2")) Pchar.quest.PrepareToEscape2.over = "yes";

}
