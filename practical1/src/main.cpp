#include "./../include/Viewer.hpp"
#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/IndexedCubeRenderable.hpp" 
#include "./../include/CubeRenderable.hpp" 
#include "./../include/CylinderRenderable.hpp" 
#include "./../include/MeshRenderable.hpp" 

#include "../include/log.hpp"
#include <fstream>

static void initialize_scene(Viewer& viewer);
static void initialize_hierarchical_scene(Viewer& viewer);
static void initialize_car(Viewer& viewer);
glm::mat4 translate(RenderablePtr obj, double x, double y, double z);
glm::mat4 rotate(RenderablePtr obj, float alpha, double x, double y, double z);
glm::mat4 scale(RenderablePtr obj, double x, double y, double z);

int main() {
    int width = 1280;
    int height = 720;
    Viewer viewer(width, height);

    //initialize_scene(viewer);
    //initialize_hierarchical_scene(viewer);
    initialize_car(viewer);

    while (viewer.isRunning()) {
        viewer.handleEvent();
        viewer.draw();
        viewer.display();
    }
}

static void check_execution_directory() {
    std::ifstream font( "../fonts/Anonymous_Pro.ttf");
    std::ifstream text_fragment( "../shaders/text_fragment.glsl");

    if( !font.is_open() || !text_fragment.is_open() ) {
        LOG( fatal, "could not find ../fonts/Anonymous_Pro.ttf or ../shaders/text_fragment.glsl" );
        LOG( fatal, "are you sure you launched the executable in the build/ directory?");
        exit( EXIT_FAILURE );
    }
}

static void initialize_hierarchical_scene(Viewer& viewer) {
    // create programs (you can use the same program for both the child and parent)
    std::string vShader = "./../shaders/flatVertex.glsl";
    std::string fShader = "./../shaders/flatFragment.glsl";
    ShaderProgramPtr parentProg = std::make_shared<ShaderProgram>(vShader, fShader);
    ShaderProgramPtr childProg = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(parentProg);
    viewer.addShaderProgram(childProg);

    std::shared_ptr<CylinderRenderable> root = std::make_shared<CylinderRenderable>(parentProg);
    std::shared_ptr<CubeRenderable> child1 = std::make_shared<CubeRenderable>(childProg);

    glm::mat4 rootParentTransform;
    root->setParentTransform(rootParentTransform);

    glm::mat4 child1ParentTransform;
    child1->setParentTransform(child1ParentTransform);
    glm::mat4 child1LocalTransform;
    child1->setLocalTransform(child1LocalTransform);
    root->setParentTransform(scale(root, 3, 1, 1));

    HierarchicalRenderable::addChild(root, child1);

    viewer.addRenderable(root);
}

static void initialize_scene(Viewer& viewer) {
    check_execution_directory();

    std::string vShader = "./../shaders/defaultVertex.glsl";
    std::string fShader = "./../shaders/defaultFragment.glsl";
    ShaderProgramPtr defaultShader = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(defaultShader);

    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(defaultShader);
    viewer.addRenderable(frame);

    vShader = "./../shaders/flatVertex.glsl";
    fShader = "./../shaders/flatFragment.glsl";
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(flatShader);

    FrameRenderablePtr flatFrame = std::make_shared<FrameRenderable>(defaultShader);
    viewer.addRenderable(flatFrame);

    IndexedCubeRenderablePtr indexedCube = std::make_shared<IndexedCubeRenderable>(flatShader);
    viewer.addRenderable(indexedCube);

    CubeRenderablePtr cube = std::make_shared<CubeRenderable>(flatShader);
    viewer.addRenderable(cube);

    indexedCube->setModelMatrix(translate(indexedCube, 3, 1, 1));
    indexedCube->setModelMatrix(rotate(indexedCube, 45.0, 1, 1, 1));
    cube->setModelMatrix(scale(cube, 2, .5, 1.5));

    CylinderRenderablePtr cylinder = std::make_shared<CylinderRenderable>(flatShader, 30);
    viewer.addRenderable(cylinder);

    MeshRenderablePtr suzanne = std::make_shared<MeshRenderable>(flatShader, "../suzanne.obj");
    viewer.addRenderable(suzanne);
}

static void initialize_car(Viewer& viewer) {
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
