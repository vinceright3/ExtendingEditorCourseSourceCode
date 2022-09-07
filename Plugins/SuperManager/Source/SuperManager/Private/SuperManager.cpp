// Copyright Epic Games, Inc. All Rights Reserved.

#include "SuperManager.h"
#include "ContentBrowserModule.h"
#include "DebugHeader.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "FSuperManagerModule"

void FSuperManagerModule::StartupModule()
{	
	InitCBMenuExtention();
}

#pragma region ContentBrowserMenuExtention

void FSuperManagerModule::InitCBMenuExtention()
{	
	FContentBrowserModule& ContentBrowserModule =
	FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& ContentBrowserModuleMenuExtenders =
	ContentBrowserModule.GetAllPathViewContextMenuExtenders();

	/*FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;
	CustomCBMenuDelegate.BindRaw(this,&FSuperManagerModule::CustomCBMenuExtender);

	ContentBrowserModuleMenuExtenders.Add(CustomCBMenuDelegate);*/

	ContentBrowserModuleMenuExtenders.Add(FContentBrowserMenuExtender_SelectedPaths::
	CreateRaw(this,&FSuperManagerModule::CustomCBMenuExtender));
}

TSharedRef<FExtender> FSuperManagerModule::CustomCBMenuExtender(const TArray<FString>& SelectedPaths)
{	
	TSharedRef<FExtender> MenuExtender (new FExtender());

	if(SelectedPaths.Num()>0)
	{
		MenuExtender->AddMenuExtension(FName("Delete"),
		EExtensionHook::After,
		TSharedPtr<FUICommandList>(),
		FMenuExtensionDelegate::CreateRaw(this,&FSuperManagerModule::AddCBMenuEntry));

		FolderPathsSelected = SelectedPaths;
	}

	return MenuExtender;
}

void FSuperManagerModule::AddCBMenuEntry(FMenuBuilder & MenuBuilder)
{
	MenuBuilder.AddMenuEntry
	(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Safely delete all unused assets under folder")),
		FSlateIcon(),
		FExecuteAction::CreateRaw(this,&FSuperManagerModule::OnDeleteUnsuedAssetButtonClicked)
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

	if(AssetsPathNames.Num()==0)
	{
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No asset found under selected folder"));
		return;
	}

	EAppReturnType::Type ConfirmResult =
	DebugHeader::ShowMsgDialog(EAppMsgType::YesNo,TEXT("A total of ") + FString::FromInt(AssetsPathNames.Num()) + TEXT(" found.\nWoudle you like to procceed?"));

	if(ConfirmResult == EAppReturnType::No) return;
	
	TArray<FAssetData> UnusedAssetsDataArray;

	for(const FString& AssetPathName:AssetsPathNames)
	{
		//Don't touch root folder
		if(AssetPathName.Contains(TEXT("Developers"))||
		AssetPathName.Contains(TEXT("Collections")))
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
		DebugHeader::ShowMsgDialog(EAppMsgType::Ok,TEXT("No unused asset found under selected folder"));
	}
}

#pragma endregion

void FSuperManagerModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSuperManagerModule, SuperManager)