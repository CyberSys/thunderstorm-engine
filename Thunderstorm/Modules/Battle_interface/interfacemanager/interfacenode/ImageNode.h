#ifndef BIManager_ImageNode_H
#define BIManager_ImageNode_H

#include "BaseNode.h"
#include "../../bi_defines.h"

class IBIImage;

class BI_ImageNode : public BI_BaseNode {
public:
  BI_ImageNode(BI_ManagerBase* pManager, const char* texture, const FRECT& uv, const RECT& pos, uint32_t color,
               long nPrioritet);
  virtual ~BI_ImageNode();

  void Update() override;

protected:
  IBIImage* m_pImage;
};

#endif
