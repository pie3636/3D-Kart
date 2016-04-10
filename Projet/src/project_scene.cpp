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
    std::string         vShader = "./../shaders/flatVertex.glsl";
    std::string         fShader = "./../shaders/flatFragment.glsl";
    ShaderProgramPtr    parentProg = std::make_shared<ShaderProgram>(vShader, fShader);
    viewer.addShaderProgram(parentProg);

    MashRenderablePtr   meshKart = createKartFromMesh();
    CubeRenderable      primKart = createKartFromPrimitives();
    CylinderRenderable  primChar = createCharacterFromPrimitives();
    
    viewer.addRenderable(meshKart);
    viewer.addRenderable(primKart);
    viewer.addRenderable(primChar);
}

MeshRenderablePtr createKartFromMesh(void) {
    return std::make_shared<MeshRenderable>(parentProg, "../Kart.obj");
}

CubeRenderablePtr createKartFromPrimitives(void) {
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

CylinderRenderablePtr createCharacterFromPrimitives(void) {
    /*
    Position (x, y, z) = centre
    x = largeur, y = profondeur, z = hauteur. Tout donné par rapport au centre du parent. Racine = torse du personnage
    Mesures = au repos (debout, bras et jambes le long du corps)
    z positif vers le haut
                                            x       z       y
    N°  Partie                  Primitive   Width   Height  Depth       posx            posz                        posy

        Body                    ----------------------------------------------------------------------------------------------------
    A   | Head                  Sphere      0.2     0.25    0.2         0               (zC+zB+zA)/2                0
    B   | Neck                  Cylinder    0.12    0.1     0.1         0               (zC+zB)/2                   0
        | Torso                 ----------------------------------------------------------------------------------------------------
    C   | | Chest               Cylinder    0.3     0.6     0.25        0               0                           0
        | | Arm x2              ----------------------------------------------------------------------------------------------------
    D   | | | Shoulder          Sphere      0.1     0.1     0.1         (xC+xD)/2       (zC-zD)/2                   0
    E   | | | Upper arm         Cylinder    0.1     0.2     0.1         (xC+xD)/2       (zC-zD-zE)/2                0
    F   | | | Elbow             Sphere      0.1     0.1     0.1         (xC+xD)/2       (zC-zD-zE-zF)/2             0
    G   | | | Forearm           Cylinder    0.1     0.25    0.1         (xC+xD)/2       (zC-zD-zE-zF-zG)/2          0
    H   | | | Wrist             Sphere      0.08    0.08    0.08        (xC+xD)/2       (zC-zD-zE-zF-zG-zH)/2       0
    I   | | | Hand              Sphere      0.1     0.18    0.05        (xC+xD)/2       (zC-zD-zE-zF-zG-zH-zI)/2    0
        | Bottom                ----------------------------------------------------------------------------------------------------
        | | Leg x2              ----------------------------------------------------------------------------------------------------
    J   | | | Upper leg         Cylinder    0.15    0.3     0.15        ±(xC-xJ)/2      (-zC-zJ)/2                  0
    K   | | | Knee              Sphere      0.12    0.12    0.12        ±(xC-xJ)/2      (-zC-zJ-zK)/2               0
    L   | | | Calf              Cylinder    0.12    0.25    0.12        ±(xC-xJ)/2      (-zC-zJ-zK-zL)/2            0
    M   | | | Ankle             Sphere      0.08    0.08    0.08        ±(xC-xJ)/2      (-zC-zJ-zK-zL-zM)/2         0
    N   | | | Foot              Sphere      0.08    0.08    0.2         ±(xC-xJ)/2      (-zC-zJ-zK-zL-zM-zN)/2      (±)(yM+yN)/2

    */
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
