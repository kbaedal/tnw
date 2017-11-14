#include <iostream>
#include <fstream>

const double kPI = 3.141592653589793;

#include "rangen.h"

#include "vec3.h"
#include "ray.h"
#include "camera.h"

#include "sphere.h"
#include "moving_sphere.h"
#include "hitablelist.h"
#include "rect.h"
#include "box.h"

#include "material.h"
#include "texture.h"
#include "lambertian.h"
#include "metal.h"
#include "dielectric.h"
#include "diffuse_light.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "float.h"

vec3 color(const ray &r, hitable *world, int depth)
{
    hit_record rec;
    if(world->hit(r, 0.001, FLT_MAX, rec)) {
        ray scattered;
        vec3 attenuation;
        vec3 emmited = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        
        if ( depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered) ) {
            return emmited + attenuation * color(scattered, world, depth+1);
            
        }
        else {
            return emmited;
        }
        
    }
    else {
        /*
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        */
        return vec3(0.0, 0.0, 0.0);          
    }
}

hitable *random_scene()
{
    int n = 500;
    hitable **list = new hitable*[n+1];
    //list[0] = new sphere(vec3(0.0, -1000.0, -1.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));
    texture *checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] = new sphere(vec3(0.0, -1000.0, -1.0), 1000.0, new lambertian(checker));
    
    int i = 1;
    
    for(int a = -11; a < 11; ++a) {
        for(int b = -11; b < 11; ++b) {
            float choose_mat = dis(gen);
            vec3 center(a+0.9*dis(gen), 0.2, b+0.9*dis(gen));
            if((center-vec3(4.0, 0.2, 0.0)).length() > 0.9) {
                if( choose_mat < 0.8 ) { // diffuse
                    //list[i++] = new sphere(center, 0.2, new lambertian(vec3(dis(gen)*dis(gen), dis(gen)*dis(gen), dis(gen)*dis(gen))));
                    list[i++] = new moving_sphere(center, center + vec3(0.0, 0.5*dis(gen), 0.0), 
                        0.0, 
                        1.0, 
                        0.2, 
                        new lambertian(new constant_texture(vec3(dis(gen)*dis(gen), dis(gen)*dis(gen), dis(gen)*dis(gen)))));
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
    list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    
    return new hitable_list(list, i);
}

hitable *standard_scene()
{
    hitable **list = new hitable*[5];
    
    list[0] = new sphere(vec3(0.0, 0.0, -1.0), 0.5, new lambertian(new constant_texture(vec3(0.3, 0.3, 0.8))));    
    list[1] = new sphere(vec3(0.0, -100.5, -1.0), 100.0, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.0))));
    list[2] = new sphere(vec3(1.0, 0.0, -1.0), 0.5, new metal(vec3(0.8, 0.6, 0.2), 1.0));
    list[3] = new sphere(vec3(-1.0, 0.0, -1.0), 0.5, new dielectric(1.5));
    list[4] = new sphere(vec3(-1.0, 0.0, -1.0), -0.45, new dielectric(1.5));
   
    return new hitable_list(list, 5);
}

hitable *two_spheres()
{
    texture *checker = new checker_texture(
        new constant_texture(vec3(0.2, 0.3, 0.1)),
        new constant_texture(vec3(0.9, 0.9, 0.9)));
        
    int n = 50;
    hitable **list = new hitable*[n+1];
    
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));
    
    return new hitable_list(list, 2);
}

hitable *two_perlin_spheres()
{
    texture *per_text = new noise_texture(4.0);
    
    hitable **list = new hitable*[2];
    
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(per_text));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(per_text));
    
    return new hitable_list(list, 2);
}

hitable *earth_sphere()
{    
    int nx, ny, nn;
    //unsigned char *tex_data = stbi_load("checker.png", &nx, &ny, &nn, 0);
    unsigned char *tex_data = stbi_load("earthmap.jpg", &nx, &ny, &nn, 0);
    material *mat =  new lambertian(new image_texture(tex_data, nx, ny));
    return new sphere(vec3(0.0, 0.0, 0.0), 2.0, mat);
}

hitable *simple_light()
{
    texture *per_text = new noise_texture(4.0);
    hitable **list = new hitable*[4];
    
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(per_text));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(per_text));
    list[2] = new sphere(vec3(0, 7, 0), 2, new diffuse_light(new constant_texture(vec3(4.0, 4.0, 4.0))));
    list[3] = new rect_xy(3.0, 5.0, 1.0, 3.0, -2.0, new diffuse_light(new constant_texture(vec3(4.0, 4.0, 4.0))));
    
    return new hitable_list(list, 4);
}

hitable *cornell_box()
{
    hitable **list = new hitable*[6];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    //material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));
    material *light2 = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
    
    list[i++] = new flip_normals(new rect_yz(0, 555, 0, 555, 555, green));
    list[i++] = new rect_yz(0, 555, 0, 555, 0, red);
    //list[i++] = new rect_xz(213, 343, 227, 332, 554, light);
    list[i++] = new rect_xz(113, 443, 127, 432, 554, light2);
    list[i++] = new flip_normals(new rect_xz(0, 555, 0, 555, 555, white));
    list[i++] = new rect_xz(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new rect_xy(0, 555, 0, 555, 555, white));
    list[i++] = new box(vec3(130, 0, 65), vec3(295, 165, 230), white);
    list[i++] = new box(vec3(265, 0, 295), vec3(430, 330, 460), white);
    
    return new hitable_list(list, i);
}

int main()
{
    int nx = 2*200;
    int ny = 2*200;
    int ns = 1000;
        
    std::ofstream myfile("test.ppm");

    myfile << "P3\n" << nx << " " << ny << "\n255\n";
    
    //hitable *world = standard_scene();
    //hitable *world = random_scene();
    //hitable *world = two_spheres();
    //hitable *world = two_perlin_spheres();
    //hitable *world = earth_sphere();
    //hitable *world = simple_light();
    hitable *world = cornell_box();
    
    /*
    vec3 lookfrom(3.0, 3.0, 2.0);
    vec3 lookat(0.0, 0.0, -1.0);
    vec3 camup(0.0, 1.0, 0.0);
    float dist_to_focus = (lookfrom - lookat).length();
    float aperture = 2.0;
    */
    
    /*
    vec3 lookfrom(13.0, 2.0, 3.0);
    vec3 lookat(0.0, 2.0, 0.0);
    vec3 camup(0.0, 1.0, 0.0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    */
    
    vec3 lookfrom(278, 278, -800);
    vec3 lookat(278.0, 278.0, 0.0);
    vec3 camup(0.0, 1.0, 0.0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    
    camera cam(lookfrom, lookat, camup, vfov, float(nx)/float(ny), aperture, dist_to_focus, 0.0, 1.0);
    
    for(int j = ny - 1, k = 1; j >= 0; --j, ++k) {
        for(int i = 0; i < nx; ++i) {
            std::cout << "\rRenderizando linea " << k << " de " << ny << " ...";
            vec3 col(0.0, 0.0, 0.0);
            
            for(int s = 0; s < ns; ++s) {
                float u = float(i + dis(gen)) / float(nx);
                float v = float(j + dis(gen)) / float(ny);
                
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            
            col /= float(ns);
            col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            
            int ir = int(255.99 * (col.r() > 1.0 ? 1.0 : col.r()));
            int ig = int(255.99 * (col.g() > 1.0 ? 1.0 : col.g()));
            int ib = int(255.99 * (col.b() > 1.0 ? 1.0 : col.b()));

            myfile << ir << " " << ig << " " << ib << "\n";
        }
    }    

    myfile.close();
}
