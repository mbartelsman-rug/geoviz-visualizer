#include "Scene.h"

#include "materials/MapSurface.h"
#include "materials/Phong.h"
#include "models/Plane.h"
#include "models/Terrain.h"

Scene::Scene() {
    Material * mapSurface = new MapSurface(":/textures/fractal_noise.png");
    Model * plane = new Terrain(mapSurface);
    materials.push_back(mapSurface);
    models.push_back(plane);
}
