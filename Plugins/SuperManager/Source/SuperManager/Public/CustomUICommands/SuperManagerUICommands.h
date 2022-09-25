// Copyright Vince Petrelli. Published in 2022.All rights reserved.

#pragma once

#include "Framework/Commands/Commands.h"


class FSuperManagerUICommands : public TCommands<FSuperManagerUICommands>
{
public:
	FSuperManagerUICommands() : TCommands<FSuperManagerUICommands>(
	TEXT("SuperManager"),
	FText::FromString(TEXT("Super Manager UI Commands")),
	NAME_None,
	TEXT("SuperManager")
	) {}

	virtual void RegisterCommands() override;

	TSharedPtr<FUICommandInfo> LockActorSelection;
	TSharedPtr<FUICommandInfo> UnlockActorSelection;
};
