#ifndef __VEC3_H__
#define __VEC3_H__

#include <math.h>
#include <stdlib.h>
#include <iostream>

#include "rangen.h"

class vec3 {
    public:
        vec3() { e[0] = e[1] = e[2] = 0.0; }
        vec3(float e0, float e1, float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }        
        
        inline float x() const { return e[0]; }
        inline float y() const { return e[1]; }
        inline float z() const { return e[2]; }
        inline float r() const { return e[0]; }
        inline float g() const { return e[1]; }
        inline float b() const { return e[2]; }
        
        inline const vec3 &operator+() const { return *this; };
        inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); };
        inline float operator[](int i) const { return e[i]; };
        inline float &operator[](int i) { return e[i]; };
        
        inline vec3 &operator+=(const vec3 &v2);
        inline vec3 &operator-=(const vec3 &v2);
        inline vec3 &operator*=(const vec3 &v2);
        inline vec3 &operator/=(const vec3 &v2);
        inline vec3 &operator*=(const float t);
        inline vec3 &operator/=(const float t);
        
        inline float length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); };
        inline float squared_length() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; };
        inline void make_unit_vector();
    
    
        float e[3];
};

vec3 &vec3::operator+=(const vec3 &v2)
{
    e[0] += v2.e[0];
    e[1] += v2.e[1];
    e[2] += v2.e[2];
    
    return *this;
}

vec3 &vec3::operator-=(const vec3 &v2)
{
    e[0] -= v2.e[0];
    e[1] -= v2.e[1];
    e[2] -= v2.e[2];
    
    return *this;    
}

vec3 &vec3::operator*=(const vec3 &v2)
{
    e[0] *= v2.e[0];
    e[1] *= v2.e[1];
    e[2] *= v2.e[2];
    
    return *this;
}

vec3 &vec3::operator/=(const vec3 &v2)
{
    e[0] /= v2.e[0] != 0.0 ? v2.e[0] : 0.0;
    e[1] /= v2.e[1] != 0.0 ? v2.e[1] : 0.0;
    e[2] /= v2.e[2] != 0.0 ? v2.e[2] : 0.0;
    
    return *this;
}

vec3 &vec3::operator*=(float t)
{
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    
    return *this;
}

vec3 &vec3::operator/=(float t)
{
    if( t != 0.0 ) {
        e[0] /= t;
        e[1] /= t;
        e[2] /= t;
    }
    
    return *this;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2)
{
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2)
{
    vec3 temp;
    
    temp.e[0] = v2.e[0] != 0.0 ? v1.e[0] / v2.e[0] : 0.0;
    temp.e[1] = v2.e[1] != 0.0 ? v1.e[1] / v2.e[1] : 0.0;
    temp.e[2] = v2.e[2] != 0.0 ? v1.e[2] / v2.e[2] : 0.0;
    
    //return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
    return temp;
}

inline vec3 operator*(const vec3 &v, float t)
{   
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(float t, const vec3 &v)
{   
    return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator/(const vec3 &v, float t)
{
    if(t != 0.0) {
        float inv = 1.0/t;    
        
        return vec3(v.e[0] * inv, v.e[1] * inv, v.e[2] * inv);
    }
    else {
        return vec3(0.0, 0.0, 0.0);
    }
}

inline float dot(const vec3 &v1, const vec3 &v2)
{
    return (v1.e[0] * v2.e[0]) + (v1.e[1] * v2.e[1]) + (v1.e[2] * v2.e[2]);
}

inline vec3 cross(const vec3 &v1, const vec3 &v2)
{
    return vec3(
            (v1.e[1] * v2.e[2]) - (v1.e[2] * v2.e[1]),
            (v1.e[2] * v2.e[0]) - (v1.e[0] * v2.e[2]),
            (v1.e[0] * v2.e[1]) - (v1.e[1] * v2.e[0])
    );
}

inline vec3 unit_vector(vec3 v)
{
    return v / v.length();
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    int test = 0;
    do {
        p = 2.0*vec3(dis(gen), dis(gen), dis(gen)) - vec3(1.0, 1.0, 1.0);
        ++test;
        
        if(test > 50)
            std::cout << "MECAGONSATAN\n";
            
    }
    while( dot(p, p) >= 1.0 );
    
    return p;
}

vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2.0 * dot(v, n) * n;
}
bool refract(const vec3 &v, const vec3 &n, float ni_over_nt, vec3 &refracted)
{
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1.0 - dt * dt);
    if( discriminant > 0.0 ) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;        
    }
    else
        return false;
}

#endif // __VEC3_H__

