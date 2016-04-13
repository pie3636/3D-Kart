#ifndef KEYFRAMED_KART_RENDERABLE_HPP
#define KEYFRAMED_KART_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "KeyframeCollection.hpp"
#include "./lighting/Material.hpp"
#include "./lighting/Light.hpp"

#include <glm/glm.hpp>

class GeometricTransformation;

class KeyframedKartRenderable : public HierarchicalRenderable
{
  public:
    ~KeyframedKartRenderable();
    KeyframedKartRenderable(
        ShaderProgramPtr program,
        const std::string& mesh_filename,
        const std::string& texture_filename );
    void setMaterial(const MaterialPtr& material);


    void addLocalTransformKeyframe( const GeometricTransformation& transformation, float time );

    void addParentTransformKeyframe( const GeometricTransformation& transformation, float time );

private:
    void do_draw();

    void do_animate( float time );

    KeyframeCollection m_localKeyframes;
    KeyframeCollection m_parentKeyframes;

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec3 > m_normals;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec2 > m_texCoords;
    std::vector< unsigned int > m_indices;

    unsigned int m_pBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    unsigned int m_iBuffer;
    unsigned int m_tBuffer;
    unsigned int m_texId;

    MaterialPtr m_material;
};

typedef std::shared_ptr<KeyframedKartRenderable> KeyframedKartRenderablePtr;
#endif
