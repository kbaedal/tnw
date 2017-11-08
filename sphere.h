#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "hitable.h"
#include "material.h"

class sphere : public hitable
{
    public:
        sphere() : center(vec3(0.0, 0.0, 0.0)), radius(1.0), mat_ptr(nullptr) {};
        sphere(vec3 cen, float r, material *mp) : center(cen), radius(r), mat_ptr(mp) {}
        ~sphere() { if(mat_ptr != nullptr) delete mat_ptr; }
        
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        
        vec3    center;
        float   radius;
        material *mat_ptr;
};

bool sphere::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    vec3 oc = r.origin() - center;
    
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    
    float discriminant = b * b - 4.0 * a * c;
    
    if( discriminant > 0.0) {
        float temp = (-b - sqrt(discriminant)) / (2.0 * a);
        
        if(temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = this->mat_ptr;
            
            return true;
        }
        
        temp = (-b + sqrt(discriminant)) / (2.0 * a);
        
        if(temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = this->mat_ptr;
            
            return true;
        }        
    }
    
    return false;
}

#endif // __SPHERE_H__