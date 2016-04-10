#include "./../include/IndexedCubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

IndexedCubeRenderable::IndexedCubeRenderable(ShaderProgramPtr shaderProgram) :
    HierarchicalRenderable(shaderProgram), m_vBuffer(0), m_cBuffer(0), m_iBuffer(0) {
        // Sommet
        m_positions.push_back(glm::vec3(0,0,0));
        m_positions.push_back(glm::vec3(0,0,1));
        m_positions.push_back(glm::vec3(0,1,0));
        m_positions.push_back(glm::vec3(0,1,1));
        m_positions.push_back(glm::vec3(1,0,0));
        m_positions.push_back(glm::vec3(1,0,1));
        m_positions.push_back(glm::vec3(1,1,0));
        m_positions.push_back(glm::vec3(1,1,1));

        // Face avant
        m_indices.push_back(0); m_indices.push_back(1); m_indices.push_back(2);
        m_indices.push_back(3); m_indices.push_back(1); m_indices.push_back(2);
        // Face arrière
        m_indices.push_back(4); m_indices.push_back(5); m_indices.push_back(6);
        m_indices.push_back(7); m_indices.push_back(5); m_indices.push_back(6);
        // Face bas
        m_indices.push_back(0); m_indices.push_back(1); m_indices.push_back(4);
        m_indices.push_back(5); m_indices.push_back(1); m_indices.push_back(4);
        // Face haut
        m_indices.push_back(2); m_indices.push_back(3); m_indices.push_back(6);
        m_indices.push_back(7); m_indices.push_back(3); m_indices.push_back(6);
        // Face gauche
        m_indices.push_back(0); m_indices.push_back(2); m_indices.push_back(4);
        m_indices.push_back(6); m_indices.push_back(2); m_indices.push_back(4);
        // Face droite
        m_indices.push_back(1); m_indices.push_back(3); m_indices.push_back(5);
        m_indices.push_back(7); m_indices.push_back(3); m_indices.push_back(5);

        // Couleurs
        m_colors.push_back(glm::vec4(1,0,0,0));
        m_colors.push_back(glm::vec4(0,1,0,0));
        m_colors.push_back(glm::vec4(0,0,1,0));
        m_colors.push_back(glm::vec4(1,1,0,0));
        m_colors.push_back(glm::vec4(1,0,1,0));
        m_colors.push_back(glm::vec4(0,1,1,0));
        m_colors.push_back(glm::vec4(1,1,1,0));
        m_colors.push_back(glm::vec4(1,0.5,0,0));
        m_colors.push_back(glm::vec4(0,1,0.5,0));
        m_colors.push_back(glm::vec4(1,0.5,0,0));
        m_colors.push_back(glm::vec4(1,0,0.5,0));
        m_colors.push_back(glm::vec4(0.5,1,0,0));
        m_model = glm::mat4(1.0);

        glcheck(glGenBuffers(1, &m_vBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

        glcheck(glGenBuffers(1, &m_cBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));

        glcheck(glGenBuffers(1, &m_iBuffer));
        glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
        glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW));
    }

void IndexedCubeRenderable::do_draw() {
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

    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorIn));
}

void IndexedCubeRenderable::do_animate(float time) {}

IndexedCubeRenderable::~IndexedCubeRenderable() {
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_iBuffer));
}
