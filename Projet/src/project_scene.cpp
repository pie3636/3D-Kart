#include "./../include/project_scene.hpp"

#include "./../include/Viewer.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/IndexedCubeRenderable.hpp" 
#include "./../include/CubeRenderable.hpp" 
#include "./../include/CylinderRenderable.hpp" 
#include "./../include/MeshRenderable.hpp"

#include "../include/log.hpp"
#include <fstream>

void initialize_scene(Viewer& viewer) {
    std::string vShader = "./../shaders/flatVertex.glsl";
    std::string fShader = "./../shaders/flatFragment.glsl";
    ShaderProgramPtr parentProg = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(parentProg);

    CubeRenderablePtr root = std::make_shared<CubeRenderable>(parentProg);
    CylinderRenderablePtr wheel_fl = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr wheel_fr = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr wheel_hl = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr wheel_hr = std::make_shared<CylinderRenderable>(parentProg);

    glm::mat4 rootParentTransform;
    root->setParentTransform(rootParentTransform);

    double hull_height = .4;
    double hull_length = 5;
    double wheel_width = .5;
    glm::mat4 rootLocalTransform = translate(root, 0, 0, -hull_height/2.0) * scale(root, hull_length, 2, hull_height);
    root->setLocalTransform(rootLocalTransform);

    glm::mat4 wheelFL = scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelFR = translate(wheel_fr, 0, 2 + wheel_width, 0) * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelHL = translate(wheel_fr, hull_length, 0, 0) * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelHR = translate(wheel_fr, hull_length, 2 + wheel_width, 0) * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    wheel_fl->setParentTransform(wheelFL);
    wheel_fr->setParentTransform(wheelFR);
    wheel_hl->setParentTransform(wheelHL);
    wheel_hr->setParentTransform(wheelHR);

    HierarchicalRenderable::addChild(root, wheel_fl);
    HierarchicalRenderable::addChild(root, wheel_fr);
    HierarchicalRenderable::addChild(root, wheel_hl);
    HierarchicalRenderable::addChild(root, wheel_hr);

    viewer.addRenderable(root);
}

glm::mat4 translate(RenderablePtr obj, double x, double y, double z) {
    return glm::translate(obj->getModelMatrix(), glm::vec3(x, y, z));
}

glm::mat4 rotate(RenderablePtr obj, float alpha, double x, double y, double z) {
    return glm::rotate(obj->getModelMatrix(), alpha, glm::vec3(x, y, z));
}

glm::mat4 scale(RenderablePtr obj, double x, double y, double z) {
    return glm::scale(obj->getModelMatrix(), glm::vec3(x, y, z));
}
