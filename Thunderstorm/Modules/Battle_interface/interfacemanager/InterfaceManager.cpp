#include "InterfaceManager.h"
#include "../image/imgrender.h"
#include "../image/image.h"
#include "../Utils.h"
#include "MousePointer.h"
#include "interfacenode/AllNodes.h"

#include "interfacegroup/BaseGroup.h"
#include "interfacegroup/SeaGroup.h"

#include "../../../Shared/bimanager/messages.h"
#include "storm_assert.h"
#include "../vmodule_api.h"

BI_InterfaceManager::BI_InterfaceManager()
{
	m_pRS = nullptr;
	m_pImgRender = nullptr;
	m_pMouse = nullptr;
	m_pInterfaceSheet = nullptr;
}

BI_InterfaceManager::~BI_InterfaceManager()
{
	STORM_DELETE( m_pInterfaceSheet );
	//m_aNodes.DelAllWithPointers();
	for (const auto &node : m_aNodes)
		delete node;
	STORM_DELETE( m_pMouse );
	STORM_DELETE( m_pImgRender );
}

bool BI_InterfaceManager::Init()
{
	m_pRS = (VDX9RENDER*)api->CreateService("DX9RENDER"); Assert(m_pRS);
	m_pImgRender = new BIImageRender(m_pRS); Assert(m_pImgRender);
	m_pMouse = new MousePointer(this,AttributesPointer); Assert(m_pMouse);

	long nBaseWidth = 800;
	long nBaseHeight = 600;
	long nBaseXOffset = 0;
	long nBaseYOffset = 0;
	if( AttributesPointer ) {
		ATTRIBUTES* pA = AttributesPointer->GetAttributeClass("BaseWindow");
		if( pA ) {
			nBaseWidth = pA->GetAttributeAsDword("width",nBaseWidth);
			nBaseHeight = pA->GetAttributeAsDword("height",nBaseHeight);
			nBaseXOffset = pA->GetAttributeAsDword("xoffset",nBaseXOffset);
			nBaseYOffset = pA->GetAttributeAsDword("yoffset",nBaseYOffset);
		}
	}
	m_pImgRender->SetBaseScreenSize( nBaseWidth,nBaseHeight, nBaseXOffset,nBaseYOffset );

	return true;
}

void BI_InterfaceManager::Execute(uint32_t delta_time)
{
}

void BI_InterfaceManager::Realize(uint32_t delta_time)
{
	if( m_pInterfaceSheet )
		m_pInterfaceSheet->Update();

	for( long n=0; n<m_aNodes.size(); n++ )
		m_aNodes[n]->Update();

	m_pMouse->Update();

	m_pImgRender->Render();
}

uint32_t BI_InterfaceManager::ProcessMessage(MESSAGE & message)
{
	switch( message.Long() )
	{
	case MSG_BIMANAGER_DELETE_SHEET: STORM_DELETE(m_pInterfaceSheet); break;

	case MSG_BIMANAGER_LOAD_SHEET: return MsgLoadSheet( message ); break;

	case MSG_BIMANAGER_EVENT: return MsgEvent( message ); break;

	case MSG_BIMANAGER_CREATE_IMAGE: return MsgCreateImage( message ); break;

	case MSG_BIMANAGER_DELETE_IMAGE: return MsgDeleteNode( message ); break;

	case MSG_BIMANAGER_CREATE_STRING: return MsgCreateString( message ); break;

	case MSG_BIMANAGER_DELETE_STRING: return MsgDeleteNode( message ); break;
	}
	return 0;
}

BI_ManagerNodeBase* BI_InterfaceManager::CreateImageNode(const char* texture, const FRECT& uv, const RECT& pos, uint32_t color, long nPrioritet)
{
	BI_ManagerNodeBase* pNod = new BI_ImageNode(this, texture, uv, pos, color, nPrioritet);
	return pNod;
}

BI_ManagerNodeBase* BI_InterfaceManager::CreateStringNode(const char* text, const char* font, uint32_t color, float scale, const RECT& pos, long nHAlign, long nVAlign, long prioritet)
{
	BI_ManagerNodeBase* pNod = new BI_StringNode(this, text, font, color, scale, pos, nHAlign, nVAlign, prioritet);
	return pNod;
}

void BI_InterfaceManager::DeleteNode(BI_ManagerNodeBase* pNod)
{
	const auto it = std::find(m_aNodes.begin(), m_aNodes.end(), pNod);
	if (it != m_aNodes.end())
		m_aNodes.erase(it);

	//long n = m_aNodes.Find( pNod );
	//if( n<0 ) return;
	//m_aNodes.DelIndex( n );
}

long BI_InterfaceManager::MsgLoadSheet(MESSAGE & message)
{
	// ������� ������ ���������
	STORM_DELETE( m_pInterfaceSheet );

	char param[512];
	message.String( sizeof(param), param );
	if( _stricmp(param,"sea")==0 )
	{ // ������ ������� ���������
		m_pInterfaceSheet = new BI_SeaGroup(this);
		if( m_pInterfaceSheet ) {
			m_pInterfaceSheet->Init();
		}
	}else
	if( _stricmp(param,"land")==0 )
	{ // ������ ������ ���������
	}
	return 0;
}

long BI_InterfaceManager::MsgCreateImage(MESSAGE & message)
{
	/*char texture[MAX_PATH];	message.String( sizeof(texture), texture );
	FRECT uv;
	RECT pos;
	uint32_t color;
	long nPrioritet;

	return (long)CreateImageNode(texture,uv,pos,color,nPrioritet);*/
	return 0;
}

long BI_InterfaceManager::MsgCreateString(MESSAGE & message)
{
	//return (long)CreateStringNode();
	return 0;
}

long BI_InterfaceManager::MsgDeleteNode(MESSAGE & message)
{
	BI_ManagerNodeBase* pNod = (BI_ManagerNodeBase*)message.Long();
	if( !pNod ) 
		return 0;

	//if( m_aNodes.Find(pNod) != INVALID_ARRAY_INDEX ) {
	//	STORM_DELETE(pNod);
	//}
	//~!~ DeleteNode?
	const auto it = std::find(m_aNodes.begin(), m_aNodes.end(), pNod);
	if (it != m_aNodes.end())
		STORM_DELETE(*it);

	return 0;
}

long BI_InterfaceManager::MsgEvent(MESSAGE & message)
{
	return 0;
}
