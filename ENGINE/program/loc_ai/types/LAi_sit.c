/*
���: �������, ������ �����, �������� �� �������, ������� �� ������

	������������ �������:
		stay
		dialog
*/



#define LAI_TYPE_SIT		"sit"


//�������������
void LAi_type_sit_Init(aref chr)
{
	DeleteAttribute(chr, "location.follower");
	DeleteAttribute(chr, "chr_ai.type");
	chr.chr_ai.type = LAI_TYPE_SIT;
	chr.chr_ai.type.wait = 1.0;
	LAi_tmpl_stay_InitTemplate(chr);
	//����� �������� ��� ������������� � �������
	chr.TiedItems.itm1.model = "HandsItems\meet";
	chr.TiedItems.itm1.locator = "Saber_hand";	
	chr.TiedItems.itm2.model = "HandsItems\cup";
	chr.TiedItems.itm2.locator = "Saber_hand";
	//��������� �������� ���������
	LAi_SetDefaultSitAnimation(chr);
	SendMessage(&chr, "lsl", MSG_CHARACTER_EX_MSG, "SetFightWOWeapon", false);
}

//��������������� ���� ���������
void LAi_type_sit_CharacterUpdate(aref chr, float dltTime)
{
	float time = stf(chr.chr_ai.type.wait) - dltTime; 
	if (time < 0.0)
	{		
		ref chrTarget;
		string snd = "male-sit";
		int iTemp;
		float fAng;
		aref tmpl;
		float xAng = 0;
		float zAng = 0;
		if(CheckAttribute(chr,"sex"))
		{
			if(chr.sex == "woman")
			{
				snd = "female-sit";
			}
		}
		if(rand(300) == 123)
		{
			LAi_CharacterPlaySound(chr, snd);
		}
		if(chr.chr_ai.tmpl != LAI_TMPL_DIALOG)
		{
			time = 20;
			int num = FindNearCharacters(chr, 2.5, -1.0, 0.0, 0.0, false, true);
			for(int i = 0; i < num; i++)
			{
				iTemp = chrFindNearCharacters[i].index;
				chrTarget = &characters[iTemp];
				if(chrTarget.chr_ai.type == LAI_TYPE_SIT && chrFindNearCharacters[i].index != nMainCharacterIndex)
				{
					GetCharacterAy(chr, &fAng);
					//xAng = stf(chrFindNearCharacters[i].dx) * cos(fAng) - stf(chrFindNearCharacters[i].dz) * sin(fAng);
					zAng = stf(chrFindNearCharacters[i].dz) * cos(fAng) + stf(chrFindNearCharacters[i].dx) * sin(fAng);				
					if (zAng > -0.9 && zAng < 0.9)
					{
						if(LAi_Character_CanDialog(chr, chrTarget) && rand(1))
						{				
							if (!LAi_tmpl_SetDialog(chr, chrTarget, 50.0)) return;
							if (!LAi_tmpl_SetDialog(chrTarget, chr, 50.0)) return;
							time = 60.0;
							LAi_CharacterPlaySound(chr, snd + "-who");
							break;
						}
						else time = 30.0;
					}
				}
			}
		}
	}
	chr.chr_ai.type.wait = time;
}

//�������� ��������� � �������
bool LAi_type_sit_CharacterLogin(aref chr)
{
	return true;
}

//�������� ��������� �� �������
bool LAi_type_sit_CharacterLogoff(aref chr)
{
	return true;
}

//���������� ������ ���������
void LAi_type_sit_TemplateComplite(aref chr, string tmpl)
{
	LAi_tmpl_player_InitTemplate(chr);
}

//�������� � ������� ������� ������
void LAi_type_sit_NeedDialog(aref chr, aref by)
{
}

//������ �� ������, ���� ���������� true �� � ���� ������ ����� ������ ������
bool LAi_type_sit_CanDialog(aref chr, aref by)
{
	if(sti(by.index) == nMainCharacterIndex && chr.chr_ai.tmpl == LAI_TMPL_DIALOG)
	{
		if(LAi_tmpl_dialog_StopNPC(chr)) return true;
	}	
	//���� ��� �������, �� �������
	if(chr.chr_ai.tmpl == LAI_TMPL_DIALOG) return false;
	//���������� �� ������
	return true;
}

//������ ������
void LAi_type_sit_StartDialog(aref chr, aref by)
{
	//���� �� �������, ��������� ������ ��� ������� ����������
	LAi_tmpl_stay_InitTemplate(chr);
	LAi_tmpl_SetActivatedDialog(chr, by);
	//LAi_tmpl_dialog_NoAni(chr);
}

//��������� ������
void LAi_type_sit_EndDialog(aref chr, aref by)
{
	LAi_tmpl_stay_InitTemplate(chr);
	chr.chr_ai.type.wait = 15;
}


//�������� ���������
void LAi_type_sit_Fire(aref attack, aref enemy, float kDist, bool isFindedEnemy)
{
}


//�������� ��������
void LAi_type_sit_Attacked(aref chr, aref by)
{
	
}


