#pragma once

#define PI 3.14159265359f
#define DEG_TO_RAD (PI/180.0f)
#define RAD_TO_DEG (180.0f/PI)
#define IS_POWER_OF_2(n) (n && !(n & (n - 1)))

#include <cmath>

#define clamp(n,mi,ma) fmin(ma, fmax(mi, n))
#define saturate(n) fmin(1.0f, fmax(0.0f, n))

namespace Waffle
{
	struct Vec2
	{
		Vec2() 
			: X(0.0f)
			, Y(0.0f) 
		{}
		Vec2(float _x, float _y)
			:X(_x)
			,Y(_y)
		{}
		union
		{
			float Data[2];
			struct { float X; float Y; };
			struct { float R; float G; };
		};

		Vec2 operator+ (const Vec2& other)
		{
			return Vec2(this->X + other.X, this->Y + other.Y);
		}

		Vec2 operator- (const Vec2& other)
		{
			return Vec2(this->X - other.X, this->Y - other.Y);
		}

		Vec2 operator* (const float& other)
		{
			return Vec2(this->X * other, this->Y * other);
		}
	};

	struct Vec3
	{
		Vec3()
			: X(0.0f)
			, Y(0.0f)
			, Z(0.0f)
		{}
		Vec3(float _x, float _y, float _z)
			: X(_x)
			, Y(_y)
			, Z(_z)
		{}
		union
		{
			float Data[3];
			struct { float X; float Y; float Z; };
			struct { float R; float G; float B; };
		};
		Vec3 operator+(const Vec3& other)
		{
			Vec3 res;
			res.X = this->X + other.X;
			res.Y = this->Y + other.Y;
			res.Z = this->Z + other.Z;
			return res;
		}
	};

	static float Dot(const Vec3& a, const Vec3& b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
	}

	static float Dot(const Vec2& a, const Vec2& b)
	{
		return a.X * b.X + a.Y * b.Y;
	}

	static float Length(const Vec2& a)
	{
		return sqrt(a.X*a.X + a.Y*a.Y);
	}

	static Vec2 Normalize(const Vec2& a)
	{
		float len = Length(a);
		return Vec2(a.X / len, a.Y / len);
	}

	struct Mat3
	{
		Mat3()
			: Row0(1.0f, 0.0f, 0.0f)
			, Row1(0.0f, 1.0f, 0.0f)
			, Row2(0.0f, 0.0f, 1.0f)
		{
		}
		union
		{
			union
			{
				float Data[3][3];
				struct { Vec3 Row0; Vec3 Row1; Vec3 Row2; };
			};

		};
		Mat3 operator* (const Mat3& other)
		{
			Mat3 ret;
			ret.Row0.X = Dot(this->Row0, Vec3(other.Row0.X, other.Row1.X, other.Row2.X));
			ret.Row0.Y = Dot(this->Row0, Vec3(other.Row0.Y, other.Row1.Y, other.Row2.Y));
			ret.Row0.Z = Dot(this->Row0, Vec3(other.Row0.Z, other.Row1.Z, other.Row2.Z));

			ret.Row1.X = Dot(this->Row1, Vec3(other.Row0.X, other.Row1.X, other.Row2.X));
			ret.Row1.Y = Dot(this->Row1, Vec3(other.Row0.Y, other.Row1.Y, other.Row2.Y));
			ret.Row1.Z = Dot(this->Row1, Vec3(other.Row0.Z, other.Row1.Z, other.Row2.Z));

			ret.Row2.X = Dot(this->Row2, Vec3(other.Row0.X, other.Row1.X, other.Row2.X));
			ret.Row2.Y = Dot(this->Row2, Vec3(other.Row0.Y, other.Row1.Y, other.Row2.Y));
			ret.Row2.Z = Dot(this->Row2, Vec3(other.Row0.Z, other.Row1.Z, other.Row2.Z));
			return ret;
		}
	};

	struct Color
	{
		Color()
			: R(1.0f)
			, G(1.0f)
			, B(1.0f)
			, A(1.0f)
		{}
		Color(float _r, float _g, float _b, float _a )
			: R(_r)
			, G(_g)
			, B(_b)
			, A(_a)
		{}
		float R;
		float G;
		float B;
		float A;
	};

	struct Transform
	{
		Transform()
			: Position(0.0f, 0.0f)
			, Scale(1.0f, 1.0f)
			, Rotation(0.0f)
		{}
		Vec2 Position;
		Vec2 Scale;
		// Rotation in degrees
		float Rotation; 

		Mat3 AsMatrix();
	};
}