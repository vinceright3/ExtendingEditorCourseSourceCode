// Copyright Vince Petrelli. Published in 2022.All rights reserved.


#include "CustomUICommands/SuperManagerUICommands.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerUICommands::RegisterCommands()
{
	UI_COMMAND(
	LockActorSelection,
	"Lock Actor Selection",
	"Lock actor selection in level, once triggered, actor can no longer be selected",
	EUserInterfaceActionType::Button,
	FInputChord(EKeys::W,EModifierKey::Alt)
	);

	UI_COMMAND(
	UnlockActorSelection,
	"Unlock Actor Selection",
	"Remove selection lock on all actors",
	EUserInterfaceActionType::Button,
	FInputChord(EKeys::W,EModifierKey::Alt | EModifierKey::Shift)
	);
}

#undef LOCTEXT_NAMESPACE
