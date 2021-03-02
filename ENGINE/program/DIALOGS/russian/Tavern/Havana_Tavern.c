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
			if (pchar.questTemp.State == "Fr7RockBras_toSeekPlace")
			{
				dialog.text = "��������� ���� �������, ������.";
				link.l1 = "� ������, ������� �� ������ ��������� ���� ���������, ���� ������ ������!";
				link.l1.go = "Step_F7_1";
				break;
			}	
			Link.l1.go = "exit";
            if (pchar.questTemp.Ascold == "ToHavanna") // ����� ��������
            {
    			Link.l1 = "������, � ������ ������� ���������� ��������. ��� �����, ��� ����... � ���� �� ���?";
    			Link.l1.go = "Step_A1";
            }
		break;
//******************** ��.�������, ����� �7. �������� ���� ��������� ******************
 	 	case "Step_F7_1":
			dialog.text = NPCStringReactionRepeat("��, �����, ���� ����. ����������� �����, ����� � ���, ������.", "�� ��� ���������� ���� � ���������.", 
				          "����� � ���������?", "�� ������� ����� �� ���� �������, ������� � ������ ���������...", "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��� ����� - �� �����... � ��� � ��� �������? ������ ������� ��� ��.", "���������? �� ��, ��������...", "��, �����, ���������, ������...", "����� � � ������...", npchar, Dialog.CurrentNode);
			link.l1.go = DialogGoNodeRepeat("Step_F7_2", "none", "none", "none", npchar, Dialog.CurrentNode);
        break;
 	 	case "Step_F7_2":
			dialog.text = "��� ��, ������. �������� � �������� ���� ��������. � ����� ��� ������ ������ �����...";
			link.l1 = "� ������ �����-�� �� �����?";
			link.l1.go = "Step_F7_3";
        break;
 	 	case "Step_F7_3":
			dialog.text = "���, ������, ��� �� �� �����������. �� ������ ���� - ������������ ������! ����������...";
			link.l1 = "�-�-�, ���� �����. �� ��� ��, ������� ����, ����.";
			link.l1.go = "exit";
        break;
//******************** ����� ��������, ������ � ��������� � �������� ������ ******************
		case "Step_A1":
			dialog.text = "���, �� ��. ���� ����������, ��� � ��� ��� �� ��������� �����. � ��� ���� � ���, ��� ������ ��������� ��������� ������� �������...";
			Link.l1 = "����� �������?";
			Link.l1.go = "Step_A2";
		break;

		case "Step_A2":
			dialog.text = "�� �������, ��������� ������ ��������. ��� �������-���������� ������-�-������ ������ ���� ������� ���������� ���������. ������� ������ �������: �������, �������, ��������� ������ ������ - ����!";
			Link.l1 = "��� ���� ��! � ���-�� �����? ������ ������� ��� ������������ � ������, � ������ ��� �� �����?";
			Link.l1.go = "Step_A3";
		break;

		case "Step_A3":
			dialog.text = "��� �� ��� �������, ��� ���� �������, ��� �����? ���� ���, ������ �� ������...";
			Link.l1 = "������ �������� � ��� �����. � ���� ����� ���� �������?";
			Link.l1.go = "Step_A4";
		break;

		case "Step_A4":
			dialog.text = "�� ������, �� �� �������������, ��������-���� � ��� � ������, � �� ��� �����? � ����� ���� �����, ���� ����� �������?";
			Link.l1 = "�� �������, ������ ��� �������... ������ �� ������ �����, �� ����� ������ �������.";
			Link.l1.go = "Step_A5";
		break;

		case "Step_A5":
			dialog.text = "��� � �����, ��� �� �����. �� ������, ��� ���� �� ��� ���� �� ������ - ��������� ��������� �� ��������.";
			Link.l1 = "�����... ��� � ��� ������ ������? �����, �������...";
			Link.l1.go = "exit";
			pchar.questTemp.Ascold = "SeekInformatorHavanna";
			AddQuestRecord("Ascold", "6");
            ref sld = GetCharacter(NPC_GenerateCharacter("FernandoGonsales", "off_spa_1", "man", "man", 28, SPAIN, -1, true));
            FantomMakeCoolFighter(sld, 28, 90, 70, BLADE_LONG, "pistol3", 10);
            LAi_SetStayType(sld);
           	sld.name 	= "��������";
			sld.lastname 	= "��������";
			sld.Dialog.Filename = "Quest\AscoldNpc.c";
			ChangeCharacterAddressGroup(sld, "Santiago_houseSp2", "goto", "goto2");
            LAi_LocationFightDisable(&Locations[FindLocation("Santiago_houseSp2")], true);
		break;


	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

