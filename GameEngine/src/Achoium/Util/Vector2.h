#pragma once

namespace ac
{
	class Vector2
	{
	public:
		double x;
		double y;
		Vector2(double x, double y);
		Vector2();
		Vector2(const Vector2& other);
		Vector2& operator=(const Vector2& other);
		Vector2 operator+(const Vector2& other) const;
		Vector2 operator-(const Vector2& other) const;
		Vector2 operator*(double scalar) const;
		Vector2 operator/(double scalar) const;
		Vector2& operator+=(const Vector2& other);
		Vector2& operator-=(const Vector2& other);
		Vector2& operator*=(double scalar);
		Vector2& operator/=(double scalar);
		bool operator==(const Vector2& other) const;
		bool operator!=(const Vector2& other) const;

	};
}