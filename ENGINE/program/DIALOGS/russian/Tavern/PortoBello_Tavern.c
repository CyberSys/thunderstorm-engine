// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("����� �������?", "��� ��� ������, " + GetAddress_Form(NPChar) + "?"), "������ ������� �� �������� ������ ��� ������, " + GetAddress_Form(NPChar) + "...", "� ������� ����� ��� �� ��� ������ ��� �������� � �����-�� �������...",
                          "����� ������� ����� ��������?", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("� ���������...", "������ ��� �� � ��� ��������"), "��, ���-�� � ������� ���� �����...",
                      "�� ��, ������������� � ������ ���...", "�� ���, ����� �������...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (pchar.RomanticQuest == "SeekInPortoBello")
			{
				link.l1 = "��������, �� �� � �����, ����� �� ���� ����� �������� ����� '������������'?";
				link.l1.go = "Romantic_1";
			}
		break;
		case "Romantic_1":
            dialog.text = "���, ���� ����� �� ���������.";
            link.l1 = "�������... �����, � ������� ���-������ ������� � ����?";
			link.l1.go = "Romantic_2";
		break;	
		case "Romantic_2":
            dialog.text = "������ ���, " + GetAddress_Form(pchar) + ", ��� �����.";
            link.l1 = "����... �� ��� ��, ������� ����.";
			link.l1.go = "exit";
			pchar.RomanticQuest = "ToRositaInBeliz";
		break;	
	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

