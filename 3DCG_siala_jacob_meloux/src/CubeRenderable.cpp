#include "./../include/CubeRenderable.hpp"
#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

CubeRenderable::CubeRenderable(ShaderProgramPtr shaderProgram) :
    HierarchicalRenderable(shaderProgram), m_vBuffer(0), m_cBuffer(0) {
    
        std::vector<glm::vec2> tmp_tex;
        getUnitCube(m_positions, m_normals, tmp_tex);
        
        // Push colors
        push_triangle(0,0,0,0);
        push_triangle(0,0,0,0);
        push_triangle(0.2,0.2,0.2,0);
        push_triangle(0.2,0.2,0.2,0);
        push_triangle(0.3,0.3,0.3,0);
        push_triangle(0.3,0.3,0.3,0);
        push_triangle(0.5,0.5,0.5,0);
        push_triangle(0.5,0.5,0.5,0);
        push_triangle(0.7,0.7,0.7,0);
        push_triangle(0.7,0.7,0.7,0);
        push_triangle(1,1,1,0);
        push_triangle(1,1,1,0);
        m_model = glm::mat4(1.0);

        // Vertices
        glcheck(glGenBuffers(1, &m_vBuffer)); // Create buffer
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer)); // Activate buffer and send data to the graphics card
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_positions.size()*sizeof(glm::vec3), m_positions.data(), GL_STATIC_DRAW));

        // Colors
        glcheck(glGenBuffers(1, &m_cBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_colors.size()*sizeof(glm::vec4), m_colors.data(), GL_STATIC_DRAW));
        
        // Normals
        glcheck(glGenBuffers(1, &m_nBuffer));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glBufferData(GL_ARRAY_BUFFER, m_normals.size()*sizeof(glm::vec3), m_normals.data(), GL_STATIC_DRAW));
}

void CubeRenderable::push_triangle(double Ar, double Ag, double Ab, double Aa) {
        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
        m_colors.push_back(glm::vec4(Ar, Ag, Ab, Aa));
}

void CubeRenderable::do_draw() {
    // Location
    int positionLocation = m_shaderProgram->getAttributeLocation("vPosition");
    int colorLocation = m_shaderProgram->getAttributeLocation("vColor");
    int normalLocation = m_shaderProgram->getAttributeLocation("vNormal");
    int modelLocation = m_shaderProgram->getUniformLocation("modelMat");

    // Send data to GPU
    if(modelLocation != ShaderProgram::null_location) {
        glcheck(glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(getModelMatrix())));
    }

    if(positionLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(positionLocation)); // Activate location
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_vBuffer)); // Bind buffer
        glcheck(glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0)); // Specify internal format
    }

    if(colorLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(colorLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_cBuffer));
        glcheck(glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    if(normalLocation != ShaderProgram::null_location) {
        glcheck(glEnableVertexAttribArray(normalLocation));
        glcheck(glBindBuffer(GL_ARRAY_BUFFER, m_nBuffer));
        glcheck(glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)0));
    }

    //Draw triangles elements
    glcheck(glDrawArrays(GL_TRIANGLES,0, m_positions.size()));

    if(positionLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(positionLocation));
    }
    if(colorLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(colorLocation));
    }
    if(normalLocation != ShaderProgram::null_location) {
        glcheck(glDisableVertexAttribArray(normalLocation));
    }
}

void CubeRenderable::do_animate(float time) {}

CubeRenderable::~CubeRenderable() {
    glcheck(glDeleteBuffers(1, &m_vBuffer));
    glcheck(glDeleteBuffers(1, &m_cBuffer));
    glcheck(glDeleteBuffers(1, &m_nBuffer));
}
