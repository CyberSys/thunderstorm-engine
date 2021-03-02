#ifndef _SCRSHOTER_H_
#define _SCRSHOTER_H_

#include "..\\..\\common_h\\dx9render.h"

class SCRSHOTER : public ENTITY
{
	VDX9RENDER  *rs;

	struct SAVETEXTURES
	{
		char * fileName;
		char * dataString;
		IDirect3DTexture9 * m_pTex;
		SAVETEXTURES * next;
	};

public:
	 SCRSHOTER();
	~SCRSHOTER();
	void SetDevice();
	bool Init();
    void Execute(dword Delta_Time);
	void Realize(dword Delta_Time);
    dword _cdecl ProcessMessage(MESSAGE & message);

private:
	bool MakeScreenShot();
	IDirect3DTexture9 * FindSaveTexture(char * fileName);
	char * FindSaveData(char * fileName);
	IDirect3DTexture9 * AddSaveTexture(char * dirName, char * fileName);
	void DelSaveTexture(char * fileName);
	IDirect3DTexture9 * GetTexFromSave(char * fileName, char **pDatStr);

	void LostRender() override;
	void RestoreRender() override;

	IDirect3DTexture9 * m_pScrShotTex;
	SAVETEXTURES * m_list;
};

#endif
