 //============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmLocations
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmLocations_H_
#define _WdmLocations_H_

#include "WdmRenderModel.h"

#define WDM_LOCATIONS_MAXLOC	8


class WdmLocations : public WdmRenderObject
{
	struct Location
	{
		//Init
		Location();
		~Location();
		bool SetModel(char * modelName);
		void SetVisible(bool isVis);
		//Work
		bool Update(float dltTime);
		
		//
		WdmRenderModel * model;		//�������� �������
		float ay;
		bool isVisible;
		float alpha;
		bool isDebugMove;
	};

	struct Label
	{
		//Init
		Label();
		~Label();
		void SetText(const char * t);
		void SetIcon(long icon);
		void SetPivot(float px, float py);
		void SetVisible(bool isVis);
		bool IsHave();
		bool IsVisible();
		float Alpha();
		//Work
		bool Update(float dltTime, WdmLocations * loc);
		//������
		char * text;
		//������� ������������ ���� �����
		float alpha;
		//Pivot
		float x, y;
		//�������
		float w, h;
		//������� ������
		float ix, iy;
		//������� ������
		float sx, sy;
		//������
		struct Params
		{
			//����� ������
			long icon;
			//������������ ������
			float alpha;
		} t[2];
		//���������
		bool isVisible, isIsland;
		long font;
	};


	struct IslandInfo
	{
		//
		IslandInfo();
		~IslandInfo();
		void SetName(char * n);
		void Update(float dltTime, WdmLocations * loc);
		//
		float x, y, z;				//�������
		float rx, rz;				//Pivot
		float radius2;				//������
		float kRadius;				//���������� ������� ������������
		char * name;				//��� �������� ��� �������
		bool inside;				//������� ������ ������ ��� ��� ���� �������
		//������� �� �������
		Location locations[WDM_LOCATIONS_MAXLOC];
		bool isNeedUpdate;
		//��������� ����� ����� �������
		Label label[WDM_LOCATIONS_MAXLOC + 1];
		//��������� ����������� ��� �����
		long labelPrt[WDM_LOCATIONS_MAXLOC + 1];
		long numLabels;
	};




//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	WdmLocations();
	virtual ~WdmLocations();

	//�������� ������ �� �������� � ��������
	void SetIslandsData(ATTRIBUTES * apnt);



	//��������� �������� ��������� ���������
	void AttributeChanged(ATTRIBUTES * apnt);

	//�������
	virtual void Update(float dltTime);
	//���������
	virtual void LRender(VDX8RENDER * rs);

	//����� ������� �� �������� ��� �������
	void FindReaction(float x, float z, float & rx, float & rz);

	//����� ��������� ������
	bool FindGoodIsland(float shipX, float shipZ, float & x, float & z, float & r, bool testInZone = false, bool retPivot = false);

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void InitLabel(Label & label, ATTRIBUTES * apnt, bool isIsl, bool isSet = false);
	void UpdateLabelPriority(IslandInfo & ii);
	void Release();
	void DrawLabels(VDX8RENDER * rs);
	void DrawIcon(VDX8RENDER * rs, float x, float y, Label & label);

private:
	IslandInfo * islandsInfo;
	long numIslands;

	float curFrame;

	long curDebugIsland, curDebugLocation;

public:
	long labelFontIslands, labelFontLocations;
	float labelPivotX, labelPivotY;
	float heightViewLabel;
	long iconTexture;
	float iconWidth, iconHeight;
	long iconNum, iconFrames;
	float iconFps;
	char iconTextureName[128];
	//������������ � ����������� �� ������
	static float heightAlpha;
};

#endif

