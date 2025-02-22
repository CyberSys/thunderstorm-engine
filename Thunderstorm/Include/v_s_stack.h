#ifndef _VS_STACK_H_
#define _VS_STACK_H_

#include "vdata.h"

class DATA;

class VS_STACK {
public:
  VS_STACK() {
  };

  virtual ~VS_STACK() {
  };
  virtual DATA* Push(DATA* pdataclass = nullptr) = 0;
  virtual DATA* Pop() = 0;
  virtual DATA* Read(uint32_t offset, uint32_t index) = 0;
  virtual DATA* Read() = 0;
  virtual uint32_t GetDataNum() = 0;
  virtual void InvalidateFrom(uint32_t index) = 0;
};

#endif
