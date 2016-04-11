#include "./../include/gl_helper.hpp"
#include "./../include/log.hpp"
#include "./../include/Utils.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <GL/glew.h>

void draw_bezier_2d_mesh (std::vector< glm::vec3 > positions){/*,
   std::vector< glm::vec4 > colors) {*/

  GLfloat ctrlpoints[positions.size()/2][positions.size()/2][3] ;
  //GLfloat couleurs[colors.size()/2][colors.size()/2][4] ;
  int l=0;
  int k=0;
  for (int i=0;i<positions.size();i++) {
    ctrlpoints[k][l][0]=positions[i][0];
    ctrlpoints[k][l][1]=positions[i][1];
    ctrlpoints[k][l][2]=positions[i][2];
    //couleurs[k][l][0]=colors[i][0];
    //couleurs[k][l][1]=colors[i][1];
    //couleurs[k][l][2]=colors[i][2];
    //couleurs[k][l][3]=colors[i][3];
    l++;
    if (l>=positions.size()/2){
      k++;l=0;
    }
  }

  glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
        0, 1, 12, 4, &ctrlpoints[0][0][0]);
  //glMap2f(GL_MAP2_COLOR_4, 0, 1, 4, 4,
    //    0, 1, 112, 4, &couleurs[0][0][0]);
  glEnable(GL_MAP2_VERTEX_3);
  //glEnable(GL_MAP2_COLOR_4);
  glEnable(GL_DEPTH_TEST);
  glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix ();
  glRotatef(85.0, 1.0, 1.0, 1.0);
  glEvalMesh2(GL_FILL, 0, 20, 0, 20);
  glDisable(GL_MAP2_VERTEX_3);
  //glDisable(GL_MAP2_COLOR_4);
  glPopMatrix();
  glFlush();
}

void points(glm::vec3 pt1, glm::vec3 pt2, std::vector< glm::vec3 > &positions) {
  glm::vec3 v = pt2-pt1;
  glm::vec3 p1 = pt1 + glm::vec3(v[0]/3,v[1]/3,v[2]/3);
  glm::vec3 p2 = pt1 + glm::vec3(v[0]*2/3,v[1]*2/3,v[2]*2/3);
  positions.push_back(pt1);
  positions.push_back(p1);
  positions.push_back(p2);
  positions.push_back(pt2);
}

glm::vec3 bezier(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3,
        glm::vec3 p4, float t) {
  //B(t)=p1*(1-t)^3 + 3*p2*t*(1-t)^2 + 3*p3*t^2(1-t) + p4*t^3  t c [0,1]

  float coef1 = pow(1-t,3);
  glm::vec3 terme1 = glm::vec3(p1[0]*coef1, p1[1]*coef1, p1[2]*coef1);

  float coef2 = 3*t*pow(1-t,2);
  glm::vec3 terme2 = glm::vec3(p2[0]*coef2, p2[1]*coef2, p2[2]*coef2);

  float coef3 = 3*pow(t,2)*(1-t);
  glm::vec3 terme3 = glm::vec3(p3[0]*coef3, p3[1]*coef3, p3[2]*coef3);

  float coef4 = pow(t,3);
  glm::vec3 terme4 = glm::vec3(p4[0]*coef4, p4[1]*coef4, p4[2]*coef4);

  return (terme1 + terme2 + terme3 + terme4);
}
