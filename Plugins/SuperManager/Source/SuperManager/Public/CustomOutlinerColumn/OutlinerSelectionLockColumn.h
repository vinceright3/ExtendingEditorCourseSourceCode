// Vince Petrelli All Rights Reserverd

#pragma once

#include "ISceneOutlinerColumn.h"

class FOutlinerSelectionLockColumn : public ISceneOutlinerColumn
{
public:
	FOutlinerSelectionLockColumn(ISceneOutliner& SceneOutliner){}

	virtual FName GetColumnID() override {return FName("SelectionLock");}

	static FName GetID() {return FName("SelectionLock");}

	virtual SHeaderRow::FColumn::FArguments ConstructHeaderRowColumn() override;

	virtual const TSharedRef< SWidget > ConstructRowWidget(FSceneOutlinerTreeItemRef TreeItem, const STableRow<FSceneOutlinerTreeItemPtr>& Row) override;
};