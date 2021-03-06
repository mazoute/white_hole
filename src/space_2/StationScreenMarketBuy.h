#pragma once
#include "FieldSet.h"
#include "StationScreenMarket.h"
#include "Tree.h"
#include "MarketItemListView.h"
#include "MarketItemDetailView.h"
#include "MarketItemStockView.h"
#include "MarketItemBuyView.h"


class StationScreenMarketBuy : public FieldSet
{
public:
	// Constructor - Destructor
	StationScreenMarketBuy(StationScreenMarket* p_market);
	~StationScreenMarketBuy(void);

	// Methods
	void reset();
	void update();
	void updatePosition();
	void updateSize();
	void update(sf::Event p_event);
	void draw();
	void notifyPositionChanged();
	void notifySizeChanged();
	void notifyItemTypeSelectedChanged();
	void notifyItemSelectedChanged();


private:
	// Attributs
	StationScreenMarket* mScreenMarket;
	Tree mTreeItemType;
	ListSelectableGroup mItemList;
	TextBox mTBSelectItem;
	MarketItemDetailView mItemDetail;
	MarketItemStockView mItemStock;
	MarketItemBuyView mItemBuy;
};

