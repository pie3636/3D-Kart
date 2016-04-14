#include "../include/KeyframedKartRenderable.hpp"
#include "../include/gl_helper.hpp"
#include "../include/GeometricTransformation.hpp"
#include "../include/Utils.hpp"
#include "./../../include/log.hpp"
#include "./../../include/Io.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

KeyframedKartRenderable::KeyframedKartRenderable(
  ShaderProgramPtr shaderProgram, const std::string& mesh_filename, const std::string& texture_filename) :
    TexturedLightedMeshRenderable(shaderProgram, mesh_filename, texture_filename) {}

void KeyframedKartRenderable::addLocalTransformKeyframe( const GeometricTransformation& transformation, float time ) {
  m_localKeyframes.add( transformation, time );
}

void KeyframedKartRenderable::addParentTransformKeyframe( const GeometricTransformation& transformation, float time ) {
  m_parentKeyframes.add( transformation, time );
}

void KeyframedKartRenderable::do_animate( float time )
{
    glm::mat4 init = rotate(glm::mat4(1.0),float (M_PI/2), glm::vec3(1, 0, 0)) * rotate(glm::mat4(1.0),float (-M_PI/2-0.15), glm::vec3(0, 1, 0))  *
                           scale(glm::mat4(1.0), glm::vec3(0.5, 0.5, 0.5)) * translate(glm::mat4(1.0), glm::vec3(-32, 1., -2.));
    //Assign the interpolated transformations from the keyframes to the local/parent transformations.
    if(!m_localKeyframes.empty()) {
        setLocalTransform( init*m_localKeyframes.interpolateTransformation( time ) );
    }
    if(!m_parentKeyframes.empty()) {
        setParentTransform(init* m_parentKeyframes.interpolateTransformation( time ) );
    }
}

KeyframedKartRenderable::~KeyframedKartRenderable() {}
