#include "./../include/MeshRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Io.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

MeshRenderable::MeshRenderable( 
        ShaderProgramPtr shaderProgram, 
        const std::string& filename) 
: HierarchicalRenderable(shaderProgram)
{

    std::vector<glm::vec2> tmp;
    read_obj(filename, m_positions, m_indices, m_normals, tmp);
    for (int i = 0; i < m_indices.size(); i++) {
        //m_colors.push_back(randomColor());
        m_colors.push_back(glm::normalize(glm::vec4(m_normals[i/3].x, m_normals[i/3].y, m_normals[i/3].z, 1)));
    }

    //Create buffers
    glGenBuffers(1, &m_vBuffer); //positions
    glGenBuffers(1, &m_cBuffer); //colors
    glGenBuffers(1, &m_nBuffer); //normals
    glGenBuffers(1, &m_iBuffer); //indices

    //Activate buffer and send data to the graphics card
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size()*sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW));
}

void MeshRenderable::do_draw()
{
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("colorIn");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");

    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));

    glcheck(glEnableVertexAttribArray(positionLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer));
    glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    glcheck(glEnableVertexAttribArray(colorLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
    glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));

    glcheck(glEnableVertexAttribArray(normalLocation));
    glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
    glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));

    //Draw triangles elements
    glcheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iBuffer));
    glcheck(glDrawElements(GL_TRIANGLES, m_indices.size()*3, GL_UNSIGNED_INT, (void*)0));

    glcheck(glDisableVertexAttribArray(positionLocation));
    glcheck(glDisableVertexAttribArray(colorLocation));
    glcheck(glDisableVertexAttribArray(normalLocation));
}

void MeshRenderable::do_animate(float time) {}

MeshRenderable::~MeshRenderable()
{
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
    glcheck(glDeleteBuffers(1, &m_iBuffer));
}
