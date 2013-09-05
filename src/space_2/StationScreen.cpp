#include "StationScreen.h"
#include "ClockManager.h"
#include "UserInterface.h"
#include "ContainerStackViewManager.h"
#include "NetworkJobManager.h"
#include "RunningStats.h"
#include "Juckebox.h"
#include "CharacterBank.h"
#include "Tools.h"


//*************************************************************
// Define
//*************************************************************
#define LEFTMENU_MARGIN						25


//*************************************************************
// Constructreur - Destructeur
//*************************************************************
StationScreen::StationScreen(Character* p_character)
{
	this->mStation = NULL;
	this->mCharacter = NULL;
	this->mLeftMenu = NULL;

	this->setCharacter(p_character);
	this->mLeftMenu = new StationScreenLeftMenu(p_character, this);

	this->notifyAppSizeChanged();
}

StationScreen::~StationScreen(void)
{
	if(this->mLeftMenu != NULL)
		delete this->mLeftMenu;
}


//*************************************************************
// Getters - Setters
//*************************************************************
Character* StationScreen::getCharacter()
{
	return this->mCharacter;
}

void StationScreen::setCharacter( Character* p_character )
{
	if(this->mCharacter != p_character)
	{
		this->mCharacter = p_character;
		this->notifyCharacterChanged();
	}
}

Station* StationScreen::getStation()
{
	return this->mStation;
}

void StationScreen::setStation( Station* p_station )
{
	if(this->mStation != p_station)
	{
		this->mStation = p_station;
		this->notifyStationChanged();
	}
}


//*************************************************************
// Methods
//*************************************************************
void StationScreen::launchBegin()
{
	// Ship Cargo refresh
	UserInterface::mUserInterface->getWindowCargoStationShip()->getContainerView()->notifyContainerableChanged();
}

void StationScreen::launchEnd()
{
	// Ship Cargo refresh
	UserInterface::mUserInterface->getWindowCargo()->getContainerView()->notifyContainerableChanged();
}

void StationScreen::launch( Station* p_station )
{
	this->launchBegin();
	this->setStation(p_station);
	this->notifyAppSizeChanged();
	while(Resource::resource->getApp()->isOpen() && Resource::resource->isAppRunning() && this->getStation() != NULL)
	{
		// Update
		EventManager::eventManager->reset();
		if(Resource::resource->getApp()->pollEvent(this->mEvent))
		{
			if( this->mEvent.type == sf::Event::Closed)
				Resource::resource->setAppRunning(false);

			if( this->mEvent.type == sf::Event::Resized)
			{
				Resource::resource->resizeAllView();
				this->notifyAppSizeChanged();
			}

			this->update(mEvent);
		}
		this->update();

		// Draw
		Resource::resource->getApp()->clear(sf::Color::White);
		this->draw();
		PopupBubble::drawAll();
		Resource::resource->getApp()->display();		
	}
	this->launchEnd();
}

void StationScreen::update()
{
	// Update Clock 
	ClockManager::update();

	// Update Juckbox
	Resource::resource->getJuckebox()->update();

	// Update Network Manager
	NetworkJobManager::getInstance()->update();

	// Update runningstats
	RunningStats::update();

	// Update Windows
	UserInterface::mUserInterface->updateWindowDynamicsStation();

	// Update Left menu
	this->mLeftMenu->update();
}

void StationScreen::updatePosition()
{
	this->mBackground.setPosition(0, 0);
	this->mLeftMenu->setPosition(LEFTMENU_MARGIN, LEFTMENU_MARGIN);
}

void StationScreen::updateBackgroundScale()
{
	float scaleWidth = Resource::resource->getViewUi()->getSize().x / this->mBackground.getLocalBounds().width;
	float scaleHeight = Resource::resource->getViewUi()->getSize().y / this->mBackground.getLocalBounds().height;
	float scale = scaleWidth > scaleHeight ? scaleWidth : scaleHeight;
	this->mBackground.setScale(scale, scale);
	this->updatePosition();
}

void StationScreen::update( sf::Event p_event )
{
	// Update drag'n'drop stack view
	if(p_event.type == sf::Event::MouseButtonReleased && p_event.mouseButton.button == sf::Mouse::Button::Left)
		ContainerStackViewManager::getInstance()->releaseDrag();

	// Update show Stats
	RunningStats::update(p_event);

	// Update windows
	UserInterface::mUserInterface->updateWindowDynamicsStation(p_event);

	// Update left menu
	this->mLeftMenu->update(p_event);	
}

void StationScreen::draw()
{
	Resource::resource->getApp()->draw(this->mBackground);
	this->mLeftMenu->draw();

	UserInterface::mUserInterface->drawWindowDynamicsStation();
	ContainerStackViewManager::getInstance()->draw();

	RunningStats::draw();
}

void StationScreen::notifyAppSizeChanged()
{
	this->updatePosition();
	this->updateBackgroundScale();

	this->mLeftMenu->setSize(StationScreenLeftMenu::getWidth(), Resource::resource->getViewUi()->getSize().y - LEFTMENU_MARGIN * 2);
}

void StationScreen::notifyStationChanged()
{
	if(this->getStation() != NULL)
	{
		this->mBackground.setTexture(*Resource::resource->getTexture(this->getStation()->getModel()->getScreenSprite()));
		this->updateBackgroundScale();
	}
}

void StationScreen::notifyCharacterChanged()
{
	if(this->mLeftMenu != NULL)
		this->mLeftMenu->setCharacter(this->getCharacter());
}





