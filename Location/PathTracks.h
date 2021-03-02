#ifndef _PathTracks_h_
#define _PathTracks_h_

#include "..\common_h\dx8render.h"
#include "..\common_h\math3D.h"
#include "CameraTracksFile.h"

class PathTracks  
{
#pragma pack(push, 1)
	struct Point
	{
		Quaternion q;
		Vector p;
		Vector s;
	};
#pragma pack(pop)

//--------------------------------------------------------------------------------------------
public:
	PathTracks();
	virtual ~PathTracks();

//--------------------------------------------------------------------------------------------
public:
	//��������� ���� � PathTracks
	bool Load(const char * fileName);
	//���������� ����
	void Draw(VDX8RENDER* render);
	//�������� ����� �����
	bool GetPoint(float index, Vector & cp, Quaternion & cq);

//--------------------------------------------------------------------------------------------
private:
	Point * point;
	long numPoints;
};

#endif
