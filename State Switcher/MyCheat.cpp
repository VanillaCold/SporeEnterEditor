#include "stdafx.h"
#include "MyCheat.h"

MyCheat::MyCheat()
{
}


MyCheat::~MyCheat()
{
}


void MyCheat::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	auto modemanager = Simulator::cGameModeManager::Get(); //check current game-mode
	auto args = line.GetArguments(1); //get arguments
	//define variables
	PropertyListPtr propList;
	bool isAlias = false;
	uint32_t aliaseditor;
	bool IsSimulator = false;
	bool CanBeUsed = true;
	auto IsValid = PropManager.GetPropertyList(id(args[0]), 0x40600100, propList); //check if editor is valid, don't bother with aliases yet
	ResourceKey key; 

	if (IsValid == true)
	{
		auto CanBeUsed = App::Property::GetKey(propList.get(), 0x00B2CCCB, key);  // checks if editor has parent
		if (CanBeUsed == false) { CanBeUsed = App::Property::GetKey(propList.get(), 0x300DB745, key); } //if editor does not have a parent, check some other variable to make sure it's safe
	}
	if (args[0] == "GlobalTemplate") { CanBeUsed = true; } //check if the editor is just the global template.
	bool IsGoodGameMode = true;
	string strargs = string(args[0]);
	//if (strargs == "Creature") { IsValid = true; }
	//if (strargs == "Cell") { IsValid = true; }
	//if (strargs == "Building") { IsValid = true; }
	//if (strargs == "Vehicle") { IsValid = true; }
	//if (strargs == "Flora") { IsValid = true; }
	//if (strargs == "Tribal") { IsValid = true; }
	//if (strargs == "Civilian") { IsValid = true; }
	//if (strargs == "Captain") { IsValid = true; }

	if ((IsValid == false && !line.HasFlag("noAlias")) || line.HasFlag("forceAlias")) {
		if (PropManager.GetPropertyList(id(strargs.c_str()), id("entereditoraliases"), propList)) //account for aliases.
		{
			isAlias = true;
			App::Property::GetKey(propList.get(), id("Alias"), key); //get the new editor to visit
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
				if (isAlias == false) { request->editorID = id(args[0]); }
//				if (strargs == "Creature") { request->editorID = id("CreatureEditorExtraLarge"); }
//				if (strargs == "Cell") { request->editorID = id("CellTemplate"); }
//				if (strargs == "Building") { request->editorID = id("BuildingTemplate"); }
//				if (strargs == "Vehicle") { request->editorID = id("VehicleTemplate"); }
//				if (strargs == "Flora") { request->editorID = id("FloraEditorSetup"); }
//				if (strargs == "Tribal") { request->editorID = id("TribalAccessoriesEditorSetup"); }
//				if (strargs == "Civilian") { request->editorID = id("CivAccessoriesEditorSetup"); }
//				if (strargs == "Captain") { request->editorID = id("adventureraccessorieseditorsetup"); }
				if (isAlias == true) { request->editorID = aliaseditor; }
				request->allowSporepedia = true;
				request->hasSaveButton = true;
				request->hasExitButton = true;
				request->sporepediaCanSwitch = true;
				request->hasPublishButton = true;
				request->hasCreateNewButton = true;
				if (IsSimulator == true) { modemanager->SubmitEditorRequest(request.get()); }
				else { Editors::EditorRequest::Submit(request.get()); }
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
		"If you're using an alias from another mod, make sure you have said mod installed."); 
	}

}

const char* MyCheat::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat lets you enter any editor, using either its name found in .package files or an alias.";
	}
	else {
		return "\n"
		"Enters the editor corresponding to the given name, if a .prop file with the given name exists in the editor_setup~ folder.\n"
		"Only works with actual editors found in the editor_setup~ folder or the aliases. (see below)\n"
		"Mods can include their own aliases, though the mod developer needs to add them.\n"
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

void MyCheat::OnShopperAccept(const ResourceKey& selection)
{

	/*auto metadata = Pollinator::GetMetadata(selection.instanceID, selection.groupID);
	App::ConsolePrintF("You selected the creation %ls", metadata->GetName().c_str());*/

}