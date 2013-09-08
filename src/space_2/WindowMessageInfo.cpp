#include "WindowMessageInfo.h"


//*************************************************************
// Constructor - Destructor
//*************************************************************
WindowMessageInfo::WindowMessageInfo( std::string p_title, std::string p_message ) : WindowMessage(p_title, p_message, SpriteParameterFactory::getSpriteParameterIcon64X64(false), IC_64X64_INFO, IC_16X16_MAIL)
{

}

WindowMessageInfo::~WindowMessageInfo(void)
{
}
