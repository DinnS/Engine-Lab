#pragma once
class AttributeComponent
{
private:


public:
	// Leveling 
	int level;
	int exp;
	int expNext;
	int attributePoints;

	// Attributes
	int strength;
	int agility;
	int intelligence;

	int vitality;
	int dexterity;

	// Stats
	int health;
	int healthMax;
	int damageMin;
	int damageMax;
	int accuracy;
	// int dodge;
	int defence;
	int luck;


	// Constructor / Destructor
	AttributeComponent(int level);
	virtual ~AttributeComponent();


	// Accessor
	

	// Functions
	std::string debugPrint() const;

	void gainExp(const int exp);

	void updateLevel();
	void updateStats(const bool reset);

	void update();


};

