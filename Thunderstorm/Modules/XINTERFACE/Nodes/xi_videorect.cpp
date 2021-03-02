#include "xi_videorect.h"
#include "../base_video.h"
#include "../xdefines.h"
#include "EntityManager.h"

CXI_VIDEORECT::CXI_VIDEORECT()
{
	m_rs=nullptr;
	m_nNodeType = NODETYPE_VIDEORECT;
}

CXI_VIDEORECT::~CXI_VIDEORECT()
{
	ReleaseAll();
}

void CXI_VIDEORECT::Draw(bool bSelected,uint32_t Delta_Time)
{
	if(m_bUse)
	{
		if( auto ptr = EntityManager::GetEntityPointer(m_eiVideo) )
		{
			IDirect3DTexture9 * pTex = ((xiBaseVideo*)ptr)->GetCurrentVideoTexture();
			if(pTex!= nullptr)
			{
				// Create rectangle
				XI_ONETEX_VERTEX v[4];
				v[0].pos.x = (float)m_rect.left;  v[0].pos.y = (float)m_rect.top;	 v[0].tu=m_rectTex.left; v[0].tv=m_rectTex.bottom;
				v[1].pos.x = (float)m_rect.left;  v[1].pos.y = (float)m_rect.bottom; v[1].tu=m_rectTex.left; v[1].tv=m_rectTex.top;
				v[2].pos.x = (float)m_rect.right; v[2].pos.y = (float)m_rect.top;    v[2].tu=m_rectTex.right; v[2].tv=m_rectTex.bottom;
				v[3].pos.x = (float)m_rect.right, v[3].pos.y = (float)m_rect.bottom; v[3].tu=m_rectTex.right; v[3].tv=m_rectTex.top;
				for(int i=0; i<4; i++)
				{
					v[i].color = m_dwColor;
					v[i].pos.z = 1.f;
				}

				m_rs->SetTexture(0,pTex);
				m_rs->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,XI_ONETEX_FVF,2,v,sizeof(XI_ONETEX_VERTEX),"iVideo");
			}
		}
	}
}

bool CXI_VIDEORECT::Init(INIFILE *ini1,char *name1, INIFILE *ini2,char *name2, VDX9RENDER *rs, XYRECT &hostRect, XYPOINT &ScreenSize)
{
	if( !CINODE::Init(ini1,name1, ini2,name2, rs,hostRect,ScreenSize) ) return false;
	SetGlowCursor(false);
	return true;
}

void CXI_VIDEORECT::LoadIni(INIFILE *ini1,char *name1, INIFILE *ini2,char *name2)
{
	char param[255];

	m_rectTex = GetIniFloatRect(ini1,name1, ini2,name2, "textureRect", FXYRECT(0.f,0.f,1.f,1.f));
	m_dwColor = GetIniARGB(ini1,name1, ini2,name2, "color", ARGB(255,128,128,128));
	if( ReadIniString(ini1,name1, ini2,name2, "videoFile", param, sizeof(param),"") )
		StartVideoPlay(param);
}

void CXI_VIDEORECT::ReleaseAll()
{
	EntityManager::EraseEntity(m_eiVideo);
}

void CXI_VIDEORECT::ChangePosition( XYRECT &rNewPos )
{
	m_rect = rNewPos;
}

void CXI_VIDEORECT::SaveParametersToIni()
{
	char pcWriteParam[2048];

	INIFILE * pIni = fio->OpenIniFile( (char*)ptrOwner->m_sDialogFileName.c_str() );
	if( !pIni ) {
		api->Trace( "Warning! Can`t open ini file name %s", ptrOwner->m_sDialogFileName.c_str() );
		return;
	}

	// save position
	sprintf_s( pcWriteParam, sizeof(pcWriteParam), "%d,%d,%d,%d", m_rect.left, m_rect.top, m_rect.right, m_rect.bottom );
	pIni->WriteString( m_nodeName, "position", pcWriteParam );

	delete pIni;
}

int CXI_VIDEORECT::CommandExecute(int wActCode)
{
	return -1;
}

uint32_t CXI_VIDEORECT::MessageProc(long msgcode, MESSAGE & message)
{
	switch(msgcode)
	{
	case 0: // ��������� �����
		{
			char param[256];
			message.String(sizeof(param),param);
			StartVideoPlay(param);
		}
	break;
	}

	return 0;
}

void CXI_VIDEORECT::StartVideoPlay(char * videoFileName)
{
	if( EntityManager::GetEntityPointer(m_eiVideo) ) {
		EntityManager::EraseEntity(m_eiVideo);
	}
	if(videoFileName== nullptr) return;

#ifndef _XBOX
	m_eiVideo = EntityManager::CreateEntity("CAviPlayer" );
	m_rectTex.bottom = 1.f - m_rectTex.bottom;
	m_rectTex.top = 1.f - m_rectTex.top;
	if(auto ptr = EntityManager::GetEntityPointer(m_eiVideo))
		((xiBaseVideo*)ptr)->SetShowVideo(false);
	api->Send_Message(m_eiVideo, "ls", MSG_SET_VIDEO_PLAY, videoFileName);


#else
	EntityManager::CreateEntity( &m_eiVideo, "WMVideoPlay" );
	api->Send_Message(m_eiVideo, "ls", MSG_SET_VIDEO_PLAY, videoFileName);
	float fScrW = (float)m_screenSize.x;
	float fScrH = (float)m_screenSize.y;
	api->Send_Message(m_eiVideo, "lffffffff", 40123,
		m_rectTex.left, m_rectTex.top, m_rectTex.right, m_rectTex.bottom,
		m_rect.left/fScrW, m_rect.top/fScrH, m_rect.right/fScrW, m_rect.bottom/fScrH);
#endif
}
