// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorActions/QuickActorActionsWidget.h"
#include "Subsystems/EditorActorSubsystem.h"
#include "DebugHeader.h"

void UQuickActorActionsWidget::SelectAllActorsWithSimilarName()
{
	if(!GetEditorActorSubsystem()) return;

	TArray<AActor*> SelectedActors = EditorActorSubsystem->GetSelectedLevelActors();
	uint32 SelectionCounter = 0;

	if(SelectedActors.Num()==0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor selected"));
		return;
	}

	if(SelectedActors.Num()>1)
	{
		DebugHeader::ShowNotifyInfo(TEXT("You can only select one actor"));
		return;
	}

	FString SelectedActorName = SelectedActors[0]->GetActorLabel();
	const FString NameToSearch = SelectedActorName.LeftChop(4);

	TArray<AActor*> AllLeveActors = EditorActorSubsystem->GetAllLevelActors();

	for(AActor* ActorInLevel:AllLeveActors)
	{
		if(!ActorInLevel) continue;

		if(ActorInLevel->GetActorLabel().Contains(NameToSearch,SearchCase))
		{
			EditorActorSubsystem->SetActorSelectionState(ActorInLevel,true);
			SelectionCounter++;
		}
	}

	if(SelectionCounter>0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully selected ") + 
		FString::FromInt(SelectionCounter) + TEXT(" actors"));
	}
	else
	{
		DebugHeader::ShowNotifyInfo(TEXT("No actor with similar name found"));
	}
}

bool UQuickActorActionsWidget::GetEditorActorSubsystem()
{	
	if(!EditorActorSubsystem)
	{
		EditorActorSubsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	}
	
	return EditorActorSubsystem != nullptr;
}