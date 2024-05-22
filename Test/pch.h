#pragma once

#include "framework.h"
#include "resource.h"
#include <windowsx.h>
#include <Windows.h>
#include <functional>
#include <xmmintrin.h>
#include <assert.h>
#include <vector>
#include <map>
#include <tuple>
#include <string>
#include <algorithm>

#define SMALL_NUMBER	(1.e-8f)

enum class BoundCheckResult : UINT32
{
	OUTSIDE = 0,
	INTERSECT,
	INSIDE,
};

class Math
{
public:
	static constexpr float PI = { 3.14159265358979323846f };
	static constexpr float TwoPI = { 2.f * PI };
	static constexpr float HalfPI = { 1.57079632679f };
	static constexpr float InvPI = { 0.31830988618f };
	static constexpr char InvalidHashName[] = "!@JK_INVALIDHASH#$";
	static const std::size_t InvalidHash;

public:

	FORCEINLINE static constexpr int TruncToInt(float InFloat)
	{
		return (int)InFloat;
	}

	FORCEINLINE static constexpr int RountToInt(float InFloat)
	{
		return TruncToInt(roundf(InFloat));
	}

	FORCEINLINE static constexpr int FloorToInt(float InFloat)
	{
		return TruncToInt(floorf(InFloat));
	}

	FORCEINLINE static int CeilToInt(float InFloat)
	{
		return TruncToInt(ceilf(InFloat));
	}

	FORCEINLINE static constexpr bool EqualsInTolerance(float InFloat1, float InFloat2, float InTolerance = SMALL_NUMBER)
	{
		return Math::Abs(InFloat1 - InFloat2) <= InTolerance;
	}

	template< class T>
	FORCEINLINE static constexpr T Lerp(const T& InSrc, const T& InDest, float InAlpha)
	{
		return (T)(InSrc + InAlpha * (InDest - InSrc));
	}

	template<class T>
	FORCEINLINE static constexpr T Square(const T InNum)
	{
		return InNum * InNum;
	}

	FORCEINLINE static constexpr float Deg2Rad(float InDegree)
	{
		return InDegree * PI / 180.f;
	}

	FORCEINLINE static constexpr float Rad2Deg(float InRadian)
	{
		return InRadian * 180.f * InvPI;
	}

	template<class T>
	FORCEINLINE static constexpr T Max(const T A, const T B)
	{
		return (A >= B) ? A : B;
	}

	template<class T>
	FORCEINLINE static constexpr T Min(const T A, const T B)
	{
		return (A <= B) ? A : B;
	}

	template< class T >
	FORCEINLINE static T constexpr Max3(const T A, const T B, const T C)
	{
		return Max(Max(A, B), C);
	}

	template< class T >
	FORCEINLINE static T constexpr Min3(const T A, const T B, const T C)
	{
		return Min(Min(A, B), C);
	}

	template<class T>
	FORCEINLINE static constexpr T Abs(const T A)
	{
		return (A >= (T)0) ? A : -A;
	}

	template<class T>
	FORCEINLINE static constexpr T Clamp(const T X, const T Min, const T Max)
	{
		return X < Min ? Min : X < Max ? X : Max;
	}

	// 언리얼 엔진 코드에서 가져옴. 지정된 각도에 대한 두 삼각함수를 함께 가져오는 함수. 
	static FORCEINLINE constexpr void GetSinCosRad(float& OutSin, float& OutCos, float InRadian)
	{
		// Copied from UE4 Source Code
		// Map Value to y in [-pi,pi], x = 2*pi*quotient + remainder.
		float quotient = (InvPI * 0.5f) * InRadian;
		if (InRadian >= 0.0f)
		{
			quotient = (float)((int)(quotient + 0.5f));
		}
		else
		{
			quotient = (float)((int)(quotient - 0.5f));
		}
		float y = InRadian - (2.0f * PI) * quotient;

		// Map y to [-pi/2,pi/2] with sin(y) = sin(Value).
		float sign = 0.f;
		if (y > HalfPI)
		{
			y = PI - y;
			sign = -1.0f;
		}
		else if (y < -HalfPI)
		{
			y = -PI - y;
			sign = -1.0f;
		}
		else
		{
			sign = +1.0f;
		}

		float y2 = y * y;

		// 11-degree minimax approximation
		OutSin = (((((-2.3889859e-08f * y2 + 2.7525562e-06f) * y2 - 0.00019840874f) * y2 + 0.0083333310f) * y2 - 0.16666667f) * y2 + 1.0f) * y;

		// 10-degree minimax approximation
		float p = ((((-2.6051615e-07f * y2 + 2.4760495e-05f) * y2 - 0.0013888378f) * y2 + 0.041666638f) * y2 - 0.5f) * y2 + 1.0f;
		OutCos = sign * p;
	}


	static FORCEINLINE constexpr void GetSinCos(float& OutSin, float& OutCos, float InDegree)
	{
		if (InDegree == 0.f)
		{
			OutSin = 0.f;
			OutCos = 1.f;
		}
		else if (InDegree == 90.f)
		{
			OutSin = 1.f;
			OutCos = 0.f;
		}
		else if (InDegree == 180.f)
		{
			OutSin = 0.f;
			OutCos = -1.f;
		}
		else if (InDegree == 270.f)
		{
			OutSin = -1.f;
			OutCos = 0.f;
		}
		else
		{
			GetSinCosRad(OutSin, OutCos, Math::Deg2Rad(InDegree));
		}
	}

	FORCEINLINE static float FMod(float X, float Y)
	{
		if (fabsf(Y) <= SMALL_NUMBER)
		{
			return 0.f;
		}

		const float quotient = (float)TruncToInt(X / Y);
		float intPortion = Y * quotient;
		if (fabsf(intPortion) > fabsf(X))
		{
			intPortion = X;
		}

		return (X - intPortion);
	}

	// 언리얼 엔진 코드에서 가져옴. 고속 역제곱근 공식
	FORCEINLINE static float InvSqrt(float InFloat)
	{
		// Performs two passes of Newton-Raphson iteration on the hardware estimate
		//    v^-0.5 = x
		// => x^2 = v^-1
		// => 1/(x^2) = v
		// => F(x) = x^-2 - v
		//    F'(x) = -2x^-3

		//    x1 = x0 - F(x0)/F'(x0)
		// => x1 = x0 + 0.5 * (x0^-2 - Vec) * x0^3
		// => x1 = x0 + 0.5 * (x0 - Vec * x0^3)
		// => x1 = x0 + x0 * (0.5 - 0.5 * Vec * x0^2)
		//
		// This final form has one more operation than the legacy factorization (X1 = 0.5*X0*(3-(Y*X0)*X0)
		// but retains better accuracy (namely InvSqrt(1) = 1 exactly).

		const __m128 fOneHalf = _mm_set_ss(0.5f);
		__m128 Y0, X0, X1, X2, FOver2;
		float temp;

		Y0 = _mm_set_ss(InFloat);
		X0 = _mm_rsqrt_ss(Y0);	// 1/sqrt estimate (12 bits)
		FOver2 = _mm_mul_ss(Y0, fOneHalf);

		// 1st Newton-Raphson iteration
		X1 = _mm_mul_ss(X0, X0);
		X1 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X1));
		X1 = _mm_add_ss(X0, _mm_mul_ss(X0, X1));

		// 2nd Newton-Raphson iteration
		X2 = _mm_mul_ss(X1, X1);
		X2 = _mm_sub_ss(fOneHalf, _mm_mul_ss(FOver2, X2));
		X2 = _mm_add_ss(X1, _mm_mul_ss(X1, X2));

		_mm_store_ss(&temp, X2);
		return temp;
	}
};

class Vector2
{
public:
	inline constexpr Vector2() = default;
	inline explicit constexpr Vector2(int InX, int InY) : X((float)InX), Y((float)InY) { }
	inline explicit constexpr Vector2(float InX, float InY) : X(InX), Y(InY) { }


	// 연산자 
	inline constexpr float operator[](unsigned char InIndex) const;
	inline constexpr float& operator[](unsigned char InIndex);
	inline constexpr Vector2 operator-() const;
	inline constexpr Vector2 operator*(float InScalar) const;
	inline constexpr Vector2 operator/(float InScalar) const;
	inline constexpr Vector2 operator*(const Vector2& InVector) const;
	inline constexpr Vector2 operator+(const Vector2& InVector) const;
	inline constexpr Vector2 operator-(const Vector2& InVector) const;
	inline constexpr Vector2& operator*=(float InScale);
	inline constexpr Vector2& operator/=(float InScale);
	inline constexpr Vector2& operator+=(const Vector2& InVector);
	inline constexpr Vector2& operator-=(const Vector2& InVector);



	//inline Vector2& operator=(const Vector3& InVector);
	// 멤버함수 
	inline float Size() const;
	inline constexpr float SizeSquared() const;
	void Normalize();
	[[nodiscard]] Vector2 GetNormalize() const;
	inline constexpr bool EqualsInTolerance(const Vector2& InVector, float InTolerance = SMALL_NUMBER) const;
	inline constexpr float Max() const;
	inline constexpr float Dot(const Vector2& InVector) const;
	inline float Angle() const;
	inline float AngleInDegree() const;
	inline Vector2 ToPolarCoordinate() const;
	inline constexpr Vector2 ToCartesianCoordinate() const;

	std::wstring Towstring() const;

	// 정적멤버변수 
	static const Vector2 UnitX;
	static const Vector2 UnitY;
	static const Vector2 One;
	static const Vector2 Zero;
	static constexpr unsigned char Dimension = 2;

	// 멤버변수
	union
	{
		struct
		{
			float X, Y;
		};

		float Scalars[Dimension] = { 0.f, 0.f };
	};
};

inline float Vector2::Size() const
{
	return sqrtf(SizeSquared());
}

inline constexpr float Vector2::SizeSquared() const
{
	return X * X + Y * Y;
}

inline void Vector2::Normalize()
{
	*this = GetNormalize();
}

inline constexpr float Vector2::operator[](unsigned char InIndex) const
{
	assert(InIndex < Dimension);
	return Scalars[InIndex];
}

inline constexpr float& Vector2::operator[](unsigned char InIndex)
{
	assert(InIndex < Dimension);
	return Scalars[InIndex];
}

inline constexpr Vector2 Vector2::operator-() const
{
	return Vector2(-X, -Y);
}

inline constexpr Vector2 Vector2::operator*(float InScalar) const
{
	return Vector2(X * InScalar, Y * InScalar);
}

inline constexpr Vector2 Vector2::operator/(float InScalar) const
{
	return Vector2(X / InScalar, Y / InScalar);
}

inline constexpr Vector2 Vector2::operator+(const Vector2& InVector) const
{
	return Vector2(X + InVector.X, Y + InVector.Y);
}

inline constexpr Vector2 Vector2::operator-(const Vector2& InVector) const
{
	return Vector2(X - InVector.X, Y - InVector.Y);
}

inline constexpr Vector2 Vector2::operator*(const Vector2& InVector) const
{
	return Vector2(X * InVector.X, Y * InVector.Y);
}

inline constexpr Vector2& Vector2::operator*=(float InScale)
{
	X *= InScale;
	Y *= InScale;
	return *this;
}

inline constexpr Vector2& Vector2::operator/=(float InScale)
{
	X /= InScale;
	Y /= InScale;
	return *this;
}

inline constexpr Vector2& Vector2::operator+=(const Vector2& InVector)
{
	X += InVector.X;
	Y += InVector.Y;
	return *this;
}

inline constexpr Vector2& Vector2::operator-=(const Vector2& InVector)
{
	X -= InVector.X;
	Y -= InVector.Y;
	return *this;
}


inline constexpr bool Vector2::EqualsInTolerance(const Vector2& InVector, float InTolerance) const
{
	return (Math::Abs(this->X - InVector.X) <= InTolerance) &&
		(Math::Abs(this->Y - InVector.Y) < InTolerance);
}

inline constexpr float Vector2::Max() const
{
	return Math::Max(X, Y);
}

inline constexpr float Vector2::Dot(const Vector2& InVector) const
{
	return X * InVector.X + Y * InVector.Y;
}

inline float Vector2::Angle() const
{
	return atan2f(Y, X);
}

inline float Vector2::AngleInDegree() const
{
	return Math::Rad2Deg(atan2f(Y, X));
}

inline Vector2 Vector2::ToPolarCoordinate() const
{
	return Vector2(Size(), Angle());
}

inline constexpr Vector2 Vector2::ToCartesianCoordinate() const
{
	// 극좌표계에서 X는 반지름, Y는 각(rad)으로 활용
	float sin = 0.f, cos = 0.f;
	Math::GetSinCosRad(sin, cos, Y);
	return Vector2(X * cos, X * sin);
}


class ScreenPoint
{
public:
	FORCEINLINE constexpr ScreenPoint() = default;
	FORCEINLINE explicit constexpr ScreenPoint(int InX, int InY) : X(InX), Y(InY) { }
	FORCEINLINE explicit constexpr ScreenPoint(float InX, float InY) : X(Math::FloorToInt(InX)), Y(Math::FloorToInt(InY)) { }
	FORCEINLINE explicit constexpr ScreenPoint(const Vector2& InPos) : ScreenPoint(InPos.X, InPos.Y) {}

	constexpr ScreenPoint GetHalf() const { return ScreenPoint(Math::FloorToInt(0.5f * X), Math::FloorToInt(0.5f * Y)); }
	constexpr float AspectRatio() const { return (float)X / (float)Y; } // Y축 기준으로 측정
	FORCEINLINE constexpr bool HasZero() const { return (X == 0 || Y == 0); }

	FORCEINLINE static constexpr ScreenPoint ToScreenCoordinate(const ScreenPoint& InScreenSize, const Vector2& InPos)
	{
		return ScreenPoint(InPos.X + InScreenSize.X * 0.5f, -InPos.Y + InScreenSize.Y * 0.5f);
	}

	FORCEINLINE constexpr Vector2 ToCartesianCoordinate(const ScreenPoint& InScreenSize)
	{
		// 데카르트 좌표계는 화면 중심을 기준으로 양,음 Y값을 모두 쓰지만 스크린 좌표계는 양의 Y값만 쓴다.
		// 따라서 스크린 좌표계의 중간값을 더해주어 데카르트 좌표계에서 양,음 Y 값을 제대로 출력할 수 있도록 한다.
		return Vector2(X - InScreenSize.X * 0.5f + 0.5f, -(Y + 0.5f) + InScreenSize.Y * 0.5f);
	}

	FORCEINLINE constexpr ScreenPoint operator-(const ScreenPoint& InPoint) const;
	FORCEINLINE constexpr ScreenPoint operator+(const ScreenPoint& InPoint) const;

	INT32 X = 0;
	INT32 Y = 0;
};

FORCEINLINE constexpr ScreenPoint ScreenPoint::operator-(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X - InPoint.X, Y - InPoint.Y);
}

FORCEINLINE constexpr ScreenPoint ScreenPoint::operator+(const ScreenPoint& InPoint) const
{
	return ScreenPoint(X + InPoint.X, Y + InPoint.Y);
}

class Color32
{
public:
	FORCEINLINE constexpr Color32() : R(0), G(0), B(0), A(0) { }
	FORCEINLINE explicit constexpr Color32(BYTE InR, BYTE InG, BYTE InB, BYTE InA = 255) : B(InB), G(InG), R(InR), A(InA) { }
	FORCEINLINE explicit constexpr Color32(UINT32 InColor) : ColorValue(InColor) { }

	FORCEINLINE constexpr const UINT32& GetColorRef() const { return ColorValue; }
	FORCEINLINE constexpr UINT32& GetColorRef() { return ColorValue; }

	FORCEINLINE constexpr bool operator==(const Color32& InC) const;
	FORCEINLINE constexpr bool operator!=(const Color32& InC) const;
	FORCEINLINE constexpr void operator+=(const Color32& InC);

	static const Color32 Error;

public:
	union
	{
		struct
		{
			BYTE B, G, R, A;
		};

		UINT32 ColorValue;
	};
};

FORCEINLINE constexpr bool Color32::operator==(const Color32& InC) const
{
	return GetColorRef() == InC.GetColorRef();
}

FORCEINLINE constexpr bool Color32::operator!=(const Color32& InC) const
{
	return GetColorRef() != InC.GetColorRef();
}

FORCEINLINE constexpr void Color32::operator+=(const Color32& InC)
{
	R = (BYTE)Math::Clamp((BYTE)R + (BYTE)InC.R, 0, 255);
	G = (BYTE)Math::Clamp((BYTE)G + (BYTE)InC.G, 0, 255);
	B = (BYTE)Math::Clamp((BYTE)B + (BYTE)InC.B, 0, 255);
	A = (BYTE)Math::Clamp((BYTE)A + (BYTE)InC.A, 0, 255);
}

class LinearColor
{
public:
	FORCEINLINE constexpr LinearColor() = default;
	FORCEINLINE explicit constexpr LinearColor(float InR, float InG, float InB, float InA = 1.f) : R(InR), G(InG), B(InB), A(InA) {}
	FORCEINLINE explicit constexpr LinearColor(const Color32& InColor32)
	{
		R = float(InColor32.R) * OneOver255;
		G = float(InColor32.G) * OneOver255;
		B = float(InColor32.B) * OneOver255;
		A = float(InColor32.A) * OneOver255;
	}

	FORCEINLINE constexpr Color32 ToColor32(const bool bSRGB = false) const;

	FORCEINLINE constexpr LinearColor operator+(const LinearColor& InColor) const;
	FORCEINLINE constexpr LinearColor operator-(const LinearColor& InColor) const;
	FORCEINLINE constexpr LinearColor operator*(const LinearColor& InColor) const;
	FORCEINLINE constexpr LinearColor operator*(float InScalar) const;
	FORCEINLINE constexpr LinearColor& operator*=(float InScale);
	FORCEINLINE constexpr LinearColor& operator/=(float InScale);
	FORCEINLINE constexpr LinearColor& operator+=(const LinearColor& InColor);
	FORCEINLINE constexpr LinearColor& operator-=(const LinearColor& InColor);

	FORCEINLINE constexpr bool operator==(const LinearColor& InColor) const;
	FORCEINLINE constexpr bool operator!=(const LinearColor& InColor) const;

	FORCEINLINE constexpr bool EqualsInRange(const LinearColor& InColor, float InTolerance = SMALL_NUMBER) const;

	static constexpr float OneOver255 = { 1.f / 255.f };
	static const LinearColor Error;
	static const LinearColor White;
	static const LinearColor Black;
	static const LinearColor Gray;
	static const LinearColor Silver;
	static const LinearColor WhiteSmoke;
	static const LinearColor LightGray;
	static const LinearColor DimGray;
	static const LinearColor Red;
	static const LinearColor Green;
	static const LinearColor Blue;
	static const LinearColor Yellow;
	static const LinearColor Cyan;
	static const LinearColor Magenta;

public:
	float R = 0.f;
	float G = 0.f;
	float B = 0.f;
	float A = 1.f;
};

FORCEINLINE constexpr Color32 LinearColor::ToColor32(const bool bSRGB) const
{
	float FloatR = Math::Clamp(R, 0.f, 1.f);
	float FloatG = Math::Clamp(G, 0.f, 1.f);
	float FloatB = Math::Clamp(B, 0.f, 1.f);
	float FloatA = Math::Clamp(A, 0.f, 1.f);

	return Color32(
		(int)(FloatR * 255.999f),
		(int)(FloatG * 255.999f),
		(int)(FloatB * 255.999f),
		(int)(FloatA * 255.999f)
	);
}

FORCEINLINE constexpr LinearColor LinearColor::operator+(const LinearColor& InColor) const
{
	return LinearColor(
		R + InColor.R,
		G + InColor.G,
		B + InColor.B,
		A + InColor.A
	);
}

FORCEINLINE constexpr LinearColor LinearColor::operator-(const LinearColor& InColor) const
{
	return LinearColor(
		R - InColor.R,
		G - InColor.G,
		B - InColor.B,
		A - InColor.A
	);
}

FORCEINLINE constexpr LinearColor LinearColor::operator*(const LinearColor& InColor) const
{
	return LinearColor(
		R * InColor.R,
		G * InColor.G,
		B * InColor.B,
		A * InColor.A
	);
}

FORCEINLINE constexpr LinearColor LinearColor::operator*(float InScalar) const
{
	return LinearColor(
		R * InScalar,
		G * InScalar,
		B * InScalar,
		A * InScalar
	);
}

FORCEINLINE constexpr LinearColor& LinearColor::operator*=(float InScale)
{
	R *= InScale;
	G *= InScale;
	B *= InScale;
	A *= InScale;
	return *this;
}

FORCEINLINE constexpr LinearColor& LinearColor::operator/=(float InScale)
{
	R /= InScale;
	G /= InScale;
	B /= InScale;
	A /= InScale;
	return *this;
}

FORCEINLINE constexpr LinearColor& LinearColor::operator+=(const LinearColor& InColor)
{
	R += InColor.R;
	G += InColor.G;
	B += InColor.B;
	A += InColor.A;
	return *this;
}

FORCEINLINE constexpr LinearColor& LinearColor::operator-=(const LinearColor& InColor)
{
	R -= InColor.R;
	G -= InColor.G;
	B -= InColor.B;
	A -= InColor.A;
	return *this;
}

FORCEINLINE constexpr bool LinearColor::operator==(const LinearColor& InColor) const
{
	return this->R == InColor.R && this->G == InColor.G && this->B == InColor.B && this->A == InColor.A;
}

FORCEINLINE constexpr bool LinearColor::operator!=(const LinearColor& InColor) const
{
	return this->R != InColor.R || this->G != InColor.G || this->B != InColor.B || this->A != InColor.A;
}

FORCEINLINE constexpr bool LinearColor::EqualsInRange(const LinearColor& InColor, float InTolerance) const
{
	return (Math::Abs(this->R - InColor.R) < InTolerance) &&
		(Math::Abs(this->G - InColor.G) < InTolerance) &&
		(Math::Abs(this->B - InColor.B) < InTolerance) &&
		(Math::Abs(this->A - InColor.A) < InTolerance);
}

struct WindowInfo
{
	HWND hwnd; // 출력 윈도우
	int width; // 너비
	int height; // 높이
	bool windowed; // 창모드 or FUllScreen

public:
	ScreenPoint ToScreenPoint() { return ScreenPoint(width, height); }
};

class RendererInterface
{
public:
	virtual bool Init(const WindowInfo& window) = 0;
	virtual void Shutdown() = 0;
	virtual bool IsInitialized() const = 0;

	virtual void Clear(const LinearColor& InClearColor) = 0;
	virtual void BeginFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void DrawPoint(const Vector2& InVectorPos, const LinearColor& InColor) = 0;
	virtual void DrawPoint(const ScreenPoint& InScreenPos, const LinearColor& InColor) = 0;
	virtual void DrawLine(const Vector2& InStartPos, const Vector2& InEndPos, const LinearColor& InColor) = 0;
	//virtual void DrawLine(const Vector4& InStartPos, const Vector4& InEndPos, const LinearColor& InColor) = 0;

	virtual float GetDepthBufferValue(const ScreenPoint& InScreenPos) const = 0;
	virtual void SetDepthBufferValue(const ScreenPoint& InScreenPos, float InDepthValue) = 0;

	virtual void DrawFullVerticalLine(int InX, const LinearColor& InColor) = 0;
	virtual void DrawFullHorizontalLine(int InY, const LinearColor& InColor) = 0;

	virtual void PushStatisticText(std::wstring&& InText) = 0;
	virtual void PushStatisticTexts(std::vector<std::wstring>&& InTexts) = 0;
};

