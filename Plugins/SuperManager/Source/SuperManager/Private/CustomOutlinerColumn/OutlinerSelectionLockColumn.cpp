// Vince Petrelli All Rights Reserverd


#include "CustomOutlinerColumn/OutlinerSelectionLockColumn.h"
#include "CustomStyle/SuperManagerStyle.h"

SHeaderRow::FColumn::FArguments FOutlinerSelectionLockColumn::ConstructHeaderRowColumn()
{	
	SHeaderRow::FColumn::FArguments ConstructedHeaderRow = 
	SHeaderRow::Column(GetColumnID())
	.FixedWidth(24.f)
	.HAlignHeader(HAlign_Center)
	.VAlignHeader(VAlign_Center)
	.HAlignCell(HAlign_Center)
	.VAlignCell(VAlign_Center)
	.DefaultTooltip(FText::FromString(TEXT("Actor Selection Lock - Press icon to lock actor selection")))
	[
		SNew(SImage)
		.ColorAndOpacity(FSlateColor::UseForeground())
		.Image(FSuperManagerStyle::GetCreatedSlateStyleSet()->GetBrush(FName("LevelEditor.LockSelection")))
	];

	return ConstructedHeaderRow;
}

const TSharedRef<SWidget> FOutlinerSelectionLockColumn::ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, 
const STableRow<FSceneOutlinerTreeItemPtr>& Row)
{
	return SNullWidget::NullWidget;
}

