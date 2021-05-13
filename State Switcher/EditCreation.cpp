#include "stdafx.h"
#include "EditCreation.h"

EditCreation::EditCreation()
{
}


EditCreation::~EditCreation()
{
}


void EditCreation::ParseLine(const ArgScript::Line& line)
{
	auto args = line.GetArguments(1);
	line2 = line;
	arg = args[0];
	Sporepedia::ShopperRequest request(this);
	request.shopperID = id("noAdv");
	Sporepedia::ShopperRequest::Show(request);
}

const char* EditCreation::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat lets you enter any editor, using either its name found in .package files or an alias.";
	}
	else {
		return "\n"
			"Enters the editor corresponding to the given name, if a .prop file with the given name exists in the editor_setup~ folder.\n"
			"Only works with actual editors found in the editor_setup~ folder or the aliases. (see below)\n"
			"Mods can include their own aliases, though the mod developer needs to add them.\n"
			"This version aso brings the selected creation into the editor, allowing for any creation to be pulled into any editor.\n"
			"Aliases included with Enter Editor:\n"
			"\n"
			"'Creature' - Loads the creature editor.\n"
			"'Cell' - Loads the cell editor setup.\n"
			"'Building' - Loads the building editor setup.\n"
			"'Vehicle' - Loads the vehicle editor setup.\n"
			"'Flora' - Loads the flora editor setup.\n"
			"'Tribal' - Loads the tribal stage outfitter.\n"
			"'Civilian' - Loads the civilization stage outfitter.\n"
			"'Captain' - Loads the captain outfitter.\n"
			"\n"
			"Use the '-forceAlias' flag to use an alias if both an editor and an alias share the same name.\n"
			"Use the '-noAlias' flag to disable the use of aliases\n."
			"\n"
			"To see modded aliases, use the 'aliasList' cheat.";
	}
}

void EditCreation::OnShopperAccept(const ResourceKey& selection)
{
	if (selection.instanceID != 0) {
		const char* carg = arg.c_str();
		uint32_t aliaseditor = 0;
		auto modemanager = Simulator::cGameModeManager::Get();
		/*auto metadata = Pollinator::GetMetadata(selection.instanceID, selection.groupID);
		App::ConsolePrintF("You selected the creation %ls", metadata->GetName().c_str());*/
		bool CanBeUsed = true;
		bool IsSimulator = false;
		bool isAlias = false;
		PropertyListPtr propList;
		auto IsValid = PropManager.GetPropertyList(id(carg), 0x40600100, propList);
		ResourceKey key;

		if (IsValid == true)
		{
			CanBeUsed = App::Property::GetKey(propList.get(), 0x00B2CCCB, key);  // checks if editor has parent
			if (CanBeUsed == false) { CanBeUsed = App::Property::GetKey(propList.get(), 0x300DB745, key); }
		}
		if (arg == "GlobalTemplate") { CanBeUsed = true; }
		bool IsGoodGameMode = true;
//		if (arg == "Creature") { IsValid = true; }
//		if (arg == "Cell") { IsValid = true; }
//		if (arg == "Building") { IsValid = true; }
//		if (arg == "Vehicle") { IsValid = true; }
//		if (arg == "Flora") { IsValid = true; }
//		if (arg == "Tribal") { IsValid = true; }
//		if (arg == "Civilian") { IsValid = true; }
//		if (arg == "Captain") { IsValid = true; }

		if ((IsValid == false && !line2.HasFlag("noAlias")) || line2.HasFlag("forceAlias")) {
			if (PropManager.GetPropertyList(id(carg), id("entereditoraliases"), propList))
			{
				isAlias = true;
				App::Property::GetKey(propList.get(), id("Alias"), key);
				aliaseditor = key.instanceID;
			}
		}

		//GameModeManager.SetActiveModeByName(args[0]);
		//check if the game mode is either space or main menu
		auto Game = GameModeManager.GetActiveModeID();
		if (Game != kGGEMode) { IsGoodGameMode = false; }
		//	if (Game == kGameCell) { IsSimulator = true; }
		//	if (Game == kGameCreature) { IsSimulator = true; IsGoodGameMode = false; }
		//	if (Game == kGameTribe) { IsSimulator = true; IsGoodGameMode = false; }
		//	if (Game == kGameCiv) { IsSimulator = true; IsGoodGameMode = false; }
		if (Game == kGameSpace) { IsSimulator = true; }
		//	if (Simulator::IsScenarioMode()) { IsSimulator = true; }
		if (IsValid == true || isAlias == true)
		{
			if (IsGoodGameMode == true || IsSimulator == true)
			{
				if (CanBeUsed == true)
				{
					EditorRequestPtr request = new Editors::EditorRequest();
					if (isAlias == false) { request->editorID = id(carg); }
//					if (arg == "Creature") { request->editorID = id("CreatureEditorExtraLarge"); }
//					if (arg == "Cell") { request->editorID = id("CellTemplate"); }
//					if (arg == "Building") { request->editorID = id("BuildingTemplate"); }
//					if (arg == "Vehicle") { request->editorID = id("VehicleTemplate"); }
//					if (arg == "Flora") { request->editorID = id("FloraEditorSetup"); }
//					if (arg == "Tribal") { request->editorID = id("TribalAccessoriesEditorSetup"); }
//					if (arg == "Civilian") { request->editorID = id("CivAccessoriesEditorSetup"); }
//					if (arg == "Captain") { request->editorID = id("adventureraccessorieseditorsetup"); }
					if (isAlias == true) { request->editorID = aliaseditor; }
					request->allowSporepedia = true;
					request->hasSaveButton = true;
					request->hasExitButton = true;
					request->sporepediaCanSwitch = true;
					request->hasPublishButton = true;
					request->hasCreateNewButton = true;
					request->creationKey = selection;
					//modemanager->SubmitEditorRequest(request.get());
					if (IsSimulator == false) { Editors::EditorRequest::Submit(request.get()); }
					else { modemanager->SubmitEditorRequest(request.get()); }
					App::ConsolePrintF("Successfully entered editor.");
				}
				else
				{
					App::ConsolePrintF("That editor does technically exist but isn't an actual editor.");
				}
			}
			else
			{
				App::ConsolePrintF("Invalid gamemode. Use this command inside either the main menu or the space stage.");
			}
		}
		else
		{
			App::ConsolePrintF("Not a valid editor/alias.\n"
				"If you're using an alias, remember that they're case-sensitive.");
		}

	}
}