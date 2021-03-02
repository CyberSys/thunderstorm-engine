// ������ �� �������
void ProcessCommonDialogEvent(ref NPChar, aref Link, aref NextDiag)
{
    switch (Dialog.CurrentNode)
	{
		case "quests":
			dialog.text = NPCStringReactionRepeat(RandPhraseSimple("����� �������?", "��� ��� ������, " + GetAddress_Form(NPChar) + "?"), "������ ������� �� �������� ������ ��� ������ " + GetAddress_Form(NPChar) + "...", "� ������� ����� ��� �� ��� ������ ��� �������� � �����-�� �������...",
                          "����������, ���� �� �� �� ����� �����, �� �� ����� ���� ��������� ��������.", "block", 1, npchar, Dialog.CurrentNode);
			link.l1 = HeroStringReactionRepeat(RandPhraseSimple("� ���������...", "������ ��� �� � ��� ��������"), "��, ���-�� � ������� ���� �����...",
                      "�� ��, ������������� � ������ ���...", "��������, �� �������� ���� ���� ������ �� ����������.", npchar, Dialog.CurrentNode);
			link.l1.go = "exit";
            // ==> ����� �5 ����.�������, ������� ��� ������.
            if (pchar.questTemp.State == "ThreeFleutes_toPortMan")
            {
    			link.l1 = "� � ��� �� ��������� ����������. ��� ���������� ������� ������������ ��� ����� ��������.";
    			link.l1.go = "Step_H5_1";
            }
		break;

//************************************* ������� �� ����.������� *******************************
		case "Step_H5_1":
    		if (GetCompanionQuantity(pchar) == 1)
            {
                dialog.text = "����������, ����������. ��� ������ ��������� � ���� ������������.";
    			link.l1 = "�����������.";
    			link.l1.go = "Step_H5_2";
            }
            else
            {
                dialog.text = "��, � � ����� ������� �������. ������, ����� ������� ������������ ��� �������� ��� ���������� ����������� �� ����� �������. ����� ��� ������� ��� � ����� ����� ���������� �� ���.";
    			link.l1 = "������, � ��� � ������.";
    			link.l1.go = "exit";
            }
		break;
		case "Step_H5_2":
            pchar.questTemp.State = "ThreeFleutes_toLaVega";
            ref sld;
            for (int i=1; i<=3; i++)
            {
                sld = GetCharacter(NPC_GenerateCharacter("Captain_"+i, "trader_"+(rand(15)+1), "man", "man", 15, HOLLAND, -1, true));
                FantomMakeCoolSailor(sld, SHIP_FLEUT, "", CANNON_TYPE_CANNON_LBS16, 30, 15, 15);
                FantomMakeCoolFighter(sld, 15, 10, 10, BLADE_SHORT, "pistol1", 0);
                sld.Ship.Cargo.Goods.Balls = 18;
                sld.Ship.Cargo.Goods.Grapes = 18;
                sld.Ship.Cargo.Goods.Knippels = 18;
                sld.Ship.Cargo.Goods.Bombs = 18;
                sld.Ship.Cargo.Goods.Weapon = 0;
                sld.Ship.Cargo.Goods.Powder = 50;
				sld.CompanionEnemyEnable = false; 
				sld.Abordage.Enable = false; // ������!
				SetShipRemovable(sld, false);
				AddCharacterGoods(sld, GOOD_CLOTHES, 5000);
                AddCharacterGoods(sld, GOOD_FRUITS, 200);
                AddCharacterGoods(sld, GOOD_RUM, 900);
                SetCompanionIndex(pchar, -1, sti(sld.index));
            	pchar.GenQuest.StoreGoods.StoreIdx = LaVega_STORE;
            	pchar.GenQuest.StoreGoods.HL5_ThreeFleutes = true;
                Pchar.quest.ThreeFleutes_TalkIntoLaVega.win_condition.l1 = "location";
                Pchar.quest.ThreeFleutes_TalkIntoLaVega.win_condition.l1.location = "LaVega_town";
                Pchar.quest.ThreeFleutes_TalkIntoLaVega.win_condition = "ThreeFleutes_TalkIntoLaVega";
            }
            DialogExit();
		break;
	}
	UnloadSegment(NPChar.FileDialog2);  // ���� ���-�� ����� ������ switch  �� return �� ������ ������� �����
}

