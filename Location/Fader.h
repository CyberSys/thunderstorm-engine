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

#include "../Common_h/vmodule_api.h"
#include "../Common_h/dx9render.h"

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
	uint32_t ProcessMessage(MESSAGE & message);
	//������
	void Execute(uint32_t delta_time);
	void Realize(uint32_t delta_time);


//--------------------------------------------------------------------------------------------
//������������
//--------------------------------------------------------------------------------------------
private:
	VDX9RENDER * rs;
	IDirect3DSurface9 * renderTarget;
	IDirect3DSurface9 * surface;

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

