//============================================================================================
//	Spirenkov Maxim aka Sp-Max Shaman, 2001
//--------------------------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------------------------
//	WdmEnemyShip
//--------------------------------------------------------------------------------------------
//	
//============================================================================================

#include "WdmEnemyShip.h"
#include "WdmIslands.h"

//============================================================================================

#define WDM_ENEMYSHIP_BRNSPD			(1.0f/3.0f)	//����� ��������� 1/sec
#define WDM_ENEMYSHIP_DELSPD			(1.0f/2.0f)	//����� �������� 1/sec
#define WDM_ENEMYSHIP_MINTIME			60.0f		//����������� ����� ����� � sec
#define WDM_ENEMYSHIP_MAXTIME			180.0f		//������������ ����� ����� � sec

//============================================================================================
//���������������, ������������������
//============================================================================================

WdmEnemyShip::WdmEnemyShip()
{
	saveAttribute = null;
	mx = mz = 0.0f;
	ix = iz = 0.0f;
	sx = sz = 0.0f;
	dx = dz = 0.0f;
	isEnableKill = true;
	isLookOnPlayer = false;
	isEnemy = false;
	isEntryPlayer = false;
	deleteAlpha = 1.0f;
	slowingAlfa = 1.0f;
	brnAlpha = 0.0f;
	alpha = 0.0f;
	type = -1;
	attack = null;
	//����� �����
	liveTime = WDM_ENEMYSHIP_MINTIME + rand()*((WDM_ENEMYSHIP_MAXTIME - WDM_ENEMYSHIP_MINTIME)/RAND_MAX);
	shipType = wdmest_unknow;
	canSkip = false;
}

WdmEnemyShip::~WdmEnemyShip()
{
	/*
	if(saveAttribute)
	{
		ATTRIBUTES * atr = saveAttribute->GetParent();
		atr->DeleteAttributeClassX(saveAttribute);
		_CORE_API->Event("WorldMap_DeleteShipEncounter", "l", type);
	}*/
}

//�������
void WdmEnemyShip::Update(float dltTime)
{
	if(!wdmObjects->playerShip) return;
	//�����������
	Move(dltTime);
	//���������� �� ��� �� ������
	float dx = wdmObjects->playerShip->mtx.Pos().x - mtx.Pos().x;
	float dz = wdmObjects->playerShip->mtx.Pos().z - mtx.Pos().z;
	float d = sqrtf(dx*dx + dz*dz);
	//���������
	//�� ���������� �� ������
	alpha = 1.0f - (d - wdmObjects->enemyshipViewDistMin)/(wdmObjects->enemyshipViewDistMax - wdmObjects->enemyshipViewDistMin);
	//�� ������� ���������
	brnAlpha += WDM_ENEMYSHIP_BRNSPD*dltTime;
	if(brnAlpha > 1.0f) brnAlpha = 1.0f;
	alpha *= brnAlpha;
	//�� ������� ��������
	alpha *= slowingAlfa;//deleteAlpha;
	if(alpha < 0.0f) alpha = 0.0f;
	if(alpha > 1.0f) alpha = 1.0f;
	//�������� ���������� �����
	if(isEnableKill)
	{
		if(deleteAlpha >= 1.0f)
		{
			//�������� �� ����������
			if(d > wdmObjects->enemyshipDistKill) deleteAlpha = 0.999999f;
			//�������� ������� �����
			liveTime -= dltTime;
			if(liveTime < 0.0f) deleteAlpha = 0.999999f;
			//������� ��������
			if(KillTest()) deleteAlpha = 0.999999f;
		}else{
			//������� ��������
			deleteAlpha -= WDM_ENEMYSHIP_DELSPD*dltTime;
			slowingAlfa = deleteAlpha;
			if(deleteAlpha < 0.0f)
			{
				const char * delEnc = "";
				if(saveAttribute)
				{
					delEnc = saveAttribute->GetThisName();
				}
				VDATA* pVDat = 0;
				if(!killMe && delEnc && delEnc[0])
				{
					pVDat = api->Event("WorldMap_EncounterDelete", "s", delEnc);
				}
				deleteAlpha = 0;
				if( !pVDat || pVDat->GetLong()!=0 )
				{
					killMe = true;
				} else {
					slowingAlfa = 1.f;
				}
			}
		}
	}
	//
	WdmShip::Update(dltTime);
	//���������� ����������
	UpdateSaveData();
}

void WdmEnemyShip::LRender(VDX8RENDER * rs)
{
	if(isWMRender && wdmObjects->isDebug)
	{
		long a = long(alpha*255.0f);
		if(a < 60) a = 60;
		if(a > 255) a = 255;
		a <<= 24;
		wdmObjects->DrawVector(mtx.Pos(), mtx.Pos() + CVECTOR(mx, 0.0f, mz)*10.0f, 0x00ff00 | a);
		wdmObjects->DrawVector(mtx.Pos(), mtx.Pos() + CVECTOR(ix, 0.0f, iz)*10.0f, 0xff0000 | a);
		wdmObjects->DrawVector(mtx.Pos(), mtx.Pos() + CVECTOR(sx, 0.0f, sz)*10.0f, 0x0000ff | a);
		wdmObjects->DrawVector(mtx.Pos(), mtx.Pos() + CVECTOR(dx, 0.0f, dz)*10.0f, 0xffffff | a);
	}
	WdmShip::LRender(rs);
}

//����� ���� ������������� � ������ �����������
void WdmEnemyShip::FindMoveForce()
{
	mx = 0.0f;
	mz = 0.0f;
}

//����� ���� ������������� �� ��������
void WdmEnemyShip::FindIslandForce()
{
	CVECTOR reaction;
	if(wdmObjects->islands)
	{
		wdmObjects->islands->FindReaction(mtx.Pos(), reaction);
		ix = reaction.x*1.5f;
		iz = reaction.z*1.5f;
	}else{
		ix = 0.0f;
		iz = 0.0f;
	}
}

//����� ���� ������������� �� ��������
void WdmEnemyShip::FindShipsForce()
{
	sx = 0.0f;
	sz = 0.0f;
	WdmShip ** s = wdmObjects->ships;
	for(long i = 0; i < wdmObjects->numShips; i++)
	{
		if(!s[i]) continue;
		if(s[i] == this) continue;
		if(s[i] == wdmObjects->playerShip && !isLookOnPlayer) continue;
		//������ �� ���� �� ���
		float fx = mtx.Pos().x - s[i]->mtx.Pos().x;
		float fz = mtx.Pos().z - s[i]->mtx.Pos().z;
		//���������
		float fl = fx*fx + fz*fz - 25.0f*25.0f;
		if( fl > 25.0f*25.0f ) continue;
		if(fl < 0.1f) fl = 0.1f;
		fl = 0.2f/powf(fl, 0.5f);
		fx *= fl;
		fz *= fl;
		sx += fx;
		sz += fz;
		//����������
		fl = fx*fx + fz*fz;
		fx *= s[i]->mtx.Vz().x;
		fz *= s[i]->mtx.Vz().z;
		sx += -fz*0.01f;
		sz += fx*0.01f;
	}
	float sl = sx*sx + sz*sz;
	if(sl > 1.0f)
	{
		sl = 1.0f/sqrtf(sl);
		sx *= sl;
		sz *= sl;
	}
}

//��� ������� �� ����������� ������
void WdmEnemyShip::Move(float dltTime)
{
	//��������� ������� ��������
	//���� �����
	FindMoveForce();
	//������������ �� ��������
	FindIslandForce();
	//������������ �� ��������
	FindShipsForce();
	//��������������
	dx = 1.0f*mx + 1.5f*ix + 1.1f*sx;
	dz = 1.0f*mz + 1.5f*iz + 1.1f*sz;
	float dl = dx*dx + dz*dz;
	//���� �����������
	float vx = mtx.Vz().x;
	float vz = mtx.Vz().z;
	float vl = vx*vx + vz*vz;
	//����� ���� ����� ���� ����� ��������
	float sn = vz*dx - vx*dz;
	if(dl*vl > 0.0f) sn /= sqrtf(dl*vl);
	//���� ����� �����, �� ������������ ������� �� ������ �������
	float cs = vx*dx + vz*dz;
	if(cs < 0.0f)
	{
		if(sn < 0.0f) sn = -1; else sn = 1.0f;
	}
	//������� ��������
	turnspd += 2.0f*WDM_SHIP_TSPEED*sn*dltTime;
	//���������� ���������
	if(cs >= 0.0f)
	{
		speed += kMaxSpeed*WDM_SHIP_INER_ST*WDM_SHIP_SPEED*dltTime;
	}else{		
		if(speed > 0.1f)
		{
			speed -= kMaxSpeed*WDM_SHIP_INER_ST*WDM_SHIP_SPEED*dltTime;
			if(speed < 0.1f) speed = 0.1f;
		}
	}
	speed *= slowingAlfa;
	turnspd *= slowingAlfa;
}

//�������� �� ����������
bool WdmEnemyShip::KillTest()
{
	return !isLive;
}

//���������� ����������� ������
void WdmEnemyShip::UpdateSaveData()
{
	if(!saveAttribute) return;
	saveAttribute->SetAttributeUseFloat("brnAlpha", brnAlpha);
	saveAttribute->SetAttributeUseFloat("deleteAlpha", deleteAlpha);
	saveAttribute->SetAttributeUseFloat("liveTime", liveTime);
	saveAttribute->SetAttributeUseFloat("x", mtx.Pos().x);
	saveAttribute->SetAttributeUseFloat("y", mtx.Pos().y);
	saveAttribute->SetAttributeUseFloat("z", mtx.Pos().z);
	saveAttribute->SetAttributeUseFloat("ax", ax);
	saveAttribute->SetAttributeUseFloat("ay", ay);
	saveAttribute->SetAttributeUseFloat("az", az);
	saveAttribute->SetAttributeUseFloat("dltAx", dltAx);
	saveAttribute->SetAttributeUseFloat("dltAz", dltAz);
	saveAttribute->SetAttributeUseFloat("speed", speed);
	saveAttribute->SetAttributeUseFloat("turnspd", turnspd);
	saveAttribute->SetAttributeUseFloat("kMaxSpeed", kMaxSpeed);
	saveAttribute->SetAttributeUseFloat("rspeed_x", rspeed.x);
	saveAttribute->SetAttributeUseFloat("rspeed_y", rspeed.y);
	saveAttribute->SetAttributeUseFloat("rspeed_z", rspeed.z);
	saveAttribute->SetAttributeUseFloat("slope", slope);
	saveAttribute->SetAttributeUseDword("enc_id", type);
	saveAttribute->SetAttributeUseDword("isEnableKill", isEnableKill);
	saveAttribute->SetAttributeUseDword("isEnemy", isEnemy);	
	saveAttribute->SetAttributeUseDword("killMe", killMe);
	saveAttribute->SetAttributeUseFloat("canSkip", canSkip);
	saveAttribute->SetAttribute("sec", wdmObjects->attrSec);
	saveAttribute->SetAttribute("min", wdmObjects->attrMin);
	saveAttribute->SetAttribute("hour", wdmObjects->attrHour);
	saveAttribute->SetAttribute("day", wdmObjects->attrDay);
	saveAttribute->SetAttribute("month", wdmObjects->attrMonth);
	saveAttribute->SetAttribute("year", wdmObjects->attrYear);
}

void WdmEnemyShip::SetSaveAttribute(ATTRIBUTES * save)
{
	saveAttribute = save;
	if(!saveAttribute) return;
	brnAlpha = saveAttribute->GetAttributeAsFloat("brnAlpha", brnAlpha);
	deleteAlpha = saveAttribute->GetAttributeAsFloat("deleteAlpha", deleteAlpha);
	liveTime = liveTime = saveAttribute->GetAttributeAsFloat("liveTime", liveTime);
	mtx.Pos().x = saveAttribute->GetAttributeAsFloat("x", mtx.Pos().x);
	mtx.Pos().y = saveAttribute->GetAttributeAsFloat("y", mtx.Pos().y);
	mtx.Pos().z = saveAttribute->GetAttributeAsFloat("z", mtx.Pos().z);
	ax = saveAttribute->GetAttributeAsFloat("ax", ax);
	ay = saveAttribute->GetAttributeAsFloat("ay", ay);
	az = saveAttribute->GetAttributeAsFloat("az", az);
	dltAx = saveAttribute->GetAttributeAsFloat("dltAx", dltAx);
	dltAz = saveAttribute->GetAttributeAsFloat("dltAz", dltAz);
	speed = saveAttribute->GetAttributeAsFloat("speed", speed);
	turnspd = saveAttribute->GetAttributeAsFloat("turnspd", turnspd);
	kMaxSpeed = saveAttribute->GetAttributeAsFloat("kMaxSpeed", kMaxSpeed);
	rspeed.x = saveAttribute->GetAttributeAsFloat("rspeed_x", rspeed.x);
	rspeed.y = saveAttribute->GetAttributeAsFloat("rspeed_y", rspeed.y);
	rspeed.z = saveAttribute->GetAttributeAsFloat("rspeed_z", rspeed.z);
	slope = saveAttribute->GetAttributeAsFloat("slope", slope);
	type = saveAttribute->GetAttributeAsDword("enc_id", type);
	isEnableKill = saveAttribute->GetAttributeAsDword("isEnableKill", isEnableKill) != 0;
	isEnemy = saveAttribute->GetAttributeAsDword("isEnemy", isEnemy) != 0;
	killMe = saveAttribute->GetAttributeAsDword("killMe", killMe) != 0;
	canSkip = saveAttribute->GetAttributeAsDword("canSkip", canSkip) != 0;
	if(killMe)
	{
		deleteAlpha = 0.001f;
	}
	UpdateSaveData();
}

void WdmEnemyShip::DeleteUpdate()
{
	if(!saveAttribute) return;
	const char * pnt = saveAttribute->GetAttribute("needDelete");
	if(pnt)
	{
		isEnableKill = true;
		deleteAlpha = 0.999999f;
		saveAttribute = 0;
	}
}

//����� ������� ��� ������� ������������ ������
bool WdmEnemyShip::GeneratePosition(float objRadius, float brnDltAng, float & x, float & z)
{
	//������� ������
	if(!wdmObjects->playerShip) return false;
	float psx = wdmObjects->playerShip->mtx.Pos().x;
	float psz = wdmObjects->playerShip->mtx.Pos().z;
	float psay = ((WdmEnemyShip *)wdmObjects->playerShip)->ay;
	//���� ��������� ���������
	byte field[32];
	for(long i = 0; i < 32; i++) field[i] = 0;
	//������� �����������
	while(true)
	{
		//��������� ���
		long ang = rand() & 31;
		if(field[ang] != 0xff)
		{
			float angle = psay + brnDltAng*(0.5f - ang/31.0f);
			//���������� ������
			for(long rad = rand() & 7; field[ang] & (1 << rad); rad = rand() & 7);
			float radius = wdmObjects->enemyshipBrnDistMin + (wdmObjects->enemyshipBrnDistMax - wdmObjects->enemyshipBrnDistMin)*rad/7.0f;
			//����������
			x = psx + radius*sinf(angle);
			z = psz + radius*cosf(angle);
			//�������� �� ����������� ��������� � ��������� �����
			if(CheckPosition(x, z, objRadius)) break;
			field[ang] |= 1 << rad;
		}
		//�������� �� ����������� �����������
		for(i = 0; i < 32 && field[i] == 0xff; i++);
		if(i == 32) return false;
	}
	return true;
}

//���������� ����� �����
void WdmEnemyShip::SetLiveTime(float time)
{
	if(time < 1.0f) time = 1.0f;
	liveTime = time;
}

//�������� ��� ��������
const char * WdmEnemyShip::GetAttributeName()
{
	static char * empty = "";
	if(saveAttribute)
	{
		return saveAttribute->GetThisName();
	}
	return empty;
}