#ifndef __XI_SCROLLTEXT_H_
#define __XI_SCROLLTEXT_H_

#include "../inode.h"

#define PICE_TYPE_STRING		0 // ��������� ������
#define PICE_TYPE_FONTCHANGE	1 // ��������� ������ �����
#define PICE_TYPE_COLORCHANGE	2 // ��������� ������ �����
#define PICE_TYPE_NEXTLINE		3 // ��������� �������� ������
#define PICE_TYPE_NEXTTABLINE	4 // ��������� �������� ������ � ������� ������

struct STRING_PICE {
  long type;

  union {
    struct {
      long startOffset;
      long charQuantity;
    } strDescr;

    long fontID;
    uint32_t color;
  } data;
};

class CXI_SCROLLTEXT : public CINODE {
public:
  CXI_SCROLLTEXT();
  ~CXI_SCROLLTEXT();

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

  void SetText(const char* newText);

protected:
  void ClearText();
  long FillPices(char* pt, size_t beg, size_t size, long& idx, STRING_PICE* spl, long wid);

  void LoadIni(INIFILE* ini1, const char* name1, INIFILE* ini2, const char* name2) override;

  void TextDownShift(float);
  void TextUpShift();

protected:
  CINODE* m_pScroller; // ������ �� �������� (!!! ������ ���� �������� ����� ����)
  long m_nMaxStringes; // ����� ����� � ��������� ����

  uint32_t m_dwFontColor; // ������� ����
  long m_idFont; // ������� �����

  char* m_pText;
  long m_nPiceQuantity;
  STRING_PICE* m_pStrList;

  long GetStringWord(char* pstr, char* buff, size_t size);
};

#endif
