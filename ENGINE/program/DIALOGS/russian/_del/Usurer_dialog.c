//nclude "DIALOGS\Usurer_dialog.h"
void ProcessDialogEvent()
{
	int LoanSum, LoanInterest, LoanPeriod, LoanResult, iPastMonths, DepositSum, DepositInterest, DepositResult;
	ref NPChar, d;

	aref Link, Diag;
	string NPC_Area;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makeref(d, Dialog);
	makearef(Diag, NPChar.Dialog);

	NPC_Area = Npchar.quest.area;

	switch(Dialog.CurrentNode)
	{
		// -----------------------------------������ ������ - ������ �������
		case "LoanGranted_exit":
			Diag.CurrentNode = Diag.TempNode;

			AddMoneyToCharacter(Pchar, makeint(Pchar.Quest.Loans.(NPC_Area).Sum));
			Pchar.Quest.Loans.(NPC_Area) = true;
			Pchar.Quest.Loans.(NPC_Area).StartDay = getDataDay();
			Pchar.Quest.Loans.(NPC_Area).StartMonth = getDataMonth();
			Pchar.Quest.Loans.(NPC_Area).StartYear = getDataYear();
			Pchar.Quest.Loans.(NPC_Area).StartTime = getTime();

			Pchar.quest.(NPC_Area).win_condition.l1 = "Timer";
			Pchar.quest.(NPC_Area).win_condition.l1.date.day = getDataDay();
			Pchar.quest.(NPC_Area).win_condition.l1.date.month = getDataMonth() + makeint(Pchar.Quest.Loans.(NPC_Area).Period);
			Pchar.quest.(NPC_Area).win_condition.l1.date.year = getDataYear();
			Pchar.quest.(NPC_Area).win_condition = "Loans_" + (NPC_Area);

			DialogExit();

		break;

		case "Deposit_exit":
			Diag.CurrentNode = Diag.TempNode;

		//	DepositSum = -DepositSum;
			AddMoneyToCharacter(Pchar, -(makeint(Pchar.Quest.Deposits.(NPC_Area).Sum)));
			Pchar.Quest.Deposits.(NPC_Area) = true;
			Pchar.Quest.Deposits.(NPC_Area).StartDay = getDataDay();
			Pchar.Quest.Deposits.(NPC_Area).StartMonth = getDataMonth();
			Pchar.Quest.Deposits.(NPC_Area).StartYear = getDataYear();
			Pchar.Quest.Deposits.(NPC_Area).StartTime = getTime();
			DialogExit();
		break;

		case "exit":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
			DeleteAttribute(Pchar, "quest.Loans." + (NPC_Area));
			DeleteAttribute(Pchar, "quest.Deposits." + (NPC_Area));
		break;

		case "exit_NoChange":
			Diag.CurrentNode = Diag.TempNode;
			DialogExit();
		break;


		case "First time":
			if(NPChar.quest.meeting == "0")
			{
				NPChar.quest.meeting = 1;
				Dialog.snd1 = "voice\USDI\USDI001";
				Dialog.snd2 = "voice\USDI\USDI002";
				Dialog.snd3 = "voice\USDI\USDI003";
				d.Text = RandPhrase(TimeGreeting() + ", ���  ���� ��� ������?", "��� �������������� ���, " + address_form.eng + ". ��� ����� ��� ������?", "������������, " + address_form.eng + ". �������, � �� ���� ����� ��� �����...", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
				link.l1 = LinkRandPhrase("���� ����� " + Pchar.name + " " + Pchar.lastname + ".", "������� " + Pchar.name + " " + Pchar.lastname + " � ����� �������.", "� " + PChar.name + ".");
				link.l1.go = "meeting";
			}
			else
			{
					Dialog.snd1 = "voice\USDI\USDI004";
					Dialog.snd2 = "voice\USDI\USDI005";
					Dialog.snd3 = "voice\USDI\USDI006";
					d.Text = RandPhrase("��� ������ ���, ������� " + PChar.name + ". ���� ��� ����� ��� ������, �� � ����� ��� �� ������� ����������.", TimeGreeting() + ". �� �� ����?", "������������, " + address_form.eng + ". ��� ��� ������? � ����� ����������� ����� ���������� ��������.", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
					link.l1 = LinkRandPhrase("� ���� ������ �����.", "��� ������ ������������ ����-������ ��������.", "��� ������ ���������� �������?");
					link.l1.go = "loan";
					if(makeint(Pchar.money) >= 100)
					{
						link.l2 = LinkRandPhrase("� �� ����� ������ ������ � ����.", "������� �� �������� ��������� ������?", "��������� �� ������ ���� �������� �����?");
						link.l2.go = "deposit";
					}
					link.l3 = "�� ��������, " + NPchar.name + ".";
					link.l3.go = "exit";

				if(CheckAttribute(Pchar, "Quest.Loans." + (NPC_Area)) && makeint(Pchar.Quest.Loans.(NPC_Area)) == true)
				{
					iPastMonths = GetPastTime("Month", makeint(Pchar.Quest.Loans.(NPC_Area).StartYear),makeint(Pchar.Quest.Loans.(NPC_Area).StartMonth),makeint(Pchar.Quest.Loans.(NPC_Area).StartDay), makefloat(Pchar.Quest.Loans.(NPC_Area).StartTime), getDataYear(),getDataMonth(),GetDataDay(), GetTime());
					Pchar.Quest.Loans.(NPC_Area).Result = makeint(Pchar.Quest.Loans.(NPC_Area).Sum) + ((makeint(Pchar.Quest.Loans.(NPC_Area).Sum)/100)*makeint(Pchar.Quest.Loans.(NPC_Area).Interest))*iPastMonths;

					Dialog.snd1 = "voice\USDI\USDI007";
					Dialog.snd2 = "voice\USDI\USDI008";
					Dialog.snd3 = "voice\USDI\USDI009";

					d.Text = RandPhrase(TimeGreeting() + "," + PChar.name + ". �� ������, ����� �� ���� ������������? ���... ���, �� ������ ��� " + Pchar.Quest.Loans.(NPC_Area).Result + " ��������.", "������������, �������. ��� ������ ��� � ������ �������. ������ �� �� ������� �� " + Pchar.Quest.Loans.(NPC_Area).Result + " ��������, ��� �������� � ����?", "����������� ���, " + address_form.eng + ". �������, ���� �������� ���� �������, � �� �������� ��� ��� ������. ������ ��������, �� ������ ������ ��� " + Pchar.Quest.Loans.(NPC_Area).Result + " ��������.", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
					if(makeint(PChar.money) >= makeint(Pchar.Quest.Loans.(NPC_Area).Result))
					{
						link.l1 = LinkRandPhrase("� ����� ������� ��� ���� ������.", "� � ��������� ��������� ��� ����.", "������� �����, �������, ������������.");
						link.l1.go = "loan_return";
					}
					link.l2 = LinkRandPhrase("� ���������, � ���� �� ���� ����� �������.", "� ��������� ���, " + NPChar.name + ".", "���, ���. � �������, ����� ������� ������.");
					link.l2.go = "Loan_Remind";
					link.l3 = "";
					link.l3.go = "exit_NoChange";

				}
				if(CheckAttribute(Pchar, "Quest.Deposits." + (NPC_Area)) && makeint(Pchar.Quest.Deposits.(NPC_Area)) == true)
				{
					iPastMonths = GetPastTime("Month", makeint(Pchar.Quest.Deposits.(NPC_Area).StartYear),makeint(Pchar.Quest.Deposits.(NPC_Area).StartMonth),makeint(Pchar.Quest.Deposits.(NPC_Area).StartDay), makefloat(Pchar.Quest.Deposits.(NPC_Area).StartTime), getDataYear(),getDataMonth(),GetDataDay(), GetTime());
					Pchar.Quest.Deposits.(NPC_Area).Result = makeint(Pchar.Quest.Deposits.(NPC_Area).Sum) + ((makeint(Pchar.Quest.Deposits.(NPC_Area).Sum)/100)*makeint(Pchar.Quest.Deposits.(NPC_Area).Interest))*iPastMonths;
					Dialog.snd1 = "voice\USDI\USDI010";
					Dialog.snd2 = "voice\USDI\USDI011";
					Dialog.snd3 = "voice\USDI\USDI012";
					d.Text = RandPhrase("��� ������ ���, ������� " + PChar.name + ". ���� ��� ����� ��� ������, �� � ����� ��� �� ������� ����������.", TimeGreeting() + ". �� �� ����?", "������������, " + address_form.eng + ". ��� ��� ������? � ����� ����������� ����� ���������� ��������.", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
					link.l1 = LinkRandPhrase("� ���� ������� ���� �����.", "������ ����� ������� ��� ������, ��� � ��� ��������.", "��� ����� ��� ������ � ���������� ��������.");
					link.l1.go = "Deposit_return";
					link.l2 = "�� ��������, " + NPchar.name + ".";
					link.l2.go = "exit_NoChange";
					link.l3 = "";
					link.l3.go = "exit_NoChange";
				}
			}
		break;

		case "Meeting":
			Dialog.snd1 = "voice\USDI\USDI013";
			Dialog.snd2 = "voice\USDI\USDI014";
			Dialog.snd3 = "voice\USDI\USDI015";
			d.Text = RandPhrase("����� �������. � " + NPChar.name + " " + NPChar.lastname + ". ��� ������� - �������������� ����� � ���� ��� ��������.", "��� ��� " + NPChar.name + " " + NPChar.lastname + ". � ���������. ���� ��� �������� ����������� �������� - � ���� ��� ��� ������.", "��������� �� ��� ������, ��� ���� ����� " + NPChar.name + " " + NPChar.lastname + ". � �������� ��������� ����� ��������...�������, �������� � ���� - ��� ����� ��� ������?", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
			link.l1 = LinkRandPhrase("� ���� ������ �����.", "��� ������ ������������ ����-������ ��������.", "��� ������ ���������� �������?");
			link.l1.go = "loan";
			if(makeint(Pchar.money) >= 100)
			{
				link.l2 = LinkRandPhrase("� �� ����� ������ ������ � ����.", "������� �� �������� ��������� ������?", "��������� �� ������ ���� �������� �����?");
				link.l2.go = "deposit";
			}
			link.l3 = "�� ��������, " + NPchar.name + ".";
			link.l3.go = "exit";
		break;

		case "loan":
			Dialog.snd = "voice\USDI\USDI016";
			d.Text = "�, ������ ����������. ����� ����� ��� ����������?";
			link.l1 = "������ ���������.";
			link.l1.go = "Small";
			link.l2 = "������ ������������.";
			link.l2.go = "Medium";
			link.l3 = "��� ������, ��� �����.";
			link.l3.go = "Large";
		break;

		case "small":
			Pchar.Quest.Loans.(NPC_Area).Sum = 500*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI017";
			d.Text = "�������. � ���������� ������� ������ ����� - ������ �����. � ���� ���������� ��� " + Pchar.Quest.loans.(NPC_Area).sum + " ��������...";
			link.l1 = "������������. ��� ����� ��������?";
			link.l1.go = "Interest";
			link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			link.l2.go = "Loan";
			link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			link.l3.go = "Exit";
		break;

		case "Medium":
			Pchar.Quest.Loans.(NPC_Area).Sum = 1500*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI018";
			d.Text = "�� ��������. �������, ��� " + Pchar.Quest.Loans.(NPC_Area).sum + " �������� ������� ��� ������ ���� ��������. ��� ������ ������������ �����.";
			link.l1 = "������������. ��� ����� ��������?";
			link.l1.go = "Interest";
			link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			link.l2.go = "Loan";
			link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			link.l3.go = "Exit";
		break;

		case "Large":
			Pchar.Quest.Loans.(NPC_Area).Sum = 3000*makeint(Pchar.rank);
			Dialog.snd = "voice\USDI\USDI019";
			d.Text = "�-��... ����������... �� �����, � ����� ������������ ��� ���� �� ����� " + Pchar.Quest.Loans.(NPC_Area).sum + ". ��� �� ���������, �������, ��� ������������� ������� ������. ����� ����� ��� �������� ��������� � ����� ����.";
			link.l1 = "������������. ��� ����� ��������?";
			link.l1.go = "Interest";
			link.l2 = "���, ��� ����� �� �������. �������-�� �� �������.";
			link.l2.go = "Loan";
			link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			link.l3.go = "Exit";
		break;

		case "Interest":
			Pchar.Quest.Loans.(NPC_Area).Interest = 11 - makeint(Pchar.skill.commerce);
			Dialog.snd = "voice\USDI\USDI020";
			d.Text = Pchar.Quest.Loans.(NPC_Area).Interest + " � �����. ����������� �� ���, ��� � � ��� ����, � �� ���� ���������� ��� ������ ������ �������.";
			link.l1 = "���� ��� ������ ����������. �������� ���������� ����.";
			link.l1.go = "Period";
			link.l3 = "�����, ��� ����� �� ������� � �����. �� ��������.";
			link.l3.go = "Exit";
		break;

		case "Period":
			Pchar.Quest.Loans.(NPC_Area).Period = makeint(makeint(Pchar.reputation)/20) + 1;
			Dialog.snd = "voice\USDI\USDI021";
			d.Text =  "�������� ��, ��� � � ��� ������, " + Pchar.name + ", � ���� ����� �������� ����� ����� � ������� " + Pchar.Quest.Loans.(NPC_Area).Period + " �������. ����� ����, ��� �� ���������, � ����� ����.";
			link.l1 = "��� ��, � � ������������� �������� ���� �������... � ���� ������.";
			link.l1.go = "LoanGranted";
			link.l3 = "���, �� � ���� �� ������������. �� ��������.";
			link.l3.go = "Exit";
		break;


		case "LoanGranted":
			Dialog.snd = "voice\USDI\USDI022";
			d.Text = "�������� ����� ���. �� ��������� ��� ��� ������������. � ����� ��������� ����� ����� � ����, ��� ������� ���� ������. ��� ��� ���� � ��� � ���� ����� ����� ������������ ��� ������, ����� �������� ��\n�� ���� ��� ������� - ������ ������������.";
			link.l1 = "��... ��-��. ����� ��������.";
			link.l1.go = "LoanGranted_exit";
		break;

		case "Loan_Remind":
			Dialog.snd = "voice\USDI\USDI023";
			d.Text = "�� ��������... �������� ������. �� � ������� � ��� �������� �� ��� �� �����.";
			link.l1 = "�� ����������. �� �������.";
			link.l1.go = "exit_NoChange";
		break;

		case "loan_return":
			addMoneyToCharacter(Pchar, -(makeint(Pchar.Quest.Loans.(NPC_Area).Result)));
			DeleteAttribute(PChar, "quest.loans." + (NPC_Area));
			Pchar.quest.(NPC_Area).over = "Yes";
			Dialog.snd1 = "voice\USDI\USDI024";
			Dialog.snd2 = "voice\USDI\USDI025";
			Dialog.snd3 = "voice\USDI\USDI026";
			d.Text = RandPhrase("�, ��� ������ �������! � ����� �����, ��� ��� ����������� ������ - � � ����� �������.", "���������. ����� ������� ����� ���� � ������� ���������, ������� ������� ������� ����. � �� ������, ������ ������...",  "�, �� ��� � ��� � ��� ����� ������, " + address_form.fra + "! �������, ��� �� ��������� ���, ����� �� ���������� � ���� �������!", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
			link.l1 = LinkRandPhrase("� ���� ������ �����.", "��� ������ ������������ ����-������ ��������.", "��� ������ ���������� �������?");
			link.l1.go = "loan";
			if(makeint(PChar.money) >= 100)
			{
				link.l2 = LinkRandPhrase("� �� ����� ������ ������ � ����.", "������� �� �������� ��������� ������?", "��������� �� ������ ���� �������� �����?");
				link.l2.go = "deposit";
			}
			link.l3 = "�� ��������, " + NPchar.name + ".";
			link.l3.go = "exit";
		break;

		case "deposit":
			Dialog.snd1 = "voice\USDI\USDI027";
			Dialog.snd2 = "voice\USDI\USDI028";
			Dialog.snd3 = "voice\USDI\USDI029";
			d.Text = RandPhrase("� ����� ��������� ��������! ��� ������� ����� �� ������ ������ � ����?", "������. �������� ���, �� �����, ��� �� ��� �������, ����� ����� ��� � ������� � �����������, � ���� � ����������...�, ������, ������ ��� �����?", "� ������, �� ���������, ��� � ���� � ���� �����. ������� �� ������ ������ ��� �� ��������?", &Dialog, Dialog.snd1, Dialog.snd2, Dialog.snd3);
			link.l1 = "�������� - " + makeint(makeint(PChar.money)/40)*10 + " ��������.";
			link.l1.go = "quarter";
			link.l2 = "�������� - " + makeint(makeint(PChar.money)/20)*10 + " ��������.";
			link.l2.go = "half";
			link.l3 = "���, ��� � ���� ���� - " + makeint(makeint(PChar.money)/10)*10 + " ��������.";
			link.l3.go = "All";
		break;

		case "quarter":
			Pchar.Quest.Deposits.(NPC_Area).Interest = makeint(Pchar.skill.commerce) + 1;
			Pchar.Quest.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/40)*10;
			Dialog.snd = "voice\USDI\USDI030";
			d.Text = "������. ��������� ��� ��� ����� � ����� ��� ����������...��...������, " + Pchar.Quest.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			link.l1 = "���� ��� ����������";
			link.l1.go = "Deposit_placed";
			link.l2 = "����� � ������ �����.";
			link.l2.go = "Deposit";
			link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			link.l3.go = "Exit";
		break;

		case "half":
			Pchar.Quest.Deposits.(NPC_Area).Interest = makeint(Pchar.skill.commerce) + 1;
			Pchar.Quest.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/20)*10;
			Dialog.snd = "voice\USDI\USDI031";
			d.Text = "������. ��������� ��� ��� ����� � ����� ��� ����������... ��... ������, " + Pchar.Quest.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			link.l1 = "���� ��� ����������";
			link.l1.go = "Deposit_placed";
			link.l2 = "����� � ������ �����.";
			link.l2.go = "Deposit";
			link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			link.l3.go = "Exit";
		break;

		case "All":
			Pchar.Quest.Deposits.(NPC_Area).Interest = makeint(Pchar.skill.commerce) + 1;
			Pchar.Quest.Deposits.(NPC_Area).Sum = makeint(makeint(PChar.money)/10)*10;
			Dialog.snd = "voice\USDI\USDI032";
			d.Text = "������. ��������� ��� ��� ����� � ����� ��� ����������...��...������, " + Pchar.Quest.Deposits.(NPC_Area).Interest + ". ����������, � �����.";
			link.l1 = "���� ��� ����������";
			link.l1.go = "Deposit_placed";
			link.l2 = "����� � ������ �����.";
			link.l2.go = "Deposit";
			link.l3 = "�������, ��� �� ����� ������������ � ��������. ����� ��������.";
			link.l3.go = "Exit";
		break;

		case "Deposit_placed":
			Dialog.snd = "voice\USDI\USDI033";
			d.Text = "������ �� ������ ���� �������, ��� ���� ����� �������� ���� �� ������� ��� ��� ������� � �������������. ���� �� �� ������ ������� �����, �� ��������� � ����� �����.";
			link.l1 = "������. ����� ��������.";
			link.l1.go = "Deposit_Exit";
		break;

		case "Deposit_return":
			Dialog.snd = "voice\USDI\USDI034";
			d.Text = "������ �� ���������, ������� �� ��������� � ������� ���, �, �������� ��������� �����, � ��������, ��� ������ ��� " + Pchar.Quest.Deposits.(NPC_Area).Result + " ��������... �� �������, ��� ������ ������� ��� ������";
			link.l1 = "���������. ������� �� ����.";
			link.l1.go = "Deposit_return_1";
			link.l2 = "������, �� �����. ����� ��� ��� ������� �������. ����� ��������.";
			link.l2.go = "Exit_noChange";
		break;

		case "Deposit_return_1":
			addMoneyToCharacter(Pchar, makeint(Pchar.Quest.Deposits.(NPC_Area).Result));
			DeleteAttribute(PChar,"quest.deposits." + (NPC_Area));
			Dialog.snd = "voice\USDI\USDI035";
			d.Text = "��... ���� ���� � ���� ������������. � � ��� ��� ���-�� ������. ��� �� - ��������� �� ��� ���.";
			link.l1 = "���� ����������� - �����. ��������� ����������.";
			link.l1.go = "Exit";
		break;

		case "DeadMotherfucker":
			Dialog.Tempnode = "DeadMotherFucker_1";
			Dialog.snd = "voice\USDI\USDI035";
			d.Text = "�� ��� ����? ��������, ������ ������� ��� ������? � �����! ��-�� ��� � ������� �������, ������� �� ��� � ����� �� ����������! � �� ���������, ���� �� ����� ��� ����!";
			link.l1 = "� ��������� ����������� �� ��� ����� ������ ����� ������!";
			link.l1.go = "Exit";
		break;

		case "DeadMotherFucker_1":
			Dialog.snd = "voice\USDI\USDI035";
			d.Text = "���������� �� ����� ����! ����� ��� ��� �� �� ������!";
			link.l1 = "...";
			link.l1.go = "Exit";
		break;
	}
}

