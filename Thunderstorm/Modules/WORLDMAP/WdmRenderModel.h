//============================================================================================
//	Spirenkov Maxim, 2001
//--------------------------------------------------------------------------------------------
//	Sea Dogs II
//--------------------------------------------------------------------------------------------
//	WdmRenderModel
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#ifndef _WdmRenderModel_H_
#define _WdmRenderModel_H_


#include "WdmRenderObject.h"
#include "WdmObjects.h"

#include "Matrix.h"

class GEOS;
class VGEOMETRY;


class WdmRenderModel : public WdmRenderObject {
  //--------------------------------------------------------------------------------------------
  //Конструирование, деструктурирование
  //--------------------------------------------------------------------------------------------
public:
  WdmRenderModel();
  virtual ~WdmRenderModel();

  virtual bool Load(const char* modelName);

  void PRender(VDX9RENDER* rs) override;
  void MRender(VDX9RENDER* rs) override;
  void LRender(VDX9RENDER* rs) override;

  void SetTech(const char* t, const char* ta);
  long GetTexture(long stage) const;
  void SetTexture(long stage, long id) const;


  CMatrix mtx;
  float alpha;

public:
  void Render(VDX9RENDER* rs) const;

  GEOS* geo;
  const char* tech;
  const char* techa;

  //Позиционированный круг
  CVECTOR center;
  float radius;

  bool drawCircle;
};

#endif
