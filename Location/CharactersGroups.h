//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	CharactersGroups
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _CharactersGroups_H_
#define _CharactersGroups_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\matrix.h"

#include "LocatorArray.h"
#include "Location.h"
#include "Character.h"

class Location;
class Character;

class CharactersGroups : public ENTITY
{
public:
	struct String
	{
		String();
		String(const char * str);
		~String();
		char * name;
		long len;
		long max;
		long hash;

		void operator = (const char * str);
		inline operator const char * (){ return name; };
		bool Cmp(const char * str, long l, long h);
		static long GetHash(const char * str);
		static long GetLen(const char * str);
	};

	enum RelState
	{
		rs_beginvalue,
		rs_friend,
		rs_neitral,
		rs_enemy,
		rs_endvalue,
	};

	struct Relation
	{
		float alarm;		//������� ��������� �������
		float alarmdown;	//�������� �������� �������
		float alarmmin;		//����� �����������
		float alarmmax;		//����� ���������
		bool isActive;		//������� �� �������
		RelState curState;	//������� ��������� ����� ��������
		RelState actState;	//��������� ����� ��������, ������� ����� ����������� � ������ ��������� �������
		RelState relState;	//��������� ����� ��������, ������� ����� ����������� � ������ ����������� �������
	};

	struct Group
	{
		long index;						//������ ������
		String name;					//��� ������
		float look;						//������ ��������� �����������
		float hear;						//������ �� ������� �������� ��������� ���������� ����������
		float say;						//������ �� ������� �������� ����� �������� �������� �� ���������
		long priority;					//��������� ������
		Relation * relations;			//������ ��������� - ������ ������������� ������� ������ � ������
		ENTITY_ID c[MAX_CHARACTERS];	//������ ���������� ����������� � ������
		long numChr;					//���������� ���������� � ������
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	CharactersGroups();
	virtual ~CharactersGroups();

	//�������������
	bool Init();
	//����������
	void Execute(dword delta_time);
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);
	//��������� ��������
	dword AttributeChanged(ATTRIBUTES * apnt);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//�������� ����������� ���������� ���������
	void CharacterVisibleCheck(Character * chr);
	//��������� ��������� ���������� �� ������
	void FindEnemyFromFindList(Character * chr, Group * grp, long num, bool visCheck);
	//�������� ��� �������� �����
	bool AddEnemyTarget(Character * chr, Character * enemy, float maxtime = -1.0);
	//������� ��� ���������� ��� ������������ ����
	void RemoveAllInvalidTargets();
	//������� ���������� ��� ������������ ����
	bool RemoveInvalidTargets(Character * chr, Character * check = null);

private:
	//��������� �� ���������������� ����
	bool MsgIsValidateTarget(MESSAGE & message);
	//����� ����������� ����
	bool MsgGetOptimalTarget(MESSAGE & message);
	//���� �� ������ ��������
	bool MsgIsEnemy(MESSAGE & message);
	//������� ����� �� �����
	void MsgAttack(MESSAGE & message);
	//�������� ����
	void MsgAddTarget(MESSAGE & message);
	//�������� ���� � ������� ���������
	void MsgUpdChrTrg(MESSAGE & message);

	//���������������� ������
	void MsgRegistryGroup(MESSAGE & message);
	//������� ������
	void MsgReleaseGroup(MESSAGE & message);
	//���������������� ������
	long RegistryGroup(const char * groupName);
	//������� ������
	void ReleaseGroup(const char * groupName);

	//���������� ��� ������ ������ ���������
	bool MsgSetGroupLook(MESSAGE & message);
	//���������� ��� ������ ������ ����������
	bool MsgSetGroupHear(MESSAGE & message);
	//���������� ��� ������ ������ ���������
	bool MsgSetGroupSay(MESSAGE & message);
	//���������� ��� ������ ���������
	bool MsgSetGroupPriority(MESSAGE & message);
	//���������� �������� ������� �������
	bool MsgSetAlarm(MESSAGE & message);
	//���������� �������� �������� �������
	bool MsgSetAlarmDown(MESSAGE & message);
	//�������� � ������ ���������
	bool MoveCharacterToGroup(MESSAGE & message);
	//���������� ��������� ����� ��������
	void MsgSetRelation(MESSAGE & message);
	//���������� ������� �� ������� ��� ���� �����
	void MsgSetAlarmReaction(MESSAGE & message);



	//�������� ���������
	void UnloadCharacter(MESSAGE & message);

	//��������� ��������� �� ���� �����
	void RemoveCharacterFromAllGroups(ENTITY_ID * chr);
public:
	//�������� ������ �� ���������
	Group * GetGroup(MESSAGE & message, bool isRegistry = true);
	//����� ������ �� �����
	Group * FindGroup(const char * name);
	//����� ������ �� �����
	long FindGroupIndex(const char * name);
	//����� ��������� �����
	Relation & FindRelation(MESSAGE & message, bool * selfgroup = null);
	//����� ��������� �����
	Relation & FindRelation(const char * name1, const char * name2, bool * selfgroup = null);
	//����� ��������� �����
	Relation & FindRelation(long g1, long g2, bool * selfgroup = null);
	//�������� ������ ������ ���������
	long GetCharacterGroup(Character * c);

	//������� ��� ����
	void ClearAllTargets();
	//��������� ����� � ������
	void SaveData();
	//��������� ����� ��������� �� �������
	void LoadDataRelations();
	//���������� ��������� ��� �������� �����
	void RestoreStates();

	//������� ���������� �� ����������
	void DumpRelations();
	//�������� ��������� � ���� ������
	const char * GetTextState(RelState state);

private:
	Group ** groups;		//������
	long numGroups;			//���������� �����
	long maxGroups;			//���������� �����
	Location * location;	//������� �������
	long curExecuteChr;		//������ �������� ������������ ���������
	float waveTime;			//����� � �������� ������� �����
	
	//������ ��� ������ ����������
	Supervisor::FindCharacter fnd[MAX_CHARACTERS];
};

#endif


