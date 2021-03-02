// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{

	switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat("���������, ���� �� ������?",
                          "�� ������ ��� ��������� ��� ����. ��, ��������, ������������...", "������� �� ��� ������ ��� �������� � �����-�� �������...",
                          "��������, ��� �������, ����� ���� �������� ���-��. �� �������� ����!", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("������, " + NPChar.name + ", ���-������ � ��������� ���.", "�����, ����� ���-��...",
                      "�� ��, ������������� � ������ ���...", "��, �� ����...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			if (pchar.questTemp.BlueBird == "toBermudes")
			{
				link.l1 = "���������, � ��� ���� ��������� �������, ������ '����� �����'. ������ � ��� �� �������? �� ����� ���������� � ��� � ����� ���������...";
				link.l1.go = "BlueBird_1";
			}
			if (pchar.questTemp.BlueBird == "weWon")
			{
				link.l1 = "����� ��� �������, ��� � �������� ���� �� '����� �����'. ������ ��� ��� ������� �� �����.";
				link.l1.go = "BlueBird_3";
			}
		break;
		case "BlueBird_1":
			dialog.text = NPCStringReactionRepeat("� ������ �� ���� �� ���� �������. ���� ��� ���, ��� �� ������, �� �������.", 
				"�����? � ��� ��� ������.", 
				"���, ����� ��� �� ��?",
                "�� ����� �� ��� ��������?! ����� ���, �� ���������� ���� �� ������!", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("���-�� �� ����� �� �������, ��������!", 
				"��-��, � �����...",
                "���.", 
				"������, ������...", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("BlueBird_2", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
		break;
		case "BlueBird_2":
			dialog.text = "� ��� �� ��������, ��� �������� � ��� ������� � ���� �������� �� ���� ������������.";
			link.l1 = "� ���! �� � ������ �������. �� �����, ������� � �� ����...";
			link.l1.go = "exit";
			AddQuestRecord("Xebeca_BlueBird", "2");
		break;

		case "BlueBird_3":
			dialog.text = NPCStringReactionRepeat("����� ��� ������, �����...", 
				"������...", 
				"��, ������� �� ������� ��������...",
                "����� � ��� ��? �-�� ����� ��� ���?", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("�-�-�, ����� �������!", 
				"��-��.",
                "��� � ���� � �������!", 
				"����� � �� ��� ���, � ����� � ��� ���-��.", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("exit", "", "", "", npchar, Dialog.CurrentNode);
		break;
	}
	UnloadSegment(NPChar.FileDialog2);
}

