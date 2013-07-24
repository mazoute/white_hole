#include <fstream>
#include "Tools.h"

using namespace std;


//*************************************************************
// Constructor - Destructor
//*************************************************************
Tools::Tools(void)
{
}

Tools::~Tools(void)
{
}


//*************************************************************
// Methode
//*************************************************************
bool Tools::isFileExist( std::string p_fileName )
{
	ifstream file(p_fileName);
	if (file.is_open())
	{
		file.close();
		return true;
	}
	return false;
}

int Tools::getIntFromString( std::string p_string )
{
	return (int)Tools::getLongFromString(p_string);
}

long Tools::getLongFromString( std::string p_string )
{
	if(p_string != "")
	{
		char * pEnd;
		return strtol(p_string.data(), &pEnd, 10);
	}
	return -1;
}

float Tools::getFloatFromString( std::string p_string )
{
	return (float)Tools::getDoubleFromString(p_string);
}

double Tools::getDoubleFromString( std::string p_string )
{
	if(p_string != "")
	{
		char * pEnd;
		return strtod(p_string.data(), &pEnd);
	}
	return -1;
}

std::string Tools::buildStringWithInt( int p_int )
{
	std::stringstream ss;
	ss << p_int;
	return ss.str();
}

std::string Tools::buildStringWithLong( long p_long )
{
	std::stringstream ss;
	ss << p_long;
	return ss.str();
}

std::string Tools::buildStringWithFloat( float p_float )
{
	std::stringstream ss;
	ss << p_float;
	return ss.str();
}

std::string Tools::buildStringWithDouble( double p_double )
{
	std::stringstream ss;
	ss << p_double;
	return ss.str();
}

std::string Tools::formatNumber( int p_value )
{
	return Tools::formatNumber((long)p_value);
}

std::string Tools::formatNumber( long p_value )
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << p_value;
	return ss.str();
}

std::string Tools::formatNumber( float p_value )
{
	return Tools::formatNumber((double)p_value);
}

std::string Tools::formatNumber( double p_value )
{
	std::stringstream ss;
	ss.imbue(std::locale(""));
	ss << std::fixed << p_value;
	return ss.str();
}

std::string Tools::formatDate( long p_value )
{
	return "This function must be implemented brawbrawbraw";
}

bool Tools::isEqual( std::string p_string_1, std::string p_string_2 )
{
	return p_string_1 == p_string_2;
}

std::string Tools::sha1( std::string p_string )
{
	unsigned char hash[20];
	char hexstring[41];

	int strByteLenght = (int)strlen(p_string.data());
	sha1::calc(p_string.data(), strByteLenght, hash);
	sha1::toHexString(hash, hexstring);

	return std::string(hexstring);
}

int Tools::random( int p_min, int p_max )
{
	if(p_min == p_max)
		return p_min;

	return p_min + (rand() % (int)(p_max - p_min + 1));
}

float Tools::random( float p_min, float p_max, int p_precision )
{
	float precisionFinal = pow(10.f, p_precision);
	int minInterger = (int)(p_min * precisionFinal);
	int maxInteger = (int)(p_max * precisionFinal);
	return (float)Tools::random(minInterger, maxInteger) / precisionFinal;
}

float Tools::randomPercentage( int p_min, int p_max )
{
	return ((float)Tools::random(p_min, p_max)) / 100.f;
}

int Tools::randomZeroToHundred()
{
	return Tools::random(0, 100);
}

float Tools::randomZeroToOne()
{
	return Tools::randomPercentage(0, 100);
}

bool Tools::randomBool()
{
	return 0 == (rand() % 2);
}

float Tools::getAngle( double p_xa, double p_ya, double p_xb, double p_yb )
{
	double diffX = p_xb - p_xa;
	double diffY = p_yb - p_ya;

	if(diffY <= 0 && diffX >= 0)
		return (float)(atan2(Tools::getAbsolute(diffX), Tools::getAbsolute(diffY)) * 180 / RESOURCE_PI);
	else if(diffY >= 0 && diffX >= 0)
		return 90.f + ((float)atan2(Tools::getAbsolute(diffY), Tools::getAbsolute(diffX)) * 180 / RESOURCE_PI);
	else if(diffY >= 0 && diffX <= 0)
		return 180.f + ((float)atan2(Tools::getAbsolute(diffX), Tools::getAbsolute(diffY)) * 180 / RESOURCE_PI);
	else
		return 270.f + ((float)atan2(Tools::getAbsolute(diffY), Tools::getAbsolute(diffX)) * 180 / RESOURCE_PI);
}

double Tools::getAbsolute( double p_num )
{
	if(p_num < 0)
		p_num = -p_num;
	return p_num;
}

float Tools::getAbsolute( float p_num )
{
	if(p_num < 0)
		p_num = -p_num;
	return p_num;
}

int Tools::getAbsolute( int p_num )
{
	if(p_num < 0)
		p_num = -p_num;
	return p_num;
}

long Tools::getAbsolute( long p_num )
{
	if(p_num < 0)
		p_num = -p_num;
	return p_num;
}

std::string Tools::getSpaceAfterColon()
{
	return "  ";
}

bool Tools::isBetween( int p_toTest, int p_a, int p_b )
{
	if(p_a < p_b)
		return p_toTest >= p_a && p_toTest <= p_b;
	else if(p_b < p_a)
		return p_toTest >= p_b && p_toTest <= p_a;
	else
		return p_toTest == p_a;
}

sf::Vector2f Tools::getCoordinate( float p_angle, float p_hypo )
{
	sf::Vector2f returnValue;
	if(p_angle <= 45)
	{
		returnValue.x = Tools::getAbsolute(sin(Tools::degreeToRadian(p_angle))) * p_hypo;
		returnValue.y = -Tools::getAbsolute(cos(Tools::degreeToRadian(p_angle))) * p_hypo;
	}
	else if(p_angle <= 90)
	{
		returnValue.x = Tools::getAbsolute(cos(Tools::degreeToRadian(90 - p_angle))) * p_hypo;
		returnValue.y = -Tools::getAbsolute(sin(Tools::degreeToRadian(90 - p_angle))) * p_hypo;
	}
	else if(p_angle <= 135)
	{
		returnValue.x = Tools::getAbsolute(cos(Tools::degreeToRadian(p_angle - 90))) * p_hypo;
		returnValue.y = Tools::getAbsolute(sin(Tools::degreeToRadian(p_angle - 90))) * p_hypo;
	}
	else if(p_angle <= 180)
	{
		returnValue.x = Tools::getAbsolute(sin(Tools::degreeToRadian(180 - p_angle))) * p_hypo;
		returnValue.y = Tools::getAbsolute(cos(Tools::degreeToRadian(180 - p_angle))) * p_hypo;
	}
	else if(p_angle <= 225)
	{
		returnValue.x = -Tools::getAbsolute(sin(Tools::degreeToRadian(p_angle - 180))) * p_hypo;
		returnValue.y = Tools::getAbsolute(cos(Tools::degreeToRadian(p_angle - 180))) * p_hypo;
	}
	else if(p_angle <= 270)
	{
		returnValue.x = -Tools::getAbsolute(cos(Tools::degreeToRadian(270 - p_angle))) * p_hypo;
		returnValue.y = Tools::getAbsolute(sin(Tools::degreeToRadian(270 - p_angle))) * p_hypo;
	}
	else if(p_angle <= 315)
	{
		returnValue.x = -Tools::getAbsolute(cos(Tools::degreeToRadian(p_angle - 270))) * p_hypo;
		returnValue.y = -Tools::getAbsolute(sin(Tools::degreeToRadian(p_angle - 270))) * p_hypo;
	}
	else if(p_angle <= 360)
	{
		returnValue.x = -Tools::getAbsolute(sin(Tools::degreeToRadian(360 - p_angle))) * p_hypo;
		returnValue.y = -Tools::getAbsolute(cos(Tools::degreeToRadian(360 - p_angle))) * p_hypo;
	}
	return returnValue;
}

float Tools::degreeToRadian( float p_degree )
{
	return p_degree * TOOLS_DEGREE_TO_RAD;
}

bool Tools::pointOnObject( int p_x, int p_y, int obj_g_x, int obj_g_y, int obj_d_x, int obj_d_y )
{
	return (p_x >= obj_g_x && p_x <= obj_d_x && p_y >= obj_g_y && p_y <= obj_d_y);
}

sf::Vector2i Tools::rotatePoint( sf::Vector2i p_point, float p_angle, sf::Vector2i p_center )
{
	float angleRadian = Tools::degreeToRadian(p_angle);
	sf::Vector2i newPoint;
	newPoint.x = (int)(cos(angleRadian) * (p_point.x - p_center.x) - sin(angleRadian) * (p_point.y - p_center.y) + p_center.x);
	newPoint.y = (int)(sin(angleRadian) * (p_point.x - p_center.x) + cos(angleRadian) * (p_point.y - p_center.y) + p_center.y);
	return newPoint;
}

sf::Vector2f Tools::rotatePoint( sf::Vector2f p_point, float p_angle, sf::Vector2f p_center )
{
	float angleRadian = Tools::degreeToRadian(p_angle);
	sf::Vector2f newPoint;
	newPoint.x = (float)(cos(angleRadian) * (p_point.x - p_center.x) - sin(angleRadian) * (p_point.y - p_center.y) + p_center.x);
	newPoint.y = (float)(sin(angleRadian) * (p_point.x - p_center.x) + cos(angleRadian) * (p_point.y - p_center.y) + p_center.y);
	return newPoint;
}



