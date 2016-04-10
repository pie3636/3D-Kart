#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram) :
    HierarchicalRenderable(shaderProgram), m_vBuffer(0), m_cBuffer(0) {
        push_triangle(0,0,0, 1,0,0, 0,1,0, 0,0,0,0);
        push_triangle(1,1,0, 1,0,0, 0,1,0, 0,0,0,0);
        push_triangle(1,0,1, 1,0,0, 1,1,0, 0.2,0.2,0.2,0);
        push_triangle(1,0,1, 1,1,1, 1,1,0, 0.2,0.2,0.2,0);
        push_triangle(0,1,0, 1,1,0, 0,1,1, 0.3,0.3,0.3,0);
        push_triangle(1,1,1, 1,1,0, 0,1,1, 0.3,0.3,0.3,0);
        push_triangle(0,0,0, 0,0,1, 0,1,0, 0.5,0.5,0.5,0);
        push_triangle(0,1,1, 0,0,1, 0,1,0, 0.5,0.5,0.5,0);
        push_triangle(0,0,1, 1,0,1, 0,1,1, 0.7,0.7,0.7,0);
        push_triangle(1,1,1, 1,0,1, 0,1,1, 0.7,0.7,0.7,0);
        push_triangle(0,0,0, 1,0,0, 0,0,1, 1,1,1,0);
        push_triangle(1,0,1, 1,0,0, 0,0,1, 1,1,1,0);
        m_model = glm::mat4(1.0);

        glcheck(glGenBuffers(1, &m_vBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

        glcheck(glGenBuffers(1, &m_cBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    }

        void CubeRenderable::push_triangle(double Ax, double Ay, double Az, double Bx, double By, double Bz, double Cx, double Cy, double Cz, double Ar, double Ag, double Ab, double Aa) {
        m_positions.push_back(glm::vec3(Ax, Ay, Az));
        m_positions.push_back(glm::vec3(Bx, By, Bz));
        m_positions.push_back(glm::vec3(Cx, Cy, Cz));

        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
}

void CubeRenderable::do_draw() {
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");
    glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));

    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    glcheck(glEnableVertexAttribArray(positionLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    int colorIn = m_shaderProgram->getAttributeLocation("colorIn");
    glcheck(glEnableVertexAttribArray(colorIn));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glVertexAttribPointer(colorIn, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

    glcheck(glDrawArrays(GL_TRIANGLES, 0, m_positions.size()));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorIn));
}

void CubeRenderable::do_animate(float time) {}

CubeRenderable::~CubeRenderable() {
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}
