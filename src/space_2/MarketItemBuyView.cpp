#include "MarketItemBuyView.h"
#include "ContainerDropDown.h"
#include "Game.h"
#include "CharacterBank.h"
#include "Tools.h"
#include "UserInterface.h"
#include "WindowMessageError.h"
#include "WindowChoiceAsk.h"
#include "WindowMessageSuccess.h"


//*************************************************************
// Define
//*************************************************************
#define BACKGROUNDCOLOR						sf::Color(25, 26, 28)
#define BORDERCOLOR							sf::Color(255, 255, 255, 0)
#define BORDERSIZE							0
#define PADDING								10
#define TFQUANTITY_WIDTH					200
#define TFQUANTITY_MARGINTOP				10
#define BUTTONQUANTITYALL_MARGINLEFT		20
#define DESTINATION_MARGINTOP				35
#define DLLDESTINATION_MARGINTOP			10
#define FIELDSETTOTAL_BACKGROUNDCOLOR		sf::Color(255, 255, 255, 0)
#define FIELDSETTOTAL_BORDERCOLOR			sf::Color(125, 125, 125, 0)
#define FIELDSETTOTAL_BORDERSIZE			0
#define FIELDSETTOTAL_MARGINLEFT			75
#define FIELDSETTOTAL_HEIGHT				185
#define FIELDSETTOTAL_PADDING				20
#define TBTOTAL_FONTSIZE					22
#define TBTOTALLABEL_MARGINBOTTOM			10
#define TBTOTAL_MARGINBOTTOM				10
#define TBTOTALAVERAGE_FONTSIZE				14
#define BUTTONBUY_MARGINTOP					40
#define UPDATE_TICK							500	// ms
#define ACTIONCOMMAND_BUY					"actionbuy"


//*************************************************************
// Constructor - Destructor
//*************************************************************
MarketItemBuyView::MarketItemBuyView(void) : mPUBTotalDetail(&this->mTBTotalAverage)
{
	this->mItemStock = NULL;

	this->setVisible(false);
	this->setDisplayTitle(false);
	this->setBackgroundColor(BACKGROUNDCOLOR, true);
	this->setBorderColor(BORDERCOLOR, true);
	this->setBorderSize(BORDERSIZE, true);
	this->setPadding(PADDING);

	this->mTBQuantity.setText(Resource::resource->getBundle()->getString("marketBuyQuantity"));
	this->mTFQuantity.setValue("0");
	this->mTBDestination.setText(Resource::resource->getBundle()->getString("marketBuyToBank"));
	this->mButtonQuantityAll.setTitle(Resource::resource->getBundle()->getString("marketBuyQuantityAll"));
	this->mTBTotalLabel.setText(Resource::resource->getBundle()->getString("totalMaj"));
	this->mTBTotalLabel.setFontSize(TBTOTAL_FONTSIZE);
	this->mTBTotal.setFontSize(TBTOTAL_FONTSIZE);
	this->mTBTotalAverage.setFontSize(TBTOTALAVERAGE_FONTSIZE);
	this->mButtonBuy.setTitle(Resource::resource->getBundle()->getString("buy"));

	this->mFieldsetTotal.setDisplayTitle(false);
	this->mFieldsetTotal.setBackgroundColor(FIELDSETTOTAL_BACKGROUNDCOLOR, true);
	this->mFieldsetTotal.setBorderColor(FIELDSETTOTAL_BORDERCOLOR, true);
	this->mFieldsetTotal.setBorderSize(FIELDSETTOTAL_BORDERSIZE, true);
	this->mFieldsetTotal.setPadding(FIELDSETTOTAL_PADDING);

	this->updateDestinations();
	this->updateFieldsetTotalSize();
}

MarketItemBuyView::~MarketItemBuyView(void)
{
}


//*************************************************************
// Getters - Setters
//*************************************************************
ItemStock* MarketItemBuyView::getItemStock()
{
	return this->mItemStock;
}

void MarketItemBuyView::setItemStock( ItemStock* p_stock )
{
	if(this->mItemStock != p_stock)
	{
		this->mItemStock = p_stock;
		this->notifyItemStockChanged();
	}
}


//*************************************************************
// Methods
//*************************************************************
void MarketItemBuyView::buy()
{
	// Check quantity & get price
	int quantity = this->mItemStockSimulator.getBuyQuantity();
	if(quantity > this->mItemStock->getStockCurrent())
	{
		quantity = this->mItemStock->getStockCurrent();
		this->mItemStockSimulator.computeBuyPrice(quantity);
	}
	float price = this->mItemStockSimulator.getBuyPrice();
	
	// Dec credit
	double oldBalance = Game::game->getCharacter()->getCredit();
	Game::game->getCharacter()->decCredit(price);
	double newBalance = Game::game->getCharacter()->getCredit();

	// Dec stock
	this->mItemStock->decStockCurrent(quantity);

	// Add to containerable
	ContainerDropDown* destinationDropDown = (ContainerDropDown*)this->mDDLDestination.getSelectedDropDownable();
	Containerable* destination = destinationDropDown->getContainer();
	destination->addItem(this->mItemStock->getItem(), quantity);

	// Success message
	std::string messageSuccess = this->mItemStock->getItem()->getName() + " " + Resource::resource->getBundle()->getString("marketBuySuccessMsg1");
	messageSuccess += Tools::formatNumber(quantity) + Resource::resource->getBundle()->getString("marketBuySuccessMsg2") + " ";
	messageSuccess += destinationDropDown->getText() + "<br/>";
	messageSuccess += Resource::resource->getBundle()->getString("marketBuySuccessMsg3") + Tools::getSpaceAfterColon() + Tools::formatNumber(price) + "<br/>";
	messageSuccess += Resource::resource->getBundle()->getString("marketBuySuccessMsg4") + Tools::getSpaceAfterColon() + Tools::formatNumber(oldBalance) + "<br/>";
	messageSuccess += Resource::resource->getBundle()->getString("marketBuySuccessMsg5") + Tools::getSpaceAfterColon() + Tools::formatNumber(newBalance);
	UserInterface::mUserInterface->addWindowPopup(new WindowMessageSuccess(Resource::resource->getBundle()->getString("marketBuySuccessTitle"), messageSuccess));
}

void MarketItemBuyView::buyConfirmation()
{
	// Check price
	if(!Game::game->getCharacter()->hasEnoughCredit(this->mItemStockSimulator.getBuyPrice()))
	{
		std::string messageError = Resource::resource->getBundle()->getString("marketBuyErrorMoneyMsg1") + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorMoneyMsg2") + Tools::getSpaceAfterColon() + Tools::formatNumber(Game::game->getCharacter()->getCredit()) + " " + Resource::resource->getBundle()->getString("creditAb") + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorMoneyMsg3") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyPrice()) + " " + Resource::resource->getBundle()->getString("creditAb") + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorMoneyMsg4") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyPrice() - Game::game->getCharacter()->getCredit()) + " " + Resource::resource->getBundle()->getString("creditAb");
		UserInterface::mUserInterface->addWindowPopup(new WindowMessageError(Resource::resource->getBundle()->getString("marketBuyErrorMoneyTitle"), messageError));
		return;
	}

	// Check space
	ContainerDropDown* destination = (ContainerDropDown*)this->mDDLDestination.getSelectedDropDownable();
	int contentFree = destination->getContainer()->getContentFreeFor(this->mItemStock->getItem());
	if(contentFree < this->mItemStockSimulator.getBuyQuantity())
	{
		std::string messageError = Resource::resource->getBundle()->getString("marketBuyErrorSpaceMsg1") + destination->getText() + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorSpaceMsg2") + Tools::getSpaceAfterColon() + Tools::formatNumber(contentFree) + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorSpaceMsg3") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyQuantity()) + "<br/>";
		messageError += Resource::resource->getBundle()->getString("marketBuyErrorSpaceMsg4") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyQuantity() - contentFree);
		UserInterface::mUserInterface->addWindowPopup(new WindowMessageError(Resource::resource->getBundle()->getString("marketBuyErrorSpaceTitle"), messageError));
		return;
	}

	// Confirmation
	std::string messageConfirmation = Resource::resource->getBundle()->getString("marketBuyConfirmationMsg1") + " ";
	messageConfirmation += this->mItemStock->getItem()->getName() + " " + Resource::resource->getBundle()->getString("marketBuyConfirmationMsg2");
	messageConfirmation += Tools::formatNumber(this->mItemStockSimulator.getBuyQuantity()) + Resource::resource->getBundle()->getString("marketBuyConfirmationMsg3") + " ";
	messageConfirmation += Tools::formatNumber(this->mItemStockSimulator.getBuyPrice()) + " " + Resource::resource->getBundle()->getString("marketBuyConfirmationMsg4");
	UserInterface::mUserInterface->addWindowPopup(new WindowChoiceAsk(	Resource::resource->getBundle()->getString("marketBuyConfirmationTitle"), 
																		messageConfirmation, this, NULL, ACTIONCOMMAND_BUY));
}

void MarketItemBuyView::update()
{
	if(this->mUpdateClock.getElapsedTimeAsMilliseconds() > UPDATE_TICK)
	{
		this->checkQuantityValue();
		this->updateFieldsetTotal();
	}

	this->mDDLDestination.update();
	this->mPUBTotalDetail.update();
}

void MarketItemBuyView::update( sf::Event p_event )
{
	if(this->isVisible())
	{
		this->mTFQuantity.update(p_event);
		this->mButtonQuantityAll.update(p_event);
		this->mDDLDestination.update(p_event);
		this->mTBTotalAverage.update(p_event);
		this->mPUBTotalDetail.update(p_event);
		this->mButtonBuy.update(p_event);

		if(this->mButtonQuantityAll.isClicked())
		{
			if(this->mItemStock != NULL)
				this->mTFQuantity.setValue(Tools::buildStringWithLong(this->mItemStock->getStockCurrent()));
		}

		if(this->mButtonBuy.isClicked())
			this->buyConfirmation();
	}
	FieldSet::update(p_event);
}

void MarketItemBuyView::updatePosition()
{
	this->mTBQuantity.setPosition(this->getContentX(), this->getContentY());
	this->mTFQuantity.setPosition(this->mTBQuantity.getX(), this->mTBQuantity.getBottomY() + TFQUANTITY_MARGINTOP);
	this->mButtonQuantityAll.setPosition(this->mTFQuantity.getRightX() + BUTTONQUANTITYALL_MARGINLEFT, this->mTFQuantity.getBottomY() - this->mButtonQuantityAll.getHeight());
	this->mTBDestination.setPosition(this->mTFQuantity.getX(), this->mTFQuantity.getBottomY() + DESTINATION_MARGINTOP);
	this->mDDLDestination.setPosition(this->mTBDestination.getX(), this->mTBDestination.getBottomY() + DLLDESTINATION_MARGINTOP);
	this->updateFieldsetTotalPosition();
}

void MarketItemBuyView::updateFieldsetTotalPosition()
{
	this->mFieldsetTotal.setPosition(this->mButtonQuantityAll.getRightX() + FIELDSETTOTAL_MARGINLEFT, this->getContentY());
	this->mTBTotalLabel.setPosition(this->mFieldsetTotal.getContentX() + (this->mFieldsetTotal.getContentWidth() - this->mTBTotalLabel.getWidth()) / 2, this->mFieldsetTotal.getContentY());
	this->mTBTotal.setPosition(this->mFieldsetTotal.getContentX() + (this->mFieldsetTotal.getContentWidth() - this->mTBTotal.getWidth()) / 2, this->mTBTotalLabel.getBottomY() + TBTOTALLABEL_MARGINBOTTOM);
	this->mTBTotalAverage.setPosition(this->mFieldsetTotal.getContentX() + (this->mFieldsetTotal.getContentWidth() - this->mTBTotalAverage.getWidth()) / 2, this->mTBTotal.getBottomY() + TBTOTAL_MARGINBOTTOM);
	this->mButtonBuy.setPosition(this->mFieldsetTotal.getContentX() + (this->mFieldsetTotal.getContentWidth() - this->mButtonBuy.getWidth()) / 2, this->mTBTotalAverage.getBottomY() + BUTTONBUY_MARGINTOP);
}

void MarketItemBuyView::updateFieldsetTotalSize()
{
	this->mFieldsetTotal.setSize(	this->getContentWidth() - this->mTFQuantity.getWidth() - BUTTONQUANTITYALL_MARGINLEFT - this->mButtonQuantityAll.getWidth() - FIELDSETTOTAL_MARGINLEFT, 
									FIELDSETTOTAL_HEIGHT);
}	

void MarketItemBuyView::updateFieldsetTotal()
{
	if(this->mItemStock != NULL)
	{
		int quantity = Tools::getLongFromString(this->mTFQuantity.getValue());
		this->mItemStockSimulator.computeBuyPrice(quantity);

		this->mTBTotal.setText(Tools::formatNumber(this->mItemStockSimulator.getBuyPrice()) + " " + Resource::resource->getBundle()->getString("creditAb"));
		this->mTBTotalAverage.setText("(" + Resource::resource->getBundle()->getString("marketBuyAverage") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyPriceAve()) + " " + Resource::resource->getBundle()->getString("creditAb") + ")");
		
		this->mPUBTotalDetail.clear(false);
		this->mPUBTotalDetail.addLine(Resource::resource->getBundle()->getString("marketBuyPriceMin") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyPriceMin()), false);
		this->mPUBTotalDetail.addLine(Resource::resource->getBundle()->getString("marketBuyPriceMax") + Tools::getSpaceAfterColon() + Tools::formatNumber(this->mItemStockSimulator.getBuyPriceMax()));

		this->mButtonBuy.setEnable(quantity > 0);

		this->updateFieldsetTotalPosition();
	}
}	

void MarketItemBuyView::updateDestinations()
{
	Character* character = Game::game->getCharacter();

	this->mDDLDestination.removeAllDropDownable();
	this->mDDLDestination.addDropDownable(new ContainerDropDown(character->getShipPiloted()));
	for(int i = 0; i < character->getBankCount(); i++)
	{
		if(character->getBank(i)->isUnlock())
			this->mDDLDestination.addDropDownable(new ContainerDropDown(character->getBank(i)));
	}
}

void MarketItemBuyView::checkQuantityValue()
{
	if(this->mItemStock != NULL)
	{
		std::string quantityValue = this->mTFQuantity.getValue();
		if(!Tools::isNumber(quantityValue) && !quantityValue.empty())
		{
			this->mTFQuantity.setValue("0");
		}
		else
		{
			long quantity = Tools::getLongFromString(quantityValue);
			if(quantity < 0)
				quantity = 0;
			else if(quantity > this->mItemStock->getStockCurrent())
				quantity = this->mItemStock->getStockCurrent();

			this->mTFQuantity.setValue(Tools::buildStringWithLong(quantity));
		}
	}
}

void MarketItemBuyView::draw()
{
	FieldSet::draw();
	if(this->isVisible())
	{
		this->mTBQuantity.draw();
		this->mTFQuantity.draw();
		this->mButtonQuantityAll.draw();
		this->mTBDestination.draw();
		this->mDDLDestination.draw();
		this->mFieldsetTotal.draw();
		this->mTBTotalLabel.draw();
		this->mTBTotal.draw();
		this->mTBTotalAverage.draw();
		this->mButtonBuy.draw();
	}
}

void MarketItemBuyView::notifyItemStockChanged()
{
	this->setVisible(this->mItemStock != NULL);

	this->mItemStockSimulator.setItemStock(this->mItemStock);
	this->updateFieldsetTotal();
}

void MarketItemBuyView::notifyPositionChanged()
{
	FieldSet::notifyPositionChanged();
	this->updatePosition();
}

void MarketItemBuyView::notifySizeChanged()
{
	FieldSet::notifySizeChanged();
	this->updateFieldsetTotalSize();
	this->updatePosition();
}

void MarketItemBuyView::onButtonTrueClicked( WindowChoiceActionObject* p_object, std::string p_actionCommand )
{
	this->buy();
}

void MarketItemBuyView::onButtonFalseClicked( WindowChoiceActionObject* p_object, std::string p_actionCommand )
{

}
