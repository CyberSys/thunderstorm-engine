//===========================================================================================================================
//  Animation temporary file
//  ANT file
//  Ver 1.0
//===========================================================================================================================
//
//  ��������� �����:
//
//     AntFileHeader
//     char[AntFileHeader.stringsTableSize]
//	   AntFileBone[AntFileHeader.bonesCount]
//     AntFileTrackElement[AntFileHeader.framesCount]
//
//
//===========================================================================================================================

#ifndef _ANTFILE_H_


#define ANTFILE_ID			(unsigned long)(('A' << 0) | ('N' << 8) | ('T' << 16)  | (' ' << 24))
#define ANTFILE_VER			(unsigned long)(('1' << 0) | ('.' << 8) | ('0' << 16)  | (' ' << 24))
#define ANTFILE_NOPARENT	(-1L)


#pragma pack(push, 1)


//��������� �����
struct AntFileHeader
{
	unsigned long id;					//������������� �����
	unsigned long ver;					//������ �����
	unsigned long bonesCount;			//���������� ������ � ����� >= 1
	unsigned long framesCount;			//���������� ������ �������� >= 1
	unsigned long stringsTableSize;		//������ ������� ����� ��� ������
	float defaultFPS;					//�������� ������������ ��������
};

//�������� �����
struct AntFileBone
{
	long parentIndex;					//������ ������������ �����, ������� ��������
	unsigned long nameIndex;			//������ ������ � ������� �����
};

//�������� �������� �����
struct AntFileTrackElement
{
	float qx, qy, qz, qw;					//����������
	float px, py, pz;						//�������
	float sx, sy, sz;						//����������� ���������������
};


#pragma pack(pop)

#endif
