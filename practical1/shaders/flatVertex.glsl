#version 400 //GLSL version, fit with OpenGL version
uniform mat4 projMat, viewMat, modelMat; 
in vec3 vPosition; 
in vec4 colorIn;
out vec4 color;

void main() {
    //Transform coordinates from local space to clipped space
    gl_Position = projMat*viewMat*modelMat*vec4(vPosition,1);
    color = abs(colorIn); //RGBA color defined in [0,1]
}
