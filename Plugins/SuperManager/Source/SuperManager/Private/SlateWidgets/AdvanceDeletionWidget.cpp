// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/AdvanceDeletionWidget.h"
#include "SlateBasics.h"

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
		.AutoHeight()
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
	const FString DisplayAssetName = AssetDataToDisplay->AssetName.ToString();
	
	TSharedRef< STableRow < TSharedPtr <FAssetData> > > ListViewRowWidget =
	SNew(STableRow < TSharedPtr <FAssetData> >,OwnerTable)
	[
		SNew(STextBlock)
		.Text(FText::FromString(DisplayAssetName))
	];

	return ListViewRowWidget;
}
