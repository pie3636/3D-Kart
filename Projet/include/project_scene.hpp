#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "Viewer.hpp"

void initialize_scene(Viewer& viewer);

MeshRenderablePtr createKartFromMesh(void);
CubeRenderablePtr createKartFromPrimitives(void);

CylinderRenderablePtr createCharacterFromPrimitives(void);

glm::mat4 translate(RenderablePtr obj, double x, double y, double z);
glm::mat4 rotate(RenderablePtr obj, float alpha, double x, double y, double z);
glm::mat4 scale(RenderablePtr obj, double x, double y, double z);

#endif // PROJECT_SCENE_H
