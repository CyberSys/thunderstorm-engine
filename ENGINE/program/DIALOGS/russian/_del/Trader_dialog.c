
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	string sAgreeLink, sDeclineLink;

	int iSuccesDelivery;

	switch(Dialog.CurrentNode)
	{
		case "First time":
			if(0)//  to_do CheckAttribute(npchar, "quest.questflag"))
			{
				dialog.text = SelectDeliveryTradeDialogQuestion(npchar);
				if(sti(npchar.quest.questflag) == 2)//����� �����
				{
					link.l1 = "��, �������.";
					link.l1.go = "delivery_trade_succesfull";
				}
				if(sti(npchar.quest.questflag) == 3)//��������� �����
				{
					if(Quests[sti(npchar.currentquest)].l.target_npc == npchar.id)
					{
						if(sti(Quests[sti(npchar.currentquest)].l.freespace) > 0 && sti(Quests[sti(npchar.currentquest)].l.isdelayed) == 1)
						{
							link.l1 = "� ���������, �� ������ ������ ��� ����� ���������� ����.";
							link.l1.go = "delivery_trade_decline";
						}
						else
						{
							link.l1 = "��������� �� ����� ����� ����������� � ����... � �����, � ��� �� ����� ������������ ��� ���.";
							link.l1.go = "delivery_trade_decline";
						}
					}
					else
					{
						dialog.text = SelectHovernorQuestReminderText(npchar);
						link.l1 = "���� ������� ��� �� ����������, �� � ������� ��� ����. � ���� �������� ���� ������.";
						link.l1.go = "store";
						link.l2 = "�������, ��� ���������. � ������ �� ������� ����� �����.";
						link.l2.go = "exit";
					}
				}
				if(sti(npchar.quest.questflag) != 2 && sti(npchar.quest.questflag) != 3)
				{
					dialog.text = "����������� ��� � ����� ��������. ������ ��������� �� ��� ������?";
					link.l1 = "�������, ��� � ���� ����?";
					link.l1.go = "trade";
					if(sti(npchar.quest.questflag) == 1)
					{
						link.l2 = "����� ���� � ������ ���, ����� ����� ��������� � �����.";
						link.l2.go = "quests";
					}
					link.l3 = "� ���� ��� �� ��� �������.";
					link.l3.go = "exit";
				}
			}
			else
			{
				dialog.text = "����������� ��� � ����� ��������. ������ ��������� �� ��� ������?";
				link.l1 = "�������, ��� � ���� ����?";
				link.l1.go = "trade";
				if(CheckAttribute(npchar, "quest.questflag"))
				{
					if(sti(npchar.quest.questflag) == 1)
					{
						link.l2 = "����� ���� � ������ ���, ����� ����� ��������� � �����.";
						link.l2.go = "quests";
					}
				}
				link.l3 = "��� �� ��� �������...";
				link.l3.go = "exit";
			}

			NextDiag.TempNode = "First time";
		break;

		case "trade":
			dialog.text = "� ���� ������� ����������� ��������� �������, �������.";
			link.l1 = "������ ��� ���� ������, ��������.";
			link.l1.go = "store";
			link.l2 = "������,  � ���� ������� ������, ����� ������ ����, ��� �������� �� ���� ������.";
			link.l2.go = "exit";
		break;

		case "store":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
			LaunchStore(sti(NPChar.StoreNum));
		break;

		case "delivery_trade_succesfull":
			if(sti(Quests[sti(npchar.currentquest)].l.isdelayed) == 1)
			{
				dialog.text = "����� � ������ �� ����� ������ � ������ �������� ������� �� ��� �������.";
				link.l1 = "�������! ��� �� �������� ����������� � ���� ���������.";
				link.l1.go = "exit";
				PrepareAcceptDeliveryQuest(npchar);
			}
			else
			{
				if(sti(Quests[sti(npchar.currentquest)].l.contribution) > 0)
				{
					dialog.text = "��.. ���������, ���������... �� �������� �� " + FindRussianDaysString(sti(Quests[sti(npchar.currentquest)].l.contribution)) + ", � ������� � �������� ��������� ��� ������� �����, ��� ���� ��������� � ���������.";
					link.l1 = "�� ������� � ���� ���� �����? ����, ������� ���� ���� ������ � ����������� ��������� ���������� � ���������.";
					link.l1.go = "exit";
					CompleteDeliveryTradeQuest(npchar);
				}
				else
				{
					dialog.text = "��� ��, � ����� ������, ���� �� �� ����������, � ������� ����� ����� ���������� � ��������� ������ �������.";
					link.l1 = "������� ����������.";
					link.l1.go = "exit";
					CompleteDeliveryTradeQuest(npchar);
				}
			}
		break;

		case "delivery_trade_decline":
			dialog.text = "����� ����.";
			link.l1 = "� �� ���-�� ��� ����, ��, ����� ����, �� �������� ��� ���� ������?";
			link.l1.go = "trade";
            link.l2 = "�����, ��������� � ��������� ���.";
			link.l2.go = "exit";
		break;

		case "quests":
			dialog.text = "��, � ��� ����������� ������, �������.";
			link.l1 = "��� �� � ��� �����-������ ��������� ��� ���� � ����� �����?";
			link.l1.go = "generate_quests";

			iSuccesDelivery = CheckForDeliverySuccess(npchar, "");
			if(iSuccesDelivery == 2)
			{
				if(npchar.id == Quests[sti(npchar.currentquest)].l.target_npc)
				{
					if(sti(Quests[sti(npchar.currentquest)].l.isdelayed) == 0)
					{
						if(CheckAttribute(npchar, "quest.punishment"))
						{
							if(sti(npchar.quest.punishment) > 0)
							{
								link.l2 = "���� ��� ��������� ����, ������� �� ��������.";
								link.l2.go = "delivery_trade_success_punishment";
							}
							else
							{
								link.l2 = "���� ��� ��������� ����, ������� �� ��������.";
								link.l2.go = "delivery_trade_success";
							}
						}
						else
						{
							link.l2 = "���� ��� ��������� ����, ������� �� ��������.";
							link.l2.go = "delivery_trade_success";
						}
					}
					else
					{
						link.l3 = "��� �������� ������� � ��� ����...";
						link.l3.go = "delay_delivery_cargo_load";
					}
				}
			}
		break;

		case "delay_delivery_cargo_load":
            dialog.text = "��, ��, �������! � ��� ��� ����� ���. ������ � ����� ������ ���� ����� ������ ��������.";
			link.l1 = "�������! ��� ������ �������� ���������� � ����������� � �������� ����.";
			link.l1.go = "exit";
			PrepareAcceptDeliveryQuest(npchar);
		break;

		case "generate_quests":
			dialog.text = SelectHovernorQuestText(npchar, &sAgreeLink, &sDeclineLink);
			link.l1 = "������� �����������, � ������ �� ��� ����.";
			link.l1.go = sAgreeLink;
			link.l2 = "�����, ��� ��� �� ��������.";
			link.l2.go = "exit";
		break;

		case "delivery_trade_accept":
			dialog.text = "������ �� �����, �������? ������ ���� ��������� ������� - �� ������ ������ ��������� ���� �� " + FindRussianDaysString(sti(Quests[sti(npchar.CurrentQuest)].l.time)) + ". � ��������� ������, �� ������ ���� ��������� �� ������ ������ 5 ��������� ������ �� ��������� ��� �����.";
			link.l1 = "��-��, � ����, ���� ������� � �� ������ �� �������� �������������.";
			link.l1.go = "exit";
			AcceptDeliveryTradeQuest(npchar);
		break;

		case "delivery_trade_success":
			dialog.text = "�������! � �� � ��� ����� �����������. ��� ���� �������, �������.";
			link.l1 = "�������, � ������, �������� ����� ������ � ��� ���� � �������.";
			link.l1.go = "trade";
			link.l2 = "��������� ���, �������, �� � ���� ��� ��������.";
			link.l2.go = "exit";
			CompleteDeliveryTradeQuest(npchar);
		break;

		case "delivery_trade_success_punishment":
			dialog.text = "��... ��� ������, �� �� �������� �� " + FindRussianDaysString(sti(npchar.quest.punishment)) + "! � ����� � ��� � ������ �������� ��������� ����� ������, ��� ��� ����� ��������� �������� ������.";
			link.l1 = "������ ������ �����������. �� ��� �������� �� ����������� ���� ������.";
			link.l1.go = "trade";
			link.l2 = "� ������� ���� ���������. � ������ ������� �������� � ���� �����, ��� � �����.";
			link.l2.go = "exit";
			CompleteDeliveryTradeQuest(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
