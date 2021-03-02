//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Window
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Window_H_
#define _Window_H_

#include "LTypes.h"

class Lights;

class Window  
{
	struct Vertex
	{
		float x, y;
		float z, rhw;
	};

	struct ListElement
	{
		enum Type
		{
			t_quick,
			t_trace,
			t_smooth,
			t_blur,
			t_save,
			t_load,
			t_savelight,
			t_amb,
			t_light,
			t_glight,
		};


		char * name;		//���
		Type type;			//���
		CVECTOR c;			//������� ��������� ����
		float st;			//������������ �����
		//������ ��� �����������
		CVECTOR * color;	//����
		float * cosine;		//���������� ��������
		float * shadow;		//���������� ����
		float * bright;		//������� ����
		float * contr;		//������������� ����
		float * gamma;		//����� ����
		float * att0;		//��������� D^0
		float * att1;		//��������� D^1
		float * att2;		//��������� D^2
		float * range;		//������ ��������
		bool * isOn;		//�������
		long litIndex;		//������ ���������
		float h;			//�������������� ������ ����
	};

//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Window();
	virtual ~Window();

	bool Init(VDX8RENDER * rs);
	void InitList(Lights & ls);
	void Draw(float dltTime);
	void Reset(bool isVis);
	


	//������ ����������� ����������
	bool isNeedInit;
	bool isNoPrepared;	
	bool isVisible;
	bool isTraceShadows;
	float tracePrc;
	bool isSmoothShadows;
	float smoothPrc;
	float smoothRad;
	bool smoothNorm;
	bool isBlurLight;
	bool isResetBlurLight;
	bool isTraceBlur;
	float blurPrc;
	float blurRad;
	float blurAtt;
	float blurCos;
	float kBlur;
	bool isLockCtrl;
	bool isUpdateLight;
	long updateLight;
	bool updateLightCos, updateLightAtt, updateLightSdw;
	bool isSaveLight;
	float isSuccessful;
	float isFailed;
	bool isFailedInit;
	bool isSmallSlider;

//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	void DrawRect(float x1, float y1, float x2, float y2, dword color);
	void DrawLine(float x1, float y1, float x2, float y2, dword color);
	void DrawLRect(float x1, float y1, float x2, float y2, dword bkgColor, dword lnColor);
	void Print(long color, float xleft, float xright, float y, float scale, bool isAlign, const char * format, ...);
	void DrawCursor();

	bool Slider(long id, float y, const char * text, float & value, float min = 0.0f, float max = 1.0f, bool drawVal = false);
	bool ColorPicker(long id, float y, CVECTOR & ref, float st, CVECTOR & res);
	bool Button(float x, float y, float w, float h, const char * text, long * act = null, long init = -1);
	void Checker(float x, float y, const char * text, bool & res);
	long SelPreset();

	void SavePreset(long prs);
	void LoadPreset(long prs);

	char * GenerateName(const char * f, const char * n);

	void UpdateColors();
	void UpdateLight(long idxLight, bool isCos, bool isAtt, bool isSdw);

private:
	//
	VDX8RENDER * rs;
	//������ �������
	float sw, sh;
	//��������� ����
	float winx, winy, winw, winh;
	dword bkgColor, frmColor, textColor, selColor;
	float sldLen;
	//Font
	long font;
	float fontHeight;
	//Mouse
	bool isMouseDown;
	bool isActiveMouseState;
	bool isOldMouseState;
	float cursx, cursy;
	//List
	bool isList;
	ListElement * list;
	long numElements;
	long selected;
	float listPos;
	float listWait;
	//Slider
	long slidID;
	float slidDltX;
	//Picker
	bool isPikerActive;
	float pikerWait;
	IDirect3DTexture9 * pickerTexture;
	//Presets
	long lastPreset;
	char prsComment[64];
	//������ ��� �����
	char ver[256];
	char stringBuffer[1024];

};

#endif

