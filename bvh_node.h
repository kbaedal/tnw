#ifndef __BVH_NODE_H__
#define __BVH_NODE_H__

#include "hitable.h"
#include "aabb.h"
#include "rangen.h"

int box_x_compare(const void *a, const void *b);
int box_y_compare(const void *a, const void *b);
int box_z_compare(const void *a, const void *b);

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
        
        if( hit_left && hit_right ) {
            if( left_rec.t < right_rec.t )
                rec = left_rec;
            else
                rec = right_rec;
                
            return true;
        }
        else if(hit_left) {
            rec = left_rec;
            return true;
        }
        else if(hit_right) {
            rec = right_rec;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}

bvh_node(hitable **l, int n, float time0, float time1)
{
    int axis = int(3 * dis(gen));
    
    if( axis == 0 )
        std::qsort(l, n, sizeof(hitable *), box_x_compare);
    else if( axis == 1 )
        std::qsort(l, n, sizeof(hitable *), box_y_compare);
    else
        std::qsort(l, n, sizeof(hitable *), box_z_compare);
    
}

#endif // __BVH_NODE_H__
