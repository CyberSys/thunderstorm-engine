#ifndef _BI_MESSAGE_ICONS_H_
#define _BI_MESSAGE_ICONS_H_

#include "defines.h"

#define MESSAGE_ICONS_COLUMN_QUANTITY	4

class MESSAGE_ICONS
{
	VDX8RENDER *rs;
	ENTITY_ID  m_idHost;

public:
	MESSAGE_ICONS();
	~MESSAGE_ICONS();

	void Update(DWORD deltaTime);
	void Draw();
	void SetShowMsg(bool bShow) {m_bShowMsgIcon = m_vMsgIconBufID>=0 ? bShow : false;}
	void StartData(ATTRIBUTES * pAData[MESSAGE_ICONS_COLUMN_QUANTITY], long pLeft[MESSAGE_ICONS_COLUMN_QUANTITY]);
	bool InitData(ENTITY_ID & host_eid, VDX8RENDER * _rs, ATTRIBUTES * pARoot);

protected:
	// ������ ���������
	bool	m_bShowMsgIcon;
	long	m_vMsgIconBufID;
	long	m_iMsgIconBufID;
	long	m_nMsgIconWidth;
	long	m_nMsgIconHeight;
	long	m_nMsgIconDist;		// ��������� ����� ��������
	long	m_nBottomY;			// ���������� ������� ���� ������
	long	m_nMsgIconQnt;
	long	m_nMsgIconRowQnt;

	// ��������
	long	m_idMsgIconsTexture;
	long	m_nHorzTextureSize; // ������ � �������
	long	m_nVertTextureSize; // ������ � �������


	ATTRIBUTES * m_pIconsAttr[MESSAGE_ICONS_COLUMN_QUANTITY];

	float	m_fNormalStateTime; // ����� ������������ ������, sec
	float	m_fBlendTime;		// ����� ��������� ������, sec
	float	m_fFallSpeed;		// �������� ������� ������ �� ��������� �����, pix/sec
	float	m_fBlindTimeUp;		// ����� �� ��������� ����� ��� �������, sec
	float	m_fBlindTimeDown;	// ����� �� ��������� ����� ��� �������, sec
	float	m_fCurBlindTime;	// ������� ����� ��� �������
	bool	m_bBlindDirectUp;	// ���� ��������� ����� ��� �������

	DWORD	m_dwHighBlindColor;	// ���� ������� ������������
	DWORD	m_dwLowBlindColor;	// ���� ������� �����������
	DWORD	m_dwNormalColor;	// ���������� ����

	struct COLUMN_DESCRIBER
	{
		COLUMN_DESCRIBER()	{pRow=null;}
		~COLUMN_DESCRIBER()	{if(pRow) delete[] pRow; pRow=null;}

		struct ROW_DESCR
		{
			long pic;
			float bottom;
			float curTime;

			bool bDoBlend;
			bool bDoBlind;
			DWORD color;
		};

		long rowQ;
		long startVertex;
		long leftPos;
		ROW_DESCR * pRow;
	};

	COLUMN_DESCRIBER	m_pMsgColumns[MESSAGE_ICONS_COLUMN_QUANTITY];
};

#endif
