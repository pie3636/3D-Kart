#ifndef CYLINDER_RENDERABLE_HPP
#define CYLINDER_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include "SphereRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class CylinderRenderable : public HierarchicalRenderable
{
public:
    ~CylinderRenderable();
    CylinderRenderable(ShaderProgramPtr program, int facets = 30, SphereRenderablePtr articulation = nullptr, SphereRenderablePtr articulation2 = nullptr);
    
    void switchNormals();
    
    SphereRenderablePtr getArticulation(void) {return m_articulation;};
    SphereRenderablePtr getArticulation2(void) {return m_articulation2;};
    void setArticulation(SphereRenderablePtr sph) {this->m_articulation = sph;};
    void setArticulation2(SphereRenderablePtr sph) {this->m_articulation2 = sph;};

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;
    std::vector< glm::vec3 > m_normals;

    unsigned int m_vBuffer;
    unsigned int m_cBuffer;
    unsigned int m_nBuffer;
    
    SphereRenderablePtr m_articulation;
    SphereRenderablePtr m_articulation2;
};

typedef std::shared_ptr<CylinderRenderable> CylinderRenderablePtr;

#endif
