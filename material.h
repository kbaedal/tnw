#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "hitable.h"

class material
{
    public:
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const = 0;
        virtual ~material() {};
};

#endif // __MATERIAL_H__