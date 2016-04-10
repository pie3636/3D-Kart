#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H

#include "Viewer.hpp"
#include "ShaderProgram.hpp"
#include "CubeRenderable.hpp" 
#include "CylinderRenderable.hpp" 
#include "MeshRenderable.hpp"

void initialize_scene(Viewer& viewer);

static MeshRenderablePtr        createKartFromMesh              (ShaderProgramPtr parentProg);
static CubeRenderablePtr        createKartFromPrimitives        (ShaderProgramPtr parentProg);
static CylinderRenderablePtr    createCharacterFromPrimitives   (ShaderProgramPtr parentProg);

static glm::mat4 translate  (RenderablePtr obj, double x, double y, double z);
static glm::mat4 rotate     (RenderablePtr obj, float alpha, double x, double y, double z);
static glm::mat4 scale      (RenderablePtr obj, double x, double y, double z);

static glm::mat4 translate  (RenderablePtr obj, glm::vec3 vec);
static glm::mat4 rotate     (RenderablePtr obj, float alpha, glm::vec3 vec);
static glm::mat4 scale      (RenderablePtr obj, glm::vec3 vec);

#endif // PROJECT_SCENE_H
