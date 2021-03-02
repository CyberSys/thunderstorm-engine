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
#include "../Common_h/geometry.h"
#include "PtcData.h"

class WdmIslandWaves : public WdmRenderModel
{
public:
	WdmIslandWaves();

	virtual void Update(float dltTime);
	virtual void LRender(VDX9RENDER * rs);
	void Render(VDX9RENDER * rs, float k);

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
		std::string modelName;		//��� �������� �������
		CVECTOR worldPosition;	//������� ������� � ����
	};

	struct Label
	{
		std::string text;			//����� �����
		CVECTOR pos;			//������� �����
		float l, t, r, b;		//�������������, ����������� ����� � �������� �����������
		float dl, dt, dr, db;	//��������, ����� �������� �������������, ��� ��������� ����� �� ������
		float textX, textY;		//������������� ������� ������
		float iconX, iconY;		//������������� ������� ��������
		float alpha;			//������� ��������� ��������������
		float heightView;		//������ ������� � ������� ����� ������
		long font;				//������ ������ � ������� �������
		long icon;				//������ ��������
		uint32_t weight;			//��� ��������
		std::string id;				//������������� �����
		uint32_t idHash;			//���� �������� ��������������
		long next;				//��������� � ������ �����
		std::string locatorName;		//��� �������� �� ������� �����������
	};

	struct Font
	{
		std::string name;			//��� ������
		long id;				//��� �������������
	};

	struct Icons
	{
		float w, h;
		float u, v;
		uint32_t num;
		uint32_t frames;
		float fps;
		float frame;
		float f[2];
		long blend;
		long texture;
	};

	struct Quest
	{
		CVECTOR pos;
		std::string name;
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
	virtual void LRender(VDX9RENDER * rs);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	bool IsShipInArea(long islIndex, CVECTOR & pos);
	static bool AddEdges(const GEOS::VERTEX * vrt, long numVrt);
	static bool FindNearPoint(const GEOS::VERTEX * vrt, long numVrt);
	void LabelsReadIconParams(ATTRIBUTES * apnt);
	long LabelsFind(const char * id, uint32_t hash);
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
	std::vector<Islands> islands;
	//�����
	std::vector<Label> labels;
	//������ ������������ �������
	std::vector<Font> fonts;
	//��������
	Icons icons;
	//������������������ �� ��������� �����
	std::vector<long> labelSort;
	//����� ����� ���������� ���������
	std::vector<CVECTOR> merchants;
	//����� ����� ���������� ��������� �����������
	std::vector<Quest> quests;

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

