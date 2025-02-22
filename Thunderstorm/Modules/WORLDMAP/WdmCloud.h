//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmCloud
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmCloud_H_
#define _WdmCloud_H_


#include "Matrix.h"
#include "WdmRenderObject.h"


class WdmCloud : public WdmRenderObject {
  static IDirect3DVertexDeclaration9* vertexDecl_;
  void CreateVertexDeclaration(VDX9RENDER* rs);

  struct MoveInfo {
    CVECTOR pos;
    CVECTOR v;
    CVECTOR cent;
    float rotSpd;
    float kSpd;
    float dx, dz;
    float dTime;
  };

  struct Rain {
    uint32_t isLive;
    CVECTOR pos;
    float alpha;
    float vy;
    float size;
    float angle;
  };

  struct Vertex {
    CVECTOR pos;
    uint32_t c;
    float u, v;
  };

  //--------------------------------------------------------------------------------------------
  //���������������, ������������������
  //--------------------------------------------------------------------------------------------
public:
  WdmCloud();
  virtual ~WdmCloud();

  //�������
  void Update(float dltTime) override;
  //���������
  void PRender(VDX9RENDER* rs) override;
  void LRender(VDX9RENDER* rs) override;

  virtual void Move(float dltTime);

  long FillRain(RS_RECT* rainRect, long rcnt);

  CVECTOR pos; //������� ������
  CVECTOR dir; //����������� ����������� ������
  float globalAlpha; //������� ������������
  float constAlpha; //������� ������������

  //--------------------------------------------------------------------------------------------
  //������������
  //--------------------------------------------------------------------------------------------
protected:
  virtual void BuildCloud(long n);
  RS_RECT rect[8];
  long texture;
  long light;
  long lightning;
  long curMove;
  long numRects;
  MoveInfo move[8];

  //��������� ������
  long curLightning;
  float lightningWaitTime;
  float lightningTime;
  float flashTime;
  long lastColor;
  long lightningColor;

  //������
  Rain rain[48];
  float rainBurnTime;

private:
  void Render(VDX9RENDER* rs);
  void FindPartPos(CVECTOR& v);
  float Rnd();
};

#endif
