#include "Character_Waiter.h"

#define movespeed 0.05f


/********************************************************************************
Constructor
********************************************************************************/
Character_Waiter::Character_Waiter() : foodready(false)
{
}

Character_Waiter::Character_Waiter(CHAR_STATES, Vector2 pos) : foodready(false)
{
}


/********************************************************************************
Destructor
********************************************************************************/
Character_Waiter::~Character_Waiter()
{
}


/********************************************************************************
Update Waiter's actions
********************************************************************************/
void Character_Waiter::Update(double dt)
{
	RunFSM();
	UpdateMovement();
}

/********************************************************************************
Update Waiter's states
********************************************************************************/
void Character_Waiter::RunFSM(void)
{
	switch (state)
	{
	case STATE_IDLE:
		break;
	case STATE_BRING_TO_SEAT:
		break;
	case STATE_TAKEFOOD:
		break;
	default:
		break;
	}
}

/********************************************************************************
Update Waiter's movements
********************************************************************************/
void Character_Waiter::UpdateMovement(void)
{
	pos += dir * movespeed;
}