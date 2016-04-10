#include "./../include/project_scene.hpp"

#include "./../include/ShaderProgram.hpp"
#include "./../include/FrameRenderable.hpp"
#include "./../include/CubeRenderable.hpp" 
#include "./../include/CylinderRenderable.hpp" 
#include "./../include/MeshRenderable.hpp" 
#include "./../include/HierarchicalRenderable.hpp" 

void initialize_practical_07_scene(Viewer& viewer){
    //Position the camera
    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, -8, 8), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

    //Default shader
    ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
    viewer.addShaderProgram(flatShader);

    //Add a 3D frame to the viewer
    FrameRenderablePtr frame = std::make_shared<FrameRenderable>(flatShader);
    viewer.addRenderable(frame);

    //Temporary variables
    glm::mat4 parentTransformation(1.0), localTransformation(1.0);
    std::string filename;

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

    viewer.startAnimation();
    viewer.setAnimationLoop(true, 4.0);
}
