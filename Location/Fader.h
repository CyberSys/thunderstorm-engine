//============================================================================================
//	Spirenkov Maxim, 2003
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Fader
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _Fader_H_
#define _Fader_H_

#include "..\common_h\vmodule_api.h"
#include "..\common_h\dx8render.h"

class Fader : public ENTITY  
{
//--------------------------------------------------------------------------------------------
//���������������, ������������������
//--------------------------------------------------------------------------------------------
public:
	Fader();
	virtual ~Fader();

	//�������������
	bool Init();
	//���������
	dword _cdecl ProcessMessage(MESSAGE & message);
	//������
	void Execute(dword delta_time);
	void Realize(dword delta_time);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX8RENDER * rs;
	IDirect3DSurface8 * renderTarget;
	IDirect3DSurface8 * surface;

	bool isWork;
	bool haveFrame;
	bool fadeIn;
	bool isStart;
	bool isAutodelete;
	bool endFade;
	float fadeSpeed;
	float alpha;
	float w, h;

	bool eventStart;
	bool eventEnd;
	long deleteMe;

	long textureID;
	long tipsID;

public:
	static long numberOfTips;
	static long currentTips;
};

#endif

