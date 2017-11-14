#ifndef __RECT_H__
#define __RECT_H__

#include "hitable.h"

class rect_xy : public hitable
{
    public:
        rect_xy() {}
        rect_xy(float _x0, float _x1, float _y0, float _y1, float _k, material *_mp) :
            x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(_mp) {}
            
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const
        {
            box = aabb(vec3(x0, y0, k-0.0001), vec3(x1, y1, k+0.0001));
            return true;
        }
                
        float x0, x1, y0, y1, k;
        material *mp;
};

class rect_xz : public hitable
{
    public:
        rect_xz() {}
        rect_xz(float _x0, float _x1, float _z0, float _z1, float _k, material *_mp) :
            x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(_mp) {}
            
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const
        {
            box = aabb(vec3(x0, k-0.0001, z0), vec3(x1, k+0.0001, z1));
            return true;
        }
        
        float x0, x1, z0, z1, k;
        material *mp;
};

class rect_yz : public hitable
{
    public:
        rect_yz() {}
        rect_yz(float _y0, float _y1, float _z0, float _z1, float _k, material *_mp) :
            y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(_mp) {}
            
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const
        {
            box = aabb(vec3(k-0.0001, y0, z0), vec3(k+0.0001, y1, z1));
            return true;
        }
        
        float y0, y1, z0, z1, k;
        material *mp;
};

bool rect_xy::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    float t = (k - r.origin().z()) / r.direction().z();
    
    if( t < tmin || t > tmax)
        return false;
        
    float x = r.origin().x() + t * r.direction().x();
    float y = r.origin().y() + t * r.direction().y();
    
    if( (x < x0) || (x > x1) || (y < y0) || (y > y1) )
        return false;
        
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0.0, 0.0, 1.0);
    
    return true;
}

bool rect_xz::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    float t = (k - r.origin().y()) / r.direction().y();
    
    if( t < tmin || t > tmax)
        return false;
        
    float x = r.origin().x() + t * r.direction().x();
    float z = r.origin().z() + t * r.direction().z();
    
    if( (x < x0) || (x > x1) || (z < z0) || (z > z1) )
        return false;
        
    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(0.0, 1.0, 0.0);
    
    return true;
}

bool rect_yz::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    float t = (k - r.origin().x()) / r.direction().x();
    
    if( t < tmin || t > tmax)
        return false;
        
    float y = r.origin().y() + t * r.direction().y();
    float z = r.origin().z() + t * r.direction().z();
    
    if( (y < y0) || (y > y1) || (z < z0) || (z > z1) )
        return false;
        
    rec.u = (y - y0) / (y1 - y0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.mat_ptr = mp;
    rec.p = r.point_at_parameter(t);
    rec.normal = vec3(1.0, 0.0, 0.0);
    
    return true;
}



#endif // __RECT_H__