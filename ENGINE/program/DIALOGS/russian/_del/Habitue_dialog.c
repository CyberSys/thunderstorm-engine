
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	int iColonyInfoSumm = SelectColonyInfoUpdate(npchar);
	
	switch(Dialog.CurrentNode)
	{

		case "First Time":
			dialog.text = "���� � ����! �� ��� ��... �-�-�... �� ����� ��� ���� �����, �� �� � �����! ������������, ������ ����, ��������� �� �����!";
			link.l1 = "� �� � ��������, �� ������ ������� ���.";
			link.l1.go = "exit";
			NextDiag.TempNode = "First Time";
		break;

		case "colony_Info_Time":
			if(iColonyInfoSumm > 100)
			{
				dialog.text = "��! ��� ���� ���, �� ��������� ��� �������, � ������ ���� ����� �������������� ���������� ������� � ����������.";
				link.l1 = "��? � ��� �� �� ��� ������ ����������?";
				link.l1.go = "colony_info_select";
				link.l2 = "���� ��� �� ����������.";
				link.l2.go = "exit";
			}
			else
			{
				dialog.text = "��������, �� � ������� �����, ����� ������������� � ����.";
				link.l1 = "�� ���� ���� ������.";
				link.l1.go = "exit";
			}
			AddDialogExitQuest("RechargeColonyInfoMan");
			NextDiag.TempNode = "First Time";
		break;

		case "colony_info_select":
			dialog.text = "� ���� ���� ��������� �������� � ������� " + XI_ConvertString(npchar.colonyinfoupdate) + ", �� � ���� ������� ��� ����� " + iColonyInfoSumm + " ������� ���������� � ��� �������.";
			if(sti(pchar.money) >= iColonyInfoSumm)
			{
				link.l1 = "������. ��� ���� ������.";
				link.l1.go = "colony_info_update";
			}
			link.l2 = "� ���� ��� ����� �����. ������.";
			link.l2.go = "exit";
		break;

		case "colony_info_update":
			dialog.text = "�������! � ������ ������... (�� ����������� ������������ ���������� ����������).";
			link.l1 = "��������� �� ������.";
			link.l1.go = "exit";
			AddMoneyToCharacter(pchar, -iColonyInfoSumm);
			ColoniesInfoUpdateFromMan(npchar);
		break;
                
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
