#ifndef __BOX_H__
#define __BOX_H__

#include "hitable.h"
#include "hitablelist.h"
#include "rect.h"

class box : public hitable
{
    public:
        box();
        box(const vec3 &p0, const vec3 &p1, material *mat_ptr);
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const 
        {
            box = aabb(pmin, pmax);
            return true;
        }
    
    vec3 pmin, pmax;
    hitable *list_ptr;
};

box::box(const vec3 &p0, const vec3 &p1, material *mat_ptr)
{
    pmin = p0;
    pmax = p1;
    hitable **list = new hitable*[6];
    
    list[0] = new rect_xy(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat_ptr);
    list[1] = new flip_normals(new rect_xy(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat_ptr));
    list[2] = new rect_xz(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat_ptr);
    list[3] = new flip_normals(new rect_xz(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat_ptr));
    list[4] = new rect_yz(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat_ptr);
    list[5] = new flip_normals(new rect_yz(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat_ptr));
    
    list_ptr = new hitable_list(list, 6);
}

bool box::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    return list_ptr->hit(r, tmin, tmax, rec);
}

#endif // __BOX_H__