
void ProcessDialogEvent()
{
	ref NPChar;
	aref Link, NextDiag;

	DeleteAttribute(&Dialog,"Links");

	makeref(NPChar,CharacterRef);
	makearef(Link, Dialog.Links);
	makearef(NextDiag, NPChar.Dialog);

	int iQuest;

	switch(Dialog.CurrentNode)
	{
		case "First time":
			dialog.text = "�������������, � ���� ����� � ��� ������������.";
			link.l1 = "��� ��� " + pchar.name + "...";
			if (sti(pchar.nation) != PIRATE)
			{
				link.l1.go = "Second time";
			}
			else
			{
				link.l1.go = "you_are_pirate";
			}

			NextDiag.TempNode = "Second time";
		break;

		case "Second time":
			if (sti(pchar.nation) != PIRATE)
			{
				dialog.text = "��� ������?";
				link.l1 = "� ���� ���������� � ���� � �����.";
				link.l1.go = "quests";
				link.l2 = "�, � ��� �����.";
				link.l2.go = "exit";
			}
			else
			{
				dialog.text = "��� ��� ��� �����? � �� ��� ������ ��� �� ���������� ����� � ���� ��������!";
				link.l1 = "����, �� ��� ��� �� ��������� � �������. ��������.";
				link.l1.go = "exit";
			}

			NextDiag.TempNode = "Second time";
		break;

		case "you_are_pirate":
			dialog.text = "��, � ������ � ���, ������� �����. � ������ ���������� �����, � �� ���� ����� � ���� ������� ���.";
			link.l1 = "��� ��� ����� ������.";
			link.l1.go = "exit";
		break;

		case "quests":
			dialog.text = "����� ���� ��� ����������?";
			if(!CheckForNationService(sti(npchar.nation)))
			{
				link.l1 = "� ���� ��������� �� ��������������� ������.";
				link.l1.go = "service";
			}
			else
			{
				link.l1 = "���� ���������� ���������� � ��������.";
				link.l1.go = "quests_completed";
			}
			link.l99 = "�������.";
			link.l99.go = "exit";
		break;

		case "quests_completed":
			dialog.text = "��� ������, �� ������ ������?";
			link.l1 = "��� �� � ��� ������-���� ������� ��� ����?";
			link.l1.go = "ship_mission";

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� �������� ������������
			///////////////////////////////////////////////////////////////
			if(CheckAttribute(npchar, "quest.disease"))
			{
				if(npchar.quest.disease == "inprogress" && GetSquadronGoods(pchar, GOOD_MEDICAMENT) >= 10)
				{
					link.l1 = "� ����� ��� ����� ��������� ����������� ��� �����������";
					link.l1.go = "disease_mission_completed";
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ��������� ��������
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.stealitem") && CheckQuestAttribute("steal_item", "stealed"))
			{
				if (CheckCharacterItem(pchar, npchar.quest.stealitem.item))
				{
					link.l1 = "���� ������� ����������.";
					link.l1.go = "steal_item_completed";
				}
				else
				{
					link.l1 = "���, ��� �� ������� ��������� ���� �������.";
					link.l1.go = "steal_item_failed";
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ��������
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.killcharacter"))
			{
				if (CheckCharacterItem(pchar, npchar.quest.killcharacter.item))
				{
					link.l1 = "���� ������� ����������.";
					if (npchar.quest.sinkship.condition == "completed" || sti(pchar.skill.luck) > rand(13))
					{
						link.l1.go = "kill_character_mission_complete";
					}
					else
					{
						link.l1.go = "kill_character_mission_failed";
					}
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ���������� �������
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.sinkship"))
			{
				if (npchar.quest.sinkship.condition == "completed")
				{
					link.l1 = "���� ������� ����������.";
					link.l1.go = "sink_ship_mission_complete";
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ������ �����
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.fort"))
			{
				if (sti(Islands[findisland(npchar.quest.fort)].nation) == sti(npchar.nation))
				{
					link.l1 = "������ " + GlobalStringConvert(npchar.quest.fort) + " ��������, � ��� ��� ������� ����������� ����������� �����.";
					link.l1.go = "fort_mission_complete";
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ������ �������
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.captureship"))
			{
				if (npchar.quest.captureship.condition == "completed")
				{
					link.l1 = "���� ������� ����������.";
					link.l1.go = "ship_mission_complete";
				}
				if (npchar.quest.captureship.condition == "failed")
				{
					link.l1 = "���, ��� �� ������� ������� ��� ������, ��� ����� ���������� ����� �� ���...";
					link.l1.go = "ship_mission_not_complete";
				}
			}

			///////////////////////////////////////////////////////////////
			/// ����� ������ �� ������ �������
			///////////////////////////////////////////////////////////////
			if (CheckAttribute(npchar, "quest.defendcolony"))
			{
				if (sti(Islands[findisland(npchar.quest.defendcolony.colony)].nation) == sti(npchar.nation) && pchar.quest.defendcolony_progress == "completed")
				{
					dialog.text = "��� ������� � ����� ������! ��������� ���������� ��� � ������������ �������!";
					link.l1 = "��������� ���.";
					link.l1.go = "exit";
					FortDefendMissionCompleted(sti(npchar.nation), npchar);
				}
				if (sti(Islands[findisland(npchar.quest.defendcolony.colony)].nation) != sti(npchar.nation) && pchar.quest.defendcolony.progress == "completed")
				{
					dialog.text = "�� �� ���������� � ����������� �������, � ���� ������� � ���������� ������� �������! � ������ �������� ����, ��� ���������� ��������� ��������.";
					link.l1 = "��� ��� ����� ������. ��, ������. ������ � �� ������� ���.";
					link.l1.go = "exit";
					FortDefendMissionNotCompleted(sti(npchar.nation), npchar);
				}
			}
			link.l99 = "������, ����, �����������, � ���������� ���� ����������� �����������.";
			link.l99.go = "exit";
		break;

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
			link.l1.go = "exit";
			PlayerChangeNation(sti(npchar.nation));
		break;

		case "service_line_quests":
			if(CheckQuestAttribute("work", "0"))
			{
			}
			else
			{
				dialog.text = "� ��� ��� ��� �������, �, ���������� �� ��������, �� ��� ��� �� ���������.";
				link.l1 = "��... �������������.";
				link.l1.go = "exit";
			}
		break;

		case "generate_quest":
			if(CheckQuestAttribute("work", "0"))
			{
				dialog.text = "���, ���, ����� ��������, ����� ���� � ��� �� ������� �������������...";
				link.l1 = "...";
				iQuest = rand(3)+1;
				switch (iQuest)
				{
					case 1:
						link.l1.go = "ship_mission";
					break;

					case 2:
						link.l1.go = "ship_sink_mission";
					break;

					case 3:
						link.l1.go = "fort_mission";
					break;

					case 4:
						link.l1.go = "delivery_passenger";
					break;

				}
				if (CheckForDisease())
				{
					link.l1.go = "disease_mission";
				}
				if(CheckForDefendColony(npchar))
				{
					link.l1.go = "defend_colony_mission";
				}
			}
			else
			{
				dialog.text = "� ��� ��� ��� �������, �, ��������� ��� ��������, �� ��� ��� �� ���������.";
				link.l1 = "��... �������������.";
				link.l1.go = "exit";
			}
		break;

		case "fort_mission":
			if(GenerateFortQuest(sti(npchar.nation), npchar))
			{
				dialog.text = "�� �������� ���� ���������� �������, � ������� � ���� �������� ��� �����, ������������� ��������� ���.";
				link.l1 = "� ����������� ������.";
				link.l1.go = "fort_mission_2";
			}
			else
			{
				dialog.text = "���... � ��� ��� ����... � ������ � ������.";
				link.l1 = "���? �����?!";
				link.l1.go = "exit";
			}
		break;

		case "fort_mission_2":
			dialog.text = "� ����, ����� �� ��������� ������ " + GlobalStringConvert(npchar.quest.fort) + ", � ������� ��� ��� ����������� �����.";
			link.l1 = "�������������, ������� �� �� ������, �� � ���������.";
			link.l1.go = "exit";
		break;

		case "fort_mission_complete":
			dialog.text = "�����! �� ������, ��� � ��� ������� �������� ��� �� �����. �� ��� ���� �������, �, ��� ���������� ��� � ������������ ���������� ������.";
			link.l1 = "��������� ���.";
			link.l1.go = "exit";
			ShipCaptureFortCompleted(sti(npchar.nation), npchar);
		break;

		case "ship_mission":
			iQuest = rand(1);
			if(GenerateCaptureShipQuest(sti(npchar.nation), npchar, iQuest))
			{
				switch(iQuest)
				{
					case 0:
						dialog.text = "���� � ���, ��� � ���� ���� �������� ��������� ������, � � ����������, ��� ��� ������ ����� " + npchar.quest.captureship + "\n���� ������ ������� ��� ��� ������. � ��������� ��� ����� ���������� ������ � ������� " + GlobalStringConvert(npchar.quest.captureship.island) + ".";
					break;

					case 1:
						dialog.text = "��� ����� ��������, ��� ����� " + npchar.quest.captureship + " ����� �� " + XI_ConvertString("From " + GetNationNameByType(sti(npchar.quest.captureship.nation)))+ " �� " + GlobalStringConvert(npchar.quest.captureship.island) + " ������ ���������. ���� ������ ���������� ��� �����, � ��������� ������.";
					break;
				}

				link.l1 = "� ������� ��� ������, ��� �����.";
				link.l1.go = "exit";
				link.l2 = "��������, � ������ � �� ���� ������� �� ��� ����.";
				link.l2.go = "exit";
			}
			else
			{
				dialog.text = "���... � ��� ��� ����... � ������ � �������� �����.";
				link.l1 = "���? �����?!";
				link.l1.go = "exit";
			}
		break;

		case "ship_mission_complete":
			dialog.text = "�����! �� ������������ ��� ��������. �� �������� �� ���� � ����: ��� ���� �������, � ������ ��������� ���������� ��� � ������������ ���������� �� ������.";
			link.l1 = "��������� ���.";
			link.l1.go = "exit";
			ShipCaptureMissionCompleted(sti(npchar.nation), npchar);
		break;

		case "ship_mission_not_complete":
			dialog.text = "���������� ������� ���, ���� � �������� � ���� ������������ �������. � ������ �������� ����.";
			link.l1 = "��� ��� ����� ������. �������, ��������� ��������� � ������� � ������.";
			link.l1.go = "exit";
			ShipCaptureMissionFailed(npchar);
		break;

		case "defend_colony_mission":
			iQuest = rand(3);
			if (iQuest<3)
			{
				dialog.text = "��� �� �������! ��� ������ ��� ��������, ��� " + XI_ConvertString(GetNationNameByType(sti(npchar.quest.defendcolony.nation))) + " ������ �� ����������� ������� " + XI_ConvertString(npchar.quest.defendcolony.colony) + ". � ���������, � ���� ��� ������ ��������� ������� ��������, ��� ��� ������ ������� ������� �� ���� �����.";
				pchar.quest.defencolony.type = "single";
			}
			else
			{
				dialog.text = "�� �������! �� ��� ��� ��������� � �������� �����. ���� � ���, ��� "  + XI_ConvertString(GetNationNameByType(sti(npchar.quest.defendcolony.nation))) + " ������ �� ����������� ������� " + XI_ConvertString(npchar.quest.defendcolony.colony) + ", � � ��������� �������� ��� � ������ ����������.";
				pchar.quest.defencolony.type = "multi";
			}
			link.l1 = "� �������, ����� ����� ����, ��� ������� ����� �����������.";
			link.l1.go = "defend_colony_mission_accept";
			link.l2 = "���, ��� ������� ��� �� �� �����.";
			link.l2.go = "defend_colony_mission_refuse";
		break;

		case "defend_colony_mission_accept":
			dialog.text = "�������! � �� ����������� ��� �������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			DefendColonyMissionAccept(npchar);
		break;

		case "defend_colony_mission_refuse":
			dialog.text = "����� ����.";
			link.l1 = "��������, ��� ����.";
			link.l1.go = "exit";
			DefendColonyMissionRefuse(npchar);
		break;

		case "ship_sink_mission":
			if (GenerateShipSinkQuest(sti(npchar.nation), npchar))
			{
				dialog.text = "� ��������� ����� ��� ������ � ������� ������������ ����� ������������� �������� " + npchar.quest.sinkship + ", ������������ ������� � ����� �����, ����������� ������ " + XI_ConvertString("From " + GetNationNameByType(sti(npchar.quest.sinkship.nation)))+ ". ��� �� ��������� ������ ��� � ������� ������ �� ����������� ����� �������, ������� ��� ��� ���������� �� " + GlobalStringConvert(npchar.quest.sinkship.island) + " ����� ����� ������������.";
				link.l1 = "����� ����������.";
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "���... � ��� ��� ����... � ������ � ����������� �������.";
				link.l1 = "���? �����?!";
				link.l1.go = "exit";
			}
		break;

		case "sink_ship_mission_complete":
			dialog.text = "�����! �� ������������ ��� ��������. �� �������� �� ���� � ����: ��� ���� �������, � ������ ��������� ���������� ��� � ������������ ���������� �� ������.";
			link.l1 = "��������� ���.";
			link.l1.go = "exit";
			ShipSinkMissionCompleted(sti(npchar.nation), npchar);
		break;

		case "kill_enemy":
			if (!CheckAttribute(pchar, "quest.killcharacter"))
			{
				if (GenerateKillQuest(sti(npchar.nation), npchar, 1))
				{
					dialog.text = "� ���� ��� �������� ��������� ���������� ����. ��������� ��, ��� �����, ����� ����� " + GetCharacterFullName(npchar.quest.killcharacter) + ", ����� ��������... �� ������ ������ ���� ������� ��������� �� ������� " + GlobalStringConvert(npchar.quest.killcharacter.island) + " � � ����, ����� �� ������� ��� �����, � ��������, � ���� �������������� ��� ������.";
					link.l1 = "����� ����������.";
					link.l1.go = "exit";
				}
				else
				{
					dialog.text = "���... � ��� ��� ����... � ������ � ���������.";
					link.l1 = "���? �����?!";
					link.l1.go = "exit";
				}
			}
			else
			{
				dialog.text = "� ������ ������ ����� ��� ��� ��� �������.";
				link.l1 = "��.";
				link.l1.go = "exit";
			}
		break;

		case "kill_character_mission_complete":
			dialog.text = "����, �� ���������� � ��������� ��� �������. ������� ��� ������������, ������������ ������ � ��� �������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			KillCharacterMissionCompleted(sti(npchar.nation), npchar);
		break;

		case "kill_character_mission_failed":
			dialog.text = "� ������, ������ ��� ���, ��, �� ����� ������������ � �����, ��� ������, �������� ��� ���� ��������� ����� ��� ��� ���. ����� � �����������.";
			link.l1 = "��������, ������ ������ �� ����������.";
			link.l1.go = "exit";
			KillCharacterMissionFailed(npchar);
		break;

		case "steal_item":
			if (GenerateStealItem(sti(npchar.nation), npchar, 0))
			{
				dialog.text = "��� ����������, ����� �� �������� ������, ������� ��������� � ����������� ������� " + XI_COnvertString(npchar.quest.stealitem.island) + ".";
				link.l1 = "����� �������.";
				link.l1.go = "exit";
			}
			else
			{
				dialog.text = "����. � ��� ��� ���� � ������ ��� ��������� �����.";
				link.l1 = "������.";
				link.l1.go = "exit";
			}
		break;

		case "steal_item_completed":
			dialog.text = "�������! �� ������� ������� � ���� ������. ��� ���� �������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			StealItemMissionCompleted(sti(npchar.nation), npchar, 0);
		break;

		case "steal_item_failed":
			dialog.text = "�������! �� ������� ������� � ���� ������. ��� ���� �������.";
			link.l1 = "�������.";
			link.l1.go = "exit";
			StealItemMissionFailed(npchar);
		break;

		case "disease_mission":
			dialog.text = "��, ���������, ��� ������ � ��� ��� � ����� ������� ������ �������...";
			link.l1 = "��� �� ��� ���� �� ������ - ��������� ����� �� ������ ������������� ����� ����.";
			link.l1.go = "disease_mission_2";
		break;

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
			link.l1.go = "��������.";
			link.l1.go = "exit";
		break;

		case "disease_mission_completed":
			if (CheckForDisease())
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

		case "delivery_passenger":
			if (GeneratePassengerQuest(npchar))
			{
				dialog.text = "�� ���������� �� �� ����������� �������� �� ����� " + npchar.quest.deliverypassenger.passenger + " �� ������� " + XI_ConvertString(npchar.quest.deliverypassenger.endcolony) + "\n�� �������� �� ����� �� �������� ����� � ���������� " + npchar.quest.deliverypassenger.money  + " ��������.";
				link.l1 = "�� ����� ������� ���������, ��� ��� � �������� �� ���.";
				link.l1.go = "delivery_passenger_accept";
				link.l2 = "���� ��� �� ����������.";
				link.l2.go = "delivery_passenger_refuse";
			}
			else
			{
				dialog.text = "� ��� ��� ���� ��� ������� �������� ������ �� �������� ���������.";
				link.l1 = "������, ��� � ���� �������.";
				link.l1.go = "exit";
			}
		break;

		case "delivery_passenger_accept":
			dialog.text = "�� ������� ��� ������� ������. �������������� �� �������� �� �������� �� �����.";
			link.l1 = "������������.";
			link.l1.go = "exit";
			AcceptPassengerQuest(npchar);
		break;

		case "delivery_passenger_refuse":
			dialog.text = "����� ����, �������� ��� ������ ����-������ �������.";
			link.l1 = "�� �������.";
			link.l1.go = "exit";
			RefusePassengerQuest(npchar);
		break;

		case "Exit":
			NextDiag.CurrentNode = NextDiag.TempNode;
			DialogExit();
		break;
	}
}
