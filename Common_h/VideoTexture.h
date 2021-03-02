#ifndef _VIDEOTEXTURE_H
#define _VIDEOTEXTURE_H

//-----------------------------------------------------------------------------
// Name: class VideoToTexture
// Desc: play video into texture
//-----------------------------------------------------------------------------
class CVideoTexture : public ENTITY
{
public:
    virtual IDirect3DTexture9* Initialize(VDX9RENDER *pRS,const char* sFileName, bool bCicled) = 0;
    virtual bool FrameUpdate() = 0;
	virtual void Release() = 0;
	IDirect3DTexture9 *m_pTexture;
	operator IDirect3DTexture9* () {return m_pTexture;}
	operator IDirect3DBaseTexture9* () {return m_pTexture;}
};

#endif