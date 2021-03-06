#include "CharacterShip.h"
#include "Option.h"
#include "Camera.h"
#include "LevelManager.h"
#include "ShipModelFactory.h"
#include "FactoryGet.h"
#include "Weapon.h"
#include "Game.h"
#include "ToolsMap.h"
#include "ToolsImage.h"
#include "NetworkJobManager.h"
#include "CharacterShipUpdate.h"


//*************************************************************
// Define
//*************************************************************
#define JSON_NAME						"name"
#define JSON_SKILLPOINTS				"skillPoints"
#define JSON_LEVEL						"level"
#define JSON_EXPERIENCE					"experience"
#define JSON_PILOTED					"piloted"
#define JSON_IDSHIPMODEL				"idShipModel"
#define JSON_IDCHARACTER				"idCharacter"
#define JSON_WEAPONS					"weapons"
#define JSON_ITEMSTACKS					"itemStacks"
#define NPCTYPE_ID						1


//*************************************************************
// Constructor - Destructor
//*************************************************************
CharacterShip::CharacterShip( Json::Value json, Character *p_character ): Ship(0, 0), Containerable(ContainerStack::ContainerStackType::TypeCharacterShip)
{
	this->mLoaded = false;
	this->setCharacter(p_character);
	this->loadFromJson(json);
	this->setObjectType(MapObjectType::TypeCharacterShip);
	this->setNpcType(FactoryGet::getNpcTypeFactory()->getNpcType(NPCTYPE_ID));

	this->mArrowObjectSelected = NULL;
	this->mArrowObjectSelected = SpriteParameterFactory::getSpriteParameterIcon32X32()->getSpritePtr(IC_32X32_ARROW);
	ToolsImage::setSpriteOriginCenter(this->mArrowObjectSelected);
}

CharacterShip::~CharacterShip(void)
{
	if(this->mArrowObjectSelected != NULL)
		delete this->mArrowObjectSelected;
}


//*************************************************************
// Getters - Setters
//*************************************************************
long CharacterShip::getIdCharacterShip()
{
	return this->mIdCharacterShip;
}

void CharacterShip::setIdCharacterShip( long p_id )
{
	this->mIdCharacterShip = p_id;
}

std::string CharacterShip::getName()
{
	return this->mName;
}

void CharacterShip::setName( std::string p_name )
{
	if(this->mName != p_name)
	{
		this->mName = p_name;
		this->notifyNameChanged();
	}
}

bool CharacterShip::hasSkillPoints()
{
	return this->mSkillPoints > 0;
}

int CharacterShip::getSkillPoints()
{
	return this->mSkillPoints;
}

void CharacterShip::setSkillPoints( int p_points )
{
	if(p_points < 0)
		p_points = 0;

	if(this->mSkillPoints != p_points)
	{
		this->mSkillPoints = p_points;
		this->notifySkillPointsChanged();
	}
}

void CharacterShip::setX( double p_x )
{
	Object::setX(p_x);
	Camera::camera->setCameraX(p_x);
}

void CharacterShip::setY( double p_y )
{
	Object::setY(p_y);
	Camera::camera->setCameraY(p_y);
}

bool CharacterShip::isPiloted()
{
	return this->mPiloted;
}

void CharacterShip::setPiloted(bool p_piloted)
{
	if(this->mPiloted != p_piloted)
	{
		this->mPiloted = p_piloted;
		this->notifyPilotedChanged();
	}
}

NpcType* CharacterShip::getNpcType()
{
	return this->mNpcType;
}

void CharacterShip::setNpcType( NpcType* p_type )
{
	this->mNpcType = p_type;
}

Character* CharacterShip::getCharacter()
{
	return this->mCharacter;
}

void CharacterShip::setCharacter( Character *p_character )
{
	this->mCharacter = p_character;
}


//*************************************************************
// Method
//*************************************************************
void CharacterShip::draw()
{
	Ship::draw();
	this->drawArrowObjectSelected();
}

void CharacterShip::drawArrowObjectSelected()
{
	if(Game::game->getMap()->getMapObjectSelector()->hasObjectSelected() && Game::game->getMap()->getMapObjectSelector()->isObjectSelectedOutOfScreen())
		Resource::resource->getApp()->draw(*this->mArrowObjectSelected);
}

void CharacterShip::update( sf::Event p_event )
{
	Ship::update(p_event);
}

void CharacterShip::update()
{
	Ship::update();
	this->updateRotation();
}

void CharacterShip::updatePosition()
{
	float varX = this->getMoveX();
	float varY = this->getMoveY();

	Camera::frameShipMoveOffset.x = varX; 
	Camera::frameShipMoveOffset.y = varY; 

	this->setX(this->Object::getX() + varX);
	this->setY(this->Object::getY() + varY);
}

void CharacterShip::updateQuickeningActive()
{
	bool keyUpPressed = sf::Keyboard::isKeyPressed(Option::option->getAppControl(OPTION_APP_CONTROL_UP_KEY));
	bool keyRightPressed = sf::Keyboard::isKeyPressed(Option::option->getAppControl(OPTION_APP_CONTROL_RIGHT_KEY));
	bool keyDownPressed = sf::Keyboard::isKeyPressed(Option::option->getAppControl(OPTION_APP_CONTROL_DOWN_KEY));
	bool keyLeftPressed = sf::Keyboard::isKeyPressed(Option::option->getAppControl(OPTION_APP_CONTROL_LEFT_KEY));
	
	this->setQuickeningActiveAt(MovableCardinality::North, keyUpPressed && !keyRightPressed && !keyLeftPressed);
	this->setQuickeningActiveAt(MovableCardinality::NorthEast, keyUpPressed && keyRightPressed );
	this->setQuickeningActiveAt(MovableCardinality::East, keyRightPressed && !keyUpPressed && !keyDownPressed);
	this->setQuickeningActiveAt(MovableCardinality::SouthEast, keyDownPressed && keyRightPressed);
	this->setQuickeningActiveAt(MovableCardinality::South, keyDownPressed && !keyRightPressed && !keyLeftPressed);
	this->setQuickeningActiveAt(MovableCardinality::SouthWest, keyDownPressed && keyLeftPressed);
	this->setQuickeningActiveAt(MovableCardinality::West, keyLeftPressed && !keyUpPressed && !keyDownPressed);
	this->setQuickeningActiveAt(MovableCardinality::NorthWest, keyUpPressed && keyLeftPressed);
}

void CharacterShip::updateSprite()
{
	Ship::updateSprite();
	if(Game::game->getMap() != NULL && Game::game->getMap()->getMapObjectSelector()->hasObjectSelected())
	{
		float angle = ToolsMap::getAngleAbsolute(this, Game::game->getMap()->getMapObjectSelector()->getSelectedMapObject());
		this->mArrowObjectSelected->setPosition(this->getScreenX() + (this->getRadius() * 2 * cos(Tools::degreeToRadian(angle + 270))), 
												this->getScreenY() + (this->getRadius() * 2 * sin(Tools::degreeToRadian(angle + 270))));
		this->mArrowObjectSelected->setRotation(angle - 90);
	}
}

void CharacterShip::updateRotation()
{
	sf::View* appView = Resource::resource->getViewMap(this->getPlane());
	float appCenterX = appView->getCenter().x;
	float appCenterY = appView->getCenter().y;
	sf::Vector2f mousePosition = EventManager::eventManager->getMousePositionMapLocal(this->getPlane());

	float angle = Tools::getAngle(appCenterX, appCenterY, mousePosition.x, mousePosition.y);
	this->setRotationTarget(angle);
}

void CharacterShip::loadFromJson( Json::Value json )
{
	this->mLoaded = false;
	this->setIdCharacterShip(json.get(CHARACTERSHIP_JSON_IDCHARACTERSHIP, -1).asInt());
	this->setName(json.get(JSON_NAME, "").asString());
	this->setSkillPoints(json.get(JSON_SKILLPOINTS, 0).asInt());
	this->setLevel(json.get(JSON_LEVEL, 0).asInt());
	this->setExperience(json.get(JSON_EXPERIENCE, 0).asInt());
	this->setPiloted(json.get(JSON_PILOTED, false).asBool());
	this->setShipModel(FactoryGet::getShipModelFactory()->getShipModel(json.get(JSON_IDSHIPMODEL, 0).asInt()));
	
	// Weapons
	Json::Value weapons = json.get(JSON_WEAPONS, NULL);
	if(weapons != NULL)
	{
		for(int i = 0; i < weapons.size(); i++)
			this->addWeapon(new Weapon(weapons.get(i, NULL), this));
	}

	// ItemStacks
	Json::Value itemStacks = json.get(JSON_ITEMSTACKS, NULL);
	if(itemStacks != NULL)
	{
		for(int i = 0; i < itemStacks.size(); i++)
		{
			Json::Value currentItemStack = itemStacks.get(i, NULL);
			ItemStack* currentStack = new ItemStack(currentItemStack.get(CHARACTERSHIP_JSON_ITEMSTACK, NULL));
			this->addItemStack(currentStack, currentItemStack.get(CHARACTERSHIP_JSON_ITEMSTACKPOSITION, 0).asInt());
		}
	}
	this->mLoaded = true;
}

Json::Value CharacterShip::saveToJson()
{
	Json::Value json;
	json[CHARACTERSHIP_JSON_IDCHARACTERSHIP] = this->getIdCharacterShip();
	json[JSON_NAME] = this->getName();
	json[JSON_SKILLPOINTS] = this->getSkillPoints();
	json[JSON_LEVEL] = this->getLevel();
	json[JSON_EXPERIENCE] = this->getExperience();
	json[JSON_PILOTED] = this->isPiloted();
	json[JSON_IDSHIPMODEL] = this->getShipModel()->getIdItem();
	json[JSON_IDCHARACTER] = this->getCharacter()->getIdCharacter();
	return json;
}

void CharacterShip::loadFromShipModel()
{
	Ship::loadFromShipModel();

	if(this->getShipModel() != NULL)
	{
		this->setCargoMax(this->mShipModel->getCargoMax());
		this->setLevelConfig(this->mShipModel->getLevelShipConfig());
	}
}

void CharacterShip::incSkillPoints()
{
	this->incSkillPoints(1);
}

void CharacterShip::incSkillPoints( int p_inc )
{
	this->setSkillPoints(this->getSkillPoints() + p_inc);
}

void CharacterShip::decSkillPoints()
{
	this->setSkillPoints(this->getSkillPoints() - 1);
}

void CharacterShip::incLevel()
{
	Levelable::incLevel();
	this->incSkillPoints(this->getLevelConfig()->getSkillPointsOnLevelUp());
	NetworkJobManager::getInstance()->addJob(new CharacterShipUpdate(this));
}

void CharacterShip::incExperience( long p_inc )
{
	Levelable::incExperience(p_inc);
	NetworkJobManager::getInstance()->addJob(new CharacterShipUpdate(this));
}

void CharacterShip::notifySkillPointsChanged()
{
	if(this->mLoaded)
		NetworkJobManager::getInstance()->addJob(new CharacterShipUpdate(this));
}

void CharacterShip::notifyNameChanged()
{
	if(this->mLoaded)
		NetworkJobManager::getInstance()->addJob(new CharacterShipUpdate(this));
}

void CharacterShip::notifyPilotedChanged()
{
	if(this->mLoaded)
		NetworkJobManager::getInstance()->addJob(new CharacterShipUpdate(this));
}

