#pragma once
#include "Resource.h"
#include "Race.h"
#include "Job.h"
#include "User.h"
#include "Levelable.h"
#include "Clock.h"

class CharacterShip;
class CharacterBank;
class SkillCharacter;

// Define
#define CHARACTER_SKILL_COUNT				9

class Character : public Levelable
{
public:
	// Constructor - Destructor
	Character(User* p_user = NULL, Json::Value json = NULL);
	~Character(void);
	void destroyShips();

	// Getters - Setters
	long getIdCharacter();
	void setIdCharacter(long p_id);

	std::string getName();
	void setName(std::string p_name);

	std::string getAvatarId();
	void setAvatarId(std::string p_id);

	bool hasEnoughCredit(double p_credit);
	double getCredit();
	void setCredit(double p_credit);

	long getDateCreation();
	void setDateCreation(long p_date);

	long getTimePlayedReal();
	long getTimePlayed();
	void setTimePlayed(long p_time);
	
	bool isAlive();
	void setAlive(bool p_alive);

	bool hasHangarSpaceFree();
	int getHangarSpaceOccuped();
	int getHangarSpaceFree();
	int getHangarSpace();
	void setHangarSpace(int p_space);

	bool hasRace();
	Race* getRace();
	void setRace(Race* p_race);

	bool hasJob();
	Job* getJob();
	void setJob(Job* p_job);

	User* getUser();
	void setUser(User *p_user);

	bool hasSkillPoints();
	int getSkillPoints();
	void setSkillPoints(int p_points);

	SkillCharacter *getSkillCharacter(int p_skillId);
	SkillCharacter *getSkillCharacterByIndex(int index);

	int getBankCount();
	CharacterBank* getBank(int p_index);
	CharacterBank* getBankByNumber(int p_number);

	int getShipCount();
	CharacterShip *getShip(int p_index);

	CharacterShip *getShipPiloted();
	void setShipPiloted(CharacterShip *p_ship);

	// Methods
	void init();
	void update();
	void updateTime();

	void addShip(CharacterShip* p_ship); 
	void removeShip(CharacterShip* p_ship);
	void addBank(CharacterBank* p_bank);

	void incCredit(double p_inc);
	void decCredit(double p_dec);
	void incSkillPoints();
	void incSkillPoints(int p_inc);
	void decSkillPoints();
	void incHangarSpace();

	void incLevel();
	void incExperience(long p_inc);

	void notifyTimePlayedChanged();
	void notifySkillPointsChanged();
	void notifyHangarSpaceChanged();
	void notifyExperienceChanged();
	void notifyLevelChanged();
	void notifyCreditChanged();

	void createBase();
	void loadFromJson(Json::Value json);
	Json::Value saveToJson();


private:
	// Attributs
	bool mLoaded;
	long mIdCharacter;
	std::string mName;
	std::string mAvatarId;
	int mSkillPoints;
	double mCredit;
	long mDateCreation;
	long mTimePlayed;
	float mTimeBuffer;
	mks::Clock mTimeBufferClock;
	bool mAlive;
	int mHangarSpace;
	Race* mRace;
	Job* mJob;
	User* mUser; // Memory not managed here so dont need to delete this ptr
	SkillCharacter* mSkillCharacters[CHARACTER_SKILL_COUNT];

	std::vector<CharacterBank*> mBanks;
	std::vector<CharacterShip*> mShips;
	CharacterShip *mShipPiloted;
};

