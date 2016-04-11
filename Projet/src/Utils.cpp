#include "./../include/Utils.hpp"
#include "./../include/log.hpp"
#include "../include/QuadRenderable.hpp"
#include "./../include/Bezier.hpp"

#include <chrono>
#include <random>
#include <glm/gtx/color_space.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// A random engine initialized on the current time.
// This way, the random numbers generated won't be the same at each execution.
static std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count() );

float random(float a, float b)
{
    uniform_real_distribution<> distribution(a,b);
    return distribution(generator);
}

glm::vec4 randomColor()
{
    uniform_real_distribution<> distribution(0,360);
    float hue = distribution(generator), saturation = 1.0, black = 1.0;
    glm::vec3 hsvColor( hue, saturation, black);
    glm::vec3 rgbColor = glm::rgbColor(hsvColor);
    return glm::vec4(rgbColor[0], rgbColor[1], rgbColor[2], 1.0);
}

glm::vec4 getColor(const float& factor, const float& low, const float& high)
{
    float hue = 0.0, saturation = 1.0, black = 1.0;
    if(factor<low) hue = 240.0;
    else if(factor>high) hue = 0.0;
    else hue = 240 + (factor-low)/(high-low)*(0.0-240.0);
    glm::vec3 hsvColor( hue, saturation, black);
    glm::vec3 rgbColor = glm::rgbColor(hsvColor);
    return glm::vec4(rgbColor[0], rgbColor[1], rgbColor[2], 1.0);
}

void getUnitPlane(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords)
{
    positions.resize(6, glm::vec3(0.0,0.0,0.0));
    texCoords.resize(6, glm::vec2(0.0,0.0));
    normals.resize(6, glm::vec3(0.0,0.0,1.0));

    //First triangle
    positions[0] =glm::vec3(-0.5,-0.5,0.0);
    positions[1] =glm::vec3(0.5,-0.5,0.0);
    positions[2] =glm::vec3(0.5,0.5,0.0);

    //TODO
    texCoords[0] =glm::vec2(0.0,0.0);
    texCoords[1] =glm::vec2(1.0,0.0);
    texCoords[2] =glm::vec2(1.0,1.0);

    //Second triangle
    positions[3] =glm::vec3(-0.5,-0.5,0.0);
    positions[4] =glm::vec3(0.5,0.5,0.0);
    positions[5] =glm::vec3(-0.5,0.5,0.0);

    texCoords[3] =glm::vec2(0.0,0.0);
    texCoords[4] =glm::vec2(1.0,1.0);
    texCoords[5] =glm::vec2(0.0,1.0);
}

void getUnitIndexedCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::ivec3>& indices)
{
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    for(const glm::vec3& x : positions)
    {
        normals.push_back( glm::normalize(x) );
    }

    indices.push_back(glm::ivec3(0, 2, 1));
    indices.push_back(glm::ivec3(0, 3, 2));
    indices.push_back(glm::ivec3(1, 6, 7));
    indices.push_back(glm::ivec3(1, 2, 6));
    indices.push_back(glm::ivec3(5, 4, 7));
    indices.push_back(glm::ivec3(5, 7, 6));
    indices.push_back(glm::ivec3(4, 3, 0));
    indices.push_back(glm::ivec3(4, 5, 3));
    indices.push_back(glm::ivec3(3, 6, 2));
    indices.push_back(glm::ivec3(3, 5, 6));
    indices.push_back(glm::ivec3(4, 0, 1));
    indices.push_back(glm::ivec3(4, 1, 7));
}

void getUnitCube(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2>& texCoords)
{
    //Face 1
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));

    //Face 2
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));
    normals.push_back(glm::vec3(0.0,0.0,-1.0));

    //Face 3
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));

    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));

    //Face 4
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));
    normals.push_back(glm::vec3(1.0,0.0,0.0));

    //Face 5
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));

    //Face 6
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));
    normals.push_back(glm::vec3(0.0,0.0,1.0));

    //Face 7
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));

    //Face 8
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));

    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));
    normals.push_back(glm::vec3(-1.0,0.0,0.0));

    //Face 9
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, -0.5));

    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));

    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));

    //Face 10
    positions.push_back(glm::vec3(-0.5, 0.5, -0.5));
    positions.push_back(glm::vec3(-0.5, 0.5, 0.5));
    positions.push_back(glm::vec3(0.5, 0.5, 0.5));

    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));

    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));
    normals.push_back(glm::vec3(0.0,1.0,0.0));

    //Face 11
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(-0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));

    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));

    //Face 12
    positions.push_back(glm::vec3(-0.5, -0.5, 0.5));
    positions.push_back(glm::vec3(0.5, -0.5, -0.5));
    positions.push_back(glm::vec3(0.5, -0.5, 0.5));

    texCoords.push_back(glm::vec2(0.0,1.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
    normals.push_back(glm::vec3(0.0,-1.0,0.0));
}

void getUnitCylinder(std::vector<glm::vec3> &positions, std::vector<glm::vec3> &normals, const unsigned int &slices)
{
    size_t number_of_triangles = slices * 4;
    size_t number_of_vertices = number_of_triangles * 3;
    float angle_step = 2.0 * M_PI / double( slices );

    positions.resize(number_of_vertices, glm::vec3(0.0,0.0,0.0));
    normals.resize(number_of_vertices, glm::vec3(0.0,0.0,0.0));

    float previous_angle = (slices - 1) * angle_step;
    float angle = 0;
    float previous_sin = std::sin( previous_angle );
    float previous_cos = std::cos( previous_angle );
    float sin = 0;
    float cos = 1;
    for( size_t i = 0; i < slices;
            ++ i,
            previous_sin = sin, previous_cos = cos,
            angle += angle_step, cos = std::cos( angle ), sin = std::sin( angle) )
    {
        size_t voffset = 12 * i;
        positions[ voffset +  0 ] = glm::vec3{0,0,1};
        positions[ voffset +  1 ] = glm::vec3{previous_cos, previous_sin, 1};
        positions[ voffset +  2 ] = glm::vec3{cos, sin,1};

        positions[ voffset +  3 ] = glm::vec3{cos, sin,1};
        positions[ voffset +  4 ] = glm::vec3{previous_cos, previous_sin, 1};
        positions[ voffset +  5 ] = glm::vec3{previous_cos, previous_sin, 0};

        positions[ voffset +  6 ] = glm::vec3{cos, sin,1};
        positions[ voffset +  7 ] = glm::vec3{previous_cos, previous_sin, 0};
        positions[ voffset +  8 ] = glm::vec3{cos, sin,0};

        positions[ voffset +  9 ] = glm::vec3{cos, sin,0};
        positions[ voffset + 10 ] = glm::vec3{previous_cos, previous_sin, 0};
        positions[ voffset + 11 ] = glm::vec3{0, 0, 0}; //useless

        normals[ voffset +  0 ] = glm::vec3{0,0,1};
        normals[ voffset +  1 ] = glm::vec3{0,0,1};
        normals[ voffset +  2 ] = glm::vec3{0,0,1};

        normals[ voffset +  3 ] = glm::vec3{cos,sin,0};
        normals[ voffset +  4 ] = glm::vec3{previous_cos,previous_sin,0};
        normals[ voffset +  5 ] = glm::vec3{previous_cos,previous_sin,0};

        normals[ voffset +  6 ] = glm::vec3{cos,sin,0};
        normals[ voffset +  7 ] = glm::vec3{previous_cos,previous_sin,0};
        normals[ voffset +  8 ] = glm::vec3{cos,sin,0};

        normals[ voffset +  9 ] = glm::vec3{0,0,-1};
        normals[ voffset + 10 ] = glm::vec3{0,0,-1};
        normals[ voffset + 11 ] = glm::vec3{0,0,-1};
    }
}

void getUnitSphere(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices)
{
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);

    unsigned int vertexNumber = slices*strips*6;
    positions.resize(vertexNumber);
    normals.resize(vertexNumber);

    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_phi = j*(M_PI/(double)strips);

            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_phi = (j+1)*(M_PI/(double)strips);

            std::array<glm::vec3,3> vTriangles;
            vTriangles[0] = center + glm::vec3(radius*cos(curr_theta)*sin(curr_phi), radius*sin(curr_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[1] = center + glm::vec3(radius*cos(next_theta)*sin(curr_phi), radius*sin(next_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[2] = center + glm::vec3(radius*cos(next_theta)*sin(next_phi), radius*sin(next_theta)*sin(next_phi), radius*cos(next_phi));

            positions[i*strips*6 + 6*j +0] = vTriangles[0];
            positions[i*strips*6 + 6*j +1] = vTriangles[1];
            positions[i*strips*6 + 6*j +2] = vTriangles[2];

            normals[i*strips*6 + 6*j +0] = glm::vec3(cos(curr_theta)*sin(curr_phi), sin(curr_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +1] = glm::vec3(cos(next_theta)*sin(curr_phi), sin(next_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +2] = glm::vec3(cos(next_theta)*sin(next_phi), sin(next_theta)*sin(next_phi), cos(next_phi));

            vTriangles[0] = center + glm::vec3(radius*cos(curr_theta)*sin(curr_phi), radius*sin(curr_theta)*sin(curr_phi), radius*cos(curr_phi));
            vTriangles[1] = center + glm::vec3(radius*cos(next_theta)*sin(next_phi), radius*sin(next_theta)*sin(next_phi), radius*cos(next_phi));
            vTriangles[2] = center + glm::vec3(radius*cos(curr_theta)*sin(next_phi), radius*sin(curr_theta)*sin(next_phi), radius*cos(next_phi));

            positions[i*strips*6 + 6*j +3] = vTriangles[0];
            positions[i*strips*6 + 6*j +4] = vTriangles[1];
            positions[i*strips*6 + 6*j +5] = vTriangles[2];

            normals[i*strips*6 + 6*j +3] = glm::vec3(cos(curr_theta)*sin(curr_phi), sin(curr_theta)*sin(curr_phi), cos(curr_phi));
            normals[i*strips*6 + 6*j +4] = glm::vec3(cos(next_theta)*sin(next_phi), sin(next_theta)*sin(next_phi), cos(next_phi));
            normals[i*strips*6 + 6*j +5] = glm::vec3(cos(curr_theta)*sin(next_phi), sin(curr_theta)*sin(next_phi), cos(next_phi));
        }
    }
}

void getTorus(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const float& a, const float& c, const unsigned int& strips, const unsigned int& slices)
{
    unsigned int vertexNumber = slices*strips*6;
    positions.resize(vertexNumber);
    normals.resize(vertexNumber);

    for(int i=0; i<strips; ++i)
    {
        for(int j=0; j<slices; ++j)
        {
            double curr_u = i*(2.0*M_PI/(double)strips);
            double curr_v = j*(2.0*M_PI/(double)slices);

            double next_u = (i+1)*(2.0*M_PI/(double)strips);
            double next_v = (j+1)*(2.0*M_PI/(double)slices);

            positions[i*strips*6 + 6*j +0] = glm::vec3((c+a*cos(curr_v))*cos(curr_u),
                    (c+a*cos(curr_v))*sin(curr_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +1] = glm::vec3((c+a*cos(curr_v))*cos(next_u),
                    (c+a*cos(curr_v))*sin(next_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +2] = glm::vec3((c+a*cos(next_v))*cos(next_u),
                    (c+a*cos(next_v))*sin(next_u),
                    a*sin(next_v) );

            normals[i*strips*6 + 6*j +0] = glm::vec3((cos(curr_v))*cos(curr_u),
                    (cos(curr_v))*sin(curr_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +1] = glm::vec3((cos(curr_v))*cos(next_u),
                    (cos(curr_v))*sin(next_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +2] = glm::vec3((cos(next_v))*cos(next_u),
                    (cos(next_v))*sin(next_u),
                    sin(next_v) );

            positions[i*strips*6 + 6*j +3] = glm::vec3((c+a*cos(curr_v))*cos(curr_u),
                    (c+a*cos(curr_v))*sin(curr_u),
                    a*sin(curr_v) );
            positions[i*strips*6 + 6*j +4] = glm::vec3((c+a*cos(next_v))*cos(next_u),
                    (c+a*cos(next_v))*sin(next_u),
                    a*sin(next_v) );
            positions[i*strips*6 + 6*j +5] = glm::vec3((c+a*cos(next_v))*cos(curr_u),
                    (c+a*cos(next_v))*sin(curr_u),
                    a*sin(next_v) );

            normals[i*strips*6 + 6*j +3] = glm::vec3((cos(curr_v))*cos(curr_u),
                    (cos(curr_v))*sin(curr_u),
                    sin(curr_v) );
            normals[i*strips*6 + 6*j +4] = glm::vec3((cos(next_v))*cos(next_u),
                    (cos(next_v))*sin(next_u),
                    sin(next_v) );
            normals[i*strips*6 + 6*j +5] = glm::vec3((cos(next_v))*cos(curr_u),
                    (cos(next_v))*sin(curr_u),
                    sin(next_v) );
        }
    }
}

void getUnitCone(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, const unsigned int& strips, const unsigned int& slices)
{
    double height = 1.0;
    double radius=1.0;
    glm::vec3 center(0.0,0.0,0.0);
    int numberOfVertices = slices*strips*6 + slices*3;
    positions.resize(numberOfVertices);
    normals.resize(numberOfVertices);

    //Lateral surface
    for(int i=0; i<slices; ++i)
    {
        for(int j=0; j<strips; ++j)
        {
            double curr_theta = i*(2.0*M_PI/(double)slices);
            double curr_u = j*(height/(double)strips);
            double next_theta = (i+1)*(2.0*M_PI/(double)slices);
            double next_u = (j+1)*(height/(double)strips);
            double factor1 = (height-curr_u)/height;
            double factor2 = (height-next_u)/height;

            positions[i*strips*6 + 6*j +0] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +1] = center + glm::vec3(factor1*radius*cos(next_theta),
                    factor1*radius*sin(next_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +2] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);

            normals[i*strips*6 + 6*j +0] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +1] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +2] = glm::normalize(glm::vec3(cos(next_theta),
                        sin(next_theta),
                        next_u));

            positions[i*strips*6 + 6*j +3] = center + glm::vec3(factor1*radius*cos(curr_theta),
                    factor1*radius*sin(curr_theta),
                    curr_u);
            positions[i*strips*6 + 6*j +4] = center + glm::vec3(factor2*radius*cos(next_theta),
                    factor2*radius*sin(next_theta),
                    next_u);
            positions[i*strips*6 + 6*j +5] = center + glm::vec3(factor2*radius*cos(curr_theta),
                    factor2*radius*sin(curr_theta),
                    next_u);

            normals[i*strips*6 + 6*j +3] = glm::normalize(glm::vec3(cos(curr_theta),
                        sin(curr_theta),
                        curr_u));
            normals[i*strips*6 + 6*j +4] = glm::normalize(glm::vec3(cos(next_theta),
                        radius*sin(next_theta),
                        next_u));
            normals[i*strips*6 + 6*j +5] = glm::normalize(glm::vec3(cos(curr_theta),
                        radius*sin(curr_theta),
                        next_u));

        }
    }

    //Bottom faces
    unsigned int counter = slices*strips*6;
    for(int i=0; i<slices; ++i)
    {
        double curr_theta = i*(2.0*M_PI/(double)slices);
        double next_theta = (i+1)*(2.0*M_PI/(double)slices);
        double u_base = 0.0;

        positions[counter+3*i+0] = center + glm::vec3(0, 0, u_base );
        positions[counter+3*i+1] = center + glm::vec3(radius*cos(curr_theta), radius*sin(curr_theta), u_base);
        positions[counter+3*i+2] = center + glm::vec3(radius*cos(next_theta), radius*sin(next_theta), u_base);

        normals[counter+3*i+0] = glm::vec3(0, 0, -1);
        normals[counter+3*i+1] = glm::vec3(0, 0, -1);
        normals[counter+3*i+2] = glm::vec3(0, 0, -1);
    }

}

void getGround(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords)
{
    positions.push_back(glm::vec3(-2.5,-2.5,0.0));
    positions.push_back(glm::vec3(2.5,-2.5,0.0));
    positions.push_back(glm::vec3(2.5,2.5,0.0));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));

    //Second triangle
    positions.push_back(glm::vec3(-2.5,-2.5,0.0));
    positions.push_back(glm::vec3(2.5,2.5,0.0));
    positions.push_back(glm::vec3(-2.5,2.5,0.0));

    texCoords.push_back(glm::vec2(0.0,0.0));
    texCoords.push_back(glm::vec2(1.0,1.0));
    texCoords.push_back(glm::vec2(0.0,1.0));

    for (int i=0; i<positions.size(); i++)
      normals.push_back(glm::vec3(0.0,0.0,1.0));
}

void getWall(std::vector<glm::vec3>& positions, std::vector<glm::vec3>& normals, std::vector<glm::vec2> &texCoords)
{

    for (int i=0; i<10; i++) {
      //Plane x=0
      //First triangle
      positions.push_back(glm::vec3(0.0,-25.0+5.0*i,0.0));
      positions.push_back(glm::vec3(0.0,-25.0+5.0*(i+1),0.0));
      positions.push_back(glm::vec3(0.0,-25.0+5.0*(i+1),5.0));
      texCoords.push_back(glm::vec2(0.0,0.0));
      texCoords.push_back(glm::vec2(1.0,0.0));
      texCoords.push_back(glm::vec2(1.0,1.0));
      //Second triangle
      positions.push_back(glm::vec3(0.0,-25.0+5.0*i,0.0));
      positions.push_back(glm::vec3(0.0,-25.0+5.0*(i+1),5.0));
      positions.push_back(glm::vec3(0.0,-25.0+5.0*i,5.0));
      texCoords.push_back(glm::vec2(0.0,0.0));
      texCoords.push_back(glm::vec2(1.0,1.0));
      texCoords.push_back(glm::vec2(0.0,1.0));
      for (int i=0; i<6; i++)
        normals.push_back(glm::vec3(1.0,0.0,0.0));
    }
}

void getRoad(ShaderProgramPtr flatShader, Viewer& viewer){
  glm::vec4 color = glm::vec4(0.5,0.5,0.5,1.0);
  glm::vec3 x1, x2, x3, x4;

  PlaneRenderablePtr portion;

  // 1ere ligne droite
  x1=glm::vec3(17.5, -12.5, 0.1);
  x2=glm::vec3(22.5, -15., 0.1);
  x3=glm::vec3(22.5, 7.5, 0.1);
  x4=glm::vec3(17.5, 5., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer.addRenderable(portion);

  // 2eme ligne droite
  x1 = glm::vec3( 12.5, -22.5, 0.1);
  x2 = glm::vec3( 14., -16.5, 0.1);
  x3 = glm::vec3( 2.5, -2.75, 0.1);
  x4 = glm::vec3( 2.5, -10.25, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer.addRenderable(portion);

  // 3eme ligne droite
  x1=glm::vec3(-12.5, 22.5, 0.1);
  x2=glm::vec3(-12.5, 17.5, 0.1);
  x3=glm::vec3(13., 12.5, 0.1);
  x4=glm::vec3(15., 17.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer.addRenderable(portion);

  // 4eme ligne droite
  x1=glm::vec3(-17.5, 12.5, 0.1);
  x2=glm::vec3(-22.5, 15., 0.1);
  x3=glm::vec3(-22.5, -17.5, 0.1);
  x4=glm::vec3(-17.5, -15., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer.addRenderable(portion);

  // 5eme ligne droite
  x1 = glm::vec3( -5., -2.75, 0.1);
  x2 = glm::vec3( -5., -10.25, 0.1);
  x3 = glm::vec3( -12.5, -22.5, 0.1);
  x4 = glm::vec3( -14.5, -17.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, x1, x2, x3, x4, color);
  viewer.addRenderable(portion);
/*
  RoadRenderablePtr virage1 = std::make_shared<RoadRenderable>( flatShader, 2 );
  viewer.addRenderable(virage1);
*/

//-----------------------------------------------------//
  glm::vec3 s1, s2, s3, s4;
  glm::vec3 t1, t2, t3, t4;

  // 1er virage
  // Points de controle de bezier
  s1 = glm::vec3( 22.5, -15., 0.1);
  s2 = glm::vec3( 22.5, -20., 0.1);
  s3 = glm::vec3( 17.5, -25., 0.1);
  s4 = glm::vec3( 12.5, -22.5, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(-4.5, 4.5, 0.);
  t4 = glm::vec3(17.5, -12.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(-3., 5.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(-1., 6., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = t2 + glm::vec3(1.5, 6., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);

  // 2eme virage
  // Points de controle de bezier
  s1 = glm::vec3(  2.5, -10.25, 0.1);
  s2 = glm::vec3( -0.5, -7., 0.1);
  s3 = glm::vec3( -2., -7., 0.1);
  s4 = glm::vec3( -5., -10.25, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(0., 6.5, 0.);
  t4 = glm::vec3(2.5, -2.75, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(0., 6.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(0., 6.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = t2 + glm::vec3(0., 7.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);

  // 3eme virage
  // Points de controle de bezier
  s1 = glm::vec3( -12.5, -22.5, 0.1);
  s2 = glm::vec3( -15, -25., 0.1);
  s3 = glm::vec3( -20., -22.5, 0.1);
  s4 = glm::vec3( -22.5, -17.5, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(-0.5, 5.5, 0.);
  t4 = glm::vec3( -14.5, -17.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(1., 5., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(3., 3.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = t2 + glm::vec3(5., 2.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);

  // 4eme virage
  // Points de controle de bezier
  s1 = glm::vec3(-17.5, 12.5, 0.1);
  s2 = glm::vec3( -16., 17.5, 0.1);
  s3 = glm::vec3( -14., 17.5, 0.1);
  s4 = glm::vec3(-12.5, 17.5, 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(-5., 4., 0.);
  t4 = glm::vec3(-22.5, 15., 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.5);
  t4 = t3;
  t3 = t2 + glm::vec3(-3., 5.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(-1.5, 5.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = t2 + glm::vec3(0., 5., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);

  // 5eme virage
  // Points de controle de bezier
  s1 = glm::vec3(13., 12.5, 0.1);
  s2 = glm::vec3(16., 12. , 0.1);
  s3 = glm::vec3(16.5, 6.5, 0.1);
  s4 = glm::vec3(17.5, 5., 0.1);
  // Quadriques
  t1 = s1,
  t2 = bezier(s1,s2,s3,s4,0.25);
  t3 = t2 + glm::vec3(3.,5.5, 0.);
  t4 = glm::vec3(15., 17.5, 0.1);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.50);
  t4 = t3;
  t3 = t2 + glm::vec3(3.5, 6., 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2,
  t2 = bezier(s1,s2,s3,s4,0.75);
  t4 = t3;
  t3 = t2 + glm::vec3(5., 4.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
  t1 = t2;
  t2 = s4;
  t4 = t3;
  t3 = t2 + glm::vec3(5., 2.5, 0.);
  portion = std::make_shared<QuadRenderable>( flatShader, t1, t2, t3, t4, color);
  viewer.addRenderable(portion);
}
