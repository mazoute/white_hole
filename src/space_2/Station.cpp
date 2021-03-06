#include "Station.h"
#include "ToolsImage.h"
#include "FactoryGet.h"
#include "ItemStock.h"


//*************************************************************
// Define
//*************************************************************
#define ROTATION_VELOCITY_MIN			0.1
#define ROTATION_VELOCITY_MAX			0.5
#define STOCKS_UPDATE_TICK				10 // Sec
#define STOCKS_UPDATE_TICK_IN			1 // Sec


//*************************************************************
// Constructor - Destructor
//*************************************************************
Station::Station(void) : Npc(this)
{
	this->mSpinnerSprite = NULL;
	this->mModel = NULL;
	this->setPlane(STATION_PLANE);
	this->setObjectType(MapObject::MapObjectType::TypeStation);

	this->setRotation(Tools::random(0, 360));
	this->setRotationInfinite(true);
	this->setRotationInfiniteRight(Tools::randomBool());
	this->setRotationVelocity(Tools::random((float)ROTATION_VELOCITY_MIN, (float)ROTATION_VELOCITY_MAX));

	// Stock
	std::vector<Item*> itemList = FactoryGet::getItemFactory()->getItemList();
	for(int i = 0; i < itemList.size(); i++)
	{
		if(itemList[i]->isBuyable())
			this->addItemStock(new ItemStock(itemList[i], this));
	}

	this->mUpdateStocks = true;
	this->mUpdateStocksTick = STOCKS_UPDATE_TICK;
	this->mStocksThread = new sf::Thread(&Station::updateStocks, this);
	this->mStocksThread->launch();
}

Station::~Station(void)
{
	delete this->mStocksThread;

	for(int i = 0; i < this->mStocks.size(); i++)
		delete this->mStocks[i];

	if(this->mSpinnerSprite != NULL)
		delete this->mSpinnerSprite;
}

void Station::terminate( bool p_instant )
{
	this->setUpdateStocks(false);
	if(p_instant)
		this->mStocksThread->terminate();
	else
		this->mStocksThread->wait();
}


//*************************************************************
// Getters - Setters
//*************************************************************
StationModel* Station::getModel()
{
	return this->mModel;
}

void Station::setModel( StationModel* p_model )
{
	if(this->mModel != p_model)
	{
		this->mModel = p_model;
		this->notifyModelChanged();
	}
}

int Station::getItemStockCount()
{
	sf::Lock lock(this->mStocksMutex);

	return this->mStocks.size();
}

ItemStock* Station::getItemStock( int p_index )
{
	sf::Lock lock(this->mStocksMutex);

	if(p_index < 0)
		p_index = 0;
	else if(p_index >= this->mStocks.size())
		p_index = this->mStocks.size() - 1;

	return this->mStocks[p_index];
}

ItemStock* Station::getItemStock( Item* p_item )
{
	sf::Lock lock(this->mStocksMutex);

	for(int i = 0; i < this->mStocks.size(); i++)
	{
		if(this->mStocks[i]->getItem()->getIdItem() == p_item->getIdItem())
			return this->mStocks[i];
	}
	return NULL;
}

bool Station::isUpdateStocks()
{
	sf::Lock lock(this->mStocksMutex);	

	return this->mUpdateStocks;
}

void Station::setUpdateStocks( bool p_value )
{
	sf::Lock lock(this->mStocksMutex);

	this->mUpdateStocks = p_value;
}

float Station::getUpdateStocksTick()
{
	sf::Lock lock(this->mStocksMutex);

	return this->mUpdateStocksTick;
}

void Station::setUpdateStocksTick( float p_tick )
{
	sf::Lock lock(this->mStocksMutex);

	this->mUpdateStocksTick = p_tick;
}


//*************************************************************
// Methods
//*************************************************************
void Station::dock()
{
	this->setUpdateStocksTick(STOCKS_UPDATE_TICK_IN);
}

void Station::undock()
{
	this->setUpdateStocksTick(STOCKS_UPDATE_TICK);
}

void Station::update()
{
	Entity::update();
	Npc::update();

	this->updateSpinner();
}

void Station::updateStocks()
{
	while(this->isUpdateStocks())
	{
		sf::sleep(sf::seconds(this->getUpdateStocksTick()));
		for(int i = 0; i < this->getItemStockCount(); i++)
		{
			if(this->isUpdateStocks())
				this->getItemStock(i)->update();
			else
				break;
		}
	}
}

void Station::updateSpinner()
{
	if(this->mSpinnerSprite != NULL)
	{
		this->mSpinnerRotation.update();
		this->mSpinnerSprite->setRotation(this->mSpinnerRotation.getRotation());
	}
}

void Station::updateSprite()
{
	Entity::updateSprite();

	sf::Vector2f objectPositionScreen = this->getScreenPosition();
	if(this->mSpinnerSprite != NULL)
		this->mSpinnerSprite->setPosition(objectPositionScreen.x, objectPositionScreen.y + this->getRocking());
}

void Station::draw()
{
	if(this->mSpinnerSprite != NULL)
		Resource::resource->getApp()->draw(*this->mSpinnerSprite);

	Entity::draw();
}

void Station::notifyModelChanged()
{
	if(this->getModel() != NULL)
	{
		WeaponableData::loadFromWeaponableData(this->getModel());
		DestructableData::loadFromDestructableData(this->getModel());
		EntityData::loadFromEntityData(this->getModel());
		this->loadNpcDataFromNpcData(this->getModel());

		this->mSpinnerRotation.setRotationVelocity(this->getModel()->getSpinnerVelocity());
		this->mSpinnerRotation.setRotationInfinite(true);
		this->mSpinnerRotation.setRotationInfiniteRight(Tools::randomBool());

		this->setScale(this->getModel()->getRandomScale());
		this->setSize(this->getModel()->getHitBoxWidth(), this->getModel()->getHitBoxHeight());
		this->loadSprite();
	}
}

void Station::loadSprite()
{
	Entity::loadSprite();

	if(this->getModel() != NULL)
	{
		this->mObjectSprite = new sf::Sprite(*Resource::resource->getTexture(this->getModel()->getSprite()));
		ToolsImage::setSpriteOriginCenter(this->mObjectSprite);
		this->mObjectSprite->setScale(this->mScale, this->mScale);
		this->mObjectSprite->setRotation(this->getRotation());

		if(this->getModel()->getSpinnerSprite() != "")
		{
			this->mSpinnerSprite = new sf::Sprite(*Resource::resource->getTexture(this->getModel()->getSpinnerSprite()));
			ToolsImage::setSpriteOriginCenter(this->mSpinnerSprite);
			this->mSpinnerSprite->setScale(this->mScale, this->mScale);
			this->mSpinnerSprite->setRotation(this->getRotation());
		}

		this->updateSprite();
	}
}

void Station::unloadSprite()
{
	Entity::unloadSprite();

	if(this->mSpinnerSprite != NULL)
	{
		delete this->mSpinnerSprite;
		this->mSpinnerSprite = NULL;
	}
}

void Station::addItemStock( ItemStock* p_stock )
{
	sf::Lock lock(this->mStocksMutex);

	this->mStocks.push_back(p_stock);
}

void Station::removeItemStock( ItemStock* p_stock )
{
	sf::Lock lock(this->mStocksMutex);

	for(int i = 0; i < this->mStocks.size(); i++)
	{
		if(this->mStocks[i]->getIdItemStock() == p_stock->getIdItemStock())
		{
			delete this->mStocks[i];
			this->mStocks.erase(this->mStocks.begin() + i);
			break;
		}
	}
}




