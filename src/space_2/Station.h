#pragma once
#include "Entity.h"
#include "StationModel.h"
#include "Npc.h"
#include "EntityData.h"
#include "Clock.h"

class ItemStock;

// Define
#define STATION_PLANE		MAPOBJECT_PLANE_1

class Station : public Entity, public Npc
{
public:
	// Constructor - Destructor
	Station(void);
	~Station(void);

	// Getters - Setters
	StationModel* getModel();
	void setModel(StationModel* p_model);

	int getItemStockCount();
	ItemStock* getItemStock(int p_index);
	ItemStock* getItemStock(Item* p_item);

	// Methods
	void addItemStock(ItemStock* p_stock);
	void removeItemStock(ItemStock* p_stock);

	void update();
	void updateSpinner();
	void updateSprite();
	void draw();
	void notifyModelChanged();
	void loadSprite();
	void unloadSprite();


private:
	// Attributs
	StationModel* mModel;
	sf::Sprite* mSpinnerSprite;
	Rotable mSpinnerRotation;
	mks::Clock mSpinnerClock;

	std::vector<ItemStock*> mStocks;
};

