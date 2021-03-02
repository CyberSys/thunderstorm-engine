//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	Supervisor
//--------------------------------------------------------------------------------------------
//	������ �������������� ����������� ����������
//============================================================================================

#ifndef _Supervisor_H_
#define _Supervisor_H_

class Character;
class LocatorArray;
struct CVECTOR;

#define MAX_CHARACTERS	64

class Supervisor
{
	friend Character;
public:
	struct CharacterInfo
	{
		Character * c;		//�������� � ������� ���������������
		float d;			//���������� �� ���������
		float maxD;			//���������� ������������� ���������
	};

	struct FindCharacter
	{
		Character * c;		//��������, �������� ������
		float dx, dy, dz;	//������ �� ��������� �� ���
		float d2;			//������� ��������� �� ��������� � xz
		float l, r, n;		//��������� �� ���������� ��������� (�����������)
		float sectDist;		//��������� �� �������
	};

	struct CharacterEx
	{
		Character * c;
		float lastTime;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Supervisor();
	virtual ~Supervisor();

	//��������� �� ����������� �������
	bool CheckPosition(float x, float y, float z, Character * c);
	//����� �� ������� ����������
	bool FindCharacters(FindCharacter fndCharacter[MAX_CHARACTERS], long & numFndCharacters, Character * chr, float radius, float angTest = 0.0f, float nearPlane = 0.4f, float ax = 0.0f, bool isSort = false, bool lookCenter = false);

	void Update(float dltTime);
	void PreUpdate(float dltTime);
	void PostUpdate(float dltTime);
	

	//���������� ������� ��� ��������
	void SetSavePositions();
	//������� ������� ��� ��������
	void DelSavePositions(bool isTeleport);

	//����� ����������� ������� ��� ����������� �������� ���������
	long FindForvardLocator(LocatorArray * la, const CVECTOR & pos, const CVECTOR & norm, bool lookChr = false);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	//�������� ��������� � �������
	void AddCharacter(Character * ch);
	//������� ��������� �� �������
	void DelCharacter(Character * ch);

	float time, waveTime;
	long curUpdate;

public:
	CharacterEx character[MAX_CHARACTERS];
	long numCharacters;
	CharacterInfo colchr[MAX_CHARACTERS*MAX_CHARACTERS];
	long numColChr;
	bool isDelete;
	Character * player;
};

#endif

