#pragma once
#include "Character.h"
#include <vector>

#define moveSpeed 0.005f
#define waitTimerMax 100.f	//total time shopper stays in 'wait' state
#define eatTimerMax 600.f	//total timer shopper eats before leaving

class CCharacter_Shopper : public CCharacter
{
public:
	CCharacter_Shopper();
	CCharacter_Shopper(CHAR_STATES state, Vector2 pos);
	~CCharacter_Shopper();

	void Update(void);
	void RunFSM(void);
	void UpdateMovement(void);

	void AddWaypoint(Vector2);

	std::vector<Vector2> m_WaypointList;
private:
	int m_iWaypoint_Index;
	
	float m_fCountdownTimer;

};

