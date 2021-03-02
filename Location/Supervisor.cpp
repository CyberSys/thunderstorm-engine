//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	Supervisor
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "Supervisor.h"
#include "Character.h"
#include "LocatorArray.h"

//============================================================================================
//���������������, ������������������
//============================================================================================

Supervisor::Supervisor()
{
	numCharacters = 0;
	time = 0.0f;
	waveTime = 0.0f;
	curUpdate = 0;
	player = null;
}

Supervisor::~Supervisor()
{	
	isDelete = true;
	for(long i = 0; i < numCharacters; i++)
	{
		character[i].c->AlreadyDelete();
		_CORE_API->DeleteEntity(character[i].c->GetID());
	}

}

//�������� ��������� � �������
void Supervisor::AddCharacter(Character * ch)
{
	Assert(ch);
	if(numCharacters >= MAX_CHARACTERS) _THROW("Number of characters amount to criticle value, don't create new character");
	character[numCharacters].c = ch;
	character[numCharacters++].lastTime = time;
}

//������� ��������� �� �������
void Supervisor::DelCharacter(Character * ch)
{
	for(long i = 0; i < numCharacters; i++)
		if(character[i].c == ch)
		{
			character[i] = character[--numCharacters];
			return;
		}
}

void Supervisor::Update(float dltTime)
{
	//���� ��� ����������, ������ �� ������
	if(!numCharacters) return;
	//���������� ����������
	for(long i = 0; i < numCharacters; i++)
	{
		character[i].c->Move(dltTime);
		character[i].c->colMove = 0.0f;
		character[i].c->isCollision = false;
	}
	//��������� ���������, � ���������� ����������������� ����������
	long chr = 0;
	for(i = 0; i < numCharacters - 1; i++)
	{
		//��������� ������
		if(character[i].c->liveValue < 0 || character[i].c->deadName) continue;
		character[i].c->startColCharacter = chr;
		CVECTOR curPos(character[i].c->curPos);
		float radius = character[i].c->radius;
		for(long j = i + 1; j < numCharacters; j++)
		{
			//��������� ������
			Character * ci = character[i].c;
			Character * cj = character[j].c;
			if(cj->liveValue < 0 || cj->deadName) continue;
			//���������� ����� �����������
			float d = ~(curPos - cj->curPos);
			//��������� �������������� ����������
			float r = radius + cj->radius;
			float rr = r*4.0f;
			if(d > rr*rr) continue;
			//��������� ���������
			colchr[chr].c = cj;
			colchr[chr].d = sqrtf(d);
			colchr[chr].maxD = rr;
			chr++;
			//��������� ����������� ���������� �� ������
			if(cj->curPos.y > ci->curPos.y + ci->height) continue;
			if(ci->curPos.y > cj->curPos.y + cj->height) continue;
			//������������ ����������
			float dx = curPos.x - cj->curPos.x;
			float dz = curPos.z - cj->curPos.z;
			d = dx*dx + dz*dz;
			r *= 0.5f;
			if(d >= r*r) continue;
			if(d)
			{
				d = sqrtf(d); 
				d = (r - d)/d;
				dx *= d; dz *= d;
				ci->isCollision = true;
				cj->isCollision = true;
				bool moveI = ci->IsMove();
				if((~ci->impulse) > 0.0001f && !moveI)
				{
					moveI = ((ci->impulse.x*dx + ci->impulse.z*dz) < 0.0f);
				}
				bool moveJ = cj->IsMove();
				if((~cj->impulse) > 0.0001f && !moveJ)
				{
					moveJ = ((cj->impulse.x*dx + cj->impulse.z*dz) > 0.0f);
				}
				if(ci->IsFight())
				{
					if(moveI == moveJ)
					{
						ci->curPos.x += dx*0.5f;
						ci->curPos.z += dz*0.5f;
						cj->curPos.x -= dx*0.5f;
						cj->curPos.z -= dz*0.5f;
					}else{
						if(moveI)
						{
							ci->curPos.x += dx*0.999f;
							ci->curPos.z += dz*0.999f;
							cj->curPos.x -= dx*0.001f;
							cj->curPos.z -= dz*0.001f;
						}else{
							ci->curPos.x += dx*0.001f;
							ci->curPos.z += dz*0.001f;
							cj->curPos.x -= dx*0.999f;
							cj->curPos.z -= dz*0.999f;
						}
					}
				}else
				if(moveI == moveJ)
				{
					ci->curPos.x += dx*0.5f;
					ci->curPos.z += dz*0.5f;
					cj->curPos.x -= dx*0.5f;
					cj->curPos.z -= dz*0.5f;
				}else{
					if(moveI)
					{
						ci->curPos.x += dx*0.9f;
						ci->curPos.z += dz*0.9f;
						cj->curPos.x -= dx*0.1f;
						cj->curPos.z -= dz*0.1f;
					}else{
						ci->curPos.x += dx*0.1f;
						ci->curPos.z += dz*0.1f;
						cj->curPos.x -= dx*0.9f;
						cj->curPos.z -= dz*0.9f;
					}
				}
			}
			
		}
		character[i].c->numColCharacter = chr - character[i].c->startColCharacter;
	}
	character[i].c->numColCharacter = 0;
	//����������
	for(i = 0; i < numCharacters; i++) character[i].c->Calculate(dltTime);
	//�������� ���������� � ��������� ����� ���������
	for(i = 0; i < numCharacters; i++) character[i].c->Update(dltTime);
}

void Supervisor::PreUpdate(float dltTime)
{
	//���������� ��������� ����������
	for(long i = 0; i < numCharacters; i++) character[i].c->Reset();
	_CORE_API->Event("CharactersStateUpdate", "f", dltTime);
}

void Supervisor::PostUpdate(float dltTime)
{
	//���������� ��������� ���������
	time += dltTime;
	waveTime += dltTime;
	//������������ ����� ��� ���������� �������
	if(time > 10000.0f)
	{
		for(long i = 0; i < numCharacters; i++) character[i].lastTime -= time;
		time -= 10000.0f;
	}
	//���� �� ������ ����� ����������, ��������� ���
	if(curUpdate >= numCharacters)
	{
		if(waveTime < 0.1f) return;
		waveTime = 0.0f;
		curUpdate = 0;
	}
	//��������� �������� ���������
	if(numCharacters)
	{
		for(long i = 0; i < 5; i++)
		{
			if(curUpdate >= numCharacters) break;
			float dlt = time - character[curUpdate].lastTime;
			character[curUpdate].lastTime = time;
			if(api->GetEntityPointer(&character[curUpdate].c->GetID()))
			{
				_CORE_API->Event("CharacterUpdate", "if", character[curUpdate].c->GetID(), dlt);
			}
			curUpdate++;
		}
	}	
}

//���������� ������� ��� ��������
void Supervisor::SetSavePositions()
{
	for(long i = 0; i < numCharacters; i++)
	{
		if(!character[i].c) continue;
		character[i].c->SetSavePosition();
	}
}

//������� ������� ��� ��������
void Supervisor::DelSavePositions(bool isTeleport)
{
	for(long i = 0; i < numCharacters; i++)
	{
		if(!character[i].c) continue;
		character[i].c->DelSavePosition(isTeleport);
	}
}

//��������� �� ����������� �������
bool Supervisor::CheckPosition(float x, float y, float z, Character * c)
{
	for(long i = 0; i < numCharacters; i++)
	{
		if(character[i].c == c) continue;
		float dx = x - character[i].c->curPos.x;
		float dy = y - character[i].c->curPos.y;
		float dz = z - character[i].c->curPos.z;
		if(fabsf(dy) > character[i].c->height*0.8f) continue;
		if(dx*dx + dz*dz > character[i].c->radius*0.8f) continue;
		return false;
	}
	return true;
}

//����� �� ������� ����������
bool Supervisor::FindCharacters(FindCharacter fndCharacter[MAX_CHARACTERS], long & numFndCharacters, Character * chr, float radius, float angTest, float nearPlane, float ax, bool isSort, bool lookCenter)
{
	numFndCharacters = 0;
	if(!chr || radius < 0.0f) return false;
	//������ ������������
	radius *= radius;
	//������� ���������
	float x = chr->curPos.x;
	float y = chr->curPos.y;
	float z = chr->curPos.z;
	//��������� ��� ������������ � ������� �� x_z
	CVECTOR N1, N2, N3;
	float d1, d2, d3;
	if(angTest > 0.0f)
	{
		CMatrix m(0.0f, chr->ay, 0.0f);
		float ang = 0.5f*angTest*3.141592654f/180.0f;
		N1 = m*CVECTOR(cosf(ang), 0.0f, sinf(ang));
		d1 = N1 | chr->curPos;
		N2 = m*CVECTOR(-cosf(-ang), 0.0f, -sinf(-ang));
		d2 = N2 | chr->curPos;
		N3 = m.Vz();
		d3 = N3 | chr->curPos;
	}
	//��������� ��� ������������ � ������� �� y_dist
	if(ax > 0.0f)
	{
		ax = sinf(0.5f*ax*3.141592654f/180.0f);
	}else ax = 1.0f;
	if(ax < 0.0f) ax = 0.0f;
	if(ax > 1.0f) ax = 1.0f;
	ax *= ax;
	float testY = y + chr->height*0.5f;
	//������������� ����������
	for(long i = 0; i < numCharacters; i++)
	{
		//�������� ����
		if(character[i].c == chr) continue;
		//�������� �� ������
		if(character[i].c->liveValue < 0 || character[i].c->deadName) continue;
		//�� ���������
		float dx = character[i].c->curPos.x - x;
		float dz = character[i].c->curPos.z - z;
		float d = dx*dx + dz*dz;
		if(d > radius) continue;
		//�� ������
		float dy = character[i].c->curPos.y + character[i].c->height - testY;
		if(dy < 0.0f && dy*dy > d*ax) continue;
		dy = testY - character[i].c->curPos.y;
		if(dy < 0.0f && dy*dy > d*ax) continue;		
		//� ��������� xz
		float dist1 = 0.0f;
		float dist2 = 0.0f;
		float dist3 = 0.0f;
		if(angTest > 0.0f && d>1.0f) //eddy. ��� ������� ������� �� ����� �������� � ���������
		{
			//�������� ������������
			float rad = !lookCenter ? -character[i].c->radius : 0.0f;
			dist1 = (N1 | character[i].c->curPos) - d1;
			if(dist1 < rad) continue;	
			dist2 = (N2 | character[i].c->curPos) - d2;
			if(dist2 < rad) continue;	
			dist3 = (N3 | character[i].c->curPos) - d3;
			if(dist3 < nearPlane) continue;	
		}
		//���������
		fndCharacter[numFndCharacters].c = character[i].c;
		fndCharacter[numFndCharacters].dx = dx;
		fndCharacter[numFndCharacters].dy = dy;
		fndCharacter[numFndCharacters].dz = dz;
		fndCharacter[numFndCharacters].d2 = d;
		numFndCharacters++;
	}
	if(isSort)
	{
		for(long i = 0; i < numFndCharacters - 1; i++)
		{
			for(long j = i + 1, k = i; j < numFndCharacters; j++)
			{
				if(fndCharacter[k].d2 > fndCharacter[j].d2) k = j;
			}
			if(k != i)
			{
				FindCharacter fc = fndCharacter[i];
				fndCharacter[i] = fndCharacter[k];
				fndCharacter[k] = fc;
			}
		}
	}
	return numFndCharacters != 0;
}

//����� ����������� ������� ��� ����������� �������� ���������
long Supervisor::FindForvardLocator(LocatorArray * la, const CVECTOR & pos, const CVECTOR & norm, bool lookChr)
{
	if(!la) return -1;
	long num = la->Num();
	CVECTOR lpos;
	float maxcs;
	for(long i = 0, l = -1; i < num; i++)
	{
		if(!la->GetLocatorPos(i, lpos.x, lpos.y, lpos.z)) continue;
		if(lookChr)
		{
			if(!CheckPosition(lpos.x, lpos.y, lpos.z, null)) continue;			
		}
		lpos -= pos; lpos.y = 0.0f;
		float cs = lpos.x*lpos.x + lpos.z*lpos.z;
		if(cs <= 0.0f) continue;
		lpos *= 1.0f/sqrtf(cs);
		cs = lpos | norm;
		if(l > 0)
		{
			if(cs > maxcs)
			{
				l = i;
				maxcs = cs;
			}
		}else{
			l = i;
			maxcs = cs;
		}
	}
	if(l >= 0 && la->GetLocatorPos(l, lpos.x, lpos.y, lpos.z))
	{
		if(!CheckPosition(lpos.x, lpos.y, lpos.z, null))
		{
			return FindForvardLocator(la, pos, norm, true);
		}
	}	
	return l;	
}




