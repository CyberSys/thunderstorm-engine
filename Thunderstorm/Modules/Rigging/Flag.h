#ifndef _FLAG_H_
#define _FLAG_H_

#include "dx9render.h"
#include "geos.h"
#include "model.h"
#include "matrix.h"

#define FLAGLXVERTEX_FORMAT		(D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_TEXTUREFORMAT2)

struct FLAGLXVERTEX {
  CVECTOR pos;
  float tu, tv;
};

class FLAG : public Entity {
  // ��������� ���������� �� INI-����� //
  //-------------------------------------
  // ������ �������
  int ENGLISH_PENNON_TEX;
  int SPANISH_PENNON_TEX;
  int FRENCH_PENNON_TEX;
  int PIRATE_PENNON_TEX;
  int ENGLISH_FLAG_TEX;
  int SPANISH_FLAG_TEX;
  int FRENCH_FLAG_TEX;
  int PIRATE_FLAG_TEX;

  float FLAGVECTORLEN; // ����� �������� �����
  float ALFA_DEPEND; // �������� ��������� ���� ���������
  float BETA_DEPEND; // �������� ��������� ���� ��������
  float ALFA_RAND; // ������������ �������� ���������� ��������� ���� Alpha
  float BETA_RAND; // ������������ �������� ���������� ��������� ���� Beta
  float fWindAm; // ��������� ��������� �����
  float fRotAm; // ��������� �������� �����
  float DOWNVAL; // �������� ���������� �� Y
  float fAlfaMax; // ������������ ���� (��� ����� �����)
  float fAlfaStep;
  float fBetaMax; // ������������ ���� (��� ����� �����)
  float fBetaStep;
  int FlagTextureQuantity;
  float fWindMaxValue;
  int MinSegmentQuantity;
  //-------------------------------------

  bool bUse;
  bool bFirstRun;
  VDX9RENDER* RenderService;
  char* TextureName;
  long texl;

  struct WIND {
    float base; // ��������� ��������� ����� �� 0 �� 1.0
    struct {
      float x, y, z;
    } ang;
  };

  WIND globalWind;
  FILETIME ft_old;
public:
  FLAG();
  ~FLAG();
  void SetDevice();
  bool Init() override;
  void Execute(uint32_t Delta_Time);
  void Realize(uint32_t Delta_Time);
  bool CreateState(ENTITY_STATE_GEN* state_gen);
  bool LoadState(ENTITY_STATE* state);
  uint64_t ProcessMessage(MESSAGE& message) override;

  void ProcessStage(Stage stage, uint32_t delta) override {
    switch (stage) {
    case Stage::execute:
      Execute(delta);
      break;
    case Stage::realize:
      Realize(delta);
      break;
      /*case Stage::lost_render:
        LostRender(delta); break;
      case Stage::restore_render:
        RestoreRender(delta); break;*/
    }
  }

private:
  struct FLAGDATA {
    uint16_t vectQuant; // number of segmentes(square) into flags

    bool triangle;
    CVECTOR spos; // start flags position
    CVECTOR dv; // delta flags start position to along
    CVECTOR dhv, ddhv; // delta from position to flag top&bottom and delta for this value

    CVECTOR curpos, dHV;
    int lineNum;
    int windIdx, curIdx;
    bool bDecreaze;

    uint32_t sv, nv; // start vertex into buffer and quantity vertex
    uint32_t st, nt; // start treangle into buffer and quantity vertex

    int flagNum; // number of flag
    int grNum; // number of flag group
    NODE* nod;
    CMatrix* pMatWorld;

    int texNum;

    FLAGDATA* next;

    float Alfa;
    float Beta;

    int HostGroup;
    bool bDeleted;
  };

  int flagQuantity;
  FLAGDATA** flist;

  struct GROUPDATA {
    int nation;
    entid_t model_id;
    bool bDeleted;
  };

  int groupQuantity;
  GROUPDATA* gdata;

  void FirstRun();
  void SetTextureCoordinate() const;
  void SetTreangle() const;
  void DoMove(FLAGDATA* pr, float delta_time) const;
  void AddLabel(GEOS::LABEL& gl, NODE* nod);
  void SetAll();
  void LoadIni();
  void GroupSTORM_DELETE(entid_t m_id);
  void DoSTORM_DELETE();
  void SetAdd(int flagNum);
  void MoveOtherHost(entid_t newm_id, long flagNum, entid_t oldm_id);

  FLAGLXVERTEX* vertBuf;
  uint16_t* indxBuf;

  D3DXMATRIX rootMatrix;

  long vBuf, iBuf;
  uint32_t nVert, nIndx;
  bool bYesDeleted;
  int wFlagLast;
};

#endif
