//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmIslands
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmIslands_H_
#define _WdmIslands_H_

#include "WdmRenderModel.h"
#include "..\common_h\geometry.h"
#include "PtcData.h"

class WdmIslandWaves : public WdmRenderModel
{
public:
	WdmIslandWaves();

	virtual void Update(float dltTime);
	virtual void LRender(VDX8RENDER * rs);
	void Render(VDX8RENDER * rs, float k);

private:
	float phase;
};


class WdmIslands : public WdmRenderObject
{
	struct Islands
	{
		WdmRenderModel * model;	//�������� �������
		WdmRenderModel * area;	//�������� ������� �������
		WdmRenderModel * palms;	//�������� � ��������
		WdmIslandWaves * waves;	//�������� � �����
		CMatrix toLocal;		//�������������� � ��������� ������� �������
		string modelName;		//��� �������� �������
		CVECTOR worldPosition;	//������� ������� � ����
	};

	struct Label
	{
		string text;			//����� �����
		CVECTOR pos;			//������� �����
		float l, t, r, b;		//�������������, ����������� ����� � �������� �����������
		float dl, dt, dr, db;	//��������, ����� �������� �������������, ��� ��������� ����� �� ������
		float textX, textY;		//������������� ������� ������
		float iconX, iconY;		//������������� ������� ��������
		float alpha;			//������� ��������� ��������������
		float heightView;		//������ ������� � ������� ����� ������
		long font;				//������ ������ � ������� �������
		long icon;				//������ ��������
		dword weight;			//��� ��������
		string id;				//������������� �����
		dword idHash;			//���� �������� ��������������
		long next;				//��������� � ������ �����
		string locatorName;		//��� �������� �� ������� �����������
	};

	struct Font
	{
		string name;			//��� ������
		long id;				//��� �������������
	};

	struct Icons
	{
		float w, h;
		float u, v;
		dword num;
		dword frames;
		float fps;
		float frame;
		float f[2];
		long blend;
		long texture;
	};

	struct Quest
	{
		CVECTOR pos;
		string name;
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmIslands();
	virtual ~WdmIslands();

	//��������� �� ��������� ������������
	bool CollisionTest(CMatrix & objMtx, float length, float width, bool heighTest = true);
	//��������� ������� � ������ ����� �������������
	bool ObstacleTest(float x, float z, float radius);

	//�������� ������ �� ��������
	void SetIslandsData(ATTRIBUTES * apnt, bool isChange);

	//����� ����������� ��� �������� � �������� ����� ���������� �� �������
	void FindDirection(const CVECTOR & position, const CVECTOR & destination, CVECTOR & direction);
	//����� ���� ������������
	void FindReaction(const CVECTOR & position, CVECTOR & reaction);
	//����� ��������� ����� ��� ��������
	bool GetRandomMerchantPoint(CVECTOR & p);
	//�������� ���������� ���������� ��������
	bool GetQuestLocator(const char * locName, CVECTOR & p);

	//��������� ��������� ��������� � ���� �������
	bool CheckIslandArea(const char * islandName, float x, float z);
	//�������� ��������� ����� � ���� �������
	void GetNearPointToArea(const char * islandName, float & x, float & z);

	
	virtual void Update(float dltTime);
	virtual void LRender(VDX8RENDER * rs);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	bool IsShipInArea(long islIndex, CVECTOR & pos);
	static bool _cdecl AddEdges(const GEOS::VERTEX * vrt, long numVrt);
	static bool _cdecl FindNearPoint(const GEOS::VERTEX * vrt, long numVrt);
	void LabelsReadIconParams(ATTRIBUTES * apnt);
	long LabelsFind(const char * id, dword hash);
	bool LabelsFindLocator(const char * name, CVECTOR & pos);
	long LabelsAddFont(const char * name);
	void LabelsRelease();
	static CVECTOR & Norm2D(CVECTOR & v);

private:
	//������, ���������� ��� ��������
	WdmRenderModel * baseModel;
	//���� ��� ������ ����
	PtcData * patch;
	//������ ��������
	array<Islands> islands;
	//�����
	array<Label> labels;
	//������ ������������ �������
	array<Font> fonts;
	//��������
	Icons icons;
	//������������������ �� ��������� �����
	array<long> labelSort;
	//����� ����� ���������� ���������
	array<CVECTOR> merchants;
	//����� ����� ���������� ��������� �����������
	array<Quest> quests;

	//������� ������� ��� ������ �����
	long labelsEntry[1024];

	static CMatrix curMatrix, locMatrix;
	static long numEdges;
	static CVECTOR curPos;
	static bool checkMode;
public:
	static CVECTOR centPos;
	

};

inline CVECTOR & WdmIslands::Norm2D(CVECTOR & v)
{
	v.y = 0.0f;
	double len = v.x*v.x + v.z*v.z;
	if(len >= 1e-30f)
	{
		len = 1.0f/sqrt(len);
		v.x = float(len*v.x);
		v.z = float(len*v.z);
	}else{
		v = 0.0f;
	}		
	return v;
}



#endif

