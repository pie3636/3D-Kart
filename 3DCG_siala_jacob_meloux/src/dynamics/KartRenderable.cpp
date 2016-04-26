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
		init = rotate(glm::mat4(1.0), float (M_PI), glm::vec3(0, 0, 1))
			 * rotate(glm::mat4(1.0), float (M_PI/2), glm::vec3(1, 0, 0))
			 * rotate(glm::mat4(1.0), float (-M_PI/2-0.15), glm::vec3(0, 1, 0))
			 * scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5));
		this->kart = kart;
		kart->setPosition(glm::vec3(3, 19, 0));
		setParentTransform(glm::translate(glm::mat4(1.0), kart->getPosition()) * init);
		
		old_angle = 0;
    }

void KartRenderable::do_draw()
{
    // Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const glm::vec3& pPosition = kart->getPosition();
    
    const glm::vec3& velocity = kart->getVelocity();
    float alpha = atan2(velocity.y, velocity.x);
    
    glm::mat4 translate = glm::translate(glm::mat4(1.0), pPosition);
    glm::mat4 rotate 	= glm::rotate(glm::mat4(1.0), alpha, glm::vec3(0, 0, 1));
    
    kart->getCharacter()->getArticulation()->setParentTransform(glm::rotate(glm::mat4(1.0), float(M_PI/6 + 10*(old_angle - alpha)), glm::vec3(1, 0, 0)));
    kart->getCharacter()->getArticulation2()->setParentTransform(glm::rotate(glm::mat4(1.0), float(M_PI/6 + 10*(alpha - old_angle)), glm::vec3(1, 0, 0)));
    
    kart->setAngle(alpha);
    
    setParentTransform(translate * rotate * init);

	old_angle = alpha;
    
    TexturedLightedMeshRenderable::do_draw();
}


KartRenderable::~KartRenderable()
{}
