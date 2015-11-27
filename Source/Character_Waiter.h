#pragma once
#include "Character.h"

class Character_Waiter : public CCharacter
{
public:
	Character_Waiter();
	Character_Waiter(CHAR_STATES, Vector2 pos);
	~Character_Waiter();

	void Update(double dt);
	void RunFSM(void);
	void UpdateMovement(void);

private:
	bool foodready;
};

