
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	int iTemp, i;

	string NodeName = Dialog.CurrentNode;
	
	switch(Dialog.CurrentNode)
	{
		case "First time":
			dialog.text = "��� �� �����? �� ���������� ���� �� ��������, � �����.";
			link.l1 = "������, ��� ������ �������� � ����.";
			link.l1.go = "exit";
			NextDiag.TempNode = "First time";
			if (npchar.quest.meeting == "0")
			{
				npchar.quest.meeting = "1";
				pchar.questTemp.LSC = "AdmiralFoundHero";
				if (CheckAttribute(loadedLocation, "storm"))
				{
					dialog.text = "�-�-�, ��� � ��. ��� ������, ��� �� ��� ������ �� ���. �� �����, ����� �� ���� ������, �� ��� � ����� �����...";
					link.l1 = "�� �� ���� ������ �� �������. ���� ����� " + GetFullName(pchar) + ".";
					link.l1.go = "FT_1";
					PChar.quest.LSC_admiralOwnFind.over = "yes";
				}
				else
				{
					dialog.text = "�-�-�, ��� � ��. � ����, ����� �� ������� �� ���.";
					link.l1 = "�����������. ���� ����� " + GetFullName(pchar) + ".";
					link.l1.go = "FT_1";
				}
			}
			//��������� ����
			if (pchar.questTemp.LSC == "toAdmNarvalRing" && CheckCharacterItem(pchar, "DOjeronRing"))
			{
				link.l4 = "�������, � ���� ���� ��� ���� ������ ����������. ��, ��� ��� �������...";
				link.l4.go = "NarvalRing_1";
			}
			if (pchar.questTemp.LSC == "NarvalDestroyed")
			{
				link.l4 = "� ��� ������, ���� '������' ���������. ������� ��� ���� ��� ������, ����� �� ����!";
				link.l4.go = "DestrNarval_1";
			}
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		case "FT_1":
			dialog.text = "����� ������, ��������, ������ ������������... � ���� ����� " + GetFullName(npchar) + ", � ������� ����� ������ ���������� ��������!"; 
			link.l1 = "��, � ���� ������ � ����! �� ��� ����� ��������� �������, ��� ����� �� ������ �� '����� �����' � ������. ��� ��� ���� �� ���������!";
			link.l1.go = "FT_1_1";
		break;
		case "FT_1_1":
			dialog.text = "��, ��� ��� ��� ������ � ��������� ����... ������, ��� ����������. ��� ��� �� ������� �� ��������� ��������, � � ���� �� ������ ����� ��������� �����... ������, ��� ��� ������� ����� ��� ���, ��������� ������ ������ �, ������� � ����.";
			link.l1 = "�������...";
			link.l1.go = "FT_1_2";
		break;
		case "FT_1_2":
			dialog.text = "�� ������ �������, ����� ���� ������� �����. ��� ���, ����� � �� �������� �����, ��� �����, ��� ���� �������... ��, ������ �� �� �� ����!";
			link.l1 = "������, ��� �����. � ����������� ������, " + npchar.name + ".";
			link.l1.go = "FT_2";
		break;
		case "FT_2":
			dialog.text = "������ ���! � ���� �������, ��� �� ���� � ���� �� ������ ������ �������, �� ���� ����������� ������, � ������ ������ ���������� ��� ������. ��� ���� �������?";
			link.l1 = "������. � ��� �� �����, ��� � ��� ����� ������������?";
			link.l1.go = "FT_3";
		break;
		case "FT_3":
			dialog.text = "� ������� �������� ���� � � ������ ���������� ������. �� ��������� �� �� �����, ��� ��� � ������ � ��� ����������, ��-��... � ����� �� ���� ����� ����������� � ��������� �������\n"+
				"������: ��� �������� ������ ���������� �������� ����� ����� �� �����. ��� ����� ����������� �������� �������, ������� ��������� � ������������ ��������, �� ���� � ���� ������������... ������, � �� �� ������ ���������� � �������?";
			link.l1 = "��� ���������. ������� ����� ��������, ���-�� ��������� �����, � ����� ��� ������ ����� �������.";
			link.l1.go = "FT_4";
		break;
		case "FT_4":
			dialog.text = "�� �� ���� ������ ��������. ��� ��� � ������� ����?";
			link.l1 = "�� ���, ������ ��, ������������... ��� ��������� ���������, ����� ��, ��� � ��, � �����.";
			link.l1.go = "FT_5";
		break;
		case "FT_5":
			dialog.text = "�������... �� ������, ����������, ��� �� ����� ���� ���. ���� �� �� ��������� ���� �������� ��� ���������, ������� ��� ������ ����� - ����� ��� �����. ������ �� ���� ����������� ������, �� ������ �������, ��������� �� ������� �������� ����� �� �����! ���, ��� ��������!..";
			link.l1 = "��, �������. �������� ��������������. ������, � ��������� ���� ����� ����� � �����-�� ������� ��������������. �������, ��� ��� � ���� ��� �����.";
			link.l1.go = "FT_6";
		break;
		case "FT_6":
			dialog.text = "���, ��� �� ����� ����. ���� ������ �������... ��, ���������. ����, � ������� ���, ������ � ����� ������������. ������: �� �� ������ ���������� �� ����� ������� ������. ���� �� ������ ����-��, �� ���� ���� �. ��� ����?";
			link.l1 = "��, ����...";
			link.l1.go = "FT_7";
		break;
		case "FT_7":
			dialog.text = "��������� �����. � ��� ��� � ������ ���� ��� ����������, ��� ���������� ����� '������' � '������'. � �����, ��� ��������� ����, ������� ����� ���������������� �������� - ��� ����� � �����. �� �� �������� ��� ������� ��� ���������� ����� ���������� - ��� ���� '��� ��������' � ������ '�������'. ������� ��� ����� �� ������, ������� ����� ����� ������ �� ������\n"+
				"��� ���, �������� ����� ���� �������� ��������� �� ����� - ������ ��� �����. ���������� ��������� ������ ����� �������� � ������������� ������, � �� ���� ���������� � ��� ��������� ���� ������. ������� �� ���� ���� ��� ����� �� ����. �������?";
			link.l1 = "�������.";
			link.l1.go = "FT_7_1";
		break;
		case "FT_7_1":
			dialog.text = "������ ������. ������: ����������� ������� �������, ����� � ������ ������������. ��������� ������ ��� ����� ����������, � ��������� ����� ������.";
			link.l1 = "� ��� �������.";
			link.l1.go = "FT_7_2";
		break;
		case "FT_7_2":
			dialog.text = "���������: �� ��������� � ������ ���������� �������� �����. ���� ���� ��������� ���������� � ����� ������� - �� ������������� ��������� ����������� � ����������� ��� ������. � ��� ������, ��� ����� �� ����� � ���� ������ ���...";
			link.l1 = "������ ���������... �� ��� ��, ��� ����. ��� ������?";
			link.l1.go = "FT_8";
		break;
		case "FT_8":
			dialog.text = "�� � �����: ��� ������������ ��������, ������� ������ � �����, ������������ ������.";
			link.l1 = "��, � ��� �� ����� ������ ���������, ����������.";
			link.l1.go = "FT_9";
		break;
		case "FT_9":
			dialog.text = "���, �� ��� � ��� �����������? ���, � �������, ��, ��� ���������� ����, ����� � �����. � ������, ����� ���� ��� �����������!\n� ��� ���� ���������, ��� ������������ ��������, ���������� �������������� ������. ��� ��� ��� ���� ������ ���� �����!";
			link.l1 = "��������!! � � ����� ��������� �� ��������!";
			link.l1.go = "FT_10";
		break;
		case "FT_10":
			if (pchar.questTemp.LSC == "AdmiralFoundHero")
			{
				dialog.text = "��� �����-�������! ������ �������� �� ���������, �� ������ ��������, � ���... �� � ���� ���� � ������� �������. ����, ��� ������������� ���������� ������, ���������� 1/50 �����!";
				link.l1 = "�� ����� �� ���������!!";
				link.l1.go = "FT_11";
				pchar.money = makeint(sti(pchar.money) / 50);
			}
			else
			{
				dialog.text = "� ������ �������� ����� �� ����������. ������-�� ����� ��������� �������� ������� ������� ����� � ����� ��������� ��� �������. �� ��� ��� �� ������� �����, � ������ �� ������ �� ��� ������� �� �������� � �����, ������ ����� ������ � ��������� ���� ���������� ���� ����. �� ���� ���� �� ��������� ������.";
				link.l1 = "��� ��� ��� ����� ������ �� ��������!";
				link.l1.go = "FT_12";
				pchar.money = 0;
			}
		break;
		case "FT_11":
			dialog.text = "������! �� ���, �� ��������� ��������... ������-��, ���� ��� ���� ��� ������� ��� �� � ����, � ��� ���� ������� �����...";
			link.l1 = "��� ��� ��� ����� ������ �� ��������!";
			link.l1.go = "FT_12";
		break;
		case "FT_12":
			dialog.text = "�� �� ������, ��������, ���� ��� �� ��������. �� ������, �� ������ ���, ��� ��� � ����� ���������. ��� ������� �� ���������� ����� ����, ��� ���� ��������� � ������� �����, ��� ������� �� ��������� � ���� ����������� �����.\n� ��� ������ ����� � ���, ����� ������. �, ����� � ����, � ������ �� �� ����������� - ���� �� ���, � �� ���!";
			link.l1 = "�� ��, �������������� ��� � ����...";
			link.l1.go = "FT_13";
		break;
		case "FT_13":
			dialog.text = "�� ������� � ����� ���� ������, �� � ������ ������� ���� ������! � ���� �����, ��� � ��������� ���, ���� �� ��� �� �����������, � ���� ������ ��������.";
			link.l1 = "�-��... �� ��� ��, �������� ���������. ������, � ���� ��� ������� ������.";
			link.l1.go = "FT_14";
		break;
		case "FT_14":
			dialog.text = "��� �����. � �� ������� ���� ��� ������, ��� ��������� �������!";
			link.l1 = "� �����...";
			link.l1.go = "FT_15";
		break;
		case "FT_15":
			dialog.text = "�� ��� � �������... � ������ �� �������� � ������ ������ ������������ �� ������ ��� ������. �����, ��� ������, ����������� ������ � ������.";
			link.l1 = "��� � ��������, ���.";
			link.l1.go = "FT_16";
		break;
		case "FT_16":
			chrDisableReloadToLocation = false;
			//bDisableFastReload = false; //������� �����������
			//������� �����
			ref rLoc = &locations[FindLocation("LostShipsCity_town")];
			DeleteAttribute(rLoc, "reload.l61.disable");
			DeleteAttribute(rLoc, "reload.l70.disable");
			DeleteAttribute(rLoc, "reload.l72.disable");
			pchar.questTemp.LSC = "AfterAdmiral";
			RemoveCharacterEquip(pchar, BLADE_ITEM_TYPE);
			RemoveCharacterEquip(pchar, GUN_ITEM_TYPE);
			RemoveCharacterEquip(pchar, SPYGLASS_ITEM_TYPE);
			RemoveCharacterEquip(pchar, PATENT_ITEM_TYPE);
			RemoveCharacterEquip(pchar, CIRASS_ITEM_TYPE);
			RemoveCharacterEquip(pchar, MAPS_ITEM_TYPE);	
			//������ ��� ��������, ����� ��������� -->
            aref arItems;
			string sName;
    		makearef(arItems, pchar.items);
    		int	Qty = GetAttributesNum(arItems);
    		for (int a=0; a<Qty; a++)
    		{
                sName = GetAttributeName(GetAttributeN(arItems, a));
				rLoc = ItemsFromID(sName);
				if (rLoc.ItemType == "QUESTITEMS")
				{
					pchar.questTemp.items = sName;
				}
    		}
			DeleteAttribute(pchar, "items");
			pchar.items = "";
			makearef(arItems, pchar.questTemp.items);
    		Qty = GetAttributesNum(arItems);
    		for (a=0; a<Qty; a++)
    		{
                sName = GetAttributeName(GetAttributeN(arItems, a));
				GiveItem2Character(pchar, sName);
    		}
			DeleteAttribute(pchar, "questTemp.items");
			//<-- ������ ��� ��������, ����� ���������	
			AddQuestRecord("ISS_MainLine", "3");
			if (npchar.chr_ai.type == LAI_TYPE_ACTOR) LAi_SetLSCoutTypeNoGroup(npchar);
			NextDiag.TempNode = "First time";
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;

		//����� � ��, ���� ��� �� ������ � ��������
		case "FoundHero":
			npchar.quest.meeting = "1";
			NextDiag.TempNode = "First time";
			pchar.questTemp.LSC = "AdmiralNotFoundHero";
			dialog.text = "���, ��� ������ � ��, �������� � ��������� ������. �����-�� ���, ��������, ������ ��� ����� � ������ ������ ���� �� ����� ������? �� ���, �� ������ ����� ��������?";
			link.l1 = "�����������. ����� ��������?";
			link.l1.go = "FoundHero_1";
		break;
		case "FoundHero_1":
			dialog.text = "������ �������, ��� �� �� ���� � ���, ��� �� ��� ����� ������ ����� ��, ��� ������ �������� � ������?";
			link.l1 = "�� ����...";
			link.l1.go = "FoundHero_2";
		break;
		case "FoundHero_2":
			dialog.text = "�������� ������ �� ����������� �� ���������������! � ������ ������, ��� ��.";
			link.l1 = "� ����� �������� �� ���������, � ��� �� ������ ������� ������. �������, ��� ������ ����� �����... ���� ����� " + GetFullName(pchar) + ".";
			link.l1.go = "FT_1";
		break;

		//�� ������ ������ ��������
		case "NarvalRing_1":
			dialog.text = "������.";
			link.l1 = "� ��� ������� ������ � ����� ������� '���������', � ���������� �������� ����� ��� ���������� �������...";
			link.l1.go = "NarvalRing_2";
		break;
		case "NarvalRing_2":
			dialog.text = "��� �� �������? ������ �������, �� ����!";
			link.l1 = "��� ������. ��� ���, ��������.";
			link.l1.go = "NarvalRing_3";
		break;
		case "NarvalRing_3":
			dialog.text = "��-��, �� ��� �� ������ '�������'! ���, ��� �� ��� �����, ��������?";
			link.l1 = "� ����� '����������'.";
			link.l1.go = "NarvalRing_4";
			TakeItemFromCharacter(pchar, "DOjeronRing");
		break;
		case "NarvalRing_3":
			dialog.text = "� ���� ��������, ��� ��� ���� ��������� �������� ������������ � �������� ����� ������?";
			link.l1 = "������...";
			link.l1.go = "NarvalRing_4";
		break;
		case "NarvalRing_4":
			dialog.text = "�-��, ���� ������� ���� ����������, ��� ��� ������... � �� ������ � '���������'?";
			link.l1 = "���, ���. ��� �� � ���� ������ �� �������. ���� ��� - ����� �������� ��� ����������.";
			link.l1.go = "NarvalRing_5";
		break;
		case "NarvalRing_5":
			dialog.text = "�� ��, ��� ������. �� ����� ���������� ��� ����� ������, ��� �����. ��� � ��� ���� ������. � �� ������, ��� ����������, ��� ������������ �����?";
			link.l1 = "��������, ����������. � ������������� ��������.";
			link.l1.go = "NarvalRing_6";
		break;
		case "NarvalRing_6":
			dialog.text = "��, ������... ���� � ���, ��� ������ � �������� �����, � ����� �������� ������ �������. ���� � ���������� ��� �� ����� � ����, ������� � ���� ������� ������� ������ ����� ����� ��� ������� � �������������. � ���� ������� ������������ ��� ����������� �����������, ��� � ���������� ����� '������' � '������'.";
			link.l1 = "�������... � ��� ������ ������? ���� ����������, ��� '�������' ��������� � �������� ������������ ������������� ������!";
			link.l1.go = "NarvalRing_7";
		break;
		case "NarvalRing_7":
			dialog.text = "���� ���������� �� ����� '������', ������� ��� ����� ���������������.";
			link.l1 = "���������� �������, �������!";
			link.l1.go = "NarvalRing_8";
		break;
		case "NarvalRing_8":
			dialog.text = "��� ��������! � � ������� ��� ����.";
			link.l1 = "��, ��� ���������?";
			link.l1.go = "NarvalRing_9";
		break;
		case "NarvalRing_9":
			dialog.text = "����������� '��������', ��������. ���� ��, ��� �� ����, ��������� ��� ����? ���� ��� ��������� ���� �� ������ ������!";
			link.l1 = "����! � ���-�� �� ����� ������� ������ �� �����������...";
			link.l1.go = "NarvalRing_10";
		break;
		case "NarvalRing_10":
			dialog.text = "����� ��������? �� �� ��������� ���, ��� ��� � �����?!";
			link.l1 = "�� �����, ������. � � ���� � ��� �������� ���������, �� �� ����������.";
			link.l1.go = "NarvalRing_11";
		break;
		case "NarvalRing_11":
			dialog.text = "�-�, �� ��� �� �����. � ��� ���� ����� ���� ����� � ������.";
			link.l1 = "��, ��� ������ ����. � ��������.";
			link.l1.go = "NarvalRing_12";
		break;
		case "NarvalRing_12":
			dialog.text = "�������! ��� ����������� �� '�������' ����� ������ � ��������� ��� ��� �������...";
			link.l1 = "������, ���, ��� ������ � ������ ����.";
			link.l1.go = "NarvalRing_13";
		break;
		case "NarvalRing_13":
			AddQuestRecord("ISS_MainLine", "12");
			pchar.questTemp.LSC = "toDestoyAllNarval";
			LocatorReloadEnterDisable("LostShipsCity_town", "reload54", true); //������� ��������, �� ������ ������
			int iQty = GetOfficersQuantity(pchar);
			int iNum = 1;
			int idxMent;
			while(iQty < 4)
			{
				idxMent = GetCharacterIndex("Ment_" + iNum);
				if (idxMent != -1)
				{
					//ChangeCharacterAddressGroup(sld, pchar.location, "reload", "reload5");
					SetOfficersIndex(pchar, -1, idxMent);
				}
				iNum++;
				iQty = GetOfficersQuantity(pchar);
				if (iNum > 6) break;
			}
			pchar.questTemp.LSC.qtyOfficers = iQty; //�������� ���-�� ��������, ��� �����
			LAi_group_SetCheck("EnemyFight", "NarvalDestroyed");
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
		//���� ������ ���������
		case "DestrNarval_1":
			dialog.text = "�� ��� ��, ��� ������. � ��� ��� ��� ����?";
			iTemp = GetPassengersQuantity(pchar);
			if (iTemp == 0)
			{
				link.l1 = "��� �������, � ���������. ��� ����� ����...";
				link.l1.go = "DestrNarval_Bad";
			}
			else
			{
				if (iTemp == sti(pchar.questTemp.LSC.qtyOfficers))
				{
					link.l1 = "�� ��� ���������. ��� ���� � �������, ��������� �� ���� � ������� � �����������.";
					link.l1.go = "DestrNarval_Ok";
				}
				else
				{
					link.l1 = "������ �������, �� ������� �� ���. ���������� � ����� ���������.";
					link.l1.go = "DestrNarval_BadOk";
				}
			}
		break;
		case "DestrNarval_Bad":
			dialog.text = "�-��, ��������... ������, ������ ���������, '�������' ����������, � ��� �������. ��� ��� ��� � �������.";
			link.l1 = "�� ��, ��� �����, ��� ���������...";
			link.l1.go = "DestrNarval_Bad1";
		break;
		case "DestrNarval_Bad1":
			dialog.text = "�� ��� ��, �������. � ���� ������ �� ����������, �� ������ ����.";
			link.l1 = "��... ��, �������, �������...";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "toTavernNarvalDestroyed";
			AddQuestRecord("ISS_MainLine", "16");
			for (i=0; i<=PASSENGERS_MAX; i++)
			{
				iTemp = GetPassenger(pchar, i);
				if(iTemp != -1)
				{
					sld = &characters[iTemp];
					RemovePassenger(pchar, sld);
					LAi_SetWarriorType(sld);
					LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
					SaveCurrentNpcQuestDateParam(sld, "location");
					ChangeCharacterAddressGroup(sld, "LostShipsCity_town", "foto",  sld.location.baseLocator);
				}
			}
		break;
		case "DestrNarval_Ok":
			dialog.text = "�������! �� ������ ��������� ������, �, �������, ������� ���� �������. ��� ����� �������! �������, ��� ����.";
			link.l1 = "��, ��� ����� ���� ������ �� �����, �� ��� ����� ������� ����.";
			link.l1.go = "DestrNarval_Ok1";
			AddQuestRecord("ISS_MainLine", "14");
			AddMoneyToCharacter(pchar, 100000);
		break;
		case "DestrNarval_Ok1":
			dialog.text = "�� �� ���! ��, � ���� ������ �� ����������, ��� ��� ������ ����� ���������� ����� ������.";
			link.l1 = "��... ������.";
			link.l1.go = "exit";
			pchar.questTemp.LSC = "toTavernNarvalDestroyed";
			for (i=0; i<=PASSENGERS_MAX; i++)
			{
				iTemp = GetPassenger(pchar, i);
				if(iTemp != -1)
				{
					sld = &characters[iTemp];
					RemovePassenger(pchar, sld);
					LAi_SetWarriorType(sld);
					LAi_group_MoveCharacter(sld, LAI_GROUP_PLAYER_OWN);
					SaveCurrentNpcQuestDateParam(sld, "location");
					ChangeCharacterAddressGroup(sld, "LostShipsCity_town", "foto",  sld.location.baseLocator);
				}
			}
		break;
		case "DestrNarval_BadOk":
			dialog.text = "�-��, ��������� ���������... �� �� �����, ���-���� ��� ���� �����, � �� ������ �������� �� ������. � �����, � ���, ��� ��� ��������� ������. ���� ��������� ���� ������� �� ������. ������ ����� �������!";
			link.l1 = "��, ������ ����� ����� �� �����... �� ��� ����� �������.";
			link.l1.go = "DestrNarval_Ok1";
			AddQuestRecord("ISS_MainLine", "15");
			AddMoneyToCharacter(pchar, 10000);
		break;
	}
}
