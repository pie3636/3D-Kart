#include "./../include/project_scene.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"

#include "../include/dynamics/DampingForceField.hpp"
#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/SpringForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"

#include "../include/dynamics/ParticleRenderable.hpp"
#include "../include/dynamics/ParticleListRenderable.hpp"
#include "../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics/SpringForceFieldRenderable.hpp"
#include "../include/dynamics/SpringListRenderable.hpp"
#include "../include/dynamics/ControlledForceFieldRenderable.hpp"

#include "./../include/Utils.hpp"
#include "./../include/QuadRenderable.hpp"
#include "./../include/SphereRenderable.hpp"
#include "./../include/IndexedCubeRenderable.hpp"

#include "../include/log.hpp"
#include <fstream>

void initialize_scene(Viewer& viewer) {

    viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, -6, 6 ), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

    std::string         vShader = "./../shaders/flatVertex.glsl";
    std::string         fShader = "./../shaders/flatFragment.glsl";

    ShaderProgramPtr    parentProg = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(parentProg);
    
    ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    viewer.addShaderProgram(texShader);

    // Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    DynamicSystemPtr system = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    system->setSolver(solver);
    system->setDt(0.01);

	// Create a renderable associated to the dynamic system
	// This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
	// It is also responsible for some of the key/mouse events
    DynamicSystemRenderablePtr systemRenderable = std::make_shared<DynamicSystemRenderable>(system);
    viewer.addRenderable(systemRenderable);

    kart_game_light(viewer);
    //kart_game_borders(viewer,system,systemRenderable);
    //kart_game_road(viewer);

    MeshRenderablePtr    				meshKar2 = createKartFromMesh               (parentProg);
    TexturedLightedMeshRenderablePtr    meshKart = createTexturedKartFromMesh       (texShader);
    CubeRenderablePtr       			primKart = createKartFromPrimitives         (parentProg);
    CylinderRenderablePtr   			primChar = createCharacterFromPrimitives    (parentProg);

	//viewer.addRenderable(meshKar2);
    viewer.addRenderable(meshKart);
    //viewer.addRenderable(primKart);
    //viewer.addRenderable(primChar);

	// Place the camera
	viewer.getCamera().setViewMatrix(glm::lookAt(glm::vec3(0, -6, 60), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

	viewer.startAnimation();
	viewer.setAnimationLoop(true, 4.0);
}

TexturedLightedMeshRenderablePtr createTexturedKartFromMesh(ShaderProgramPtr texShader) {
    
    TexturedLightedMeshRenderablePtr kart = std::make_shared<TexturedLightedMeshRenderable>(texShader, "../meshes/Kart3.obj", "../textures/wood.jpg");
    kart->setMaterial(Material::Pearl());
    glm::mat4 parentTransformation;
    parentTransformation = glm::translate( glm::mat4(1.0), glm::vec3( 0, 4, 1.0 ) );
    parentTransformation = glm::rotate( parentTransformation, float(M_PI_2), glm::vec3(1,0,0) );
    parentTransformation = glm::scale( parentTransformation, glm::vec3(2,2,2));
    kart->setParentTransform( parentTransformation );
    /*double scaleFactor = 0.5;
    kart->setParentTransform(translate(kart, 0, 0, 0.41) * rotate(kart, M_PI/2, 1, 0, 0) * rotate(kart, M_PI, 0, 1, 0) * scale(kart, scaleFactor, scaleFactor, scaleFactor));*/
    return kart;
}

MeshRenderablePtr createKartFromMesh(ShaderProgramPtr parentProg) {
	MeshRenderablePtr kart = std::make_shared<MeshRenderable>(parentProg, "../meshes/Kart.obj");
    double scaleFactor = 0.5;
    kart->setParentTransform(translate(kart, 0, 0, 0.41) * rotate(kart, M_PI/2, 1, 0, 0) * rotate(kart, M_PI, 0, 1, 0) * scale(kart, scaleFactor, scaleFactor, scaleFactor));
    return kart;
}

CubeRenderablePtr createKartFromPrimitives(ShaderProgramPtr parentProg) {
    CubeRenderablePtr       root = std::make_shared<CubeRenderable>(parentProg);
    CylinderRenderablePtr   wheel_fl = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr   wheel_fr = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr   wheel_hl = std::make_shared<CylinderRenderable>(parentProg);
    CylinderRenderablePtr   wheel_hr = std::make_shared<CylinderRenderable>(parentProg);

    glm::mat4 rootParentTransform;
    root->setParentTransform(rootParentTransform);

    double hull_height  = .4;
    double hull_width   = 2;
    double hull_length  = 5;
    double wheel_width  = .5;

    glm::mat4 rootLocalTransform = translate(root, 0, 0, -hull_height/2.0) * scale(root, hull_length, hull_width, hull_height);
    root->setLocalTransform(rootLocalTransform);

    glm::mat4 wheelFL =                                                           scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelFR = translate(wheel_fr, 0, 2 + wheel_width, 0)              * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelHL = translate(wheel_fr, hull_length, 0, 0)                  * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);
    glm::mat4 wheelHR = translate(wheel_fr, hull_length, 2 + wheel_width, 0)    * scale(wheel_fl, 1, wheel_width, 1) * rotate(wheel_fl, (float)(M_PI/2.0), 1, 0, 0);

    wheel_fl->setParentTransform(wheelFL);
    wheel_fr->setParentTransform(wheelFR);
    wheel_hl->setParentTransform(wheelHL);
    wheel_hr->setParentTransform(wheelHR);

    HierarchicalRenderable::addChild(root, wheel_fl);
    HierarchicalRenderable::addChild(root, wheel_fr);
    HierarchicalRenderable::addChild(root, wheel_hl);
    HierarchicalRenderable::addChild(root, wheel_hr);

    return root;
}

CylinderRenderablePtr createCharacterFromPrimitives(ShaderProgramPtr parentProg) {

    // The indentation reflects the tree-like structure of the character.
    CylinderRenderablePtr   chest                           = std::make_shared<CylinderRenderable>  (parentProg);
    CylinderRenderablePtr       neck                        = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr         head                        = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr         shoulder_l                  = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr           upperArm_l              = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                 elbow_l             = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr                   forearm_l       = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                         wrist_l     = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr                             hand_l  = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr         shoulder_r                  = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr           upperArm_r              = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                 elbow_r             = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr                   forearm_r       = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                         wrist_r     = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr                             hand_r  = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr       upperLeg_r                  = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr             knee_r                  = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr               calf_r              = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                     ankle_r         = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr                         foot_r      = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr       upperLeg_l                  = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr             knee_l                  = std::make_shared<SphereRenderable>    (parentProg);
    CylinderRenderablePtr               calf_l              = std::make_shared<CylinderRenderable>  (parentProg);
    SphereRenderablePtr                     ankle_l         = std::make_shared<SphereRenderable>    (parentProg);
    SphereRenderablePtr                         foot_l      = std::make_shared<SphereRenderable>    (parentProg);

    // Scale vectors (see ./struct/Personnage)
    glm::vec3 chestS    (0.3,  0.25, 0.6);
    glm::vec3 neckS     (0.12, 0.1,  0.1);
    glm::vec3 headS     (0.2,  0.2,  0.25);
    glm::vec3 shoulderS (0.1,  0.1,  0.1);
    glm::vec3 upperArmS (0.1,  0.1,  0.2);
    glm::vec3 elbowS    (0.1,  0.1,  0.1);
    glm::vec3 forearmS  (0.1,  0.1,  0.25);
    glm::vec3 wristS    (0.08, 0.08, 0.08);
    glm::vec3 handS     (0.1,  0.05, 0.18);
    glm::vec3 upperLegS (0.15, 0.15, 0.3);
    glm::vec3 kneeS     (0.12, 0.12, 0.12);
    glm::vec3 calfS     (0.12, 0.12, 0.25);
    glm::vec3 ankleS    (0.08, 0.08, 0.08);
    glm::vec3 footS     (0.08, 0.2,  0.08);

    // Translation vectors (see ./struct/Personnage)
    glm::vec3 chestT        (0,                             0,                          -chestS.z/2                                                                            );
    glm::vec3 neckT         (0,                             0,                          chestS.z/2      			                                                           );
    glm::vec3 headT         (0,                             0,                          (chestS.z + headS.z)/2 + neckS.z                                                       );
    glm::vec3 shoulderLT    (-chestS.x - shoulderS.x,       0,                          (chestS.z - shoulderS.z)/2                                                             );
    glm::vec3 upperArmLT    (-chestS.x - shoulderS.x,       0,                          (chestS.z - 2*upperArmS.z)/2 - shoulderS.z                                             );
    glm::vec3 elbowLT       (-chestS.x - shoulderS.x,       0,                          (chestS.z - elbowS.z)/2 - shoulderS.z - upperArmS.z                                    );
    glm::vec3 forearmLT     (-chestS.x - shoulderS.x,       0,                          (chestS.z - 2*forearmS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z                     );
    glm::vec3 wristLT       (-chestS.x - shoulderS.x,       0,                          (chestS.z - wristS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z - forearmS.z            );
    glm::vec3 handLT        (-chestS.x - shoulderS.x,       0,                          (chestS.z - handS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z - forearmS.z - wristS.z  );
    glm::vec3 shoulderRT    (chestS.x + shoulderS.x,        0,                          (chestS.z - shoulderS.z)/2                                                             );
    glm::vec3 upperArmRT    (chestS.x + shoulderS.x,        0,                          (chestS.z - 2*upperArmS.z)/2 - shoulderS.z                                             );
    glm::vec3 elbowRT       (chestS.x + shoulderS.x,        0,                          (chestS.z - elbowS.z)/2 - shoulderS.z - upperArmS.z                                    );
    glm::vec3 forearmRT     (chestS.x + shoulderS.x,        0,                          (chestS.z - 2*forearmS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z                     );
    glm::vec3 wristRT       (chestS.x + shoulderS.x,        0,                          (chestS.z - wristS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z - forearmS.z            );
    glm::vec3 handRT        (chestS.x + shoulderS.x,        0,                          (chestS.z - handS.z)/2 - shoulderS.z - upperArmS.z - elbowS.z - forearmS.z - wristS.z  );
    glm::vec3 upperLegLT    ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - 2*upperLegS.z)/2                                                          );
    glm::vec3 kneeLT        ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - kneeS.z)/2 - upperLegS.z                                                  );
    glm::vec3 calfLT        ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - 2*calfS.z)/2 - upperLegS.z - kneeS.z                                      );
    glm::vec3 ankleLT       ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - ankleS.z)/2 - upperLegS.z - kneeS.z - calfS.z                             );
    glm::vec3 footLT        ((upperLegS.x - chestS.x)/2,    (ankleS.x + footS.x)/2,     (-chestS.z - footS.z)/2 - upperLegS.z - kneeS.z - calfS.z - ankleS.z                   );
    glm::vec3 upperLegRT    ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - 2*upperLegS.z)/2                                                          );
    glm::vec3 kneeRT        ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - kneeS.z)/2 - upperLegS.z                                                  );
    glm::vec3 calfRT        ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - 2*calfS.z)/2 - upperLegS.z - kneeS.z                                      );
    glm::vec3 ankleRT       ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - ankleS.z)/2 - upperLegS.z - kneeS.z - calfS.z                             );
    glm::vec3 footRT        ((chestS.x - upperLegS.x)/2,    (ankleS.x + footS.x)/2,     (-chestS.z - footS.z)/2 - upperLegS.z - kneeS.z - calfS.z - ankleS.z                   );

    // Set global position so that the character can be standing
    chest->setParentTransform(translate(chest, 2, 2, ankleS.z + calfS.z + kneeS.z + upperLegS.z + chestS.z));

    // Apply transforms
    chest       ->setLocalTransform(translate(chest,        chestT     ) * scale(chest,        chestS     ));
    neck        ->setLocalTransform(translate(neck,         neckT      ) * scale(neck,         neckS      ));
    head        ->setLocalTransform(translate(head,         headT      ) * scale(head,         headS      ));
    shoulder_l  ->setLocalTransform(translate(shoulder_l,   shoulderLT ) * scale(shoulder_l,   shoulderS  ));
    upperArm_l  ->setLocalTransform(translate(upperArm_l,   upperArmLT ) * scale(upperArm_l,   upperArmS  ));
    elbow_l     ->setLocalTransform(translate(elbow_l,      elbowLT    ) * scale(elbow_l,      elbowS     ));
    forearm_l   ->setLocalTransform(translate(forearm_l,    forearmLT  ) * scale(forearm_l,    forearmS   ));
    wrist_l     ->setLocalTransform(translate(wrist_l,      wristLT    ) * scale(wrist_l,      wristS     ));
    hand_l      ->setLocalTransform(translate(hand_l,       handLT     ) * scale(hand_l,       handS      ));
    shoulder_r  ->setLocalTransform(translate(shoulder_r,   shoulderRT ) * scale(shoulder_r,   shoulderS  ));
    upperArm_r  ->setLocalTransform(translate(upperArm_r,   upperArmRT ) * scale(upperArm_r,   upperArmS  ));
    elbow_r     ->setLocalTransform(translate(elbow_r,      elbowRT    ) * scale(elbow_r,      elbowS     ));
    forearm_r   ->setLocalTransform(translate(forearm_r,    forearmRT  ) * scale(forearm_r,    forearmS   ));
    wrist_r     ->setLocalTransform(translate(wrist_r,      wristRT    ) * scale(wrist_r,      wristS     ));
    hand_r      ->setLocalTransform(translate(hand_r,       handRT     ) * scale(hand_r,       handS      ));
    upperLeg_l  ->setLocalTransform(translate(upperLeg_l,   upperLegLT ) * scale(upperLeg_l,   upperLegS  ));
    knee_l      ->setLocalTransform(translate(knee_l,       kneeLT     ) * scale(knee_l,       kneeS      ));
    calf_l      ->setLocalTransform(translate(calf_l,       calfLT     ) * scale(calf_l,       calfS      ));
    ankle_l     ->setLocalTransform(translate(ankle_l,      ankleLT    ) * scale(ankle_l,      ankleS     ));
    foot_l      ->setLocalTransform(translate(foot_l,       footLT     ) * scale(foot_l,       footS      ));
    upperLeg_r  ->setLocalTransform(translate(upperLeg_r,   upperLegRT ) * scale(upperLeg_r,   upperLegS  ));
    knee_r      ->setLocalTransform(translate(knee_r,       kneeRT     ) * scale(knee_r,       kneeS      ));
    calf_r      ->setLocalTransform(translate(calf_r,       calfRT     ) * scale(calf_r,       calfS      ));
    ankle_r     ->setLocalTransform(translate(ankle_r,      ankleRT    ) * scale(ankle_r,      ankleS     ));
    foot_r      ->setLocalTransform(translate(foot_r,       footRT     ) * scale(foot_r,       footS      ));

    HierarchicalRenderable::addChild(chest,         neck       );
    HierarchicalRenderable::addChild(neck,          head       );
    HierarchicalRenderable::addChild(chest,         shoulder_l );
    HierarchicalRenderable::addChild(shoulder_l,    upperArm_l );
    HierarchicalRenderable::addChild(upperArm_l,    elbow_l    );
    HierarchicalRenderable::addChild(elbow_l,       forearm_l  );
    HierarchicalRenderable::addChild(forearm_l,     wrist_l    );
    HierarchicalRenderable::addChild(wrist_l,       hand_l     );
    HierarchicalRenderable::addChild(chest,         shoulder_r );
    HierarchicalRenderable::addChild(shoulder_r,    upperArm_r );
    HierarchicalRenderable::addChild(upperArm_r,    elbow_r    );
    HierarchicalRenderable::addChild(elbow_r,       forearm_r  );
    HierarchicalRenderable::addChild(forearm_r,     wrist_r    );
    HierarchicalRenderable::addChild(wrist_r,       hand_r     );
    HierarchicalRenderable::addChild(chest,         upperLeg_l );
    HierarchicalRenderable::addChild(upperLeg_l,    knee_l     );
    HierarchicalRenderable::addChild(knee_l,        calf_l     );
    HierarchicalRenderable::addChild(calf_l,        ankle_l    );
    HierarchicalRenderable::addChild(ankle_l,       foot_l     );
    HierarchicalRenderable::addChild(chest,         upperLeg_r );
    HierarchicalRenderable::addChild(upperLeg_r,    knee_r     );
    HierarchicalRenderable::addChild(knee_r,        calf_r     );
    HierarchicalRenderable::addChild(calf_r,        ankle_r    );
    HierarchicalRenderable::addChild(ankle_r,       foot_r     );

    return chest;
}

glm::mat4 translate(RenderablePtr obj, double x, double y, double z) {
    return translate(obj, glm::vec3(x, y, z));
}

glm::mat4 rotate(RenderablePtr obj, float alpha, double x, double y, double z) {
    return rotate(obj, alpha, glm::vec3(x, y, z));
}

glm::mat4 scale(RenderablePtr obj, double x, double y, double z) {
    return scale(obj, glm::vec3(x, y, z));
}

glm::mat4 translate(RenderablePtr obj, glm::vec3 vec) {
    return glm::translate(obj->getModelMatrix(), vec);
}

glm::mat4 rotate(RenderablePtr obj, float alpha, glm::vec3 vec) {
    return glm::rotate(obj->getModelMatrix(), alpha, vec);
}

glm::mat4 scale(RenderablePtr obj, glm::vec3 vec) {
    return glm::scale(obj->getModelMatrix(), vec);
}

void kart_game_light(Viewer& viewer) {
  // Default shader
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
  viewer.addShaderProgram(flatShader);

  // Temporary variables
  glm::mat4 localTransformation(1.0);

  // Define a directional light for the whole scene
  glm::vec3 d_direction = glm::normalize(glm::vec3(0.0, 0.0, -1.0));
  glm::vec3 d_ambient(1.0, 1.0, 1.0), d_diffuse(1.0, 1.0, 0.8), d_specular(1.0, 1.0, 1.0);
  DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);

  //Add a renderable to display the light and control it via mouse/key event
  glm::vec3 lightPosition(0.0,0.0,5.0);
  DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
  directionalLightRenderable->setLocalTransform(localTransformation);
  viewer.setDirectionalLight(directionalLight);
  viewer.addRenderable(directionalLightRenderable);
}

void kart_game_borders(Viewer& viewer , DynamicSystemPtr& system, DynamicSystemRenderablePtr& systemRenderable) {
  //Temporary variables
  glm::mat4 parentTransformation(1.0);
  std::string filename;
  MaterialPtr pearl = Material::Pearl();

  //Textured shader
  ShaderProgramPtr texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
  viewer.addShaderProgram(texShader);

  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
  viewer.addShaderProgram(flatShader);

// collision ***********************************

  // Activate collision detection
  system->setCollisionsDetection(true);

  // Initialize the restitution coefficient for collision
  // 1.0 = full elastic response
  // 0.0 = full absorption
  system->setRestitution(0.8f);

  // Textured plane
  glm::vec3 p1(-25.0, -25.0, -0.2), p2(25.0, -25.0, -0.2), p3(25.0, 25.0, -0.2), p4(-25.0, 25.0, -0.2);
  PlanePtr plane = std::make_shared<Plane>(p1, p2, p3);
  system->addPlaneObstacle(plane);
  filename = "./../textures/grass_texture.png";
  TexturedPlaneRenderablePtr ground = std::make_shared<TexturedPlaneRenderable>(texShader, filename,true);
  parentTransformation = glm::scale(glm::mat4(1.0), glm::vec3(10.0, 10.0, 10.0));
  ground->setParentTransform(parentTransformation);
  ground->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, ground);

  // Textured plane
  filename = "./../textures/mur_pierre.jpeg";

  p1 = glm::vec3(25.0, -25.0, 0.0);
  p2 = glm::vec3(25.0, 25.0,  0.0);
  p3 = glm::vec3(25.0, 25.0,  5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  system->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall1 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,false);
  parentTransformation= glm::translate(glm::mat4(1.0), glm::vec3(25.0, 0.0, 0.0));
  wall1->setParentTransform(parentTransformation);
  wall1->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall1);

  p1 = glm::vec3(-25.0, -25.0, 0.0);
  p2 = glm::vec3(-25.0, 25.0,  0.0);
  p3 = glm::vec3(-25.0, 25.0,  5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  system->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,false);
  parentTransformation= glm::translate(glm::mat4(1.0), glm::vec3(-25.0, 0.0, 0.0));
  wall2->setParentTransform(parentTransformation);
  wall2->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall2);

  p1 = glm::vec3(-25.0, 25.0, 0.0);
  p2 = glm::vec3(25.0,  25.0, 0.0);
  p3 = glm::vec3(25.0,  25.0, 5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  system->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall3 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,false);
  parentTransformation= glm::rotate(glm::mat4(1.0), float (M_PI/2), glm::vec3(0.0, 0.0, 1.0));
  parentTransformation*= glm::translate(glm::mat4(1.0), glm::vec3(25.0, 0.0, 0.0));
  wall3->setParentTransform(parentTransformation);
  wall3->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall3);

  p1 = glm::vec3(-25.0, -25.0, 0.0);
  p2 = glm::vec3(25.0,  -25.0, 0.0);
  p3 = glm::vec3(25.0,  -25.0, 5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  system->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall4 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,false);
  parentTransformation = glm::rotate(glm::mat4(1.0), float (-M_PI/2), glm::vec3(0.0,0.0,1.0));
  parentTransformation*= glm::translate(glm::mat4(1.0), glm::vec3(25.0,0.0,0.0));
  wall4->setParentTransform(parentTransformation);
  wall4->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall4);

  /*
  glm::vec3 px,pv;
  float pm, pr;
  px = glm::vec3(20.0,0.0,3.0);
  pv = glm::vec3(10.0,0.0,0.0);
  pr = 1.;
  pm = 5.;
  ParticlePtr particle = std::make_shared<Particle>(px, pv, pm, pr);
  system->addParticle(particle);
  ParticleRenderablePtr particleRenderable = std::make_shared<ParticleRenderable>(flatShader, particle);
  HierarchicalRenderable::addChild(systemRenderable, particleRenderable);
  */

  // Initialize a force field that apply to all the particles of the system to simulate gravity
  // Add it to the system as a force field
  ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(system->getParticles(), glm::vec3{0,0,-10});
  system->addForceField(gravityForceField);
}

void kart_game_road(Viewer& viewer) {
  // Default shader
  ShaderProgramPtr flatShader = std::make_shared<ShaderProgram>("../shaders/flatVertex.glsl","../shaders/flatFragment.glsl");
  viewer.addShaderProgram(flatShader);

  getRoad(flatShader, viewer);
}
