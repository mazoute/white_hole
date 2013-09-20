#include "ContainerStack.h"
#include "CharacterShipItemStackUpdate.h"
#include "CharacterBankItemStackUpdate.h"
#include "Game.h"
#include "NetworkJobManager.h"


//*************************************************************
// Constructor - Destructor
//*************************************************************
ContainerStack::ContainerStack( ContainerRow* p_parent, ItemStack* p_stack, int p_position, ContainerStackType p_type ) : mItemStack(NULL)
{
	this->mContainerRow = p_parent;
	this->mItemStack = NULL;
	this->mType = p_type;
	this->mPosition = p_position;
	this->mItemStackChanged = false;
	this->mItemStack = p_stack;
}

ContainerStack::~ContainerStack(void)
{
	if(this->mItemStack != NULL)
		delete this->mItemStack;
}


//*************************************************************
// Getters - Setters
//*************************************************************
ContainerRow* ContainerStack::getContainerRow()
{
	return this->mContainerRow;
}

ItemStack* ContainerStack::getItemStack()
{
	return this->mItemStack;
}

void ContainerStack::setItemStack( ItemStack *p_stack, bool p_removePrec, bool p_notify )
{
	if(p_removePrec && this->mItemStack != NULL && this->mItemStack != p_stack)
		delete this->mItemStack;

	this->mItemStack = p_stack;
	if(p_notify)
		this->notifyItemStackChanged();
}

bool ContainerStack::hasItemStack()
{
	return this->mItemStack != NULL;
}

ContainerStack::ContainerStackType ContainerStack::getType()
{
	return this->mType;
}

void ContainerStack::setType( ContainerStackType p_type )
{
	this->mType = p_type;
}

int ContainerStack::getPosition()
{
	return this->mPosition;
}

void ContainerStack::setPosition( int p_position )
{
	this->mPosition = p_position;
}

bool ContainerStack::isItemTypeAllowed( ItemType* p_type )
{
	if(this->mItemTypesAllowed.size() == 0)
		return true;

	for(int i = 0; i < this->mItemTypesAllowed.size(); i++)
	{
		if(this->mItemTypesAllowed[i] != NULL && p_type != NULL && this->mItemTypesAllowed[i]->getId() == p_type->getId())
			return true;
	}

	return false;
}

bool ContainerStack::isItemStackChanged()
{
	bool returnValue = this->mItemStackChanged;
	this->mItemStackChanged = false;
	return returnValue;
}


//*************************************************************
// Methods
//*************************************************************
void ContainerStack::notifyItemStackChanged()
{
	this->mItemStackChanged = true;
	this->getContainerRow()->getContainerable()->notifyContentChanged();

	// Update server
	switch(this->mType)
	{
	case ContainerStack::ContainerStackType::TypeCharacterShip:
		if(Game::game != NULL && Game::game->getCharacterShip() != NULL)
			NetworkJobManager::getInstance()->addJob(new CharacterShipItemStackUpdate(this->getItemStack(), this->getPosition()));
		break;

	case ContainerStack::ContainerStackType::TypeCharacterBank:
		CharacterBank* bank = dynamic_cast<CharacterBank*>(this->getContainerRow()->getContainerable());
		if(Game::game != NULL && bank != NULL)
			NetworkJobManager::getInstance()->addJob(new CharacterBankItemStackUpdate(bank, this->getItemStack(), this->getPosition()));
		break;
	}
}

void ContainerStack::addItemTypeAllowed( ItemType* p_type )
{
	this->mItemTypesAllowed.push_back(p_type);
}

int ContainerStack::incStackSize( int p_size, bool p_notify )
{
	if(this->mItemStack != NULL)
	{
		int returnValue = this->mItemStack->incStackSize(p_size);
		if(p_notify)
			this->notifyItemStackChanged();
		return returnValue;
	}
}

int ContainerStack::decStackSize( int p_size, bool p_notify )
{
	if(this->mItemStack != NULL)
	{
		int returnValue = this->mItemStack->decStackSize(p_size);
		if(p_notify)
			this->notifyItemStackChanged();
		return returnValue;
	}
}

void ContainerStack::setStackSize( int p_size, bool p_notify )
{
	if(this->mItemStack != NULL)
	{
		this->mItemStack->setStackSize(p_size);
		if(p_notify)
			this->notifyItemStackChanged();
	}
}


