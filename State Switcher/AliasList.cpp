#include "stdafx.h"
#include "AliasList.h"

AliasList::AliasList()
{
}


AliasList::~AliasList()
{
}


void AliasList::ParseLine(const ArgScript::Line& line)
{
	vector<uint32_t> Aliases;
	PropManager.GetAllListIDs(id("entereditoraliases"), Aliases);
	for (int i = 0; i < Aliases.capacity(); i += 1)
	{
		PropertyListPtr propList;
		string16 AliasName;
		if (PropManager.GetPropertyList(Aliases[i], id("entereditoraliases"), propList)) {
			
			App::Property::GetString16(propList.get(), id("AliasName"), AliasName);
			string AliasName2;
			AliasName2.assign_convert(AliasName);
			App::ConsolePrintF(AliasName2.c_str());
		}
	}
}

const char* AliasList::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Lists all EnterEditor aliases.";
	}
	else {
		return "aliasList: Lists all aliases for use with enterEditor and editCreation.\n"
			"Includes both custom aliases and aliases included with enterEditor.";
	}
}
