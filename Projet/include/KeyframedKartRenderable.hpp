#ifndef KEYFRAMED_KART_RENDERABLE_HPP
#define KEYFRAMED_KART_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "KeyframeCollection.hpp"
#include "../include/texturing/TexturedLightedMeshRenderable.hpp"
#include "./lighting/Material.hpp"
#include "./lighting/Light.hpp"

#include <glm/glm.hpp>

class GeometricTransformation;

class KeyframedKartRenderable : public TexturedLightedMeshRenderable
{
  public:
    ~KeyframedKartRenderable();
    KeyframedKartRenderable(
        ShaderProgramPtr program,
        const std::string& mesh_filename,
        const std::string& texture_filename);

    void addLocalTransformKeyframe( const GeometricTransformation& transformation, float time );

    void addParentTransformKeyframe( const GeometricTransformation& transformation, float time );

private:
    void do_animate(float time);

    KeyframeCollection m_localKeyframes; /*!< A collection of keyframes for the local transformation of renderable. */
    KeyframeCollection m_parentKeyframes; /*!< A collection of keyframes for the parent transformation of renderable. */
};

typedef std::shared_ptr<KeyframedKartRenderable> KeyframedKartRenderablePtr;
#endif
