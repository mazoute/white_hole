#include "Game.h"
#include "RunningStats.h"
#include "ParticleManager.h"
#include "AnimationManager.h"
#include "ImageGIFFactory.h"
#include "AutoManager.h"
#include "NetworkJobManager.h"
#include "WreckManager.h"
#include "Juckebox.h"
#include "ClockManager.h"
#include "EntityManager.h"
#include "PopupManager.h"

using namespace sf;


//*************************************************************
// Implementation static
//*************************************************************
Game * Game::game = NULL;


//*************************************************************
// Constructor - Destructor
//*************************************************************
Game::Game(void)
{
	// Init
	this->game				= this;
	this->mMap				= NULL;
	this->mCamera			= NULL;
	this->mUserInterface	= NULL;
	this->mCharacter		= NULL;
}

Game::~Game(void)
{

}


//*************************************************************
// Getters - Setters
//*************************************************************
Map* Game::getMap()
{
	return this->mMap;
}

void Game::setMap( Map* p_myMap )
{
	// Delete myMap if exist
	if(this->mMap != NULL)
	{
		delete this->mMap;
		this->mMap = NULL;
	}

	// Set
	this->mMap = p_myMap;
}

UserInterface* Game::getUserInterface()
{
	return this->mUserInterface;
}

Character* Game::getCharacter()
{
	return this->mCharacter;
}

CharacterShip* Game::getShipPiloted()
{
	if(this->mCharacter != NULL)
		return this->mCharacter->getShipPiloted();
	else 
		return NULL;
}


//*************************************************************
// Getters - Setters
//*************************************************************
void Game::launchInit( Character* p_character )
{
	// Init basic
	this->mCharacter		= p_character;
	this->mMap				= new Map();
	this->mCamera			= new Camera();
	this->mUserInterface	= new UserInterface(this->mCharacter);

	// Init
	sf::Vector2i characterShipPosition = MapObject::convertPosition(sf::Vector2i(SECTOR_WIDTH / 2, SECTOR_HEIGHT / 2), SECTOR_PLANE, SHIP_PLANE);

	this->mCharacter->init();
	this->getShipPiloted()->setPosition(characterShipPosition.x, characterShipPosition.y);
	this->getUserInterface()->getXpBarCharacter()->setLevelable(this->getCharacter());
	this->notifyShipPilotedChanged();

	Resource::resource->getJuckebox()->playlistLaunch();
	Resource::resource->updateViewMap();
}

void Game::launchUninit()
{
	// Delete basic
	delete this->mMap;
	delete this->mCamera;
	delete this->mUserInterface;
}

void Game::launch(Character* p_character)
{
	this->launchInit(p_character);
	this->launchTest();
	while(Resource::resource->getApp()->isOpen() && Resource::resource->isAppRunning())
	{
		// Update
		EventManager::eventManager->reset();
		if(Resource::resource->getApp()->pollEvent(this->mEvent))
		{
			if( this->mEvent.type == Event::Closed)
				Resource::resource->setAppRunning(false);

			if( this->mEvent.type == Event::Resized)
				Resource::resource->resizeAllView();

			// Update Game when event
			this->update(mEvent);
		}
		this->update();

		// Draw
		this->mCamera->draw();
		PopupManager::getInstance()->draw();
		Resource::resource->getApp()->display();		
	}
}

void Game::launchTest()
{
	// Function tests
	/*
	Animation * plop = new Animation(UiImageGIFFactory::getGifShield());
	plop->setAnimationX(250);
	plop->setAnimationY(250);
	plop->setSpriteOriginCenter();
	plop->rotate(45, 90, 15000, ANIMATION_ROTATION_DIRECTION_LEFT);
	plop->scaleNewSize(96, 96, 10000);
	plop->translate(750, 100, 7500);
	plop->start();
	AnimationManager::addAnimation(plop);
	*/
}

void Game::update( sf::Event p_event )
{
	// Event Manager
	EventManager::eventManager->handlePlayerAction(p_event);

	// Update UserInterface
	Game::game->getUserInterface()->update(p_event);

	// Update show Stats
	RunningStats::update(p_event);

	// Update ship
	Game::game->getShipPiloted()->update(p_event);

	// Update Map
	this->getMap()->update(p_event);
}

void Game::update()
{
	// Update ClockManager
	ClockManager::update();

	// Event Manager Pre
	EventManager::eventManager->handlePlayerActionPre();

	// Update Juckbox
	Resource::resource->getJuckebox()->update();

	// Update Animation
	AnimationManager::update();

	// Update character
	this->getCharacter()->update();

	// Update ship
	this->getShipPiloted()->update();

	// Update runningstats
	RunningStats::update();

	// Update particle
	ParticleManager::update();

	// Update Map
	this->getMap()->update();

	// Update AutoManager
	AutoManager::update();

	// Update NetworkJobManager
	NetworkJobManager::getInstance()->update();

	// Update UserInterface
	this->getUserInterface()->update();

	// Event Manager Post
	EventManager::eventManager->handlePlayerActionPost();
}

void Game::changedShipPiloted( CharacterShip* p_ship )
{
	EntityManager::remove(this->getShipPiloted());
	this->getMap()->getMapObjectSelector()->removeMapObject(this->getShipPiloted());

	this->getCharacter()->setShipPiloted(p_ship);
	this->notifyShipPilotedChanged();
}

void Game::notifyShipPilotedChanged()
{
	this->mUserInterface->notifyWeaponViewChanged();
	this->mUserInterface->getXpBarCharacterShip()->setLevelable(this->getShipPiloted());
	this->mUserInterface->getWindowShipSmall()->setCharacterShip(this->getCharacter()->getShipPiloted());
	this->mUserInterface->getWindowCargo()->getContainerView()->setContainerable(this->getCharacter()->getShipPiloted());
	this->mUserInterface->getWindowCargoStationShip()->getContainerView()->setContainerable(this->getCharacter()->getShipPiloted());

	EntityManager::add(this->getShipPiloted());
	this->getMap()->getMapObjectSelector()->addMapObject(this->getShipPiloted());
}
