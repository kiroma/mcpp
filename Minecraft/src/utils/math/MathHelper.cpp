#include "MathHelper.h"

float MathHelper::Lerp(float s, float e, float t)
{ return s + (e - s) * t; }

float MathHelper::Blerp(float c00, float c10, float c01, float c11, float x, float y)
{ return Lerp(Lerp(c00, c10, x), Lerp(c01, c11, x), y); }
