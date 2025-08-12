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
	if (selection != ResourceKey(0, 0, 0))
	{

	}




	if (selection.instanceID != 0) {
		const char* carg = arg.c_str();
		uint32_t aliaseditor = 0;
		auto modemanager = Simulator::cGameModeManager::Get();

		bool canBeUsed = true;
		bool isSimulator = false;
		bool isAlias = false;
		PropertyListPtr propList;
		auto isValidEditor = PropManager.GetPropertyList(id(carg), 0x40600100, propList);
		ResourceKey key;

		if (isValidEditor == true)
		{
			canBeUsed = App::Property::GetKey(propList.get(), 0x00B2CCCB, key);  // checks if editor has parent
			if (canBeUsed == false) 
			{ 
				canBeUsed = App::Property::GetKey(propList.get(), 0x300DB745, key); 
			}
		}
		if (arg == "GlobalTemplate") { CanBeUsed = true; }
		bool IsGoodGameMode = true;

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
		auto gameMode = GameModeManager.GetActiveModeID();
		IsGoodGameMode = gameMode ==
		if (Game == kGameSpace) { IsSimulator = true; }
		if (IsValid || isAlias)
		{
			if (IsGoodGameMode || IsSimulator)
			{
				if (CanBeUsed)
				{
					EditorRequestPtr request = new Editors::EditorRequest();
					request->editorID = id(carg);
					if (isAlias == true) 
					{ 
						request->editorID = aliaseditor; 
					}

					request->allowSporepedia = true;
					request->hasSaveButton = true;
					request->hasExitButton = true;
					request->sporepediaCanSwitch = true;
					request->hasPublishButton = true;
					request->hasCreateNewButton = true;
					request->creationKey = selection;

					if (IsSimulator == false) 
					{ 
						Editors::EditorRequest::Submit(request.get()); 
					}
					else 
					{ 
						modemanager->SubmitEditorRequest(request.get()); 
					}
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