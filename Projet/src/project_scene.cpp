#include "./../include/project_scene.hpp"

#include "../include/ShaderProgram.hpp"
#include "../include/FrameRenderable.hpp"

#include "../include/KeyframedKartRenderable.hpp"

#include "../include/texturing/TexturedPlaneRenderable.hpp"
#include "../include/lighting/DirectionalLightRenderable.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"

#include "../include/dynamics/ConstantForceField.hpp"
#include "../include/dynamics/EulerExplicitSolver.hpp"


#include "../include/dynamics/ConstantForceFieldRenderable.hpp"
#include "../include/dynamics/ControlledForceFieldRenderable.hpp"

#include "./../include/Utils.hpp"
#include "./../include/QuadRenderable.hpp"
#include "./../include/SphereRenderable.hpp"
#include "./../include/IndexedCubeRenderable.hpp"

#include "../include/log.hpp"
#include <fstream>

int Scene::kartCount = 0;

Scene::Scene(Viewer* viewer) {
	this->viewer = viewer;

    flatShader = std::make_shared<ShaderProgram>("./../shaders/flatVertex.glsl", "./../shaders/flatFragment.glsl");
    viewer->addShaderProgram(flatShader);

    texShader = std::make_shared<ShaderProgram>("../shaders/textureVertex.glsl","../shaders/textureFragment.glsl");
    viewer->addShaderProgram(texShader);

    // Initialize a dynamic system (Solver, Time step, Restitution coefficient)
    dynSystem = std::make_shared<DynamicSystem>();
    EulerExplicitSolverPtr solver = std::make_shared<EulerExplicitSolver>();
    dynSystem->setSolver(solver);
    dynSystem->setDt(0.01);
	dynSystem->setCollisionsDetection(true); // Activate collision detection
	dynSystem->setRestitution(0.8f); // Restitution coefficient for collision - 1.0 = full elastic response, 0.0 = full absorption

	// Create a renderable associated to the dynamic system
	// This renderable is responsible for calling DynamicSystem::computeSimulationStep() in the animate() function
	// It is also responsible for some of the key/mouse events
    systemRenderable = std::make_shared<DynamicSystemRenderable>(dynSystem);
    viewer->addRenderable(systemRenderable);

	// Initialize a force field that apply to all the particles of the system to simulate gravity
	// Add it to the system as a force field
	ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(dynSystem->getKarts(), glm::vec3{0,0,-10});
	dynSystem->addForceField(gravityForceField);

    kart_game_light();
    kart_game_borders();
    getRoad(flatShader, *viewer);

    //TexturedLightedMeshRenderablePtr    meshKart = createTexturedKartFromMesh       ();
    //TexturedLightedMeshRenderablePtr    meshKar2 = createTexturedKartFromMesh       ();
    //TexturedLightedMeshRenderablePtr    meshKar3 = createTexturedKartFromMesh       ();
    //TexturedLightedMeshRenderablePtr    meshKar4 = createTexturedKartFromMesh       ();

    //viewer->addRenderable(meshKart);
    //viewer->addRenderable(meshKar2);
    //viewer->addRenderable(meshKar3);
    //viewer->addRenderable(meshKar4);

		createTexturedKartFromMesh();

		
	// Place the camera
	viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(-25, -25, 60), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

	viewer->startAnimation();
	viewer->setAnimationLoop(true, 50.0);
}

Scene::~Scene() {}

KeyframedKartRenderablePtr Scene::createTexturedKartFromMesh() {
	//std::string tex[] = {"grass_texture.png", "mur_pierre.jpeg", "wood.jpg", "metal wall2.jpg"};
    KeyframedKartRenderablePtr kart = std::make_shared<KeyframedKartRenderable>(texShader, "../meshes/Kart.obj", "../textures/wood.jpg");
    //kartCount++;
    kart->setMaterial(Material::Pearl());
		kart->setParentTransform(rotate(kart, M_PI/2, 1, 0, 0) * rotate(kart, -M_PI/2-0.2, 0, 1, 0)  *
														 scale(kart, 0.5, 0.5, 0.5) * translate(kart, -32, 1., -3.) );
    HierarchicalRenderable::addChild(systemRenderable, 	kart							);
    //HierarchicalRenderable::addChild(kart, 				createCharacterFromPrimitives() );
		moving_kart(kart,texShader, "../meshes/Kart.obj", "../textures/wood.jpg");
}

/*
TexturedLightedMeshRenderablePtr Scene::createTexturedKartFromMesh() {
	std::string tex[] = {"grass_texture.png", "mur_pierre.jpeg", "wood.jpg", "metal wall2.jpg"};
    TexturedLightedMeshRenderablePtr kart = std::make_shared<TexturedLightedMeshRenderable>(texShader, "../meshes/Kart.obj", "../textures/" + tex[kartCount]);
    kartCount++;
    kart->setMaterial(Material::Pearl());
    kart->setParentTransform(translate(kart, 3 * kartCount, 0, 1) * scale(kart, kartScaleFactor, kartScaleFactor, kartScaleFactor));
    kart->setLocalTransform(rotate(kart, M_PI/2, 1, 0, 0) * rotate(kart, M_PI, 0, 1, 0));
    HierarchicalRenderable::addChild(systemRenderable, 	kart							);
    HierarchicalRenderable::addChild(kart, 				createCharacterFromPrimitives() );
    return kart;
}
*/
CylinderRenderablePtr Scene::createCharacterFromPrimitives() {

    // The indentation reflects the tree-like structure of the character.
    CylinderRenderablePtr   chest                           = std::make_shared<CylinderRenderable>  (flatShader);
    CylinderRenderablePtr       neck                        = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr             head                    = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr         shoulder_l                  = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr           upperArm_l              = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr                 elbow_l             = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr                   forearm_l       = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr                         wrist_l     = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr                             hand_l  = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr         shoulder_r                  = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr           upperArm_r              = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr                 elbow_r             = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr                   forearm_r       = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr                         wrist_r     = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr                             hand_r  = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr         hip_r  						= std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr       	upperLeg_r              = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr             	knee_r              = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr               	calf_r          = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr                     	ankle_r     = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr                         	foot_r  = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr         hip_l  						= std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr       	upperLeg_l              = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr             	knee_l              = std::make_shared<SphereRenderable>    (flatShader);
    CylinderRenderablePtr           	    calf_l          = std::make_shared<CylinderRenderable>  (flatShader);
    SphereRenderablePtr             	        ankle_l     = std::make_shared<SphereRenderable>    (flatShader);
    SphereRenderablePtr             	            foot_l  = std::make_shared<SphereRenderable>    (flatShader);

    // Scale vectors (see ./struct/Personnage)
    glm::vec3 chestS    (0.3,  0.2,  0.6);
    glm::vec3 neckS     (0.12, 0.1,  0.1);
    glm::vec3 headS     (0.2,  0.2,  0.25);
    glm::vec3 shoulderS (0.1,  0.1,  0.1);
    glm::vec3 upperArmS (0.1,  0.1,  0.2);
    glm::vec3 elbowS    (0.1,  0.1,  0.1);
    glm::vec3 forearmS  (0.1,  0.1,  0.25);
    glm::vec3 wristS    (0.08, 0.08, 0.08);
    glm::vec3 handS     (0.1,  0.05, 0.18);
    glm::vec3 hipS      (0.12, 0.12, 0.12);
    glm::vec3 upperLegS (0.15, 0.15, 0.46);
    glm::vec3 kneeS     (0.12, 0.12, 0.12);
    glm::vec3 calfS     (0.12, 0.12, 0.41);
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
    glm::vec3 hipLT    		((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - hipS.z)/2                                                                 );
    glm::vec3 upperLegLT    ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - 2*upperLegS.z)/2 - hipS.z                                                          );
    glm::vec3 kneeLT        ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - kneeS.z)/2 - hipS.z - upperLegS.z                                                  );
    glm::vec3 calfLT        ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - 2*calfS.z)/2 - hipS.z - upperLegS.z - kneeS.z                                      );
    glm::vec3 ankleLT       ((upperLegS.x - chestS.x)/2,    0,                          (-chestS.z - ankleS.z)/2 - hipS.z - upperLegS.z - kneeS.z - calfS.z                             );
    glm::vec3 footLT        ((upperLegS.x - chestS.x)/2,    (ankleS.x + footS.x)/2,     (-chestS.z - footS.z)/2 - hipS.z - upperLegS.z - kneeS.z - calfS.z - ankleS.z                   );
    glm::vec3 hipRT    		((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - hipS.z)/2                                                                 );
    glm::vec3 upperLegRT    ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - 2*upperLegS.z)/2 - hipS.z                                                          );
    glm::vec3 kneeRT        ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - kneeS.z)/2 - upperLegS.z - hipS.z                                                  );
    glm::vec3 calfRT        ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - 2*calfS.z)/2 - upperLegS.z - kneeS.z - hipS.z                                      );
    glm::vec3 ankleRT       ((chestS.x - upperLegS.x)/2,    0,                          (-chestS.z - ankleS.z)/2 - upperLegS.z - kneeS.z - calfS.z - hipS.z                             );
    glm::vec3 footRT        ((chestS.x - upperLegS.x)/2,    (ankleS.x + footS.x)/2,     (-chestS.z - footS.z)/2 - upperLegS.z - kneeS.z - calfS.z - ankleS.z - hipS.z                   );

    // Set position in kart
    double characterScale = 1.5;
    chest->setParentTransform(translate(chest, -0.6, -0.5, 1)   								// Kart positioning - Standing height is ankleS.z + calfS.z + kneeS.z + upperLegS.z + hipS.z + chestS.z
    						* rotate(chest, M_PI/4, 1, 0, 0) 									// Inclination in kart seat
    						* scale(chest, characterScale, characterScale, characterScale)); 	// Scale adaptation

    // Apply transforms (standing position)
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
    hip_l  		->setLocalTransform(translate(hip_l,   		hipLT 	   ) * scale(hip_l,   	   hipS  	  ));
    upperLeg_l  ->setLocalTransform(translate(upperLeg_l,   upperLegLT ) * scale(upperLeg_l,   upperLegS  ));
    knee_l      ->setLocalTransform(translate(knee_l,       kneeLT     ) * scale(knee_l,       kneeS      ));
    calf_l      ->setLocalTransform(translate(calf_l,       calfLT     ) * scale(calf_l,       calfS      ));
    ankle_l     ->setLocalTransform(translate(ankle_l,      ankleLT    ) * scale(ankle_l,      ankleS     ));
    foot_l      ->setLocalTransform(translate(foot_l,       footLT     ) * scale(foot_l,       footS      ));
    hip_r  		->setLocalTransform(translate(hip_r,   		hipRT 	   ) * scale(hip_r,   	   hipS  	  ));
    upperLeg_r  ->setLocalTransform(translate(upperLeg_r,   upperLegRT ) * scale(upperLeg_r,   upperLegS  ));
    knee_r      ->setLocalTransform(translate(knee_r,       kneeRT     ) * scale(knee_r,       kneeS      ));
    calf_r      ->setLocalTransform(translate(calf_r,       calfRT     ) * scale(calf_r,       calfS      ));
    ankle_r     ->setLocalTransform(translate(ankle_r,      ankleRT    ) * scale(ankle_r,      ankleS     ));
    foot_r      ->setLocalTransform(translate(foot_r,       footRT     ) * scale(foot_r,       footS      ));

    // Move character to have a proper posture in the kart
    auto tmp = chest->getParentTransform() * glm::vec4(0, 1, 0, 0) - chest->getParentTransform() * glm::vec4(0, 0, 0, 0);

    neck        ->setParentTransform(rotateParent(neck, -M_PI/8, glm::vec3(tmp)));
    /*
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
    hip_l  		->setLocalTransform(translate(hip_l,   		hipLT 	   ) * scale(hip_l,   	   hipS  	  ));
    upperLeg_l  ->setLocalTransform(translate(upperLeg_l,   upperLegLT ) * scale(upperLeg_l,   upperLegS  ));
    knee_l      ->setLocalTransform(translate(knee_l,       kneeLT     ) * scale(knee_l,       kneeS      ));
    calf_l      ->setLocalTransform(translate(calf_l,       calfLT     ) * scale(calf_l,       calfS      ));
    ankle_l     ->setLocalTransform(translate(ankle_l,      ankleLT    ) * scale(ankle_l,      ankleS     ));
    foot_l      ->setLocalTransform(translate(foot_l,       footLT     ) * scale(foot_l,       footS      ));
    hip_r  		->setLocalTransform(translate(hip_r,   		hipRT 	   ) * scale(hip_r,   	   hipS  	  ));
    upperLeg_r  ->setLocalTransform(translate(upperLeg_r,   upperLegRT ) * scale(upperLeg_r,   upperLegS  ));
    knee_r      ->setLocalTransform(translate(knee_r,       kneeRT     ) * scale(knee_r,       kneeS      ));
    calf_r      ->setLocalTransform(translate(calf_r,       calfRT     ) * scale(calf_r,       calfS      ));
    ankle_r     ->setLocalTransform(translate(ankle_r,      ankleRT    ) * scale(ankle_r,      ankleS     ));
    foot_r      ->setLocalTransform(translate(foot_r,       footRT     ) * scale(foot_r,       footS      ));*/

    HierarchicalRenderable::addChild(chest,         	neck       );
    HierarchicalRenderable::addChild(neck,          	head       );
    HierarchicalRenderable::addChild(chest,         	shoulder_l );
    HierarchicalRenderable::addChild(shoulder_l,    	upperArm_l );
    HierarchicalRenderable::addChild(upperArm_l,    	elbow_l    );
    HierarchicalRenderable::addChild(elbow_l,       	forearm_l  );
    HierarchicalRenderable::addChild(forearm_l,     	wrist_l    );
    HierarchicalRenderable::addChild(wrist_l,       	hand_l     );
    HierarchicalRenderable::addChild(chest,         	shoulder_r );
    HierarchicalRenderable::addChild(shoulder_r,    	upperArm_r );
    HierarchicalRenderable::addChild(upperArm_r,    	elbow_r    );
    HierarchicalRenderable::addChild(elbow_r,       	forearm_r  );
    HierarchicalRenderable::addChild(forearm_r,     	wrist_r    );
    HierarchicalRenderable::addChild(wrist_r,       	hand_r     );
    HierarchicalRenderable::addChild(chest,         	hip_l 	   );
    HierarchicalRenderable::addChild(hip_l,         	upperLeg_l );
    HierarchicalRenderable::addChild(upperLeg_l,    	knee_l     );
    HierarchicalRenderable::addChild(knee_l,        	calf_l     );
    HierarchicalRenderable::addChild(calf_l,        	ankle_l    );
    HierarchicalRenderable::addChild(ankle_l,       	foot_l     );
    HierarchicalRenderable::addChild(chest,         	hip_r 	   );
    HierarchicalRenderable::addChild(hip_r,         	upperLeg_r );
    HierarchicalRenderable::addChild(upperLeg_r,    	knee_r     );
    HierarchicalRenderable::addChild(knee_r,        	calf_r     );
    HierarchicalRenderable::addChild(calf_r,        	ankle_r    );
    HierarchicalRenderable::addChild(ankle_r,       	foot_r     );

    return chest;
}

glm::mat4 Scene::translate(HierarchicalRenderablePtr obj, double x, double y, double z) {
    return translate(obj, glm::vec3(x, y, z));
}

glm::mat4 Scene::rotate(HierarchicalRenderablePtr obj, float alpha, double x, double y, double z) {
    return rotate(obj, alpha, glm::vec3(x, y, z));
}

glm::mat4 Scene::scale(HierarchicalRenderablePtr obj, double x, double y, double z) {
    return scale(obj, glm::vec3(x, y, z));
}

glm::mat4 Scene::translate(HierarchicalRenderablePtr obj, glm::vec3 vec) {
    return glm::translate(obj->getLocalTransform(), vec);
}

glm::mat4 Scene::rotate(HierarchicalRenderablePtr obj, float alpha, glm::vec3 vec) {
    return glm::rotate(obj->getLocalTransform(), alpha, vec);
}

glm::mat4 Scene::scale(HierarchicalRenderablePtr obj, glm::vec3 vec) {
    return glm::scale(obj->getLocalTransform(), vec);
}

glm::mat4 Scene::translateParent(HierarchicalRenderablePtr obj, double x, double y, double z) {
    return translateParent(obj, glm::vec3(x, y, z));
}

glm::mat4 Scene::rotateParent(HierarchicalRenderablePtr obj, float alpha, double x, double y, double z) {
    return rotateParent(obj, alpha, glm::vec3(x, y, z));
}

glm::mat4 Scene::scaleParent(HierarchicalRenderablePtr obj, double x, double y, double z) {
    return scaleParent(obj, glm::vec3(x, y, z));
}

glm::mat4 Scene::translateParent(HierarchicalRenderablePtr obj, glm::vec3 vec) {
    return glm::translate(obj->getParentTransform(), vec);
}

glm::mat4 Scene::rotateParent(HierarchicalRenderablePtr obj, float alpha, glm::vec3 vec) {
    return glm::rotate(obj->getParentTransform(), alpha, vec);
}

glm::mat4 Scene::scaleParent(HierarchicalRenderablePtr obj, glm::vec3 vec) {
    return glm::scale(obj->getParentTransform(), vec);
}

void Scene::kart_game_light() {
  // Temporary variables
  glm::mat4 localTransformation(1.0);

  // Define a directional light for the whole scene
  glm::vec3 d_direction = glm::normalize(glm::vec3(0.0, 0.0, -1.0));
  glm::vec3 d_ambient(1.0, 1.0, 1.0), d_diffuse(1.0, 1.0, 0.8), d_specular(1.0, 1.0, 1.0);
  DirectionalLightPtr directionalLight = std::make_shared<DirectionalLight>(d_direction, d_ambient, d_diffuse, d_specular);

  // Add a renderable to display the light and control it via mouse/key event
  glm::vec3 lightPosition(0.0,0.0,5.0);
  DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, lightPosition);
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
  directionalLightRenderable->setLocalTransform(localTransformation);
  viewer->setDirectionalLight(directionalLight);
  viewer->addRenderable(directionalLightRenderable);
}

void Scene::kart_game_borders() {
  // Temporary variables
  glm::mat4 parentTransformation(1.0);
  std::string filename;
  MaterialPtr pearl = Material::Pearl();

  // Textured plane
  glm::vec3 p1(-25.0, -25.0, -0.2), p2(25.0, -25.0, -0.2), p3(25.0, 25.0, -0.2), p4(-25.0, 25.0, -0.2);
  PlanePtr plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  filename = "./../textures/grass_texture.png";
  TexturedPlaneRenderablePtr ground = std::make_shared<TexturedPlaneRenderable>(texShader, filename,0);
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
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall1 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation= glm::translate(glm::mat4(1.0), glm::vec3(25.0, 0.0, 0.0));
  wall1->setParentTransform(parentTransformation);
  wall1->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall1);

  p1 = glm::vec3(-25.0, -25.0, 0.0);
  p2 = glm::vec3(-25.0, 25.0,  0.0);
  p3 = glm::vec3(-25.0, 25.0,  5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation= glm::translate(glm::mat4(1.0), glm::vec3(-25.0, 0.0, 0.0));
  wall2->setParentTransform(parentTransformation);
  wall2->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall2);

  p1 = glm::vec3(-25.0, 25.0, 0.0);
  p2 = glm::vec3(25.0,  25.0, 0.0);
  p3 = glm::vec3(25.0,  25.0, 5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall3 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation= glm::rotate(glm::mat4(1.0), float (M_PI/2), glm::vec3(0.0, 0.0, 1.0));
  parentTransformation*= glm::translate(glm::mat4(1.0), glm::vec3(25.0, 0.0, 0.0));
  wall3->setParentTransform(parentTransformation);
  wall3->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall3);

  p1 = glm::vec3(-25.0, -25.0, 0.0);
  p2 = glm::vec3(25.0,  -25.0, 0.0);
  p3 = glm::vec3(25.0,  -25.0, 5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall4 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation = glm::rotate(glm::mat4(1.0), float (-M_PI/2), glm::vec3(0.0,0.0,1.0));
  parentTransformation*= glm::translate(glm::mat4(1.0), glm::vec3(25.0,0.0,0.0));
  wall4->setParentTransform(parentTransformation);
  wall4->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall4);

  filename =  "./../textures/michelin.jpeg";
  TexturedPlaneRenderablePtr ad = std::make_shared<TexturedPlaneRenderable>(texShader, filename, 2);
  ad->setMaterial(pearl);
  viewer->addRenderable(ad);
}


void Scene::moving_kart(KeyframedKartRenderablePtr root,
				ShaderProgramPtr program,
				const std::string& mesh_filename,
				const std::string& texture_filename )
{
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{0,0,0}), 0.25 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{0,0,33}), 0.5 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{2.5,0,40}, glm::quat( glm::vec3{0.0,  0.5, 0.} )), 0.6 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{5,0,45}, glm::quat( glm::vec3{0.0,  1., 0.} )), 0.75 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{10,0,45}, glm::quat( glm::vec3{0.0,  1.75, 0.} )), 0.9 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{60,0,38}, glm::quat( glm::vec3{0.0,  8., 0.} )), 1.5 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{75,0,25}, glm::quat( glm::vec3{0.0,  10., 0.} )), 2. );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{50,0,10}, glm::quat( glm::vec3{0.0,  -8., 0.} )), 3. );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{45,0,5}, glm::quat( glm::vec3{0.0,  10., 0.} )), 3.25 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{40,0,0}, glm::quat( glm::vec3{0.0,  15., 0.} )), 3.75 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{60,0,-25}, glm::quat( glm::vec3{0.0,  -10., 0.} )), 4.5 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{65,0,-33}, glm::quat( glm::vec3{0.0,  -15., 0.} )), 5. );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{50,0,-40}, glm::quat( glm::vec3{0.0,  -8., 0.} )), 5.25 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{-5,0, -25}, glm::quat( glm::vec3{0.0,  0., 0.} )), 6. );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{-5,0, 10}, glm::quat( glm::vec3{0.0,  1., 0.} )), 6.5 );
		root->addLocalTransformKeyframe( GeometricTransformation( glm::vec3{-5,0, 10}, glm::quat( glm::vec3{0.0,  1., 0.} )), 7 );

		viewer->addRenderable( root );
}
