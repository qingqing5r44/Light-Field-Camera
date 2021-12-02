#include "Renderer.hpp"
#include "Scene.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"
#include "global.hpp"
#include <chrono>

// In the main function of the program, we create the scene (create objects and
// lights) as well as set the options for the render (image width and height,
// maximum recursion depth, field-of-view, etc.). We then call the render
// function().
int main(int argc, char** argv)
{
    MeshTriangle bunny1("../../models/bunny.obj", -3.5, -1, 4, 50, MaterialType::DIFFUSE_AND_GLOSSY);
    MeshTriangle bunny2("../../models/bunny.obj", 6.5, 3, -4, 70, MaterialType::DIFFUSE_AND_GLOSSY);
    MeshTriangle bunny3("../../models/bunny.obj", 1.5, -2, 0, 60, MaterialType::DIFFUSE_AND_GLOSSY);


    std::cout << "Test Depth Refocusing: \n";
    int depth[] = {3, 15, 25};
    for (int i = 0; i < 3; i ++) {
        Scene scene(1280, 960);

        scene.Add(&bunny1);
        scene.Add(&bunny2);
        scene.Add(&bunny3);

        scene.focalDepth = depth[i];
        std::cout << "depth = "<< scene.focalDepth <<"\n";

        scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 30), 1));
        scene.Add(std::make_unique<Light>(Vector3f(20, 70, 30), 1));
        scene.buildBVH();

        Renderer r;

        auto start = std::chrono::system_clock::now();
        r.Render(scene, i);
        auto stop = std::chrono::system_clock::now();

        std::cout << "Picture "<< i <<" complete: \n";
        //std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
        //std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
        //std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    }

    std::cout << "Test Aperture Adjustment: \n";
    for (int i = 0; i < 2; i ++) {
        Scene scene(1280, 960);

        scene.Add(&bunny1);
        scene.Add(&bunny2);
        scene.Add(&bunny3);

        scene.focalDepth = depth[2];
        scene.aperture = scene.aperture + (i + 1) * 0.2;
        printf("apartment = %lf\n",scene.aperture);

        scene.Add(std::make_unique<Light>(Vector3f(-20, 70, 30), 1));
        scene.Add(std::make_unique<Light>(Vector3f(20, 70, 30), 1));
        scene.buildBVH();

        Renderer r;

        auto start = std::chrono::system_clock::now();

        int num = i + sizeof(depth)/sizeof(int);
        r.Render(scene, num);
        auto stop = std::chrono::system_clock::now();

        std::cout << "Picture "<< num <<" complete: \n";
        //std::cout << "Time taken: " << std::chrono::duration_cast<std::chrono::hours>(stop - start).count() << " hours\n";
        //std::cout << "          : " << std::chrono::duration_cast<std::chrono::minutes>(stop - start).count() << " minutes\n";
        //std::cout << "          : " << std::chrono::duration_cast<std::chrono::seconds>(stop - start).count() << " seconds\n";

    }    
    return 0;
}