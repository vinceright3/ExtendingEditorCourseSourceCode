// Copyright Vince Petrelli. Published in 2022.All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "QuickActorActionsWidget.generated.h"

UENUM(BlueprintType)
enum class E_DuplicationAxis : uint8
{
	EDA_XAxis UMETA (DisplayName = "X Axis"),
	EDA_YAxis UMETA (DisplayName = "Y Axis"),
	EDA_ZAxis UMETA (DisplayName = "Z Axis"),
	EDA_MAX UMETA (DisplayName = "Default Max")
};

USTRUCT(BlueprintType)
struct FRandomActorRotation
{	
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRandomizeRotYaw = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotYaw"))
	float RotYawMin = -45.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotYaw"))
	float RotYawMax = 45.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRandomizeRotPitch = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotPitch"))
	float RotPitchMin = -45.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotPitch"))
	float RotPitchMax = 45.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRandomizeRotRoll = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotRoll"))
	float RotRollMin = -45.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, meta = (EditCondition = "bRandomizeRotRoll"))
	float RotRollMax = 45.f;

};
/**
 * 
 */
UCLASS()
class SUPERMANAGER_API UQuickActorActionsWidget : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:

#pragma region ActorBatchSelection

	UFUNCTION(BlueprintCallable)
	void SelectAllActorsWithSimilarName();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchSelection")
	TEnumAsByte<ESearchCase::Type> SearchCase = ESearchCase::IgnoreCase;

#pragma endregion

#pragma region ActorBatchDuplication

	UFUNCTION(BlueprintCallable)
	void DuplicateActors();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	E_DuplicationAxis AxisForDuplication = E_DuplicationAxis::EDA_XAxis;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	int32 NumberOfDuplicates = 5;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "ActorBatchDuplication")
	float OffsetDist = 300.f;

#pragma endregion

#pragma region RandomizeActorTransform
	
	UFUNCTION(BlueprintCallable)
	void RandomizeActorTransform();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform")
	FRandomActorRotation RandomActorRotation;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform")
	bool bRandomizeScale = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform",meta = (EditCondition = "bRandomizeScale"))
	float ScaleMin = .8f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform",meta = (EditCondition = "bRandomizeScale"))
	float ScaleMax = 1.2f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform")
	bool bRandomizeOffset = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform",meta = (EditCondition = "bRandomizeOffset"))
	float OffsetMin = -50.f;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "RandomizeActorTransform",meta = (EditCondition = "bRandomizeOffset"))
	float OffsetMax = 50.f;

#pragma endregion

private:
	UPROPERTY()
	class UEditorActorSubsystem* EditorActorSubsystem;

	bool GetEditorActorSubsystem();
};
