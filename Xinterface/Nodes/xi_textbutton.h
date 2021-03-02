#ifndef __XI_TEXTBUTTON_H__
#define __XI_TEXTBUTTON_H__

#include "..\\inode.h"

// picture
class CXI_TEXTBUTTON : public CINODE
{
public:
	CXI_TEXTBUTTON();
	~CXI_TEXTBUTTON();

	void	Draw(bool bSelected,dword Delta_Time);
	bool	Init(INIFILE *ini1,char *name1, INIFILE *ini2,char *name2, VDX8RENDER *rs, XYRECT &hostRect, XYPOINT &ScreenSize);
	void	ReleaseAll();
	int		CommandExecute(int wActCode);
	bool	IsClick(int buttonID,long xPos,long yPos);
	void	MouseThis(float fX, float fY) {}
	void	ChangePosition( XYRECT &rNewPos );
	void	SaveParametersToIni();
	dword _cdecl MessageProc(long msgcode, MESSAGE & message);
	void	NotUsingTime(dword Delta_Time) {m_nPressedDelay=0;}
	void	SetUsing(bool bUsing);
	void	MakeLClickPreaction();

protected:
	void	LoadIni(INIFILE *ini1,char *name1, INIFILE *ini2,char *name2);
	void	FillPositionIntoVertices();

protected:
	char *	m_sGroupName;
	long	m_idTex; // texture identity
	long	m_idShadowTex; // shadow texture
	long	m_idUnSelectMiddle; // picture id for middle part of unselect button
	long	m_idSelectMiddle; // picture id for middle part of select button
	long	m_idUnSelectLeft; // picture id for left part of unselect button
	long	m_idSelectLeft; // picture id for left part of select button
	long	m_idUnSelectRight; // picture id for right part of unselect button
	long	m_idSelectRight; // picture id for right part of select button
	bool	m_bCurrentSelected;

    CVideoTexture *m_pTex;
	XI_ONETEX_VERTEX m_v[8];

	DWORD   m_dwShadowColor;	// ���� ����
	DWORD   m_dwFaceColor;		// ���� ��������� �������
	DWORD   m_dwPressedFaceColor; // ���� �������� �������
	DWORD	m_dwFontColor;		// ���� ������
	DWORD	m_dwUnselFontColor;	// ���� ������ ��� ����������� �������
	float	m_fFontScale;

	float   m_fXShadow;			// �������� �� X ��� ���� ���������� �������
	float   m_fYShadow;			// �������� �� Y ��� ���� ���������� �������
	float   m_fXShadowPress;	// �������� �� X ��� ���� ������� �������
	float   m_fYShadowPress;	// �������� �� Y ��� ���� ������� �������
	float   m_fXDeltaPress;		// �������� �� X ��� ������� ������
	float   m_fYDeltaPress;		// �������� �� Y ��� ������� ������

	int		m_nPressedDelay;
	int     m_nMaxDelay;

	// string parameters
	int		m_nFontNum;
	long	m_idString;
	int		m_dwStrOffset;
	char *	m_sString;

	// picture index & vertex buffers
	long	m_idVBuf; // identificator of the vertex buffer
	long	m_idIBuf; // identificator of the index buffer
	long	m_nVert;  // vertex quantity
	long	m_nIndx;  // index quantity

	bool	m_bVideoToBack;
	DWORD	m_dwBackColor;

	float	m_fShadowScale;
};

#endif
