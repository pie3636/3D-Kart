#include "./../../include/dynamics/KartRenderable.hpp"
#include "./../../include/gl_helper.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Utils.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

KartRenderable::KartRenderable(
  ShaderProgramPtr shaderProgram, KartPtr kart, const std::string& mesh_filename, const std::string& texture_filename) :
    TexturedLightedMeshRenderable(shaderProgram, mesh_filename, texture_filename) {
      glm::mat4 init = rotate(glm::mat4(1.0),float (M_PI/2), glm::vec3(1, 0, 0)) * rotate(glm::mat4(1.0),float (-M_PI/2-0.15), glm::vec3(0, 1, 0)) *
                            scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5)) * translate(glm::mat4(1.0), glm::vec3(0., 1., 0.));
      setParentTransform(glm::translate(glm::mat4(1.0),kart->getPosition())*init);

    }

void KartRenderable::do_draw()
{
    //kart->setPosition(glm::vec3(0,0,0));
    TexturedLightedMeshRenderable::do_draw();
}


KartRenderable::~KartRenderable()
{}
