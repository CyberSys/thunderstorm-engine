// ������ ��������, ����� ���������� � ������������ �����, ���������� �������
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
    
// ============================================================================
// ============================= ���� angry ==========>>>>>>>>>>>>>>>>>>>>>>>>>
    if (CheckAttribute(npchar, "angry") && !CheckAttribute(npchar, "angry.ok"))
    {
        npchar.angry.ok = 1;
        if (!CheckAttribute(npchar, "angry.first")) //������� ������� ������������
        {
            NextDiag.TempNode = NextDiag.CurrentNode;
            Dialog.CurrentNode = "AngryExitAgain";
            npchar.angry.first = 1;
        }
        else
        {
            switch (npchar.angry.kind) //���� ����������� ������� �����. � npchar.angry.name ������� �������!!!
            {
                case "repeat":
                    if (npchar.angry.name == "Firsttime") Dialog.CurrentNode = "AngryRepeat_1";
                break;
            }
        }
    }
// <<<<<<<<<<<<<<<<<<<<<<======= ���� angry ===================================
// ============================================================================
	switch(Dialog.CurrentNode)
	{
		case "First time":
            dialog.text = NPCStringReactionRepeat("� ���� �� ��� ����? ���? ����� ��������� ������!",
                         "� ������� ���� ���������...", "���� � ��������� � ����, �� �� ����������� ��������� ����!",
                         "��, �� �� �� � ���� �������, ��� ���...", "repeat", 30, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��� �����.",
                                               "��, " + npchar.name + ", ���������� ����.",
                                               "������, " + npchar.name + "...",
                                               "��, ��...", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
		break;

 		case "Exit":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
		break;
		
		//********************* ���������� ������� *********************
 		case "EngLine_quests":
			dialog.text = "� ������ ���, �������.";
			link.l1 = "� ������ � ��� �� ��������� �������-����������� ������ ���� ������ ���������.";
			link.l1.go = "Step_1";
  		break;
 		case "Step_1":
			dialog.text = "������ �����������.";
			link.l1 = "��� ����� �������� ������ ��� ������� ��������� �������, ���������� �� � ��������� ����������� ������� � ��������� ���� - ����� �������.";
			link.l1.go = "Step_2";
  		break;
 		case "Step_2":
			dialog.text = "��... � � ��� ������� ������ �������? ���������, ��� �������� ��������. ��������� �� �������� �� ����������. ��, � ���������� ���� ������������ � �������� ���������, �� ��� ���������������� �� ��������� �����. ����� �� - ����� ����.";
			link.l1 = "���, ����� ������� � ���������� ����������� ��������� �������� �����. ����� ����, ���������� ������� ���� �������� ��������� �� ���������� � ������� ������, �� ��� �����.";
			link.l1.go = "Step_3";
  		break;
 		case "Step_3":
			dialog.text = "�������. ��������� �� ����������. � ����� �� ����.";
			link.l1 = "����, � ��� ������ �������� �� ����������.";
			link.l1.go = "Step_4";
  		break;
 		case "Step_4":
			dialog.text = "���� ���, � �� ���� ��� ���� �������������� ������ �� ������� �������-����������� ������.";
			link.l1 = "������ ��� ������� ��� ����� ������ ��������.";
			link.l1.go = "Step_5";
  		break;
 		case "Step_5":
			dialog.text = "������ ���������. ��� ��������� ������� - ������ ���������. � - ���������.";
			link.l1 = "�� �� ���������� ����� � ����������� ���������������� ���� ������ ���������. ��� ������� ���������� ����� ������������ ����� �� ��� �������.";
			link.l1.go = "Step_6";
  		break;
 		case "Step_6":
			dialog.text = "��� ����� �������� ���, ��� ������. � ������������� ���� ���������� ��������� ������, �� ��� ������ �� �������� ����, ��� � ���� ������ ������ ���� �����������������. ����� ������ �������� ������ �� ����. � ������� ���, ����� ��� ������ �������. ��������.";
			link.l1 = "��� �, ���� ��� ������ ������ �������, �� ��������.";
			link.l1.go = "exit";
            NextDiag.TempNode = "First time";
            AddQuestRecord("Eng_Line_2_Talking", "3");

            pchar.questTemp.Count = makeint(sti(pchar.questTemp.Count) + 1);
            if (pchar.questTemp.Count == "4")
			{
				pchar.questTemp.State = "after_talks_with_pirates";
			}
  		break;
 		case "No_Mansfield_first_time":
			dialog.text = "������ ����, ���� ����� ������� ���. ����� ������ ��������� � ������ ������ ����������.";
			link.l1 = "����� �������, �������.";
			link.l1.go = "exit";
			NextDiag.TempNode = "No_Mansfield";
  		break;
 		case "No_Mansfield":
			dialog.text = "��� ������ ������� � ����� �������������. ����� �����.";
			link.l1 = "� ��� �����.";
			link.l1.go = "exit";
  		break;
		//********************* ����������� ������� *********************
  		case "HolLine3_WormEngPlans":
			dialog.text = "��� ������� ���� �� ���?";
			link.l1 = "� ������ � ��� �� ��������� �������-����������� ������� ������ ����������.";
			link.l1.go = "Step_H3_1";
        break;
 		case "Step_H3_1":
			dialog.text = "����� ������. ��� ����� �� ���� �������-����������� ����������?";
			link.l1 = "������, ��������� ������ �� ����������, �������� ��������� �������� �����.";
			link.l1.go = "Step_H3_2";
  		break;
 		case "Step_H3_2":
			dialog.text = "��������� �������, � ���� ���� � ���� �����...";
			link.l1 = "��� ����� �� ����������, ����� ������������.";
			link.l1.go = "Step_H3_3";
  		break;
 		case "Step_H3_3":
			dialog.text = "���, �� ����� ���������... ��, �� ������ ���� ������������� ��� � ���, ��� ����� ����������� �������� �� ������� � ������ ������� � �����.";
			link.l1 = "���������� �����. ����� ��� - ��������� ������������� ��� ����������� ���������.";
			link.l1.go = "Step_H3_4";
            TakeItemFromCharacter(pchar, "letter_1");
            BackItemDescribe("letter_1");
  		break;
 		case "Step_H3_4":
			dialog.text = "��� ���� � ���, ��� � �� ���� ������ � ������ ��������� �� ���� �����. � ���� � ��� ���� ������� �������, �������� ����� �� ��� ���.";
			link.l1 = "� ��� ��� �������� ����������? ��� �� ������ �� ������ � ����� �� ������� ������ ����� ������ � ������� ���?";
			link.l1.go = "Step_H3_5";
  		break;
 		case "Step_H3_5":
			dialog.text = "�� ���� �������� ���������, ������� �������, � ����, ��� � �����. �� � ������������� ������ �� ����� � ������ ���������. ������, ��� ��� � ���� ��������.\n"+
                          "��������� �� ������� ������ �� ��� �������� ����� �������� �� ���������, ������� ���������� ���� ������������ ���������� �� �������. �, �����������, ���������. ���������� ����� ������, ������� � ���� ������.\n"+
                          "����� ����������, ��� ��������� �������� ����� �������� ������� ������� ������ �������� ������ ����������� � ������� �������. �����, ��� ��� ��� �������...";
			link.l1 = "����������, ��� �������� ��������� ������� �� �������?";
			link.l1.go = "Step_H3_6";
  		break;
 		case "Step_H3_6":
			dialog.text = "����������� ��������� �� ������� �� �����, � ��� ��������� ������ ����������� - ���� ��. � ���� ������������ ��� ��� ����� ��������, �� �� ������� ����� �� ������� �������� ����������.\n"+
                          "�� � ������, ����� �������� ������� ���� ����� �� ��� �� �������� �������� ���������� ����������� ���������� ���������� ������� �� ����� ������. ��� ��� ���, ������� ������ ��� �� �� �����.";
			link.l1 = "�� ��� ��, ��� ����, ��������� ����� �� ���������...";
			link.l1.go = "Step_H3_7";
  		break;
 		case "Step_H3_7":
			dialog.text = "� �� ��� ����������� �� ������. ���� � ���, ��� ��� ������� ����������� �������� � ��������� ����, ������ �������� ���-���� ����� ������ ������� ������ ��������. ��� ��� ���� ����� ����� ������� � ���������� �� ����� �����.\n"+
                          "��� ��� � ���� �������� ������� - ����������� ���������� � ���� ����� � ����� �������.";
			link.l1 = "� ����� �������, �������� ��������?";
			link.l1.go = "Step_H3_8";
  		break;
 		case "Step_H3_8":
			dialog.text = "��, ������ ���. � �� ����������, ��������� �� ���� �� ��� ����� �������. ���� ���� ������, �� ���� ����� ������ � ��� � �������. ��������� ��� �� ����� ����� ������ ����.";
			link.l1 = "� �� �������?";
			link.l1.go = "Step_H3_9";
  		break;
 		case "Step_H3_9":
			dialog.text = "���� ������ ������, �� ������� ���.";
			link.l1 = "������, �� �� �������� ��� ������ ��� �������, ����� �� ������� ���� ������?";
			link.l1.go = "Step_H3_10";
  		break;
 		case "Step_H3_10":
			dialog.text = "���, ��� �� �����������. ���� �� ���-�� � ����� ��� ����� ������� � ����������� �������, ����� ������� ����� �� ��������. �� ���� �������.";
			link.l1 = "������ ���������, �� ��������� ��� ������ ���������. ������� ��� �� ������!";
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
    		pchar.questTemp.State = "WormEnglPlans_toMorgan";
    		AddQuestRecord("Hol_Line_3_WormEnglishPlans", "2");
    		QuestSetCurrentNode("Henry Morgan", "HolLine3_fromMansfield");
            ChangeCharacterAddressGroup(&characters[GetCharacterIndex("Henry Morgan")], "SentJons_HouseF3", "goto", "goto1");
            LAi_SetHuberStayType(characterFromId("Henry Morgan"));
          break;


// ======================== ���� ��� angry ===============>>>>>>>>>>>>>>>
		case "AngryRepeat_1":
            dialog.text = RandPhraseSimple("����� ��� ������!", "��� �� ����� ����!");
			link.l1 = "��-��-��...";
		    link.l1.go = "AngryExitAgainWithOut";
            if (CheckAttribute(npchar, "angry.terms")) //����������� � ���������� ����� 30 ����.
            {
                if (GetNpcQuestPastDayParam(npchar, "angry.terms") > sti(npchar.angry.terms))
                {
         			dialog.text = "���� �� ����� ��������� ����, ������� ���.";
        			link.l1 = "� �����, ������, ������ �� ����.";
        			link.l1.go = NextDiag.TempNode;
        			CharacterDelAngry(npchar);
                }
            }
    	break;
		case "AngryExitAgain":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok");
		break;
		case "AngryExitAgainWithOut":
            DialogExit();
            DeleteAttribute(npchar, "angry.ok");
            DoReloadCharacterToLocation("LaVega_town","reload","reload6");
		break;
// <<<<<<<<<<<<============= ���� ��� angry =============================
	}
}
