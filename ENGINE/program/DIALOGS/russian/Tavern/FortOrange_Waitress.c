// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("�������, " + GetAddress_Form(NPChar) + "?", "��� ��� ������, " + GetAddress_Form(NPChar) + "?"), "��, �� �� ��������� �� �� � ���, " + GetAddress_Form(NPChar) + "? ����� �� �������� ���������...", "� ������� ����� ��� �� ��� ������ ��� �������� � �����-�� �������... ��� ����� ��������?",
                          "����� ������� ������ ��������, " + GetAddress_Form(NPChar) + "?", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("� ���������...", "������ ��� �� � ��� ��������"), "�� ���, ��� ��, ���������...",
                      "����� ���, ��������, �������...", "�� ���, ����� �������...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
		break;

        case "DefenceOrange": //����������� �������, ����� �4, ������ ����� �����.
            dialog.text = "��, ������, � ������ ���������, ��� �������� ����! � ��� � ��������...";
            link.l2 = "� ��� �� ����?";
            link.l2.go = "Step_H4_1";
        break;

        case "Step_H4_1":
			dialog.text = "� ��� � ��������, ��� ���� ������ �������� �������� � �������! ���� ���� ��� ����������, �� ������ ������ ���!";
			link.l1 = "�� �� ��� ��, ���������, � �������� ���� ����...";
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
		break;
		
	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

