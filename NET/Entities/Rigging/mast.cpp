#include <stdio.h>
#include "mast.h"

#define DELTA_TIME(x)			((x) * 0.001f)
#define DELTA_TIME_ROTATE(x)	((x) * 0.01f)

const char* MAST_INI_FILE="resource\\ini\\mast.ini";

float MAST_MOVE_STEP=0.2f;
float MAST_FALL_STEP=.05f;
float MAST_MAX_FALL_SPEED=.5f;
float MAX_FALL_ANGLE=PI;
float YROTATE_STEP=0.1f;
float TRACE_HEIGHT=40.f;
float TRACE_ADDING=.01f;
float TRACE_SLIDING=0.1f;
float MAST_WIDTH=0.15f;
float MINZ_COMPARE=1.f;
float MAX_CHANGE_LENGTH=.8f;
float MAX_SLIDING_LENGHT=1.2f;
float DESTRUCT_MAST_DEEP=20.f;
float DEEP_FALL_STEP=3.f;
int   MAX_MOVE_CICLES=50;
int   MIN_MOV_COUNTER=5;
// ���� ��������(�������) �����
float MIN_X_DANG=-0.01f;
float VAR_X_DANG=0.02f;
float MIN_Z_DANG=0.07f;
float VAR_Z_DANG=0.03f;
float MIN_SIGNIFICANT=0.1f;


NetMast::NetMast()
{
	RenderService = 0;
    wMoveCounter=0;
    bUse=false;
}

NetMast::~NetMast()
{
    AllRelease();
}

bool NetMast::Init()
{
	GUARD(NetMast::Init())

	SetDevice();

	UNGUARD
	return true;
}
 
void NetMast::SetDevice()
{
	GUARD(NetMast::SetDevice())

	RenderService = (VDX8RENDER *)_CORE_API->CreateService("dx8render");
    if(!RenderService) _THROW("No service: dx8render");

	pCollide = (COLLIDE*)_CORE_API->CreateService("COLL");
	if (!pCollide) _THROW("No service: collide");

    LoadIni();

	UNGUARD
}

bool NetMast::CreateState(ENTITY_STATE_GEN * state_gen)
{
	GUARD(bool NetMast::CreateState(ENTITY_STATE_GEN * state_gen))

	return true;
	UNGUARD
}

bool NetMast::LoadState(ENTITY_STATE * state)
{
	GUARD(bool NetMast::LoadState(ENTITY_STATE * state))

	SetDevice();

	UNGUARD
	return true;
}

void NetMast::Execute(dword Delta_Time)
{
    GUARD(void NetMast::Execute(dword Delta_Time))

    if(bUse)
    {
        //====================================================
        // ���� ��� ������� ���-����, �� ������� ���� �� ����
        WIN32_FIND_DATA	wfd;
	    HANDLE h = _CORE_API->fio->_FindFirstFile(MAST_INI_FILE,&wfd);
        if (INVALID_HANDLE_VALUE != h)
	    {
		    FILETIME ft_new = wfd.ftLastWriteTime;
    	    _CORE_API->fio->_FindClose(h);

	        if (CompareFileTime(&ft_old,&ft_new)!=0)
            {
                LoadIni();
            }
	    }
        doMove(Delta_Time);
        MODEL* mdl=(MODEL*)_CORE_API->GetEntityPointer(&model_id);
        if(mdl) mdl->Update();
    }
    else
    {
        _CORE_API->DeleteEntity(GetID());
    }
    UNGUARD
}

#define D3DLXLINEVERTEX_FORMAT		(D3DFVF_DIFFUSE|D3DFVF_XYZ|D3DFVF_TEX0)
void NetMast::Realize(dword Delta_Time)
{
	GUARD(void NetMast::Realize(dword Delta_Time))

    MODEL *mdl;
    if( (mdl=(MODEL*)_CORE_API->GetEntityPointer(&model_id))!=0 )
    {
		RenderService->SetRenderState(D3DRS_LIGHTING,true);
        mdl->Realize(Delta_Time);
		RenderService->SetRenderState(D3DRS_LIGHTING,false);

/*        CVECTOR bp=mdl->mtx*mm.bp;
        CVECTOR ep=mdl->mtx*mm.ep;
        CVECTOR brey=mdl->mtx*mm.brey;
        CVECTOR erey=mdl->mtx*mm.erey;

        CMatrix mtx;
        mtx.SetIdentity();
        RenderService->SetTransform(D3DTS_WORLD,(D3DXMATRIX*)&mtx);

        struct LINEVERTEX
        {
            CVECTOR pos;
            float   col;
        };
        LINEVERTEX pVerts[4];
        pVerts[0].pos=bp; pVerts[0].col=0xffffff;
        pVerts[1].pos=ep; pVerts[1].col=0xffffff;
        pVerts[2].pos=brey; pVerts[2].col=0xffffff;
        pVerts[3].pos=erey; pVerts[3].col=0xffffff;
        RenderService->DrawPrimitiveUP(D3DPT_LINELIST, D3DLXLINEVERTEX_FORMAT, 2, pVerts, sizeof(LINEVERTEX));*/
    }

	UNGUARD
}

dword _cdecl NetMast::ProcessMessage(MESSAGE & message)
{
	GUARD(dword _cdecl NetMast::ProcessMessage(MESSAGE message))

	switch (message.Long())
	{
    case MSG_MAST_SETGEOMETRY:
		{
			NODE * newRootNode = (NODE*)message.Pointer(); // new root node pointer
			ENTITY_ID shipEI = message.EntityID(); // ship entity
			ENTITY_ID oldModelEI = message.EntityID(); // old model entity
			Mount( oldModelEI, shipEI, newRootNode );
		}
        break;
	}

	UNGUARD
	return 0;
}

#define ADD_MINIMUM .01f
void _cdecl NetMast::Mount( ENTITY_ID modelEI, ENTITY_ID shipEI, NODE* mastNodePointer )
{
	if(mastNodePointer==NULL) return;
    MODEL * oldmdl=(MODEL*)_CORE_API->GetEntityPointer(&modelEI);
    if(oldmdl==0) return; // ������ �� �����, ���� ��� ������ ������
    oldmodel_id=modelEI;
    ship_id=shipEI;

    ENTITY_ID ropeEI; bool bRope;
    bRope = NetFindClass(IsServer(), &ropeEI, "NetRope");
    ENTITY_ID sailEI; bool bSail;
    bSail = NetFindClass(IsServer(), &sailEI, "NetSail");
    ENTITY_ID flagEI; bool bFlag;
    bFlag = NetFindClass(IsServer(), &flagEI, "NetFlag");
    ENTITY_ID vantEI; bool bVant;
    bVant = NetFindClass(IsServer(), &vantEI, "NetVant");

	// ������ ���������
	VAI_OBJBASE * pVAI = NULL;	pVAI = (VAI_OBJBASE*)api->GetEntityPointer(&shipEI);
	ATTRIBUTES * pA = NULL;	if(pVAI!=NULL) pA = pVAI->GetACharacter();

	ATTRIBUTES * pAMasts = NULL; if(pA!=NULL) pAMasts = pA->FindAClass(pA,"Ship.Masts");
	float fMastDamage = 0.f;
	if(pAMasts!=NULL)
		fMastDamage = pAMasts->GetAttributeAsFloat((char*)mastNodePointer->GetName(),0.f);
	long chrIdx = -1;
	if(pA!=NULL)	chrIdx = pA->GetAttributeAsDword("index",-1);
	api->Event("EventMastFall", "lsl", chrIdx, mastNodePointer->GetName(), fMastDamage<1.f);
	if(fMastDamage<1.f)
	{
		if(bSail) api->Send_Message(sailEI, "lls", MSG_SAIL_MAST_PROCESSING, chrIdx, mastNodePointer->GetName());
	}

    if(mastNodePointer!=null)
    {
        int i,j;

        // �������� ����� ������
        bModel=true; model_id = mastNodePointer->Unlink2Model();

        // ������� �� ���� �������� ������ ����� � �������� ��
		if(bVant) api->Send_Message(vantEI, "lip", MSG_VANT_DEL_MAST, modelEI, mastNodePointer);
		MODEL * mdl = (MODEL*)_CORE_API->GetEntityPointer(&model_id);
		if(mdl!=NULL)	for(i=0; i<10000; i++)
		{
			NODE* nod=mdl->GetNode(i);
			if(nod==NULL || nod->geo==NULL) break;
			GEOS::INFO gi; nod->geo->GetInfo(gi);
			for(j=0; j<gi.nlabels; j++)
			{
				GEOS::LABEL gl; nod->geo->GetLabel(j,gl);
				if(!strncmp(gl.name,"rope",4))
				{
					if(bRope)
						api->Send_Message(ropeEI,"lil",MSG_ROPE_DELETE,modelEI,atoi(&gl.name[5]));
				}
				else if(!strncmp(gl.name,"sail",4))
				{
					if(bSail)
						api->Send_Message(sailEI,"liplii",MSG_SAIL_TO_NEWHOST,modelEI,nod,atoi(&gl.group_name[5]), GetID(),model_id);
				}
				else if(!strncmp(gl.group_name,"flag",4))
				{
					if(bFlag)
						api->Send_Message(flagEI,"lili",MSG_FLAG_TO_NEWHOST,modelEI,atoi(&gl.group_name[4]),model_id);
				}
			}
			// ����� ����� ������ ��������� � ������ ������
			if(bSail)
			{
				_CORE_API->Send_Message(sailEI,"liii",MSG_SAIL_CHECK,shipEI,GetID(),model_id);
				_CORE_API->Send_Message(sailEI,"li",MSG_SAIL_FREE_GROUP,GetID());
			}
		}
		if(bSail) api->Send_Message(sailEI, "ll", MSG_SAIL_MAST_PROCESSING, -1);

        // ��������� �������������� ��������� �������� �����
        SHIP_BASE *sb; sb=(SHIP_BASE*)_CORE_API->GetEntityPointer(&shipEI);
        if(sb)
        {
            mm.ang= sb->State.vAng;

            mm.dmov.y = 0.f;
		    float k = KNOTS2METERS(sb->State.vSpeed.z);
		    mm.dmov.z= k * cosf(sb->State.vAng.y);
		    mm.dmov.x= k * sinf(sb->State.vAng.y);
		    k = KNOTS2METERS(sb->State.vSpeed.x);
		    mm.dmov.z += k * cosf(sb->State.vAng.y+PId2);
		    mm.dmov.x += k * sinf(sb->State.vAng.y+PId2);

            mm.sdmov = mm.dmov;
        }
        else
        {
            mm.ang=CVECTOR(0.f,0.f,0.f);
            mm.dmov=mm.sdmov= CVECTOR(0.f,0.f,0.f);
        }
        mm.mov = mastNodePointer->glob_mtx.Pos();
        mm.dang = CVECTOR(MIN_X_DANG + VAR_X_DANG*(float)rand()/(float)RAND_MAX,
                          0.f, MIN_Z_DANG + VAR_Z_DANG*(float)rand()/(float)RAND_MAX);
        // ������ ��������� �������
        ENTITY_ID tmpEI;
        float minDist=10000.f;
        SHIP_BASE *minDstShip;
        if (NetFindClass(IsServer(), &tmpEI, "NetShip")) do
        {
            if(tmpEI==ship_id) continue;
            SHIP_BASE *sb = (SHIP_BASE*)_CORE_API->GetEntityPointer(&tmpEI);
            float tmpDist = ~(sb->State.vPos-mm.mov);
            if(tmpDist<minDist)
            {
                minDist = tmpDist;
                minDstShip = sb;
            }
        } while(NetFindClassNext(IsServer(), &tmpEI));
        if(minDist<4000.f) // ���� ��������� ������� ������ � ���, �� ����� ����� � ��������������� �������
        {
            CVECTOR vect;
            mastNodePointer->glob_mtx.MulToInvNorm(minDstShip->State.vPos-mm.mov,vect);
            if(vect.x<0.f) mm.dang.z=-mm.dang.z;
        }
        else if(rand()&1) mm.dang.z=-mm.dang.z;

        if(mm.ang.x>MIN_SIGNIFICANT)
            if(mm.dang.x<0.f) mm.dang.x=-mm.dang.x;
        if(mm.ang.x<-MIN_SIGNIFICANT)
            if(mm.dang.x>0.f) mm.dang.x=-mm.dang.x;
        if(mm.ang.z>MIN_SIGNIFICANT)
            if(mm.dang.z<0.f) mm.dang.z=-mm.dang.z;
        if(mm.ang.z<-MIN_SIGNIFICANT)
            if(mm.dang.z>0.f) mm.dang.z=-mm.dang.z;

        mm.sdang=mm.dang;

        GEOS::INFO gi;
        // ��������� ������� � ������ ����� �����
        mastNodePointer->geo->GetInfo(gi);
        mm.ep=mm.bp = *(CVECTOR*)&gi.boxcenter;
        mm.bp.y-=gi.boxsize.y*.5f;
        mm.ep.y+=gi.boxsize.y*.5f;
        if(gi.boxsize.z>MINZ_COMPARE)
        {
            mm.bp.z-=gi.boxsize.z*.5f;
            mm.ep.z+=gi.boxsize.z*.5f;
        }
        // �������� ��������� ����� �����, � ���� ��� �������� � �������, ��
        // ���������� �� �� ����� ��������������� � ��������
        CVECTOR bv = mastNodePointer->glob_mtx*mm.bp;
        CVECTOR ev = mastNodePointer->glob_mtx*mm.ep;
        // ������� ��������� �������
        mastNodePointer->loc_mtx.SetIdentity();
        float tmpTrace = oldmdl->Trace(ev,bv);
        if(tmpTrace<=1.f)
        {
            bv = ev + (bv-ev)*tmpTrace;
            mastNodePointer->glob_mtx.MulToInv(bv,mm.bp);
        }

        // ��������� ����� � ������ ����� ���
        for(i=0; i<mastNodePointer->nnext; i++)
            if(!strncmp(mastNodePointer->next[i]->GetName(),"rey",3))
            {
                mastNodePointer->next[i]->geo->GetInfo(gi);
                mm.brey=mm.erey = *(CVECTOR*)&gi.boxcenter;
                mm.brey.x-=gi.boxsize.x*.5f;
                mm.erey.x+=gi.boxsize.x*.5f;
                if(gi.boxsize.z>MINZ_COMPARE)
                {
                    mm.brey.z-=gi.boxsize.z*.5f;
                    mm.erey.z+=gi.boxsize.z*.5f;
                }
                break;
            }
        if(i==mastNodePointer->nnext)
            mm.brey=mm.erey = CVECTOR(0.f,0.f,0.f);
        else
        {
            mm.brey= mastNodePointer->next[i]->loc_mtx*mm.brey;
            mm.erey= mastNodePointer->next[i]->loc_mtx*mm.erey;
        }

        bUse=true;
        bFallUnderWater=false;
    }
}

void NetMast::LoadIni()
{
    GUARD(NetMast::LoadIni());
	char	section[256];

	INIFILE * ini;
	WIN32_FIND_DATA	wfd;
	HANDLE h = _CORE_API->fio->_FindFirstFile(MAST_INI_FILE,&wfd);
	if (INVALID_HANDLE_VALUE != h) 
	{
		ft_old = wfd.ftLastWriteTime;
		_CORE_API->fio->_FindClose(h);
	}
	ini = _CORE_API->fio->OpenIniFile((char*)MAST_INI_FILE);
	if(!ini) THROW("mast.ini file not found!");

	sprintf(section,"MAST");

  /*=========================================================================
 =============================================================================
=======   �������� ����������   ===============================================
==============================================================================*/
    // ��� �������� ����� ��� ��������� ������ ����� � ����
    MAST_MOVE_STEP= ini->GetFloat(section,"water_slide_step",MAST_MOVE_STEP);
    // ��������� ���� ���������� ������� ���� ����� ����
    MAST_FALL_STEP= ini->GetFloat(section,"downfall_acceleration",MAST_FALL_STEP);
    // ������������ ��� ������� ����� ����
    MAST_MAX_FALL_SPEED= ini->GetFloat(section,"downfall_maxspeed",MAST_MAX_FALL_SPEED);
    // ������������ ���� �������� �����
    MAX_FALL_ANGLE= ini->GetFloat(section,"fMaxAngle",MAX_FALL_ANGLE);
    // ��� �������� ����� ������ ��� Y (��� ������������ ��� � ��������)
    YROTATE_STEP= ini->GetFloat(section,"fYRotateStep",YROTATE_STEP);
    // ������ ���� ����������� ��� ��������
    TRACE_HEIGHT= ini->GetFloat(section,"fTraceHeight",TRACE_HEIGHT);
    // �������������� ������� �� ������ � ����� �������� ��� ���������� ��������� ��������
    TRACE_ADDING= ini->GetFloat(section,"fTraceHeightAdding",TRACE_ADDING);
    // ��� ������ ����� �� �����������
    TRACE_SLIDING= ini->GetFloat(section,"fTraceSliding",TRACE_SLIDING);
    // ������������ ��������� ��������� ��� ����� ��������� ������ ������� �����
    MAX_CHANGE_LENGTH= ini->GetFloat(section,"fMaxPosChange",MAX_CHANGE_LENGTH);
    // ������������ ��������� ��������� ��� ���������� �����
    MAX_SLIDING_LENGHT= ini->GetFloat(section,"fMaxSlideLenght",MAX_SLIDING_LENGHT);
    // ����� �������� �������� ���� �� ������� �����
    MAST_WIDTH= ini->GetFloat(section,"fMastWidth",MAST_WIDTH);
    // ����������� �������� Z � ������� NODE ��� ����� ��� � �������� ������ ����� �����������
    MINZ_COMPARE= ini->GetFloat(section,"fMinZCompare",MINZ_COMPARE);
    // ������� � ������� ����� �����������
    DESTRUCT_MAST_DEEP= ini->GetFloat(section,"fMaxDeep",DESTRUCT_MAST_DEEP);
    // ��� ��������� ����� � �������
    DEEP_FALL_STEP= ini->GetFloat(section,"fDeepStep",DEEP_FALL_STEP);
    // ����� ������ �������� ����� �������� ����������� �������� � ���������
    MAX_MOVE_CICLES= ini->GetLong(section,"maxMoveCicles",MAX_MOVE_CICLES);
    // ����� ������ �������� ����� �������� ���������� ������� �����
    MIN_MOV_COUNTER= ini->GetLong(section,"minMoveCicles",MIN_MOV_COUNTER);
    // ����������� ���� �������� ����� �� X
    MIN_X_DANG = ini->GetFloat(section,"fMinXdang",MIN_X_DANG);
    // ������ ��������� ���� �������� ����� �� X
    VAR_X_DANG = ini->GetFloat(section,"fVarXdang",VAR_X_DANG);
    // ����������� ���� �������� ����� �� Z
    MIN_Z_DANG = ini->GetFloat(section,"fMinZdang",MIN_Z_DANG);
    // ������ ��������� ���� �������� ����� �� Z
    VAR_Z_DANG = ini->GetFloat(section,"fVarZdang",VAR_Z_DANG);
    // ���� ��������� ������� ������� � �������� ����� ������ �� ����� ����
    MIN_SIGNIFICANT= ini->GetFloat(section,"fAngSignificant",MIN_SIGNIFICANT);

    delete ini;
    UNGUARD
}

void NetMast::doMove(dword DeltaTime)
{
    if(wMoveCounter<=MAX_MOVE_CICLES) wMoveCounter++;

    float dtime=DELTA_TIME((float)DeltaTime);
    float rtime=DELTA_TIME_ROTATE((float)DeltaTime);

    MODEL* mdl = (MODEL*)api->GetEntityPointer(&model_id); // ��� ������ ��������� �����
    if(mdl!=0)
    {
        if(bFallUnderWater) // ���� ����� ��� �����
        {
			// ����� �� �������, ��� ���������� �����
            if( mdl->mtx.Pos().y < -DESTRUCT_MAST_DEEP )	bUse=false;
			// �������� ����� ���� � ������ ����
            else	mdl->mtx.Move( 0.f, -DEEP_FALL_STEP*dtime, 0.f );
        }
        else // ����� �� �����... ���� ������
        {
            mm.mov += mm.dmov*dtime;
			// ���� ��� �� ��������� ����� �������������, �� ���������� ���������
            if( mm.ang.z<MAX_FALL_ANGLE && mm.ang.z>-MAX_FALL_ANGLE &&
                mm.ang.x<MAX_FALL_ANGLE && mm.ang.x>-MAX_FALL_ANGLE )
                mm.ang += mm.dang*rtime;

			// ��������� ���������� ������� ��� ������ ��������� �����
            CMatrix mtx;
            mtx.BuildMatrix(mm.ang);
            mtx.SetPosition(mm.mov);
            mtx=mdl->GetNode(0)->loc_mtx*mtx;

            CVECTOR bp; // ���������� �������� ����� �����
            CVECTOR ep; // ���������� ������� ����� �����
            CVECTOR lp; // ���������� ����� ����� ��� (���������)
            CVECTOR rp; // ���������� ������ ����� ��� (���������)
            bool bNextClass = (wMoveCounter <= MAX_MOVE_CICLES); // ��������� ��������, ������������ ����� ���
            bool bStopRotate = false; // �� ��������� �� ������������� �������� ����� ��� �������
            ENTITY_ID modEI,findEI;
            bp = mtx*mm.bp;
            ep = mtx*mm.ep;
            lp = mtx*mm.brey;
            rp = mtx*mm.erey;
            // �������� �������� ������� �����
            if(bp.y>0.f && ep.y>0.f && wMoveCounter>MIN_MOV_COUNTER) mm.dmov.y -= MAST_FALL_STEP; // ������ �������
            if(mm.dmov.y < -MAST_MAX_FALL_SPEED) mm.dmov.y = -MAST_MAX_FALL_SPEED; // ����������� �� ������������ �������� �������
            // ������������ ����� � ��������������� ������ �������� �����
            bp.y-=MAST_WIDTH; ep.y-=MAST_WIDTH; lp.y-=MAST_WIDTH; rp.y-=MAST_WIDTH;
            while(bNextClass)
            {
                bNextClass=false;
				// �������� � ��������
                if( NetFindClass(IsServer(), &findEI, "NetIsland") && api->GetEntityPointer(&findEI) != NULL )
				{
					modEI = ((ISLAND_BASE*)api->GetEntityPointer(&findEI))->GetModelEID();

                    CVECTOR dp;
                    int tmp;
                    float yAng;
                    if((tmp=GetSlide(modEI,bp,ep,dp, lp,rp,yAng))!=0)
                    {
                        if(tmp&SR_STOPROTATE) bStopRotate=true;
                        if(tmp&SR_YROTATE) mdl->GetNode(0)->loc_mtx.RotateY(yAng);
                        if(tmp&SR_MOVE)
                        {
                            mm.mov+=dp;
                            mm.dmov+=dp;
                            mtx.SetPosition(mm.mov);
                            bp+=dp; ep+=dp; lp+=dp; rp+=dp;
                            bNextClass=true;
                            break;
                        }
                    }
				}
				// �������� � ��������
				if(NetFindClass(IsServer(), &findEI, "NetShip"))	do
                {
					if( api->GetEntityPointer(&findEI)==NULL ) continue;
					modEI = ((VAI_OBJBASE*)api->GetEntityPointer(&findEI))->GetModelEID();
                    CVECTOR dp;
                    int tmp;
                    float yAng;
                    if((tmp=GetSlide(modEI,bp,ep,dp, lp,rp,yAng))!=0)
                    {
                        if(tmp&SR_STOPROTATE) bStopRotate=true;
                        if(tmp&SR_YROTATE) mdl->GetNode(0)->loc_mtx.RotateY(yAng);
                        if(tmp&SR_MOVE)
                        {
                            mm.mov+=dp;
                            mm.dmov+=dp;
                            mtx.SetPosition(mm.mov);
                            bp+=dp; ep+=dp; lp+=dp; rp+=dp;
                            bNextClass=true;
                            break;
                        }
                    }
                }while(NetFindClassNext(IsServer(), &findEI));
            }

            if(bp.y<=-MAST_WIDTH || ep.y<=-MAST_WIDTH)
            {
                mm.dang=0.2f*mm.sdang;
                CVECTOR dm;
                if(bp.y<=0.f)
                    dm=!(bp-ep);
                else
                    dm=!(ep-bp);
                mm.dmov=dm*MAST_MOVE_STEP;
            }
            else mm.dang=mm.sdang;

            if(bStopRotate) mm.dang=CVECTOR(0.f,0.f,0.f);

            if(bp.y<0.f && ep.y<0.f)
                bFallUnderWater=true;
            else
                mdl->mtx=mtx;
        }
    }
}

int NetMast::GetSlide(ENTITY_ID &mod, CVECTOR &pbeg, CVECTOR &pend, CVECTOR &dp, CVECTOR &lrey, CVECTOR &rrey, float &angl)
{
    int retVal=0;

    // �������� ���
    CVECTOR vl=lrey;
    CVECTOR vr=rrey;
    CVECTOR vcentr=(vl+vr)*.5f;
    float ang=0.f;
    float lf=pCollide->Trace(mod,vl,vcentr);
    float rf=pCollide->Trace(mod,vr,vcentr);

    if( (lf<=1.f && rf>1.f) || (lf>1.f && rf<=1.f) )
    {
        if(lf>1.f)
            angl=-YROTATE_STEP;
        else
            angl=YROTATE_STEP;
        retVal|=SR_YROTATE;
    }

    // �������� �����
    float tmp;
    float hVal=0.f;
    float sVal=0.f;
    CVECTOR vb=pbeg;
    CVECTOR ve=pend;
    dp=CVECTOR(0.f,0.f,0.f);
    if((tmp=pCollide->Trace(mod,ve,vb))<=1.f)
    {
        retVal|=SR_MOVE;
        if(tmp<0.5f) retVal|=SR_STOPROTATE;
        do
        {
            if(hVal>MAX_CHANGE_LENGTH)
            {
                if(sVal>MAX_SLIDING_LENGHT) return (retVal&(~SR_MOVE));
                dp.y-=hVal;  vb.y-=hVal;  ve.y-=hVal;
                hVal=0;
                MODEL* pmdl=(MODEL*)_CORE_API->GetEntityPointer(&mod);
                if(pmdl)
                {
                    NODE* pnod = pmdl->GetCollideNode();
                    if(pnod)
                    {
                        CVECTOR cv = vb+(ve-vb)*tmp - pnod->glob_mtx.Pos();
                        cv.y=0.f;
                        if(cv.x==0.f && cv.z==0.f) cv.y=TRACE_SLIDING;
                        else cv=TRACE_SLIDING*(!cv);

                        sVal+=TRACE_SLIDING;
                        dp+=cv; vb+=cv; ve+=cv;
                    }
                    else return 0;
                }
                else return 0;
            }
            else
            {
                hVal+=TRACE_ADDING;
                dp.y+=TRACE_ADDING;
                vb.y+=TRACE_ADDING;
                ve.y+=TRACE_ADDING;
            }
        } while((tmp=pCollide->Trace(mod,ve,vb))<=1.f);
    }
    else return retVal;

    return retVal;
}

void NetMast::AllRelease()
{
    ENTITY_ID tmp_id;

    // ������� ������ �������
    if(NetFindClass(IsServer(), &tmp_id, "NetSail"))
        _CORE_API->Send_Message(tmp_id,"li",MSG_SAIL_DEL_GROUP,GetID());

    // ������� ������ ������
    if(NetFindClass(IsServer(), &tmp_id, "NetFlag"))
        _CORE_API->Send_Message(tmp_id,"li",MSG_FLAG_DEL_GROUP,model_id);

    // ������� ������
    _CORE_API->DeleteEntity(model_id);
}
