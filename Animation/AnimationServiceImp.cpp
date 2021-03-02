//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	Storm engine v2.00
//--------------------------------------------------------------------------------------------
//	AnimationServiceImp
//--------------------------------------------------------------------------------------------
//	������ ��������, ����������� ��������� ������� AnimationManager
//============================================================================================

#include "AnimationServiceImp.h"
#include "AnimationImp.h"
#include "an_file.h"


//============================================================================================
					
//����� �������� �������������� ��������
#define ASRV_DOWNTIME	1
//���������� ��������� �������, ���������� � AnimationManager
#define ASRV_MAXDLTTIME	50

//����
#define ASKW_PATH_ANI	"resource\\animation\\"
#define ASKW_PATH_JFA	"resource\\animation\\"

//�������� �����
#define ASKW_JFA_FILE	"animation"			//���� �� �������� � ���������
#define ASKW_STIME		"start_time"		//��������� ����� ��������
#define ASKW_ETIME		"end_time"			//�������� ����� ��������
#define ASKW_RATE		"speed"				//���������� �������� ���������������
#define ASKW_TYPE		"type"				//��� ��������
#define ASKW_LOOP		"loop"				//��������� ��������
#define ASKW_EVENT		"event"				//�������
#define ASKW_DATA		"data"				//���������������� ������
#define ASKW_BONE		"bone"				//������� ������ ������������ ��� ��������

//��� ��������:
#define ASKWAT_NORMAL		"normal"		//�� ������ �� ����� � ������������
#define ASKWAT_REVERSE		"reverse"		//�� ����� �� ������ � ������������
#define ASKWAT_PINGPONG		"pingpong"		//�� ������ �� �����, ������� � ������������
#define ASKWAT_RPINGPONG	"rpingpong"		//�� ����� �� ������, ������� � ������������
//������������� ��������
#define ASKWAL_TRUE		"true"				//���������� ������������� ��������
#define ASKWAL_FALSE	"false"				//���������� ������������� ��������
//��� �������
#define ASKWAE_ALWAYS	"always"			//������ ��������
#define ASKWAE_NORMAL	"normal"			//��� ������ ������������� ��������
#define ASKWAE_REVERSE	"reverse"			//��� �������� ������������� ��������

//============================================================================================

INTERFACE_FUNCTION
CREATE_SERVICE(AnimationServiceImp)

//============================================================================================

char AnimationServiceImp::key[1024];

//============================================================================================
//���������������, ������������������
//============================================================================================

AnimationServiceImp::AnimationServiceImp()
{
	ainfo = null;
	numInfos = 0;
	animation = null;
	numAnimations = 0;
	AnimationImp::SetAnimationService(this);
}

AnimationServiceImp::~AnimationServiceImp()
{
	if(animation)
	{
		for(long i = 0; i < numAnimations; i++)
			if(animation[i])
			{
				_CORE_API->Trace("No release Animation pnt:0x%x for %s.ani", animation[i], animation[i]->GetAnimationInfo()->GetName());
				delete animation[i];
			}
		delete animation;		
	}
	if(ainfo)
	{
		for(long i = 0; i < numInfos; i++)
				if(ainfo[i]) delete ainfo[i];
		delete ainfo;
	}
}

//============================================================================================

//����� ����������
dword AnimationServiceImp::RunSection()
{
	return SECTION_REALIZE;
};

//������� ����������
void AnimationServiceImp::RunStart()
{
#ifndef _XBOX
	if(api->Controls->GetDebugAsyncKeyState(VK_F4)) return;
#endif
	dword dltTime = _CORE_API->GetDeltaTime();
	if(dltTime > 1000) dltTime = 1000;
	//���������� ��� ��������
	for(long i = 0; i < numInfos; i++) 
		if(ainfo[i])
		{
			ainfo[i]->AddDowntime(dltTime);
			if(ainfo[i]->GetDowntime() >= ASRV_DOWNTIME)
			{
				//��������� ����� �� ������������ ��������
				//_CORE_API->Trace("Download animation %s", ainfo[i]->GetName());
				delete ainfo[i];
				ainfo[i] = null;				
			}
		}
	//�������� ��� ��������
	for(i = 0; i < numAnimations; i++)
		if(animation[i])
		{
			for(long dt = dltTime; dt > ASRV_MAXDLTTIME; dt -= ASRV_MAXDLTTIME)
								animation[i]->Execute(ASRV_MAXDLTTIME);
			if(dt > 0) animation[i]->Execute(dt);
			//_CORE_API->Trace("Animation: 0x%.8x Time: %f", animation[i], animation[i]->Player(0).GetPosition());
		}
}

void AnimationServiceImp::RunEnd()
{

}

//������� �������� ��� ������, ������� ����� delete
Animation * AnimationServiceImp::CreateAnimation(const char * animationName)
{
	//���� ��������, ���� ���, �� ���������
	for(long i = 0; i < numInfos; i++) 
		if(ainfo[i])
		{
			if(ainfo[i][0] == animationName) break;
		}	
	if(i == numInfos)
	{
		i = LoadAnimation(animationName);
		if(i < 0) return null;
	}
	long aniIndex = i;
	//�������� ���������, ������ �������� ��������
	for(i = 0; i < numAnimations; i++)
		if(!animation[i])  break;
	if(i == numAnimations)
	{
		numAnimations += 64;
		animation = (AnimationImp **)RESIZE(animation, numAnimations*4);
		for(long j = i; j < numAnimations; j++) animation[j] = null;
	}
	animation[i] = NEW AnimationImp(i, ainfo[aniIndex]);
	return animation[i];
}

//������� �������� (���������� �� �����������)
void AnimationServiceImp::DeleteAnimation(AnimationImp * ani)
{
	Assert(ani);
	Assert(ani->GetThisID() >= 0 || ani->GetThisID() < numAnimations);
	Assert(animation[ani->GetThisID()] == ani);
	animation[ani->GetThisID()] = null;
}

//�������
void AnimationServiceImp::Event(const char * eventName)
{
	//�������� ��������� �������
	_CORE_API->Trace("Called function <void AnimationServiceImp::Event(%s)>, please make it.", eventName);
}

//��������� ��������
long AnimationServiceImp::LoadAnimation(const char * animationName)
{
	//��������� ��� �����
	static char path[MAX_PATH];
	strcpy(path, ASKW_PATH_ANI);
	strcat(path, animationName);
	strcat(path, ".ani");
	//��������� ini ����, ����������� ��������
	INIFILE * ani = _CORE_API->fio->OpenIniFile(path);
	if(!ani)
	{
		_CORE_API->Trace("Cannot open animation file %s", path);
		return -1;
	}
	//�������� ��� jfa ����� �� ��������
	strcpy(path, ASKW_PATH_JFA);
	int l = strlen(path);
	if(!ani->ReadString(null, ASKW_JFA_FILE, path + l, MAX_PATH - l - 1, null))
	{
		_CORE_API->Trace("Incorrect key \"%s\" in animation file %s.ani", ASKW_JFA_FILE, animationName);
		delete ani;
		return -1;
	}
	//��������� ��������
	AnimationInfo * info = NEW AnimationInfo(animationName);
	//�������� �����	
	if(!LoadAN(path, info))
	{
		delete ani;
		delete info;
		_CORE_API->Trace("Animation file %s is damaged!", path);
		return -1;	
	}
	//���������� ���������������� ������
	LoadUserData(ani, null, info->GetUserData(), animationName);
	//�������� ��������	
	for(bool isHaveSection = ani->GetSectionName(path, 63);
		isHaveSection;
		isHaveSection = ani->GetSectionNameNext(path, 63))
	{
		//��������� ��������
		if(path[0] == 0 || strlen(path) >= 64)
		{
			_CORE_API->Trace("Incorrect name action [%s] of animation file %s.ani", path, animationName);
			continue;
		}
		//���������� �������
		long stime = ani->GetLong(path, ASKW_STIME, -1);
		if(stime < 0)
		{
			_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani", ASKW_STIME, path, animationName);
			continue;
		}
		long etime = ani->GetLong(path, ASKW_ETIME, -1);
		if(etime < 0)
		{
			_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani", ASKW_ETIME, path, animationName);
			continue;
		}	
		//��������� ��������
		ActionInfo * aci = info->AddAction(path, stime, etime);
		if(aci == null)
		{
			_CORE_API->Trace("Warning! Action [%s] of animation file %s.ani is repeated, skip it", path, animationName);
			continue;
		}
		//���������� �������� ���������������
		float rate = ani->GetFloat(path, ASKW_RATE, 1.0f);
		aci->SetRate(rate);
		//��� ��������
		AnimationType type = at_normal;
		if(ani->ReadString(path, ASKW_TYPE, key, 256, ASKWAT_NORMAL))
		{
			if(stricmp(key, ASKWAT_NORMAL) == 0) type = at_normal;
			else
			if(stricmp(key, ASKWAT_REVERSE) == 0) type = at_reverse;
			else
			if(stricmp(key, ASKWAT_PINGPONG) == 0) type = at_pingpong;
			else
			if(stricmp(key, ASKWAT_RPINGPONG) == 0) type = at_rpingpong;
			else{
				_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nNo set %s, set type is %s\n", ASKW_TYPE, path, animationName, key, ASKWAT_NORMAL);
			}
		}
		aci->SetAnimationType(type);
		//������������� ��������
		bool isLoop = true;
		if(ani->ReadString(path, ASKW_LOOP, key, 256, "false"))
		{
			if(stricmp(key, ASKWAL_TRUE) == 0) isLoop = true;
			else
			if(stricmp(key, ASKWAL_FALSE) == 0) isLoop = false;
			else{
				_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nThis parameter (%s) use is default value %s\n", ASKW_LOOP, path, animationName, key, ASKWAL_FALSE);
			}
		}
		aci->SetLoop(isLoop);
		//�������
		if(ani->ReadString(path, ASKW_EVENT, key, 256, ""))
		{
			do{
				key[256] = 0;
				memcpy(key + 257, key, 257);
				//������ �����
				if(key[0] != '"')
				{
					_CORE_API->Trace("Incorrect %s <%s> in action [%s] of animation file %s.ani\nFirst symbol is not '\"'\n", ASKW_EVENT, key + 257, path, animationName);
					continue;
				}
				//����� �����
				for(long p = 1; key[p] && key[p] != '"'; p++);
				if(!key[p])
				{
					_CORE_API->Trace("Incorrect %s <%s> in action [%s] of animation file %s.ani\nNot found closed symbol '\"'\n", ASKW_EVENT, key + 257, path, animationName);
					continue;
				}
				if(p == 1)
				{
					_CORE_API->Trace("Incorrect %s <%s> in action [%s] of animation file %s.ani\nName have zero lenght\n", ASKW_EVENT, key + 257, path, animationName);
					continue;
				}
				if(p > 65)
				{
					_CORE_API->Trace("Incorrect %s <%s> in action [%s] of animation file %s.ani\nName have big length (max 63)\n", ASKW_EVENT, key + 257, path, animationName);
					continue;
				}
				key[p++] = 0;
				//���������� �����
				//������ �����
				for(; key[p] && (key[p] < '0' || key[p] > '9'); p++);
				if(!key[p])
				{
					_CORE_API->Trace("Incorrect %s <%s> in action [%s] of animation file %s.ani\nNo found time\n", ASKW_EVENT, key + 257, path, animationName);
					continue;
				}
				char * em = key + p;
				//���� ��������� �����
				for(; key[p] >= '0' && key[p] <= '9'; p++);
				float tm = 0;
				if(key[p] != '%')
				{
					//��������� �������� �������
					if( key[p] ) key[p++] = 0;
					tm = float(atof(em));
				}else{
					//������������� �������� �������
					key[p++] = 0;
					tm = float(atof(em));
					if(tm < 0) tm = 0;
					if(tm > 100) tm = 100;
					tm = stime + 0.01f*tm*(etime - stime);
				}
				if(tm < stime) tm = float(stime);
				if(tm > etime) tm = float(etime);
				//���������� ��� �������
				ExtAnimationEventType ev = eae_normal;
				if(key[p])
				{
					//���� ������
					for(p++; key[p]; p++)
						if((key[p] >= 'A' &&  key[p] <= 'Z') ||
							(key[p] >= 'a' &&  key[p] <= 'z')) break;
					em = key + p;
					//���� ���������
					for(p++; key[p]; p++)
						if(!(key[p] >= 'A' &&  key[p] <= 'Z') &&
							!(key[p] >= 'a' &&  key[p] <= 'z')) break;
					key[p] = 0;					
					if(em[0] == 0)
					{
					}else
					if(stricmp(em, ASKWAE_ALWAYS) == 0)
					{
						ev = eae_always;
					}else
					if(stricmp(em, ASKWAE_NORMAL) == 0)
					{
						ev = eae_normal;
					}else
					if(stricmp(em, ASKWAE_REVERSE) == 0)
					{
						ev = eae_reverse;
					}else{
						_CORE_API->Trace("Warning: Incorrect %s <%s> in action [%s] of animation file %s.ani,\nunknow event type <%s> -> set is default value\n", ASKW_EVENT, key + 257, path, animationName, em);
					}
				}
				//_CORE_API->Trace("Add event %s, time = %f to action %s", key + 1, (tm - stime)/float(etime - stime), path);
				//��������� �������
				if(!aci->AddEvent(key + 1, tm, ev))
				{
					_CORE_API->Trace("Warning: Incorrect %s <%s> in action [%s] of animation file %s.ani,\nvery many events -> ignory it\n", ASKW_EVENT, key + 257, path, animationName);
				}
			}while(ani->ReadStringNext(path, ASKW_EVENT, key, 256));
		}
		//�����

		//���������������� ������
		LoadUserData(ani, path, aci->GetUserData(), animationName);
	}
	//������� ini ����
	delete ani;
	//���� ��������� ���������
	for(long i = 0; i < numInfos; i++)
					if(!ainfo[i]) break;
	//���� ��� ������, �������� ������
	if(i == numInfos)
	{
		numInfos += 64;
		ainfo = (AnimationInfo **)RESIZE(ainfo, numInfos*4);
		for(long j = i; j < numInfos; j++) ainfo[j] = null;
	}
	ainfo[i] = info;
	return i;
}

//��������� �� ������� ������ ���������������� ������
void AnimationServiceImp::LoadUserData(INIFILE * ani, const char * sectionName, UserData & data, const char * animationName)
{
	if(ani->ReadString((char *)sectionName, ASKW_DATA, key, 1023, ""))
	{
		do
		{
			key[1023] = 0;
			//���� ��� ������
			//������ �����
			if(key[0] != '"')
			{
				if(sectionName)
					_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nFirst symbol is not '\"'", ASKW_DATA, sectionName, animationName);
				else
					_CORE_API->Trace("Incorrect %s in global data of animation file %s.ani\nFirst symbol is not '\"'", ASKW_DATA, animationName);
				continue;
			}
			//����� �����
			for(long p = 1; key[p] && key[p] != '"'; p++);
			if(!key[p])
			{
				if(sectionName)
					_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nNot found closed symbol '\"' for data name", ASKW_DATA, sectionName, animationName);
				else
					_CORE_API->Trace("Incorrect %s in global data of animation file %s.ani\nNot found closed symbol '\"' for data name", ASKW_DATA, animationName);
				continue;
			}
			if(p == 1)
			{
				if(sectionName)
					_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nName have zero lenght", ASKW_DATA, sectionName, animationName);
				else
					_CORE_API->Trace("Incorrect %s in global data of animation file %s.ani\nName have zero lenght", ASKW_DATA, animationName);
				continue;
			}
			key[p++] = 0;
			//��������� ������� ������
			if(data.GetData(key + 1))
			{
				if(sectionName)
					_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nUser data repeated", ASKW_DATA, sectionName, animationName);
				else
					_CORE_API->Trace("Incorrect %s in global data of animation file %s.ani\nUser data repeated", ASKW_DATA, animationName);

				continue;
			}
			//���� ������ � �������
			for(; key[p] && key[p] != '"'; p++);
			if(!key[p])
			{
				if(sectionName)
					_CORE_API->Trace("Incorrect %s in action [%s] of animation file %s.ani\nNo data string", ASKW_DATA, sectionName, animationName);
				else
					_CORE_API->Trace("Incorrect %s in global data of animation file %s.ani\nNo data string", ASKW_DATA, animationName);
				continue;
			}
			//���� ��������� ������ ������
			char * uds = key + ++p;
			for(; key[p] && key[p] != '"'; p++);
			key[p] = 0;
			//��������� ������
			//_CORE_API->Trace("Add user data \"%s\", \"%s\" of \"%s\"", key + 1, uds, sectionName);
			data.AddData(key + 1, uds);
		}while(ani->ReadStringNext((char *)sectionName, ASKW_DATA, key, 1023));
		data.FreeExtra();
	}	
}

//��������� AN
bool AnimationServiceImp::LoadAN(const char * fname, AnimationInfo * info)
{
	HANDLE fl = INVALID_HANDLE_VALUE;
	try{
		fl = _CORE_API->fio->_CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		if(fl == INVALID_HANDLE_VALUE)
		{
			_CORE_API->Trace("Cannot open file: %s", fname);
			return false;
		}		
		//������ ��������� �����
		ANFILE::HEADER header;
		if(!_CORE_API->fio->_ReadFile(fl, &header, sizeof(ANFILE::HEADER), 0) || header.nFrames <= 0 || header.nJoints <= 0 || header.framesPerSec < 0.0f || header.framesPerSec > 1000.0f)
		{
			_CORE_API->Trace("Incorrect file header in animation file: %s", fname);
			_CORE_API->fio->_CloseHandle(fl);
			return false;
		}	
		//��������� ����� ��������
		info->SetNumFrames(header.nFrames);
		//��������� �������� ��������
		info->SetFPS(header.framesPerSec);
		//������� ������ ����� ������
		info->CreateBones(header.nJoints);
		//������������� ���������
		long * prntIndeces = NEW long[header.nJoints];
		if(!_CORE_API->fio->_ReadFile(fl, prntIndeces, header.nJoints*sizeof(long), 0))
		{
			_CORE_API->Trace("Incorrect parent indeces block in animation file: %s", fname);
			delete prntIndeces;
			_CORE_API->fio->_CloseHandle(fl);
			return false;
		}
		for(long i = 1; i < header.nJoints; i++)
		{
			Assert(prntIndeces[i] >= 0 || prntIndeces[i] < header.nJoints);
			Assert(prntIndeces[i] != i);
			info->GetBone(i).SetParent(&info->GetBone(prntIndeces[i]));
		}
		delete prntIndeces;
		//��������� ������� ������
		CVECTOR * vrt = NEW CVECTOR[header.nJoints];
		if(!_CORE_API->fio->_ReadFile(fl, vrt, header.nJoints*sizeof(CVECTOR), 0))
		{
			_CORE_API->Trace("Incorrect start joints position block block in animation file: %s", fname);
			delete vrt;
			_CORE_API->fio->_CloseHandle(fl);
			return false;
		}
		for(i = 0; i < header.nJoints; i++)
		{
			info->GetBone(i).SetNumFrames(header.nFrames, vrt[i], i == 0);
		}
		delete vrt;

		//������� ������� �����
		vrt = NEW CVECTOR[header.nFrames];
		if(!_CORE_API->fio->_ReadFile(fl, vrt, header.nFrames*sizeof(CVECTOR), 0))
		{
			_CORE_API->Trace("Incorrect root joint position block block in animation file: %s", fname);
			delete vrt;
			_CORE_API->fio->_CloseHandle(fl);
			return false;
		}
		info->GetBone(0).SetPositions(vrt, header.nFrames);
		delete vrt;

		//����
		D3DXQUATERNION *ang = NEW D3DXQUATERNION[header.nFrames];
		for(i = 0; i < header.nJoints; i++)
		{
			if(!_CORE_API->fio->_ReadFile(fl, ang, header.nFrames*sizeof(*ang), 0))
			{
				_CORE_API->Trace("Incorrect joint angle block (%i) block in animation file: %s", i, fname);
				_CORE_API->fio->_CloseHandle(fl);
				return false;
			}
			info->GetBone(i).SetAngles(ang, header.nFrames);
		}		
		delete ang;

		//-----------------------------------------------
		for(i = 0; i < header.nJoints; i++)
		{
			info->GetBone(i).BuildStartMatrix();
		}
		for(i = 0; i < header.nJoints; i++)
		{
			info->GetBone(i).start.Transposition();
		}
		//-----------------------------------------------
	
		//������� ����
		_CORE_API->fio->_CloseHandle(fl);
		return true;
	}catch(...){
		if(fl != INVALID_HANDLE_VALUE) _CORE_API->fio->_CloseHandle(fl);
		_CORE_API->Trace("Error reading animation file: %s", fname);
		return false;
	}
}

