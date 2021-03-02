#ifndef _XI_STRCOLLECTION_H
#define _XI_STRCOLLECTION_H

#include "..//inode.h"

// text collection
class CXI_STRCOLLECTION : public CINODE
{
public:
	CXI_STRCOLLECTION();
	~CXI_STRCOLLECTION();

	void Draw(bool bSelected, uint32_t Delta_Time) override;
	bool Init(INIFILE* ini1, const char * name1, INIFILE* ini2, const char * name2, VDX9RENDER* rs, XYRECT& hostRect,
	          XYPOINT& ScreenSize) override;
	void ReleaseAll() override;
	int CommandExecute(int wActCode) override;
	bool IsClick(int buttonID, long xPos, long yPos) override;

	void MouseThis(float fX, float fY) override
	{
	}

	bool GetInternalNameList(std::vector<std::string>& aStr) override;
	void SetInternalName(std::string& sName) override;
	void ChangePosition(XYRECT& rNewPos) override;
	void SaveParametersToIni() override;
	uint32_t MessageProc(long msgcode, MESSAGE& message) override;

	void ChangeString(long num, const char* sValue);
	void ChangeStringColor(long num, uint32_t dwColor);
	void ChangeStringPos(long num, XYPOINT& pntNewPos);

protected:
	struct STRINGDESCR
	{
		char* sFontName;
		int nFontNum;
		long strNum;
		char* strStr;
		char* strID;
		XYPOINT scrPos; // screen position for string show
		uint32_t foreColor; // color of font
		uint32_t backColor; // color of the font background
		int wAlignment; // print text alignment
		bool bShadow; // do shadow
		float fScale; // scale for string out
	};

protected:
	void LoadIni(INIFILE* ini1, const char * name1, INIFILE* ini2, const char * name2) override;
	STRINGDESCR* CreateNewDinamicString(char* strID, char* strStr);

protected:
	int m_nStr; // string quantity
	STRINGDESCR* m_pStrDescr; // pointer to array of the string descriptors
	long m_nEditIndex;
};

#endif
