// boal 25/04/04 ����� ������ �������
#include "DIALOGS\russian\Rumours\Common_rumours.c"  //homo 25/06/06
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

    // ����� ������� �� ������� -->
    NPChar.FileDialog2 = "DIALOGS\" + LanguageGetLanguage() + "\Citizen\" + NPChar.City + "_Citizen.c";
    if (LoadSegment(NPChar.FileDialog2))
	{
        ProcessCommonDialog(NPChar, Link, NextDiag);
		UnloadSegment(NPChar.FileDialog2);
	}
    // ����� ������� �� ������� <--
    ProcessCommonDialogRumors(NPChar, Link, NextDiag);//homo 16/06/06
    int iTest, iTemp;
    iTest = FindColony(NPChar.City); // �����
    ref rColony;
	if (iTest != -1)
	{
		rColony = GetColonyByIndex(iTest);
	}
	
	switch(Dialog.CurrentNode)
	{
		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "fight":
            LAi_group_Attack(NPChar, Pchar);
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
		case "First time":
            /*if (GetNationRelation2MainCharacter(sti(NPChar.nation)) == RELATION_ENEMY && sti(NPChar.nation) != PIRATE)
			{
				dialog.text = RandPhraseSimple("�����? ������!!!!!", "������! � ������ ����!");
				link.l1 = "��������! � ��� �����.";
				link.l1.go = "fight";
				break;
			}*/
			
			if (sti(NPChar.nation) == PIRATE)
			{
				if (sti(rColony.HeroOwn) == true) // ��� ���������
				{
				    dialog.text = RandPhraseSimple("����� ����, ��� �� � ���� �������� ����� ������, ���.", "����� �������� �������, ��� ����� ������ ��������� �� ��� � ��������.");
					link.l1 = RandPhraseSimple("� ���������. �����!", "������ ���������� �����. �� ��������.");
					link.l1.go = "exit";
					link.l2 = RandPhraseSimple("� ���� ���� ������ � ����.", "��� ����� ����������.");
					link.l2.go = "quests";//(���������� � ���� ������)
					if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
		            {
		                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
		                {
							link.l3 = RandPhraseSimple("���� �������� ���������� ����.", "���������� ��������...");
							link.l3.go = "LoanForAll";//(���������� � ��������� ���������)
						}
					}
					break;
				}
				else
				{ // ������, �� ����
				    dialog.text = RandPhraseSimple("���� ����? ������� ����.", "������ ������������ ������, ������.");
					link.l1 = RandPhraseSimple("��� ����.", "�� ����� ��� �������. �� ��������.");
					link.l1.go = "exit";
					link.l2 = RandPhraseSimple("����� ���� ��������� ��������.", "������� ��������, ����� ���� ��������� ������.");
					link.l2.go = "quests";//(���������� � ���� ������)
					if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
		            {
		                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
		                {
							link.l3 = RandPhraseSimple("���� �������� ���������� ����.", "���������� ��������...");
							link.l3.go = "LoanForAll";//(���������� � ��������� ���������)
						}
					}
					break;
				}
			}
            if(NPChar.quest.meeting == "0")
			{
				NPChar.quest.meeting = "1";

                // homo 05/08/06
				dialog.Text =NPCharRepPhrase(npchar,
                                LinkRandPhrase("���� ����� ", "��� ��� ", "������ �������� ���� ")
                                +GetFullName(npchar)
                                +LinkRandPhrase(". ���� ���� �����?", ". � ���� ������ ����� �� "
                                +NPCharSexPhrase(NPChar, "�����", "������")
                                +", ��� �� �����?", ". ��� �� � ��� ���� �� ���� ����?"),

        		                LinkRandPhrase("����������� ���, " + GetAddress_Form(NPChar) + ". ��� ��� " + GetFullName(npchar) + ". � ��� ����� ���?",
                                 "������������, " + GetAddress_Form(NPChar) + "! � " + GetFullName(npchar) + ". ���� � ������ ���� ���?",
                                 "��, " + GetAddress_Form(NPChar) + ". ��� �� ������? ������, ���� ����� " + GetFullName(npchar) + ". � ���?"));

				Link.l1 = pcharrepphrase(
                                LinkRandPhrase("������� ���� ����! ", "������ ������! ", "���� ���� ����� ����! ") + "�� � ������� "+ GetFullName(Pchar)+ LinkRandPhrase(", �� ����� �� ������"+NPCharSexPhrase(NPChar,"","�")+" ��� ���, "+NPCharSexPhrase(NPChar,"�������?","?"), " � ����� ��������� ����� � ���� �����! ", " � ����������� ��� �� ���� �����, ���� ��� �� ���!"),
                                LinkRandPhrase("� " + GetFullName(Pchar) + ", �������.", "���� ����� " + GetFullName(Pchar) + ".", "������ �������� ���� ������� " + GetFullName(Pchar)));

                //LinkRandPhrase("� " + GetFullName(Pchar) + ", �������.", "���� ����� " + GetFullName(Pchar) + ".", "������ �������� ���� ������� " + GetFullName(Pchar));
				Link.l1.go = "Meeting";
			}
			else
			{
				link.l1 = PCharRepPhrase("����� ���-��� ������ �� ���� ���������!", "����� ������ ���-��� � ������.");
				link.l1.go = "new question";
 				link.l2 = RandPhraseSimple("� ���� ���� ������ � ����.", "��� ����� ����������.");
				link.l2.go = "quests";//(���������� � ���� ������)
				if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
	            {
	                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
	                {
						link.l3 = RandPhraseSimple("���� �������� ���������� ����.", "���������� ��������...");
						link.l3.go = "LoanForAll";//(���������� � ��������� ���������)
					}
				}
				// --> ��������� ��������� ����, ����� ����� �� �����. eddy
				if (CheckAttribute(pchar, "GenQuest.SeekSpy.City") && pchar.location == (npchar.city + "_town"))
	            {
					link.l4 = NPCharSexPhrase(npchar, LinkRandPhrase("������! ���������� ������� ���, ��� �� �����?! � ��� ���������� ��������� �� ��������� ����������� " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + "!", 
						"���������� ����! � �������� � ��������� ������ " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + " �� ��������� �����������. ��� � ������ ���������� ������.", 
						"������, ��������! ������� ���������� " + characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].lastname + " ����� ����� ��������� ������ � ������."), 
						LinkRandPhrase("�������, ������! ���������� ������� ���, ��� �� �����?! � ��� ���������� ��������� �� ��������� ����������� " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + "!", 
						"�������, � ������ ��� ���������! � �������� � ��������� ������ " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + " �� ��������� �����������. ��� � ������ ���������� ������.", 
						"������, ���������! ������� ���������� " + characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].lastname + " ����� ����� ��������� ������ � ������..."));
					link.l4.go = "SitySpy";
				}
				// <-- ��������� ��������� ����, ����� ����� �� �����
                dialog.Text = NPCharRepPhrase(npchar,
		                PCharRepPhrase(LinkRandPhrase("�, ��� �� "+GetFullName(Pchar)
                        +" - ������ ��������", "��� ���� ������ ������� "
                        + Pchar.name, "�, �� ��� �� ��� ������� "
                         +GetFullName(Pchar))
                                        +LinkRandPhrase(". �����, ���� ��� ��� �� ���� �����!", ". �, ��������, ��� ������ ����.", ". �, ��� ����, ��� �� �������!")
                                        +LinkRandPhrase(" ��, ������, ���� ���� �����?", " ���� ���� �� ���� ���?", " ����� ��������� ���� �� ��������?"),
                                        LinkRandPhrase(TimeGreeting() + ", ������� ", "������������, "+GetAddress_Form(NPChar)+" ", "����������� ���, ������� ")+GetFullName(Pchar)
                                        +LinkRandPhrase(". ��� ����� ���� ����� ������ ���������� ��������, ��� ��, �� ����?", ". ����� �� ������?", ". ��� �� ���� ��� �� ������ ������?")

                                        ),
                                        
                        PCharRepPhrase(LinkRandPhrase("������������, ������� ", "�����������, �� ��� �� "+GetAddress_Form(NPChar)+" ", "�, ������� ")
                                        +GetFullName(Pchar)
                                        +LinkRandPhrase(", � �����"+NPCharSexPhrase(NPChar, " ", "� ")+"�� ������ � ���� �� ����������,",
                                                         ", �� �����, ��� ���"+NPCharSexPhrase(NPChar, " ", "� ")+"��� ������, ��", ", ����, �� ��� ����, ����������, �")
                                        +LinkRandPhrase(" ��� ��� �����?", " ����� �� ������?", " ��� ���� ���� "+NPCharSexPhrase(NPChar, "�������?", "�������?")),
                                        LinkRandPhrase(TimeGreeting() + ", ������� " + GetFullName(Pchar) + ". ��� ���� ������?",
                                        "�, �� ��� �� ��� ������� " + GetFullName(Pchar) + "! ��� �� ������ ������ �� ���� ���?",
                                        "������������, " + GetAddress_Form(NPChar) + " " + Pchar.lastname + ". �� ���-�� ������?"))
                                        );                         
	   			link.l5 = PCharRepPhrase(RandPhraseSimple("���� �����������, � �����.", "���, ������ - ������ ��� � �������."),
                                        RandPhraseSimple("�����, ������. �����!", "������ ���������� �����. �� ��������."));
				link.l5.go = "exit";
			}
			NextDiag.TempNode = "First time";
		break;
		
		case "Meeting":
		    dialog.Text  = NPCharRepPhrase(npchar, LinkRandPhrase("� �� �� �����, ������� " + GetFullName(Pchar)+"?",
                                                                    "� ��� � ���� " + PChar.name+"?",
                                                                    "� ������� ����, " + GetFullName(Pchar)+".")
                                                                     + " � ������ ������, ���� ���� ����?",
                                                    LinkRandPhrase("������� �������������, ������� " + PChar.name,
                                                                    "���"+NPCharSexPhrase(NPChar, " ", "� ")+" ������ ����������, " + GetAddress_Form(NPChar) + " " + PChar.lastname,
                                                                     "����� �������, ������� " + PChar.name) + ". �� ���� �� �� ������ ������ ������ ��� ���, �� ��� ��?");

            link.l1 = PCharRepPhrase("����� ���-��� ������ �� ���� ���������!", "����� ������ ���-��� � ������.");
			link.l1.go = "new question";
            Link.l2 = RandPhraseSimple("� ���� ���� ������ � ����.", "��� ����� ����������.");
			Link.l2.go = "quests";
			if (CheckAttribute(pchar, "GenQuest.Loan.FindCitizen") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenDone") && !CheckAttribute(pchar, "GenQuest.Loan.FindCitizenFalied"))
            {
                if (Characters[sti(pchar.GenQuest.Loan.FindCitizenIdx)].id == Npchar.id)
                {
					link.l3 = RandPhraseSimple("���� �������� ���������� ����.", "���������� ��������...");
					link.l3.go = "LoanForAll";//(���������� � ��������� ���������)
				}
			}
			// --> ��������� ��������� ����, ����� ����� �� �����. eddy
			if (CheckAttribute(pchar, "GenQuest.SeekSpy.City") && pchar.location == (npchar.city + "_town"))
	        {
				link.l4 = NPCharSexPhrase(npchar, LinkRandPhrase("������! ���������� ������� ���, ��� �� �����?! � ��� ���������� ��������� �� ��������� ����������� " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + "!", 
					"���������� ����! � �������� � ��������� ������ " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + " �� ��������� �����������. ��� � ������ ���������� ������.", 
					"������, ��������! ������� ���������� " + characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].lastname + " ����� ����� ��������� ������ � ������."), 
					LinkRandPhrase("�������, ������! ���������� ������� ���, ��� �� �����?! � ��� ���������� ��������� �� ��������� ����������� " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + "!", 
					"�������, � ������ ��� ���������! � �������� � ��������� ������ " + XI_ConvertString("Colony"+characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].city+"Gen") + " �� ��������� �����������. ��� � ������ ���������� ������.", 
					"������, ���������! ������� ���������� " + characters[GetCharacterIndex(pchar.GenQuest.SeekSpy.MayorId)].lastname + " ����� ����� ��������� ������ � ������..."));
				link.l4.go = "SitySpy";
			}
			// <-- ��������� ��������� ����, ����� ����� �� �����
			Link.l5 = PCharRepPhrase(RandPhraseSimple("���� �����������, � �����.", "���, ������ - ������ ��� � �������."),
                                        RandPhraseSimple("�����, ������. �����!", "������ ���������� �����. �� ��������."));
			Link.l5.go = "exit";
		break;
		
		case "new question":
            dialog.text = NPCharRepPhrase(npchar,
		                PCharRepPhrase(LinkRandPhrase("�, �� � ���"+NPCharSexPhrase(NPChar, " ", "� ")+" ����, ��� �������, ���������, ���� ������.", "������ ���������, �������? ��� �, � ����... �������� �� ��������� ����.", "��, " + PChar.name + "?"),
                                        LinkRandPhrase("��, ���� ���� ���?", "����, ������� "+PChar.name+", �� ������� ���������? ", "������� ���������, �������, ����� �� ������� �����.")),
		                PCharRepPhrase(LinkRandPhrase("��� �� ������ ������ "+ GetAddress_Form(NPChar) + "?", "� ��� ������ �������.", "��� ����� ����, ������� ����������� ������, �������."),
                                        LinkRandPhrase("������ ���"+NPCharSexPhrase(NPChar, " ", "� ")+" ��������� �����������, " + GetAddress_Form(NPChar) + " " + PChar.lastname + ". ��������.",
                                                        "��, " + GetAddress_Form(NPChar) + "?",
                                                        "������ ���������, �������? ��� �, � ����..."))
                            );

            // homo 25/06/06
			link.l1 = LinkRandPhrase ("����� ����� � ���� � ������� �������?",
                                    "��� ���������� � ���� �����?",
                                    "��� ����� ����� �� ����?");
			link.l1.go = "rumours_citizen";
			link.l2 = LinkRandPhrase("�� ��������� ��� ������?", "���-�� � ����� �� ����� ������ ���-����...", "���������� ������...");
			link.l2.go = "town";  //(���������� � ���� ������)
			if(Rand(1)==0)
			{
				link.l3 = LinkRandPhrase("���� ������, ��� ��� ������� �� ����� ��������.", "�� ������, ��� ������� ��� �� ����� �������?", "�������� �� ������ ���-��� � ������� �������.");
				link.l3.go = "info"; //(���������� � ���� ������)
			}
			else
			{
				link.l3 = "���������� ��� � ����� �������.";
				link.l3.go = "colony";
			}
            link.l4 = RandPhraseSimple("���� ������ ����!", "� ���� � ��� ����.");
			link.l4.go = "quests";  //(���������� � ���� ������)
			link.l5 = PCharRepPhrase(RandPhraseSimple("���� �����������, � �����.", "���, ������ - ������ ��� � �������."),
                                        RandPhraseSimple("�����, ������. �����!", "������ ���������� �����. �� ��������."));
			link.l5.go = "exit";
		break;
		
		case "colony":
			dialog.text = LinkRandPhrase("������ ������, � �� �������-�� ���������� � ����. �� ���-��� ���� ����������.",
                           "��� �� ��� ���������� � ����� �������?", "������ ����������. � ��� �� ������ ��������?");
            switch(Rand(1))
			{
				case 0:
					link.l1 = "��� ��� �� �����?";
					link.l1.go = "colony_town";
				break;

				case 1:
					link.l1 = "��� �� ������ � �����, ���������� �����?";
					link.l1.go = "fort";
				break;
            }
			link.l2 = "��������� � ���-������ ������.";
			link.l2.go = "new question";
		break;
	  // to_do
		case "colony_town":
            dialog.text = "�, �� �� �����������? ��� ������? ���������� � ������ ���� ������.";
			link.l1 = "�����, �����.";
		    link.l1.go = "exit";
		break;
		
		case "fort":
            dialog.text = "������ ��� ��� �� ����.";
			link.l1 = "����.";
		    link.l1.go = "exit";
		break;
		//============================ ��������� ����. ����� �� ������� ������ �� ����� ============================
		case "SitySpy":
			dialog.text = NPCStringReactionRepeat(NPCharSexPhrase(npchar, "��, � � ����� ��� ���? � ������� ������!", "��, � � �� ����� ��� ���? � ���� � ���� ������, � ����� ������ ������ �� �����!"), 
				NPCharSexPhrase(npchar, "��� ����� ��� �� ���� �����? ������� ��� ���, ��� � �� �����!", "� �� �������� ��� ���, ��� � �������!"), 
				NPCharSexPhrase(npchar, "��, ������� �����? ��������� �� ����, � ������� ������!", "�� ��� �� ����� �� ��� ��������� �� ������ ��������� �����������?"), 
				NPCharSexPhrase(npchar, "���, �����?! �� ��� �� � ����� ������ �� �����!!!", "����������, ��������� �� ���� �������, ����� ���!!"), "block", 0, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat("��� ����� ����������� ���� �����?", "��, ��������, � ��� ��� ���������...",
                      "��������, ������� ������...", "������, ������...", npchar, Dialog.CurrentNode);
		    link.l1.go = DialogGoNodeRepeat("SitySpy_1", "exit", "exit", "exit", npchar, Dialog.CurrentNode);
			// ==> �������� ��������������
			int num = FindNearCharacters(npchar, 20.0, -1.0, -1.0, 1.0, false, true);
			if(num > 0)
			{
				for(int i = 0; i < num; i++)
				{
					iTemp = chrFindNearCharacters[i].index;
					if (CheckAttribute(&characters[iTemp], "CityType"))
					{
						if(characters[iTemp].CityType == "soldier" || characters[iTemp].CityType == "citizen")
						{
							break;
						}
					}
				}
				pchar.GenQuest.SeekSpy.Idx = chrFindNearCharacters[i].index; //������ ����������
				pchar.GenQuest.SeekSpy.Type = characters[sti(pchar.GenQuest.SeekSpy.Idx)].chr_ai.type; //��� ��� �����
				pchar.GenQuest.SeekSpy.BaseIdx = npchar.index; //������
			}
		break;
		case "SitySpy_1":
			if (sti(pchar.GenQuest.SeekSpy.Idx) == -1 || sti(pchar.GenQuest.SeekSpy.Idx) == 1) //�� ������ ������
			{
				dialog.text = "�� ��� ������! ����������, ��� �� ���� ��������?! ���� � ��� �����-�� ��������, �� ��������� � �����������, � ��� ��������!",
				link.l1 = "��... �������, �� ����� ��� ��������� ���. � ��� ����...";
				link.l1.go = "exit";
				break;
			}
			dialog.text = "��� ������ � ���� ������! " + NPCharSexPhrase(npchar, GetFullName(&characters[sti(pchar.GenQuest.SeekSpy.Idx)]) + " ����� ��� �������, ����� " + NPCharSexPhrase(&characters[sti(pchar.GenQuest.SeekSpy.Idx)], "��", "���") + " � ���� �����.",
				"��, �������, �� ���, ���� �� " + NPCharSexPhrase(&characters[sti(pchar.GenQuest.SeekSpy.Idx)], GetAddress_Form(NPChar) + " " + characters[sti(pchar.GenQuest.SeekSpy.Idx)].lastname, GetFullName(&characters[sti(pchar.GenQuest.SeekSpy.Idx)])) + " �����.");				
			link.l1 = "��� �, ������� ���������...";
		    link.l1.go = "SitySpy_2";
			sld = &characters[sti(pchar.GenQuest.SeekSpy.Idx)];
			sld.dialog.currentnode = "SeekSpy_Checking";
			pchar.GenQuest.SeekSpy.time = 4.0; //����� �� ���������
			if (sld.chr_ai.type == "merchant") pchar.GenQuest.SeekSpy.time = 0.0; //������� ������� �����
            LAi_SetActorTypeNoGroup(sld);
		break;
		case "SitySpy_2":
			sld = &characters[sti(pchar.GenQuest.SeekSpy.Idx)];
			LAi_SetActorTypeNoGroup(npchar);
			LAi_ActorTurnToCharacter(npchar, sld);
			LAi_ActorDialog(sld, pchar, "", stf(pchar.GenQuest.SeekSpy.time), 0); 
			DialogExit();
		break;
		//=================== ���� ������ ����. ����� ������ �� �����, ����� ��������� ==================
		case "SeekSpy_Checking":
			dialog.text = "��, � �����������, " + NPCharSexPhrase(&characters[sti(pchar.GenQuest.SeekSpy.BaseIdx)], "�� ������������� ������� ������.", "��� ������������� ������� ����������.");
			link.l1 = RandPhraseSimple("� �����. ������� �� ������.", "��� ����. ������� �� ������.");
			link.l1.go = "SeekSpy_Checking_1";
		break;
		case "SeekSpy_Checking_1":
			switch (pchar.GenQuest.SeekSpy.Type)
			{
				case "guardian": LAi_SetGuardianTypeNoGroup(npchar); break;
				case "patrol":   LAi_SetPatrolTypeNoGroup(npchar);   break;
				case "citizen":  LAi_SetCitizenTypeNoGroup(npchar);  break;
				case "merchant": LAi_SetMerchantTypeNoGroup(npchar); break;
			}
			LAi_SetCitizenTypeNoGroup(&characters[sti(pchar.GenQuest.SeekSpy.BaseIdx)]);
			NextDiag.CurrentNode = "First Time";
			DialogExit();
		break;
	}
}
