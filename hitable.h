#ifndef __HITABLE_H__
#define __HITABLE_H__

class ray;
class material;
class aabb;

struct hit_record
{
    float       t,
                u,
                v;
    vec3        p,
                normal;
    material    *mat_ptr;
};

class hitable
{
    public:
        virtual ~hitable() {};
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const = 0;
        virtual bool bounding_box(float t0, float t1, aabb &box) const = 0;
};

class flip_normals : public hitable
{
    public:
        flip_normals(hitable *p) : ptr(p) {}
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const
        {
            if(ptr->hit(r, tmin, tmax, rec)) {
                rec.normal = -rec.normal;
                return true;
            }
            else {
                return false;
            }
        }
        virtual bool bounding_box(float t0, float t1, aabb &box) const
        {
            return ptr->bounding_box(t0, t1, box);
        }
  
    hitable *ptr;
};

#endif // __HITABLE_H__