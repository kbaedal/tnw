#ifndef __DIFFUSE_LIGHT_H__
#define __DIFFUSE_LIGHT_H__

#include "material.h"

class diffuse_light : public material
{
    public:
        diffuse_light() {}
        diffuse_light(texture *a) : emit(a) {}
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            return false;
        }
        virtual vec3 emitted(float u, float v, const vec3 &p) const
        {
            return emit->value(u, v, p);
        }
        
        texture *emit;
};

#endif // __DIFFUSE_LIGHT_H__