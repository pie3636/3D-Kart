#ifndef HIERARCHICAL_TORUS_RENDERABLE_HPP
#define HIERARCHICAL_TORUS_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class HierarchicalTorusRenderable : public HierarchicalRenderable
{
    public:
        ~HierarchicalTorusRenderable();
        HierarchicalTorusRenderable( ShaderProgramPtr program );

    private:
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        std::vector< glm::vec3 > m_normals;

        unsigned int m_pBuffer;
        unsigned int m_cBuffer;
        unsigned int m_nBuffer;
};

typedef std::shared_ptr<HierarchicalTorusRenderable> HierarchicalTorusRenderablePtr;

#endif
