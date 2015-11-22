#pragma once

#include "Vector2.h"
class CCharacter
{
public:
	CCharacter();
	~CCharacter();

	enum CHAR_STATES
	{
		STATE_DEFAULT = 0,	//default state when character is initialised
		//Shopping character states
		STATE_PATROL,		//'Patrol' state for shopping characters - patrol to several shops(patrol points)
		STATE_STARE,		//'State' state for shopping characters - 'state' at items in shop for a duration before moving on
		STATE_WALK_TO_RES,	//State for shopping characters to walk to a restaraunt
		STATE_WAIT_TO_SIT,	//State for shopping characters to wait to be seated by a waiter
		STATE_WAIT_FOR_FOOD,//State for shopping characters to wait for food to be ready
		STATE_EAT,			//State for shopping characters - character 'eats' for a duration before going back to patrol
		STATE_FOLLOW,		//State for shopping characters - character follows waiter to a empty seat
		//Waiter character states
		/* Enter states here */
		NUM_STATES,
	};

	enum CHAR_FLAGS
	{
		FLAG_REACHED,
		NUM_FLAGS,
	};

	virtual float GetXPos(void);
	virtual float GetYPos(void);
	virtual Vector2 GetDir(void);
	virtual CHAR_STATES GetState(void);

	virtual void SetXPos(float);
	virtual void SetYPos(float);
	virtual void SetDir(Vector2);
	virtual void SetState(CHAR_STATES);

	virtual void Update(void);
	virtual void RunFSM(void);
	virtual void UpdateMovement(void);

	//exprimental
	virtual void receiveFlag(CHAR_FLAGS);

	//special
	virtual void AddWaypoint(Vector2);

protected:
	Vector2 pos;		//Character position
	Vector2 dir;		//Character facing direction
	CHAR_STATES state;	// Character state
	float m_fSize;		//Size of character to be rendered
};

