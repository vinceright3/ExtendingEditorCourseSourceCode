// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickMaterialCreationWidget.generated.h"

/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickMaterialCreationWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()
	
public:

#pragma region QuickMaterialCreationCore
	
	UFUNCTION(BlueprintCallable)
	void CreateMaterialFromSelectedTextures();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "CreateMaterialFromSelectedTextures")
	FString MaterialName = TEXT("M_");

#pragma endregion


};
