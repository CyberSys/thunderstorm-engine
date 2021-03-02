// ������ ������� ��� ������� BOAL 07.06.05
// ��������� ����, ����_���
string LastSpeakDate()
{
    return environment.date.day + " " + environment.date.month;
}
// ��� �������� ��� -->
string GetNPCQuestDate(ref npchar, string _type)
{
    if (!CheckAttribute(npchar, "QuestDate." + _type))
    {
        npchar.QuestDate.(_type) = "";
    }
    return npchar.QuestDate.(_type);
}
void SetNPCQuestDate(ref npchar, string _type)
{
    npchar.QuestDate.(_type) = LastSpeakDate();
}
bool CheckNPCQuestDate(ref npchar, string _type)
{
    if (GetNPCQuestDate(npchar, _type) != LastSpeakDate())
    {
        return true;
    }
    return false;
}
// ��� �� <--
void SaveCurrentQuestDateParam(string _quest)
{ // ������ ���� ��������� ������
    aref  arQ;
    makearef(arQ, PChar.(_quest));
    
    arQ.control_day = GetDataDay();
    arQ.control_month = GetDataMonth();
    arQ.control_year = GetDataYear();
    arQ.control_time = GetTime();
}
int GetQuestPastMonthParam(string _quest)
{
    aref  arQ;
    makearef(arQ, PChar.(_quest));

    return GetPastTime("month", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), stf(arQ.control_time),GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
}
int GetQuestPastDayParam(string _quest)
{
    aref  arQ;
    makearef(arQ, PChar.(_quest));
    if (CheckAttribute(PChar, _quest + ".control_year"))
    {
    	return GetPastTime("day", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), stf(arQ.control_time),GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
	} 
	return 0;   	
}
int GetQuestPastTimeParam(string _quest)
{
    aref  arQ;
    makearef(arQ, PChar.(_quest));
    if (CheckAttribute(PChar, _quest + ".control_year"))
    {
    	return GetPastTime("hour", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), stf(arQ.control_time), GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
	}  
	return 0;
}
//navy -->
//���� ��� ���� �������, �.�. ����� ��������� ������.
int GetQuestPastMinutesParam(string _quest)
{
    aref  arQ;
    makearef(arQ, PChar.(_quest));
    if (CheckAttribute(PChar, _quest + ".control_year"))
    {
    	return GetPastTime("minute", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), stf(arQ.control_time), GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
	}
	return 0;
}
//navy <--

// eddy
// ************* ��� �������.
void SaveCurrentNpcQuestDateParam(ref _character, string _quest)
{ // ������ ���� ��������� ������
    aref  arQ;
    makearef(arQ, _character.(_quest));

    arQ.control_day = GetDataDay();
    arQ.control_month = GetDataMonth();
    arQ.control_year = GetDataYear();
    arQ.control_time = GetTime();
}

int GetNpcQuestPastDayParam(ref _character, string _quest)
{
    aref  arQ;
    makearef(arQ, _character.(_quest));
    if (CheckAttribute(_character, _quest + ".control_year"))
    {
        return GetPastTime("day", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), 0,GetDataYear(), GetDataMonth(), GetDataDay(), 0);
    }
    else {return 0;}
}

int GetNpcQuestPastDayWOInit(ref _character, string _quest)
{
    aref  arQ;
    makearef(arQ, _character.(_quest));
    if (CheckAttribute(_character, _quest + ".control_year"))
    {
        return GetPastTime("day", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), 0,GetDataYear(), GetDataMonth(), GetDataDay(), 0);
    }
    else {return 1000;}  // ���� ��� �����, �� ��� ������ ����� ����� ���� ����
}

int GetNpcQuestPastTimeParam(ref _character, string _quest)
{
    aref  arQ;
    makearef(arQ, _character.(_quest));
    if (CheckAttribute(_character, _quest + ".control_year"))
    {
    	return GetPastTime("hour", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), sti(arQ.control_time), GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
	}
	return 0;
}
int GetNpcQuestPastMinutesParam(ref _character, string _quest)
{
    aref  arQ;
    makearef(arQ, _character.(_quest));
    if (CheckAttribute(_character, _quest + ".control_year"))
    {
    	return GetPastTime("minute", sti(arQ.control_year), sti(arQ.control_month), sti(arQ.control_day), stf(arQ.control_time), GetDataYear(), GetDataMonth(), GetDataDay(), GetTime());
	}
	return 0;
}
// *************


void initStartState2Character(ref ch)
{// ����� ���������� � ������ ����� ����, ��������� �������� ����� ����������, ���� ������ ������ �������� � if
    NullCharacter.Siege = "";
    
	ch.SystemInfo.SaveCount = 0;
    ch.SystemInfo.LoadCount = 0;
    ch.SystemInfo.DeadCount = 0;

    ch.SystemInfo.CabinType          = ""; // boal 28.03.05 ����� ���  My_Cabin_Small
    
    ch.questTemp.Contraband.relation = 60;
    DeleteAttribute(ch, "quest.meeting");
    ch.ShipInStock = 0; // �������� � �����
	ch.questTemp.abordage = 0;   // ������� �������
	ch.questTemp.donate   = 0; // ������������� ������

	ch.questTemp.friend_in_tavern = ""; // ����������� boal
	ch.questTemp.RelationAgentMet = "0"; // ����������� boal

	ch.questTemp.NationQuest = ""; // ������ ������� �������

	ch.GenQuest.BrothelCount = "0"; // ������� ��������� �������
	//ch.GenQuest.Hunter2Pause   = false; // ������� ����� � ����� �� ���� - �� ������ �����
	ch.I_MAP_SHOW = false;  // boal ����� ������� �� ���������
	ch.CameraHoldPos = false;
	ch.CameraShipNPC = 0;// ����� ���������� ��� ������ 0 - ��
	ch.GenQuestFort.FarLocator = true; // ��� ������� ������� �������� � �����

	// ������ ���������
	ch.GenQuest.GhostShip.DeadByMe       = 0;
	ch.GenQuest.GhostShip.KillMe         = 0;
	ch.GenQuest.GhostShip.AskAbout       = 0; // � ������� ��� ���� �������� �� ���������
	ch.GenQuest.GhostShip.lastspeak_date = "";
    GhostShipInit();
    
    ch.GenQuest.Find_Merchant.lastspeak_date = "";
    ch.GenQuest.ConvoyPassenger = ""; //��������� ����������� �� ������ �������� ���������

	// ==> ��������� ������ ������� �������
	ch.questTemp.State = "";
	// ==> ���-�� ���� �������� ������ ���������� ������ �������, �������� - ��� ���
	ch.questTemp.Waiting_time = "2";
	// ==> ����� �������� ������, ����������� ����� ������, �.�. ������ ������� �� ������ ������� ��������� � ������.
	ch.questTemp.CurQuestNumber = "2";
	//��������� �������� � ���� ��� ����������� �����, ���, � ����� ��� ���� :)
	InitTravelMap();
	// ==> �������� �� ��������� �������� ����� �������
	pchar.RomanticQuest.HorseCheck = -1;
	// ==> ����� ��������.
    ch.questTemp.Ascold = "canTakeQuest"; // ���������, ���� ����� "", �� ���� �� ���� �����.
	// ==> ����� ������ ����.
    ch.questTemp.Azzy = "1";
	// ==> ����� ������ �������� � �������� �������� �� ��������.
	SelectAscoldRendom();
	// ==> ����� ��������
	IsabellaInit()
	// ==>  ����� � ����� ��������� 
	BerglarsInit();
	// ==>  �����
	PoormansInit();
	// ==>  ���-�� ����, ��������� �������
	pchar.questTemp.HorseLoot = 0;
	// ==>  ���-�� ��������� �� �������
	pchar.questTemp.HorseQty = 0;
	// ==> ���������� �� ��� � ���������� ��������
	PearlGen_SetSantaCatalina("none");
	PearlGen_SetTeno("none");
	// ==> ��������������
	QuestionsInit();
	// ==> �������� ���� �� ������ ������
	ch.questTemp.jailCanMove = false;
	// ==> ������� ���
	ch.questTemp.LSC = false;
	ch.questTemp.LSC.checkBoxes = false; //���� �������� �������� � �������
	pchar.quest.FirstLoginLostShipsCity.win_condition.l1 = "location";
	pchar.quest.FirstLoginLostShipsCity.win_condition.l1.location = "LostShipsCity_town";
	pchar.quest.FirstLoginLostShipsCity.function = "FirstLoginLostShipsCity";
	// ==> ����� ��������� ����� �����
	ch.questTemp.BlueBird = "begin";
	ch.questTemp.BlueBird.City = "";
	ch.questTemp.BlueBird.count = 0;
	// ==> ��������� ����� �� �������
	ch.questTemp.Sharp = "begin";
	ch.questTemp.Sharp.City = "";
	ch.questTemp.Sharp.count = 0; //������� ���-�� �������
	ch.questTemp.Sharp.brothelChance = 1; //�������, ���� ��������� ����� ����� � �������
	ch.questTemp.Sharp.SeekSpy = "begin"; //��������� ����� �� ������ ����������� �����
	// ==> ��������� �������
	PiratesLineInit();
	// ==> ����� ������ �������, ������ �� ������ �� ��������
	ch.questTemp.different = "free";
	// ==> ������� ����� homo
	ch.questTemp.CapBloodLine = true;
	ch.questTemp.CapBUnarmed = false; //�� ����� �� �������� - false
}

// ��������� ������ � ���������� � ��� �� ��������� ��� �� ����� �� ������� �������.
void CharacterIntoCompanionAndGoOut(ref _mainCharacter, ref _compCharacter, string _group, string _locator, int _timeToGoOut, bool _compRemove)
{
    if (!bDisableFastReload) //����� �� ������� ������, ���� �� ���������� ������ �������
	{
		bDisableFastReload = true;
		_mainCharacter.GenQuest.CallFunctionParam.CharacterIntoCompanion.FastReload = true;
	}
	if (_group == "")    {_group = "reload";}
    if (_locator == "")
    {
    	float locx, locy, locz;
        GetCharacterPos(_compCharacter, &locx, &locy, &locz);
        _locator = LAi_FindNearestFreeLocator(_group, locx, locy, locz);
    }
    LAi_ActorGoToLocation(_compCharacter, _group, _locator, "none", "", "", "", _timeToGoOut);
    _mainCharacter.GenQuest.CallFunctionParam = "Character_into_companion";
    _mainCharacter.GenQuest.CallFunctionParam.CharacterIntoCompanion.Id = _compCharacter.id;
    _mainCharacter.GenQuest.CallFunctionParam.CharacterIntoCompanion.Remove = _compRemove;
    DoQuestCheckDelay("CallFunctionParam", _timeToGoOut);
}

void Character_into_companion()    // ��������� � ������ ����.
{
    ref NPChar = characterFromID(PChar.GenQuest.CallFunctionParam.CharacterIntoCompanion.Id);
    SetCompanionIndex(pchar, -1, getcharacterIndex(PChar.GenQuest.CallFunctionParam.CharacterIntoCompanion.Id));
    if (PChar.GenQuest.CallFunctionParam.CharacterIntoCompanion.Remove == false)    {SetCharacterRemovable(npchar, false);}
	//������� ������, ������ ���� �� ���������� ���� �������
	if (CheckAttribute(PChar, "GenQuest.CallFunctionParam.CharacterIntoCompanion.FastReload")) bDisableFastReload = false;
}

string GetBookData(int day, int mn, int year)
{
	string result = "";

	if (day < 10)
	{
	    result = result + "0";
	}
	result = result + day + ".";
	if (mn < 10)
	{
	    result = result + "0";
	}
	result = result + mn + "." + year;

    return result;
}

// ========>>>>>> ����� ����, ������� ����������� ���� �� ������������ =================
void AzzyCheckSumControl()
{
    int   QtyKilled, Result;
    float CurHP, MaxHP;

	if (CheckAttribute(pchar, "questTemp.Azzy.Immortal"))
    {
        QtyKilled = Statistic_AddValue(pchar, "Solder_s", 0) +
                    Statistic_AddValue(pchar, "Solder_g", 0) +
                    Statistic_AddValue(pchar, "Warrior_s", 0) +
                    Statistic_AddValue(pchar, "Warrior_g", 0) +
                    Statistic_AddValue(pchar, "Citizen_s", 0) +
                    Statistic_AddValue(pchar, "Citizen_g", 0) +
                    Statistic_AddValue(pchar, "Monster_s", 0) +
                    Statistic_AddValue(pchar, "Monster_g", 0) -
                    sti(pchar.questTemp.Azzy.Immortal.EarlyKilled);
        pchar.questTemp.Azzy.Immortal.AllQuota = makeint(pchar.questTemp.Azzy.Immortal.AllQuota) + sti(pchar.questTemp.Azzy.Immortal.DayQuota);
        Result = QtyKilled - sti(pchar.questTemp.Azzy.Immortal.AllQuota);
        if (Result < 0)
        {
            MaxHP = LAi_GetCharacterMaxHP(pchar);
            CurHP = LAi_GetCharacterHP(pchar);
            MaxHP = MaxHP - 5;
            pchar.questTemp.Azzy.Immortal.Penalty = makeint(pchar.questTemp.Azzy.Immortal.Penalty) + 5;
            if (!CheckAttribute(pchar, "questTemp.Azzy.OK"))
            {
                pchar.questTemp.Azzy = "HowToKnowAzzy";
                pchar.questTemp.Azzy.OK = 1;
            }
            if (MaxHP < 1)
            {
                LAi_KillCharacter(pchar);
                Log_SetStringToLog("��������� �� ���: ���� ��������� ���� ���� ������������ �������� � ���� ������ ����� �����. ����� ���������� � ��.");
            }
            else
            {
                Log_SetStringToLog("������ ����������� � �������� ��������. ����������, ������������!");
            }
            LAi_SetHP(pchar, CurHP, MaxHP);
        }
        if (GetQuestPastDayParam("questTemp.Azzy.Immortal") > 30)
        {
            pchar.questTemp.Azzy.Immortal.DayQuota = makeint(pchar.questTemp.Azzy.Immortal.DayQuota) * 2;
            SaveCurrentQuestDateParam("questTemp.Azzy.Immortal");
        }
        pchar.questTemp.Azzy.Immortal.CurKilled = QtyKilled;
        pchar.questTemp.Azzy.Immortal.CurBalance = Result;
        DeleteQuestHeader("Azzy_HellSign");
		if (sti(pchar.questTemp.Azzy.Immortal.Penalty) > 0)
		{		
			AddQuestRecord("Azzy_HellSign", "2");	
			AddQuestUserData("Azzy_HellSign", "Penalty", pchar.questTemp.Azzy.Immortal.Penalty);
		}
		AddQuestRecord("Azzy_HellSign", "1");
		AddQuestUserData("Azzy_HellSign", "sName", GetMainCharacterNameGen());
		AddQuestUserData("Azzy_HellSign", "DayQuota", pchar.questTemp.Azzy.Immortal.DayQuota);
		AddQuestUserData("Azzy_HellSign", "AllQuota", pchar.questTemp.Azzy.Immortal.AllQuota);
		AddQuestUserData("Azzy_HellSign", "CurKilled", pchar.questTemp.Azzy.Immortal.CurKilled);
		AddQuestUserData("Azzy_HellSign", "CurBalance", pchar.questTemp.Azzy.Immortal.CurBalance);
    }
}
// <<<<<======== ����� ����, ������� ������������ ���� �� ������������ =================

// ��������� ������� ����� � ���������
bool isBadReputation(ref _pchar, int _rep)
{
    if (GetCharacterPerkUsing(_pchar, "Trustworthy") || sti(_pchar.reputation) >= _rep)
    {
        return false;// good guy
    }
    return true;// bad guy
}
// ��������� ������ ����� �� ����� ��������
void FillAboardCabinBox(ref _location, ref _npchar)
{
    int     iTemp;
    bool    ok;
	
	_location.box1 = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear()); // �����, ���� �� �����������
	// ����� ��������, ��� � ������� ���������� ��������� ����, ���� ��� ����������� ����� � ������ ���� ������
	
	ok = true;
	// ��������� ��� �����
	if (_npchar.id == "GhostCapt" && CheckAttribute(pchar , "GenQuest.GhostShip.LastBattle"))
	{
        // ���� ��� � �������
        DeleteAttribute(_location, "box1");
        // ������ ����� �����
        _location.box1.items.indian11 = 1;
        _location.box1.money = 66666;
        if (CheckAttribute(PChar, "GenQuest.GhostShip.TakeShotgun"))
        {
            DeleteAttribute(PChar, "GenQuest.GhostShip.TakeShotgun");
            _location.box1.items.pistol7 = 1;
        }
        ok = false;
	}
	// �������� ����������
	/*if (CheckAttribute(_npchar, "Ship.Name") && _npchar.Ship.Name == FUNY_SHIP_NAME)
	{
        // �� ���� ��� � �������
        _location.box1.items.mineral4 = 25;
        ok = false;
        PChar.questTemp.FUNY_SHIP_FIND = true;
	}       */
	// ����� �4, ����., ������ � ����������� ������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.EngLineQ4_PutLetter"))
	{
        DeleteAttribute(_location, "box1");
        ChangeItemDescribe("letter_1", "itmdescr_letter_1_EngLineQ4");
        _location.box1.items.letter_1 = 1;
        _location.box1.money = 7460;
        _location.box1.items.map_Curacao = 1;
        ok = false;
	}
	// ����� �6, ����., ������ � ��������� �������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.EngLineQ6_PutMoney"))
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 140000;
        _location.box1.items.map_venesuela = 1;
        _location.box1.items.spyglass3 = 1;
        _location.box1.items.potion4 = 10;
        _location.box1.items.indian7 = 1;
        ok = false;
	}
    // ����� �8, ����., ������ � ��������� ��������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.ELQ8_PutLetter"))
	{
        DeleteAttribute(_location, "box1");
        ChangeItemDescribe("letter_2", "itmdescr_letter_2_EngLineQ8");
        _location.box1.items.letter_2 = 1;
        _location.box1.money = 2203;
        _location.box1.items.map_cuba = 1;
        _location.box1.items.spyglass2 = 1;
        _location.box1.items.potion2 = 7;
        _location.box1.items.potionwine = 2;
        ok = false;
	}
    // ����� �9, ����., ������ � ��������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.ELQ9_Diffrent"))
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 853000;
        _location.box1.items.blade28 = 1;
        _location.box1.items.pistol6 = 1;
        _location.box1.items.pistol4 = 1;
        _location.box1.items.potion4 = 10;
        _location.box1.items.chest = 1;
        _location.box1.items.icollection = 1;
        _location.box1.items.map_cuba = 1;
        _location.box1.items.jewelry4 = 25;
        _location.box1.items.cirass3 = 1;
        ok = false;
	}
    // ����� �7, ������� , �������� �������, ������ ���� ������������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.SLQ7_Diffrent"))
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 753400;
        _location.box1.items.blade28 = 1;
        _location.box1.items.pistol6 = 1;
        _location.box1.items.pistol4 = 1;
        _location.box1.items.potion2 = 10;
        _location.box1.items.potionwine = 1;
        _location.box1.items.jewelry4 = 25;
        _location.box1.items.cirass3 = 1;
		_location.box1.items.cirass4 = 1;
        ok = false;
	}
    // ����� �8, ������� , �������� ������, ������ ���� ������������
	if (_npchar.id == "CaptainAttack_1" && CheckAttribute(PChar, "questTemp.SLQ8_Diffrent"))
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 996400;
        _location.box1.items.spyglass4 = 1;
        _location.box1.items.potionrum = 2;
        _location.box1.items.pistol4 = 5;
        _location.box1.items.potion3 = 11;
        _location.box1.items.chest = 1;
        _location.box1.items.icollection = 1;
        _location.box1.items.jewelry5 = 50;
		_location.box1.items.indian13 = 1;
        ok = false;
	}
	// ����� ��������.
	if (CheckAttribute(PChar, "questTemp.Ascold.Ship"))
	{
        if (_npchar.id == pchar.questTemp.Ascold.Ship)
        {
            DeleteAttribute(_location, "box1");
            _location.box1.items.ShipsJournal = 1;
            ok = false;
        }
	}
	// �����  homo 22/10/06	 
	if (findsubstr(_npchar.id, "SiegeCap_" , 0) != -1)
	{
	    aref aData, itm;
	    makearef(aData, NullCharacter.Siege);
	    string sGroup = "Sea_"+NationShortName(sti(aData.nation))+"SiegeCap_1";
	    ref rchar = Group_GetGroupCommander(sGroup);  //����� �������� ���������, �� �������� �� ������
	    Log_TestInfo( _npchar.id +" == "+ rchar.id);
		if (_npchar.id == rchar.id && CheckAttribute(aData, "loot"))
		{
	        DeleteAttribute(_location, "box1");
	        _location.box1.money = sti(aData.loot);
	        _location.box1.items.potionrum = 3;
	        _location.box1.items.potionwine = 1;
	        string idmap = "map_"+aData.island;
            if (Items_FindItem(idmap, &itm) != -1)
            {
                _location.box1.items.(idmap) = 1;
            }

	        ok = false;
		}
	}
	//homo =>
    if (findsubstr(_npchar.id, "_QuestMerchant" , 0) != -1)
    {
        if (trap)
        {
            DeleteAttribute(_location, "box1");
            _location.box1.money = 7000;
            _location.box1.items.blade20 = 1;
            ok = false;
        }
    }
    
    if (_npchar.id == "Head_of_Gold_Squadron" )
    {

        DeleteAttribute(_location, "box2");
        _location.box2.money = 2212;
        _location.box2.items.map_normal = 1;
        ok = false;

    }
    //homo <=
    // �������, ����� �7. ������������� � �����
	if (_npchar.id == "QuestCap_PL7")
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 3983;
        _location.box1.items.jewelry1 = 763;
        _location.box1.items.jewelry2 = 562;
        _location.box1.items.jewelry3 = 1771;
        _location.box1.items.jewelry4 = 678;
        _location.box1.items.jewelry6 = 236;
        _location.box1.items.jewelry7 = 321;
        _location.box1.items.jewelry14 = 121;
		_location.box1.items.jewelry15 = 45;
		_location.box1.items.jewelry18 = 311;
        ok = false;
	}
    if (ok) // �� ���������
    {
    	// ��� ��� ����
    	iTemp = GetCharacterShipClass(_npchar);
    	if (CheckAttribute(_npchar, "Ship.Mode") && _npchar.Ship.Mode == "Trade") // ��������
    	{
    	    _location.box1.money = (10 - iTemp) * 200 + rand(10 - iTemp) * 2000 + rand(10)*50 + rand(6 - iTemp) * 4000;
    	}
    	else
    	{
    		_location.box1.money = (10 - iTemp) * 90 + rand((10 - iTemp) * 2000);
    	}
    }
}

// ==> ��� ������� ���� ��� �������� �����������.
void FantomMakeCoolSailor(ref _Character, int _ShipType, string _ShipName, int _CannonsType, int _Sailing, int _Accuracy, int _Cannons)
{
    _Character.Ship.Cannons.Type = _CannonsType;
	_Character.skill.Sailing  = GetCoffDiff(_Sailing, SKILL_MAX);
	_Character.skill.Accuracy = GetCoffDiff(_Accuracy, SKILL_MAX);
	_Character.skill.Cannons  = GetCoffDiff(_Cannons, SKILL_MAX);
    
    _Character.DontRansackCaptain = true; //��������� �� �������
    _Character.SinkTenPercent     = false; // �� ������ ��� 10%, �� ������� � ���
    _Character.AboardToFinalDeck  = true; // ������� ������
	_Character.DontClearDead      = true;
	_Character.SaveItemsForDead   = true;
	_Character.AlwaysSandbankManeuver = true;

    _Character.Ship.Type = GenerateShip(_ShipType, true);
    if (_ShipName == "none" || _ShipName == "") {SetRandomNameToShip(_Character)}
    else {_Character.Ship.Name = _ShipName}

    SetBaseShipData(_Character);
    SetCrewQuantityFull(_Character);
    Fantom_SetBalls(_Character, "pirate");

	SetCharacterPerk(_Character, "FastReload");
	SetCharacterPerk(_Character, "HullDamageUp");
	SetCharacterPerk(_Character, "SailsDamageUp");
	SetCharacterPerk(_Character, "CrewDamageUp");
	SetCharacterPerk(_Character, "CriticalShoot");
	SetCharacterPerk(_Character, "LongRangeShoot");
	SetCharacterPerk(_Character, "CannonProfessional");
	SetCharacterPerk(_Character, "ShipDefenseProfessional");
	SetCharacterPerk(_Character, "ShipSpeedUp");
	SetCharacterPerk(_Character, "ShipTurnRateUp");

    DeleteAttribute(_Character, "ship.sails");// ������ ���� �� �������
    DeleteAttribute(_Character, "ship.blots");
    DeleteAttribute(_Character, "ship.masts");// ������� ������ �����
    DeleteAttribute(_Character, "Killer.status"); // ����� �������� "��� �������� �� �������"
    _Character.questTemp.abordage = 0; //����� �������� ������ ���������� �������
   	DeleteAttribute(_Character, "Abordage.Enable"); //����� ������������ ��������
}

void FantomMakeCoolFighter(ref _Character, int _Rank, int _Fencing, int _Pistol, string _Blade, string _Gun, float _AddHP)
{
    _Character.rank = GetCoffDiff(_Rank, 1000);
    _Character.skill.Fencing = GetCoffDiff(_Fencing, SKILL_MAX);
    _Character.Skill.FencingLight  = GetCoffDiff(sti(_Character.skill.Fencing), SKILL_MAX);
    _Character.Skill.FencingHeavy  = GetCoffDiff(sti(_Character.skill.Fencing), SKILL_MAX); 
    _Character.skill.Pistol = GetCoffDiff(_Pistol, SKILL_MAX);
	_Character.chr_ai.hp = stf(_Character.chr_ai.hp) + GetCoffDiff(_AddHP, 5000);
	_Character.chr_ai.hp_max = stf(_Character.chr_ai.hp_max) + GetCoffDiff(_AddHP, 5000);
	SetCharacterPerk(_Character, "Energaiser"); // ������� ���� ���� 1.5 � �������� �������, ������ �� � ������ �������
	SetCharacterPerk(_Character, "SwordplayProfessional");
	SetCharacterPerk(_Character, "AdvancedDefense");
	SetCharacterPerk(_Character, "CriticalHit");
	SetCharacterPerk(_Character, "Sliding");
	DeleteAttribute(_Character, "Items");
	_Character.SuperShooter  = true;
    GiveItem2Character(_Character, _Blade);
    _Character.equip.blade = _Blade;
    TakeNItems(_Character,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
    GiveItem2Character(_Character, _Gun);
    _Character.equip.gun = _Gun;
    TakeNItems(_Character,"bullet", 100);
	GiveItem2Character(_Character,"spyglass3");
	EquipCharacterbyItem(_Character, "spyglass3");
    FaceMaker(_Character);
    SetNewModelToChar(_Character);  // ���������� ������ �� ����
}

int GetCoffDiff(float _num, int _maxRange)
{
	switch (MOD_SKILL_ENEMY_RATE)
	{
		case  1: _num *= 0.6;  break;
		case  2: _num *= 0.75; break;
		case  3: _num *= 0.85; break;
		case  4: _num *= 0.9;  break;
		case  5: _num *= 0.95; break;
		case  6: _num *= 1;    break;
		case  7: _num *= 1.1;  break;
		case  8: _num *= 1.2;  break;
		case  9: _num *= 1.3;  break;
		case 10: _num *= 1.5;  break;
	}
	_num += 0.5;
	if (_num > _maxRange) return _maxRange;
	else return sti(_num);
}
// ���������� �������� � ��������� �� ������
bool SetLocationQuestRandItem(int _index, aref _location, string _locatorName, aref al) // al - ����� �������� �� ������, ��������� ���� �� ��������� �������
{   // ����� ������������� �� ����� - ��� �������� � �������
	string  lastSpawnTimeString;
    int     n;
	string  itemId;
	aref checkAref

	/* ������
 	pchar.GenQuestRandItem.QC_Port = true;
    pchar.GenQuestRandItem.QC_Port.randitem1 = "pistol6";
    // ���� ����� ���� ���� ������
	pchar.GenQuestRandItem.QC_Port.stay = true; - ������ ����� ��� ����� ������ �� ������
	
    QC_Port - ������� ���
    randitem1 - �������
    pistol6 - �������
    
    ���� ����� ����� ��������� ���������, ��
    pchar.GenQuestRandItem.QC_Port.randitem1 = "pistol6";
    pchar.GenQuestRandItem.QC_Port.randitem2 = "pistol1";
 	*/
	lastSpawnTimeString = _location.id;
    if (CheckAttribute(pchar , "GenQuestRandItem." + lastSpawnTimeString) && CheckAttribute(pchar , "GenQuestRandItem." + lastSpawnTimeString +"."+_locatorName))
    {
        itemId = pchar.GenQuestRandItem.(lastSpawnTimeString).(_locatorName);
        if (!CheckAttribute(pchar , "GenQuestRandItem." + lastSpawnTimeString +".stay"))
        {
			//--> fix eddy. ������ ���� �������� � ��������� ���������
			DeleteAttribute(pchar , "GenQuestRandItem." + lastSpawnTimeString + "." + _locatorName); // ���� ���� ���� ���
			makearef(checkAref,  pchar.GenQuestRandItem.(lastSpawnTimeString));
			if (GetAttributesNum(checkAref) == 0) DeleteAttribute(pchar , "GenQuestRandItem." + lastSpawnTimeString);
        }
        // ������ ���� �������!!!!!!!!!!!!
        n = SetRandItemShow(_index, al, itemId); // ������� 3� ������ � �������, ���� ������ ������ ����
		if (n != -1)
		{
            Log_TestInfo("SetLocationQuestRandItem");
            lastSpawnTimeString = "LastSpawnTime"+_index;
		    _location.(lastSpawnTimeString) = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear());
            lastSpawnTimeString = "RandItemType"+_index;
        	_location.(lastSpawnTimeString) = n;
            return true;
        }
    }
    
    return false;
}
// ��������� ������
int SetRandItemShow(int _index, aref al, string _itemId)
{
    int     n;
    aref    randItem;
    
    n = Items_FindItem(_itemId, &randItem);
	if (n != -1)
	{
        if (!CheckAttribute(randItem, "model") || randItem.model == "")
    	{
    		Trace("SetRandItemShow: no model for item "+randItem.id);
    		return -1;
    	}
	    Items_LoadModel(&randItemModels[_index],  randItem);
    	SendMessage(&randItemModels[_index], "lffffffffffff", MSG_MODEL_SET_POSITION, makeFloat(al.x), makeFloat(al.y), makeFloat(al.z), makeFloat(al.vx.x), makeFloat(al.vx.y), -makeFloat(al.vx.z), makeFloat(al.vy.x), makeFloat(al.vy.y), -makeFloat(al.vy.z), makeFloat(al.vz.x), makeFloat(al.vz.y), -makeFloat(al.vz.z));

        return n;
    }
    return n;
}
// ��������� ������
bool SetLocationQuestBox(ref _location, string _locatorName)
{   // ����� ������������� �� ����� - ��� �������� � �������
    bool    ok = false;
    string  locId;
    aref    arToBox;
    aref    arFromBox;

    /* ������
 	pchar.GenQuestBox.Havanna_town_04 = true;
    pchar.GenQuestBox.Havanna_town_04.box1.items.jewelry1 = 4;
    pchar.GenQuestBox.Havanna_town_04.box1.items.mineral2 = 10;
    // ���� ����� ������, ����� �� ���������
	pchar.GenQuestBox.Havanna_town_04.box1.money          = 100;
    // ���� ����� ���� ���� ������
	pchar.GenQuestBox.Havanna_town_04.stay = true; - ������ ����� ��� ����� ������ �� ������

    Havanna_town_04 - ������� ���
    box1 - �������
    items - ������ ���������
    
    ���� ����� ����� ��������� ���������, ��
    pchar.GenQuestBox.Havanna_town_04.box2.items.jewelry1 = 34;
 	*/
 	locId  = _location.id;
    if (CheckAttribute(pchar , "GenQuestBox." + locId) && CheckAttribute(pchar , "GenQuestBox." + locId + "." + _locatorName))
    {
        Log_TestInfo("SetLocationQuestBox");

        makearef(arToBox, _location.(_locatorName));
        makearef(arFromBox, pchar.GenQuestBox.(locId).(_locatorName));
        CopyAttributes(arToBox, arFromBox);
        _location.(_locatorName) = Items_MakeTime(GetTime(), GetDataDay(), GetDataMonth(), GetDataYear()); // ������, ���� �� ��������� �������� ��� ������ �� �������
        if (!CheckAttribute(pchar , "GenQuestBox." + locId +".stay"))
        {
			//--> fix eddy. ������ ���� �������� � ��������� ���������
			DeleteAttribute(pchar , "GenQuestBox." + locId + "." + _locatorName); // ���� ���� ���� ���
			makearef(arToBox,  pchar.GenQuestBox.(locId));
			if (GetAttributesNum(arToBox) == 0) DeleteAttribute(pchar , "GenQuestBox." + locId);
        }
        return true;
    }
    return false;
}

// ==> ����� ��������\�������� ��������� ������. ���� _flag=true - ������� �������, ���� _flag=false - �������.
void LocatorReloadEnterDisable(string _locationID, string _locator, bool _flag)
{
    aref arDis, arRld;
	makearef(arRld, Locations[FindLocation(_locationID)].reload);
	int	Qty = GetAttributesNum(arRld);
	for (int i=0; i<Qty; i++)
	{
		arDis = GetAttributeN(arRld, i);
        if (arDis.name == _locator)
        {
    		if (_flag) arDis.disable = 1;
    		else DeleteAttribute(arDis, "disable");
            break;
        }
	}
}

//===>>> ������� �� ������ � ����������� �������. ����. ================================================================
// == _strNormal - ������� ������ �������
// == _strBad1, _strBad2, _strBad3 - ������ ������� ����������,  "" � "none" - ������� �����.
// == _kind - ��� ����������: "repeat" - ��������� �� ��������� ���� ��� �������, ��� ��� ������ �� ���������� (�� ������������ ����-����).
//                            "quest"  - ��������� ����������, �� ��������� �� ��������� ����, �� ����� ���� ����� ���� ���������� ����� �������� �� �����.
//                            "block"  - ������� ��� �� ��������� ����� ��� ��������� �����, ��� ����, ����� ���������� ���������, �� �� ��������. ����� ������ �� ������.
//                            "cycle"  - ������ ���� �� ����������, ����� �� ��������. �� ����. ���� ��� ���������.
// == _terms ��� "repeat" � "quest" - ����� � ���� ���������� ����-����� ����� ������������� �����-�����, ���� == 0, �� ������ �������� (���������� ������ ����� �������).
//    _terms ��� "block" - ����� �������� � ����� � ����, ���� 0 - ��������.
// == _character � _Node - npchar � Dialog.CurrentNode
string NPCStringReactionRepeat(string _strNormal, string _strBad1, string _strBad2, string _strBad3, string _kind, int _terms, ref _character, string _Node)
{
    _Node = stripblank(_Node); //fix spaces
	string strBack;
    string strTemp = "quest.repeat." + _Node;
    if (!CheckAttribute(_character , strTemp))
    {
        _character.quest.repeat.(_Node) = 0;
        _character.quest.repeat.(_Node).ans = 0;
    }
    if (_kind == "cycle" || _kind == "repeat") //�� ����.���� ��� ���������.
    {
        if (GetNpcQuestPastDayParam(_character, strTemp) >= 1)
        {
            _character.quest.repeat.(_Node) = 0;
            _character.quest.repeat.(_Node).ans = 0;
        }
    }
    else
    {
        if (GetNpcQuestPastDayParam(_character, strTemp) >= _terms && _terms!=0 && _kind != "quest") //������ �� "block"
        {
            _character.quest.repeat.(_Node) = 0;
            _character.quest.repeat.(_Node).ans = 0;
        }
    }
    switch(_character.quest.repeat.(_Node))
	{
        case "0":
            strBack = _strNormal;
            _character.quest.repeat.(_Node) = 1;
            _character.quest.repeat.(_Node).ans = 0;
            SaveCurrentNpcQuestDateParam(_character, strTemp);
        break;
        case "1":
            if (_strBad1 == "none" || _strBad1 == "")
            {
                strBack = LinkRandPhrase("� �� ���� �������� �� ����� � ��� �� �� ��������� ���.",
                          "�� ��� �������� �� ����.",
                          "��� ���� �� ��� ���������.");
            }
            else
            {
                strBack = _strBad1;
            }
            _character.quest.repeat.(_Node) = 2;
            _character.quest.repeat.(_Node).ans = 1;
        break;
        case "2":
            if (_strBad2 == "none" || _strBad2 == "")
            {
                strBack = LinkRandPhrase("��� ��� ��������, ��� � �� ���� �������� �� ����� � ��� �� �� ������ ���!",
                          "��������, �� ��� �������� �� ����!",
                          "� �� ������, �� ��� ��� ���������. �������� � �������?");
            }
            else
            {
                strBack = _strBad2;
            }
            _character.quest.repeat.(_Node) = 3;
            _character.quest.repeat.(_Node).ans = 2;
        break;
        case "3":
            if (_strBad3 == "none" || _strBad3 == "")
            {
                strBack = LinkRandPhrase("� �� ������� ����� � ���� ��������.",
                          "�� ���� � ����� ������ �������������!",
                          "�� ���� �������� ��������, �������� ������� �����.");
            }
            else
            {
                strBack = _strBad3;
            }
            // ==> ������� �� ������ ����������.
            if (_kind == "cycle") //����� �� ������ ������.
            {
			    _character.quest.repeat.(_Node) = 0;
            }
			else
            {
                if(_kind != "block") //���� block, �� ����� � ��������� ������
                {
                    _character.quest.repeat.(_Node) = 4;
                    CharacterAddAngry(_character, _Node, _kind, _terms);
    			}
            }
            _character.quest.repeat.(_Node).ans = 3;
        break;
        case "4":
            strBack = "NPCStringReactionRepeat. ���� �� ������� ���, �� ��� ����� ���. �������� �������� ������� ������, ��� ����."
        break;
	}
    return strBack;
}

string HeroStringReactionRepeat(string _strNormal, string _strBad1, string _strBad2, string _strBad3, ref _character, string _Node)
{
    _Node = stripblank(_Node); //fix spaces
	string strBack;
    int Temp = sti(_character.quest.repeat.(_Node).ans);
    switch(Temp)
	{
        case "0":
            strBack = _strNormal;
        break;
        case "1":
            if (_strBad1 == "none" || _strBad1 == "")
            {
                strBack = LinkRandPhrase("������, ��������.",
                          "��� �������.",
                          "�� �� ������, ������...");
            }
            else
            {
                strBack = _strBad1;
            }
        break;
        case "2":
            if (_strBad2 == "none" || _strBad2 == "")
            {
                strBack = LinkRandPhrase("������, ������. ������ �� ��������.",
                          "��� �������, ������ �� ���� �������.",
                          "������, ���-�� � ������� �� � �����... ������ � ���� �������������...");
            }
            else
            {
                strBack = _strBad2;
            }
        break;
        case "3":
            if (_strBad3 == "none" || _strBad3 == "")
            {
                strBack = LinkRandPhrase("��� ������.",
                          "����� ����...",
                          "�������� ������.");
            }
            else
            {
                strBack = _strBad3;
            }
        break;
        case "4":
            strBack = "HeroStringReactionRepeat. ���� �� ������� ���, �� ��� ����� ���. �������� �������� ������� ������, ��� ����."
        break;
	}
    return strBack;
}

// �������� ������ ��� ���������� ����: _GoNode1 � _GoNode2, ������ �� �����, �.�. ���� ����� � �����-����.
// �������, ������� ������ ���� ��� ������ � �������.
string DialogGoNodeRepeat(string _NormalNode, string _GoNode1, string _GoNode2, string _GoNode3, ref _character, string _Node)
{
    _Node = stripblank(_Node); //fix spaces
	string strBack, Temp;
    Temp = sti(_character.quest.repeat.(_Node).ans);
    switch(Temp)
	{
        case "0":
            strBack = _NormalNode;
        break;
        case "1":
            if (_GoNode1 == "none" || _GoNode1 == "")
            {
                strBack = "exit";
            }
            else
            {
                strBack = _GoNode1;
            }
        break;
        case "2":
            if (_GoNode2 == "none" || _GoNode2 == "")
            {
                strBack = "exit";
            }
            else
            {
                strBack = _GoNode2;
            }
        break;
        case "3":
            if (_GoNode3 == "none" || _GoNode3 == "")
            {
                strBack = "exit";
            }
            else
            {
                strBack = _GoNode3;
            }
        break;
        case "4":
            strBack = "exit"
            Log_SetStringToLog("DialogGoNodeRepeat#4. ���� � ��� ����������� ������ ������, �� ��� ����� ���. ����� �������� ������� ������ ���, ��� ����.");
        break;
    }
    return strBack;
}

// _Node - ��� ����, ������������� ������� �����. ���� ����� ���������� �� � ���������� �����, �� == "none".
void CharacterAddAngry(ref _character, string _Node, string _kind, int _terms)
{
    if (_kind == "repeat" || _kind == "quest")
    {
        DeleteAttribute(_character, "angry");
        _character.angry.kind = _kind;
        _character.angry.name = _Node; //� ��� ���� ����, ��� ���������� ������ �����
        if (_terms != 0) // ���� == 0, �� ����� ������������ � �������� �������.
        {
            _character.angry.terms = _terms;
            SaveCurrentNpcQuestDateParam(_character, "angry.terms");
            if (_terms < 10) // ������ �������������� �� ����������, ���� �/�
            {
                _character.angry.QtyMoney = (rand(10)+1) * 500;
            }
            else
            {
                _character.angry.QtyMoney = (rand(10)+1) * 5000;
            }
        }
        else
        {
            _character.angry.QtyMoney = (rand(10)+1) * 10000;
        }
    }
    else {Log_Info("CharacterAddAngry. ���� �� ������ ��� ������, �� ��� ����� ���. ��������, �������� ������� ������ ���, for Eddy.");}
}

void CharacterDelAngry(ref _character)
{
    DeleteAttribute(_character, "angry");
    DeleteAttribute(_character, "quest.repeat");
}

//� �������� ���������� ���������� ���������� ����� �� �������, ����� ���������������.
//���� ���� ������ ��� � �����, ����� � ���� ������������ �����, �� ���� ����� ���������� ����� ����������.
//�����. ������ ���� ���������� ������ �������.
void QuestSetCurrentNode(string _chID, string _Node)
{
    ref chref = characterFromID(_chID);
    if (CheckAttribute(chref, "angry")) chref.dialog.TempNode = _Node;
    else chref.dialog.currentnode = _Node;
}

// ����� �� ���� ���
string NPCharSexPhrase(ref _character, string StrMan, string StrWoman)
{
	string strBack;
    if (_character.sex == "woman")
    {
        strBack = StrWoman;
    }
    else
    {
        strBack = StrMan;
    }
    return strBack;
}
// ����� �� ���� ��
string GetSexPhrase(string StrMan, string StrWoman)
{
    return NPCharSexPhrase(PChar, StrMan, StrWoman);
}
//<<<=== ������� �� ������ � ����������� �������. =======================================================================


// �������� ��������� ��� ������ � ����� ������� � ������
//--------------------------------------------------------
bool SetTempRemoveParam(ref _refCharacter, string _param)
{
    string sParam = "TmpRemember" + _param;
    
	if( CheckAttribute(_refCharacter, sParam) ) return false;
	if( !CheckAttribute(_refCharacter, _param) ) return false;

	aref dstRef; makearef(dstRef, _refCharacter.(sParam));
	aref srcRef; makearef(srcRef, _refCharacter.(_param));

	CopyAttributes(dstRef,srcRef);
	return true;
}

// ������������ ������ � ����� ������ ������� �� ������
//------------------------------------------------------
bool RestoreTempRemoveParam(ref _refCharacter, string _param)
{
    string sParam = "TmpRemember" + _param;
    
	if( !CheckAttribute(_refCharacter, sParam) ) return false;

	aref dstRef; makearef(dstRef, _refCharacter.(_param));
	aref srcRef; makearef(srcRef, _refCharacter.(sParam));

	DeleteAttribute(_refCharacter, _param);
	//--> eddy. ��������� ���� ������, ����� ����� � �����.
	_refCharacter.(_param) = "";
	CopyAttributes(dstRef,srcRef);
	DeleteAttribute(_refCharacter, sParam);
	return true;
}

// ��������� ������ ������ � ���������� ��� ������� ������
// �����: �������� ������ �� � ��������� ������ - ����� ��� ������ ����� � ����������� ��� ��� ������
void SetCaptureResidenceQuest(string _city, string _method, bool _majorOff)
{
    PChar.GenQuestFort.ResidenceQuest.(_city) = true;
    // ������� �� ����������� �� ������� ������, �� ��������, �� ����� ������� ����������, ���� �� ������ ��� ����.
    PChar.GenQuestFort.ResidenceQuest.(_city).MayorOff = _majorOff;
    if (_method != "")
    {
        PChar.GenQuestFort.ResidenceQuest.(_city).method = _method;
	}
}

// ������ ������ ����� ��������� � �������
void  SetReturn_Gover_Dialog_Exit(ref NPChar)
{
	LAi_LoginInCaptureTown(NPChar, false);

	PChar.GenQuest.GoverIdx = NPChar.index;
	Pchar.quest.Return_Gover_Dialog_Exit.win_condition.l1          = "ExitFromLocation";
    Pchar.quest.Return_Gover_Dialog_Exit.win_condition.l1.location = Pchar.location;
    Pchar.quest.Return_Gover_Dialog_Exit.win_condition             = "Return_Gover_Dialog_Exit";
}

void SetNewModelToChar(ref chref)
{
    float liveTime = 0.1;
	int colors = argb(64, 64, 64, 64);
	int colore = argb(0, 32, 32, 32);

    if (IsEntity(chref))
    {
    	if(CheckAttribute(chref, "model"))
        {
            SendMessage(chref, "lss",   MSG_CHARACTER_SETMODEL, chref.model, chref.model.animation);
        }
        if(CheckAttribute(chref, "equip.gun"))
        {
            SendMessage(chref, "ls",    MSG_CHARACTER_SETGUN,   chref.equip.gun);
        }
        if(CheckAttribute(chref, "equip.blade"))
        {
            //SendMessage(chref, "lsfll", MSG_CHARACTER_SETBLADE, chref.equip.blade, liveTime, colors, colore);
            SendMessage(chref, "llsfll", MSG_CHARACTER_SETBLADE, 0, chref.equip.blade, liveTime, colors, colore);
        }
    }
}
// ������� �� �����_������
void SetNationToOfficers(int _nat)
{
    int j, cn;
    ref officer;

    for(j=1; j<COMPANION_MAX; j++)
    {
        cn = GetCompanionIndex(GetMainCharacter(),j);
        if( cn>0 )
        {
	        officer = GetCharacter(cn);
	        if (!GetRemovable(officer)) continue;
	        officer.nation = _nat;
	        Ship_FlagRefresh(officer); //���� �� ����
	    }
	}
}

bool IsOfficerCompanion(ref _refCharacter)
{
	int findIdx = sti(_refCharacter.index);
	ref mc = GetMainCharacter();
	int ci, cn;
	ref npc;
	for(int i=1; i<COMPANION_MAX; i++)
	{
		ci = GetCompanionIndex(mc,i);
		npc = GetCharacter(ci);
		for(int j=1; j<4; j++)
		{
			if(GetOfficersIndex(npc,j)==findIdx) return true;
		}
	}
	return false;
}
///////////////////////////////////////////////////////////////////
void SortItems(ref NPChar)
{// ������������� �������� � �������, �������
    aref   arToChar;
    aref   arFromChar;
    object objChar;
    int    i;
    aref   curItem;
	string attr;
	ref    itm;
	ref    rObj;
	int    iSortIndex;
	bool   ok;

    objChar.Items = "";
    rObj = &objChar;

    makearef(arToChar,   rObj.Items);
    makearef(arFromChar, NPChar.Items);

    CopyAttributes(arToChar, arFromChar);

    DeleteAttribute(NPChar, "Items");
    NPChar.Items = "";

    makearef(arFromChar, NPChar.equip); // ����������
    int iMax = GetAttributesNum(arFromChar);
    for(i=0; i<iMax; i++)
    {
        curItem = GetAttributeN(arFromChar, i);
        attr = GetAttributeValue(curItem);
        if (attr != "") //������� ������
        {
        	NPChar.Items.(attr) = sti(rObj.Items.(attr));
        }
    }
    // ������������� ���������� �� ������� itm.SortIndex
	// ����������� ������� ������������ ��������� - ����� ���� ����������!!, ���������� � 1 - ����� �����
	ok = true;
	iSortIndex = 1;
	while (iSortIndex < 3)// 2 ����
	{
		ok = false;
		for (i=0; i<ITEMS_QUANTITY; i++)
		{
			makeref(itm, Items[i]);
			attr = itm.id;
			if (CheckAttribute(rObj, "items."+attr) && CheckAttribute(itm, "SortIndex") && sti(itm.SortIndex) == iSortIndex)
			{
                NPChar.Items.(attr) = sti(rObj.Items.(attr));
	   			ok = true;
	      	}
	    }
	    iSortIndex++;
    }
    // ��� ���������
    for (i=0; i<ITEMS_QUANTITY; i++)
	{
		makeref(itm, Items[i]);
		attr = itm.id;
		if (CheckAttribute(rObj, "items."+attr) && !CheckAttribute(itm, "SortIndex"))
		{
   			NPChar.Items.(attr) = sti(rObj.Items.(attr));
   			ok = true;
      	}
    }
}
// ��������� ��������� ������� � ���
void SetQuestGoodsToStore(ref refStore)
{
	string    goodName;
	// ������ ������������� �� �����_������
	/*
	pchar.GenQuest.StoreGoods.StoreIdx = Bridgetown_STORE;
	pchar.GenQuest.StoreGoods.NowNeedToBe = true;
	*/
	if (CheckAttribute(pchar, "GenQuest.StoreGoods.StoreIdx") && refStore.index == pchar.GenQuest.StoreGoods.StoreIdx)
	{
        // ������ ��������� �������
		/*if (CheckAttribute(pchar, "GenQuest.StoreGoods.NowNeedToBe")) // ��������� ���������, �� ����� ������
		{
		    DeleteAttribute(pchar, "GenQuest.StoreGoods"); // ����������� ��������, ���� ��� �����
		    goodName = Goods[GOOD_RUM].Name;
		    refStore.Goods.(goodName).Quantity       = 666;
		    // ���� �� ��������� ���������� 20 � �����������
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_RUM, PRICE_TYPE_BUY, pchar, 20);
		}  */
		if (CheckAttribute(pchar, "GenQuest.StoreGoods.HL2_DiscountPrices")) //����.�������, ����� �2, �������� � ����� �����.
		{
		    DeleteAttribute(pchar, "GenQuest.StoreGoods"); // ����������� ��������, ���� ��� �����
			goodName = Goods[GOOD_EBONY].Name;
		    pchar.questTemp.HL2Save.EbonyQty = refStore.Goods.(goodName).Quantity;
		    refStore.Goods.(goodName).Quantity       = 400;
		    pchar.questTemp.HL2Save.EbonyPrice = GetStoreGoodsPrice(refStore, GOOD_EBONY, PRICE_TYPE_BUY, pchar, refStore.Goods.(goodName).RndPriceModify);
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_EBONY, PRICE_TYPE_BUY, pchar, 10);

		    goodName = Goods[GOOD_COFFEE].Name;
		    pchar.questTemp.HL2Save.CoffeeQty = refStore.Goods.(goodName).Quantity;
		    refStore.Goods.(goodName).Quantity       = 400;
		    pchar.questTemp.HL2Save.CoffeePrice = GetStoreGoodsPrice(refStore, GOOD_COFFEE, PRICE_TYPE_BUY, pchar, refStore.Goods.(goodName).RndPriceModify);
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_COFFEE, PRICE_TYPE_BUY, pchar, 9);

		    goodName = Goods[GOOD_MAHOGANY].Name;
		    pchar.questTemp.HL2Save.MahoganyQty = refStore.Goods.(goodName).Quantity;
		    refStore.Goods.(goodName).Quantity       = 400;
		    pchar.questTemp.HL2Save.MahoganyPrice =  GetStoreGoodsPrice(refStore, GOOD_MAHOGANY, PRICE_TYPE_BUY, pchar, refStore.Goods.(goodName).RndPriceModify);
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_MAHOGANY, PRICE_TYPE_BUY, pchar, 14);

		    goodName = Goods[GOOD_SANDAL].Name;
		    pchar.questTemp.HL2Save.SandalQty = refStore.Goods.(goodName).Quantity;
		    refStore.Goods.(goodName).Quantity       = 400;
		    pchar.questTemp.HL2Save.SandalPrice = GetStoreGoodsPrice(refStore, GOOD_SANDAL, PRICE_TYPE_BUY, pchar, refStore.Goods.(goodName).RndPriceModify);
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_SANDAL, PRICE_TYPE_BUY, pchar, 11);
        	pchar.GenQuest.StoreGoods.StoreIdx = FortOrange_STORE; //������ ���� ��� ����. ���������
        	pchar.GenQuest.StoreGoods.HL2_AgainDiscount = true;
		}
		if (CheckAttribute(pchar, "GenQuest.StoreGoods.HL2_AgainDiscount")) //����.�������, ����� �2, ������ ���������� ����
		{
		    goodName = Goods[GOOD_EBONY].Name;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_EBONY, PRICE_TYPE_BUY, pchar, 10);

		    goodName = Goods[GOOD_COFFEE].Name;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_COFFEE, PRICE_TYPE_BUY, pchar, 9);

		    goodName = Goods[GOOD_MAHOGANY].Name;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_MAHOGANY, PRICE_TYPE_BUY, pchar, 14);

		    goodName = Goods[GOOD_SANDAL].Name;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_SANDAL, PRICE_TYPE_BUY, pchar, 11);
		}		
		if (CheckAttribute(pchar, "GenQuest.StoreGoods.HL2_QtyPriceIsBack")) //����.�������, ����� �2, ������� ��� � ���-�� ����� ������.
		{
            DeleteAttribute(pchar, "GenQuest.StoreGoods"); // ����������� ��������, ���� ��� �����
		    goodName = Goods[GOOD_EBONY].Name;
		    refStore.Goods.(goodName).Quantity       = pchar.questTemp.HL2Save.EbonyQty;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_EBONY, PRICE_TYPE_BUY, pchar, sti(pchar.questTemp.HL2Save.EbonyPrice));

		    goodName = Goods[GOOD_COFFEE].Name;
		    refStore.Goods.(goodName).Quantity       = pchar.questTemp.HL2Save.CoffeeQty;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_COFFEE, PRICE_TYPE_BUY, pchar, sti(pchar.questTemp.HL2Save.CoffeePrice));

		    goodName = Goods[GOOD_MAHOGANY].Name;
		    refStore.Goods.(goodName).Quantity       = pchar.questTemp.HL2Save.MahoganyQty;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_MAHOGANY, PRICE_TYPE_BUY, pchar, sti(pchar.questTemp.HL2Save.MahoganyPrice));

		    goodName = Goods[GOOD_SANDAL].Name;
		    refStore.Goods.(goodName).Quantity       = pchar.questTemp.HL2Save.SandalQty;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_SANDAL, PRICE_TYPE_BUY, pchar, sti(pchar.questTemp.HL2Save.SandalPrice));
            DeleteAttribute(pchar, "questTemp.HL2Save");
        }
		if (CheckAttribute(pchar, "GenQuest.StoreGoods.HL5_ThreeFleutes")) //����.�������, ����� �5, ��������� ����������.
		{
            DeleteAttribute(pchar, "GenQuest.StoreGoods"); // ����������� ��������, ���� ��� �����
            goodName = Goods[GOOD_FOOD].Name;
		    refStore.Goods.(goodName).Quantity       = 100000;
			refStore.Goods.(goodName).RndPriceModify = GetStoreGoodsRndPriceModify(refStore, GOOD_FOOD, PRICE_TYPE_BUY, pchar, 10);

        }
	}
}

void HolLine2_NullGoods() //��� ������� ������ ����. �������.
{
    ref sld;
    int chComp;
    if (CheckAttribute(pchar, "questTemp.EbonyQtyElseNeed")) pchar.Ship.Cargo.Goods.Ebony = 0;
    if (CheckAttribute(pchar, "questTemp.CoffeeQtyElseNeed")) pchar.Ship.Cargo.Goods.Coffee = 0;
    if (CheckAttribute(pchar, "questTemp.MahoganyQtyElseNeed")) pchar.Ship.Cargo.Goods.Mahogany = 0;
    if (CheckAttribute(pchar, "questTemp.SandalQtyElseNeed")) pchar.Ship.Cargo.Goods.Sandal = 0;
    for (int i=1; i<=COMPANION_MAX; i++)
	{
        chComp = GetCompanionIndex(pchar,i);
        if(chComp != -1)
		{
			sld = GetCharacter(chComp);
            if (CheckAttribute(pchar, "questTemp.EbonyQtyElseNeed")) sld.Ship.Cargo.Goods.Ebony = 0;
            if (CheckAttribute(pchar, "questTemp.CoffeeQtyElseNeed")) sld.Ship.Cargo.Goods.Coffee = 0;
            if (CheckAttribute(pchar, "questTemp.MahoganyQtyElseNeed")) sld.Ship.Cargo.Goods.Mahogany = 0;
            if (CheckAttribute(pchar, "questTemp.SandalQtyElseNeed")) sld.Ship.Cargo.Goods.Sandal = 0;
        }
    }
}

// ����� ��������� �������� � ����� �� ��������. ������ �������� �� ����� + �����
// refChar - ��� ������, � �� �������� ���, �� �����, ������ �� ����������, �� �� � ������ �� ���, � ���������, ������� ��������� ����� ���� �������
// CaptanId - �������� �� ����, ����������� � ������ �����������.
void SetQuestAboardCabinDialog(ref refChar)
{
	// ������. ��� ����� ��� �����, ��� ��� ������ ���� �� ����������� �������� �� �������
	//refChar.DontClearDead      = true;
	//refChar.SaveItemsForDead   =   true;
	//TakeNItems(refChar, "Chest", 1);
	// ������. <--

	if (CheckAttribute(refChar, "CaptanId"))
	{
        pchar.GenQuest.QuestAboardCabinDialogIdx = refChar.index;
        // ��� ������������, ������
		/*if (true) // true - ��� �����, ������ ���� ��� CheckAttribute(pchar, "��������� �����, ���� �����") && refChar.CaptanId == "IdNPC")
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
            refChar.Dialog.Filename    = "Capitans_dialog.c";   // ��� ����� ������ �� ����������� ���� � �� �����
	    	refChar.Dialog.CurrentNode = "QuestAboardCabinDialog";
	    	// ��� ��������� � �������, �� �������
		}*/
        //����. �������, ����� �7, �������� ������ �� �������
        if (refChar.CaptanId == "CaptainAttack_1" && pchar.questTemp.State == "DelivLettTortuga_NotFoundLetter")
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		if (refChar.CaptanId == "AntonioDeBarras" && pchar.questTemp.State == "DelivLettTortuga_NotFoundLetter")
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
        //���. �������, ����� �2, ������� ����������
		if (refChar.CaptanId == "JansenDeFonceicao" && CheckAttribute(pchar, "questTemp.State.Usurer"))
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//���. �������, ����� �3, ������ ���� ���������
		if (refChar.CaptanId == "RockBrasilian" && pchar.questTemp.State == "TakeRockBras_toNearestMaracaibo")
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//���. �������, ����� �7, �������� �������� �������, ����� � ������� ���������
		if (refChar.CaptanId == "CaptainAttack_1" && pchar.questTemp.State == "Sp7SavePardal_toAntigua")
		{
		    LAi_SetCheckMinHP(refChar, 50, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		if (refChar.CaptanId == "CaptainAttack_1" && pchar.questTemp.State == "Sp7SavePardal_PardalIsSink")
		{
		    LAi_SetCheckMinHP(refChar, 50, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		if (refChar.CaptanId == "CaptainAttack_1" && pchar.questTemp.State == "Sp8SaveCumana_toCumana")
		{
		    LAi_SetCheckMinHP(refChar, 50, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//��. �������, ����� �2, ����� ������� ��������� �������
		if (refChar.CaptanId == "CaptainAttack_1" && pchar.questTemp.State == "Fr2Letter_SeekProblems")
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//������� �����, ������� ����� �������� �� ���� ����������
		if (refChar.CaptanId == "CaptainAttack_1" && pchar.RomanticQuest == "AttackBrig")
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//������� �� ������ ���� �� ����� � ����������� ������.
		if (refChar.CaptanId == "MQPirate" && CheckAttribute(pchar, "GenQuest.DestroyPirate"))
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
			refChar.dialog.currentnode = "DestroyPirate_Abordage";
		}
		//������ �� �� �� ������� �� ����� homo
		if (findsubstr(refChar.CaptanId, "_QuestMerchant" , 0) != -1  && trap)
		{
		    LAi_SetCheckMinHP(refChar, 30, true, "QuestAboardCabinDialog");  // ������� �� ���
		    refChar.dialog.filename = "Capitans_dialog.c";  //fix homo 23/03/07
			refChar.dialog.currentnode = "MerchantTrap_Abordage";
		}
		//������� ������ ����� �����
		if (refChar.CaptanId == "BlueBirdCapitain" && pchar.questTemp.BlueBird == "toSeaBattle")
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//������� ������ ����� �����
		if (refChar.CaptanId == "BlueBirdTrader" && pchar.questTemp.BlueBird == "attackFleut")
		{
		    LAi_SetCheckMinHP(refChar, 20, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//�������, ������ ��� �� ����� ������� ����
		if (refChar.CaptanId == "EdwardLoy" && pchar.questTemp.piratesLine == "KillLoy_SeaWolfBattle")
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//�������, ����� �7, ���, ��������� ��������
		if (refChar.CaptanId == "LeonCapitain" && pchar.questTemp.piratesLine == "Soukins_seekLeon")
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//����� �������� ����������� �������
		if (findsubstr(refChar.CaptanId, "SeekCap_" , 0) != -1)
		{
		    LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
		}
		//����� ���������� �� ����������� ���������� ����
		if (findsubstr(refChar.CaptanId, "SeekCitizCap_" , 0) != -1)
		{
			if (refChar.quest.SeekCap == "manRapeWife")
			{
				LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
				refChar.Dialog.CurrentNode = "RapeWifeCap_Board"; //���� ���������� ����
			}
		    if (refChar.quest.SeekCap == "womanRevenge")
			{
				LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
				refChar.Dialog.CurrentNode = "RevengeCap_board"; //���� ���������� ����
			}
			if (refChar.quest.SeekCap == "womanPirates")
			{
				LAi_SetCheckMinHP(refChar, 5, true, "QuestAboardCabinDialog");  // ������� �� ���
				refChar.Dialog.CurrentNode = "PiratesCap_Board"; //���� ���������� ����
			}
		}
	}
}

void QuestAboardCabinDialogFree()
{
	ref sld;
	sld = &Characters[sti(pchar.GenQuest.QuestAboardCabinDialogIdx)];
	Lai_SetPlayerType(pchar);
	LAi_RemoveCheckMinHP(sld);
	LAi_SetImmortal(sld, false);
    LAi_SetWarriorType(sld);
    LAi_group_MoveCharacter(sld, LAI_GROUP_BRDENEMY);
}
//����� ��� ������, �� � ���� �� ����
void QuestAboardCabinDialogNotBattle()
{
	ref sld;
	sld = &Characters[sti(pchar.GenQuest.QuestAboardCabinDialogIdx)];
	Lai_SetPlayerType(pchar);
	LAi_RemoveCheckMinHP(sld);
	LAi_SetImmortal(sld, false);
    LAi_SetWarriorType(sld);
	DoQuestCheckDelay("LAi_ReloadBoarding", 1.0);
}
// ����� � �������
void QuestAboardCabinDialogExitWithBattle(string _questName)
{
    QuestAboardCabinDialogFree(); // ������ �����
	ref sld;
	sld = &Characters[sti(pchar.GenQuest.QuestAboardCabinDialogIdx)];
	LAi_SetFightMode(pchar, true);
	LAi_SetFightMode(sld, true);
	LAi_group_SetRelation(LAI_GROUP_BRDENEMY, LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
    LAi_group_FightGroups(LAI_GROUP_BRDENEMY, LAI_GROUP_PLAYER, true);
	LAi_group_SetCheck(LAI_GROUP_BRDENEMY, _questName);
	// SetCharacterTask_Fight(sld, pchar);
}

void QuestAboardCabinDialogExitWithBattleNoParam()// homo ���� �����, ������ ��� ����������
{
    QuestAboardCabinDialogExitWithBattle("");
}

void QuestAboardCabinDialogSurrender()
{
 	ref sld;
	sld = &Characters[sti(pchar.GenQuest.QuestAboardCabinDialogIdx)];
	DeleteAttribute(sld, "DontRansackCaptain"); // ���� ���� �����-��
	pchar.GenQuest.QuestAboardCaptanSurrender = true;
	Lai_SetPlayerType(pchar);
	LAi_RemoveCheckMinHP(sld);
	LAi_SetImmortal(sld, false);
	//�� ����� ������� LAi_SetCurHP(characterFromId(sld.CaptanId), 0.0); 
	//sld.LifeDay = 0;    ��� �� ������ �����������, � ������ �������, ������ �� ��� ��� �������� �������, �� �� ��� ��������
	DoQuestCheckDelay("LAi_ReloadBoarding", 1.0);
}
//==> ��������� �������� � ����������� ������� ��������.
void QuestAboardCabinDialogQuestSurrender()
{
	ref sld;
	sld = &Characters[sti(pchar.GenQuest.QuestAboardCabinDialogIdx)];
	sld.DontRansackCaptain = true; // ���� �� ������ ������ ���
	Lai_SetPlayerType(pchar);
	LAi_RemoveCheckMinHP(sld);
	LAi_SetImmortal(sld, false);
	//�� ����� ������� LAi_SetCurHP(characterFromId(sld.CaptanId), 0.0); 
	//sld.LifeDay = 0;
	pchar.GenQuest.LastQuestPrisonerIdx = SetCharToPrisoner(sld);
	SetCharacterRemovable(sld, false);
	DoQuestCheckDelay("LAi_ReloadBoarding", 1.0);
}
//==> eddy. ��������� ��������� '���� �����'.
void QuestActions()
{
	ref sld;
	// �������� ������������ ��������
	for (i=0; i<4; i++)
	{
		if (CheckCharacterItem(pchar, NationShortName(i)+"TradeLicence") && GetDaysContinueNationLicence(i) == -1)
		{
			TakeNationLicence(i);
		}		
	}
	//���� ���� ������� ���, �� ������� �������� �������. ���, ����� ����������.
	if (sti(pchar.questTemp.LSC.checkBoxes))
	{
		LSC_checkBoxes();
	}
	//************** ��.�������, ������ �����. ��������� ����� ���� ���������� ������ *******************
	if (pchar.questTemp.State == "Fr3TakeAnna_WaitMidnight")
	{
		LocatorReloadEnterDisable("Havana_town", "houseS1", false);
        Pchar.quest.Fr3TakeAnna_FightInHouse.win_condition.l1 = "location";
        Pchar.quest.Fr3TakeAnna_FightInHouse.win_condition.l1.location = "Havana_houseS1";
        Pchar.quest.Fr3TakeAnna_FightInHouse.win_condition = "Fr3TakeAnna_FightInHouse";
		pchar.questTemp.State = "Fr3TakeAnna_DoorIsOpen";
	}
	//************** ��������, ����� ����� ����� �������� ������ �������� *******************
	if (CheckAttribute(pchar, "RomanticQuest") && pchar.RomanticQuest == "DelivMigel")
	{		
		pchar.RomanticQuest = "SeeTalkNearHouse"; //�������� �� ����� ������ ���������        
        sld = CharacterFromID("MigelDeValdes");
		//������
        sld.dialog.currentnode = "TalkNearHouse_1";
        ChangeCharacterAddressGroup(sld, "SanJuan_town", "quest", "goto24");
        LAi_SetActorType(sld); 
		sld.rank = 1; //����� ���� �����
		sld.skill.Fencing = 1;                
		sld.skill.Pistol = 1;
		LAi_SetHP(sld, 1, 1);
		DeleteAttribute(sld, "Items");
		GiveItem2Character(sld, "blade1");
		GiveItem2Character(sld, "pistol1");
		GiveItem2Character(sld, "order");
		TakeNItems(sld,"bullet", 1); 
		//��������� � �����������
		LAi_group_Delete("EnemyFight");
        sld = CharacterFromID("Husband");
        sld.dialog.currentnode = "TalkNearHouse_1";
        ChangeCharacterAddressGroup(sld, "SanJuan_town", "quest", "goto25");		
        for (int i=8; i<=9; i++)
        {
			sld = GetCharacter(NPC_GenerateCharacter("Bandit_"+i, "pirate_"+i, "man", "man", 30, PIRATE, 1, true));
			FantomMakeCoolFighter(sld, 30, 80, 70, "blade28", "pistol3", 100);			
			LAi_SetStayType(sld);			
			if (i==8) ChangeCharacterAddressGroup(sld, "SanJuan_town", "quest", "goto7");
			else ChangeCharacterAddressGroup(sld, "SanJuan_town", "quest", "goto26");
			LAi_group_MoveCharacter(sld, "EnemyFight");
		}
		SetTimerCondition("Romantic_DeadBrother_Cancel", 0, 0, 1, false);
	}
	//************** ��������, �������!!!! *******************
	if (CheckAttribute(pchar, "RomanticQuest") && pchar.RomanticQuest == "WaitBeginWidding")
	{	
		pchar.RomanticQuest = "BeginWidding";
		pchar.quest.Isabella_widding.win_condition.l1 = "locator";
		pchar.quest.Isabella_widding.win_condition.l1.location = "SanJuan_church";
		pchar.quest.Isabella_widding.win_condition.l1.locator_group = "quest";
		pchar.quest.Isabella_widding.win_condition.l1.locator = "detector1";
		pchar.quest.Isabella_widding.function = "Isabella_widding";
		sld = CharacterFromID("Isabella");
		LAi_SetActorType(sld);
		ChangeCharacterAddressGroup(sld , "SanJuan_church", "goto",  "goto11");
		sld.dialog.currentnode = "TalkWidding";
		//�����
		LAi_SetActorType(CharacterFromID("SanJuan_Priest"));
		SetTimerCondition("Romantic_Widding_Cancel", 0, 0, 1, false);
	}
	//************** ������ ������ � ������ *******************
	if (CheckAttribute(pchar, "questTemp.different.Church_NightGuard"))
	{
		if (pchar.questTemp.different.Church_NightGuard == pchar.location)
		{
			if (!CheckAttribute(pchar, "pchar.quest.ChurchNightGuard_exitFromLoc") && sti(pchar.questTemp.different.Church_NightGuard.NightQty) < 3)
			{
				pchar.quest.ChurchNightGuard_exitFromLoc.win_condition.l1 = "ExitFromLocation";
				pchar.quest.ChurchNightGuard_exitFromLoc.win_condition.l1.location = pchar.location;
				pchar.quest.ChurchNightGuard_exitFromLoc.function = "ChurchNightGuard_exitFromLoc";
				DoQuestFunctionDelay("ChurchNightGuard_timerInChurch", 10.0 + frand(10.0));
			}
		}
		else
		{	//�� �� ��������� � ������ ������ � ������ �������
			string sTitle = pchar.questTemp.different.Church_NightGuard + "Church_NightGuard";
			AddQuestRecordEx(sTitle, "Church_NightGuard", "2");
			CloseQuestHeader(sTitle);
			DeleteAttribute(pchar, "questTemp.different.Church_NightGuard");
			ChangeCharacterReputation(pchar, -20);
		}
	}
	//************** ������� ���, ������� �  *******************
	if (pchar.questTemp.LSC == "toWineCellar" )
	{
		if (GetHour() < 1)
		{
			pchar.questTemp.LSC = "toWineCellarGone";
			sld = characterFromId("LSCBarmen");
			LAi_SetStayTypeNoGroup(sld);
			ChangeCharacterAddressGroup(sld, "FleuronTavern", "goto", "goto10");
			PChar.quest.LSC_returnBarmen.win_condition.l1            = "Timer";
			PChar.quest.LSC_returnBarmen.win_condition.l1.date.hour  = 1;
			PChar.quest.LSC_returnBarmen.win_condition.l1.date.day   = GetAddingDataDay(0, 0, 0);
			PChar.quest.LSC_returnBarmen.win_condition.l1.date.month = GetAddingDataMonth(0, 0, 0);
			PChar.quest.LSC_returnBarmen.win_condition.l1.date.year  = GetAddingDataYear(0, 0, 0);
			PChar.quest.LSC_returnBarmen.function = "LSC_returnBarmen";
		}
		else pchar.questTemp.LSC = "toWineCellarLate";
	}
}
// eddy. �������� �������� ��������. -->
//���� ��������
void GiveNationLicence(int _nation, int _validity)
{
	string sTemp; 
	ref rItem;
	for(int i=0; i<4; i++)
	{
		if (CheckNationLicence(i) && GetDaysContinueNationLicence(i) == -1) TakeNationLicence(i);
	}
	if (_nation != PIRATE) 
	{
		if (CheckNationLicence(_nation)) TakeNationLicence(_nation);
		sTemp = NationShortName(_nation)+"TradeLicence";
		GiveItem2Character(pchar, sTemp);
		rItem = ItemsFromID(sTemp);
		SaveCurrentNpcQuestDateParam(rItem, "Action_date");
		rItem.Action_date = GetCurrentDate(); 
		rItem.Validity = FindRussianDaysString(_validity); //������ ��� ���������
		rItem.Validity.QtyDays = _validity; //����� �������� �������� � ���� ��� ��������
	}
}
//������� �������� 
void TakeNationLicence(int _nation)
{
	string sTemp; 
	if (_nation != PIRATE && CheckNationLicence(_nation)) 
	{
		sTemp = NationShortName(_nation)+"TradeLicence";
		TakeItemFromCharacter(pchar, sTemp);
		DeleteAttribute(ItemsFromID(sTemp), "Action_date");
		DeleteAttribute(ItemsFromID(sTemp), "Validity");
	}
}
//��������� ������� ��������
bool CheckNationLicence(int _nation)
{
	if (_nation != PIRATE) 
	{
		if (CheckCharacterItem(pchar, NationShortName(_nation)+"TradeLicence")) return true; 
	}
	return false;
}
//��������� ����� ��������, ������� �������� ����. ���� -1, �� ���������� ��� �����������
int GetDaysContinueNationLicence(int _nation)
{
	int iTerms = -1;
	ref rItem;
	if (_nation != PIRATE) 
	{
		if (CheckNationLicence(_nation))
		{
			rItem = ItemsFromID(NationShortName(_nation)+"TradeLicence");
			int Validity = sti(rItem.Validity.QtyDays);
			iTerms = GetNpcQuestPastDayParam(rItem, "Action_date");
			if (iTerms > Validity) iTerms = -1;
			else iTerms = Validity - iTerms;
		}
	}
	return iTerms;
}
//���� ������������ ��������, �������� "�������� ����������� ����-������� ��������"
string GetRusNameNationLicence(int _nation)
{
	string sTemp, itmTitle;
	int lngFileID;
	if (_nation != PIRATE) 
	{
		if (CheckNationLicence(_nation))
		{
			sTemp = NationShortName(_nation)+"TradeLicence";
			lngFileID = LanguageOpenFile("ItemsDescribe.txt");
			itmTitle = LanguageConvertString(lngFileID, Items[GetItemIndex(sTemp)].name);
		}
	}
	return itmTitle;
}
// eddy. �������� �������� ��������. <--

//--> eddy. ����� ��������, ������ ��� �������� �������� �����
//������� ������� ��������� ����������. �������� ��� �� �� ���, � ���� �� �����������.
// ���� _TypeOfCheck == "all", �� �������� ���. _TypeOfCheck ����� ���� "KillFort", "TakeTown", 
void IsabellaSetCurrentState(string _TypeOfCheck)
{
	if (_TypeOfCheck == "all" || _TypeOfCheck == "KillFort")
	{
		DeleteAttribute(pchar, "RomanticQuest.Cheking.KillFort");
		pchar.RomanticQuest.State.KillFort = Statistic_AddValue(PChar, "pir_KillFort", 0) +
											Statistic_AddValue(PChar, "eng_KillFort", 0) +
											Statistic_AddValue(PChar, "fra_KillFort", 0) +
											Statistic_AddValue(PChar, "spa_KillFort", 0) +
											Statistic_AddValue(PChar, "hol_KillFort", 0);
	}
	if (_TypeOfCheck == "all" || _TypeOfCheck == "TakeTown")
	{
		DeleteAttribute(pchar, "RomanticQuest.Cheking.TakeTown");
		pchar.RomanticQuest.State.TakeTown = Statistic_AddValue(PChar, "pir_TakeTown", 0) +
											Statistic_AddValue(PChar, "eng_TakeTown", 0) +
											Statistic_AddValue(PChar, "fra_TakeTown", 0) +
											Statistic_AddValue(PChar, "spa_TakeTown", 0) +
											Statistic_AddValue(PChar, "hol_TakeTown", 0);
	}
	if (_TypeOfCheck == "all" || _TypeOfCheck == "GrabbingTown")
	{
		DeleteAttribute(pchar, "RomanticQuest.Cheking.GrabbingTown");
		pchar.RomanticQuest.State.GrabbingTown = Statistic_AddValue(PChar, "pir_GrabbingTown", 0) +
												Statistic_AddValue(PChar, "eng_GrabbingTown", 0) +
												Statistic_AddValue(PChar, "fra_GrabbingTown", 0) +
												Statistic_AddValue(PChar, "spa_GrabbingTown", 0) +
  												Statistic_AddValue(PChar, "hol_GrabbingTown", 0);
	}
	if (_TypeOfCheck == "all" || _TypeOfCheck == "AbordShip")
	{
		DeleteAttribute(pchar, "RomanticQuest.Cheking.AbordShip");
		pchar.RomanticQuest.State.AbordShip = Statistic_AddValue(PChar, "pir_AbordShip", 0) +
											Statistic_AddValue(PChar, "eng_AbordShip", 0) +
											Statistic_AddValue(PChar, "fra_AbordShip", 0) +
											Statistic_AddValue(PChar, "spa_AbordShip", 0) +
											Statistic_AddValue(PChar, "hol_AbordShip", 0);
	}
	if (_TypeOfCheck == "all" || _TypeOfCheck == "KillShip")
	{
		DeleteAttribute(pchar, "RomanticQuest.Cheking.KillShip");	
		pchar.RomanticQuest.State.KillShip = Statistic_AddValue(PChar, "pir_KillShip", 0) +
											Statistic_AddValue(PChar, "eng_KillShip", 0) +
											Statistic_AddValue(PChar, "fra_KillShip", 0) +
											Statistic_AddValue(PChar, "spa_KillShip", 0) +
											Statistic_AddValue(PChar, "hol_KillShip", 0);
	}
}
//�������� ������� ��������� � ������ �������� �� �����������
void IsabellaCheckBreachState()
{
	int iTemp;
	iTemp = Statistic_AddValue(PChar, "pir_KillFort", 0) +
			Statistic_AddValue(PChar, "eng_KillFort", 0) +
			Statistic_AddValue(PChar, "fra_KillFort", 0) +
			Statistic_AddValue(PChar, "spa_KillFort", 0) +
			Statistic_AddValue(PChar, "hol_KillFort", 0);
	if (iTemp > sti(pchar.RomanticQuest.State.KillFort)) pchar.RomanticQuest.Cheking.KillFort = iTemp - sti(pchar.RomanticQuest.State.KillFort);
	iTemp = Statistic_AddValue(PChar, "pir_TakeTown", 0) +
			Statistic_AddValue(PChar, "eng_TakeTown", 0) +
			Statistic_AddValue(PChar, "fra_TakeTown", 0) +
			Statistic_AddValue(PChar, "spa_TakeTown", 0) +
			Statistic_AddValue(PChar, "hol_TakeTown", 0);
	if (iTemp > sti(pchar.RomanticQuest.State.TakeTown)) pchar.RomanticQuest.Cheking.TakeTown = iTemp - sti(pchar.RomanticQuest.State.TakeTown);
	iTemp = Statistic_AddValue(PChar, "pir_GrabbingTown", 0) +
			Statistic_AddValue(PChar, "eng_GrabbingTown", 0) +
			Statistic_AddValue(PChar, "fra_GrabbingTown", 0) +
			Statistic_AddValue(PChar, "spa_GrabbingTown", 0) +
  			Statistic_AddValue(PChar, "hol_GrabbingTown", 0);
	if (iTemp > sti(pchar.RomanticQuest.State.GrabbingTown)) pchar.RomanticQuest.Cheking.GrabbingTown = iTemp - sti(pchar.RomanticQuest.State.GrabbingTown);
	iTemp = Statistic_AddValue(PChar, "pir_AbordShip", 0) +
			Statistic_AddValue(PChar, "eng_AbordShip", 0) +
			Statistic_AddValue(PChar, "fra_AbordShip", 0) +
			Statistic_AddValue(PChar, "spa_AbordShip", 0) +
			Statistic_AddValue(PChar, "hol_AbordShip", 0);
	if (iTemp > sti(pchar.RomanticQuest.State.AbordShip)) pchar.RomanticQuest.Cheking.AbordShip = iTemp - sti(pchar.RomanticQuest.State.AbordShip);
	iTemp = Statistic_AddValue(PChar, "pir_KillShip", 0) +
			Statistic_AddValue(PChar, "eng_KillShip", 0) +
			Statistic_AddValue(PChar, "fra_KillShip", 0) +
			Statistic_AddValue(PChar, "spa_KillShip", 0) +
			Statistic_AddValue(PChar, "hol_KillShip", 0);
	if (iTemp > sti(pchar.RomanticQuest.State.KillShip)) pchar.RomanticQuest.Cheking.KillShip = iTemp - sti(pchar.RomanticQuest.State.KillShip);		
	return;
}
//�������� ������� �� ��������� �������
void IsabellaNullBudget()
{
	pchar.RomanticQuest.Budget = "0";
	SaveCurrentQuestDateParam("RomanticQuest.Budget");
}
//���������� ������� ������������� � �������. ������������� �������� - ������., ������������� - ��������.
int IsabellaCheckBudgetMoney()
{
	int Debt = GetQuestPastMonthParam("RomanticQuest.Budget")*MOD_SKILL_ENEMY_RATE*10000;
	int CurMoney = sti(pchar.RomanticQuest.Budget);
	return Debt - CurMoney;
}
//�������� ����� �� ������� ������ �������
void IsabellaAddMoneyBudget(int _money)
{
	pchar.RomanticQuest.Budget = sti(pchar.RomanticQuest.Budget) + _money;
	AddMoneyToCharacter(pchar, -_money);
}
// <-- eddy. ����� ��������, ������ ��� �������� �������� �����

void SelectAscoldRendom()
{
	if (CheckAttribute(&colonies[1], "nation"))
	{
		int n, nation;
		int storeArray[MAX_COLONIES];
		int howStore = 0;
		for(n=0; n<MAX_COLONIES; n++)
		{			
			if (colonies[n].nation != "none" && sti(colonies[n].nation) != PIRATE && colonies[n].id != "FortOrange")
			{           
				storeArray[howStore] = n;
				howStore++;
			}
		}
		nation = storeArray[rand(howStore-1)];
		pchar.questTemp.Ascold.TraderId = colonies[nation].id + "_trader";
		nation = storeArray[rand(howStore-1)];
		pchar.questTemp.Ascold.MerchantColony = colonies[nation].id; 
		nation = storeArray[rand(howStore-1)];
		while (colonies[nation].id == "Panama")
		{
			nation = storeArray[rand(howStore-1)];
		}
		pchar.questTemp.Ascold.ShipyarderId = colonies[nation].id + "_shipyarder"; 
		pchar.questTemp.Ascold.ShipyarderColony = colonies[nation].id;
		items[GetItemIndex("Azzy_bottle")].useLocation = colonies[nation].id + "_Shipyard";
	}
}
// ����� ��������
void IsabellaInit()
{
	ref ch;
	// ==> ��������� ������ ��������, ���� ����� "" - �� ����� ������ �� ����.
	pchar.RomanticQuest = "canTakeQuest";
	//������
	ch = GetCharacter(NPC_GenerateCharacter("Atilla", "pirate_1", "man", "man", 22, PIRATE, -1, false));
	ch.name 	= "������";
	ch.lastname 	= "";
	GiveItem2Character(ch, GUN_COMMON);
	ch.equip.gun = GUN_COMMON;
	GiveItem2Character(ch, BLADE_LONG);
	ch.equip.blade = BLADE_LONG;
	ch.location	= "Pirates_town";
	ch.location.group = "goto";
	ch.location.locator = "goto1";
	ch.Dialog.Filename = "Quest\Isabella\Atilla.c";
	ch.greeting = "Gr_Smuggler Agent";
	ch.rank 	= 25;
	ch.reputation = "30";
	ch.talker = 5; //������ ������
    SetRandSPECIAL(ch);
    SetSelfSkill(ch, 80, 70, 90, 60, 50);
    SetShipSkill(ch, 50, 10, 60, 60, 40, 70, 70, 40, 50);
	ch.money = "200";
	LAi_SetCitizenType(ch);
	LAi_group_MoveCharacter(ch, "PIRATE_CITIZENS");
	LAi_SetLoginTime(ch, 0.0, 24.0);
	LAi_SetHP(ch, 200.0, 200.0);
	LAi_NoRebirthEnable(ch);
	ch.DontClearDead = true;  // �� ������� ����
	ch.greeting = "Ascold";
	// ���� ��� :)
	ch = GetCharacter(NPC_GenerateCharacter("Husband", "Usurer_7", "man", "man", 22, PIRATE, -1, false));	
	ch.name 	= "���������";
	ch.lastname = "��������";
	ch.location	= "SanJuan_houseSp6";
	ch.location.group = "goto";
	ch.location.locator = "goto2";
	ch.Dialog.Filename = "Quest\Isabella\Husband.c";
	GiveItem2Character(ch, "pistol5");
	ch.equip.gun = "pistol5";
	TakeNItems(ch,"bullet", Rand(20)+20);
	GiveItem2Character(ch, "blade34");
	ch.equip.blade = "blade34";
	ch.rank 	= 40;
	ch.reputation = "35";
    SetRandSPECIAL(ch);
    SetSelfSkill(ch, 90, 90, 90, 70, 90);
    SetShipSkill(ch, 20, 90, 10, 10, 10, 20, 10, 10, 50);
	ch.money = "1000";
	ch.SaveItemsForDead = true; // ��������� �� ����� ����
	ch.DontClearDead = true; // �� ������� ���� ����� 200�
	LAi_SetLoginTime(ch, 0.0, 24.0);
	LAi_SetHP(ch, 220.0, 220.0);
	LAi_SetHuberStayType(ch);
	LAi_SetStayHuberPointWindow(ch, "goto", "goto1");
	LAi_SetStayHuberPointMap(ch, "goto", "goto2");
	LAi_group_MoveCharacter(ch, "SPAIN_CITIZENS");
	LAi_SetImmortal(ch, true);
	LAi_LoginInCaptureTown(ch, true);
	ch.greeting = "IsabellaHasband";
}
//���������� ��������� �� ����������. ����� �� ��������.
void SetIsabellaTalker(string qName)
{
	ref chr = characterFromID("Isabella");
	chr.talker = 10;
}

void BerglarsInit()
{
	ref sld;
	pchar.questTemp.tugs.berglarState = 1; //�������
	//============> ��������� � ����-������
	pchar.questTemp.tugs.berglarSentJons = "SentJons_TownCave"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarSentJons.hp = 160; //������� HP
	pchar.questTemp.tugs.berglarSentJons.locator = "basement1"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarSentJons", "citiz_12", "man", "man", 22, ENGLAND, -1, false));
	sld.name 	= "�������";
	sld.lastname = "����";
	sld.rank = 25;
	sld.city = "SentJons";
	sld.location	= "SentJons_town";
	sld.location.group = "goto";
	sld.location.locator = "goto4";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol2");
	GiveItem2Character(sld, "blade21");
	sld.money = 11460;
	sld.talker = 7; //������ ������
	SetSelfSkill(sld, 10, 10, 60, 40, 70);
	SetCharacterPerk(sld, "SwordplayProfessional");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//=============> ��������� �� ���������
	pchar.questTemp.tugs.berglarFortFrance = "FortFrance_Dungeon"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarFortFrance.hp = 180; //������� HP
	pchar.questTemp.tugs.berglarFortFrance.locator = "basement1"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarFortFrance", "citiz_11", "man", "man", 23, FRANCE, -1, false));
	sld.name 	= "�������";
	sld.lastname = "�����";
	sld.rank = 25;
	sld.city = "FortFrance";
	sld.location	= "FortFrance_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol3");
	GiveItem2Character(sld, "blade24");
	sld.money = 22670;
	sld.talker = 7; //������ ������
	SetSelfSkill(sld, 10, 70, 10, 50, 60);
	SetCharacterPerk(sld, "BasicDefense");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//============> ��������� � ������ 
	pchar.questTemp.tugs.berglarMarigo = "Marigo_Cave"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarMarigo.hp = 200; //������� HP
	pchar.questTemp.tugs.berglarMarigo.locator = "gate"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarMarigo", "citiz_1", "man", "man", 21, HOLLAND, -1, false));
	sld.name 	= "�������";
	sld.lastname = "����";
	sld.rank = 25;
	sld.city = "Marigo";
	sld.location	= "Marigo_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol6");
	GiveItem2Character(sld, "blade22");
	sld.money = 27480;
	sld.talker = 7; //������ ������
	SetSelfSkill(sld, 60, 10, 10, 40, 40);
	SetCharacterPerk(sld, "CriticalHit");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "HOLLAND_CITIZENS");
	//============> ��������� � ������
	pchar.questTemp.tugs.berglarPanama = "Panama_Cave"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarPanama.hp = 200; //������� HP
	pchar.questTemp.tugs.berglarPanama.locator = "gate"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarPanama", "citiz_4", "man", "man", 27, SPAIN, -1, false));
	sld.name 	= "����";
	sld.lastname = "������";
	sld.rank = 27;
	sld.city = "Panama";
	sld.location	= "Panama_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol3");
	GiveItem2Character(sld, "blade33");
	sld.money = 30180;
	sld.talker = 8; //������ ������
	SetSelfSkill(sld, 10, 10, 70, 50, 70);
	SetCharacterPerk(sld, "AdvancedDefense");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//============> ��������� � ���������
	pchar.questTemp.tugs.berglarCartahena = "Cartahena_Cave"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarCartahena.hp = 170; //������� HP
	pchar.questTemp.tugs.berglarCartahena.locator = "gate"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarCartahena", "citiz_7", "man", "man", 22, SPAIN, -1, false));
	sld.name 	= "������";
	sld.lastname = "��������";
	sld.rank = 22;
	sld.city = "Cartahena";
	sld.location	= "Cartahena_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol6");
	GiveItem2Character(sld, "blade23");
	sld.money = 19980;
	sld.talker = 7; //������ ������
	SetSelfSkill(sld, 60, 10, 10, 50, 50);
	SetCharacterPerk(sld, "Sliding");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//============> ��������� � ����� ��������
	pchar.questTemp.tugs.berglarSantaCatalina = "SantaCatalina_Cave"; //�������� ��� �������� ������
	pchar.questTemp.tugs.berglarSantaCatalina.hp = 230; //������� HP
	pchar.questTemp.tugs.berglarSantaCatalina.locator = "gate"; //�������� �������
	sld = GetCharacter(NPC_GenerateCharacter("BerglarSantaCatalina", "citiz_9", "man", "man", 25, SPAIN, -1, false));
	sld.name 	= "�������";
	sld.lastname = "�������";
	sld.rank = 25;
	sld.city = "SantaCatalina";
	sld.location	= "SantaCatalina_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.dialog.filename   = "Quest\Berglars.c";
	sld.greeting = "pirat_quest";
	GiveItem2Character(sld, "pistol4");
	GiveItem2Character(sld, "topor2");
	sld.money = 30450;
	sld.talker = 7; //������ ������
	SetSelfSkill(sld, 10, 10, 80, 50, 80);
	SetCharacterPerk(sld, "CriticalHit");
	LAi_SetHP(sld, 1.0, 1.0);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
}

void QuestionsInit()
{
	ref sld;
	//============> ���� ��������, ���������� ����� � ����, �� � ���. ��� �� ��������.
	NullCharacter.questions = 21; //���-�� �������� (��������� � ����, �����. ��������� ������ ��������� !!!)
	NullCharacter.questions.q0 = "��� ���������� ������ ������, ������������ � ������������� �������� ���� �������� �������� � ���������� ����� ����������� ���� �� �����? ���������: ����� ��� ����, ����� �������� ����������� �������� � ����� � ���������� �� �����.";
	NullCharacter.questions.a0 = "�������";
	NullCharacter.questions.q1 = "�������� ��������� ���������� ������� ��� ������� ������� ����?";
	NullCharacter.questions.a1 = "�������";
	NullCharacter.questions.q2 = "��� ���������� ���������� �������������� ������� �� ������, ���������� �� ������, �������� ��� ������� ������ ������ �� ���������� ������?";
	NullCharacter.questions.a2 = "������";
	NullCharacter.questions.q3 = "��� ���������� ��������� ��� �������� ������� ������� ����� ��� �����������?";
	NullCharacter.questions.a3 = "�������";
	NullCharacter.questions.q4 = "��� ���������� ����� �����, ������� �����, � ������� �������� ��� ��� ������ ������������ � ������ � ����� ��� �������?";
	NullCharacter.questions.a4 = "������";
	NullCharacter.questions.q5 = "��� ���������� ��������� ���� ������ ������ ������ ��� �����?";
	NullCharacter.questions.a5 = "�������";
	NullCharacter.questions.q6 = "�������� ������ ��� ������ � ������, ��������������� ���, ��� ������� ����� � ������������ � ����� �� � ������ �����. �������� ���������� ������� ����� �������.";
	NullCharacter.questions.a6 = "��������� ������ �����������";
	NullCharacter.questions.q7 = "�������� ����� ������ ������������ ������� �������.";
	NullCharacter.questions.a7 = "�������";
	NullCharacter.questions.q8 = "�������� ����� ������ ��� ���������� �������������� ��������� ���������� �����, � ������� ������������� ������ � ������.";
	NullCharacter.questions.a8 = "�������";
	NullCharacter.questions.q9 = "�������� ����� ������ �������� ����� ��� ���������� ��������� ������ ��������.";
	NullCharacter.questions.a9 = "������";
	NullCharacter.questions.q10 = "�������� ������ ��� ��������� ���������������� �������, �������� ������ ��������� � ���� � �������� ���� ���� ������. ����� ��� �������, ������� ����� ����� �������� � �������, ��� � ������ ���� �������, �������� � ��� �� �����. � ������ ����� ����� ����� ���������� ��������.";
	NullCharacter.questions.a10 = "��� ���� ������";
	NullCharacter.questions.q11 = "�������� ������ ��� ����������� �������� ����������������, ��������� ��� ���� ������ �� �����������, ������� � 67 ���� �� �.�. � ��� ������ ������� ����������� ���� �� ������� � ��������� ������� ��� ������ - �������.";
	NullCharacter.questions.a11 = "���� ������ �������";
	NullCharacter.questions.q12 = "����� ������� ������� ������� �������������� � ������ ������ ������� ��� ����� '������'?";
	NullCharacter.questions.a12 = "�������";
	NullCharacter.questions.q13 = "�������� ������ ���������� �����, ���������� �� ��������� �������.";
	NullCharacter.questions.a13 = "����������";
	NullCharacter.questions.q14 = "��� ��������� ����������� ���, ����������� � 1494 ���� ����� ���� ����� �������� � ����������� �� 50-� �������?";
	NullCharacter.questions.a14 = "��������������� �������";
	NullCharacter.questions.q15 = "�������� ������ ��� �����������, ������ ������������ ������������ ��������.";
	NullCharacter.questions.a15 = "������ ��������";
	NullCharacter.questions.q16 = "��� ��������� ������ �������, ��������� ����� ������ ����� � ���������� ��������� � ����������� � ���� ��������.";
	NullCharacter.questions.a16 = "��������";
	NullCharacter.questions.q17 = "�������� ������ ��� ��������, ������������ ������ ������������ ��������.";
	NullCharacter.questions.a17 = "������� �����";
	NullCharacter.questions.q18 = "�������� ������ ��� ��������, ������������ ������ ������������ ��������.";
	NullCharacter.questions.a18 = "����� ��������";
	NullCharacter.questions.q19 = "�������� ������ ����������� �����, ���������� ��������� � �������.";
	NullCharacter.questions.a19 = "�����-�������";
	NullCharacter.questions.q20 = "�������� ������ ��� ������� ������ ���������� ����.";
	NullCharacter.questions.a20 = "���������� �� ��������";
	NullCharacter.questions.q21 = "��� ������ ���������� ������ � ��������� ����, �������� ����������� �������� � 1494 ���� � �������� �� �������� ��������.";
	NullCharacter.questions.a21 = "������";
	//============> ������������ � ���������
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsPortSpein", "citiz_"+(rand(11)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.rank = 5;
	sld.city = "PortSpein";
	sld.location	= "PortSpein_town";
	sld.location.group = "goto";
	sld.location.locator = "goto1";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "PortSpein";
	sld.greeting = "cit_common";
	sld.talker = 4; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	SetSelfSkill(sld, 10, 10, 10, 10, 10);
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//============> ���������������� � ����������
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsVillemstad", "girl_"+(rand(7)+1), "woman", "towngirl", 5, HOLLAND, -1, false));
	sld.rank = 5;
	sld.city = "Villemstad";
	sld.location	= "Villemstad_town";
	sld.location.group = "goto";
	sld.location.locator = "goto1";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "Villemstad";
	sld.greeting = "Gr_Woman_Citizen";
	sld.talker = 3; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "HOLLAND_CITIZENS");
	//============> ���������������� � ����-�-������
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsPortPax", "girl_"+(rand(7)+1), "woman", "towngirl", 5, FRANCE, -1, false));
	sld.rank = 5;
	sld.city = "PortPax";
	sld.location	= "PortPax_town";
	sld.location.group = "goto";
	sld.location.locator = "goto1";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "PortPax";
	sld.greeting = "Gr_Woman_Citizen";
	sld.talker = 6; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//============> ������������ � ���-�����
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsSanJuan", "citiz_"+(rand(11)+1), "man", "man", 5, SPAIN, -1, false));
	sld.rank = 5;
	sld.city = "SanJuan";
	sld.location	= "SanJuan_town";
	sld.location.group = "goto";
	sld.location.locator = "goto1";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "SanJuan";
	sld.greeting = "cit_common";
	sld.talker = 6; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//============> ������������ � ������
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsCumana", "citiz_"+(rand(11)+1), "man", "man", 5, SPAIN, -1, false));
	sld.rank = 5;
	sld.city = "Cumana";
	sld.location	= "Cumana_town";
	sld.location.group = "goto";
	sld.location.locator = "goto10";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "Cumana";
	sld.greeting = "cit_common";
	sld.talker = 6; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//============> ������������ � ���� �����
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsPortRoyal", "girl_"+(rand(7)+1), "woman", "towngirl", 5, ENGLAND, -1, false));
	sld.rank = 5;
	sld.city = "PortRoyal";
	sld.location	= "PortRoyal_town";
	sld.location.group = "goto";
	sld.location.locator = "goto10";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "PortRoyal";
	sld.greeting = "Gr_Woman_Citizen";
	sld.talker = 3; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//============> ������������ � ��������
	sld = GetCharacter(NPC_GenerateCharacter("QuetionsSantiago", "citiz_"+(rand(11)+1), "man", "man", 5, SPAIN, -1, false));
	sld.rank = 5;
	sld.city = "Santiago";
	sld.location	= "Santiago_town";
	sld.location.group = "goto";
	sld.location.locator = "goto10";
	sld.dialog.filename   = "Quest\Questions.c";
	sld.dialog.currentnode   = "Santiago";
	sld.greeting = "cit_common";
	sld.talker = 5; //������ ������
	sld.quest.numQuestion = SelectQuestions(); //����� �������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");

}
// ==> ��������� ���� ��������, ��������� � ������ ����
int SelectQuestions()
{
	bool bOk = true;
	int iTemp;
	string sTemp;
	while (bOk)
	{
		iTemp = rand(sti(NullCharacter.questions));
		sTemp = "m" + iTemp;
		if (!CheckAttribute(NullCharacter, "questions" + sTemp))
		{
			NullCharacter.questions.(sTemp) = true;
			break;
		}
	}
	return iTemp;
}
// -- ����� --
void PoormansInit()
{
	ref sld;
	//����� � ����-������
	sld = GetCharacter(NPC_GenerateCharacter("SentJons_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.city = "SentJons";	
	sld.location	= "SentJons_town";
	sld.location.group = "goto";
	sld.location.locator = "goto6";
	sld.forStay.locator = "goto6"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto15";
	sld.forSit.locator1 = "goto17"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto4";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//����� � ����������
	sld = GetCharacter(NPC_GenerateCharacter("Bridgetown_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.city = "Bridgetown";
	sld.location	= "Bridgetown_town";
	sld.location.group = "goto";
	sld.location.locator = "goto12";
	sld.forStay.locator = "goto12"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto9";
	sld.forSit.locator1 = "goto19"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto4";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//����� � ������
	sld = GetCharacter(NPC_GenerateCharacter("Beliz_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Beliz";
	sld.location	= "Beliz_town";
	sld.location.group = "goto";
	sld.location.locator = "goto9";
	sld.forStay.locator = "goto9"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto7";
	sld.forSit.locator1 = "goto19"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto3";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ��������
	sld = GetCharacter(NPC_GenerateCharacter("Caracas_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Caracas";
	sld.location	= "Caracas_town";
	sld.location.group = "goto";
	sld.location.locator = "goto20";
	sld.forStay.locator = "goto20"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto1";
	sld.forSit.locator1 = "goto21"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto23";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ���������
	sld = GetCharacter(NPC_GenerateCharacter("Cartahena_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Cartahena";
	sld.location	= "Cartahena_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.forStay.locator = "goto5"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto18";
	sld.forSit.locator1 = "goto14"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto6";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ������
	sld = GetCharacter(NPC_GenerateCharacter("Cumana_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Cumana";
	sld.location	= "Cumana_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.forStay.locator = "goto5"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto13";
	sld.forSit.locator1 = "goto15"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto3";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � �������
	sld = GetCharacter(NPC_GenerateCharacter("Villemstad_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, HOLLAND, -1, false));
	sld.city = "Villemstad";
	sld.location	= "Villemstad_town";
	sld.location.group = "goto";
	sld.location.locator = "goto24";
	sld.forStay.locator = "goto24"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto23";
	sld.forSit.locator1 = "goto7"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto8";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "HOLLAND_CITIZENS");
	//����� � ���-����, ���������
	sld = GetCharacter(NPC_GenerateCharacter("BasTer_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, FRANCE, -1, false));
	sld.city = "BasTer";
	sld.location	= "BasTer_town";
	sld.location.group = "goto";
	sld.location.locator = "goto18";
	sld.forStay.locator = "goto18"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto24";
	sld.forSit.locator1 = "goto16"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto17";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//����� � ������
	sld = GetCharacter(NPC_GenerateCharacter("Havana_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Havana";
	sld.location	= "Havana_town";
	sld.location.group = "goto";
	sld.location.locator = "goto12";
	sld.forStay.locator = "goto12"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto16";
	sld.forSit.locator1 = "goto22"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto23";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ���� �����
	sld = GetCharacter(NPC_GenerateCharacter("PortRoyal_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.city = "PortRoyal";
	sld.location	= "PortRoyal_town";
	sld.location.group = "goto";
	sld.location.locator = "goto15";
	sld.forStay.locator = "goto15"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto16";
	sld.forSit.locator1 = "goto28"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto7";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//����� � ����-��-������, ���������
	sld = GetCharacter(NPC_GenerateCharacter("FortFrance_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, FRANCE, -1, false));
	sld.city = "FortFrance";
	sld.location	= "FortFrance_town";
	sld.location.group = "goto";
	sld.location.locator = "goto4";
	sld.forStay.locator = "goto4"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto14";
	sld.forSit.locator1 = "goto10"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto1";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//����� �� ������
	sld = GetCharacter(NPC_GenerateCharacter("Charles_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.city = "Charles";
	sld.location	= "Charles_town";
	sld.location.group = "goto";
	sld.location.locator = "goto5";
	sld.forStay.locator = "goto5"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto19";
	sld.forSit.locator1 = "goto13"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto3";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//����� � ����� �����
	sld = GetCharacter(NPC_GenerateCharacter("PortoBello_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "PortoBello";
	sld.location	= "PortoBello_town";
	sld.location.group = "goto";
	sld.location.locator = "goto15";
	sld.forStay.locator = "goto15"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto3";
	sld.forSit.locator1 = "goto25"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto12";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ����-�-�����, ���������
	sld = GetCharacter(NPC_GenerateCharacter("PortPax_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, FRANCE, -1, false));
	sld.city = "PortPax";
	sld.location	= "PortPax_town";
	sld.location.group = "goto";
	sld.location.locator = "goto14";
	sld.forStay.locator = "goto14"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto25";
	sld.forSit.locator1 = "goto13"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto11";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//����� � ��������
	sld = GetCharacter(NPC_GenerateCharacter("Santiago_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, SPAIN, -1, false));
	sld.city = "Santiago";
	sld.location	= "Santiago_town";
	sld.location.group = "goto";
	sld.location.locator = "goto6";
	sld.forStay.locator = "goto6"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto3";
	sld.forSit.locator1 = "goto15"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto28";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "SPAIN_CITIZENS");
	//����� � ������, ��� ������
	sld = GetCharacter(NPC_GenerateCharacter("Marigo_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, HOLLAND, -1, false));
	sld.city = "Marigo";
	sld.location	= "Marigo_town";
	sld.location.group = "goto";
	sld.location.locator = "goto16";
	sld.forStay.locator = "goto16"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto1";
	sld.forSit.locator1 = "goto4"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto9";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "HOLLAND_CITIZENS");
	//����� � �������
	sld = GetCharacter(NPC_GenerateCharacter("Tortuga_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, FRANCE, -1, false));
	sld.city = "Tortuga";
	sld.location	= "Tortuga_town";
	sld.location.group = "goto";
	sld.location.locator = "goto23";
	sld.forStay.locator = "goto23"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto16";
	sld.forSit.locator1 = "goto18"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto1";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "FRANCE_CITIZENS");
	//����� � ���� ������, ��������
	sld = GetCharacter(NPC_GenerateCharacter("PortSpein_Poorman", "panhandler_"+(rand(2)+1), "man", "man", 5, ENGLAND, -1, false));
	sld.city = "PortSpein";
	sld.location	= "PortSpein_town";
	sld.location.group = "goto";
	sld.location.locator = "goto6";
	sld.forStay.locator = "goto6"; //��� ��������� � ������ �������
	sld.forSit.locator0 = "goto9";
	sld.forSit.locator1 = "goto14"; //��� ��������, ��� ��������� � ������ �������
	sld.forSit.locator2 = "goto1";
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Common_poorman.c";
	LAi_SetPoorType(sld);
	LAi_SetHP(sld, 50.0, 50.0);
	sld.greeting = "cit_common";
	LAi_group_MoveCharacter(sld, "ENGLAND_CITIZENS");
	//�������� �����
	sld = GetCharacter(NPC_GenerateCharacter("PoorKillSponsor", "smuggler_boss", "man", "man", 40, PIRATE, -1, false));
	sld.name = "������";
	sld.lastname = "�����";
	sld.talker = 8; //������ ������
	LAi_SetLoginTime(sld, 9.0, 22.0);
	sld.Dialog.Filename = "Quest\ForAll_dialog.c";
	sld.dialog.currentnode = "Begin_PoorKill";
	LAi_SetCitizenType(sld);
	LAi_SetHP(sld, 100.0, 100.0);
	sld.greeting = "cit_common";
}

string GetStrSmallRegister(string sBase)
{
	string sResult, Simbol;
	sResult = "";
	int qty = strlen(sBase);
	for (int i=0; i<qty; i++)
	{
		Simbol = GetSymbol(sBase, i);
		switch (Simbol)
		{
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
			case "�": sResult += "�"; continue; break;
		}
		sResult += Simbol;
	}
	return sResult;
}

//===>>> ���������� ���������� �� ��������� ���������
void PearlGen_SetSantaCatalina(string sQuest)
{
	pchar.quest.PearlGen_checkSantaCatalina.win_condition.l1 = "location";
	pchar.quest.PearlGen_checkSantaCatalina.win_condition.l1.location = "Pearl_Jungle_06";
	pchar.quest.PearlGen_checkSantaCatalina.win_condition = "PearlGen_checkSantaCatalina";
}
void PearlGen_SetTeno(string sQuest)
{
	pchar.quest.PearlGen_checkTeno.win_condition.l1 = "location";
	pchar.quest.PearlGen_checkTeno.win_condition.l1.location = "Pearl_Jungle_02";
	pchar.quest.PearlGen_checkTeno.win_condition = "PearlGen_checkTeno";
}
//�������� ����������. ��������� ������ "�������� ���"
void HouseEnc_TimerGoUot(string qName)
{
	if (CheckAttribute(pchar, "questTemp.HouseEncTimer"))
	{
		if (pchar.location == pchar.questTemp.HouseEncTimer && !actLoadFlag && !LAi_grp_alarmactive)
		{
			LAi_group_Attack(characterFromId(pchar.questTemp.HouseEncTimer.Id), Pchar);
		}
		DeleteAttribute(pchar, "questTemp.HouseEncTimer");
	}
}

//�������� ������ � ���������
void Delay_DeleteGroup(string GroupName)
{	
	DoQuestCheckDelay("DeleteGroupOnExitLocation", 2.0);
	pchar.quest.DeleteGroupOnExitLocation.GroupName = GroupName;
}
//��������� �������� �� �������
void SetSkeletonsToLocation(aref _location)
{
	int iRank, iStep;
	float sTime, eTime;
	ref sld;
	bMonstersGen = true; //���� ��������� ��������
	//--> ������� ��������� � �����
	if (!CheckAttribute(_location, "Monsters_step"))
	{
		iStep = 0;
		_location.Monsters_step = sti(MOD_SKILL_ENEMY_RATE / 1.5 + 0.5);
		SaveCurrentNpcQuestDateParam(_location, "Monsters_step"); //������ ���� 
	}
	else
	{
		if (GetNpcQuestPastDayParam(_location, "Monsters_step") > 3)
		{
			iStep = 0;
			_location.Monsters_step = sti(MOD_SKILL_ENEMY_RATE / 1.5 + 0.5);
			SaveCurrentNpcQuestDateParam(_location, "Monsters_step"); //������ ���� 
		}
		else
		{
			iStep = _location.Monsters_step;
			_location.Monsters_step = iStep + sti(MOD_SKILL_ENEMY_RATE / 1.5 + 0.5);
		}
	}
	LAi_group_Delete("EnemyFight");
	//<-- ������� ��������� � �����
	//--> ������� ���� 
	if (sti(pchar.rank) > 6) 
	{
		if (sti(pchar.rank) > 20) iRank = sti(pchar.rank) + MOD_SKILL_ENEMY_RATE;
		else iRank = sti(pchar.rank) + sti(MOD_SKILL_ENEMY_RATE/2);
	}
	else
	{	//�������� ������� ���� �� ������ ����
		iRank = sti(pchar.rank);
	}
	iRank += iStep;
	//<-- ������� ���� 
	aref grp;
	makearef(grp, _location.locators.monsters);
	int num = GetAttributesNum(grp);
	for(int i = 0; i < num; i++)
	{
		sld = GetCharacter(NPC_GenerateCharacter("Skelet"+_location.index+"_"+i, "Skel"+(rand(3)+1), "skeleton", "man", iRank, PIRATE, 0, true));
		//���� ����� �� �������� �� ������� - �������� ������ �����
		if (CheckAttribute(_location, "DestroyGhost"))
		{
			FantomMakeCoolFighter(sld, iRank, 90, 90, LinkRandPhrase("blade33", "blade26", "blade23"), RandPhraseSimple("pistol4", "pistol6"), 100);
			sld.SuperShooter  = true;
		}
		else
		{
			SetFantomParamFromRank(sld, iRank, true);
		}
		LAi_SetWarriorType(sld);
		LAi_warrior_SetStay(sld, true);
		//if (CheckAttribute(_location, "DestroyGhost"))
		//{
		//	LAi_group_MoveCharacter(sld, "EnemyFight");
		//}
		//else
		//{
			LAi_group_MoveCharacter(sld, LAI_GROUP_MONSTERS);
		//}
		ChangeCharacterAddressGroup(sld, _location.id, "monsters", GetAttributeName(GetAttributeN(grp, i)));
	}
	//���������, ���� �� �� ������� ����� �� ������� �� �������
	if (CheckAttribute(_location, "DestroyGhost"))
	{	
		chrDisableReloadToLocation = true;
		characters[sti(_location.DestroyGhost)].DestroyGhost = "GoodResult";
		DeleteAttribute(_location, "DestroyGhost");
		LAi_LocationDisableMonGenTimer(_location.id, 3); //�������� �� �������� 3 ���
		LAi_group_SetCheck(LAI_GROUP_MONSTERS, "OpenTheDoors");
		AddQuestRecordEx(_location.id + "Church_DestroyGhost", "Church_DestroyGhost", "2");
	}
}

//�������� ���� ������� �� ������� � �����
void QuestCheckTakeBoxes(ref itemsRef) 
{	
    ref locLoad = &locations[FindLocation(pchar.location)];
    ref sld;
	int i, num;
	//--> ���� "� �������"
	bMainCharacterInBox = true;
	//<-- ���� "� �������"
	//-------------------- ����� ------------>>>>>>>>>>>>>
	if (CheckAttribute(itemsRef, "Treasure"))
	{
		Log_Info("������, ��� ��� � ���� ����.");
		PlaySound("interface\notebook.wav");
		DeleteAttribute(itemsRef, "Treasure");
		//eddy. ��� ���������������� �������

		if (LAi_LocationIsMonstersGen(locLoad) && !bMonstersGen && LAi_grp_playeralarm == 0)
		{
			SetSkeletonsToLocation(locLoad)
		}
	}
	//<<<<<<<<<<---------- ����� --------------------------
	//-->> ��������� ������� �������, � ����� ��������� 
	if (GetCharacterIndex("HeadMan_2") != -1 && locLoad.id == "PearlTown2_Townhall")
	{
		if (pchar.questTemp.Sharp.SeekSpy != "begin" && pchar.questTemp.Sharp.SeekSpy != "over")
		{	//���� ���� ����� �� ������ ����� ����� - ��������� ���
			pchar.questTemp.Sharp.SeekSpy = "over";
			CloseQuestHeader("SharpPearl_SeekSpy");
    		ChangeCharacterReputation(pchar, -10);
			ChangeCharacterNationReputation(pchar, SPAIN, -10);
		}
		ChangeCharacterReputation(pchar, -20);
		SetNationRelation2MainCharacter(SPAIN, RELATION_ENEMY);
		LAi_group_Attack(characterFromId("HeadMan_2"), Pchar);
		chrDisableReloadToLocation = true;
		pchar.quest.PearlGen_HeadMan_2_death.win_condition.l1 = "NPC_Death";
		pchar.quest.PearlGen_HeadMan_2_death.win_condition.l1.character = "HeadMan_2";
		pchar.quest.PearlGen_HeadMan_2_death.win_condition = "OpenTheDoors";
		return;
	}
	if (GetCharacterIndex("HeadMan_1") != -1 && locLoad.id == "PearlTown1_Townhall")
	{
		if (pchar.questTemp.Sharp.SeekSpy != "begin" && pchar.questTemp.Sharp.SeekSpy != "over")
		{	//���� ���� ����� �� ������ ����� ����� - ��������� ���
			pchar.questTemp.Sharp.SeekSpy = "over";
			CloseQuestHeader("SharpPearl_SeekSpy");
    		ChangeCharacterReputation(pchar, -10);
			ChangeCharacterNationReputation(pchar, SPAIN, -10);
		}		
		ChangeCharacterReputation(pchar, -20);
		SetNationRelation2MainCharacter(SPAIN, RELATION_ENEMY);
		LAi_group_Attack(characterFromId("HeadMan_1"), Pchar);
		chrDisableReloadToLocation = true;
		pchar.quest.PearlGen_HeadMan_1_death.win_condition.l1 = "NPC_Death";
		pchar.quest.PearlGen_HeadMan_1_death.win_condition.l1.character = "HeadMan_1";
		pchar.quest.PearlGen_HeadMan_1_death.win_condition = "OpenTheDoors";
		return;
	}
	//<<-- ��������� ������� �������, � ����� ��������� 
	//-->> ����� ��������.
	if (GetCharacterIndex("Husband") != -1  && locLoad.id == "SanJuan_houseSp6" && characters[GetCharacterIndex("Isabella")].location != "SanJuan_houseSp6" && characters[GetCharacterIndex("Husband")].location == "SanJuan_houseSp6")
	{		
		ChangeCharacterReputation(pchar, -3);
		SetNationRelation2MainCharacter(SPAIN, RELATION_ENEMY);
		sld = characterFromId("Husband");
		LAi_SetWarriorType(sld);
		LAi_SetImmortal(sld, false);
		LAi_group_Attack(sld, Pchar);
		chrDisableReloadToLocation = true;
		LAi_LocationFightDisable(locLoad, false); 
		LocatorReloadEnterDisable(locLoad.id, "reload2", true);
		pchar.quest.Husband_death.win_condition.l1 = "NPC_Death";
		pchar.quest.Husband_death.win_condition.l1.character = "Husband";
		pchar.quest.Husband_death.win_condition = "OpenTheDoors";
	}
	//<<-- ����� ��������.
}
//�������� ������ � ������� ������ ��� ��������. ������ �� ������� ����� + ��� ����, ����� ��������� ����.
void SetOpenDoorCommonLoc(string City, string locationId)
{
	aref arRld, arDis, arRld2, arDis2;
	string LocId;
	makearef(arRld, locations[FindLocation(City + "_town")].reload);
    int Qty2, n, i;
	int Qty = GetAttributesNum(arRld);
	for (i=0; i<Qty; i++)
    {
    	arDis = GetAttributeN(arRld, i);
		LocId = arDis.go;
    	if (LocId == locationId)
    	{
			arDis.disable = false;
			arDis.canEnter = true;
			return;
    	}
		if (arDis.label != "Sea")
		{	
			makearef(arRld2, Locations[FindLocation(LocId)].reload);
			Qty2 = GetAttributesNum(arRld2);
			for (n=0; n<Qty2; n++)
			{
    			arDis2 = GetAttributeN(arRld2, n);
				LocId = arDis2.go;
				if (LocId == locationId)
    			{
					arDis.disable = false;
					arDis.canEnter = true;
					return;					
				}
			}
		}
    }
}

//=============== ������� ���� ====================
string Sharp_choiceAction()
{
	string sBack;
	ref sld;
	//������ ����� � �����, ���� �� ������ ��� ���-��
	Map_ReleaseQuestEncounter("Sharp");
	group_DeleteGroup("Sharp_Group");
	if (rand(3) != 2 && sti(pchar.questTemp.Sharp.count) < 5)
	{	//��� �������, ���������� �� � ������ �����
		string sCity = pchar.questTemp.Sharp.City; //���������� ����, �������� ��� ��
		pchar.questTemp.Sharp.City.rumour = true; //���� ���� ���� � ������
		pchar.questTemp.Sharp.City = GetSharpCity(); //����� ����� 
		sBack = GetSharpRumour_toCityTarget();
		pchar.questTemp.Sharp.count = sti(pchar.questTemp.Sharp.count) + 1; //�������
		AddQuestRecord("SharpPearl", "2");
		AddQuestUserData("SharpPearl", "sOldTarget", XI_ConvertString("Colony" + sCity + "Dat"));
		AddQuestUserData("SharpPearl", "sTarget", XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc"));
		if (GetIslandByCityName(pchar.questTemp.Sharp.City) != pchar.questTemp.Sharp.City)
		{
			AddQuestUserData("SharpPearl", "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(pchar.questTemp.Sharp.City) + "Dat"));
		}
		//��������� ����� ����� �� �����
		sld = characterFromId("Sharp");
		string sGroup = "Sharp_Group";
		Group_FindOrCreateGroup(sGroup);
		Group_SetTaskAttackInMap(sGroup, PLAYER_GROUP);
		Group_LockTask(sGroup);
		Group_AddCharacter(sGroup, sld.id);
		Group_SetGroupCommander(sGroup, sld.id);
		sld.mapEnc.type = "trade";
		sld.mapEnc.worldMapShip = "quest_ship";
		sld.mapEnc.Name = "���� '����������'";
		string sColony= SelectAnyColony(sCity); //�������, ������ ������ ����
		int daysQty = GetMaxDaysFromIsland2Island(GetArealByCityName(sColony), GetArealByCityName(pchar.questTemp.Sharp.City))+3; //���� ������� ���� � �������
		Map_CreateTrader(sColony, pchar.questTemp.Sharp.City, sld.id, daysQty);
		Log_TestInfo("���������� ������������ �� " + sColony + "  � " + pchar.questTemp.Sharp.City);
	}
	else
	{
		if (rand(5) < 4)
		{
			sBack = GetSharpRumour_inIsland();
			pchar.questTemp.Sharp.Island = GiveArealByLocation(&locations[FindLocation(pchar.location)]);
			pchar.questTemp.Sharp.count = 0; //������� ����� ��� ����. ����
			pchar.questTemp.Sharp.price = 5000+rand(20)*1000; //���� �� �������
			pchar.questTemp.Sharp.price.evil = rand(1); //������ ��� ����, ��� ������
			Pchar.quest.Sharp_loginNearIsland.win_condition.l1 = "location";
			Pchar.quest.Sharp_loginNearIsland.win_condition.l1.location = pchar.questTemp.Sharp.Island;
			Pchar.quest.Sharp_loginNearIsland.function = "Sharp_loginNearIsland";
			AddQuestRecord("SharpPearl", "3");
			SaveCurrentQuestDateParam("questTemp.Sharp");
			Log_QuestInfo("��������� �� ������.");
		}
		else
		{
			sBack = GetSharpRumour_inCity();
			pchar.questTemp.Sharp.count = 0; //������� ����� ��� ����. ����
			pchar.questTemp.Sharp.price = 5000+rand(20)*1000; //���� �� �������
			pchar.questTemp.Sharp.price.evil = rand(1); //������ ��� ����, ��� ������
			float locx, locy, locz;
			GetCharacterPos(pchar, &locx, &locy, &locz);
			sld = &characters[GetCharacterIndex("Sharp")];
			sld.dialog.currentnode = "CitySharp";
			LAi_SetCitizenType(sld);
			LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
			ChangeCharacterAddressGroup(sld, pchar.location, "patrol", LAi_FindFarLocator("patrol", locx, locy, locz));
			AddQuestRecord("SharpPearl", "11");
			Log_QuestInfo("��������� �� �����.");
		}
	}
	return sBack;
}

//���� ����� ����� 
string GetSharpCity()
{
	int n, iRes;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		if (colonies[n].nation != "none" && GetRelation2BaseNation(sti(colonies[n].nation)) != RELATION_ENEMY && GiveArealByLocation(&locations[FindLocation(pchar.location)]) != colonies[n].island) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	iRes = storeArray[rand(howStore-1)];
	return colonies[iRes].id;
}
//����� �� ���������
string GetSharpRumour_default()
{
	string sRumour;
	switch (rand(2))
    {
        case 0: sRumour = "������� ���� ��� �����, � ��� �����. �� ������ ��� � ������ ���, � ���� ���� - �� ����."; break;
        case 1: sRumour = "������� ���� ������� � ��� �����. ��� �� ������ � �� ����, � ��� ��� ����� ���."; break;
        case 2:	sRumour = "������� ���������� ���� ���� �� ������ ��������� �� ����..."; break;
    }
	return sRumour;
}
//����� � ������ �����
string GetSharpRumour_toCityTarget()
{
	string sRumour;
	switch (rand(2))
    {
        case 0: sRumour = "������� ���������� ���� ��� � ��� � ������ �, ��� ������, ��������� '����������'... �� ������ ��� ����� ���. �� ������ �� ���� � " + XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc") + "."; break;
        case 1: sRumour = "������, ������� ���� ��� � ������ �������. ������� �����, ���� ����������... ������, ������, ��� �� ������� ��� �����. ������� �� ���� � " + XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc") + "."; break;
        case 2:	sRumour = "�� ������, ������� ��� ������ ����� ������������ ������� ����. ��, � ������ �������� �������������, ������ ���������� �������� '�� �����' �� ���������. ����... � ������ ��� ��� � ��� � ������. �������, �� ���� � " + XI_ConvertString("Colony" + pchar.questTemp.Sharp.City + "Acc") + ". ����� �� �������..."; break;
    }
	return sRumour;
}
//����� � ������
string GetSharpRumour_inIsland()
{
	string sRumour;
	switch (rand(2))
    {
        case 0: sRumour = "�� ������, ������� ���� ������ ��� ����� �� ������ �� ����� �����. ���� �� �� ������� � ��� ���� �����, �� ������� �� ��� ����� � �����."; break;
        case 1: sRumour = "������� ����... �� ���, ��� ��� �������. ������, �� ������ ��� ������� ��� �����. ���� �� ������������, �� ������� ��� ������� � ��������� �����."; break;
        case 2:	sRumour = "���������� ���� ��������� � ��� ����� ����� �� ������ ����� � ����."; break;
    }
	return sRumour;
}
//����� � ������
string GetSharpRumour_inCity()
{
	string sRumour;
	switch (rand(2))
    {
        case 0: sRumour = "���� ����� ����� ������� ���� ���������� �� ���� ����� � ����. ����� ����� �� ������, �������, ��� �� � �������..."; break;
        case 1: sRumour = "������ ��� �� ����� �������� ���������� � ��������� ������. ��������, ������ ��, ���..."; break;
        case 2:	sRumour = "������� ���� ������ ������ �� ������. � �� ��� ����, ���� �� �� ������� � ��������� ���..."; break;
    }
	return sRumour;
}

//=============== ������ ����� �����.  ====================
string BlueBurd_setTradeShip()
{
	pchar.questTemp.BlueBird.Island = GiveArealByLocation(&locations[FindLocation(pchar.location)]);
	Pchar.quest.BlueBird_loginFleut.win_condition.l1 = "location";
	Pchar.quest.BlueBird_loginFleut.win_condition.l1.location = pchar.questTemp.BlueBird.Island;
	Pchar.quest.BlueBird_loginFleut.function = "BlueBird_loginFleut";
	pchar.questTemp.BlueBird.nation = colonies[FindColony(pchar.questTemp.BlueBird.City)].nation; //����� �������, ������ ��������
	aref aName;
	makearef(aName, pchar.questTemp.BlueBird);
	SetRandomNameToShip(aName);
	AddQuestRecord("Xebeca_BlueBird", "10");
	AddQuestUserData("Xebeca_BlueBird", "sCity", XI_ConvertString("Colony" + pchar.questTemp.BlueBird.City + "Dat"));
	AddQuestUserData("Xebeca_BlueBird", "sShipName", "'" + aName.Ship.Name + "'");
	AddQuestUserData("Xebeca_BlueBird", "sCity_2", XI_ConvertString("Colony" + pchar.questTemp.BlueBird.City + "Gen"));
	AddQuestUserData("Xebeca_BlueBird", "sTradeName", GetFullName(characterFromId(pchar.questTemp.BlueBird.City + "_trader")));
	AddQuestUserData("Xebeca_BlueBird", "sIsland", XI_ConvertString(pchar.questTemp.BlueBird.Island + "Gen"));
	SaveCurrentQuestDateParam("questTemp.BlueBird");
	return GetBlueBirdRumour_Ship(); //����� �����
}
//����� �� ������, ������� �� ������� ��������
string GetBlueBirdRumour_Ship()
{
	string sRumour;
	switch (rand(2))
    {
        case 0: sRumour = "�� ������, ������� ��������, " + GetFullName(characterFromId(pchar.questTemp.BlueBird.City + "_trader")) + ", ����� ���������� ����������� ����� '" + pchar.questTemp.BlueBird.Ship.Name + "' � ������ ���� �� �������. ������� ����� ����� �� �����."; break;
		case 1: sRumour = "��, � ���� � ������ ���������� ������������� ���� �������! " + GetFullName(characterFromId(pchar.questTemp.BlueBird.City + "_trader")) + " ����� �������� ����������� ����� '" + pchar.questTemp.BlueBird.Ship.Name + "' �� �������, ��� ������ ��� ����� �� ������ �����."; break;
        case 2:	sRumour = "�-��, � " + GetFullName(characterFromId(pchar.questTemp.BlueBird.City + "_trader")) + " ��� �������� � ��������. ������ ����� ��� �������� ����� ��� ���������� � �����, � ������� ���� ��� ���������� � ���. ����� '" + pchar.questTemp.BlueBird.Ship.Name + "' ���� ����� �� �������, ��� ��!"; break;
    }
	return sRumour;
}
//======================= ��������� ������� ===================================
void PiratesLineInit()
{
	ref sld;
	pchar.questTemp.piratesLine = "begin";
	sld = GetCharacter(NPC_GenerateCharacter("QuestPirate1", "officer_9", "man", "man", 20, PIRATE, -1, true));
	sld.name = "�������";
	sld.lastname = "�����";
	sld.rank = 20;
	sld.city = "PuertoPrincipe";
	sld.location	= "PuertoPrincipe_town";
	sld.location.group = "goto";
	sld.location.locator = "goto7";
	sld.dialog.filename   = "Quest\PiratesLine_dialog.c";
	sld.dialog.currentnode   = "PiratesMan1";
	sld.greeting = "pirat_quest";
	sld.talker = 8; //������ ������
	LAi_SetLoginTime(sld, 6.0, 21.99);
	LAi_SetCitizenType(sld);
	LAi_group_MoveCharacter(sld, "PIRATE_CITIZENS");
}

//���� ����� ������������ �����, ��������� ������� ���������� � ����������
string GetQuestNationsCity(int _nation)
{
	int n, iRes;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		if (colonies[n].nation != "none" && sti(colonies[n].nation) == _nation && GiveArealByLocation(&locations[FindLocation(pchar.location)]) != colonies[n].island) //�� �� ���� ������
		{
			if (GetCharacterIndex(colonies[n].id + "_tavernkeeper") > 0 && GetCharacterIndex(colonies[n].id + "_usurer") > 0)
			{
				storeArray[howStore] = n;
				howStore++;
			}
		}
	}
	if (howStore == 0) return "none";
	iRes = storeArray[rand(howStore-1)];
	return colonies[iRes].id;
}

//���� �� ��������� �������, ���� ����� �������
string SelectNotEnemyColony(ref NPChar)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		nation = GetNationRelation(sti(npchar.nation), sti(colonies[n].nation));
		if (nation != RELATION_ENEMY && colonies[n].id != "Panama" && colonies[n].nation != "none" && GetIslandByCityName(npchar.city) != colonies[n].islandLable) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}

//���� ����� ������������ �������, ���� ����� �������
string SelectAnyColony(string _City)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		if (colonies[n].id != "Panama" && colonies[n].id != "FortOrange" && colonies[n].nation != "none" && sti(colonies[n].nation) != PIRATE && GetIslandByCityName(_City) != colonies[n].islandLable) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}

string SelectAnyColony2(string _City1, string _City2)
{
	int n, nation;
    int storeArray[MAX_COLONIES];
    int howStore = 0;

	for(n=0; n<MAX_COLONIES; n++)
	{
		if (colonies[n].id != "Panama" && colonies[n].id != "FortOrange" && colonies[n].nation != "none" && sti(colonies[n].nation) != PIRATE && GetIslandByCityName(_City1) != colonies[n].islandLable && GetIslandByCityName(_City2) != colonies[n].islandLable) //�� �� ���� ������
		{
			storeArray[howStore] = n;
			howStore++;
		}
	}
	if (howStore == 0) return "none";
	nation = storeArray[rand(howStore-1)];
	return colonies[nation].id;
}

ref CheckLSCCitizen()
{
	ref rCharacter;
	string sTemp, sSeeked;
	sSeeked = GetStrSmallRegister(dialogEditStrings[3]);
	if (sSeeked == "�������" || sSeeked == "�������a")
	{
		return characterFromId("LSCMayor");
	}
	if (sSeeked == "����������" || sSeeked == "�����������" || sSeeked == "���������"|| sSeeked == "����������" || sSeeked == "������ �������" || sSeeked == "������� �������")
	{
		return characterFromId("LSCBarmen");
	}
	if (sSeeked == "����������" || sSeeked == "����������")
	{
		return characterFromId("LSCwaitress");
	}
	for(int n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(rCharacter, Characters[n]);
		if (CheckAttribute(rCharacter, "city") && rCharacter.city == "LostShipsCity")
		{
			sTemp = GetStrSmallRegister(rCharacter.name + " " + rCharacter.lastname);
			if (findsubstr(sSeeked, sTemp , 0) != -1)
			{
				return rCharacter;			
			}
		}
	}
	return &NullCharacter;
}

string WhereLSCCitizen(ref rCharacter)
{
	string sShip = "none";
	string sTemp;
	if (rCharacter.location	== "LostShipsCity_town")
	{
		sTemp = rCharacter.location.locator;
		sTemp = strcut(sTemp, 4, 5);
		switch (sTemp)
		{
			case "01": sShip = "�� ��������� '��������', ��� ������"; break;
			case "02": sShip = "�� ������� '��������'"; break;
			case "03": sShip = "�� ������ '����� �����'"; break;
			case "04": sShip = "�� ������� '����������'"; break;
			case "05": sShip = "�� ��������� '������', ��� �������"; break;
			case "06": sShip = "�� ������ '�����', � ������� �����"; break;
			case "07": sShip = "�� ������ '�����', � �������� �����"; break;
			case "08": sShip = "�� ������� '������', ��� �������"; break;
			case "09": sShip = "�� ����� '�����'"; break;
			case "10": sShip = "�� ������� '���������'"; break;
			case "11": sShip = "�� ������� '��� ��������', ��� ���������� ��������"; break;
			case "12": sShip = "�� ����� '��� ��������', � ������� �����"; break;
			case "13": sShip = "�� ����� '��� ��������', � �������� �����"; break;
			case "14": sShip = "�� ����� '����� ����������'"; break;
			case "15": sShip = "�� ������� '���'"; break;
			case "16": sShip = "�� ������� '�������'"; break;
		}
	}
	else
	{
		switch (rCharacter.location)
		{
			case "AvaShipInside3": sShip = "� ������� '���'"; break;
			case "CarolineBank": sShip = "� ������� '��������'"; break;
			case "CeresSmithy": sShip = "�� ������ '����� �����'"; break;
			case "EsmeraldaStoreBig": sShip = "� ������� '����������'"; break;
			case "FenixPlatform": sShip = "� ��������� '������'"; break;
			case "FernandaDiffIndoor": sShip = "�� ������ '�������� ���������'"; break;
			case "FleuronTavern": sShip = "� ��������� '������', � �������"; break;
			case "FurieShipInside2": sShip = "� ������ '�����'"; break;
			case "GloriaChurch": sShip = "� ������� '������', � ������"; break;
			case "PlutoStoreSmall": sShip = "�� ����� '�����'"; break;
			case "ProtectorFisher": sShip = "� ������� '���������'"; break;
			case "SanAugustineResidence": sShip = "� ������� '��� ��������', � ���������� ��������"; break;
			case "SanGabrielMechanic": sShip = "� ����� '��� ��������'"; break;
			case "SantaFlorentinaShipInside4": sShip = "� ����� '����� ����������'"; break;
			case "TartarusPrison": sShip = "� ��������� '��������', � ������"; break;
			case "VelascoShipInside1": sShip = "� ������� '�������'"; break;
		}
	}
	return sShip;
}

//������ ������� ������� ������. �������� � ����� ����, ������� ������� � ����������� �������
void SetCapitainFromSeaToCity(string sChar)
{
	ref sld = &characters[GetCharacterIndex(sChar)];
	sld.quest = "InCity"; //���� � ������
	sld.City = sld.quest.targetCity; //�������� �������� ������ ��������
	sld.location	= sld.City + "_PortOffice";
	sld.location.group = "goto";
	sld.location.locator = "goto"+(rand(2)+1);
	LAi_SetCitizenType(sld);

	/*int iColony = FindColony(sld.City);
	string sGroup = "PorpmansShip_" + sld.index
	group_DeleteGroup(sGroup);
	Group_AddCharacter(sGroup, sld.id);			
	Group_SetGroupCommander(sGroup, sld.id);
	Group_SetAddress(sGroup, colonies[iColony].island, "quest_ships", "Quest_ship_"+(rand(2)+1));
	Group_SetTaskNone(sGroup);*/

	//������ ����� ������� ����� ����� � ����
	int Qty = rand(4)+3;
	string name = "Timer_" + sld.id;
	PChar.quest.(name).win_condition.l1            = "Timer";
    PChar.quest.(name).win_condition.l1.date.day   = GetAddingDataDay(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.month = GetAddingDataMonth(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.year  = GetAddingDataYear(0, 0, Qty);
    PChar.quest.(name).function					= "SetCapitainFromCityToSea";
	pchar.quest.(name).CapId = sld.id; //� ���������� �������� Id ����
	//���� ����, ��� ��� � ������
	AddSimpleRumourCity(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ������ � ������. �������, ��� ����� " + GetFullName(sld) + ". �� ������ ������� ���� � ���������� �����.", 
		"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ������ ����� ������� � ���������� �����.", 
		"���� �� ������ ���������� � ��������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ����� �������� � ���������� �����. " + GetFullName(sld) + " ������ ���..."), sld.City, Qty, 1, "none");
}
//������ ������� ������� ������. ������� � �������� �� ������
void PortmansBook_writeQuestBook(ref rid)
{
	ref sld = characterFromId(rid.addString);
	string sTitle = characters[GetCharacterIndex(sld.quest.firstCity + "_PortMan")].id + "PortmansBook_Delivery";
	AddQuestRecordEx(sTitle, "PortmansBook_Delivery", "4");
	AddQuestUserData(sTitle, "sCapName", GetFullName(sld));
	AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + rid.city + "Gen"));
	AddQuestUserData(sTitle, "sTargetCity", XI_ConvertString("Colony" + sld.quest.targetCity + "Acc"));
	if (GetIslandByCityName(sld.quest.targetCity) != sld.quest.targetCity)
	{
		AddQuestUserData(sTitle, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(sld.quest.targetCity) + "Dat"));
	}
}

//��������� ����-����. �������� � ����� ����, ������� ������� � ����������� �������
void SetRobberFromMapToSea(string sChar)
{
	ref sld = &characters[GetCharacterIndex(sChar)];
	sld.quest = "InPort"; //���� ���� ����� � �����
	sld.City = sld.quest.targetCity; //�������� �������� ������ ��������
	int iColony = FindColony(sld.City);
	sld.nation = colonies[iColony].nation; //����� �����, ��� � � ������

	string sGroup = "SeekCapShip_" + sld.index
	group_DeleteGroup(sGroup);
	Group_AddCharacter(sGroup, sld.id);			
	Group_SetGroupCommander(sGroup, sld.id);
	Group_SetAddress(sGroup, colonies[iColony].island, "quest_ships", "Quest_ship_"+(rand(2)+1));
	Group_SetTaskNone(sGroup);

	//������ ����� ������� ����� ����� �� �����
	int Qty = rand(5)+4;
	string name = "SecondTimer_" + sld.id;
	PChar.quest.(name).win_condition.l1            = "Timer";
    PChar.quest.(name).win_condition.l1.date.day   = GetAddingDataDay(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.month = GetAddingDataMonth(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.year  = GetAddingDataYear(0, 0, Qty);
    PChar.quest.(name).function					= "SetRobberFromSeaToMap";
	pchar.quest.(name).CapId = sld.id; //� ���������� �������� Id ����
	//���� ����, ��� ��� �� �����
	AddSimpleRumourCity(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ����� � ����� �� �����. �������, ��� ����� " + GetFullName(sld) + ". �������� ���, ����� � ���, �� ����� �� ������ ������.", 
		"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ������ ����� ���������, ������ ���� ����� �� ������ � ����. ������� ����� �� �����, �� ������� � ����� �� �������, ������������ ���������� �� �������. � � ������� ���?..", 
		"���� �� ������ ���������� � ��������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� �������� � ����. " + GetFullName(sld) + " ����� �� ��� �����... � �����, �� ������ ����� � �����, �� �� ����� �� ������. ��������� ������� ����, ���� � �������� ������ ������������ �� �����."), sld.City, Qty, 1, "none");
}
//��������� ����-����. ������� � �������� �� ������
void PortmansSeekShip_writeQuestBook(ref rid)
{
	ref sld = characterFromId(rid.addString);
	ref npchar = &characters[GetCharacterIndex(sld.quest.firstCity + "_PortMan")];
	string sTitle = npchar.id + "Portmans_SeekShip";
	AddQuestRecordEx(sTitle, "Portmans_SeekShip", "3");
	AddQuestUserData(sTitle, "sShipTypeName", GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName)));
	AddQuestUserData(sTitle, "sShipName", sld.Ship.name);
	AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + rid.city + "Gen"));
	int iTemp = RealShips[sti(sld.Ship.Type)].basetype;
	AddQuestUserData(sTitle, "sSexWord", GetShipSexWord(ShipsTypes[iTemp].name, "����", "����"));
	AddQuestUserData(sTitle, "sTargetCity", XI_ConvertString("Colony" + sld.quest.targetCity + "Acc"));
	if (GetIslandByCityName(sld.quest.targetCity) != sld.quest.targetCity)
	{
		AddQuestUserData(sTitle, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(sld.quest.targetCity) + "Dat"));
	}
}

//������ ����� ����������
void CitizCapFromMapToCity(string sChar)
{
	ref sld = &characters[GetCharacterIndex(sChar)];
	sld.quest = "InCity"; //���� ���� ����� �� ������
	sld.City = sld.quest.targetCity; //�������� �������� ������ ��������
	int iColony = FindColony(sld.City);
	sld.nation = colonies[iColony].nation; //����� �����, ��� � � ������
	if (rand(1))
	{	//����� � ������
		sld.location	= sld.City + "_town";
		sld.location.group = "goto";
		sld.location.locator = "goto"+(rand(5)+1);
		LAi_SetCitizenType(sld);
		string slai_group = GetNationNameByType(sti(colonies[iColony].nation))  + "_citizens";
		LAi_group_MoveCharacter(sld, slai_group);
		//���� ����, ��� ��� �� �����
		AddSimpleRumourCity(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' � ������. �������, ��� ����� " + GetFullName(sld) + ".", 
			"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ������ ����� ������� � ������.", 
			"���� �� ������ ���������� � ��������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ������� ��� � �� ������. " + GetFullName(sld) + " ����� �� ��� �����..."), sld.City, Qty, 1, "none");
	}
	else
	{	//����� �� ����� � �����
		string sGroup = "SeekCapShip_" + sld.index
		group_DeleteGroup(sGroup);
		Group_AddCharacter(sGroup, sld.id);			
		Group_SetGroupCommander(sGroup, sld.id);
		Group_SetAddress(sGroup, colonies[iColony].island, "quest_ships", "Quest_ship_"+(rand(2)+1));
		Group_SetTaskNone(sGroup);
		//���� ����, ��� ��� �� �����
		AddSimpleRumourCity(LinkRandPhrase("������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ��������� � ����� �� �����. �������, ��� ����� " + GetFullName(sld) + ".", 
			"�� ������, �������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "' ������ ����� ������� � ����� �����. ��� ������� ����� �� �����.", 
			"���� �� ������ ���������� � ��������� " + GetStrSmallRegister(XI_ConvertString(RealShips[sti(sld.Ship.Type)].BaseName + "Acc")) + " '" + sld.Ship.name + "', �� ������� ��� ������� � ����� �����. " + GetFullName(sld) + " ����� �� ��� �����..."), sld.City, Qty, 1, "none");
	}
	//������ ����� ������� ����� ����� �� �����
	int Qty = rand(7)+5; //����� ������� ���� ������ �� �����
	string name = "SecondTimer_" + sld.id;
	PChar.quest.(name).win_condition.l1            = "Timer";
    PChar.quest.(name).win_condition.l1.date.day   = GetAddingDataDay(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.month = GetAddingDataMonth(0, 0, Qty);
    PChar.quest.(name).win_condition.l1.date.year  = GetAddingDataYear(0, 0, Qty);
    PChar.quest.(name).function					= "CitizCapFromSeaToMap";
	pchar.quest.(name).CapId = sld.id; //� ���������� �������� Id ����
}

//������ ���� �� �������. ������� � �������� �� ������
void CitizSeekCap_writeQuestBook(ref rid)
{
	ref sld = characterFromId(rid.addString);
	ref npchar = &characters[GetCharacterIndex("QuestCitiz_" + sld.quest.cribCity)];
	string sTitle = sld.quest.cribCity + "SCQ_" + npchar.quest.SeekCap;
	AddQuestRecordEx(sTitle, "SCQ_" + npchar.quest.SeekCap, "3");
	AddQuestUserData(sTitle, "sCapName", GetFullName(sld));
	AddQuestUserData(sTitle, "sCity", XI_ConvertString("Colony" + rid.city + "Gen"));
	AddQuestUserData(sTitle, "sTargetCity", XI_ConvertString("Colony" + sld.quest.targetCity + "Acc"));
	if (GetIslandByCityName(sld.quest.targetCity) != sld.quest.targetCity)
	{
		AddQuestUserData(sTitle, "sAreal", ", ��� ��������� �� " + XI_ConvertString(GetIslandByCityName(sld.quest.targetCity) + "Dat"));
	}
}

//������� ����� � ���������
//��������� ����� ������ � �����
void SetShipInBridgetown()
{
	int n = FindLocation("Bridgetown_town");
	locations[n].models.always.ship = "Bridgetown_ship";
	locations[n].models.always.shipreflect = "Bridgetown_shipreflect";
	Locations[n].models.always.shipreflect.sea_reflection = 1;
	locations[n].models.always.locators = "Bridgetown_locatorsShip";
	locations[n].models.day.fonarShip = "Bridgetown_fdShip";
	locations[n].models.night.fonarShip = "Bridgetown_fnShip";
	locations[n].models.day.charactersPatch = "Bridgetown_patchship_day";
	locations[n].models.night.charactersPatch = "Bridgetown_patchship_night";

	locations[n].reload.ship1.name = "reloadShip";
	locations[n].reload.ship1.go = "Cabin";
	locations[n].reload.ship1.emerge = "reload1";
	locations[n].reload.ship1.autoreload = "0";
	locations[n].reload.ship1.label = "cabine";

	n = FindLocation("Cabin");
	DeleteAttribute(&locations[n], "boarding");
	DeleteAttribute(&locations[n], "camshuttle");
	DeleteAttribute(&locations[n], "CabinType");

	locations[n].reload.l1.name = "reload1";
	locations[n].reload.l1.go = "Bridgetown_town";
	locations[n].reload.l1.emerge = "reloadShip";
	locations[n].reload.l1.autoreload = "0";
	locations[n].reload.l1.label = "Ship";
}
//������ ����� ������ �� ������
void RemoveShipFromBridgetown()
{
	int n = FindLocation("Bridgetown_town");	
	DeleteAttribute(&locations[n], "models.always.ship");
	DeleteAttribute(&locations[n], "models.always.shipreflect");
	DeleteAttribute(&locations[n], "models.always.shipreflect.sea_reflection");
	locations[n].models.always.locators = "Bridgetown_locators";
	DeleteAttribute(&locations[n], "models.day.fonarShip");
	DeleteAttribute(&locations[n], "models.night.fonarShip");
	locations[n].models.day.charactersPatch = "Bridgetown_patch_day";
	locations[n].models.night.charactersPatch = "Bridgetown_patch_night";
	DeleteAttribute(&locations[n], "reload.ship1");

	n = FindLocation("Cabin");
	DeleteAttribute(&locations[n], "reload");
	Locations[n].boarding = "true";
	Locations[n].boarding.nextdeck = "";
	Locations[n].camshuttle = 1;
	Locations[n].boarding.locatorNum = 1;
	Locations[n].CabinType = true;
	locations[n].environment.weather.rain = false;
	Locations[n].boarding.Loc.Hero = "loc0";
    Locations[n].boarding.Loc.Capt = "aloc2";
}

//������� ���
//����������� �� ��������
void LSC_checkBoxes()
{
	ref loc;
	aref arBox;
	string sName;
	for(int i=0; i<MAX_LOCATIONS; i++)
	{				
		loc = &locations[i];
		if (CheckAttribute(loc, "fastreload") && loc.fastreload == "LostShipsCity" && loc.id != "FernandaDiffIndoor")
		{	
			for(int n=1; n<=MAX_HANDLED_BOXES; n++)
			{
				sName = "private" + n;
				if (CheckAttribute(loc, sName))
				{					
					makearef(arBox, loc.(sName));
					if (CheckAttribute(arBox, "equip") && rand(1))
					{
						DeleteAttribute(arBox, "items");
						arBox.items = "";
					}
				}
				else break;
			}
		}
	}
}