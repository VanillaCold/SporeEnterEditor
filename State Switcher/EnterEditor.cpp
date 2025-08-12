#include "stdafx.h"
#include "EnterEditor.h"

EnterEditor::EnterEditor()
{
}


EnterEditor::~EnterEditor()
{
}


void EnterEditor::ParseLine(const ArgScript::Line& line)
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
	

	if(!noAlias && !isAlias)
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


	Editors::EditorRequest* request = new Editors::EditorRequest();

	request->editorID = editorID;
	request->allowSporepedia = true;
	request->hasSaveButton = true;
	request->hasExitButton = true;
	request->sporepediaCanSwitch = true;
	request->hasPublishButton = false;
	request->hasCreateNewButton = true;

	if (bSimulatorMode)
	{
		Simulator::cGameModeManager::Get()->SubmitEditorRequest(request);
	}
	else
	{
		Editors::EditorRequest::Submit(request);
	}

}

const char* EnterEditor::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "This cheat does something.";
	}
	else {
		return "EnterEditor: Elaborate description of what this cheat does.";
	}
}
