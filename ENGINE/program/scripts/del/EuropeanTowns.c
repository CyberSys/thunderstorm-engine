
void SetNames()
{
	for(int i = 1; i <MAX_CHARACTERS; i++)
	{
		if (!CheckAttribute(&Characters[i], "name") || Characters[i].name == "none")
		{
			SetRandomNameToCharacter(&Characters[i]);
		}
	}
}

void InitTowns()
{	
	Towns[0].nation = FRANCE;
	Towns[0].name = "�����";
	
	Towns[1].nation = FRANCE;
	Towns[1].name = "������";
	
	Towns[2].nation = FRANCE;
	Towns[2].name = "���";
	
	Towns[3].nation = FRANCE;
	Towns[3].name = "�����";
	
	Towns[4].nation = FRANCE;
	Towns[4].name = "�����";
	
	Towns[5].nation = FRANCE;
	Towns[5].name = "�����";
	
	Towns[6].nation = FRANCE;
	Towns[6].name = "������";
	
	Towns[7].nation = FRANCE;
	Towns[7].name = "�� ������";
	
	Towns[8].nation = FRANCE;
	Towns[8].name = "����";
	
	Towns[9].nation = FRANCE;
	Towns[9].name = "��-����";
	
	Towns[10].nation = ENGLAND;
	Towns[10].name = "������";
	
	Towns[11].nation = ENGLAND;
	Towns[11].name = "����";
	
	Towns[12].nation = ENGLAND;
	Towns[12].name = "������";
	
	Towns[13].nation = ENGLAND;
	Towns[13].name = "�������";
	
	Towns[14].nation = ENGLAND;
	Towns[14].name = "��������";
	
	Towns[15].nation = ENGLAND;
	Towns[15].name = "�������";
	
	Towns[16].nation = ENGLAND;
	Towns[16].name = "�������";
	
	Towns[17].nation = ENGLAND;
	Towns[17].name = "�����";
	
	Towns[18].nation = ENGLAND;
	Towns[18].name = "�������";
	
	Towns[19].nation = ENGLAND;
	Towns[19].name = "�������";
	
	Towns[20].nation = SPAIN;
	Towns[20].name = "������";
	
	Towns[21].nation = SPAIN;
	Towns[21].name = "������";
	
	Towns[22].nation = SPAIN;
	Towns[22].name = "���������";
	
	Towns[23].nation = SPAIN;
	Towns[23].name = "�����";
	
	Towns[24].nation = SPAIN;
	Towns[24].name = "������";
	
	Towns[25].nation = SPAIN;
	Towns[25].name = "�������";
	
	Towns[26].nation = SPAIN;
	Towns[26].name = "��������";
	
	Towns[27].nation = SPAIN;
	Towns[27].name = "�������";
	
	Towns[28].nation = SPAIN;
	Towns[28].name = "��������";
	
	Towns[29].nation = SPAIN;
	Towns[29].name = "���������";
		
	Towns[30].nation = HOLLAND;
	Towns[30].name = "��������";
	
	Towns[31].nation = HOLLAND;
	Towns[31].name = "�����";
	
	Towns[32].nation = HOLLAND;
	Towns[32].name = "������";
	
	Towns[33].nation = HOLLAND;
	Towns[33].name = "��������";
	
	Towns[34].nation = HOLLAND;
	Towns[34].name = "�����";
	
	Towns[35].nation = HOLLAND;
	Towns[35].name = "�������";
	
	Towns[36].nation = HOLLAND;
	Towns[36].name = "�����";
	
	Towns[37].nation = HOLLAND;
	Towns[37].name = "�������";
	
	Towns[38].nation = HOLLAND;
	Towns[38].name = "�����";
	
	Towns[39].nation = HOLLAND;
	Towns[39].name = "������";
}
