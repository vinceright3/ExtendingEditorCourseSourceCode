// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"
#include "DebugHeader.h"

void SAdvanceDeletionTab::Construct(const FArguments & InArgs)
{
	bCanSupportFocus = true;
	
	StoredAssetsData = InArgs._AssetsDataToStore;

	FSlateFontInfo TitleTextFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleTextFont.Size = 30;

	ChildSlot
	[	//Main vertical box
		SNew(SVerticalBox)

		//First vertical slot for title text
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Advance Deletion")))
			.Font(TitleTextFont)
			.Justification(ETextJustify::Center)
			.ColorAndOpacity(FColor::White)
		]

		//SecondSlot for drop down to specify the listing condition and help text
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]

		//Third slot for the asset list
		+SVerticalBox::Slot()
		.VAlign(VAlign_Fill)
		[
			SNew(SScrollBox)

			+SScrollBox::Slot()
			[
				SNew(SListView< TSharedPtr <FAssetData> >)
				.ItemHeight(24.f)
				.ListItemsSource(&StoredAssetsData)
				.OnGenerateRow(this,&SAdvanceDeletionTab::OnGenerateRowForList)
			]
		]

		//Fourth slot for 3 buttons
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
		]
	];
}

TSharedRef<ITableRow> SAdvanceDeletionTab::OnGenerateRowForList(TSharedPtr<FAssetData> AssetDataToDisplay, const TSharedRef<STableViewBase>& OwnerTable)
{	
	if(!AssetDataToDisplay.IsValid()) return SNew(STableRow < TSharedPtr <FAssetData> >,OwnerTable);

	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	
	TSharedRef< STableRow < TSharedPtr <FAssetData> > > ListViewRowWidget =
	SNew(STableRow < TSharedPtr <FAssetData> >,OwnerTable)
	[	
		SNew(SHorizontalBox)

		//First slot for check box
		+SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.FillWidth(.05f)
		[
			ConstructCheckBox(AssetDataToDisplay)
		]
				
		//Second slot for displaying asset class name

		//Third slot for displaying asset name
		+SHorizontalBox::Slot()
		[
			SNew(STextBlock)
			.Text(FText::FromString(DisplayAssetName))
		]
		
		//Fourth slot for a button

		
	];

	return ListViewRowWidget;
}

TSharedRef<SCheckBox> SAdvanceDeletionTab::ConstructCheckBox(const TSharedPtr<FAssetData>& AssetDataToDisplay)
{	
	TSharedRef<SCheckBox> ConstructedCheckBox = SNew(SCheckBox)
	.Type(ESlateCheckBoxType::CheckBox)
	.OnCheckStateChanged(this,&SAdvanceDeletionTab::OnCheckBoxStateChanged,AssetDataToDisplay)
	.Visibility(EVisibility::Visible);

	return ConstructedCheckBox;
}

void SAdvanceDeletionTab::OnCheckBoxStateChanged(ECheckBoxState NewState, TSharedPtr<FAssetData> AssetData)
{	
	switch(NewState)
	{
	case ECheckBoxState::Unchecked:

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is unchecked"), FColor::Red);

		break;

	case ECheckBoxState::Checked:

		DebugHeader::Print(AssetData->AssetName.ToString() + TEXT(" is checked"), FColor::Green);

		break;

	case ECheckBoxState::Undetermined:
		break;

	default:
		break;
	}
	
}