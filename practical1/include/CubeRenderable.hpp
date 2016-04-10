#ifndef CUBE_RENDERABLE_HPP
#define CUBE_RENDERABLE_HPP

#include "HierarchicalRenderable.hpp"
#include <vector>
#include <glm/glm.hpp>

class CubeRenderable : public HierarchicalRenderable
{
    public:
        ~CubeRenderable();
        CubeRenderable();
        CubeRenderable( ShaderProgramPtr program);

    private:
        void push_triangle(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz, double Ar, double Ag, double Ab, double Aa);
        void do_draw();
        void do_animate( float time );

        std::vector< glm::vec3 > m_positions;
        std::vector< glm::vec4 > m_colors;
        unsigned int m_vBuffer;
        unsigned int m_cBuffer;
};

typedef std::shared_ptr<CubeRenderable> CubeRenderablePtr;

#endif
