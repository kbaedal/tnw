#ifndef __AABB_H__
#define __AABB_H__

#include "vec3.h"
#include "ray.h"

inline float ffmin(float a, float b) { return (a < b) ? a : b; }
inline float ffmax(float a, float b) { return (a > b) ? a : b; }

class aabb {
    public:
        aabb() {}
        aabb(const vec3 &a, const vec3 &b) : _min(a), _max(b) {}
        
        vec3 min() const { return _min; }
        vec3 max() const { return _max; }
        
        bool hit(const ray &r, float tmin, float tmax)
        {
            for(int a = 0; a < 3; ++a) {
                float invD = 1.0 / r.direction()[a];
                float t0 = (_min[a] - r.origin()[a]) * invD;
                float t1 = (_max[a] - r.origin()[a]) * invD;
                
                if(invD < 0.0f)
                    std::swap(t0, t1);
                    
                tmin = (t0 > tmin) ? t0 : tmin;
                tmax = (t1 < tmax) ? t1 : tmax;
                
                if(tmax <= tmin)
                    return false;
            }
            return true;
        }
        
        vec3 _min;
        vec3 _max;
};

aabb surrounding(aabb box0, aabb box1) {
    vec3 small(std::fmin(box0._min.x(), box1._min.x()),
        std::fmin(box0._min.y(), box1._min.y()),
        std::fmin(box0._min.z(), box1._min.z()));
        
    vec3 big(std::fmax(box0._max.x(), box1._max.x()),
        std::fmax(box0._max.y(), box1._max.y()),
        std::fmax(box0._max.z(), box1._max.z()));
        
    return aabb(small, big);
}

#endif // __AABB_H__