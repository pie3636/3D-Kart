#ifndef KART_RENDERABLE_HPP
#define KART_RENDERABLE_HPP

#include "../HierarchicalRenderable.hpp"
#include "Kart.hpp"
#include "../texturing/TexturedLightedMeshRenderable.hpp"
#include "../lighting/Material.hpp"
#include "../lighting/Light.hpp"

#include <glm/glm.hpp>

class GeometricTransformation;

class KartRenderable : public TexturedLightedMeshRenderable
{
  public:
    ~KartRenderable();
    KartRenderable(
        ShaderProgramPtr program, KartPtr kart,
        const std::string& mesh_filename,
        const std::string& texture_filename);
        const KartPtr getKart() const {return kart;};

  private:
    void do_draw();
    KartPtr kart;
    
    float old_angle;
    
    glm::mat4 init;
};

typedef std::shared_ptr<KartRenderable> KartRenderablePtr;
#endif
