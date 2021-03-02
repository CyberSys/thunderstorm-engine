
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;
	string sTemp;
	bool bOk;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "First time":
			dialog.text = "����������� ����, ����!";
			link.l1 = "����� � ���� ����? �� �� ���� ����������-������. � ��� �� � ����� ���������?";
			link.l1.go = "Meeting_1";
			NextDiag.TempNode = "First time";
		break;

        case "Meeting_1":
			dialog.text = "��� ������? �� ����� �������, � �����, ��� ��� ���� '����� �������'. ������ - ������ � �������, � ������ ��, ��� ����� �����, �����.";
			link.l1 = "�� ��, �� ������� �����, ������ ��� ���� ���� ��� �� �������� ����������, � ��� ��� � ���. ����� ��� ����� ������ � ���������?";
			link.l1.go = "Meeting_2";
		break;
		
		case "Meeting_2":
			dialog.text = "���� �� �������. � ��� ������ ������� ����� � ����� ������� ������� �������. � ������ ���, ���� ���� �� ������ ���� � �� ������� � ����.";
			link.l1 = "�������, ��������� ��������, ���� ���. �� �������� ������?";
			link.l1.go = "Meeting_3";
		break;
		
		case "Meeting_3":
			dialog.text = "������ � ��������.";
			link.l1 = "�������� ��� ����������? � ���� ����?";
			link.l1.go = "Meeting_4";
		break;
		
		case "Meeting_4":
			dialog.text = "���, ������ ���, � �����.";
			link.l1 = "������� �� ����, ����� ��������. ������.";
			link.l1.go = "Exit";
			NextDiag.TempNode = "AfterMeet";
			ReOpenQuestHeader("GhostShipQuest");
        	AddQuestRecord("GhostShipQuest", "4");
		break;
		
		case "AfterMeet":
			dialog.text = "����� ���� ��������. ���� ��� �������?";
			link.l1 = "���, ��� �� � ��� ��������.";
			link.l1.go = "Exit";
			NextDiag.TempNode = "AfterMeet";
			bOk = (sti(PChar.GenQuest.GhostShip.KillMe) > 0) || (sti(PChar.GenQuest.GhostShip.DeadByMe) > 0);
			if (bOk && !CheckAttribute(pchar , "GenQuest.GhostShip.NeedCoins"))
            {
    			Link.l3 = "��� �� ������ � �������-��������?";
    			Link.l3.go = "GhostCapt";
			}
			if (GetCharacterItem(Pchar, "Coins") >= 666 && CheckAttribute(pchar , "GenQuest.GhostShip.NeedCoins"))
			{
                Link.l4 = "� ������ ���� ���������.";
    			Link.l4.go = "quest_coin_2";
			}
		break;
		
		// �������
		case "GhostCapt":
			NextDiag.TempNode = "AfterMeet";
			dialog.Text = "������� �������? ������� �����-����� ������, ������ ���� �������, ����� ����� ������� � ����� �������.";
			Link.l1 = "����� � ��� �� ��������?";
			Link.l1.go = "GhostCapt_1";
		break;
		
		case "GhostCapt_1":
			dialog.Text = "������, � ����� ���� ��� �������� �������. ��� ���� ��� 40 ��� �����, ���� � ��������� � ����. ������� ������ ������� �����, ����� ��� ���� �����, ���������� ���, �������� ��������� ������ ������.";
			Link.l1 = "��... ���������� ����, �������-������� ��� ������?";
			Link.l1.go = "GhostCapt_2";
		break;
		
		case "GhostCapt_2":
			dialog.Text = "� ��� ��������� ���. �� �� �������? ��� ����� ���������� ����� �������� � ���� ����������, ������� ���������� �������� � ������� ������ ������, ������. ����� ����� ������ ������ ������� �������� � �������.";
			Link.l1 = "��������� �������� ��� ���-�� ������. ��� ��� ������ ���������� � ����������?";
			Link.l1.go = "GhostCapt_3";
		break;
		
		case "GhostCapt_3":
			dialog.Text = "��� � ��������, ������ ��������. ���������� ��� �� ���, ���������� ��������� �������� �������� ��, ��� ����� ��������, ���� �� ������� �� �������. � ������ ����� � �������� � �������� ������� ��� ������, ������ �� �����, ��� ���������, � ������.";
			Link.l1 = "��-����� �������� �� ����� ��������� ������ ������ ����. �������� �� �� ������ ����� � ���� ���. ��� ����� � ��� ����������?";
			Link.l1.go = "GhostCapt_4";
		break;
		
		case "GhostCapt_4":
			dialog.Text = "�������� ����, ��� ��� ����� ��� �����������, � ������� ���, ��� ����, �������� ������ ����, ����� 666 ������ �������� � �� �������. ����� ��������� ��������� ���������� ��������.";
			Link.l1 = "������, ��� ���� � � �������!";
			Link.l1.go = "Exit";
			AddQuestRecord("GhostShipQuest", "5");
			PChar.GenQuest.GhostShip.NeedCoins = true;
		break;
		
		case "quest_coin_2":
            dialog.Text = "������������. �� ������ ���� ���! ������ ����� ��. ���� �������� ������ ��� ��� ��������� ������� � ������� ��� ��� ����� ����. �� ���� �� ���� �����. ������ ��� ����� ������ ������, ��� ������� ������.";
			Link.l1 = "������. ������� ��.";
			Link.l1.go = "Exit";
			TakeNItems(pchar, "Coins", -666);
			AddQuestRecord("GhostShipQuest", "6");
			PChar.GenQuest.GhostShip.LastBattle = true;
			NPChar.location = "none";
			NPChar.LifeDay = 0;
            NextDiag.TempNode = "temp_coins";
		break;
		
		case "temp_coins":
            dialog.Text = "�������, ����.";
			Link.l1 = "�������, ��� �����, ��� �� ������.";
			Link.l1.go = "Exit";
			NextDiag.TempNode = "temp_coins";
		break;
	}
}