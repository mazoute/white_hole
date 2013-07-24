#pragma once
#include "Resource.h"
#include "KeyValueFile.h"

// Define
#define ITEMTYPE_CONFIG_ID				"id"
#define ITEMTYPE_CONFIG_NAME			"name"
#define ITEMTYPE_CONFIG_DESCRIPTION		"description"
#define ITEMTYPE_CONFIG_PARENT			"itemtype_parent"
#define ITEMTYPE_CONFIG_LEAF			"itemtype_leaf"

#define ITEMTYPE_AMMO_MISSILE			33
#define ITEMTYPE_AMMO_ROQUET			34

class ItemType
{
public:
	// Constructor - Destructor
	ItemType(KeyValueFile* p_config);
	~ItemType(void);

	// Getters - Setters
	long getId();
	void setId(long p_id);

	std::string getName();
	void setName(std::string p_name);

	std::string getDescription();
	void setDescription(std::string p_description);

	ItemType *getParent();
	void setParent(ItemType *p_parent);

	bool isLeaf();
	void setLeaf(bool p_isleaf);

	// Methode
	void loadFromConfig(KeyValueFile* p_config);


private:
	// Attributs
	long mId;
	std::string mName;
	std::string mDescription;
	ItemType *mParent;
	bool mLeaf;
};
