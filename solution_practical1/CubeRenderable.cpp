#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>


CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram)
  : Renderable(shaderProgram), m_vBuffer(0), m_cBuffer(0)
{
    //Create cube geometry : 12 faces
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));

    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    //Assign one color to each face
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(1,0,0,1));

    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));

    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,0,1,1));

    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));

    m_colors.push_back(glm::vec4(1,0,1,1));
    m_colors.push_back(glm::vec4(1,0,1,1));
    m_colors.push_back(glm::vec4(1,0,1,1));

    m_colors.push_back(glm::vec4(1,1,0,1));
    m_colors.push_back(glm::vec4(1,1,0,1));
    m_colors.push_back(glm::vec4(1,1,0,1));

    m_colors.push_back(glm::vec4(0,0.5,0.5,1));
    m_colors.push_back(glm::vec4(0,0.5,0.5,1));
    m_colors.push_back(glm::vec4(0,0.5,0.5,1));

    m_colors.push_back(glm::vec4(0.5,0,0.5,1));
    m_colors.push_back(glm::vec4(0.5,0,0.5,1));
    m_colors.push_back(glm::vec4(0.5,0,0.5,1));

    m_colors.push_back(glm::vec4(0.5,0.5,0,1));
    m_colors.push_back(glm::vec4(0.5,0.5,0,1));
    m_colors.push_back(glm::vec4(0.5,0.5,0,1));

    m_colors.push_back(glm::vec4(0.8,0.2,0,1));
    m_colors.push_back(glm::vec4(0.8,0.2,0,1));
    m_colors.push_back(glm::vec4(0.8,0.2,0,1));

    m_colors.push_back(glm::vec4(0.2,0,0.8,1));
    m_colors.push_back(glm::vec4(0.2,0,0.8,1));
    m_colors.push_back(glm::vec4(0.2,0,0.8,1));

    m_colors.push_back(glm::vec4(0,0.8,0.2,1));
    m_colors.push_back(glm::vec4(0,0.8,0.2,1));
    m_colors.push_back(glm::vec4(0,0.8,0.2,1));

    //Create buffers
    glGenBuffers(1, &m_vBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
}

void CubeRenderable::do_draw()
{
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(m_model)));

    glcheck(glEnableVertexAttribArray(positionLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    glcheck(glEnableVertexAttribArray(colorLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorLocation));
}

void CubeRenderable::do_animate(float time) {}

CubeRenderable::~CubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
}
