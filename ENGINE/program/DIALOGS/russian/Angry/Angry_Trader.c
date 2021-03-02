// 18.12.05 ����, ���� ��������� �� ������
void GeneralAngryDialog(aref _pchar, aref npchar, aref Link)
{
    if (CheckAttribute(npchar, "angry") && !CheckAttribute(npchar, "angry.ok"))
    {
        npchar.angry.ok = 1;
        switch (npchar.angry.kind)
        {
            case "quest":
                Dialog.CurrentNode = "AngryQuest";
            break;
        }
    }
	switch(Dialog.CurrentNode)
	{
		case "AngryQuest":
            dialog.text = RandPhraseSimple("� �� ���� � ����� ������ ����� ����.", "������ � ���� ������ ��������� �� ����.");
			link.l1 = RandPhraseSimple("�� ��� ������. �� ����� ������, ������ ���� �� �����.", "�������� ������...");
		    link.l1.go = "AngryExitAgain";
		    //===>> ����� ��������, ������ ������� �����.
            if (_pchar.questTemp.Ascold == "Seek_powder" && npchar.id == _pchar.questTemp.Ascold.TraderName)
            {
                dialog.text =  "�� ����� ���������, ������ ���� ������� ���� ������������� �����.";
    			link.l1 = "������ ���. � ������ � ���� �����, ���� �� �� � ���� �� ����� ����. � �� �����, ��� ��������� ����� ����.";
    		    link.l1.go = "AngStep_1";
            }
            if (CheckAttribute(npchar, "angry.terms")) //������� �� ������ �������� ����� 10 ����.
            {
                if (GetNpcQuestPastDayParam(npchar, "angry.terms") > sti(npchar.angry.terms) && _pchar.questTemp.Ascold != "Seek_powder")
                {
         			dialog.text = NPCharSexPhrase(npchar, "����� ����������� �� ��, ��� �� �� ������ ���� �������� ��������� ���������. �������, � ����� ����� ���� � �����.", "����� ������� �� ��, ��� �� �� ������ ���������� �� ��� � ����� �������� �������� ���������. �������, � ������ ��������� � �����.");
        			link.l1 = RandPhraseSimple("�������!", "�����������!");
        			link.l1.go = "First time";
        			CharacterDelAngry(npchar);
                }
            }
    	break;

		case "AngryExitAgain":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok")
		break;


//=========================>>>>>>>>>>>> ���� �������
		case "AngStep_1":
            dialog.text = "����, �� �� ��� ����, ��� ���������. �� ���� ������. ������ �����, � �� ����.";
            if (sti(_pchar.money) >= 1000)
            {
    			link.l1 = "������, ��� ���� ������.";
    		    link.l1.go = "AngryExitAgain";
    		    _pchar.questTemp.Ascold = "PowderWasSeek";
    		    CharacterDelAngry(npchar);
    		    AddMoneyToCharacter(_pchar, -1000);
    		    GiveItem2Character(_pchar,"Powder_mummie");
            }
            else
            {
    			link.l1 = "������ � ���� � ����� ��� ����� �����. ��� ������ ������ - ����� �� ������ ����.";
    		    link.l1.go = "AngryExitAgain";
            }
    	break;
//<<<<<<<<<<<<========================= ���� �������
	}
}

