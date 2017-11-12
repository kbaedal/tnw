#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "hitable.h"

class material
{
    public:
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
        virtual vec3 emitted(float u, float v, const vec3 &v) const { return vec3(0.0, 0.0, 0.0); }
        virtual ~material() {};
};

#endif // __MATERIAL_H__