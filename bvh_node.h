#ifndef __BVH_NODE_H__
#define __BVH_NODE_H__

#include "hitable.h"
#include "aabb.h"

class bvh_node : public hitable 
{
    public:
        bvh_node() {}
        bvh_node(hitable **l, int n, float time0, float time1);
        
        virtual bool hit(const ray &r, float tmin, float tmax, hit_record &rec) const;
        virtual bool bounding_box(float t0, float t1, aabb &box) const;
        
        hitable *left,
                *right;
        aabb    box;
};

bool bvh_node::hit(const ray &r, float tmin, float tmax, hit_record &rec) const
{
    if(box.hit(r, tmin, tmax)) {
        hit_record right_rec, left_rec;
        
        bool hit_left = left->hit(r, tmin, tmax, left_rec);
        bool hit_right = right->hit(r, tmin, tmax, right_rec);
        
        if(hit_left && hit_right) {
            if(hit.left.t)
        }
            
        
        
    }
}

#endif // __BVH_NODE_H__
