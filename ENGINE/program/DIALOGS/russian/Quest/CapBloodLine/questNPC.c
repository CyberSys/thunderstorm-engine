// ����
void ProcessDialogEvent()
{
	ref NPChar, sld;
	aref Link, NextDiag;
    string sLocator;
    int iTime;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);




	switch(Dialog.CurrentNode)
	{

		// ----------------------------------- ������ ������ - ������ �������
		case "Exit":
            NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		
        case "Exit_Away":
            
            LAi_SetActorType(npchar);
            LAi_ActorGoToLocation(npchar, "reload", Pchar.questTemp.sLocator, "none", "", "", "", sti(Pchar.questTemp.iTime));
            NextDiag.CurrentNode = NextDiag.TempNode;
            DialogExit();
        break;
        
        case "Exit_RunAway":

            LAi_SetActorTypeNoGroup(npchar);
            LAi_ActorRunToLocation(npchar, "reload", Pchar.questTemp.CapBloodLine.sLocator, "none", "", "", "", sti(Pchar.questTemp.CapBloodLine.iTime));
            NextDiag.CurrentNode = NextDiag.TempNode;
            DialogExit();
        break;
        
        case "fight":
			DialogExit();
			NextDiag.CurrentNode = NextDiag.TempNode;
   	        LAi_SetWarriorType(NPChar);
            LAi_group_MoveCharacter(NPChar, "TmpEnemy");
            LAi_SetWarriorType(NPChar);
            /*
            LAi_group_SetHearRadius("TmpEnemy", 100.0);
            LAi_group_FightGroupsEx("TmpEnemy", LAI_GROUP_PLAYER, true, Pchar, -1, false, false);
            LAi_group_SetRelation("TmpEnemy", LAI_GROUP_PLAYER, LAI_GROUP_ENEMY);
			LAi_group_Attack(NPChar, Pchar);
			*/
			LAi_group_FightGroups("TmpEnemy", LAI_GROUP_PLAYER, true);
			LAi_group_FightGroups(GetNationNameByType(ENGLAND) + "_citizens", LAI_GROUP_PLAYER, true);
			AddDialogExitQuest("MainHeroFightModeOn");
			chrDisableReloadToLocation = false;

		break;

		case "First time":
        	dialog.text = "������";
            link.l1 = "...";
            link.l1.go = "Exit";
            npchar.quest.meeting = "1";
            

            if (CheckAttribute(npchar, "CityType") && npchar.CityType == "soldier")
            {

                if(Pchar.questTemp.CapBloodLine.stat == "PrepareToEscape2_1")
                {
                    dialog.text = "������� ����� �������, � �� �������� ����.";
					link.l1 = "�����, ���� ���������. � ����� ����� ��������� ��������, �������� ����, ������. �� ���������� � ��� ����������, � ������ ����� � �������� ������� �������.";
					link.l1.go = "SQStep_0";
					Pchar.questTemp.CapBloodLine.stat = "PrepareToEscape2_2";
				    break;
                }

        		switch (rand(5))
					{
						case 0: ////////////////////////////////////////
							dialog.text = "� ���� ��� ��� �������� ��� ��� ����?";
							link.l1 = "� �������� ��������� ���������� ������.";
							link.l1.go = "exit";
						break;

						case 1:
							dialog.text = "�, ��� ��, ������ ����. �� ������ ����, ����� ���� ������� � ''����� �� �����''";
							link.l1 = "� ���, ��� �� ����������.";
							link.l1.go = "exit";
						break;

						case 2: ///////////////////////////////////////////
							dialog.text = "���� �����, ���!";
							link.l1 = "� ���� ��� ���� ������, ��� � �� �� ���.";
							link.l1.go = "exit";
						break;

						case 3:
							dialog.text = "����� ������, � � �������� ������ �����. ��� ������������ ����������� ����������� � ��������� ��� �� ������ ����.";
							link.l1 = "����������. �� ����� �� ���� ������, ������ � ���� �����.";
							link.l1.go = "exit";
						break;

						case 4: ///////////////////////////////////////////
							dialog.text = "���� ���� ������� ���������, �� ������� ���� ����-������ �������, � ����� ����������� �� ���������.";
							link.l1 = "����������.";
							link.l1.go = "exit";
						break;



						case 5: ////////////////////////////////////////////
							dialog.text = "������� ����� �������, � �� �������� ����.";
							link.l1 = "��� �������, ������.";
							link.l1.go = "exit";
						break;


					}
    		}
    		
    		if (CheckAttribute(npchar, "CityType") && npchar.CityType == "citizen")
    		{
            	dialog.text = TimeGreeting() + ", ������.";
        		link.l1 = "�����������.";
        		link.l1.go = "Exit";
        		if( Pchar.questTemp.CapBloodLine.stat == "WakerOffer")
        		{
                    link.l2 = RandPhraseSimple("�� ����������, ��� ��� ����� ������� ������?", "��� ����� ������ �����. �� ������ ���?");
                    link.l2.go = "Citizen_0";
                }
    		}
    		
    		if (CheckAttribute(npchar, "CityType") && npchar.CityType == "citizen" && npchar.location.group == "merchant")
    		{
                if(findsubstr(Pchar.questTemp.CapBloodLine.stat, "PrepareToEscape" , 0) != -1)
                {
                        if (CheckAttribute(npchar, "quest.bGoodMerch") && sti(Pchar.reputation) >= 55)
                        {
                            dialog.text = "�, ������ ����, ������������! � ��� � �� ������������"+NPCharSexPhrase(NPChar, " ", "� ")+" ��� ����� �� �������� ����� ���������� ����, � ��� �� ��� ��� ����� ��������� � �������. ��� ������, ��� � �������� ��� �������!";
                            link.l1 = "�������������, ��� ������, ��� ���� ��������� ������������� � ����� ���������� ������. �� ���������� ��, "+NPCharSexPhrase(NPChar, "������� ���� ", "��� ������� ")+", ��� � ���� � ���������� ����� ��������� ����������� ����������� ������� � �����-������� ���������� ��� ������� ���� � �����?";
                            link.l1.go = "Merchant_0";
                            break;
                        }
                }
                if (CheckAttribute(npchar, "quest.bGoodMerch"))
                {
                    dialog.text = TimeGreeting() + ", ������.";
                    link.l1 = "�����������.";
                }
                else
                {
                    dialog.text = "�� ��� ����� �������������. � �� ����, ������ �������!";
                    link.l1 = "��� �����.";
                }
        		link.l1.go = "Exit";
    		}
    		
    		if (npchar.id == "Bridgetown_Poorman")
    		{
            	dialog.text = "��, ������ ������� �������... � ��� ��, ������ ����.";
        		link.l1 = "�����.";
        		link.l1.go = "Exit";
    		}
    		
    		if (npchar.id == "QuestCitiz_Bridgetown")
    		{
            	dialog.text = TimeGreeting() + ", ������.";
        		link.l1 = "������.";
        		link.l1.go = "Exit";
    		}
    		
    		if (npchar.id == "SolderTakeBlades")
    		{
            	dialog.text = RandPhraseSimple("� ����� ��� ��� ���� �����������, ��� ������ � �������?", "�� �� ��� ���������� ������, ������ �� ��������?");
        		link.l1 = RandPhraseSimple("���...", "��� �� �����...");
        		link.l1.go = "STBStep_0";
    		}
    		
            
            
		break;
		
		
		case "Citizen_0":
		
            if (!CheckAttribute(npchar, "quest.btmp"))
            {
                npchar.quest.btmp = true;
                if(rand(3)==2)
                {
                    dialog.text = LinkRandPhrase("����� � ������� �������� - ��� ���. �� ������ �������� ��� �������.", "������� ����� � �������, �����. ���� ��������� �����, �� �������.", "�������� � ����� ������ ���... � ��������, ��� ��?");
                    link.l1 = "�������.";
                    link.l1.go = "Exit";
                }
                else
                {
                    dialog.text = RandPhraseSimple(RandPhraseSimple("�� ����, �����!", "����� �� ���� ������."), RandPhraseSimple("� �� ����, ������ �������!", "� ��� ���? � ���� ������ ������� �����... �, ��� ��, ������������."));
                    link.l1 = "�������.";
                    link.l1.go = "Exit";
                }
                
            }
            else
            {
                dialog.text = "������, �� ��� ���������� � ���� �� ����.";
                link.l1 = "��������.";
                link.l1.go = "Exit";
            }

		break;
		
		case "Merchant_0":
		
        	dialog.text = "� �� ���� ����������, ��� ���� ��� ��� ������������, � ���� ���� �������, ��� �� ������� ����� � �������������. ��� �� �����, ����� � ���� �������� �� ����� ��� �����������, ��� ���...";
    		link.l1 = "����������� �� ��� ������� ����� �������, ��� ������������ � ������������� ����������� ��������� ������ � ���������� ������� ������ �����, ��� ��� � ���������� ��������. ���� �� ��������� ����� ����� ������...";
    		link.l1.go = "Merchant_1";
    		
		break;
		
		case "Merchant_1":

        	dialog.text = "��� ��� ������ ��� ������, �������, ��� � �. ���, ��������, ������ ����. � ����� ���!";
    		link.l1 = "�������� ����������.";
    		link.l1.go = "Exit";
            NextDiag.TempNode = "Merchant_2";
            Pchar.questTemp.CapBloodLine.stat = "ReadyToEscape";
            GiveItem2Character(Pchar, "Weapon_for_escape");
            AddQuestRecord("WeaponsForEscape", "10");
    		CloseQuestHeader("WeaponsForEscape");

		break;
		
		case "Merchant_2":

        	dialog.text = "� ����� ���, ������ ����!";
    		link.l1 = "���������.";
    		link.l1.go = "Exit";
    		NextDiag.TempNode = "Merchant_2";

		break;
		
		
		// ==> �������� ������, �������.
		case "STBStep_0":
        	dialog.text = "� ��, ���������� ����  ��� ������! ������� �������� ���������� ������, ����� �� ������ ���� ��� �������...";
    		link.l1 = "��� ��������, � ������� �� ����.";
    		link.l1.go = "STBStep_1";
            link.l2 = "������ �� ���������, ��� ���� ������!";
            link.l2.go = "fight";
		break;

		case "STBStep_1":
		
                dialog.text = "� �����, ����������.";
                link.l1.go = "Exit_Away";
                Pchar.questTemp.sLocator = "reload1_back";
                Pchar.questTemp.iTime = 40;
                RemoveCharacterEquip(pchar, BLADE_ITEM_TYPE);
                RemoveCharacterEquip(pchar, GUN_ITEM_TYPE);
                while (FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE) != "" && FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE) != "blade5")
                {
                    TakeItemFromCharacter(pchar, FindCharacterItemByGroup(pchar, BLADE_ITEM_TYPE));

                }
                while (FindCharacterItemByGroup(pchar, GUN_ITEM_TYPE) != "")
                {
                    TakeItemFromCharacter(pchar, FindCharacterItemByGroup(pchar, GUN_ITEM_TYPE));
                }
                GiveItem2Character(Pchar, "unarmed");
                EquipCharacterByItem(Pchar, "unarmed");
                
                //��������
                pchar.quest.CapBloodDetectBlades1.over = "yes";
                pchar.quest.CapBloodDetectBlades2.over = "yes";
                chrDisableReloadToLocation = false;

		break;
		

        // --> ��������� ������


 		case "SQStep_0":

            dialog.text = "�������? �� �� �����������!.. ������� �� ���... ���...?";
        	link.l1 = "������ ���, ��� ��� �������.";
            link.l1.go = "SQStep_1";
		break;
		
 		case "SQStep_1":

            dialog.text = "� ���������� ����������� ����. ��� ��������� ���� ���?";
        	link.l1 = "��� ��� ������� ��������, �� ��������� �� ������� �����������.";
            link.l1.go = "Exit_RunAway";
            Pchar.questTemp.CapBloodLine.sLocator = "houseSp2";
            Pchar.questTemp.CapBloodLine.iTime = 20;
            
            string smodel = NPChar.model;
            if (findsubstr(smodel, "eng_mush" , 0) != -1) smodel = "sold_eng_"+(rand(7)+1);
            sld = GetCharacter(NPC_GenerateCharacter("CPBQuest_Solder", smodel, "man", "man", 10, ENGLAND, 1, false));
            sld.dialog.filename = "Quest\CapBloodLine\questNPC.c";
            SetFantomParamHunter(sld); //������ �����
            sld.SaveItemsForDead = true; // ��������� �� ����� ����
            sld.DontClearDead = true;
            LAi_SetActorTypeNoGroup(sld);
            LAi_ActorSetLayMode(sld);
            LAi_SetImmortal(sld, true);
			ChangeCharacterAddressGroup(sld, "CommonFlamHouse", "reload","reload3");

            sld = &characters[GetCharacterIndex("Griffin")];
            ChangeCharacterAddressGroup(sld, "CommonRoom_MH4", "barmen","bar1");
            LAi_SetActorTypeNoGroup(sld);
            LAi_ActorTurnToLocator(sld, "goto","goto2");
            LAi_SetStayTypeNoGroup(sld);
            sld = &characters[GetCharacterIndex("Spain_spy")];
            ChangeCharacterAddressGroup(sld, "CommonRoom_MH4", "goto","goto2");
            sld.dialog.currentnode = "SSStep_3";
            LAi_SetActorType(sld);
            LAi_ActorDialog(sld, pchar, "", 0, 0);

            
            pchar.quest.PrepareToEscape2_3.win_condition.l1          = "location";
            pchar.quest.PrepareToEscape2_3.win_condition.l1.location = "CommonFlamHouse";
            pchar.quest.PrepareToEscape2_3.function                  = "_DeadSolder";

            Pchar.questTemp.CapBloodLine.stat = "PrepareToEscape2_3";
            DoQuestFunctionDelay("SpainSpyAttack", 20.0);
            
		break;




	}
}