
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	
	string sAgreeLink, sDeclineLink;
	switch(Dialog.CurrentNode)
	{
		case "First time":
			dialog.text = "������������, ����������, � ���� �����, � ��� ������������.";
			link.l1 = "��� ��� " + GetCharacterFullName(pchar.id) + ".";
			link.l1.go = "Second Time";
			NextDiag.TempNode = "Second Time";
		break;

		case "Second Time":
			dialog.text = SelectHovernorGreetingsDialog(npchar);
			link.l2 = "��� ������ ���������� ������������������� ��� ���� ��������, � ������ � �����.";
			link.l2.go = "exit";
			///////////////////////////////////////////////////////
			/// �������� ����������� �� ������ (������ ���������)
			///////////////////////////////////////////////////////
			if(!CheckForNationService(sti(npchar.nation)))
			{
				link.l1 = "� ���� ��������� �� ��������������� ������.";
				link.l1.go = "service";
			}
			///////////////////////////////////////////////////////
			/// �������� �������� �� ������� (������ ���������)
			///////////////////////////////////////////////////////
			if(CheckForDisease(npchar) == true && !CheckForNationService(sti(npchar.nation)))
			{
				dialog.text = "�������� ����, �� �� ������� ������������ �������� ���������...";
				if(GetCargoGoods(pchar, GOOD_MEDICAMENT) > 10)
				{
					link.l1 = "� ���� �� ����� ���� �����������, ����� ���� � ����� ��� ������?";
					link.l1.go = "disease_mission_completed";
				}
				else
				{
					link.l1 = "��� �� ��� ���� �� ������ - ��������� ����� �� ������ ������������� ����� ����.";
					link.l1.go = "disease_mission_2";
				}
			}
			///////////////////////////////////////////////////////
			/// �������� �� ����������� ������
			///////////////////////////////////////////////////////
			if(CheckAttribute(npchar, "quest.questflag"))
			{
				if(sti(npchar.quest.questflag) == 1)//����� �����
				{
					link.l2 = "��� �� � ��� ��� ���� ������-������ �������?";
					link.l2.go = "taking_quest_line";
				}
				if(sti(npchar.quest.questflag) == 2)//����� �����
				{
					link.l2 = "���� ������� ����������..";
					link.l2.go = "check_quests_lines";
				}
				if(sti(npchar.quest.questflag) == 3)//��������� �����
				{
					link.l2 = "� ���� ���������� � ���� � �������, ������� �� ��� ��������.";
					link.l2.go = "reminder_quests_lines";
				}
			}
			NextDiag.TempNode = "Second Time";
		break;

		///////////////////////////////////////////////////////
		/// ����� �� �������� ������������
		///////////////////////////////////////////////////////
		case "disease_mission_2":
			dialog.text = "���... � ������ ������������ ������� � �����... ��� �� ������� �� ��, ��� � ������� ��� ��������� �� ������ ���� ������������?";
			link.l1 = "� �����, ��� � �������� �������� �� ����� ����������� ������.";
			link.l1.go = "disease_mission_accept";
			link.l2 = "��� �� ��� ��������, ��� ��� ������������ ����, ��� ������.";
			link.l2.go = "disease_mission_refused";
		break;

		case "disease_mission_accept":
			dialog.text = "����� ����! ��������, � �� ������ ��������� ������.";
			if (GetSquadronGoods(pchar, GOOD_MEDICAMENT) >= 10)
			{
				link.l1 = "�� �����, ��� �����, ��� � ���� �� ����� ��� ���� ����������� ��� ���������� ������������, � � � �������� ��������� �� ���.";
				link.l1.go = "disease_mission_completed";
			}
			else
			{
				link.l1 = "� �� ���������� � ����. � ������ ��������� �����������, � ������ ��������� ���� �������.";
				link.l1.go = "exit";
				DiseaseMissionInProgress(npchar);
			}
		break;

		case "disease_mission_refused":
			dialog.text = "�� ���� ��� ������ � ����. ��� �� ��������.";
			link.l1 = "��������.";
			link.l1.go = "exit";
		break;

		case "disease_mission_completed":
			if (CheckForDisease(npchar))
			{
				dialog.text = "�� ������ �����! ��������� ������� ��� ��� �����, � ���� ��������������� ������� ����� �������.";
				link.l1 = "�������.";
				link.l1.go = "exit";
				DiseaseMissionCompleted(npchar, 0);
			}
			else
			{
				dialog.text = "� ������� ��� ���������������, �� �� ������ �������� ������� ������ ������.";
				link.l1 = "��� ��, �� ��� � ���� ���.";
				link.l1.go = "exit";
				DiseaseMissionCompleted(npchar, 1);
			}
		break;

		///////////////////////////////////////////////////////
		/// ������ ������
		///////////////////////////////////////////////////////
		case "taking_quest_line":
			dialog.text = SelectHovernorQuestText(npchar, &sAgreeLink, &sDeclineLink);
			link.l1 = "���������� �����������. � ������ �� ��� �������.";
			link.l1.go = sAgreeLink;
			link.l2 = "����� ��� ��� �� ��������.";
			link.l2.go = sDeclineLink;
		break;

		///////////////////////////////////////////////////////
		/// ����� �� ������ �����
		///////////////////////////////////////////////////////
		case "capture_fort_accept":
			dialog.text = "�� ������� ��� ���.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptCaptureFortQuest(npchar);
		break;

		case "capture_fort_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
		break;

		///////////////////////////////////////////////////////
		/// ����� �� �������� ��������
		///////////////////////////////////////////////////////
		case "delivery_item_accept":
			dialog.text = "����� � ������� ��� �������� ��������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptDeliveryItemQuest(npchar);
		break;

		case "delivery_item_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
		break;

		///////////////////////////////////////////////////////
		/// ����� �� ������������� �����
		///////////////////////////////////////////////////////
		case "escort_ship_accept":
			dialog.text = "����� � ������� ��� �������� ��������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptEscortQuest(npchar);
		break;

		case "escort_ship_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
		break;
		
		///////////////////////////////////////////////////////
		/// ����� �� ����������/������ �����
		///////////////////////////////////////////////////////
		case "sink_ship_accept":
			dialog.text = "����� � ������� ��� �������� ��������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptSinkShipQuest(npchar);
		break;

		case "sink_ship_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
		break;

		///////////////////////////////////////////////////////
		/// ������������ �����
		///////////////////////////////////////////////////////
		case "delivery_passenger_accept":
			dialog.text = "����� � ������� ��� �������� ��������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptDeliveryPassengerQuest(npchar);
		break;

		case "delivery_passenger_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
			DeletePassengerQuest(npchar);
		break;

		///////////////////////////////////////////////////////
		/// �������� �����
		///////////////////////////////////////////////////////
		case "delivery_trade_accept":
			dialog.text = "����� � ������� ��� �������� ��������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			AcceptDeliveryTradeQuest(npchar);
		break;

		case "delivery_trade_decline":
			dialog.text = "����� ��� �������� ��� ������������ - ������ ������� � ���� ���.";
			link.l1 = "������, � ����� ������ � ����� �������..";
			link.l1.go = "exit";
		break;

		///////////////////////////////////////////////////////
		/// �������� ���������� �������
		///////////////////////////////////////////////////////
		case "check_quests_lines":
			if(CheckAttribute(npchar, "quest.punishment"))
			{
				if(sti(npchar.quest.punishment) > 0)
				{
					dialog.text = "��� ��, ���� �� � ����������� �� " + FindRussianDaysString(sti(npchar.quest.punishment)) + ", � ����� � ��� � �������� ��������� ��� ������� �����, � ����� � ������� �����������.";
					link.l1 = "������ � ����� �������.";
					link.l1.go = "quest_done_prepare_exit";
				}
				else
				{
					dialog.text = "� ���, ��� �� ������� ��������� ����������� �� ��� ���������, � ��������� ��� �������. ��������� ������������� ��� �� ������� ������.";
					link.l1 = "��������� ��� �� ����� ������� ������ ���� ��������.";
					link.l1.go = "quest_done_prepare_exit";
				}
			}
			else
			{
				dialog.text = "� ���, ��� �� ������� ��������� ����������� �� ��� ���������, � ��������� ��� �������. ��������� ������������� ��� �� ������� ������.";
				link.l1 = "��������� ��� �� ����� ������� ������ ���� ��������.";
				link.l1.go = "quest_done_prepare_exit";
			}
		break;

		case "reminder_quests_lines":
			dialog.text = SelectHovernorQuestReminderText(npchar);
			link.l1 = "� ��� ��� ������� ��� ����.";
			link.l1.go = "exit";
		break;

		///////////////////////////////////////////////////////
		/// ����������� �� ������
		///////////////////////////////////////////////////////
		case "service":
			if (CheckAttribute(pchar, GetNationNameByType(sti(npchar.nation)) + "_line_counter"))
			{
				dialog.text = "���� ��� �� �������� ������, �� ��� �������� ������ ���� �������\n�� ��� �����������, ����� ����, ��� �� ���� �������� � ���������������.";
				link.l1 = "��... ����� ����, ����� ����� �������� ���� � ��� �����.";
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "�������! �� ��������� � ����� ����� ��� ��.";
				link.l1 = "����� ������ ��� ���������� ���������, ����� ������� ������������?";
				link.l1.go = "service_2";
			}
		break;
		
		case "service_2":
			dialog.text = "������ ���������, ������� ��� ������� ���� �������. ��� ���� ����� ��������� ����� �������, �� ������ ������� �������, �� ��������� ��� ������������, � ������������ ���� ������.";
			link.l1 = "��� ��� ��������. ������� ������.";
			link.l1.go = "service_3";
			link.l2 = "�! �����, ��� ���������� �����, ����� ������� ������������� �������.";
			link.l2.go = "exit";
		break;
		
		case "service_3":
			dialog.text = "��� �������� ���� �������������! ������ ��� �������� �������� ���� ���������� �� ����\n��� ���, ����� �� ������, ��� ������ ��� ����� �� ������ ��� ��������� �����, ������� �� ��� �, ��������, � ���� �������� ��� ��� ����.";
			link.l1 = "�������.";
			link.l1.go = "service_exit";
		break;

		case "service_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			PlayerChangeNation(npchar);
		break;

		case "quest_done_prepare_exit":
			CharacterDoneQuest(npchar);
			if(CheckForNextRank(npchar) > 0)
			{
				dialog.text = "��������� ���������� ��� � ��������� �������, �������!";
				link.l1 = "��������� ��� �� �������� �������.";
				link.l1.go = "quest_done_exit";
				PlayerNextRank(npchar);
			}
			else
			{
				NextDiag.CurrentNode = NextDiag.TempNode;
				DialogExit();
			}
		break;

		case "quest_done_exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			//CharacterDoneQuest(npchar);
		break;
		
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;		
	}
}
