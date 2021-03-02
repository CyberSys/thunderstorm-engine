#include "TEXT\characters\QuestsUtilite.h"

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
                Log_SetStringToLog(QUEST_UTIL[0]);
            }
            else
            {
                Log_SetStringToLog(QUEST_UTIL[1]);
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
        /*if (CheckAttribute(PChar, "GenQuest.GhostShip.TakeShotgun"))
        {
            DeleteAttribute(PChar, "GenQuest.GhostShip.TakeShotgun");
            _location.box1.items.pistol7 = 1;
        }*/
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
            _location.box1.money = (cRand(30)+1) * 40;
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
    // �������, ����� �7. ������������� � �����
	if (_npchar.id == "MushketCap")
	{
        DeleteAttribute(_location, "box1");
        _location.box1.money = 103;
		_location.box1.items.mushket2x2 = 1;
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
    DeleteAttribute(_Character, "Killer.status"); // ����� �������� '��� �������� �� �������'
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
    EquipCharacterbyItem(_Character, _Blade);
    TakeNItems(_Character,"potion1", rand(MOD_SKILL_ENEMY_RATE)+1);
    if (_Gun != "")
	{
		GiveItem2Character(_Character, _Gun);
		EquipCharacterbyItem(_Character, _Gun);
		TakeNItems(_Character,"bullet", 100);
	}
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
                strBack = LinkRandPhrase(QUEST_UTIL[2],
                          QUEST_UTIL[3],
                          QUEST_UTIL[4]);
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
                strBack = LinkRandPhrase(QUEST_UTIL[5],
                          QUEST_UTIL[6],
                          QUEST_UTIL[7]);
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
                strBack = LinkRandPhrase(QUEST_UTIL[8],
                          QUEST_UTIL[9],
                          QUEST_UTIL[10]);
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
            strBack = "NPCStringReactionRepeat error!!!"
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
                strBack = LinkRandPhrase(QUEST_UTIL[11],
                          QUEST_UTIL[12],
                          QUEST_UTIL[13]);
            }
            else
            {
                strBack = _strBad1;
            }
        break;
        case "2":
            if (_strBad2 == "none" || _strBad2 == "")
            {
                strBack = LinkRandPhrase(QUEST_UTIL[14],
                          QUEST_UTIL[15],
                          QUEST_UTIL[16]);
            }
            else
            {
                strBack = _strBad2;
            }
        break;
        case "3":
            if (_strBad3 == "none" || _strBad3 == "")
            {
                strBack = LinkRandPhrase(QUEST_UTIL[17],
                          QUEST_UTIL[18],
                          QUEST_UTIL[19]);
            }
            else
            {
                strBack = _strBad3;
            }
        break;
        case "4":
            strBack = "HeroStringReactionRepeat error!!!"
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
            Log_SetStringToLog("DialogGoNodeRepeat#4 error!!!");
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
    else {Log_Info("CharacterAddAngry error!!!");}
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
	if (CheckAttribute(pchar, "GenQuest.StoreGoods.StoreIdx") && refStore.index == pchar.GenQuest.StoreGoods.StoreIdx)
	{
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
	if (CheckAttribute(refChar, "CaptanId"))
	{
        pchar.GenQuest.QuestAboardCabinDialogIdx = refChar.index;
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
		if (refChar.CaptanId == "IsaAttackCap" && pchar.RomanticQuest == "AttackBrig")
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
			refChar.Dialog.CurrentNode = "SeekCap"; //���� ���������� ����
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
		//������������ �����, ����� ����� � �������
		if (refChar.CaptanId == "Danielle")
		{
		    LAi_SetCheckMinHP(refChar, 10, true, "QuestAboardCabinDialog");  // ������� �� ���
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
	SetCharacterRemovable(&characters[sti(pchar.GenQuest.LastQuestPrisonerIdx)], false);
	DoQuestCheckDelay("LAi_ReloadBoarding", 1.0);
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
//���� ������������ ��������, �������� '�������� ����������� ����-������� ��������'
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
	ch.name 	= QUEST_UTIL[20];
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
	ch.TiedItems.itm1.model = "HandsItems\meet";
	ch.TiedItems.itm1.locator = "Saber_hand";	
	ch.TiedItems.itm2.model = "HandsItems\cup";
	ch.TiedItems.itm2.locator = "Saber_hand";
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
	ch.greeting = "Gr_Atilla";
	// ���� ��� :)
	ch = GetCharacter(NPC_GenerateCharacter("Husband", "Usurer_7", "man", "man", 22, PIRATE, -1, false));	
	ch.name 	= QUEST_UTIL[21];
	ch.lastname = QUEST_UTIL[22];
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
	ch.greeting = "Gr_OliverTrast";
}
//���������� ��������� �� ����������. ����� �� ��������.
void SetIsabellaTalker(string qName)
{
	ref chr = characterFromID("Isabella");
	chr.talker = 10;
}




string GetStrSmallRegister(string sBase)
{
	int numSimbol;
	string sResult, Simbol;
	string LowRegister = XI_ConvertString("alphabet low register");
	string UpRegister  = XI_ConvertString("alphabet up register");
	sResult = "";
	int qty = strlen(sBase);
	for (int i=0; i<qty; i++)
	{
		Simbol = GetSymbol(sBase, i);
		numSimbol = findsubstr(UpRegister, Simbol , 0);
  		if (numSimbol != -1)
 		{
			Simbol = GetSymbol(LowRegister, numSimbol);
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
//�������� ����������. ��������� ������ '�������� ���'
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
			FantomMakeCoolFighter(sld, iRank, 90, 90, LinkRandPhrase("blade33", "blade26", "blade23"), RandPhraseSimple("pistol6", "pistol3"), 20);
			DeleteAttribute(sld, "SuperShooter");
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
	ref locLoad = &locations[reload_location_index];
    ref sld;
	int i, num;
	//--> ���� '� �������'
	bMainCharacterInBox = true;
	//<-- ���� '� �������'
	//-------------------- ����� ------------>>>>>>>>>>>>>
	if (CheckAttribute(itemsRef, "Treasure"))
	{
		Log_Info(QUEST_UTIL[23]);
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
		//LAi_SetImmortal(sld, false);
		LAi_group_Attack(sld, Pchar);
		chrDisableReloadToLocation = true;
		LAi_LocationFightDisable(locLoad, false); 
		LocatorReloadEnterDisable(locLoad.id, "reload2", true);
		pchar.quest.Husband_death.win_condition.l1 = "NPC_Death";
		pchar.quest.Husband_death.win_condition.l1.character = "Husband";
		pchar.quest.Husband_death.win_condition = "OpenTheDoors";
	}
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
