// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("����� �������?", "��� ��� ������, " + GetAddress_Form(NPChar) + "?"), "������ ������� �� �������� ������ ��� ������ " + GetAddress_Form(NPChar) + "...", "� ������� ����� ��� �� ��� ������ ��� �������� � �����-�� �������...",
                          "����������, ���� �� �� �� ����� �����, �� �� ����� ���� ��������� ��������.", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("� ���������...", "������ ��� �� � ��� ��������"), "��, ���-�� � ������� ���� �����...",
                      "�� ��, ������������� � ������ ���...", "��������, �� �������� ���� ���� ������ �� ����������.", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
            // ==> ����� �6 ����.�������, ����� � ������
            if (pchar.questTemp.State == "MorrisWillams_SeekSpanishGoods")
            {
    			link.l1 = "��������, � ���� � ����� �� ���� �������� � ��������� �����?";
    			link.l1.go = "Step_E6_1";
            }
		break;
//************************************* ������� �� ����.������� *******************************
		case "Step_E6_1":
            dialog.text = "������� ����� ������ �����. �� ���� ������ ������� ����� ������ ����� �����, ������ ��� ������� ������ ���������.";
			link.l1 = "��� ��� ��! � ���� ��� ���������� �������� �������� ��������� �������?";
			link.l1.go = "Step_E6_2";
		break;

		case "Step_E6_2":
            dialog.text = "�� ���� ��� �����, �� ����� ������ �������-�����������.";
			link.l1 = "� ��� ������ ���� �������? � ��� �� �������?";
			link.l1.go = "Step_E6_3";
		break;

		case "Step_E6_3":
            dialog.text = "����� '����� ������', ���� ����� � �������� ������ ������ �� ���-����. � ��� ���� �������?";
			link.l1 = "���� ��������� � ���������, �� ��� ������...";
			link.l1.go = "Step_E6_4";
		break;

		case "Step_E6_4":
            dialog.text = "���, ������... �� �� ����, ��� ����� ����������. ���� �� ������ ������� ����� ������ �� ���-�����, ���� ����� ����� � ���� ����� ������. �� � ��, �������� �� - ������� ������...";
			link.l1 = "���-���� ���������. �� �������� � ���� ������.";
			link.l1.go = "exit";
			AddQuestRecord("Eng_Line_6_MorrisWillams", "9");
			pchar.questTemp.State = "MorrisWillams_CatchUpPinnace";
            SaveCurrentQuestDateParam("questTemp");
			pchar.quest.MorrisWillams_CheckTimeForForward.win_condition.l1 = "location";
			pchar.quest.MorrisWillams_CheckTimeForForward.win_condition.l1.location = "Jamaica";
			pchar.quest.MorrisWillams_CheckTimeForForward.win_condition = "MorrisWillams_CheckTimeForForward";
		break;
	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

