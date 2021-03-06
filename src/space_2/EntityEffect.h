#pragma once
#include "Resource.h"

class Entity;

class EntityEffect
{
public:
	// Constructor - Destructor
	EntityEffect(Entity* p_entity, Json::Value p_json);
	EntityEffect(Entity* p_entity, float p_offsetX, float p_offsetY, int p_size);
	~EntityEffect(void);

	// Getters - Setters
	Entity* getEntity();
	void setEntity(Entity* p_entity);

	float getOffsetX();
	void setOffsetX(float p_x);

	float getOffsetXRotate();
	void setOffsetXRotate(float p_x);

	float getOffsetY();
	void setOffsetY(float p_y);

	float getOffsetYRotate();
	void setOffsetYRotate(float p_y);

	int getSize();
	void setSize(int p_size);

	// Methods
	virtual void update();
	virtual void updateOffsetRotate();
	virtual void draw();


protected:
	// Attributs
	Entity* mEntity;
	sf::Vector2f mOffset;
	sf::Vector2f mOffsetRotate;
	int mSize;
};

