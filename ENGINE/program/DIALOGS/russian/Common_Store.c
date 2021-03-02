// boal 08/04/04 ����� ������ ���������
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06

void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);
	

	string iDay, iMonth, sTemp, sMoney;
	
	iDay = environment.date.day;
	iMonth = environment.date.month;
	string lastspeak_date = iday + " " + iMonth;
	
	int iMoney;
	int iQuantityGoods;
	int iTradeGoods;
	int iTmp;
	
    bool  ok;

	int iTest;
    iTest = FindColony(NPChar.City); // ����� ��������
    ref rColony;
	if (iTest != -1)
	{
		rColony = GetColonyByIndex(iTest);
	}
	
    int iSeaGoods = LanguageOpenFile("ShipEatGood.txt"); // ����� �������� �� GetGoodsNameAlt(idx)

    if (!CheckAttribute(npchar, "quest.item_date"))
    {
        npchar.quest.item_date = "";
    }
    if (!CheckAttribute(npchar, "quest.trade_date"))
    {
        npchar.quest.trade_date = "";
    }
    
    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Store\" + NPChar.City + "_Store.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
    ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 25/06/06
	switch(Dialog.CurrentNode)
	{
		case "First time":
			if (LAi_grp_playeralarm > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("� ������ ������� �������, ���� ����� ����! �� ����� ����� � �� �� ���� ����� �������������.", "��� ��������� ������ ����� �� ������ � ������� ����. � �� ����� � ������������� � ����� �� ����!", "����, ��������, ���� ������� �� ������� �� ���� ������..."), 
					LinkRandPhrase("��� ���� �����, �������?! ��������� ������ ��� ����� ���� ����, ������ ���� �� ����, ������� �����!", "������� ������, ��� �� ����� ����! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("������ ���� �����, " + GetWorkTypeOfMan(npchar, "") + ", � �� �� ����� ���� ������� ����!", "���, " + GetWorkTypeOfMan(npchar, "") + ", � ��� ���� �� - ������� ������! ��� ��� � ���� �����, ��������: ���� ���� � ������ ����..."));
				link.l1.go = "fight";
				break;
			}
			//homo ������� �����
            if (Pchar.questTemp.CapBloodLine == true )
            {
                dialog.Text = LinkRandPhrase("��, ������ ����! " + TimeGreeting() + ".",
                                    "��� ������ ���, ����� ����.",
                                    "������, ��� �� ��������� �� ���, " + GetFullName(pchar) + ". ��� �������� ��������� �����?");
                Link.l1 = "���, � ��� �����, " + NPChar.name + ". �� �������.";
				Link.l1.go = "exit";
				NextDiag.TempNode = "First time";
				break;
            }
            dialog.text = NPCharRepPhrase(npchar, "����� ������ - ����� ������. "
							+ LinkRandPhrase("������ �� ����� ������. ", "������� �� ������. ","����� ������������ ��� �� ������! ")+
							+GetFullName(npchar)+  " � ����� �������!",
			                + LinkRandPhrase("������� ������������ � ����� ��������, ", "��� ������ �������, ", "����� ����������, ")
							+ GetAddress_Form(NPChar)+ ". ��� ��� "  + GetFullName(npchar)+
							+ RandPhraseSimple(", ��� ������� � ����� �������.",", � ��������� � ����� ������������."));
			link.l1 = "��������� �������������. " +RandPhraseSimple("���� ����� ","� - ")+ "������� "+ GetFullName(pchar)+".";
			link.l1.go = "node_1";
			NextDiag.TempNode = "Second time";
		break;

		case "second time":
			if (LAi_group_GetPlayerAlarm() > 0)
			{
       			dialog.text = NPCharRepPhrase(pchar, 
					LinkRandPhrase("� ������ ������� �������, ���� ����� ����! �� ����� ����� � �� �� ���� ����� �������������.", "��� ��������� ������ ����� �� ������ � ������� ����. � �� ����� � ������������� � ����� �� ����!", "����, ��������, ���� ������� �� ������� �� ���� ������..."), 
					LinkRandPhrase("��� ���� �����, �������?! ��������� ������ ��� ����� ���� ����, ������ ���� �� ����, ������� �����!", "������� ������, ��� �� ����� ����! ������!!", "� �� ����� ����, ��������! ����� ���� ������� � ����� �����, ������ ���� �� ����..."));
				link.l1 = NPCharRepPhrase(pchar,
					RandPhraseSimple("���, ������� ��� ���� �� ��������...", "�� ���� �� �� ��� �� �������."), 
					RandPhraseSimple("������ ���� �����, " + GetWorkTypeOfMan(npchar, "") + ", � �� �� ����� ���� ������� ����!", "���, " + GetWorkTypeOfMan(npchar, "") + ", � ��� ���� �� - ������� ������! ��� ��� � ���� �����, ��������: ���� ���� � ������ ����..."));
				link.l1.go = "fight";
				break;
			}
            /*if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY && sti(NPChar.nation) != PIRATE)
			{
				dialog.text = NPCharRepPhrase(npchar,
                     		  LinkRandPhrase("������� ���� ����! ", "������!!! ", "������ ������!!! ")
							+ LinkRandPhrase("���������� ","��������� ", "��������� ����� ")+ LinkRandPhrase("� ������! ","����� �� �����! ", "� �������! ") + RandPhraseSimple("������!", "�������!"),
							  LinkRandPhrase("��������� ����","������� ������", "������ ���������")+ "! "+ LinkRandPhrase("������! " ,"��������! ","�� �� �����! ")
							+ LinkRandPhrase("� ������ �����! ","��������� � ������! ","�������������! ")+"�������, ����!!");
				link.l1 = pcharrepphrase(LinkRandPhrase("������ �����!","������ �� ������!","�������!")+ " ��� � "
											  + LinkRandPhrase("����� ���� ���� � ������� ��� ����, ", "������ ���� �����, ","����� ���� ������ �� �������, ")+ LinkRandPhrase("�������!", "������!", "��������!"),
							                    LinkRandPhrase("���� ����, ", "����� ���, ", "���������� ")+LinkRandPhrase("�������� ���!", "���������� ��������, ��� ������ �� �����!", "����������, �������!")
											  + LinkRandPhrase(" ��� � �� ��� �� ���� ����� � ��������!"," ���� ��� ����� ���! � ������� �� ����! �� ������?"," ��� ��� �������� ��� ����� �� �����."));
				link.l1.go = "fight";
				break;
			}*/
   			// �������� �������
			/*if (sti(NPChar.nation) != PIRATE && ChangeCharacterHunterScore(Pchar, NationShortName(sti(NPChar.nation)) + "hunter", 0) >= 40)
			{
				dialog.text = NPCharRepPhrase(npchar,
                     LinkRandPhrase("������� ���� ����! ", "������!!! ", "������ ������!!! ")+GetFullName(pchar)+ ", �� �������� "
							+ LinkRandPhrase("'�����', ","'������', ", "'�����������', ") +LinkRandPhrase(" � ������! "," ����� �� �����! ", " � ������� �����! ") + RandPhraseSimple("������!", "�������!"),
							  LinkRandPhrase("��������� ����","������� ������", "������ ���������")+ "! ��� ��� "+GetFullName(pchar)+ LinkRandPhrase("! ������ " ,"! �������� ","! ������� ")
							+ LinkRandPhrase("����������! ","���� � ������! ","������� ����������! ")+"�������, ����!!");
				link.l1 =      pcharrepphrase(LinkRandPhrase("������ �����!","������ �� ������!","�������!")+ " � �� �� �"
											  + LinkRandPhrase(" ����� ���� ����, �������!", " ����� �� ���� �����!"," ���� ����� ������!"),
							                    LinkRandPhrase("���� ���� ���������!", "����� ���! �� �������!", "� ���� ���� ���� ���?")
											  + LinkRandPhrase(" � ��� �����!"," ���� ��� ����� ���!"," � �� ���� ��� �������!"));
				link.l1.go = "fight";
				break;
			}    */
			dialog.text = NPCharRepPhrase(npchar,
			                 pcharrepphrase(LinkRandPhrase("������� ", "�� ��� ��� ", "���� � ����? ��� �� ") +GetFullName(pchar)+"! "
							                 + LinkRandPhrase("���� ��� �� ��������� �� ���? ��-��!","�������, �� ��� � ������ � �������� �������!", "� � ������, ���� �������� � ����-�����.")
											  + RandPhraseSimple(" ��� ���� ������!", " ����� ����������!"),
							                LinkRandPhrase("������������, ", "����� ����������, ", "������ ����, ") + "�������"
											+ RandPhraseSimple(". ��� ���������� ",". ����������� �� ��� ")+" ���"+ RandPhraseSimple(" ������? "," �������? ")
											+ LinkRandPhrase("��� ������ �� �����,","�������� �����,", "����� �������� �������,")+RandPhraseSimple(" �� ������ ��?", " �� ��� ��?")),
							 pcharrepphrase(LinkRandPhrase("������� ","��, ��� ����� ��, ","�� ���� ��������, ") + GetFullName(pchar)
							 				+ LinkRandPhrase(". ����� ������ ������ ��� �������� ����������� �����!",". � �������, � ��� ��� ������ � �������?",". ���� ��������� ��������� ������ �������.")
											 +LinkRandPhrase(" ���� ������ �� ��� �����?"," ��� ��� ������?", " ����� �� ������?"),
							                "����� ����������, ������� " +GetFullName(pchar)+ LinkRandPhrase("! ������� ������� �������",
											"! ������, ���������� �����","! ����������� ������� �����") +", �� ������ ��?"));

			link.l1 = NPCharRepPhrase(npchar,
			            pcharrepphrase(LinkRandPhrase("�� ��� ��� ���, ", "� � ���� ���, ","�� � ����� ����� ������� � �������, ")
									  +LinkRandPhrase("�������!","�������!","������� ����!") +" � � �����, "
									  +LinkRandPhrase("���� ���� ��� ������� ���� � ����!","���� ������� �������!", "���� ������� ����� ���� �� �����!"),
						               LinkRandPhrase("������ ����, ","������� ��������, ","������, ") + GetAddress_FormToNPC(NPChar)+" " +GetFullName(Npchar)
									   +RandPhraseSimple(". �� �����, ",". ���������� ��������, ")
									   +LinkRandPhrase("� ������� ������� ������ �������������", "� ���� ������ �������", "� ������� ���� �������� ������ ���� ������")+ "."),
						pcharrepphrase(RandPhraseSimple("� ����� ����� ���������, ","����������� �� ������ ����, ") +GetFullName(Npchar)+ "! "
						              +LinkRandPhrase("��������, ��� �������!", "� �� ������ ��� ������!","���� ����� �� ������ ������!")
						              +RandPhraseSimple(" � ������ � �������!"," � ����� �� ����� ������?"),
						               "������ ����, " + GetAddress_FormToNPC(NPChar)+ "! ��, �� ����� " + LinkRandPhrase("��� ������ �����.",
									    "���� �������� ������.", "�� ���� �� �������.")));
			link.l1.go = "node_1";
			NextDiag.TempNode = "Second time";
		break;

		case "node_1":
			dialog.text = NPCStringReactionRepeat(NPCharRepPhrase(npchar,
			                                         pcharrepphrase(RandPhraseSimple("�������� ���� ������! ","���� � �������! ") +LinkRandPhrase("��� ����� ������������ ����� � ���� ��������, ��-��?",
													                                 "����� �������� ������� ��� ������� ������� ��������! ��-��!", "����� �������� ������ ���� ������� � ��� �������?"),
            														LinkRandPhrase("������� �����!", "������ � �����!","������� ����� �� �����!")+RandPhraseSimple(" � ���� �� �������"," � ���� ��������")
																	+LinkRandPhrase(" ����� ������ ���� �� �������."," ���������� ����� �������."," ������ ������ ������� ��������.")
																	+RandPhraseSimple(" � � ����� �������"," ���, ��� ���������")+", �������!"),
													 pcharrepphrase(RandPhraseSimple("����� ������ ��� ������������ �����! ��-��! ���� �� �� �������� �������� �� ��������!",
													                                 "����� ������ ���������� � ����� � �������� ���� �����. �� ������� �� ������, �����?!"),
                 												"������� " +LinkRandPhrase("������ ������ ������ " + NationNameGenitive(sti(NPChar.nation)),"������ ��� ������� ����", "�������� �������� �����")
																	+RandPhraseSimple("? � ��������� � ����� ������������!","? ��� ������� � ����� �������!")
																	+RandPhraseSimple(" ��� ��� ������"," ��������������") +", �������!")),
				                                  NPCharRepPhrase(npchar,
												  	 pcharrepphrase("��� ���� �����, ���: �� ����� ����� �� �������. ���������� ����� �������!",
													                "� � ������� ������� ������, ��� �� ������ �����. " +RandPhraseSimple("�� ������ � ����, ��� �� ����� �����.", "�� �����, �� "
																	+RandPhraseSimple("�� � ���� ������.", "���� �� ��������."))),
													 pcharrepphrase("� ����� �������� � ��� ���� ���� � ����������� ������. ������ ������ ������ ��� �������? ��� ������ � ����!",
													                "�������� ������������ - �������� ������, ������� " + GetFullName(pchar)+", �� ��� ��?")),
												  NPCharRepPhrase(npchar,
												     RandPhraseSimple("��� ��������� ���� �� ��������� �����! ��� �������� � ������ �������! " ,
													   RandPhraseSimple("���������� ������� �� ������ ��������!","���, � ������ ���� ������� �� �����!") +" ����-��! � ������� ����. ")
													   +RandPhraseSimple("���� ����, ���?", "��� �� ������ ������, �������?"),
													 pcharrepphrase("� �� ������ �������������, ������� " + GetFullName(pchar)+". ��� ����������� �������, � ��������� ��� ����� �������� ����!",
													                "�� ������, ������� " + GetFullName(pchar)+", �� �������� � ��� � ����� ��� ������� �������� � ����! ��� �� ������?")),
												  NPCharRepPhrase(npchar,
												     "�������, ��� ����� ������������� � �����. "
													 +LinkRandPhrase("���� ���-��� �������� �����. �� ��� ����� ������. ","�� �����, ��� ����� ���� ������ ����� ������� �����? ",
													 pcharrepphrase("���� � ������ �����. ����������� ���� - � �� ��������.","��� ��� ���� ������ ��������� ���� � ����! � ����!!! �� ����� �� �� ���� �������...")),
													 pcharrepphrase(RandPhraseSimple("� �������, ","� ��������, ")+RandPhraseSimple("��� �� �������� ��� ������.","��� ������ ������� ��� �� �����.")
													 +RandPhraseSimple(" �������� ����� � ��� �� ���, ��� ���������� �������..."," �������� ������� �� ��������� ����� ����� ��������?!"),
													 "������� " + GetFullName(pchar)+", ���� ������� � ��������� ����������� ��� ��� ��������!"
													 +RandPhraseSimple(" ������ ������� ��������� ������� ����� ���������?", " �� ������ ������ ���-�� ���������?"))),
										"cycle", 10, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("�����, ���������, ��� � ���� ����, ������ �����!",
													               "� ����� ���� ���� ���������� �� ���� ������, ��������."),
													pcharrepphrase("������� ���������, " +GetFullName(npchar)+", � �� ������� ���� ������ ���� �� ���! ���� ��������� ���� ������!",
													               "������� ��������� ���� ������, " +GetAddress_FormToNPC(NPChar)+ ".")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("��� �����! ������� �� ��������! ��, ��� � ���� � ����� ����?",
													               "��, ��������, ����� ��������� ����� �����. ����� ������ ������ ��� ����������?"),
												    pcharrepphrase("������� � �� ������� ���� ������, ��-��. ����. ����� ���� ������.",
													               "������� ������� �������� ����, " +GetAddress_FormToNPC(NPChar)+ " " +GetFullName(npchar)+ "")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("��� ��� ����? ���� ������ ���-���!",
													               "�������� ��� ������ � ������, " +GetFullName(npchar)+ ". ��� ����� ���������� ���� ������."),
												    pcharrepphrase("���� � ���� ������! ��� ����� ����� �� ���� ����! ���� ��������� ���� ������",
													               "��� ������ �������. ������� �����������.")),
											  "��������� ������, ������ ���� � ������ ����. � ���������.",
											  npchar, Dialog.CurrentNode);
			link.l1.go = "market";
			link.l2 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("��, ��� �� � �������� ����-������! �����, ����� � ����.",
													               "�������� � ������ �����, ��������"),
													pcharrepphrase("��������! � ������ �� ������� ����!",
													               "��� ����� � ���� ���������� �� ����, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+".")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("� ���� � ���� ����, ������ �����!",
													               "�������� � �����, ��������!"),
													pcharrepphrase("������� ��������, ���� ���� �� ��������, ��������! � ���� ������ ����!",
													               "��������� ��������, " +GetAddress_FormToNPC(NPChar)+ " " +GetFullName(npchar)+ ", � ���� �������� � ���� ���� ����.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("��� ��������, ��������?! � ���� � ���� ����.",
													               "� ������ �� ���������, ��������. � ���� ������ ����."),
													pcharrepphrase("������� ���� ���� � ������ ������, �������! � ���� ���� � ����.",
													               "������ ������ �������, " + GetAddress_FormToNPC(NPChar)+ " " +GetFullName(npchar)+ ", �� � �� ������� ����.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("�������� ����, ������ �������, � ���� ���� � ����",
													               "������ ��� �� ����, � �������. �������, � �� ��� ���� �� � ��������. �������� � ����."),
													pcharrepphrase("�� � ��� ��� ������� ���� �� �����, ��������! � ���� � ���� ����.",
													               "���� ������ ���������, � ���� � ��� ���� ����� �������")),
											   npchar, Dialog.CurrentNode);
			link.l2.go = "business";
			link.l3 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("������� ����, ����� �������. ���� � ���� ������ ���-���.",
													               "�� ������, ��� ����� ����������."),
													pcharrepphrase("� ����� ����! ������� �� ������� ���, ��� ��� �����.",
													               "� �������������. � ���� ������ ���-���.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("������� �� �����, ������ ������. ��� ���� ���-��� ������.",
													               "� ���� ������������ ������."),
												    pcharrepphrase("������ ��������. � ���� ������ ���-���.",
													               "��, � ��������. ���� ��� �������� ��� � ���.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("� �����, �� ������, � ��� � ���� ��������.",
													               "� �������, �� �������� ��� �� ��� ������."),
													pcharrepphrase("� ���� � ������ ���� ���-������ ����� ��������? �� ���� � � ���� ��������.",
													               "���� �� �� ����. ��� ����� ���-����� ����������.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("�� ��������, �������! � ������ ���� �������� ����.",
													               "�� ���������� ���, ��� ������ ����. �������� ��� �� ������, � � ����"),
													pcharrepphrase("�� ��� �� ���� ������ ����� - �� � ���� ���������. �� �� ���-��� ������.",
													               "� �� ������� ��� �������. ����� ���� ������.")),
											   npchar, Dialog.CurrentNode);
			link.l3.go = "quests";
			//--> eddy. �������, ����� � ��������� ���������
			if (pchar.questTemp.Ascold == "Seek_powder" && pchar.questTemp.Ascold.TraderId == npchar.id && !CheckAttribute(Pchar, "RomanticQuest.TalkInShop")) 
			{
				link.l3.go = "AscoldTrader";
			}
			if (pchar.questTemp.Ascold == "PowderWasSeek" && pchar.questTemp.Ascold.TraderId == npchar.id) 
			{
				link.l3.go = "AscoldTrader_WasSeek";
			}
			//<-- eddy. �������, ����� � ��������� ���������
			link.l4 = HeroStringReactionRepeat(NPCharRepPhrase(npchar,
			                                        pcharrepphrase("����, ������ ��������.",
													               "����� �������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"."),
													pcharrepphrase("��������! ������ �� �����. � ��� �����.",
													               "������� ���� ��� ������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"! �� ��������!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("������ ��������!",
													               "�� ��������, ��������."),
													pcharrepphrase("��! �� ��� ��� �������? ���? ���!?! �� �����, �����, ����� �������.",
													               "����� ���� ��������, � ���� ������� ���� � ������.")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("���! ��� ��������!",
													               "��� ���� ����. ��������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!"),
													pcharrepphrase("� �� ���� ��� ��������! ������� ��� ��� � ����� �������!",
													               "�������, ����� ���������, ��� ���� �� �������. ��������, "+GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											   NPCharRepPhrase(npchar,
											        pcharrepphrase("��� � �����, �������? ��� �� � - " +GetFullName(pchar)+ "! ��������, � ����� �����.",
													               "�����, ���� ������ ��������� ��� ������, ��� � ��� ��� ����� ����."),
													pcharrepphrase("������ ���� �������, ��������! � ��� �����.",
													               "��� ��� ��� ������, " + GetAddress_FormToNPC(NPChar)+" " +GetFullName(npchar)+"!")),
											  npchar, Dialog.CurrentNode);
			link.l4.go = "exit";
		break;

		case "market":
//navy -->
			//����� ���
			if (CheckFreeServiceForNPC(NPChar, "Store") != -1)	 // to_do ��� ������
			{
				dialog.text = "��������, �� � ������ �����. ������ �������! �������� ������.";
				link.l1 = "�� ��� �� ��������! �����, ����� �������.";
				link.l1.go = "exit";
				break;
			}
//navy <--
			dialog.text = RandPhraseSimple("� ���� ���� �����, ���������, �������� ����, ������ ��������� � ������ ������! ��� ��� ����������?", "������ ������ ������ ��� �������? ���, �����, ����������� �������� ������?");
			link.l1 = pcharrepphrase(LinkRandPhrase("� ���� ����� ����� ������! ������� �� ������, �� ��� ��?",
			                         "��� ����� ���������� ���� ���� �� ������ � ������ ��� ����� �������. ��-��!",
									 "�� ����������� ������� �� � ������ ���� �������! �� ��� ����� ���� ����� ��������� ������."),
									 LinkRandPhrase("���� ������� ���� � ��������� ������.",
			                         "� ���� ������ ����� �� �������.",
									 "������ ��� ����, ����� � ������ ������."));
			link.l1.go = "trade_1";
			link.l2 = LinkRandPhrase("������ ��� �����, ���������, � ��� ��� � ���� ��� ����?",
			                         "��� �� �������� ���� ����� ����������, ���� � �������� ��������.",
									 "����������� �����, ���������� ����� - ��� ��� ���� ����������.");
			link.l2.go = "items";
			link.l3 = pcharrepphrase(RandPhraseSimple("��������� ��� ��������. �����!",
			                                          "�����, ����� �������. ������ �� �����."),
									 RandPhraseSimple("���, ��� ������ �� �� �������. ��������.",
									                  "��������, � ���� ������� ����. �� ��������!"));
			link.l3.go = "exit";
		break;

		case "trade_1":
            ok = (rColony.from_sea == "") || (Pchar.location.from_sea == rColony.from_sea);
		    if (sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok)
			{
			    NextDiag.CurrentNode = NextDiag.TempNode;
			    DialogExit();
			    Pchar.PriceList.StoreManIdx = rColony.index; // boal 27.02.05
			    LaunchStore(sti(rColony.StoreNum));
			}
			else
			{
			    dialog.text = NPCharRepPhrase(npchar, pcharrepphrase("������ ����? � ��� �� ���� �������? � ����� ��� ���!",
				                                                     "������� ��������, ���� �� �������� ����! � ����� ��� ������ �������!"),
													  pcharrepphrase("� �� ���� ������ ������� � �����, ������� " +GetFullName(pchar)+ ". �������, ��� �� '������� ���������'?",
													                 "�������, ������� ������� ����� � �����. ������������ ������� � ���������."));
				link.l1 = NPCharRepPhrase(npchar, pcharrepphrase("" +RandSwear()+"����� ������������!!! �����, ������ �������, ��� ��������!",
				                                                 "� �� ����� ��� �������� " +GetFullName(npchar)+ ", ������� �� ������ ������� �������."),
												  pcharrepphrase("���. ��� ������� ���������� Black Perl. ��� ��� �� �������? ��-��! �����!",
												                 "������� �� �����, ����������� �� ������������."));
			    link.l1.go = "exit";//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}
		break;

		case "items":
			if (npchar.quest.item_date != lastspeak_date)
			{
				GiveItemToTrader(npchar);
				npchar.quest.item_date = lastspeak_date;
			}
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
			LaunchItemsTrade(NPChar);
		break;

		case "business":
			iTest = 0;
    		//����� ����� �����, ��������� ������
			if (pchar.questTemp.BlueBird == "begin" && sti(npchar.nation) == GetBaseHeroNation() && sti(npchar.nation) != PIRATE)
			{
				dialog.text = RandPhraseSimple("�������, ����� ���, �������� ���!!!", "�������, � ����� � ��� ������ �� ����� ���� ���������!");
				link.l1 = "��� ���������? ��� � ���� ��� ������?"; 
				link.l1.go = "RBlueBird";
				break;
			}			
//navy -->
			//����� ���
			iTmp = CheckAvailableTaskForNPC(NPChar, PGG_TASK_WORKONSTORE);
			if (iTmp != -1)
			{
				dialog.text = "���� � ���� ���������, �� " + GetFullName(&Characters[iTmp]) + " ��� ������ ��������� �� ��� ����.";
				link.l1 = "�� ��� �� ��������! �����, ����� �������.";
				link.l1.go = "exit";
				break;
			}
//navy <--
			dialog.text = NPCharRepPhrase(npchar,"����?! ���������� ��� �� �������!","� ��� ������. � ����� ���� ���� ����?");
            ok = (rColony.from_sea == "") || (Pchar.location.from_sea == rColony.from_sea);
            if (sti(Pchar.Ship.Type) != SHIP_NOTUSED && ok)
			{
    			/*if (CheckAttribute(pchar, "CargoQuest.iQuantityGoods"))
    			{
    				int iQuantityShipGoods = pchar.CargoQuest.iQuantityGoods;
    				int iQuestTradeGoods = pchar.CargoQuest.iTradeGoods;
    			}  */
    			if (CheckQuestAttribute("generate_trade_quest_progress", "begin") || CheckQuestAttribute("generate_trade_quest_progress",  "failed"))
    			{
    				if (pchar.CargoQuest.TraderID == npchar.id )//&&  GetSquadronGoods(pchar, iQuestTradeGoods) >= iQuantityShipGoods)
    				{
    					link.l1 = RandPhraseSimple("� ����� ����� ������� ���� ��� ������ ��������.", "��� ������� ����������� ��� �������� ����� � ��� �������. ����� ��� ���������.");
    					link.l1.go = "generate_quest_2";
    				}
    				else
    				{
                        if (!CheckQuestAttribute("generate_trade_quest_progress",  "failed") && pchar.CargoQuest.GiveTraderID == npchar.id && GetNationRelation2MainCharacter(sti(characters[GetCharacterIndex(pchar.CargoQuest.TraderID)].nation)) == RELATION_ENEMY)
                        {
                            link.l1 = "� ���������, � �������� ���������� �� ������ ������������. ������������ ���������� �� ��������� ��� ��������� ����.";
            				link.l1.go = "generate_quest_cannot_done";
                        }
                        else
                        {
                            if (rand(1) == 1)
                            {
                                link.l1 = RandPhraseSimple("� ���� ���������� ��� ������������ ��� ����� ��� ��������� �����.",
								                           "� ���� ���������� �������, � � ���� ��������� ����� ����, ���� ������ �� ���������.");
            				    link.l1.go = "generate_quest_not_closed";
        				    }
    				    }
    				}
    			}
    			else
    			{
    				link.l1 = "�� ������ ������������ �����, " +GetAddress_FormToNPC(NPChar)+ "? � ���� �������� ������� � ������� �������.";
    				link.l1.go = "generate_quest";
    			}
    			// --> �� ��������� ���������
    			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
    			{
	                link.l3 = pcharrepphrase(RandPhraseSimple("��, ��������� � ����... ��� ���, ��������, �������� ������ ����� �����.",
	                                                          "�� ���, ������ ����, ����� ������� ���� ���� ��������."),
	                                        RandPhraseSimple("� ����� �� �������� � ���� ���������� �������.",
	                                                         "������� ������� ���������� �������, ��� ���� � ��� ������������."));,
	
	                link.l3.go = "LoanForAll";
                }
				//����� ���� - �� ����� � ����� �������
  				if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") 
				{
					link.l4 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
						GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
					link.l4.go = "IntelligenceForAll";
				}	
				// ----------------- ����� ��������� ����� �����, ����� ����� -----------------
				if (pchar.questTemp.BlueBird == "weWon" && pchar.questTemp.BlueBird.traiderId == npchar.id)
				{
					link.l1 = "���� ��� ���������, ��� ��� ������� ���������� � ����� ����������. ������ '����� �����' �� ����� ����� ��������� ���������. � ������ �� ����������� ���������.";
					link.l1.go = "RBlueBirdWon";
				}
				if (pchar.questTemp.BlueBird == "DieHard" && pchar.questTemp.BlueBird.traiderId == npchar.id)
				{
					link.l1 = "������ ��� ��������, ��� ��� �� ������� �������� ������ '����� �����'. � ������ ���, ��� ���...";
					link.l1.go = "RBlueBirdDieHard";
				}
				if (pchar.questTemp.BlueBird == "returnMoney" && pchar.questTemp.BlueBird.traiderId == npchar.id && sti(pchar.questTemp.BlueBird.count) > 0)
				{
					if (sti(pchar.questTemp.BlueBird.count) < 5)
					{
						link.l1 = "��� ����������� ������ ����� ������ ��������? ��� ���������?";
						link.l1.go = "RBlueBird_retMoney_1";
					}
					else
					{
						link.l1 = "�� ��� �� ���������, �� ������� ������ ��� ��� ������ ������������ ������?";
						link.l1.go = "RBlueBird_retMoney_3";
					}
				}
				// ----------------- ����� ��������� ����� �����, ����������� -----------------
				//homo 25/06/06 �����
                link.l6 = pcharrepphrase(RandPhraseSimple("���������� ��� ��������� �������? ��� ����� ������� ��� ����� ����.",
                                                          "�� �������, ������ ��������, � ����� ��������, ��� ����� ����� �� ����?"),
                                        RandPhraseSimple("�������, ��� ������ � ���� ����� ��������. ��� ���������� � ����� �����?",
                                                         "��, ��������, ������ ��� ��������� �������? ��� ������� ���������?"));,

    		    link.l6.go = "rumours_trader";
				// <-- homo
				link.l99 = "�������, ������ �� �� ����������.";
    			link.l99.go = "no_quests";
			}
			else
			{
                link.l1 = NPCharRepPhrase(npchar, pcharrepphrase("" +RandSwear()+"� ������� ������� �� ������ ������� �������. ����� �������!", "����� ���� ��������, " +GetFullName(npchar)+ ", � ����� �����. ��� ������� ��� �� ������� � �����."), pcharrepphrase("������! ��� ������� �� ��������� ������� ��� ��������! ����� �����!", "��������, � ������ ��������� ��������� ���� ������� � �����!"));
                link.l1.go = "exit";
    			// --> �� ��������� ���������
    			if (CheckAttribute(pchar, "GenQuest.LoanChest.TakeChest") && sti(pchar.GenQuest.LoanChest.TargetIdx) == sti(NPChar.index))
    			{
	                link.l3 = pcharrepphrase(RandPhraseSimple("��, ��������� � ����... ��� ���, ��������, �������� ������ ����� �����.",
	                                                          "�� ���, ������ ����, ����� ������� ���� ���� ��������."),
	                                        RandPhraseSimple("� ����� �� �������� � ���� ���������� �������.",
	                                                         "������� ������� ���������� �������, ��� ���� � ��� ������������."));,
	
	                link.l3.go = "LoanForAll";
                }
				//����� ���� - �� ����� � ����� �������
  				if (CheckAttribute(pchar, "GenQuest.Intelligence") && pchar.GenQuest.Intelligence.SpyId == npchar.id && pchar.GenQuest.Intelligence == "") 
				{
					link.l7 = RandPhraseSimple("� ����� �� ��������� ������ ��������. ��� ����� ���������� " + GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + ".", 
						GetFullName(characterFromId(pchar.GenQuest.Intelligence.MayorId)) + " ������� ���� � ���. � ������ ���-��� �������...");
					link.l7.go = "IntelligenceForAll";
				}	
			}
		break;
		
		case "generate_quest":
			if (npchar.quest.trade_date != lastspeak_date || bBettaTestMode)
			{
				npchar.quest.trade_date = lastspeak_date;
				//�������� ������������ ��� ������ ��������
				if (sti(NPChar.nation) != PIRATE && GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY)
				{
					dialog.text = "���, �� �����! �� ������� ����, �� ��� ���, ���� �� ��������� � " + NationNameAblative(sti(NPChar.nation)) + ".";
					link.l1 = "� ����� ������, ��������.";
					link.l1.go = "exit";
				}
				else
				{
                    // �������� �� ���� ����������� -->
                    if (isBadReputation(pchar, 42))
                    {
                        dialog.text = RandSwear() + "�� ���� " + GetFullName(pchar)+ ", �� ��� ��? � ���� " +
                                      RandPhraseSimple("����� ������� �����","������� �� ������ �������") + ", � �� ���� ����� � ����� ����.";
						link.l1 = RandPhraseSimple("��... ����� �������� ��� ����������, � ��� ����� � ���� �������.",
						                           RandSwear()+ "��� ��, � ���� ���� ������� � ������������, "+GetFullName(npchar)+".");
						link.l1.go = "exit";
                        break;
                    }
                    // �������� �� ���� ����������� <--
                    int iTradeNation = GenerateNationTrade(sti(NPChar.nation));

					if (iTradeNation < 0)
					{
						dialog.text = NPCharRepPhrase(npchar, "��, "+GetAddress_Form(NPChar)+", �� �� ������� �������. ��������� ������.","������� ������. ��������, � ������ ��� �������� �����������.");
						link.l1 = "������, � ����� ������.";
						link.l1.go = "exit";
					}
					else
					{
                        int storeMan = findStoreMan(NPChar, iTradeNation);
                        if (storeMan > 0)
                        {
                            //��������� ������/�������
    						iTradeGoods = rand(GOOD_BRICK); //���� � ������ �� ����, ����� � ��� - ��!!
    						//��������� ��������� ����� (��� ���� ������ ��������� �� ������� ���� 100 ������ ���������� �����
    						RecalculateSquadronCargoLoad(pchar); // fix �������� �����
    						iQuantityGoods = GetSquadronFreeSpace(pchar, iTradeGoods);
    						if (iQuantityGoods < 100)// ��� � ��. ������
    						{
    							dialog.text = NPCharRepPhrase(npchar, "� ���� ������ �������� �� ���������� ���� ����, ������� ������ �� �����.","� ���������, ������� "+GetFullName(pchar)+", ��� �������� ����� ��� ����� ����� ������������� �������, ��� ���.");
    							link.l1 = NPCharRepPhrase(npchar, "� ���� ��������� �������, �� � ����� ���� �����. �� ��������.","� ��� �������, ������ ������ �����. ��������.");
    							link.l1.go = "exit";
    						}
    						else
    						{
    							iQuantityGoods = iQuantityGoods - rand(makeint(iQuantityGoods/3)) - 10;
    							iMoney = makeint((iQuantityGoods * sti(Goods[iTradeGoods].Weight) / sti(Goods[iTradeGoods].Units)) * (4+rand(3) + GetSummonSkillFromNameToOld(pchar, SKILL_COMMERCE)) + 0.5);

    							pchar.CargoQuest.iTradeGoods = iTradeGoods;
    							pchar.CargoQuest.iQuantityGoods = iQuantityGoods;
    							pchar.CargoQuest.iMoney = iMoney;
    							pchar.CargoQuest.iTradeNation = iTradeNation;
    							pchar.CargoQuest.iDaysExpired = 25 + rand(5);
    							
    							pchar.CargoQuest.iTradeColony = Characters[storeMan].city;
    							pchar.CargoQuest.iTradeIsland = GetIslandByCityName(Characters[storeMan].city);
    							pchar.CargoQuest.TraderID     = Characters[storeMan].id;
    							pchar.CargoQuest.GiveTraderID = NPChar.id;
                                SaveCurrentQuestDateParam("CargoQuest");
    							string sNation = XI_ConvertString("Colony"+Characters[storeMan].city);
                                sTemp = "";
                                if (pchar.CargoQuest.iTradeIsland != Characters[storeMan].city)
								{
                                    sTemp = ", ��� �� " + XI_ConvertString(pchar.CargoQuest.iTradeIsland+"Dat");
                                }
                                dialog.text =  "�! � ��� ��� ����� ��� ��������� ��� �� �������� ��� ����� ������. ���� � ���, ��� ��� ����� ��������� ���� " +
                                               GetGoodsNameAlt(iTradeGoods)+ " � ���������� " + FindRussianQtyString(iQuantityGoods) + " � ����� " +
                                           sNation + sTemp + ", � ��� ����� �������. ���� �� ������� ������� ��� �� " +  FindRussianDaysString(makeint(pchar.CargoQuest.iDaysExpired)) +
                                           ", �� �� �������� �� ����� �������� " +
                                           FindRussianMoneyString(iMoney) + " ��������������. �� ���?";
    							link.l1 = "�����, ��� � ��������.";
    							link.l1.go = "exit_trade";
    							link.l2  = "�� �����, ��� ��� ��� ��������.";
    							link.l2.go = "exit";
    						}
                        }
                        else
                        {
                            dialog.text = "������������ ���������� � ���������� �� ��������� ��� ���������. "+
                                          XI_ConvertString(NationShortName(sti(NPChar.nation))+"hunter") + " �� ������ �� �����, � ������� ������� ������ � ����.";
    						link.l1 = "�! �������. ����� ��������";
    						link.l1.go = "exit";
                        }
					}
				}
			}
			else
			{
				dialog.text = NPCharRepPhrase(npchar, "����� ����������, ��� � ���� ������� �� ������ ���������!","� �� ��� ������� ��� �������, ��� � ���� ������ ��� ��� ���.");
				link.l1 = NPCharRepPhrase(npchar, "�� ������ ������������� ������, ��� � ����� ���� ����������!", "�, ��������.");
				link.l1.go = "exit";
			}
		break;

		case "exit_trade":
			AddDialogExitQuest("trade_quest_open");
			Nextdiag.CurrentNode = Nextdiag.TempNode;
			DialogExit();
		break;

		case "generate_quest_2":
			if (pchar.quest.generate_trade_quest_progress == "failed")
			{
				dialog.text = RandSwear() + "�� ��������� � ���������� ��� ���� ������� ������, �� ��������? ��� �������� ���������� ����� ��������. ��������.";
				link.l1 = "�������� ����, �� � ��� ������. ��������, ���� � ������ ���� ������, ��� ����������� ��� ������� ���������?";
				link.l1.go = "generate_quest_failed";
				link.l9 = "����� ��������.";
				link.l9.go = "exit";
				ChangeCharacterReputation(pchar, -1);
			}
			else
			{
                iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
                iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);
                
                dialog.text = "�����! � ��� ����� ����. �� ������ ��������� ��� ���� " +
                GetGoodsNameAlt(iTradeGoods) + " � ���������� " + FindRussianQtyString(iQuantityGoods) + " � �������� �� ��� " +
                FindRussianMoneyString(sti(pchar.CargoQuest.iMoney)) + ".";
				link.l1 = "������ ���.";
				link.l1.go = "generate_quest_ready";
			}
		break;
		
		case "generate_quest_ready":
            iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
            iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);
            // ���� �������� ��� �� ����� -->
            if (rand(8) == 1 && sti(Goods[iTradeGoods].Cost) <= 44 && GetQuestPastDayParam("CargoQuest") > 5)
            {
                dialog.text = RandSwear() + "� �������� ���������� ����� ����, "+GetAddress_Form(NPChar)+
                              ". ���� � ���, ��� � " + RandPhraseSimple("� ��������������� ��������� � �� ����� �������� ���� ������"," ��� ���� ���� ���� � ������ ����� �� ����� �������� ����")+
                              ". � �������� ������ ������ ������ � ������� ��� ���� ������������ ���� ����.";
				link.l1 = RandSwear() + RandPhraseSimple("� � ����� ���� ������ ���� �� ���� ��������!!! � ��� ����� ���� ��-�� ���� ����!",
                                                         "�� ������� ���� � ����������� ���������, � ��� �������� �����������.");
				link.l1.go = "exit";
				ChangeCharacterReputation(pchar, 2);
				AddCharacterExpToSkill(pchar, "Sailing", 60);
				AddCharacterExpToSkill(pchar, "Leadership", 20);
				AddCharacterExpToSkill(pchar, "COMMERCE", 15);

				pchar.quest.generate_trade_quest_progress = "";
				pchar.quest.generate_trade_quest.over = "yes";
                
                AddQuestRecord("DELIVERY_TRADE_QUEST", "4");
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodGen", GetGoodsNameAlt(iTradeGoods));
			    AddQuestUserData("DELIVERY_TRADE_QUEST", "sTargetColony",XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony+"Dat"));
                CloseQuestHeader("DELIVERY_TRADE_QUEST");
                break;
            }
            // ���� �������� ��� �� ����� <--
			if (GetSquadronGoods(pchar, iTradeGoods) < iQuantityGoods)
			{
                iTmp = iQuantityGoods - GetSquadronGoods(pchar, iTradeGoods);
                dialog.text = "�� �������� �� ���� �����!! �������� �������� �������� ��������� ��� "
                              + FindRussianQtyString(iTmp) + " �����.";
				link.l9 = RandPhraseSimple("��, �������������. �������� ���������� ���������. ����� ��� ���.",
                                           RandSwear() +"����������� ����� ��������� ������ ����. � �������� ��������� � ����� ��� ���.");
				link.l9.go = "exit";
                AddQuestRecord("DELIVERY_TRADE_QUEST", "8");
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sHaveNotQty", iTmp);
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodGen", GetGoodsNameAlt(iTradeGoods));
			}
			else
			{
                dialog.text = "������������! ��� ���� �������, ������� "+GetFullName(pchar)+".";
				link.l1 = "������� � ���� ��������.";
				link.l1.go = "exit";
				ChangeCharacterReputation(pchar, 3);
				AddCharacterExpToSkill(pchar, "Sailing", 100);
				AddCharacterExpToSkill(pchar, "Leadership", 30);
				AddCharacterExpToSkill(pchar, "COMMERCE", 100);

				AddMoneyToCharacter(pchar, makeint(pchar.CargoQuest.iMoney));
				pchar.quest.generate_trade_quest_progress = "";
				pchar.quest.generate_trade_quest.over = "yes";
				RemoveCharacterGoods(pchar, makeint(pchar.CargoQuest.iTradeGoods), makeint(pchar.CargoQuest.iQuantityGoods));
				
                OfficersReaction("good");
                
                AddQuestRecord("DELIVERY_TRADE_QUEST", "2");
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodGen", GetGoodsNameAlt(iTradeGoods));
			    AddQuestUserData("DELIVERY_TRADE_QUEST", "sTargetColony",XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony+"Gen"));
                CloseQuestHeader("DELIVERY_TRADE_QUEST");
			}
		break;
		
		case "generate_quest_failed":
            iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
            iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);
            iMoney =  3*makeint(iQuantityGoods * sti(Goods[iTradeGoods].Cost) / sti(Goods[iTradeGoods].Units));
			dialog.text = "������... ����� � " + FindRussianMoneyString(iMoney) + " ��� ��� ������� ��� ������ �� ��������� ���� ������ �������� ������.";
			link.l1 = "���! ��� ��! �������, � �� ���� ������ ������ ������ ��� ������� �����! � ���� �� � ��� ����������!";
			link.l1.go = "exit";
            if (sti(pchar.Money) >= iMoney)
            {
    			link.l2 = "��� ��������� �����";
    			link.l2.go = "generate_quest_failed_2";
			}
            else
            {
                AddQuestRecord("DELIVERY_TRADE_QUEST", "7");
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sMoney", FindRussianMoneyString(iMoney));
    		    AddQuestUserData("DELIVERY_TRADE_QUEST", "sTargetColony",XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony+"Gen"));
            }
		break;
		
		case "generate_quest_failed_2":
            iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
            iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);
            iMoney =  3*makeint(iQuantityGoods * sti(Goods[iTradeGoods].Cost) / sti(Goods[iTradeGoods].Units));
			dialog.text = "������������. ������ � ���� �������� ����� ��������, ��� � ���� ��� �� ����� ����� ����.";
			link.l1 = "�������! � ��������� ��� � �� �������.";
			link.l1.go = "exit";
			ChangeCharacterReputation(pchar, 10);
			AddCharacterExpToSkill(pchar, "Sailing", -100);
			AddCharacterExpToSkill(pchar, "Leadership", -50);
			AddCharacterExpToSkill(pchar, "COMMERCE", -100);

			AddMoneyToCharacter(pchar, -iMoney);
			pchar.quest.generate_trade_quest_progress = "";
			pchar.quest.generate_trade_quest.over = "yes";

            AddQuestRecord("DELIVERY_TRADE_QUEST", "5");
            AddQuestUserData("DELIVERY_TRADE_QUEST", "sMoney", FindRussianMoneyString(iMoney));
            AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodGen", GetGoodsNameAlt(iTradeGoods));
		    AddQuestUserData("DELIVERY_TRADE_QUEST", "sTargetColony",XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony+"Gen"));
            CloseQuestHeader("DELIVERY_TRADE_QUEST");
		break;
		
		case "generate_quest_not_closed":
            iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
            iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);

            dialog.text = "��. � ������, ��� �� ��� �� ��������� ������������� �� ����������� ������, � ��� ������� �����. �� ������ ���� ��������� ���� " +
            LanguageConvertString(iSeaGoods, "seg_" + Goods[iTradeGoods].Name)+ " � " + XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony) + ".";
			link.l1 = "��, �����! �� �����!";
			link.l1.go = "exit";
		break;
		
		case "generate_quest_cannot_done":
            dialog.text = RandSwear() + GetAddress_Form(NPChar) +", �� ��������� ����!!! �����, ����������� ���-�� ������ ��� ��������?";
			link.l1 = "������. � ���������� ��������� ����.";
			link.l1.go = "exit";
			link.l2 = "���. �� �����. �������";
			link.l2.go = "generate_quest_cannot_done_2";
			
		break;
		
		case "generate_quest_cannot_done_2":
		    iTradeGoods    =  makeint(pchar.CargoQuest.iTradeGoods);
            iQuantityGoods =  makeint(pchar.CargoQuest.iQuantityGoods);
            if (GetSquadronGoods(pchar, iTradeGoods) < iQuantityGoods)
			{
				dialog.text = "���������! �� �� ������� ��� ������ ���������� ������!! ��� ��������� ��� "
                              + FindRussianQtyString(iQuantityGoods - GetSquadronGoods(pchar, iTradeGoods)) + " �����.";
				link.l9 = RandPhraseSimple("��, �������������. �������� ���������� ���������.",
                                           RandSwear() +"����������� ����� ��������� ����. � ������� ����� � ����� ��� ���.");
				link.l9.go = "exit";
			}
			else
			{
                dialog.text = "�����, � �������...  �������� ����� �������.";
    			link.l1 = "�������. � ��������� ��� � �� �������!";
    			link.l1.go = "exit";
    			ChangeCharacterReputation(pchar, -3);

				pchar.quest.generate_trade_quest_progress = "";
				pchar.quest.generate_trade_quest.over = "yes";
				RemoveCharacterGoods(pchar, makeint(pchar.CargoQuest.iTradeGoods), makeint(pchar.CargoQuest.iQuantityGoods));
                
                AddQuestRecord("DELIVERY_TRADE_QUEST", "6");
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodQty", FindRussianQtyString(sti(pchar.CargoQuest.iQuantityGoods)));
                AddQuestUserData("DELIVERY_TRADE_QUEST", "sGoodGen", GetGoodsNameAlt(iTradeGoods));
    		    AddQuestUserData("DELIVERY_TRADE_QUEST", "sTargetColony",XI_ConvertString("Colony"+pchar.CargoQuest.iTradeColony+"Dat"));
                CloseQuestHeader("DELIVERY_TRADE_QUEST");
            }
		break;

		case "no_quests":
			dialog.text = "� ������ ������ � �� �������� � ����� �������.";
			link.l1 = "����� ����. ����� ������� ��������� � ������.";
			link.l1.go = "node_1";
		break;
		
        case "fight":
			DialogExit();
            NextDiag.CurrentNode = NextDiag.TempNode;
			LAi_group_Attack(NPChar, Pchar);
		break;
		
		case "Exit":
			if (pchar.questTemp.Ascold == "canTakeQuest" && pchar.questTemp.Ascold.TraderId == npchar.id)//eddy. �������, ������
			{
				dialog.Text = "�� ����� ��� �������. ������, ���� ��������� ���-������ ������� ���������� �����, �� ���� ����, ������� ��� ���. � ���� ���� ��������� ������ �� ��� ����.";
				Link.l1 = "������.";
				Link.l1.go = "exit";
				SaveCurrentQuestDateParam("questTemp.Ascold");
				pchar.questTemp.Ascold = "Seek_powder";
				AddQuestRecord("Ascold", "1");
				AddQuestUserData("Ascold", "sName", GetFullName(npchar));
				AddQuestUserData("Ascold", "sCity", XI_ConvertString("Colony" + npchar.city + "Dat"));
			}
			else
			{
				Nextdiag.CurrentNode = Nextdiag.TempNode;
				DialogExit();
			}
		break;
	}
	LanguageCloseFile(iSeaGoods);
}

int findStoreMan(ref NPChar, int iTradeNation)
{
    ref ch;
	int n;
    int storeArray[30];
    int howStore = 0;

	for(n=0; n<MAX_CHARACTERS; n++)
	{
		makeref(ch,Characters[n]);
        if (CheckAttribute(ch, "Dialog.Filename") && ch.Dialog.Filename == "Common_Store.c") // �������
		{
            if (sti(ch.nation) !=  iTradeNation) continue;
            if (NPChar.id == ch.id) continue;
            if (NPChar.id == "Panama_trader") continue; //������ �������
            if (ch.location == "none") continue; // ���� ��� �����, ��������� �� ���� ��������
            if (GetIslandByCityName(ch.city) == GetIslandByCityName(NPChar.city)) continue; // ���� ���, � �� ����!
            storeArray[howStore] = n;
            howStore++;
		}
    }
    if (howStore == 0)
    {
        return -1;
    }
    else
    {
        return storeArray[rand(howStore-1)];
    }
}
