#include "MathUtils.h"

#include <cmath>

using namespace Waffle;

Mat3 Transform::AsMatrix()
{
	float angleRads = Rotation * DEG_TO_RAD;
	float cos0 = cos(angleRads);
	float sin0 = sin(angleRads);

	Mat3 rotation;
	rotation.Row0.X = cos0; rotation.Row0.Y = -sin0;
	rotation.Row1.X = sin0; rotation.Row1.Y =  cos0;

	Mat3 scale;
	scale.Row0.X = Scale.X;
	scale.Row1.Y = Scale.Y;

	Mat3 translate;
	translate.Row0.Z = Position.X;
	translate.Row1.Z = Position.Y;

	return translate * rotation * scale;
}
