#include "../include/project_scene.hpp"

int Scene::kartCount = 0;
const double Scene::kartScaleFactor = 0.5;

//float Scene::times[16] = {1.25, 1.5, 1.6, 1.75, 1.9, 2.5, 3, 4, 4.25, 4.75, 5.5, 6, 6.25, 7, 7.5, 8};
float Scene::times[16] = {1.5, 4, 4.3, 4.5, 5, 6.5, 8, 9, 9.7, 11.2, 13.2, 14.7, 16.7, 18.7, 21.7, 23.2};

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
    kart_game_road();

	KeyframedKartRenderablePtr    meshKart2 = createTexturedMovingKartFromMesh();

	KartRenderablePtr mobileRenderable = createControllableKart();

	// Place the camera
	viewer->getCamera().setViewMatrix(glm::lookAt(glm::vec3(-25, -25, 60), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1)));

	viewer->startAnimation();
	viewer->setAnimationLoop(true, 50.0);
}

Scene::~Scene() {}

KeyframedKartRenderablePtr Scene::createTexturedMovingKartFromMesh() {
	std::string tex[] = {"metal wall2.jpg", "mur_pierre.jpeg", "wood.jpg", "grass_texture.png"};
	KartPtr realKart = std::make_shared<Kart>(glm::vec3(3, 19, 0), glm::vec3(0, 0, 0), 1, 5, 2, 1.5, float(M_PI));
	realKart->setFixed(true);
    KeyframedKartRenderablePtr kart = std::make_shared<KeyframedKartRenderable>(texShader, "../meshes/Kart.obj", "../textures/" + tex[kartCount], realKart);
    kart->setMaterial(Material::Pearl());
	kart->setParentTransform(rotate(kart, M_PI/2, 1, 0, 0)
						   * rotate(kart, -M_PI/2 - 0.15, 0, 1, 0)
						   * scale(kart, kartScaleFactor, kartScaleFactor, kartScaleFactor)
						   * translate(kart, -32, 1., -2.));
    HierarchicalRenderable::addChild(systemRenderable, 	kart							);
    HierarchicalRenderable::addChild(kart, 				createCharacterFromPrimitives() );
    dynSystem->addKart(realKart);
	return moving_kart(kart, texShader, "../meshes/Kart.obj", "../textures/wood.jpg");
}

KartRenderablePtr Scene::createControllableKart() {
	std::string tex[] = {"grass_texture.png", "mur_pierre.jpeg", "wood.jpg", "metal wall2.jpg"};
 	KartPtr mobile = std::make_shared<Kart>(glm::vec3(3, 19, 0), glm::vec3(0, 0, 0), 1, 5, 2, 1.5, 0.0);
 	mobile->setCharacter(createCharacterFromPrimitives());
    dynSystem->addKart(mobile);
    kartCount++;
	KartRenderablePtr mobileRenderable = std::make_shared<KartRenderable>(texShader, mobile, "../meshes/Kart.obj", "../textures/" + tex[kartCount]);
	HierarchicalRenderable::addChild(systemRenderable, mobileRenderable);
    HierarchicalRenderable::addChild(mobileRenderable, mobile->getCharacter());
    // Initialize a force field that applies only to the mobile kart
    glm::vec3 nullForce(0.0,0.0,0.0);
    std::vector<KartPtr> vKart;
    vKart.push_back(mobile);
    
    ConstantForceFieldPtr force = std::make_shared<ConstantForceField>(vKart, nullForce);
    dynSystem->addForceField(force);
    // Initialize a renderable for the force field applied on the mobile particle.
    // This renderable allows to modify the attribute of the force by key/mouse events
    // Add this renderable to the systemRenderable.
    ControlledForceFieldRenderablePtr forceRenderable = std::make_shared<ControlledForceFieldRenderable>(texShader, force, viewer);
    HierarchicalRenderable::addChild(systemRenderable, forceRenderable);
    
	ConstantForceFieldPtr gravityForceField = std::make_shared<ConstantForceField>(dynSystem->getKarts(), glm::vec3{0,0,-10});
	dynSystem->addForceField(gravityForceField);
	
	// Add a damping force field to the mobile.
    DampingForceFieldPtr dampingForceField = std::make_shared<DampingForceField>(vKart, 0.9);
    dynSystem->addForceField(dampingForceField);
	return mobileRenderable;
}

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
    glm::vec3 neckS     (0.12, 0.1,  0.2);
    glm::vec3 headS     (0.2,  0.2,  0.25);
    glm::vec3 shoulderS (0.1,  0.1,  0.1);
    glm::vec3 upperArmS (0.1,  0.1,  0.3);
    glm::vec3 elbowS    (0.1,  0.1,  0.1);
    glm::vec3 forearmS  (0.1,  0.1,  0.35);
    glm::vec3 wristS    (0.08, 0.08, 0.08);
    glm::vec3 handS     (0.1,  0.05, 0.18);
    glm::vec3 hipS      (0.12, 0.12, 0.12);
    glm::vec3 upperLegS (0.15, 0.15, 0.46);
    glm::vec3 kneeS     (0.12, 0.12, 0.12);
    glm::vec3 calfS     (0.12, 0.12, 0.41);
    glm::vec3 ankleS    (0.08, 0.08, 0.08);
    glm::vec3 footS     (0.08, 0.2,  0.08);

    // Translation vectors (see ./struct/Personnage)
    glm::vec3 chestT        (0,                         0,                      0                           );
    glm::vec3 neckT         (0,                         0,                      chestS.z      			    );
    glm::vec3 headT         (0,                         0,                      headS.z/2 + neckS.z         );
    glm::vec3 shoulderLT    (-chestS.x - shoulderS.x,   0,                      chestS.z - shoulderS.z      );
    glm::vec3 upperArmLT    (0,       					0,                      -upperArmS.z - shoulderS.z/2);
    glm::vec3 elbowLT       (0,       					0,                      -elbowS.z/2                 );
    glm::vec3 forearmLT     (0,       					0,                      -forearmS.z - elbowS.z/2    );
    glm::vec3 wristLT       (0,       					0,                      -wristS.z/2            		);
    glm::vec3 handLT        (0,       					0,                      -wristS.z/2 - handS.z 		);
    glm::vec3 shoulderRT    (chestS.x + shoulderS.x,    0,                      chestS.z - shoulderS.z      );
    glm::vec3 upperArmRT    (0,       					0,                      -upperArmS.z - shoulderS.z/2);
    glm::vec3 elbowRT       (0,       					0,                      -elbowS.z/2                 );
    glm::vec3 forearmRT     (0,       					0,                      -forearmS.z - elbowS.z/2    );
    glm::vec3 wristRT       (0,       					0,                      -wristS.z/2            		);
    glm::vec3 handRT        (0,       					0,                      -wristS.z/2 - handS.z 		);
    glm::vec3 hipLT    		(upperLegS.x - chestS.x,    0,                      -hipS.z/2                   );
    glm::vec3 upperLegLT    (0,    						0,                      -hipS.z/2 - upperLegS.z     );
    glm::vec3 kneeLT        (0,    						0,                      -kneeS.z/2                  );
    glm::vec3 calfLT        (0,    						0,                      -kneeS.z/2 - calfS.z        );
    glm::vec3 ankleLT       (0,    						0,                      -ankleS.z/2                 );
    glm::vec3 footLT        (0,    						(ankleS.x + footS.x)/2,	-ankleS.z/2 - footS.z/2     );
    glm::vec3 hipRT    		(chestS.x - upperLegS.x,    0,                      -hipS.z/2                   );
    glm::vec3 upperLegRT    (0,    						0,                      -hipS.z/2 - upperLegS.z     );
    glm::vec3 kneeRT        (0,    						0,                      -kneeS.z/2                  );
    glm::vec3 calfRT        (0,    						0,                      -kneeS.z/2 - calfS.z        );
    glm::vec3 ankleRT       (0,    						0,                      -ankleS.z/2					);
    glm::vec3 footRT        (0,    						(ankleS.x + footS.x)/2, -ankleS.z/2 - footS.z/2		);

    // Apply transforms (standing position)
    chest       ->setParentTransform(translate(chest,        chestT     ));
    neck        ->setParentTransform(translate(neck,         neckT      ));
    head        ->setParentTransform(translate(head,         headT      ));
    shoulder_l  ->setParentTransform(translate(shoulder_l,   shoulderLT ));
    upperArm_l  ->setParentTransform(translate(upperArm_l,   upperArmLT ));
    elbow_l     ->setParentTransform(translate(elbow_l,      elbowLT    ));
    forearm_l   ->setParentTransform(translate(forearm_l,    forearmLT  ));
    wrist_l     ->setParentTransform(translate(wrist_l,      wristLT    ));
    hand_l      ->setParentTransform(translate(hand_l,       handLT     ));
    shoulder_r  ->setParentTransform(translate(shoulder_r,   shoulderRT ));
    upperArm_r  ->setParentTransform(translate(upperArm_r,   upperArmRT ));
    elbow_r     ->setParentTransform(translate(elbow_r,      elbowRT    ));
    forearm_r   ->setParentTransform(translate(forearm_r,    forearmRT  ));
    wrist_r     ->setParentTransform(translate(wrist_r,      wristRT    ));
    hand_r      ->setParentTransform(translate(hand_r,       handRT     ));
    hip_l  		->setParentTransform(translate(hip_l,   	 hipLT 	   ));
    upperLeg_l  ->setParentTransform(translate(upperLeg_l,   upperLegLT ));
    knee_l      ->setParentTransform(translate(knee_l,       kneeLT     ));
    calf_l      ->setParentTransform(translate(calf_l,       calfLT     ));
    ankle_l     ->setParentTransform(translate(ankle_l,      ankleLT    ));
    foot_l      ->setParentTransform(translate(foot_l,       footLT     ));
    hip_r  		->setParentTransform(translate(hip_r,   	 hipRT 	   ));
    upperLeg_r  ->setParentTransform(translate(upperLeg_r,   upperLegRT ));
    knee_r      ->setParentTransform(translate(knee_r,       kneeRT     ));
    calf_r      ->setParentTransform(translate(calf_r,       calfRT     ));
    ankle_r     ->setParentTransform(translate(ankle_r,      ankleRT    ));
    foot_r      ->setParentTransform(translate(foot_r,       footRT     ));

    chest		->setLocalTransform(scale(chest,        chestS     ));
    neck        ->setLocalTransform(scale(neck,         neckS      ));
    head        ->setLocalTransform(scale(head,         headS      ));
    shoulder_l  ->setLocalTransform(scale(shoulder_l,   shoulderS  ));
    upperArm_l  ->setLocalTransform(scale(upperArm_l,   upperArmS  ));
    elbow_l     ->setLocalTransform(scale(elbow_l,      elbowS     ));
    forearm_l   ->setLocalTransform(scale(forearm_l,    forearmS   ));
    wrist_l     ->setLocalTransform(scale(wrist_l,      wristS     ));
    hand_l      ->setLocalTransform(scale(hand_l,       handS      ));
    shoulder_r  ->setLocalTransform(scale(shoulder_r,   shoulderS  ));
    upperArm_r  ->setLocalTransform(scale(upperArm_r,   upperArmS  ));
    elbow_r     ->setLocalTransform(scale(elbow_r,      elbowS     ));
    forearm_r   ->setLocalTransform(scale(forearm_r,    forearmS   ));
    wrist_r     ->setLocalTransform(scale(wrist_r,      wristS     ));
    hand_r      ->setLocalTransform(scale(hand_r,       handS      ));
    hip_l  		->setLocalTransform(scale(hip_l,   	   hipS  	  ));
    upperLeg_l  ->setLocalTransform(scale(upperLeg_l,   upperLegS  ));
    knee_l      ->setLocalTransform(scale(knee_l,       kneeS      ));
    calf_l      ->setLocalTransform(scale(calf_l,       calfS      ));
    ankle_l     ->setLocalTransform(scale(ankle_l,      ankleS     ));
    foot_l      ->setLocalTransform(scale(foot_l,       footS      ));
    hip_r  		->setLocalTransform(scale(hip_r,   	   hipS  	  ));
    upperLeg_r  ->setLocalTransform(scale(upperLeg_r,   upperLegS  ));
    knee_r      ->setLocalTransform(scale(knee_r,       kneeS      ));
    calf_r      ->setLocalTransform(scale(calf_r,       calfS      ));
    ankle_r     ->setLocalTransform(scale(ankle_r,      ankleS     ));
    foot_r      ->setLocalTransform(scale(foot_r,       footS      ));

    // Move character to have a proper posture in the kart
    double characterScale = 1.5;
    chest		->setParentTransform(translateParent(chest, 0, 0.5, 0)   						// Kart positioning
						    * rotateParent(chest, M_PI, 0, 1, 0) 									// Face the right direction
    						* rotateParent(chest, -M_PI/4, 1, 0, 0) 								// Inclination in kart seat
    						* scaleParent(chest, characterScale, characterScale, characterScale));	// Scale adaptation
    neck		->setParentTransform(rotateParent(neck, -M_PI/4, 1, 0, 0)); 						// Restore head inclination
    shoulder_l	->setParentTransform(rotateParent(shoulder_l, M_PI/6, 1, 0, 0));					// Arms relaxed
    shoulder_r	->setParentTransform(rotateParent(shoulder_r, M_PI/6, 1, 0, 0));
    elbow_l		->setParentTransform(rotateParent(elbow_l, M_PI/6, 1, 0, 0));						// Forearms aiming at the steering wheel
    elbow_r		->setParentTransform(rotateParent(elbow_r, M_PI/6, 1, 0, 0));
    wrist_l		->setParentTransform(rotateParent(wrist_l, M_PI/6, 1, 0, 0));						// Hands on the steering wheel (kind of)
    wrist_r		->setParentTransform(rotateParent(wrist_r, M_PI/6, 1, 0, 0));
    hip_l		->setParentTransform(rotateParent(hip_l, M_PI/4, 1, 0, 0));							// Horizontal legs
    hip_r		->setParentTransform(rotateParent(hip_r, M_PI/4, 1, 0, 0));
    knee_l		->setParentTransform(rotateParent(knee_l, -M_PI/4, 1, 0, 0));						// Calves are following the curve of the body
    knee_r		->setParentTransform(rotateParent(knee_r, -M_PI/4, 1, 0, 0));

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
    
    chest->setArticulation(elbow_l);
    chest->setArticulation2(elbow_r);

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
  glm::vec3 lightPosition(0.0, 0.0, 5.0);
  DirectionalLightRenderablePtr directionalLightRenderable = std::make_shared<DirectionalLightRenderable>(flatShader, directionalLight, glm::vec3(0.0,0.0,5.0));
  localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
  directionalLightRenderable->setLocalTransform(localTransformation);
  viewer->setDirectionalLight(directionalLight);
  viewer->addRenderable(directionalLightRenderable);
  
	// Define a spot light
    glm::vec3 s_position(0.0, 49.0, 8.0), s_spotDirection = glm::normalize(glm::vec3(0.0, 0.0, -1.0));
    glm::vec3 s_ambient(0.5, 0.5, 0.5), s_diffuse(0.5, 0.5, 0.5), s_specular(0.5, 0.5, 0.5);
    float s_constant = 1.0, s_linear = 0.0, s_quadratic = 0.0;
    float s_innerCutOff = std::cos(glm::radians(20.0f)), s_outerCutOff = std::cos(glm::radians(40.0f));
    SpotLightPtr spotLight = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                         s_ambient, s_diffuse, s_specular,
                                                         s_constant, s_linear, s_quadratic,
                                                         s_innerCutOff, s_outerCutOff);
    SpotLightRenderablePtr spotLightRenderable = std::make_shared<SpotLightRenderable>(flatShader, spotLight);
    localTransformation = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
    spotLightRenderable->setLocalTransform(localTransformation);
    viewer->addSpotLight(spotLight);
    viewer->addRenderable(spotLightRenderable);
    
    s_position.x += 10;
    SpotLightPtr spotLight2 = std::make_shared<SpotLight>(s_position, s_spotDirection,
                                                          s_ambient, s_diffuse, s_specular,
                                                          s_constant, s_linear, s_quadratic,
                                                          s_innerCutOff, s_outerCutOff);
    SpotLightRenderablePtr spotLightRenderable2 = std::make_shared<SpotLightRenderable>(flatShader, spotLight2);
    spotLightRenderable2->setLocalTransform(localTransformation);
    viewer->addSpotLight(spotLight2);
    viewer->addRenderable(spotLightRenderable2);
}

void Scene::kart_game_borders() {
  // Temporary variables
  glm::mat4 parentTransformation(1.0);
  std::string filename;
  MaterialPtr pearl = Material::Pearl();

  // Textured plane
  glm::vec3 p1(-50.0, -50.0, -0.2), p2(50.0, -50.0, -0.2), p3(50.0, 50.0, -0.2), p4(-50.0, 50.0, -0.2);
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

  p1 = glm::vec3(50.0, -50.0, 0.0);
  p2 = glm::vec3(50.0, 50.0,  0.0);
  p3 = glm::vec3(50.0, 50.0,  5.0);
  plane = std::make_shared<Plane>(p3, p2, p1); // Ordre important pour les collisions
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall1 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(50.0, 0.0, 0.0));
  wall1->setParentTransform(parentTransformation);
  wall1->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall1);

  p1 = glm::vec3(-50.0, -50.0, 0.0);
  p2 = glm::vec3(-50.0, 50.0,  0.0);
  p3 = glm::vec3(-50.0, 50.0,  5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall2 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation = glm::translate(glm::mat4(1.0), glm::vec3(-50.0, 0.0, 0.0));
  wall2->setParentTransform(parentTransformation);
  wall2->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall2);

  p1 = glm::vec3(-50.0, 50.0, 0.0);
  p2 = glm::vec3(50.0,  50.0, 0.0);
  p3 = glm::vec3(50.0,  50.0, 5.0);
  plane = std::make_shared<Plane>(p1, p2, p3);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall3 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation = glm::rotate(glm::mat4(1.0), float (M_PI/2), glm::vec3(0.0, 0.0, 1.0));
  parentTransformation *= glm::translate(glm::mat4(1.0), glm::vec3(50.0, 0.0, 0.0));
  wall3->setParentTransform(parentTransformation);
  wall3->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall3);

  p1 = glm::vec3(-50.0, -50.0, 0.0);
  p2 = glm::vec3(50.0,  -50.0, 0.0);
  p3 = glm::vec3(50.0,  -50.0, 5.0);
  plane = std::make_shared<Plane>(p3, p2, p1);
  dynSystem->addPlaneObstacle(plane);
  TexturedPlaneRenderablePtr wall4 = std::make_shared<TexturedPlaneRenderable>(texShader, filename,1);
  parentTransformation = glm::rotate(glm::mat4(1.0), float (-M_PI/2), glm::vec3(0.0,0.0,1.0));
  parentTransformation *= glm::translate(glm::mat4(1.0), glm::vec3(50.0,0.0,0.0));
  wall4->setParentTransform(parentTransformation);
  wall4->setMaterial(pearl);
  HierarchicalRenderable::addChild(systemRenderable, wall4);

  TexturedPlaneRenderablePtr ad = std::make_shared<TexturedPlaneRenderable>(texShader, "./../textures/michelin.jpeg", 2);
  ad->setMaterial(pearl);
  viewer->addRenderable(ad);
  
  TexturedPlaneRenderablePtr ad2 = std::make_shared<TexturedPlaneRenderable>(texShader, "./../textures/coca.png", 3);
  ad2->setMaterial(pearl);
  viewer->addRenderable(ad2);

}


void Scene::kart_game_road(){
  glm::vec4 color = glm::vec4(0.5,0.5,0.5,1.0);
  glm::vec3 x1, x2, x3, x4;

  PlaneRenderablePtr portion;

  // Ligne de depart
  x1=glm::vec3(-1., 21.75, 0.2);
  x2=glm::vec3(-1.,  15, 0.2);
  x3=glm::vec3(-0.15,  14.75, 0.2);
  x4=glm::vec3(-0.15, 21.75, 0.2);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, glm::vec4(1.,1.,1.,1.));
  viewer->addRenderable(portion);
  x1=glm::vec3(0.15, 21.75, 0.2);
  x2=glm::vec3(0.15, 14.75, 0.2);
  x3=glm::vec3(1., 14.5, 0.2);
  x4=glm::vec3(1.,21.5, 0.2);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, glm::vec4(1.,1.,1.,1.));
  viewer->addRenderable(portion);

  // 1ere ligne droite
  x1=glm::vec3(15.5, -12.5, 0.1);
  x2=glm::vec3(22.5, -15., 0.1);
  x3=glm::vec3(22.5, 7.5, 0.1);
  x4=glm::vec3(15.5, 5., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer->addRenderable(portion);
/*
	for (int i=-9; i<7; i++) {
		HierarchicalTorusRenderablePtr torus1 = std::make_shared<HierarchicalTorusRenderable>(flatShader);
		HierarchicalTorusRenderablePtr torus2 = std::make_shared<HierarchicalTorusRenderable>(flatShader);
		torus1->setParentTransform(glm::translate(glm::mat4(1.0),glm::vec3(23,2*i,0.5)));
		torus2->setParentTransform(glm::translate(glm::mat4(1.0),glm::vec3(0,0,0.5)));
		HierarchicalRenderable::addChild(systemRenderable, torus1);
		HierarchicalRenderable::addChild(torus1, torus2);
}*/

  // 2eme ligne droite
  x1 = glm::vec3( 11.5, -22.5, 0.1);
  x2 = glm::vec3( 13., -15.5, 0.1);
  x3 = glm::vec3( 2.5, -1.25, 0.1);
  x4 = glm::vec3( 2.5, -11.25, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer->addRenderable(portion);

  // 3eme ligne droite
  x1=glm::vec3(-12.5, 24., 0.1);
  x2=glm::vec3(-12.5, 17., 0.1);
  x3=glm::vec3(13., 12.5, 0.1);
  x4=glm::vec3(15., 19., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4,  color);
  viewer->addRenderable(portion);

  // 4eme ligne droite
  x1=glm::vec3(-15.5, 12.5, 0.1);
  x2=glm::vec3(-22.5, 15., 0.1);
  x3=glm::vec3(-22.5, -17.5, 0.1);
  x4=glm::vec3(-15.5, -15., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4,  color);
  viewer->addRenderable(portion);

  // 5eme ligne droite
  x1 = glm::vec3( -5., -1.5, 0.1);
  x2 = glm::vec3( -5., -10.25, 0.1);
  x3 = glm::vec3( -13., -22.5, 0.1);
  x4 = glm::vec3( -13., -12.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer->addRenderable(portion);
/*
  RoadRenderablePtr virage1 = std::make_shared<RoadRenderable>( flatShader, 2 );
  viewer->addRenderable(virage1);
*/

//-----------------------------------------------------//
  glm::vec3 s1, s2, s3, s4;
  glm::vec3 t1, t2, t3, t4;

  // 1er virage
  // Points de controle de bezier
  s1 = glm::vec3( 22.5, -15., 0.1);
  s2 = glm::vec3( 22.5, -20., 0.1);
  s3 = glm::vec3( 17.5, -25., 0.1);
  s4 = glm::vec3(11.5, -22.5, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(-6.25, 4.5, 0.);
  t4 = glm::vec3(15.5, -12.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(-4.5, 6., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4,  color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(-1., 7.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4,  color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = glm::vec3(13., -15.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4,  color);
  viewer->addRenderable(portion);

  // 2eme virage
  // Points de controle de bezier
  s1 = glm::vec3( 2.5, -11.25, 0.1);
  s2 = glm::vec3( -0.5, -7., 0.1);
  s3 = glm::vec3( -2., -7., 0.1);
  s4 = glm::vec3( -5., -10.25, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(0.25, 8.5, 0.);
  t4 = glm::vec3( 2.5, -1.25, 0.1 );
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(0.25, 8.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(-0.5, 8.25, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = glm::vec3( -5., -1.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);

  // 3eme virage
  // Points de controle de bezier
  s1 = glm::vec3( -13., -22.5, 0.);
  s2 = glm::vec3( -15, -25., 0.1);
  s3 = glm::vec3( -20., -22.5, 0.1);
  s4 = glm::vec3( -22.5, -17.5, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(1.25, 9.5, 0.);
  t4 = glm::vec3( -13., -12.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(3., 8., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(4.75, 6., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = glm::vec3(-15.5, -15., 0.1); ;
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);

  // 4eme virage
  // Points de controle de bezier
  s1 = glm::vec3(-15.5, 12.5, 0.1);
  s2 = glm::vec3( -16., 16.5, 0.1);
  s3 = glm::vec3( -14., 16.5, 0.1);
  s4 = glm::vec3(-12.5, 17., 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(-6., 4.5, 0.);
  t4 = glm::vec3(-22.5, 15., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.5);
  t4 = t3;
  t3 = t2 + glm::vec3(-4.5, 6.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(-1.5, 7.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = glm::vec3(-12.5, 24., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);

  // 5eme virage
  // Points de controle de bezier
  s1 = glm::vec3(13., 12.5, 0.1);
  s2 = glm::vec3(14.5, 12. , 0.1);
  s3 = glm::vec3(16., 6.5, 0.1);
  s4 = glm::vec3(15.5, 5., 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(5.,6., 0.);
  t4 = glm::vec3(15., 19., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(5.5, 6.25, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(6.25, 5.75, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = glm::vec3(22.5, 7.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer->addRenderable(portion);
}


KeyframedKartRenderablePtr Scene::moving_kart(KeyframedKartRenderablePtr root,
				ShaderProgramPtr program,
				const std::string& mesh_filename,
				const std::string& texture_filename )
{
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,0}), times[0] );
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0,33}), times[1]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{2.5,0,35}, glm::quat( glm::vec3{0.0,  0.5, 0.} )), times[2]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{5,0,40}, glm::quat( glm::vec3{0.0,  1., 0.} )), times[3]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{10,0,40}, glm::quat( glm::vec3{0.0,  1.75, 0.} )), times[4]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{60,0,33}, glm::quat( glm::vec3{0.0,  8., 0.} )), times[5] );
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{67,0,25}, glm::quat( glm::vec3{0.0,  10., 0.} )), times[6]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{40,0,10}, glm::quat( glm::vec3{0.0,  -8., 0.} )), times[7]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{38,0,5}, glm::quat( glm::vec3{0.0,  10., 0.} )), times[8]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{38,0,0}, glm::quat( glm::vec3{0.0,  15., 0.} )), times[9]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{60,0,-25}, glm::quat( glm::vec3{0.0,  -10., 0.} )), times[10]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{65,0,-33}, glm::quat( glm::vec3{0.0,  -15., 0.} )), times[11]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{50,0,-35}, glm::quat( glm::vec3{0.0,  -8., 0.} )), times[12] );
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{3,0, -25}, glm::quat( glm::vec3{0.0,  -1., 0.} )), times[13]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0, 10}, glm::quat( glm::vec3{0.0,  0., 0.} )), times[14]);
		root->addParentTransformKeyframe( GeometricTransformation( glm::vec3{0,0, 10}, glm::quat( glm::vec3{0.0,  0., 0.} )), times[15]);

		return root ;
}
