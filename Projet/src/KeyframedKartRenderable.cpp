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
    //Assign the interpolated transformations from the keyframes to the local/parent transformations.
    if(!m_localKeyframes.empty()) {
        setLocalTransform( m_localKeyframes.interpolateTransformation( time ) );
    }
    if(!m_parentKeyframes.empty()) {
        setParentTransform( m_parentKeyframes.interpolateTransformation( time ) );
    }
}

KeyframedKartRenderable::~KeyframedKartRenderable() {}
