#ifndef CYLINDER_RENDERABLE_HPP
#define CYLINDER_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class CylinderRenderable : public HierarchicalRenderable
{
public:
    ~CylinderRenderable();
    CylinderRenderable(ShaderProgramPtr program, int facets = 30);

private:
    void do_draw();
    void do_animate( float time );

    std::vector< glm::vec3 > m_positions;
    std::vector< glm::vec4 > m_colors;

    unsigned int m_vBuffer;
    unsigned int m_cBuffer;
};

typedef std::shared_ptr<CylinderRenderable> CylinderRenderablePtr;

#endif
