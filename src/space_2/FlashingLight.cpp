#include "FlashingLight.h"
#include "ToolsImage.h"
#include "Tools.h"


//*************************************************************
// Define
//*************************************************************
#define JSON_OFFSETX				"x"
#define JSON_OFFSETY				"y"
#define JSON_SIZE					"size"
#define JSON_COLOR					"color"
#define JSON_FLASHTICKMIN			"tickmin"
#define JSON_FLASHTICKMAX			"tickmax"
#define IMG_FLASHLIGHT				"flashinglight.png"
#define IMG_LIGHT_BLUE				"light_blue.png"
#define IMG_LIGHT_GREEN				"light_green.png"
#define IMG_LIGHT_GREY				"light_grey.png"
#define IMG_LIGHT_PURPLE			"light_purple.png"
#define IMG_LIGHT_RED				"light_red.png"
#define FLASH_SIZE_PERCENT			0.25
#define FLASH_TIME					50 //ms


//*************************************************************
// Constructreur - Destructeur
//*************************************************************
FlashingLight::FlashingLight( Entity* p_entity, Json::Value p_lightJson ) : EntityEffect(p_entity)
{
	this->setOffsetX(p_lightJson.get(JSON_OFFSETX, 0).asInt());
	this->setOffsetY(p_lightJson.get(JSON_OFFSETY, 0).asInt());
	this->updateOffsetRotate();
	this->setSize(p_lightJson.get(JSON_SIZE, 0).asInt());

	this->mLightColor = (LightColor)p_lightJson.get(JSON_COLOR, 0).asInt();
	this->mFlashTickMin = p_lightJson.get(JSON_FLASHTICKMIN, 0).asFloat();
	this->mFlashTickMax = p_lightJson.get(JSON_FLASHTICKMAX, 0).asFloat();
	this->computeFlashTick();
	this->mFlashDraw = false;

	this->mLightSprite.setTexture(*Resource::resource->getTexture(this->getLightSprite()));
	ToolsImage::setSpriteOriginCenter(&this->mLightSprite);	
	ToolsImage::resizeSprite(&this->mLightSprite, this->getSize(), this->getSize());

	this->mFlashSprite.setTexture(*Resource::resource->getTexture(IMG_FLASHLIGHT));
	ToolsImage::setSpriteOriginCenter(&this->mFlashSprite);	
	ToolsImage::resizeSprite(&this->mFlashSprite, this->getSize() * FLASH_SIZE_PERCENT, this->getSize() * FLASH_SIZE_PERCENT);
}

FlashingLight::~FlashingLight(void)
{
}


//*************************************************************
// Getters - Setters
//*************************************************************
std::string FlashingLight::getLightSprite()
{
	switch(this->mLightColor)
	{
	case LightColor::Blue:
		return IMG_LIGHT_BLUE;
		break;

	case LightColor::Green:
		return IMG_LIGHT_GREEN;
		break;

	case LightColor::Grey:
		return IMG_LIGHT_GREY;
		break;

	case LightColor::Purple:
		return IMG_LIGHT_PURPLE;
		break;

	case LightColor::Red:
		return IMG_LIGHT_RED;
		break;
	}
}


//*************************************************************
// Methods
//*************************************************************
void FlashingLight::update()
{
	EntityEffect::update();
	if(this->getEntity()->isVisible())
	{
		this->updatePosition();
		this->updateFlash();
	}
}

void FlashingLight::updatePosition()
{
	this->mLightSprite.setPosition(	this->getEntity()->getScreenX() + this->getOffsetXRotate(), 
									this->getEntity()->getScreenY() + this->getEntity()->getRocking() + this->getOffsetYRotate());

	this->mFlashSprite.setPosition(	this->getEntity()->getScreenX() + this->getOffsetXRotate(), 
									this->getEntity()->getScreenY() + this->getEntity()->getRocking() + this->getOffsetYRotate());
}

void FlashingLight::updateFlash()
{
	if(this->mFlashClock.getElapsedTimeAsMilliseconds() > this->mFlashTick)
	{
		this->mFlashDraw = true;
		this->computeFlashTick();
		this->mFlashClock.restart();
	}
	else if(this->mFlashClock.getElapsedTimeAsMilliseconds() > FLASH_TIME)
	{
		this->mFlashDraw = false;
	}
}

void FlashingLight::draw()
{
	EntityEffect::draw();
	if(this->getEntity()->isVisible())
	{
		Resource::resource->getApp()->draw(this->mLightSprite);
		if(this->mFlashDraw)
			Resource::resource->getApp()->draw(this->mFlashSprite);
	}
}

void FlashingLight::computeFlashTick()
{
	this->mFlashTick = Tools::random(this->mFlashTickMin, this->mFlashTickMax);
}

