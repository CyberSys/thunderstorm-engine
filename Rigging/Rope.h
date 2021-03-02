#ifndef _ROPE_H_
#define _ROPE_H_

#include "..\common_h\dx8render.h"
#include "..\common_h\geos.h"
#include "..\common_h\matrix.h"
#include "..\common_h\sail_base.h"

class NODE;

#define ROPEVERTEX_FORMAT		(D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXTUREFORMAT2)

struct ROPEVERTEX
{
	CVECTOR pos;
	float tu, tv;
};

#define ROPE_EDGE 5 // ����� ������ � ������� �������

class ROPE : public ROPE_BASE
{
    // ��������� ���������� �� INI-����� //
    //-------------------------------------
float ROPE_SEG_LENGTH; // ����� ������ �������� �������
float ROPE_WIDTH; // ������� �������
float ROPE_END_LENGTH; // ����� ������� (������������) � ����� ���������� � ������� ������������ ����� ������
float ROPE_WAVE; // ��������� ��������� ������� � ���������� ���������
float MaxCh; // ������������ ��������� ���������� ��� ������� ���������� ���������� ���� �������
float ROPE_TEX_LEN; // ����� ������� �� ������� ������� ��������
float fMaxRopeDist; // ������������ ��������� � ��������� ����� �������
float DEEP_CHANGE_SPEED; // �������� ��������� ������� ������� �������
float ROTATE_SPEED; // �������� ������� �������
float MIN_DEEP_MUL; // ����������� �������� ������������ ������������� ������� �������
float VAR_DEEP_MUL; // ������ ��������� ������������ ������������� ������� �������
float VAR_ROTATE_ANGL; // ��������� ���� ������� �������
    //-------------------------------------

	D3DMATERIAL9 mat;

    bool bUse;
    bool bFirstRun;
    bool bYesDeleted;
    int  wRopeLast;

    char* TextureName;
    long texl;

	VDX8RENDER  *RenderService;
public:
	 ROPE();
	~ROPE();
    // Entity Function
	void SetDevice();
	bool Init();
	void Realize(dword Delta_Time);
	void Execute(dword Delta_Time);
	bool CreateState(ENTITY_STATE_GEN * state_gen);
	bool LoadState(ENTITY_STATE * state);
    dword _cdecl ProcessMessage(MESSAGE & message);
    // Service Function
    void GetEndPoint(CVECTOR* cv,int ropenum,ENTITY_ID &mdl_id);
    bool IsAbsentRope(ENTITY_ID &mdl_id, int ropenum);
    void DoDeleteUntie(ENTITY_ID &mdl_id, NODE *rnod, int gNum);

private:
    ROPEVERTEX* vertBuf;
    // ������ �������
    struct ROPEDATA
    {
        bool bUse;
        bool bDeleted;
        int HostGroup;

        CVECTOR pBeg,pEnd,pOld;

        float len;
        CVECTOR vDeep;
        float angDeep,angRot;

        bool btie,etie;
        int bgnum,egnum;
        CMatrix *bMatWorld,*eMatWorld;

        DWORD sv,nv, st,nt;

        int ropeNum;
        CVECTOR pos[ROPE_EDGE];
        CVECTOR cv;
        WORD segnum;
        WORD segquant;

        float ropeWave; // ��������� ��������� �������
		bool bMakeWave;
    };
    int ropeQuantity;
    ROPEDATA** rlist;
    // ������ ����� (��������) �������
    struct GROUPDATA
    {
        bool        bDeleted;
        int         ropeQuantity;
        int         *ropeIdx;
        ENTITY_ID   shipEI;
        ENTITY_ID   modelEI;
        CMatrix     *pMatWorld;
        long        sv,nv;
        long        st,nt;
    };
    int groupQuantity;
    GROUPDATA* gdata;

    void SetVertexes();
    void SetVertexes(ROPEDATA *pr,float dtime);
    void SetTextureGrid(ROPEDATA *pr);
    void SetIndex();
    void DoDelete();
    void AddLabel(GEOS::LABEL &lbl, NODE* nod, bool bDontSage );
    void SetAdd(int firstNum);
    void LoadIni();
    void FirstRun();

    long vBuf,iBuf;
    DWORD nVert,nIndx;

    dword execute_tm;
    dword realize_tm;
};


/*API_MODULE_START("rope")
	CREATE_CLASS(0,ROPE)
API_MODULE_END
*/

#endif