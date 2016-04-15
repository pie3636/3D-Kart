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
		init = rotate(glm::mat4(1.0),float (M_PI/2), glm::vec3(1, 0, 0))
			 * rotate(glm::mat4(1.0),float (-M_PI/2-0.15), glm::vec3(0, 1, 0))
			 * scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5))
			 * glm::translate(glm::mat4(1.0), glm::vec3(-35, 5., -2.));
		this->kart = kart;
		setParentTransform(glm::translate(glm::mat4(1.0), kart->getPosition())*init);
    }

void KartRenderable::do_draw()
{
    // Update the parent and local transform matrix to position the geometric data according to the particle's data.
    const glm::vec3& pPosition = kart->getPosition();
    auto tmp = kart->getVelocity();
    float alpha = -atan2(kart->getVelocity().y, kart->getVelocity().x);
	std::cout << "FORCE " << tmp.x << " " << tmp.y << " " << tmp.z << "\n";
	std::cout << "ANGLE " << alpha << "\n";
    
    glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(-2*pPosition.x, 2*pPosition.y, -2*pPosition.z));
    glm::mat4 rotate 	= glm::rotate(glm::mat4(1.0), -alpha, glm::vec3(0, 0, 1));
    
    setParentTransform(translate * rotate * init);
    
    TexturedLightedMeshRenderable::do_draw();
}


KartRenderable::~KartRenderable()
{}
