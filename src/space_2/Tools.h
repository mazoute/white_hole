#pragma once
#include "Resource.h"
#include "sha1.h"

// Define
#define TOOLS_DEGREE_TO_RAD		0.0174532925
#define TOOLS_COS45				0.70710678118
#define TOOLS_SIN45				TOOLS_COS45

class Tools
{
public:
	// File
	static bool isFileExist(std::string p_fileName);

	// Format
	static int getIntFromString(std::string p_string);
	static long getLongFromString(std::string p_string);
	static float getFloatFromString(std::string p_string);
	static double getDoubleFromString(std::string p_string);
	static std::string buildStringWithInt(int p_int);
	static std::string buildStringWithLong(long p_long);
	static std::string buildStringWithFloat(float p_float);
	static std::string buildStringWithDouble(double p_double);
	static std::string formatNumber(int p_value);
	static std::string formatNumber(long p_value);
	static std::string formatNumber(float p_value);
	static std::string formatNumber(double p_value);
	static std::string formatDate(long p_value);
	static std::string getSpaceAfterColon();

	// Misc
	static bool isEqual(std::string p_string_1, std::string p_string_2);
	static bool isBetween(int p_toTest, int p_a, int p_b);

	// Sec
	static std::string sha1(std::string p_string);

	// Random
	static int random(int p_min, int p_max);
	static float random(float p_min, float p_max, int p_precision);
	static float randomPercentage(int p_min, int p_max);
	static int randomZeroToHundred();
	static float randomZeroToOne();
	static bool randomBool();

	// Trigo / maths
	static bool pointOnObject(int p_x, int p_y, int obj_g_x, int obj_g_y, int obj_d_x, int obj_d_y);
	static float degreeToRadian(float p_degree);
	static float getAngle(double p_xa, double p_ya, double p_xb, double p_yb);
	static double getAbsolute(double p_num);
	static float getAbsolute(float p_num);
	static int getAbsolute(int p_num);
	static long getAbsolute(long p_num);
	static sf::Vector2f getCoordinate(float p_angle, float p_hypo);
	static sf::Vector2i rotatePoint(sf::Vector2i p_point, float p_angle, sf::Vector2i p_center = sf::Vector2i(0, 0));			// p_angle in degree
	static sf::Vector2f rotatePoint(sf::Vector2f p_point, float p_angle, sf::Vector2f p_center = sf::Vector2f(0.f, 0.f));		// p_angle in degree


private:
	// Constructor - Destructor
	Tools(void);
	~Tools(void);
};
