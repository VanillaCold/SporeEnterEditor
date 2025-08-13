#pragma once

#include <Spore\BasicIncludes.h>

class EditCreation 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
{
public:
	EditCreation();
	~EditCreation();

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;

	void OnShopperAccept(const ResourceKey& selection) override;

	uint32_t mEditorID;
	bool mbIsSimulator;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;
};

