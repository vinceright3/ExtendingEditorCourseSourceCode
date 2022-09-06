// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

void UQuickAssetAction::DuplicateAssets(int32 NumOfDuplicates)
{
	if(NumOfDuplicates<=0)
	{
		ShowMsgDialog(EAppMsgType::Ok,TEXT("Please enter a VALID number"));
		return;
	}

	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 Counter = 0;

	for(const FAssetData& SelectedAssetData:SelectedAssetsData)
	{
		for(int32 i = 0; i<NumOfDuplicates; i++)
		{
			const FString SourceAssetPath = SelectedAssetData.ObjectPath.ToString();
			const FString NewDuplicatedAssetName = SelectedAssetData.AssetName.ToString() + TEXT("_") + FString::FromInt(i+1);
			const FString NewPathName = FPaths::Combine(SelectedAssetData.PackagePath.ToString(),NewDuplicatedAssetName);

			if(UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, NewPathName))
			{	
				UEditorAssetLibrary::SaveAsset(NewPathName,false);
				++Counter;
			}
		}
	}

	if(Counter>0)
	{	
		ShowNotifyInfo(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"));
		/*Print(TEXT("Successfully duplicated " + FString::FromInt(Counter) + " files"),FColor::Green);*/
	}
}

void UQuickAssetAction::AddPrefixes()
{
	TArray<UObject*>SelectedObjects = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 Counter = 0;

	for(UObject* SelectedObject:SelectedObjects)
	{
		if(!SelectedObject) continue;

		FString* PrefixFound = PrefixMap.Find(SelectedObject->GetClass());

		if(!PrefixFound||PrefixFound->IsEmpty())
		{
			Print(TEXT("Failed to find prefix for class ") + SelectedObject->GetClass()->GetName(),FColor::Red);
			continue;
		}

		FString OldName = SelectedObject->GetName();

		if(OldName.StartsWith(*PrefixFound))
		{
			Print(OldName + TEXT(" already has prefix added"),FColor::Red);
			continue;
		}

		if(SelectedObject->IsA<UMaterialInstanceConstant>())
		{
			OldName.RemoveFromStart(TEXT("M_"));
			OldName.RemoveFromEnd(TEXT("_Inst"));
		}

		const FString NewNameWithPrefix = *PrefixFound + OldName;

		UEditorUtilityLibrary::RenameAsset(SelectedObject,NewNameWithPrefix);

		++Counter;
	}

	if(Counter>0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed "+FString::FromInt(Counter)+" assets"));
	}
}

void UQuickAssetAction::RemoveUnusedAssets()
{
	TArray<FAssetData> SelectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> UnusedAssetsData;

	for(const FAssetData& SelectedAssetData:SelectedAssetsData)
	{	
		TArray<FString> AssetRefrencers =
		UEditorAssetLibrary::FindPackageReferencersForAsset(SelectedAssetData.ObjectPath.ToString());

		if(AssetRefrencers.Num()==0)
		{
			UnusedAssetsData.Add(SelectedAssetData);
		}
	}

	if(UnusedAssetsData.Num()==0)
	{
		ShowMsgDialog(EAppMsgType::Ok,TEXT("No unused asset found among selected assets"),false);
		return;
	}

	const int32 NumOfAssetsDeleted = ObjectTools::DeleteAssets(UnusedAssetsData);

	if(NumOfAssetsDeleted == 0) return;

	ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(NumOfAssetsDeleted) + TEXT(" unused assets")));
}