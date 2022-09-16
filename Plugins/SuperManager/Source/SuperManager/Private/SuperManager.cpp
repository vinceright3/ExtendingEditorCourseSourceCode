// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "SlateWidgets/AdvanceDeletionWidget.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{	
	InitCBMenuExtention();

	RegisterAdvanceDeletionTab();
}

#pragma region ContentBrowserMenuExtention

void FSuperManagerModule::InitCBMenuExtention()
{	
	FContentBrowserModule& ContentBrowserModule =
	FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	//Get hold of all the menu extenders
	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	/*FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this,&FSuperManagerModule::CustomCBMenuExtender);

	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);*/

	//We add custom delete to all the existing delegate
	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::
	CreateRaw(this,&FSuperManagerModule::CustomCBMenuExtender));
}

//To define the positio for inserting menu entry
TSharedRef<FExtender> FSuperManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{	
	TSharedRef<FExtender> MenuExtender (new FExtender());

	if(SelectedPaths.Num()>0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"), //Extend hook, position to insert
		EExtensionHook::After, //Insert before or after
		TSharedPtr<FUICommandList>(), //Custom hot keys 
		FMenuExtensionDelegate::CreateRaw(this,&FSuperManagerModule::AddCBMenuEntry)); //Second binding, will define details for this menu entry
		  
		FolderPathsSelected = SelectedPaths;
	}

	return MenuExtender;
}

//Define details for the custom menu entry
void FSuperManagerModule::AddCBMenuEntry(FMenuBuilder & MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unused Assets")), //Title text for menu entry
		FText::FromString(TEXT("Safely delete all unused assets under folder")), //Tooltip text
		FSlateIcon(),	//Custom icon
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnDeleteUnsuedAssetButtonClicked) //The actual function to excute
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Empty Folders")), //Title text for menu entry
		FText::FromString(TEXT("Safely delete all empty folders")), //Tooltip text
		FSlateIcon(),	//Custom icon
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked) //The actual function to excute
	);

	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Advance Deletion")), //Title text for menu entry
		FText::FromString(TEXT("List assets by specific condition in a tab for deleting")), //Tooltip text
		FSlateIcon(),	//Custom icon
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnAdvanceDeletionButtonClicked) //The actual function to excute
	);
}

void FSuperManagerModule::OnDeleteUnsuedAssetButtonClicked()
{
	if(FolderPathsSelected.Num()>1)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("You can only do this to one folder"));
		return;
	}

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);

	//Whether there are assets under selected folder
	if(AssetsPathNames.Num()==0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No asset found under selected folder"),false);
		return;
	}

	EAppReturnType::Type ConfirmResult =
	DebugHeader::ShowMsgDialog(EAppMsgType::YesNo,TEXT("A total of ") + FString::FromInt(AssetsPathNames.Num()) 
	+ TEXT(" assets need to be checked.\nWould you like to procceed?"),false);

	if(ConfirmResult == EAppReturnType::No) return;
	
	FixUpRedirectors();

	TArray<FAssetData> UnusedAssetsDataArray;

	for(const FString& AssetPathName:AssetsPathNames)
	{
		//Don't touch root folder
		if(AssetPathName.Contains(TEXT("Developers"))||
		AssetPathName.Contains(TEXT("Collections")) ||
		AssetPathName.Contains(TEXT("__ExternalActors__")) ||
		AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if(!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		TArray<FString> AssetReferencers =
		UEditorAssetLibrary::FindPackageReferencersForAsset(AssetPathName);

		if(AssetReferencers.Num()==0)
		{
			const FAssetData UnusedAssetData = UEditorAssetLibrary::FindAssetData(AssetPathName);
			UnusedAssetsDataArray.Add(UnusedAssetData);
		}
	}

	if(UnusedAssetsDataArray.Num()>0)
	{
		ObjectTools::DeleteAssets(UnusedAssetsDataArray);
	}
	else
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No unused asset found under selected folder"),false);
	}
}

void FSuperManagerModule::OnDeleteEmptyFoldersButtonClicked()
{	
	FixUpRedirectors();

	TArray<FString> FolderPathsArray = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0],true,true);
	uint32 Counter = 0;

	FString EmptyFolderPathsNames;
	TArray<FString> EmptyFoldersPathsArray;

	for(const FString& FolderPath:FolderPathsArray)
	{
		if(FolderPath.Contains(TEXT("Developers"))||
		FolderPath.Contains(TEXT("Collections")) ||
		FolderPath.Contains(TEXT("__ExternalActors__")) ||
		FolderPath.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if(!UEditorAssetLibrary::DoesDirectoryExist(FolderPath)) continue;

		if(!UEditorAssetLibrary::DoesDirectoryHaveAssets(FolderPath))
		{
			EmptyFolderPathsNames.Append(FolderPath);
			EmptyFolderPathsNames.Append(TEXT("\n"));

			EmptyFoldersPathsArray.Add(FolderPath);
		}		
	}

	if(EmptyFoldersPathsArray.Num()==0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No empty folder found under selected folder"),false);
		return;
	}

	EAppReturnType::Type ConfirmResult = DebugHeader::ShowMsgDialog(EAppMsgType::OkCancel,
	TEXT("Empty folders found in:\n") + EmptyFolderPathsNames +TEXT("\nWould you like to delete all?"),false);

	if(ConfirmResult==EAppReturnType::Cancel) return;
	
	for(const FString& EmptyFolderPath:EmptyFoldersPathsArray)
	{
		UEditorAssetLibrary::DeleteDirectory(EmptyFolderPath)?
		++Counter : DebugHeader::Print(TEXT("Failed to delete " + EmptyFolderPath),FColor::Red);
	}

	if(Counter>0)
	{
		DebugHeader::ShowNotifyInfo(TEXT("Successfully deleted ") + FString::FromInt(Counter) + TEXT(" folders"));
	}

}

void FSuperManagerModule::OnAdvanceDeletionButtonClicked()
{	
	FixUpRedirectors();

	FGlobalTabmanager::Get()->TryInvokeTab(FName("AdvanceDeletion"));
}

void FSuperManagerModule::FixUpRedirectors()
{
	TArray<UObjectRedirector*> RedirectorsToFixArray;

	FAssetRegistryModule& AssetRegistryModule =
	FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassNames.Emplace("ObjectRedirector");

	TArray<FAssetData> OutRedirectors;
	AssetRegistryModule.Get().GetAssets(Filter,OutRedirectors);

	for(const FAssetData& RedirectorData:OutRedirectors)
	{
		if(UObjectRedirector* RedirectorToFix = Cast<UObjectRedirector>(RedirectorData.GetAsset()))
		{
			RedirectorsToFixArray.Add(RedirectorToFix);
		}
	}

	FAssetToolsModule& AssetToolsModule =
	FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetTools"));

	AssetToolsModule.Get().FixupReferencers(RedirectorsToFixArray);
}

#pragma endregion

#pragma region CustomEditorTab
	
void FSuperManagerModule::RegisterAdvanceDeletionTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(FName("AdvanceDeletion"),
	FOnSpawnTab::CreateRaw(this,&FSuperManagerModule::OnSpawnAdvanceDeltionTab))
	.SetDisplayName(FText::FromString(TEXT("Advance Deletion")));
}

TSharedRef<SDockTab> FSuperManagerModule::OnSpawnAdvanceDeltionTab(const FSpawnTabArgs &SpawnTabArgs)
{	
	return
	SNew(SDockTab).TabRole(ETabRole::NomadTab)
	[
		SNew(SAdvanceDeletionTab)
		.AssetsDataToStore(GetAllAssetDataUnderSelectedFolder())
		.CurrentSelectedFolder(FolderPathsSelected[0])
	];
}

TArray<TSharedPtr<FAssetData>> FSuperManagerModule::GetAllAssetDataUnderSelectedFolder()
{
	TArray< TSharedPtr <FAssetData> > AvaiableAssetsData;

	TArray<FString> AssetsPathNames = UEditorAssetLibrary::ListAssets(FolderPathsSelected[0]);

	for(const FString& AssetPathName:AssetsPathNames)
	{
		//Don't touch root folder
		if(AssetPathName.Contains(TEXT("Developers"))||
		AssetPathName.Contains(TEXT("Collections")) ||
		AssetPathName.Contains(TEXT("__ExternalActors__")) ||
		AssetPathName.Contains(TEXT("__ExternalObjects__")))
		{
			continue;
		}

		if(!UEditorAssetLibrary::DoesAssetExist(AssetPathName)) continue;

		const FAssetData Data = UEditorAssetLibrary::FindAssetData(AssetPathName);

		AvaiableAssetsData.Add(MakeShared<FAssetData>(Data));
	}

	return AvaiableAssetsData;
}

#pragma endregion

#pragma region ProccessDataForAdvanceDeletionTab

bool FSuperManagerModule::DeleteSingleAssetForAssetList(const FAssetData & AssetDataToDelete)
{	
	TArray<FAssetData> AssetDataForDeletion;
	AssetDataForDeletion.Add(AssetDataToDelete);

	if(ObjectTools::DeleteAssets(AssetDataForDeletion)>0)
	{
		return true;
	}

	return false;
}

bool FSuperManagerModule::DeleteMultipleAssetsForAssetList(const TArray<FAssetData>& AssetsToDelete)
{
	if(ObjectTools::DeleteAssets(AssetsToDelete)>0)
	{
		return true;
	}

	return false;
}

void FSuperManagerModule::ListUnusedAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, 
TArray<TSharedPtr<FAssetData>>& OutUnusedAssetsData)
{
	OutUnusedAssetsData.Empty();

	for(const TSharedPtr<FAssetData>& DataSharedPtr:AssetsDataToFilter)
	{	
		TArray<FString> AssetReferencers =
		UEditorAssetLibrary::FindPackageReferencersForAsset(DataSharedPtr->ObjectPath.ToString());

		if(AssetReferencers.Num()==0)
		{
			OutUnusedAssetsData.Add(DataSharedPtr);
		}
	}
}

void FSuperManagerModule::ListSameNameAssetsForAssetList(const TArray<TSharedPtr<FAssetData>>& AssetsDataToFilter, 
TArray<TSharedPtr<FAssetData>>& OutSameNameAssetsData)
{
	OutSameNameAssetsData.Empty();

	//Multimap for supporting finding assets with same name
	TMultiMap<FString, TSharedPtr<FAssetData> > AssetsInfoMultiMap;

	for(const TSharedPtr<FAssetData>& DataSharedPtr:AssetsDataToFilter)
	{
		AssetsInfoMultiMap.Emplace(DataSharedPtr->AssetName.ToString(), DataSharedPtr);
	}

	for(const TSharedPtr<FAssetData>& DataSharedPtr:AssetsDataToFilter)
	{	
		TArray< TSharedPtr <FAssetData> > OutAssetsData;
		AssetsInfoMultiMap.MultiFind(DataSharedPtr->AssetName.ToString(), OutAssetsData);

		if(OutAssetsData.Num()<=1) continue;
		
		for(const TSharedPtr<FAssetData>& SameNameData:OutAssetsData)
		{
			if(SameNameData.IsValid())
			{
				OutSameNameAssetsData.AddUnique(SameNameData);
			}
		}
	}
}

void FSuperManagerModule::SyncCBToClickedAssetForAssetList(const FString & AssetPathToSync)
{
	TArray<FString> AssetsPathToSync;
	AssetsPathToSync.Add(AssetPathToSync);

	UEditorAssetLibrary::SyncBrowserToObjects(AssetsPathToSync);
}
#pragma endregion

void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)