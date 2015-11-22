#include "Character.h"


CCharacter::CCharacter()
: pos(0.f, 0.f)
, dir(0.f, 0.f)
, state(STATE_DEFAULT)
{
}


CCharacter::~CCharacter()
{
}

float CCharacter::GetXPos(void)
{
	return pos.x;
}

float CCharacter::GetYPos(void)
{
	return pos.y;
}

Vector2 CCharacter::GetDir(void)
{
	return dir;
}

CCharacter::CHAR_STATES CCharacter::GetState(void)
{
	return state;
}

void CCharacter::SetXPos(float x)
{
	this->pos.x = x;
}

void CCharacter::SetYPos(float y)
{
	this->pos.y = y;
}

void CCharacter::SetDir(Vector2 dir)
{
	this->dir = dir;
}

void CCharacter::SetState(CCharacter::CHAR_STATES state)
{
	this->state = state;
}

void CCharacter::Update(void)
{
}

void CCharacter::RunFSM(void)
{
}

void CCharacter::UpdateMovement(void)
{
}

void CCharacter::receiveFlag(CCharacter::CHAR_FLAGS)
{

}

void CCharacter::AddWaypoint(Vector2)
{

}