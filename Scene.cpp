#include "Scene.h"

#include "materials/Phong.h"
#include "materials/displacement.h"
#include "initializer/demloader.h"
#include "models/Plane.h"
#include "models/Terrain.h"

Scene::Scene()
{
    // TODO un hardcode file
    DemLoader demLoader("../../data/6702_1_10m_z32.dem");
    Displacement *dismap = demLoader.getDisplacement();

    materials.append(dismap);

    Plane *plane = new Plane(dismap);
    models.push_back(plane);

    //Plane *plane = new Plane(map);
    //models.push_back(plane);
}
