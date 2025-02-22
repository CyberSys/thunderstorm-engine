#ifndef _XI_STATUSLINE_H
#define _XI_STATUSLINE_H

#include "..//inode.h"

// static inactive images into one object
class CXI_STATUSLINE : public CINODE {
public:
  CXI_STATUSLINE();
  ~CXI_STATUSLINE();

  void Draw(bool bSelected, uint32_t Delta_Time) override;
  bool Init(INIFILE* ini1, const char* name1, INIFILE* ini2, const char* name2, VDX9RENDER* rs, XYRECT& hostRect,
            XYPOINT& ScreenSize) override;
  void ReleaseAll() override;
  int CommandExecute(int wActCode) override;
  bool IsClick(int buttonID, long xPos, long yPos) override;

  void MouseThis(float fX, float fY) override {
  }

  void ChangePosition(XYRECT& rNewPos) override;
  void SaveParametersToIni() override;
  uint32_t MessageProc(long msgcode, MESSAGE& message) override;

protected:
  void LoadIni(INIFILE* ini1, const char* name1, INIFILE* ini2, const char* name2) override;
  void Refresh() const;

protected:
  char* m_sGroupName; // image list name
  long m_idTex; // texture identificator
  long m_vBuf; // vertex buffer identificator
  long m_iBuf; // index buffer identificator
  long m_nVert; // vertex quantity
  long m_nIndx; // index quantity
  float m_fLineOffset; // �������� �� ���� ��� �����
  FXYRECT m_texRect1;
  FXYRECT m_texRect2;
};

#endif
