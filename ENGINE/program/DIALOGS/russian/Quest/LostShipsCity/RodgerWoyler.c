
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
		case "First time":
			NextDiag.TempNode = "First time";			
			if (npchar.quest.meeting == "0")
			{
				npchar.quest.meeting = "1";
				if (CheckAttribute(loadedLocation, "storm"))
				{
					dialog.text = "�������� �� �������������� �����, ������� ��� ������������, �������.";
					link.l1 = "������������?";
					link.l1.go = "FS_1";
				}
				else
				{
					dialog.text = "��� ������������, �������.";
					link.l1 = "������������?";
					link.l1.go = "FT_1";
				}
			}
			else
			{
				dialog.text = "��� �� ������, ���?";
				link.l1 = "����������, � ��� ������ ��������. �� ������, ��� �� ������?";
				link.l1.go = "SeekCitizen";
				link.l2 = "���� ������ ��� ������.";
				link.l2.go = "int_quests";
				link.l10 = "������...";
				link.l10.go = "exit";
			}
		break;
		case "FS_1":
			dialog.text = "�� �������! �� �������� ��������, ������ ���� ������ ����, ����� ������ � ����. ����� �� ����� � ���� ����������? � ����� ������� ��� ������ � ������, � ���� �� ������ �� ��� �� ������.";
			link.l1 = "��������...";
			link.l1.go = "FT_2";
		break;
		case "FT_1":
			dialog.text = "�� �������! �� �������� ��������, ������ ���� ������ ����, ����� ������ � ����. ����� �� ����� � ���� ����������?";
			link.l1 = "��� ������, ���...";
			link.l1.go = "FT_2";
		break;
		case "FT_2":
			dialog.text = GetFullName(npchar) + ", ���.";
			link.l1 = "��� ����������, �����. ���� ����� ������� " + GetFullName(pchar) + ".";
			link.l1.go = "FT_3";
		break;
		case "FT_3":
			dialog.text = "�������, �������. ��� ���� ������?";
			link.l1 = "����� �� � ������?";
			link.l1.go = "FT_4";
		break;
		case "FT_4":
			dialog.text = "����� �����, ������� �������. �� �����, ��� � ����� ������ ����, ��...";
			link.l1 = "�� ���� ����� �� �������?..";
			link.l1.go = "FT_5";
		break;
		case "FT_5":
			dialog.text = "�... ���, ����� ��� ����� ��������?";
			link.l1 = "������ ������ ������ �������� �����?";
			link.l1.go = "FT_6";
		break;
		case "FT_6":
			dialog.text = "�� �� ���� ������ �������, �������. � �� �� ��������� ��� ����� � ��� ����.";
			link.l1 = "������?";
			link.l1.go = "FT_7";
		break;
		case "FT_7":
			dialog.text = "������ ��� ��� ������������ � ���� ������ ����� ������� �������� � �� ��� �� ������... ��������.";
			link.l1 = "���� ���?..";
			link.l1.go = "FT_8";
		break;
		case "FT_8":
			dialog.text = "��� ��� ��������, �������, � ����� ������� ��������� �� ���� ����, �� �� �� ���, ��� ����������� ��� � ���� ������, ����� ����������� � �������� �������.";
			link.l1 = "��������, ������� � ��� � ��� �� ��������... ��� �� ��� �� ����, � ������� ���� ������, ������ ����, ���������?";
			link.l1.go = "FT_9";
		break;
		case "FT_9":
			dialog.text = "� ���� ����� ������� ������ ���! �� � ���������, �������, ����� �� �� ���������� ���� �����. ������� ������ � ����������� �� ��, ������� ������ ������ ��� ��� ����� ����, ��� �� ������, � � �����, ����� �� ���, ������... �� ��� ��� ������.";
			link.l1 = "������ �� �� ����������, ������?";
			link.l1.go = "FT_10";
		break;
		case "FT_10":
			dialog.text = "��� �����, ��� �����...";
			link.l1 = "��� �, ��������� �� ������������.";
			link.l1.go = "FT_11";
		break;
		case "FT_11":
			dialog.text = "�������� ����, �������.";
			link.l1 = "�������������.";
			link.l1.go = "exit";
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		//���� ��������
		case "SeekCitizen":
			dialog.text = "� ���� �� �����?";
			Link.l1.edit = 3;
			Link.l1 = "";
			Link.l1.go = "SeekCitizen_Choice_1";
		break;
		case "SeekCitizen_Choice_1":
			sld = CheckLSCCitizen();
			if (sld.id == "none")
			{
				dialog.text = "� �� �������, � ��� �� ��������. ��� ����� ����� ��� � ������� ����� ��������.";
				Link.l1 = "�������. ������ � ��� �������� �������.";
				Link.l1.go = "SeekCitizen_Choice_2";				
				Link.l2 = "�������, � ����� ��� �����.";
				Link.l2.go = "exit";	
			}
			else
			{
				if (sld.id == npchar.id)
				{
					dialog.text = "��� ��� �� �!";
					link.l1 = "���� ��, ����� �� ��� �����!";
					link.l1.go = "exit";
					break;
				}				
				if (sld.sex == "man")
				{
					dialog.text = GetFullName(sld) + ", �� � ��� ��������?";
					Link.l1 = "��-��, �����, ��� ��.";
					Link.l1.go = "SeekCitizen_agree";				
					Link.l2 = "���, �� � ���. ������ ��� ��� ������.";
					Link.l2.go = "SeekCitizen_Choice_2";
				}
				else
				{
					dialog.text = GetFullName(sld) + ", �� � ��� ��������?";
					Link.l1 = "���, ������ � ���.";
					Link.l1.go = "SeekCitizen_agree";				
					Link.l2 = "���, �� � ���. ����������, ������� � ��� ��� �������� �������.";
					Link.l2.go = "SeekCitizen_Choice_2";
				}
				Link.l3 = "������, �� ���� � ������ �� � ��� ����������. �� ��������.";
				Link.l3.go = "exit";
				npchar.quest.seekIdx = sld.index;
			}
		break;
        case "SeekCitizen_Choice_2":
			dialog.text = "����� �������� ��� ��� ��� � �������.";
			Link.l1.edit = 3;
			Link.l1 = "";
			Link.l1.go = "SeekCitizen_Choice_1";
		break;

		case "SeekCitizen_agree":
			sld = &characters[sti(npchar.quest.seekIdx)];
			npchar.quest.seekIdx.where = WhereLSCCitizen(sld);
			if (npchar.quest.seekIdx.where == "none")
			{
				if (sld.sex == "man")
				{
					dialog.text = LinkRandPhrase("�� ����, ����� ��� �� �����.", "���� ��� �����, ��� ��...", "�� ����� ��� ��� ��������, ��� ��� �� �����.");
					link.l1 = RandPhraseSimple("�������.", "����...");
					link.l1.go = "exit";
				}
				else
				{
					dialog.text = LinkRandPhrase("�� ����� �� ��� �������� �����, �������.", "������� �� ����, ��� ��� ������.", "��, � �� ����, ��� ��� ����� ������ ����.");
					link.l1 = RandPhraseSimple("����.", "����...");
					link.l1.go = "exit";
				}
			}
			else
			{
				if (sld.sex == "man")
				{
					if (sld.location == "LostShipsCity_town")  
					{						
						string Str1 = npchar.location.locator;
						string Str2 = sld.location.locator;
						if (npchar.location == sld.location && strcut(Str1, 0, 5) == strcut(Str2, 0, 5))
						{
							dialog.text = LinkRandPhrase("�� �����, �� ���� �������. ����� ������������.", "��, ��� �� �����, �� ���� �������!", "�� �� ���� �������, �������, ��� �� ��� �� ������� ���.");
						}
						else
						{
							dialog.text = LinkRandPhrase("� ����� ��� ������ ������� " + npchar.quest.seekIdx.where + ".", "������� �������� ��� " + npchar.quest.seekIdx.where + ". ��� ��� ����� ��� ���.", "��������� � ����, ������ �� ��������� " + npchar.quest.seekIdx.where + ".");
						}
					}
					else
					{
						if (npchar.location == sld.location)
						{
							dialog.text = LinkRandPhrase("��� �� �����, " + npchar.quest.seekIdx.where + ". �������� ������������.", "��� ���� �� �����, ����� ���!", "���� �� ��������? �� �� ����� ���������.");
						}
						else
						{
							dialog.text = LinkRandPhrase("� ����� ��� ������ ������� " + npchar.quest.seekIdx.where + ".", "������� �������� ��� " + npchar.quest.seekIdx.where + ". ��� ��� ����� ��� ���.", "��������� � ����, ������ �� ��������� " + npchar.quest.seekIdx.where + ".");
						}
					}
					link.l1 = RandPhraseSimple("���, �������. ������� ���.", "������� ���!");
					link.l1.go = "exit";
				}
				else
				{
					if (sld.location == "LostShipsCity_town")  
					{						
						string Str3 = npchar.location.locator;
						string Str4 = sld.location.locator;
						if (npchar.location == sld.location && strcut(Str3, 0, 5) == strcut(Str4, 0, 5))
						{
							dialog.text = LinkRandPhrase("��� �����, �� ���� �������. ����� ������������.", "��, ��� ��� �����, �� ���� �������!", "��� �� ���� �������, �������, ��� �� ��� �� ������� ��.");
						}
						else
						{
							dialog.text = LinkRandPhrase("� ����� �� ������ ������� " + npchar.quest.seekIdx.where + ".", "������� �������� �� " + npchar.quest.seekIdx.where + ". �� ���� ����������.", "��������� � ����, ������ ��� ��������� " + npchar.quest.seekIdx.where + ".");
						}
					}
					else
					{
						if (npchar.location == sld.location)
						{
							dialog.text = LinkRandPhrase("��� ��� �����, " + npchar.quest.seekIdx.where + ". �������� ������������.", "��� ���� ��� �����, ����� ���!", "���� �� ��������? ��� �� ���-�� ����� �����.");
						}
						else
						{
							dialog.text = LinkRandPhrase("� ����� �� ������ ������� " + npchar.quest.seekIdx.where + ".", "������� �������� �� " + npchar.quest.seekIdx.where + ". �� ���� ���������� � ���.", "��������� � ����, ������ ��� ��������� " + npchar.quest.seekIdx.where + ".");
						}
					}
					link.l1 = RandPhraseSimple("���, �������. ������� ���, "+GetAddress_Form(NPChar)+".", "������� ���!");
					link.l1.go = "exit";
				}
			}
		break;
		//�������
		case "int_quests":
			dialog.text = "����������� ��� ������, �������.";
			if (!CheckAttribute(NextDiag, NodeName+".l3"))
			{
				link.l3 = "� ��������� ������� ������ ����� �����?";
				link.l3.go = "ansewer_3";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l1"))
			{
				link.l1 = "����� ���-�� �������, �� �� ��������?";
				link.l1.go = "ansewer_1";
			}
			if (!CheckAttribute(NextDiag, NodeName+".l2"))
			{
				link.l2 = "������ ������ ����� � ���������? ����� ���� ����� � ������� �����?";
				link.l2.go = "ansewer_2";
			}
			link.l10 = "��� ��������. ��������...";
			link.l10.go = "exit";
		break;
		case "ansewer_3":
			dialog.text = "�� �������. ��������� ������ ������ � ��� ���������� �������. ������, �������, ���� � ����� ������, ����� ����� �������� ������ � ������ ������...";
			link.l1 = "� ����� ������?";
			link.l1.go = "ansewer_3_1";
		break;
		case "ansewer_3_1":
			dialog.text = "������� ������! �� ��� ����� �� �����, ������ ��� ��������� ��� ������������. � ��� �����...";
			link.l1 = "�� ��� ��, ��������.";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l3 = true;
		break;
		case "ansewer_1":
			dialog.text = "��� �� ��� ���� �����! ���, ��� ����� �� ����� � �� ���� ������ ������, �� ���������� ���� ����� �� �������.";
			link.l1 = "��, �� � � �� ���.";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l1 = true;
		break;
		case "ansewer_2":
			dialog.text = "����� ��� �����? ���� ��������� �������� �������, ����� �� ��������� �����... ������ �����, ������� �����, ��������� ��� ������� � ����� �����. � ��� ����� �������.";
			link.l1 = "���������.";
			link.l1.go = "exit";
			NextDiag.(NodePrevName).l2 = true;
		break;
	}
	NextDiag.PrevNode = NodeName;
}
