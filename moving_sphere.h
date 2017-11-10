#ifndef __MOVING_SPHERE_H__
#define __MOVING_SPHERE_H__

#include "hitable.h"
#include "material.h"
#include "aabb.h"

class moving_sphere : public hitable
{
    public:
        moving_sphere() {};
        moving_sphere(vec3 c0, vec3 c1, float t0, float t1, float r, material *m) :
            center0(c0), center1(c1), time0(t0), time1(t1), radius(r), mat_ptr(m) {}
        ~moving_sphere() { if(mat_ptr != nullptr) delete mat_ptr; }
    
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const;
        
        vec3    center(float time) const;
        
        vec3    center0, center1;
        float   time0, time1;
        float   radius;
        material *mat_ptr;
};

bool moving_sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec) const
{
    vec3 oc = r.origin() - center(r.time());
    
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    
    float discriminant = b * b - 4.0 * a * c;
    
    if( discriminant > 0.0) {
        float temp = (-b - sqrt(discriminant)) / (2.0 * a);
        
        if(temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = this->mat_ptr;
            
            return true;
        }
        
        temp = (-b + sqrt(discriminant)) / (2.0 * a);
        
        if(temp < tmax && temp > tmin) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center(r.time())) / radius;
            rec.mat_ptr = this->mat_ptr;
            
            return true;
        }        
    }
    
    return false;
}

vec3 moving_sphere::center(float time) const
{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool moving_sphere::bounding_box(float t0, float t1, aabb &box) const
{
    aabb boxt0(center(t0) - vec3(radius, radius, radius), center(t0) + vec3(radius, radius, radius));
    aabb boxt1(center(t1) - vec3(radius, radius, radius), center(t1) + vec3(radius, radius, radius));
    
    box = surrounding(boxt0, boxt1);
    
    return true;
}

#endif // __MOVING_SPHERE_H__
