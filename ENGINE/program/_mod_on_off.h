// BOAL 07.09.03  Mods on_off mode
// ����� � ����� ��� boal - ��� �������������� ��� ALexusB

#define VERSION_NUMBER1       "�������: ����������� �������  "
#define VERSION_NUMBER2       "ver. 1.1.8.0608"
#define VERSION_NUMBER3       "Seaward.Ru" // ALexusB&Co

#define VERSION_NUM      80608  // ������� �����
#define VERSION_NUM_PRE  80608  // ��������� ����������

//����: http://seadogs.4fans.org
#define VERSION_WWW     "http://www.seaward.ru"

// Show special message for test game statment
#define MOD_BETTATESTMODE			"On"  // On -DEV version; Test - TST ver; Off - relize

// ���������� ��������� ����������
string sGlobalTemp;

// quest.c bool    bQuestCheckProcessFreeze = false; // ��������� �������

// ������ ���� ������� �� ���� (������� � ����� ����)
bool    bDisableLandEncounters = false;
// ������ ���� �2
bool    bDisableCharacterMenu  = false;
// ���� - ����� �����
bool    bPlayerIsCheater =  false;  // �� �����������, ������
// ����� ����� �����
bool    bBettaTestMode = false;
// ���������� quest_reaction Log
bool    bQuestLogShow  = false;
// ���������� ���� �� �������
bool    bExpLogShow    = false;
// ������ ������� �������� - �������
bool    TestRansackCaptain = false;
// ����� ���������
bool    bLocatorShow  = false;
// �� �����, ��� ����
bool    bNoEatNoRats  = false;
// ����� ��� �� �����: ����� �� ���������� �� �������, ����� �� ��������� ����� � ���, ������� �� ����� ����� ����, ����� ������ ����� ���� (���� � ��������)
bool    bWorldAlivePause      = true;
// ����� �����
bool bPauseContrabandMetro = false;
// ��� ��� ���
bool bPGGLogShow = false;
// ���� ��������� ��������
bool bMonstersGen = false;
// ���� ��������� ���������� �����������
bool bLandEncountersGen = true;
// ���� "� �������"
bool bMainCharacterInBox = false;

// ��������� �������� - ������������
bool    bNewFantomGenerator       = true;  // ������ "����� ����"
bool    bRechargePistolOnLine  = true;  // ��������� �������������� ��� �����
// ��������� ������������
bool    bNewCodeOn                = false;  // ��������� ��������
// ��� ������, ���� � ������ ����
bool    bHardcoreGame             = false;
// ������� ����-����
//bool    bPayForSaveLoad           = true;

///////////////////////////////////////////////////
//          complexity
//  Next Level rate
int MOD_EXP_RATE           = 10; // ��� ����... + (���������)
//int Level_Complexity       = 5; // by default ������ ���������
//  complexity of fantom skill
int MOD_SKILL_ENEMY_RATE   = 5; // 1- easy; 2; 3; 4;  5-medium; 6; 7; 8; 9;  10 - hard (please, do not set more 10 or less 1 it mays crash game


// Killed body clear
#define MOD_DEAD_CLEAR           "On"
#define MOD_DEAD_CLEAR_TIME      100 // in sec.

#define CHAR_ITEMS_WEIGHT        70

#define InstantRepairRATE        65.0

// ��������� �� ������� ��� ���������
#define DistanceToShipTalk        300.0

//#define ShipyardMoneyInit        190000.0
//#define ShipyardMoneyMax         900000.0
//#define ShipyardMoneyAdd         2000

// boal 23.01.2004 ��������� 091004 ����� ������ ������ -->
int    startHeroType = 1;
//int    heroFaceId    = 1;
// boal 23.01.2004 <--

int ItemDeadStartCount = 0; // ����������� � itemInit ����� ��� ��������� ������ (�����������)

#define STARTGAME_YEAR       1665
#define STARTGAME_MONTH      1
#define STARTGAME_DAY        1

// �������� ����������
#define FUNY_SHIP_NAME    "����������"

int TimeScaleCounter = 0;
// ������ ��������
bool chrDisableReloadToLocation = false;
// ��� ������� ����� � �� �����
bool bIsFortAtIsland = false;
float fFort_x, fFort_z;
int iFortNation, iFortCommander;
//������ � ����� ��������� ��� ��������� item
int ItemsForLocators_start, ItemsForLocators_end;

int iScriptVersion = 74632; //������ ��������� ��� ����������, ������ ������ #define ENGINE_SCRIPT_VERSION � core.h
//==> �������� ��������� ������ �� ������� ������. ���� �� ����, �� ��������� �������������� ������ �
//==> �������� ������� ��� ����, ���� ��� - �����. 74632 - ������� �����.
GetEngineVersion();
/////////////////////////////