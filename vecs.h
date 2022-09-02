#ifndef VECS_
#define VECS_
#define TYPE float

typedef struct
{
    TYPE x, y, z;
}Vec3T;

Vec3T Vec3TFromScalar(TYPE v) { return (Vec3T){v, v, v}; }
Vec3T Vec3TFrom(TYPE x, TYPE y, TYPE z) { return (Vec3T){x, y, z}; }
Vec3T Vec3TAdd(Vec3T v1, Vec3T v2) { v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
Vec3T Vec3TSub(Vec3T v1, Vec3T v2) { v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
Vec3T Vec3TMul(Vec3T v1, Vec3T v2) { v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }
Vec3T Vec3TDiv(Vec3T v1, Vec3T v2) { v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }

typedef struct
{
    float x, y, z;
}Vec3f;

Vec3f Vec3fFromScalar(float v) { return (Vec3f){v, v, v}; }
Vec3f Vec3fFrom(float x, float y, float z) { return (Vec3f){x, y, z}; }
Vec3f Vec3fAdd(Vec3f v1, Vec3f v2) { v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
Vec3f Vec3fSub(Vec3f v1, Vec3f v2) { v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
Vec3f Vec3fMul(Vec3f v1, Vec3f v2) { v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }
Vec3f Vec3fDiv(Vec3f v1, Vec3f v2) { v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }

typedef struct
{
    double x, y, z;
}Vec3d;

Vec3d Vec3dFromScalar(double v) { return (Vec3d){v, v, v}; }
Vec3d Vec3dFrom(double x, double y, double z) { return (Vec3d){x, y, z}; }
Vec3d Vec3dAdd(Vec3d v1, Vec3d v2) { v1.x += v2.x; v1.y += v2.y; v1.z += v2.z; return v1; }
Vec3d Vec3dSub(Vec3d v1, Vec3d v2) { v1.x -= v2.x; v1.y -= v2.y; v1.z -= v2.z; return v1; }
Vec3d Vec3dMul(Vec3d v1, Vec3d v2) { v1.x *= v2.x; v1.y *= v2.y; v1.z *= v2.z; return v1; }
Vec3d Vec3dDiv(Vec3d v1, Vec3d v2) { v1.x /= v2.x; v1.y /= v2.y; v1.z /= v2.z; return v1; }

double clamp_(double value, double min, double max)
{
    return value * (value >= min && value <= max) + max * (value > max) + min * (value < min);
}

double interp(double value, double min_from, double max_from, double min_to, double max_to)
{
    value = clamp_(value, min_from, max_from);
    double a = (min_to - max_to) / (min_from - max_from);
    double b = -(max_from * min_to - max_to * min_from) / (min_from - max_from);
    return a * value + b;
}
#endif