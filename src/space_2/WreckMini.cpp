#include "WreckMini.h"
#include "ToolsImage.h"
#include "Wreck.h"
#include "Tools.h"


//*************************************************************
// Define
//*************************************************************
#define SPRITE_BASE_PRE				"debris_"
#define SPRITE_BASE_SUF				".png"
#define SPRITE_EMBER_PRE			"debris_ember_"
#define SPRITE_EMBER_SUF			".png"
#define SPRITE_COUNT				6
#define QUICKENING					100
#define VELOCITY_MIN				250
#define VELOCITY_MAX				400
#define SIZE						32
#define ROTATION_VELOCITY_MIN		5
#define ROTATION_VELOCITY_MAX		10


//*************************************************************
// Constructor - Destructor
//*************************************************************
WreckMini::WreckMini( Wreck* p_parent )
{
	this->setVelocityMax(Tools::random(VELOCITY_MIN, VELOCITY_MAX));
	int vectorDirection = Tools::random(0, 3);
	if(vectorDirection == 0)	// North-East
	{
		this->setVelocityAt(Movable::MovableCardinality::North, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::NorthEast, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::East, Tools::random(0, this->getVelocityMax()));
	}
	else if(vectorDirection == 1)	// South-East
	{
		this->setVelocityAt(Movable::MovableCardinality::South, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::SouthEast, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::East, Tools::random(0, this->getVelocityMax()));
	}
	else if(vectorDirection == 2)	// South-West
	{
		this->setVelocityAt(Movable::MovableCardinality::South, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::SouthWest, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::West, Tools::random(0, this->getVelocityMax()));
	}
	else	// North-West
	{
		this->setVelocityAt(Movable::MovableCardinality::North, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::NorthWest, Tools::random(0, this->getVelocityMax()));
		this->setVelocityAt(Movable::MovableCardinality::West, Tools::random(0, this->getVelocityMax()));
	}

	this->setRockingActived(false);
	this->setQuickening(QUICKENING);
	this->setSize(SIZE, SIZE);
	this->setPosition(p_parent->Object::getX(), p_parent->Object::getY());
	this->setRotationInfinite(true);
	this->setRotationInfiniteRight(Tools::randomBool());
	this->setRotationVelocity(Tools::random(ROTATION_VELOCITY_MIN, ROTATION_VELOCITY_MAX));
	
	this->mSpriteId = Tools::random(1, SPRITE_COUNT);
}

WreckMini::~WreckMini(void)
{
}


//*************************************************************
// Methods
//*************************************************************
void WreckMini::update()
{
	EntityMovable::update();
}

void WreckMini::updateSprite()
{
	EntityMovable::updateSprite();
}

void WreckMini::loadSprite()
{
	EntityMovable::loadSprite();

	if(this->mObjectSprite == NULL)
	{
		this->mObjectSprite = new sf::Sprite(*Resource::resource->getTexture(SPRITE_BASE_PRE + Tools::buildStringWithInt(this->mSpriteId) + SPRITE_BASE_SUF));
		ToolsImage::setSpriteOriginCenter(this->mObjectSprite);
	}
}

void WreckMini::draw()
{
	EntityMovable::draw();
}
