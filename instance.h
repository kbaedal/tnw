#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include "hitable.h"

class translate : public hitable
{
    public:
        translate(hitable *p, const vec3 &displacement) : ptr(p), offset(displacement) {}
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const;
        
        hitable *ptr;
        vec3 offset;    
};

bool translate::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if(ptr->hit(moved_r, tmin, tmax, rec)) {
        rec.p += offset;
        return true;
    }
    else {
        return false;
    }
}

bool translate::bounding_box(float t0, float t1, aabb &box) const
{
    if(ptr->bounding_box(t0, t1, box)) {
        box = aabb(box.min() + offset, box.max() + offset);
        return true;
    }
    else {
        return false;
    }        
}

class rotate_y : public hitable
{
    public:
        rotate_y(hitable *p, float angle);
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const
        {
            box = bbox;
            return hasbox;
        }
        
        hitable *ptr;
        float   sin_theta,
                cos_theta;
        bool    hasbox;
        aabb    bbox;
};

rotate_y::rotate_y(hitable *p, float angle) : ptr(p)
{
    float radians = (kPI / 180.0) * angle;
    sin_theta = std::sin(radians);
    cos_theta = std::cos(radians);
    
    hasbox = ptr->bounding_box(0, 1, bbox);
    
    vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 2; ++j) {
            for(int k = 0; k < 2; ++k) {
                float x = i*bbox.max().x() + (1 - i)*bbox.min().x();
                float y = j*bbox.max().y() + (1 - j)*bbox.min().y();
                float z = k*bbox.max().z() + (1 - k)*bbox.min().z();
                
                float newx = cos_theta*x + sin_theta*z;
                float newz = -sin_theta*x + cos_theta*z;
                
                vec3 tester(newx, y, newz);
                
                for(int c = 0; c < 3; ++c) {
                    if( tester[c] > max[c])
                        max[c] = tester[c];
                    if( tester[c] < min[c])
                        min[c] = tester[c];
                }
            }
        }
    }
    
    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    vec3 origin = r.origin();
    vec3 direction = r.direction();
    
    origin[0] = cos_theta*origin.x() - sin_theta*origin.z();
    origin[2] = sin_theta*origin.x() + cos_theta*origin.z();
    
    direction[0] = cos_theta*direction.x() - sin_theta*direction.z();
    direction[2] = sin_theta*direction.x() + cos_theta*direction.z();
    
    ray rotated_r(origin, direction, r.time());
    
    if( ptr->hit(rotated_r, tmin, tmax, rec) ) {
        vec3 p = rec.p;
        vec3 normal = rec.normal;
        
        p[0] = cos_theta*p.x() + sin_theta*p.z();
        p[2] = -sin_theta*p.x() + cos_theta*p.z();
        
        normal[0] = cos_theta*normal.x() + sin_theta*normal.z();
        normal[2] = -sin_theta*normal.x() + cos_theta*normal.z();
        
        rec.p = p;
        rec.normal = normal;
        
        return true;
    }
    else {
        return false;
    }    
}

#endif // __INSTANCE_H__
