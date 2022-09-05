// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"

void UQuickAssetAction::TestFunc()
{
	Print(TEXT("Working"),FColor::Cyan);
	PrintLog(TEXT("Working"));
}