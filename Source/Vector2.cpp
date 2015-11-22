#include "Vector2.h"
#include <cmath>

Vector2::Vector2( float a, float b )
	: x(a)
	, y(b)
{
}

Vector2::Vector2( const Vector2 &rhs )
	: x(rhs.x)
	, y(rhs.y)
{
}

void Vector2::Set( float a, float b )
{
	x = a;
	y = a;
}

Vector2 Vector2::operator+( const Vector2& rhs ) const
{
	return Vector2(x + rhs.x, y + rhs.y);
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	x = +rhs.x;
	y = +rhs.y;
	return *this;
}


Vector2 Vector2::operator-( const Vector2& rhs ) const
{
	return Vector2(this->x - rhs.x, this->y - rhs.y);
}

Vector2 Vector2::operator-( void ) const
{
	return Vector2(-x, -y);
}

Vector2 Vector2::operator*( float scalar ) const
{
	return Vector2(scalar * x, scalar * y);
}

float Vector2::Length( void ) const
{
	return sqrt((x * x) + (y * y));
}

float Vector2::Dot( const Vector2& rhs ) const
{
	return 0;
}

Vector2 Vector2::Normalized( void ) const throw( DivideByZero )
{
	float d = Length();
	if(d <= Math::EPSILON && -d <= Math::EPSILON)
	  throw DivideByZero();
	return Vector2(x / d, y / d);
}
