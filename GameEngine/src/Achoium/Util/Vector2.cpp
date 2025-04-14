#include "acpch.h"
#include "Vector2.h"
namespace ac
{
	Vector2::Vector2(double x, double y) : x(x), y(y) {}

	Vector2::Vector2() : x(0), y(0) {}

	Vector2::Vector2(const Vector2& other) : x(other.x), y(other.y) {}

	Vector2& Vector2::operator=(const Vector2& other)
	{
		if (this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return Vector2(x + other.x, y + other.y);
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return Vector2(x - other.x, y - other.y);
	}

	Vector2 Vector2::operator*(double scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}

	Vector2 Vector2::operator/(double scalar) const
	{
		if (scalar != 0)
			return Vector2(x / scalar, y / scalar);
		else
			throw std::invalid_argument("Division by zero");
	}

	Vector2& Vector2::operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2& Vector2::operator-=(const Vector2& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2& Vector2::operator*=(double scalar)
	{
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2& Vector2::operator/=(double scalar)
	{
		if (scalar != 0)
			x /= scalar, y /= scalar;
		else
			throw std::invalid_argument("Division by zero");

		return *this;
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		return x == other.x && y == other.y;
	}

	bool Vector2::operator!=(const Vector2& other) const
	{
		return !(*this == other);
	}
}