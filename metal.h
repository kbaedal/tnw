#ifndef __METAL_H__
#define __METAL_H__

#include "material.h"

class metal : public material
{
    public:
        metal(const vec3 &a, float f) : albedo(a) { fuzz = f < 1.0 ? f : 1.0; }
        virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
            attenuation = albedo;
            
            return ( dot(scattered.direction(), rec.normal) > 0.0 );
        }
        
        vec3 albedo;
        float fuzz;
};

#endif // __METAL_H__