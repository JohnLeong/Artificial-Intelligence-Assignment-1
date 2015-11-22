#include "Character_Shopper.h"

CCharacter_Shopper::CCharacter_Shopper()
: m_iWaypoint_Index(0)
, m_fCountdownTimer(0.f)
{
}

CCharacter_Shopper::CCharacter_Shopper(CHAR_STATES state, Vector2 pos)
: m_iWaypoint_Index(0)
, m_fCountdownTimer(0.f)
{
	AddWaypoint(pos);

}


CCharacter_Shopper::~CCharacter_Shopper()
{
	//Clear patrol points
	while (m_WaypointList.size() > 0)
	{
		m_WaypointList.pop_back();
	}
	m_WaypointList.clear();
}

void CCharacter_Shopper::Update(void)
{
	RunFSM();//Update characters based on states
	UpdateMovement();//Update character positions based on direction
}

void CCharacter_Shopper::RunFSM(void)
{
	switch (state)
	{
	case STATE_PATROL:
		dir = (m_WaypointList[m_iWaypoint_Index] - pos).Normalized();
		if (true)//markof condition here
		{
			state = STATE_STARE;
			m_fCountdownTimer = waitTimerMax;
		}
		else
		{
			state = STATE_WALK_TO_RES;
		}
		break;
	case STATE_STARE:
		m_fCountdownTimer -= 1.f;
		if (m_fCountdownTimer <= 0.f)
			state = STATE_PATROL;
		break;
	case STATE_WALK_TO_RES:
		break;
	case STATE_WAIT_TO_SIT:
		break;
	case STATE_WAIT_FOR_FOOD:
		break;
	case STATE_EAT:
		m_fCountdownTimer -= 1.f;
		if (m_fCountdownTimer <= 0.f)
			state = STATE_PATROL;
		break;
	case STATE_FOLLOW:
		break;
	default:
		break;
	}
}

void CCharacter_Shopper::UpdateMovement(void)
{
	pos = pos + (dir * moveSpeed);
	//pos += dir * moveSpeed;
}

void CCharacter_Shopper::AddWaypoint(Vector2 waypoint)
{
	m_WaypointList.push_back(waypoint);
}