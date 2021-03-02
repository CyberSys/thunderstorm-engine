
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	string NodeName = Dialog.CurrentNode;
	string NodePrevName = "";
	if (CheckAttribute(NextDiag, "PrevNode")) NodePrevName = NextDiag.PrevNode;

	switch(Dialog.CurrentNode)
	{
        case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "First time":
			NextDiag.TempNode = "First time";
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = "� �� ����� � ����� �������������! �� ����� �� �������!!";
				link.l1 = "��� ������..."
				link.l1.go = "exit";
				break;
			}
			if(NPChar.quest.meeting == "0")
			{
				npchar.quest.meeting = "1";
				if (CheckAttribute(loadedLocation, "storm"))
				{
					dialog.Text = "��-��, � ���� ���! ���� ����� ������� ��� �������. ��� �, ������ ���� ������, � ������ ��� ���������� � �����! �� ��� ��������� �� ���� �������...";
					Link.l1 = "�� ��, ����� ��������...";
					link.l1.go = "FT_1";
				}
				else
				{				
					dialog.Text = "��-��, � ���� ���! ��� ��� ������� ������� ��� ���������. ��� �, ������ ���� ������, � ������ ��� ���������� � �����! �� ��� ��������� �� ���� �������...";
					Link.l1 = "�� ��, ����� ��������...";
					Link.l1.go = "FT_1";
				}
			}
			else
			{
				dialog.text = "��� �� ������, ����� �������?";
				Link.l1 = "���� ������ ���� ���� ��������, " + npchar.name + ".";
				Link.l1.go = "int_quests";
				link.l10 = "�� ���, ������, ����������...";
				link.l10.go = "exit";
			}
		break;
		case "FT_1":
			dialog.text = "�� ��� ��, ������� �����������. ���� ����� " + GetFullName(npchar) + ". � ���?";
			link.l1 = "��� ��� - " + GetFullName(pchar) + ". � �������... �-��, ������ �������...";
			link.l1.go = "FT_2";
		break;
		case "FT_2":
			dialog.text = "��� �� ������� ���� �������, ��� �������?! �����������, ��� ��� ������ ������� �� ������ ����� �����. ��, ��� ����� ����, ��������...";
			link.l1 = "�� ��� �������, ����������...";
			link.l1.go = "FT_3";
		break;
		case "FT_3":
			dialog.text = "� ���� �������� ��� '����� ���������'! �� �� ������?";
			link.l1 = "�� ������, " + npchar.name + ". ������� ��� ������...";
			link.l1.go = "exit";
		break;
		//�������
		case "int_quests":
			dialog.text = "���������, �������.";
			if (!CheckAttribute(NextDiag, NodeName+".l3"))
			{
				link.l3 = "����� ���-�� �������, �� �� ��������? ��� ������� ��� � ���...";
				link.l3.go = "ansewer_3";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l1"))
			{
				link.l1 = "� �� ��� ���� ������?";
				link.l1.go = "ansewer_1";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l2"))
			{
				link.l2 = "�� �� ������ ��������� ������?";
				link.l2.go = "ansewer_2";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l4"))
			{
				link.l4 = "��������, � ��� �� ����� � ��� ��� ����������? � ���� ����� '��������' � '��������'.";
				link.l4.go = "ansewer_4";
			}
			link.l10 = "�� ������, ���-�� � �����, � ��� ����� ��������. ������, ��� ������...";
			link.l10.go = "exit";
		break;
		case "ansewer_3":
			dialog.text = "�������, ��... �� �� �� �����, ��� �� ���������.";
			link.l1 = "��� ��, �� ��� ��... ���������� ���-��.";
			link.l1.go = "ansewer_3_1";
			NextDiag.(NodePrevName).l3 = true;
		break;
		case "ansewer_3_1":
			dialog.text = "�� ������� �����, ��� ������ � ����, ��� ���� ��������. ��� ����������, ������� �� ������ ���������.";
			link.l1 = "���������?";
			link.l1.go = "ansewer_3_2";
		break;
		case "ansewer_3_2":
			dialog.text = "�� ��������� � ���������, ���������� ����� - � ��� ����� ������.";
			link.l1 = "�������... ������� �� ���������.";
			link.l1.go = "exit";
		break;
		case "ansewer_1":
			dialog.text = "��� ��, ��� � ��� �����. ��� ������� �������, � ������ �������. ������ ������, � ��������, ��� ���� �� ���, ����� ���� ������ ���������. ��� �����-�� ������� ��������, ���-��... � �����, �� ���� �, �� ����, ��� ���������, ������.";
			link.l1 = "����...";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l1 = true;
		break;
		case "ansewer_2":
			dialog.text = "��-��, ��� ����� ��������� ������. ������ ��� ����������.";
			link.l1 = "�� �������?";
			link.l1.go = "ansewer_2_1";
			NextDiag.(NodePrevName).l2 = true;
		break;
		case "ansewer_2_1":
			dialog.text = "��, � ���������. ������� �������� ������ ����, ������ ����� ��� �� ��������. � ������� ����� ������� ��������� �������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
		break;
		case "ansewer_4":
			dialog.text = "��... � ���� ����� ���� ����� ������� ����, �, �������, �� �� ��������� ��� �������. �� ���� � ���!!";
			link.l1 = "� � ��� ����?";
			link.l1.go = "ansewer_4_1";
			NextDiag.(NodePrevName).l4 = true;
		break;
		case "ansewer_4_1":
			dialog.text = "���� � ���, ��� ���� ������ �����. ������ ���� ����� �� ��� � ����������, ��� �� �������� ������ ����������������. ��� ����� ��������� ����� ������ ���� ��������� �� ������� ����.";
			link.l1 = "�������, ����.";
			link.l1.go = "exit";
		break;
	}
	NextDiag.PrevNode = NodeName;
}
 
