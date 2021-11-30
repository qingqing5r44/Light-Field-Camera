//
// Created by goksu on 2/25/20.
//

#include <fstream>
#include "Scene.hpp"
#include "Renderer.hpp"
// For uniform_int_distribution::operator()
// #include <chrono>
#include <random>


inline float deg2rad(const float& deg) { return deg * M_PI / 180.0; }

const float EPSILON = 0.00001;

// The main render function. This where we iterate over all pixels in the image,
// generate primary rays and cast these rays into the scene. The content of the
// framebuffer is saved to a file.
void Renderer::Render(const Scene& scene, int sceneNumber)
{
    std::vector<Vector3f> framebuffer(scene.width * scene.height);

    float scale = tan(deg2rad(scene.fov * 0.5));
    float imageAspectRatio = scene.width / (float)scene.height;
    Vector3f eye_pos(-1, 5, 10);
    int m = 0;
    for (uint32_t j = 0; j < scene.height; ++j) {
        for (uint32_t i = 0; i < scene.width; ++i) {
            // generate primary ray direction
            // generate primary ray direction
            float x;
            float y;
            x = (2 * (i + 0.5) / (float)scene.width - 1) * imageAspectRatio * scale;
            y = (1 - 2 * (j + 0.5) / (float)scene.height) * scale;

            Vector3f dir = Vector3f(x, y, -1); // Don't forget to normalize this direction!

            // get a uniform random number genertor using current time as seed
            // reference: http://www.cplusplus.com/reference/random/uniform_real_distribution/operator()/
            // unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
            // std::default_random_engine generator (seed);
            // std::uniform_int_distribution<int> distribution(0,1000);
            
            // Vector2f point = Vector2f(0.001f * distribution(generator), 0.001f * distribution(generator));
            double rand1 = ((double) rand() / (RAND_MAX));
            double rand2 = ((double) rand() / (RAND_MAX));
            
            Vector3f apertureOffset = Vector3f(rand1 * scene.aperture, rand2 * scene.aperture, 0.0f);
            // Vector3f apertureOffset = Vector3f(point.x * scene.aperture, point.y * scene.aperture, 0.0f);
            eye_pos = eye_pos + apertureOffset;
            dir = dir * scene.focalDepth; // scale ray dir to focus at particular depth
            dir = dir - apertureOffset; 
            dir = normalize(dir); // normalizing the dir vect

            // ### PRINT STATEMENTS for checking values ###
            // std::cout << "point x times app: " << point.x * scene.aperture << std::endl;
            // std::cout << "rand1 " << rand1 * scene.aperture <<std::endl;
            // std::cout << "point y times app: " << point.y * scene.aperture << std::endl;
            // std::cout << "app offset: " << apertureOffset << std::endl;
            // std::cout << "New Eye pos: " << eye_pos << std::endl;

            framebuffer[m++] = scene.castRay(Ray(eye_pos, dir), 0);
            eye_pos = Vector3f(-1, 5, 10); // reset eye_pos for next ray 
        }
        UpdateProgress(j / (float)scene.height);
    }
    UpdateProgress(1.f);

    // save framebuffer to file
    std::ofstream ofs;
    ofs.open("./out" + std::to_string(sceneNumber) + ".ppm" );
    ofs << "P6\n" << scene.width << " " << scene.height << "\n255\n";
    for (uint32_t i = 0; i < scene.height * scene.width; ++i) {
        auto r = (uint8_t)(255 * clamp(0, 1, framebuffer[i].x));
        auto g = (uint8_t)(255 * clamp(0, 1, framebuffer[i].y));
        auto b = (uint8_t)(255 * clamp(0, 1, framebuffer[i].z));
        ofs << r << g << b;
    }
    ofs.close();
}
