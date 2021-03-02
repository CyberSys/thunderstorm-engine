
#define MAX_STORY_EVENTS	100;

object HistoryEvents[MAX_STORY_EVENTS];

void ActivateHistoryEvents()
{
	FillHistoryEvents();
	//RechargeHistoryEvents();
	
	
	string sQuest = "";
	int iday, imonth, iyear;
	for(int i = 0; i<MAX_STORY_EVENTS; i++)
	{
		if (CheckAttribute(&HistoryEvents[i], "text"))
		{
			sQuest = "history_event" + "_" + i;
			iday = sti(HistoryEvents[i].day);
			imonth = sti(HistoryEvents[i].month);
			iyear = sti(HistoryEvents[i].year);
			
			pchar.quest.(sQuest).win_condition.l1 = "timer";
			pchar.quest.(sQuest).win_condition.l1.date.day = iday;
			pchar.quest.(sQuest).win_condition.l1.date.month = imonth;
			pchar.quest.(sQuest).win_condition.l1.date.year = iyear;
			pchar.quest.(sQuest).win_condition.l1.date.hour = rand(23);
			pchar.quest.(sQuest).win_condition = "Historical_event";
			pchar.quest.(sQuest).number = i;
		}
	}
}

void CheckHistoryEvent(string sQuest)
{
	
	
	int iNumber = sti(pchar.quest.(sQuest).number);
	GenerateRumour("history_event", iNumber, 0, 0);
}

void FillHistoryEvents()
{
	int n = 0;
	
	HistoryEvents[n].text = "� ������ ��� �������� ��������� ������ �����, ���������� ������ ����������� �����.";
	HistoryEvents[n].day = 23;
	HistoryEvents[n].month = 10;
	HistoryEvents[n].year = 1642;
	n++;
	
	HistoryEvents[n].text = "�� ������� ��������� �������� ������� (����� ��� �� �����).";
	HistoryEvents[n].day = 4;
	HistoryEvents[n].month = 12;
	HistoryEvents[n].year = 1642;
	n++;
	
	HistoryEvents[n].text = "�� ������� ��������� ������ ������� XIII. ������ �������� �������� ����������� ������ ��������� � �������� ��� ���������� �������� XIV.";
	HistoryEvents[n].day = 14;
	HistoryEvents[n].month = 5;
	HistoryEvents[n].year = 1643;
	n++;
	
	HistoryEvents[n].text = "� �������� �������� ��� ����� ��������� ������������� ��������� ����� ����������� ������ ����� I.";
	HistoryEvents[n].day = 14;
	HistoryEvents[n].month = 6;
	HistoryEvents[n].year = 1645;
	n++;
	
	HistoryEvents[n].text = "������ ���������� �������� � ������� � ���� I ��� �������� ������ � ���������.";
	HistoryEvents[n].day = 23;
	HistoryEvents[n].month = 6;
	HistoryEvents[n].year = 1646;
	n++;
	
	HistoryEvents[n].text = "� �������� ���� �������� ������ �������� ������ ����� ���������� � �������� � ���� ����� I.";
	HistoryEvents[n].day = 17;
	HistoryEvents[n].month = 8;
	HistoryEvents[n].year = 1648;
	n++;
	
	HistoryEvents[n].text = "���������� ������������� ������� ��������, ����������� ����� �������������� �����.";
	HistoryEvents[n].day = 24;
	HistoryEvents[n].month = 10;
	HistoryEvents[n].year = 1648;
	n++;
	
	HistoryEvents[n].text = "�� ��������� ���� � ������ ��� ������ ������ ���� I. ������ ������������� �����������.";
	HistoryEvents[n].day = 30;
	HistoryEvents[n].month = 1;
	HistoryEvents[n].year = 1649;
	n++;
	
	HistoryEvents[n].text = "������ �������� �������� ���������� ��������� � ��������������� �����.";
	HistoryEvents[n].day = 20;
	HistoryEvents[n].month = 4;
	HistoryEvents[n].year = 1652;
	n++;
	
	HistoryEvents[n].text = "������ ������ ����� � ����������.";
	HistoryEvents[n].day = 29;
	HistoryEvents[n].month = 5;
	HistoryEvents[n].year = 1652;
	n++;
	
	HistoryEvents[n].text = "������ ��������� ��� ����������.";
	HistoryEvents[n].day = 20;
	HistoryEvents[n].month = 4;
	HistoryEvents[n].year = 1654;
	n++;
	
	HistoryEvents[n].text = "� ��������������� ������ ���������� ������������� ��������� ���������� �������� � ��������� �����-����������.";
	HistoryEvents[n].day = 16;
	HistoryEvents[n].month = 2;
	HistoryEvents[n].year = 1654;
	n++;
	
	HistoryEvents[n].text = "��������� �������� XIV.";
	HistoryEvents[n].day = 7;
	HistoryEvents[n].month = 6;
	HistoryEvents[n].year = 1654;
	n++;

	HistoryEvents[n].text = "� ���������� ������� ��������� ������ ��������.";
	HistoryEvents[n].day = 3;
	HistoryEvents[n].month = 9;
	HistoryEvents[n].year = 1658;
	n++;
	
	HistoryEvents[n].text = "�� �������� ������� �� ����������� ���� �������� ��� �������� ����������� ��� ����� �������� � ��������.";
	HistoryEvents[n].day = 7;
	HistoryEvents[n].month = 11;
	HistoryEvents[n].year = 1659;
	n++;
	
	HistoryEvents[n].text = "������� ������ ���� ������ ������ � ������ � ������������� ������� ����� II.";
	HistoryEvents[n].day = 29;
	HistoryEvents[n].month = 5;
	HistoryEvents[n].year = 1660;
	n++;
	
	HistoryEvents[n].text = "� ����������� ������� XIV ��������� ������ � ��������� �������� ������ ��������.";
	HistoryEvents[n].day = 3;
	HistoryEvents[n].month = 6;
	HistoryEvents[n].year = 1660;
	n++;
	
	HistoryEvents[n].text = "�� ������� ��������� �������� ������ �������� ��������. ������� XIV ����� ������ ��������� � ���� ����.";
	HistoryEvents[n].day = 9;
	HistoryEvents[n].month = 3;
	HistoryEvents[n].year = 1661;
	n++;
	
	HistoryEvents[n].text = "�������� ���� ������ � ������.";
	HistoryEvents[n].day = 10;
	HistoryEvents[n].month = 4;
	HistoryEvents[n].year = 1665;
	n++;
	
	HistoryEvents[n].text = "���� II ���������� ������� �������.";
	HistoryEvents[n].day = 1;
	HistoryEvents[n].month = 1;
	HistoryEvents[n].year = 1665;
	n++;
	
} 