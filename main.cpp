#include <iostream>
#include <fstream>

const double kPI = 3.141592653589793;

#include "rangen.h"

#include "vec3.h"
#include "ray.h"
#include "camera.h"

#include "sphere.h"
#include "hitablelist.h"

#include "material.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"

#include "float.h"

vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        
        if ( depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth+1);
        }
        else {
            return vec3(0.0, 0.0, 0.0);
        }
        
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0.0, -1000.0, -1.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));
    
    int i = 1;
    
    for(int a = -11; a < 11; ++a) {
        for(int b = -11; b < 11; ++b) {
            float choose_mat = dis(gen);
            vec3 center(a+0.9*dis(gen), 0.2, b+0.9*dis(gen));
            if((center-vec3(4.0, 0.2, 0.0)).length() > 0.9) {
                if( choose_mat < 0.8 ) { // diffuse
                    list[i++] = new sphere(center, 0.2, new lambertian(vec3(dis(gen)*dis(gen), dis(gen)*dis(gen), dis(gen)*dis(gen))));
                }
                else if( choose_mat < 0.95 ) { // metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen)), 0.5 * (1 + dis(gen))), 0.5 * dis(gen)));
                }
                else { // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0.0, 1.0, 0.0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    
    return new hitable_list(list, i);
}

hitable *standard_scene()
{
    hitable **list = new hitable*[5];
    
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(vec3(0.3, 0.3, 0.8)));    
    list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100.0, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 1.0));
    list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
   
    return new hitable_list(list, 5);
}

int main()
{
    int nx = 6*200;
    int ny = 6*100;
    int ns = 100;
        
    std::ofstream myfile("test.ppm");

    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    
    //hitable *world = standard_scene();
    hitable *world = random_scene();
    
    /*
    vec3 lookfrom(3.0, 3.0, 2.0);
    vec3 lookat(0.0, 0.0, -1.0);
    vec3 camup(0.0, 1.0, 0.0);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 2.0;
    */
    
    vec3 lookfrom(13.0, 2.0, 3.0);
    vec3 lookat(0.0, 0.0, 0.0);
    vec3 camup(0.0, 1.0, 0.0);
    float dist_to_focus = 10.0;
    float aperture = 0.1;
    
    camera cam(lookfrom, lookat, camup, 20, float(nx)/float(ny), aperture, dist_to_focus);

    for(int j = ny - 1; j >= 0; --j) {
        for(int i = 0; i < nx; ++i) {
            vec3 col(0.0, 0.0, 0.0);
            
            for(int s = 0; s < ns; ++s) {
                float u = float(i + dis(gen)) / float(nx);
                float v = float(j + dis(gen)) / float(ny);
                
                ray r = cam.get_ray(u, v);
                
                //vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            
            int ir = int(255.99 * col.r());
            int ig = int(255.99 * col.g());
            int ib = int(255.99 * col.b());

            myfile << ir << " " << ig << " " << ib << "\n";
        }
    }

    myfile.close();
}
