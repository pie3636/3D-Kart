#include "./../include/IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram) : Renderable(shaderProgram)
{
    //Create geometry : 8 vertices, 12 faces, one color per vertex
    m_positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    m_positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    m_positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    m_positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));
    m_colors.push_back(glm::vec4(1,0,0,1));
    m_colors.push_back(glm::vec4(0,1,0,1));
    m_colors.push_back(glm::vec4(0,0,1,1));
    m_colors.push_back(glm::vec4(0,1,1,1));

    m_indices.push_back(glm::ivec3(0, 2, 1));
    m_indices.push_back(glm::ivec3(0, 3, 2));
    m_indices.push_back(glm::ivec3(1, 6, 7));
    m_indices.push_back(glm::ivec3(1, 2, 6));
    m_indices.push_back(glm::ivec3(5, 4, 7));
    m_indices.push_back(glm::ivec3(5, 7, 6));
    m_indices.push_back(glm::ivec3(4, 3, 0));
    m_indices.push_back(glm::ivec3(4, 5, 3));
    m_indices.push_back(glm::ivec3(3, 6, 2));
    m_indices.push_back(glm::ivec3(3, 5, 6));
    m_indices.push_back(glm::ivec3(4, 0, 1));
    m_indices.push_back(glm::ivec3(4, 1, 7));

    //Create buffers
    glGenBuffers(1, &m_vBuffer); //vertices
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_iBuffer); //indices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(glm::ivec3), m_indices.data(), GL_STATIC_DRAW));
}

void IndexedCubeRenderable::do_draw()
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
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size()*3, GL_UNSIGNED_INT, (void*)0));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorLocation));
}

void IndexedCubeRenderable::do_animate(float time) {}

IndexedCubeRenderable::~IndexedCubeRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_iBuffer));
}
