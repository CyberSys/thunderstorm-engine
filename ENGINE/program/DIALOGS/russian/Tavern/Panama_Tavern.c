// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat("��� ������� ������ "+ GetCityName(npchar.city) +" � ����� �������. ��� �� �� ������ ������?",
                          "�� ������ ��� ��������� ��� ����. ��, ��������, ������������...", "������� �� ��� ������ ��� �������� � �����-�� �������...",
                          "��� �� ������� ��� ������� ���� � �� ��...", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("������, " + NPChar.name + ", ���-������ � ��������� ���.", "�����, ����� ���-��...",
                      "�� ��, ������������� � ������ ���...", "�� ��...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
            // ==> �������� �� ������
			if (CheckAttribute(pchar, "questTemp.State.Store"))
            {
                link.l1 = "��������, ���� ������ �� �������� � ����������� �������� �� ����� ���� ����� �� �������?";
                link.l1.go = "Step_S2_1";
            }
            // <== �������� �� ������
		break;
//*************************** ����� �2 �������, ������� ���������� ***************************
 		case "Step_S2_1":
			dialog.text = NPCStringReactionRepeat("���, �� ����� ����� ������ � �� ������ � ���.", "�� ��� ��������� �� ����, � ���� �������.", "� �� ������ ����, �� ��� ��������� �� ���� �����.", "��������, ������ � �� �����! ������ ��� �� ����� �������?", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�� ��� ��, ������� ����.", "��, �� ��...", "��, �����, ���������...", "������, " + npchar.name + "...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;

	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

