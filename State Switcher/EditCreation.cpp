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
	// This method is called when your cheat is invoked.
	// Put your cheat code here.
	bool isAlias = line.HasFlag("alias");
	bool noAlias = line.HasFlag("noAlias");
	bool bSimulatorMode = false;
	if (GameModeManager.GetActiveModeID() == GameModeIDs::kEditorMode)
	{
		App::ConsolePrintF("Error; this cheat cannot be used from inside the editor.");
		App::ConsolePrintF("Return to the main menu to use this cheat.");
	}

	if (noAlias && isAlias)
	{
		App::ConsolePrintF("Error; Both alias and noAlias flags were set.");
		return;
	}

	auto args = line.GetArguments(1);
	uint32_t inputHash = id(args[0]);
	if (line.HasFlag("hash"))
	{
		inputHash = mpFormatParser->ParseUInt(args[0]);
	}


	if (!noAlias && !isAlias)
	{
		// check which to use.
		isAlias = PropManager.HasPropertyList(inputHash, id("entereditoraliases"));
		noAlias = PropManager.HasPropertyList(inputHash, GroupIDs::EditorConfig);

		if (isAlias && noAlias)
		{
			App::ConsolePrintF("There is both an alias and an editor under that name. \nPlease use the command with either -alias or -noAlias to use this name.");
			return;
		}

		if (!noAlias && !isAlias)
		{
			App::ConsolePrintF("There is no editor, or alias, under that name.");
			return;
		}
	}

	uint32_t editorID = inputHash;

	if (isAlias)
	{
		PropertyListPtr propList;

		if (PropManager.GetPropertyList(inputHash, id("entereditoraliases"), propList))
		{
			ResourceKey key;
			App::Property::GetKey(propList.get(), id("Alias"), key);
			editorID = key.instanceID;

			if (!PropManager.HasPropertyList(editorID, GroupIDs::EditorConfig))
			{
				App::ConsolePrintF("This alias is invalid; it points to an editor which does not exist.");
			}
		}
	}


	if (GameModeManager.GetActiveModeID() != GameModeIDs::kEditorMode)
	{
		bSimulatorMode = true;
	}


	mbIsSimulator = bSimulatorMode;
	mEditorID = editorID;

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
		Editors::EditorRequest* request = new Editors::EditorRequest();

		request->editorID = mEditorID;
		request->allowSporepedia = true;
		request->hasSaveButton = true;
		request->hasExitButton = true;
		request->sporepediaCanSwitch = true;
		request->hasPublishButton = false;
		request->hasCreateNewButton = true;

		request->creationKey = selection;

		if (mbIsSimulator)
		{
			Simulator::cGameModeManager::Get()->SubmitEditorRequest(request);
		}
		else
		{
			Editors::EditorRequest::Submit(request);
		}
	}
	
	else
	{
		App::ConsolePrintF("No valid creation selected.");
	}
}