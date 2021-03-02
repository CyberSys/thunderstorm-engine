#ifndef _VANT_H_
#define _VANT_H_

#include "..\..\common.h"

class NODE;

#define VANT_EDGE 5 // ����� ������ � ������� �������

#define VANTVERTEX_FORMAT		(D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXTUREFORMAT2)

struct VANTVERTEX
{
	CVECTOR pos;
	float tu, tv;
};

class NetVant : public ENTITY
{
    // ��������� ���������� �� INI-����� //
    //-------------------------------------
	int ROPE_QUANT;// ���������� �������
	float ROPE_WIDTH;// ������� �������
	float upWidth;// ������ �������� ������������
	float upHeight;// ������ �������� ������������
	float treangXl;// ���������� �������� ������������
	float treangXr;//
	float treangYu;//
	float treangYd;//
	float balkYu;// ���������� �������� �����
	float balkYd;//
	float ropeXl;// ���������� �������� ������������ �������
	float ropeXr;//
	float vRopeXl;// ���������� �������� �������������� ������� xBeg
	float vRopeXr;//
	float vRopeHeight;// ������ ������������ �������
	float hRopeHeight;// ������ �������������� �������
	float fBalkHeight;// ������ ����� ������������ ������ ������������
	float fBalkWidth;//
	float fVantMaxDist;// ������� ���������� � �������� �� ����� �����
	float ZERO_CMP_VAL;// ��� ������������� �������� �����
	float MAXFALL_CMP_VAL;// ������������ ��������� ��������� ����� ��� ������� ���� ��������� ������������
    //-------------------------------------
    FILETIME ft_old;

    bool bUse;
    bool bRunFirstTime;
    bool bYesDeleted;
    int  wVantLast;
	VDX8RENDER  *RenderService;
    char* TextureName;
    long texl;

public:
	 NetVant();
	~NetVant();
	void SetDevice();
	bool Init();
	void Realize(dword Delta_Time);
	void Execute(dword Delta_Time);
	bool CreateState(ENTITY_STATE_GEN * state_gen);
	bool LoadState(ENTITY_STATE * state);
    dword _cdecl ProcessMessage(MESSAGE & message);

private:
    struct VANTDATA
    {
        bool bDeleted;
        CVECTOR pUp,pLeft,pRight;
        CMatrix *pUpMatWorld,*pDownMatWorld;
        DWORD sv,nv, st,nt;

        int vantNum;
        CVECTOR pos[VANT_EDGE];

        CVECTOR pUpOld,pLeftOld, pUpStart,pLeftStart;
        int HostGroup;
    };
    int vantQuantity;
    VANTDATA **vlist;

    struct GROUPDATA
    {
        bool bDeleted;
        int vantQuantity;
        int *vantIdx;
        long sVert,nVert;
        long sIndx,nIndx;

        CMatrix *pMatWorld;
        ENTITY_ID model_id;
		ENTITY_ID shipEI;
    };
    int groupQuantity;
    GROUPDATA *gdata;

    void SetVertexes();
    void SetIndex();
    void AddLabel(GEOS::LABEL &lbl, NODE* nod);
    void SetAll();
    void SetAdd(int firstNum);
    void LoadIni();
    void doMove();
    bool VectCmp(CVECTOR v1,CVECTOR v2,float minCmpVal);
    void FirstRun();
    void DoDelete();

    VANTVERTEX *vertBuf;

    long vBuf,iBuf;
    DWORD nVert,nIndx;
};



#endif