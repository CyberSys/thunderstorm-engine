// 28.11.05 ����, ����� angry-������ ��� ���������
void GeneralAngryDialog(aref _pchar, aref npchar, aref Link)
{
    if (CheckAttribute(npchar, "angry") && !CheckAttribute(npchar, "angry.ok"))
    {
        npchar.angry.ok = 1;
        switch (npchar.angry.kind)
        {
            case "repeat":
                Dialog.CurrentNode = "AngryRepeat";
            break;
        }
    }
	switch(Dialog.CurrentNode)
	{
		case "AngryRepeat":
            dialog.text = LinkRandPhrase(GetFullName(_pchar) + " � ���� �������� �� ����� �� ��������������. �� ���� �������������.", "�� ���� ��������, ��� ������� ��� ���� ������.", "�� ���� ������� ��������, ��� ��� � ���� �������� ���� ����������� �� �����.");
			link.l1 = LinkRandPhrase("��������� ��� ������ ��������.", "��, ���� ���� ������ �� �����...", "��, ��������� � ������ �����.");
		    link.l1.go = "AngryExitAgain";
    		link.l2 = RandPhraseSimple("��������, �� ��� ����������... ����� ���-�� ���������� ���������, ��� ����� ������ ������ ��������.", "������, �������� � ����� ��� ���� �����. ����� ������� ��� ������ ������?");
			link.l2.go = "CheckTimeOver";
    		link.l3 = "��� � ���� �������, ����� �� ������� ������ ��� ��� � ������ �������?";
			link.l3.go = "AngryStory";
    	break;

		case "CheckTimeOver": //���������� ����� ��������� �����
			dialog.text = RandPhraseSimple("����� �������� � �������...", "��� ��, ����� ������� ��� �� � ����.");
			link.l1 = RandPhraseSimple("��, ��� ������.", "��� ������� ����!");
			link.l1.go = "AngryExitAgain";
            if (CheckAttribute(npchar, "angry.terms"))
            {
                if (GetNpcQuestPastDayParam(npchar, "angry.terms") > sti(npchar.angry.terms))
                {
         			dialog.text = "��, ������������� ���� �� ��� �����������, ��� ������ ���������� �������... �� ��� �, ��� ������� ����� � ����� �������.";
        			link.l1 = RandPhraseSimple("�������!", "�����������!");
        			link.l1.go = "node_1";
        			CharacterDelAngry(npchar);
                }
            }
		break;

		case "AngryExitAgain":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok");
		break;

		case "AngryStory":
			dialog.text = "� ��� �� ������ ���������� �������� ��� ���� � ����� ��������?";
			link.l1 = "������.";
			link.l1.go = "ProposalMoney";
			link.l2 = "������� �����-������. � �������� �������, ����������...";
			link.l2.go = "ProposalItem";
		break;
//================ ���� ��������� � ���������� =================
		case "ProposalMoney":
			dialog.text = NPCStringReactionRepeat("�� ��� �, �����, ��� ����� � " + npchar.angry.QtyMoney + " ����� ������ ���������� ��� ��������� �����.", "��������, ����� �������� � " + npchar.angry.QtyMoney + " ������ ��� ��������.", "��� ��� ��� ���, ��� ����� ����� ��������. " + npchar.angry.QtyMoney + " - � �� ����� ������ ������.", "none", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("����� ������������� ��������� ����� � ���������� �� ����.", "������, �� ������� �� ����.", "��� ����, �� ������� � �� ���������.", "none", npchar, Dialog.CurrentNode);
			link.l1.go = "AngryExitAgain";
            if (sti(_pchar.money) >= sti(npchar.angry.QtyMoney))
            {
    			link.l2 = "� ����� ��� ����������.";
    			link.l2.go = "GiveMoney";
            }
		break;

		case "GiveMoney":
			dialog.text = "����� ��� ������ ������������, ���� ���. ����� ����� �������� ��� ���� ����� �������.";
            link.l1 = RandPhraseSimple("�������!", "�����������!");
			link.l1.go = "node_1";
			AddMoneyToCharacter(_pchar, -sti(npchar.angry.QtyMoney));
			CharacterDelAngry(npchar);
		break;

		case "ProposalItem":
			dialog.text = NPCStringReactionRepeat("�� � �������� ����������, � �������� ������. ������ �� �����.", "none", "none", "none", "block", 0, npchar, Dialog.CurrentNode);
            link.l1 = HeroStringReactionRepeat("����� ����...", "none", "none", "none", npchar, Dialog.CurrentNode);
			link.l1.go = "AngryExitAgain";
		break;

	}
}

