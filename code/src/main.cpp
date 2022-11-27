#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "scene.hpp"
#include "image.hpp"
#include "camera.hpp"
#include "group.hpp"
#include "light.hpp"
#include "radiance.hpp"

#include <string>

using namespace std;

int main() {
    for(int i = 0; i < scenes.size(); i++) {
        Scene* scene = scenes[i];
        Renderer* renderer = scene->renderer;
        Camera* camera = scene->camera;
        Group* group = scene->group;

        printf("Painting scene %d...\n", i);
        Image image(camera->getWidth(), camera->getHeight());
        renderer->render(&image, camera, group);
        std::string path = "output/scene" + std::to_string(i) + ".ppm";
        image.SavePPM(path.c_str());
        printf("Scene %d completed painting. Saved in '%s'\n", i, path);
    }
    return 0;
}

