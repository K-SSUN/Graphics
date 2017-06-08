// main.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;

// Camera
#include "Camera.h"

// Object
#include "Object.h"

#include "Shader.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <SOIL.h>

void init();
void load_texture();

void display();
void reshape(int, int);
void idle();

void keyboard(unsigned char, int, int);
void special(int, int, int);

int calc(char*);

GLuint      program;

GLint       loc_a_vertex;
GLint       loc_a_normal;
GLint       loc_a_texcoord;

GLint       loc_u_pvm_matrix;
GLint       loc_u_mv_matrix;
GLint       loc_u_normal_matrix;
GLint       loc_u_texid;

GLint       loc_u_material_diffuse;

GLint       loc_u_light_position;
GLint       loc_u_light_diffuse;

glm::mat4   mat_PVM;

float       aspect_ratio = 1.0f;

vector<glm::vec3> vertices;
vector<glm::vec2> texcoords;

// Camera
Camera      g_camera; 

// Object
Object      g_cat, g_T_rex, g_ceratop, g_rham, g_stego;      
GLuint      textureid_cat, textureid_T_rex, textureid_triceratop, textureid_rham, textureid_stego;

string g_filename1, g_filename2, g_filename3, g_filename4, g_filename5;
string g_texturename1, g_texturename2, g_texturename3, g_texturename4, g_texturename5;

float rotation_angle1, rotation_angle2, rotation_angle3, rotation_angle4, rotation_angle5;
float scale_size1, scale_size2, scale_size3, scale_size4, scale_size5;

glm::vec3   cat_loc, T_rex_loc, ceratop_loc, rham_loc, stego_loc;

int main(int argc, char* argv[]){
  // read scene file
  ifstream inFile("scene_file.txt");
  for(int i=0; i<5; i++) {
    if(i==0) 
      inFile >> g_filename1 >> cat_loc.x >> cat_loc.y >> cat_loc.z >> rotation_angle1 >> scale_size1 >> g_texturename1;
    else if(i==1)
      inFile >> g_filename2 >> T_rex_loc.x >> T_rex_loc.y >> T_rex_loc.z >> rotation_angle2 >> scale_size2 >> g_texturename2;
    else if(i==2)
      inFile >> g_filename3 >> ceratop_loc.x >> ceratop_loc.y >> ceratop_loc.z >> rotation_angle3 >> scale_size3 >> g_texturename3;
    else if(i==3)
      inFile >> g_filename4 >> rham_loc.x >> rham_loc.y >> rham_loc.z >> rotation_angle4 >> scale_size4 >> g_texturename4;
    else 
      inFile >> g_filename5 >> stego_loc.x >> stego_loc.y >> stego_loc.z >> rotation_angle5 >> scale_size5 >> g_texturename5;
  }
  inFile.close();

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 800);
  glutCreateWindow("Image Texture Mapping");

  // keybord
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);

  if (glewInit() != GLEW_OK){
    std::cerr << "failed to initialize glew" << std::endl;
    return -1;
  }

  init();

  glutMainLoop();

  return 0;
}

void init(){
  // load object
  g_cat.load_simple_obj(g_filename1);
  g_T_rex.load_simple_obj(g_filename2);
  g_ceratop.load_simple_obj(g_filename3);
  g_rham.load_simple_obj(g_filename4);
  g_stego.load_simple_obj(g_filename5);

  load_texture();

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  program = Shader::create_program("./shader/texture.vert", "./shader/texture.frag");

  loc_a_vertex = glGetAttribLocation(program, "a_vertex");
  loc_a_normal = glGetAttribLocation(program, "a_normal");
  loc_a_texcoord = glGetAttribLocation(program, "a_texcoord");

  loc_u_pvm_matrix = glGetUniformLocation(program, "u_pvm_matrix");
  loc_u_mv_matrix = glGetUniformLocation(program, "mv_matrix");
  loc_u_normal_matrix = glGetUniformLocation(program, "normal_matrix");
  loc_u_texid = glGetUniformLocation(program, "u_texid");

  loc_u_material_diffuse = glGetUniformLocation(program, "material_diffuse");

  loc_u_light_position = glGetUniformLocation(program, "light_position");
  loc_u_light_diffuse = glGetUniformLocation(program, "light_diffuse");
}

void load_texture(){
  int width, height, channels;

  textureid_cat = SOIL_load_OGL_texture(
    "./data/cat_diff.tga",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );
  glBindTexture(GL_TEXTURE_2D, textureid_cat);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  textureid_T_rex = SOIL_load_OGL_texture(
      "./data/Rex_D.tga",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
  );
  glBindTexture(GL_TEXTURE_2D, textureid_T_rex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  textureid_triceratop = SOIL_load_OGL_texture(
      "./data/trike_diffuse.tga",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
  );
  glBindTexture(GL_TEXTURE_2D, textureid_triceratop);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  textureid_rham = SOIL_load_OGL_texture(
      "./data/rham_diff.tga",
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
      SOIL_FLAG_INVERT_Y
  );
  glBindTexture(GL_TEXTURE_2D, textureid_rham);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   textureid_stego = SOIL_load_OGL_texture(
    "./data/Stegosaurus_D.tga",
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
  );
  glBindTexture(GL_TEXTURE_2D, textureid_stego);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (textureid_cat && textureid_T_rex && textureid_triceratop && textureid_rham== 0){
    std::cerr << "Fail to load an image file with SOIL_load_OGL_texture() function." << std::endl;
    return;
  }
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(program);

  glm::mat4   mat_Proj, mat_View, mat_Model, mat_MV;
  glm::mat4   matrix = glm::mat4(1.0);

  glm::mat3 mat_Normal;
  glm::vec3 light_position(10.0f, 10.0f, 10.0f);
  glm::vec4 light_diffuse(1.0f, 1.0f, 1.0f, 1.0f);

  mat_View = glm::lookAt(
    g_camera.position(),        // eye position
    g_camera.center_position(), // center position
    g_camera.up_direction()     // up direction 

    );

  mat_Proj = glm::perspective(glm::radians(45.0f), 1.0f, 0.001f, 10000.0f);
  mat_Model = glm::mat4(1.0);
  mat_PVM = mat_Proj*mat_View*mat_Model;
  mat_MV = mat_Model*mat_View;;

  for(int i = 0; i < 3; i++){
      for(int j = 0; j < 3; j++){
        mat_Normal[i][j] = mat_MV[i][j];
      }
    }

  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  glUniformMatrix4fv(loc_u_mv_matrix, 1, false, glm::value_ptr(mat_MV));
  glUniformMatrix3fv(loc_u_normal_matrix, 1, false, glm::value_ptr(mat_Normal));

  glUniform3fv(loc_u_light_position, 1, glm::value_ptr(light_position));
  glUniform4fv(loc_u_light_diffuse, 1, glm::value_ptr(light_diffuse));

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureid_cat);
  glUniform1i(loc_u_texid, 0);
  matrix = glm::translate(mat_Model, cat_loc);
  matrix = glm::rotate(matrix, rotation_angle1, glm::vec3(0.0f, 1.0f, 0.0f));
  mat_PVM = mat_Proj*mat_View*matrix;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_cat.draw(loc_a_texcoord, loc_a_vertex, loc_a_normal, 0, loc_u_material_diffuse, 0, 0);


  glBindTexture(GL_TEXTURE_2D, textureid_T_rex);
  glUniform1i(loc_u_texid, 0);
  mat_Model = glm::mat4(1.0);
  matrix = glm::translate(mat_Model, T_rex_loc);
  matrix = glm::rotate(matrix, rotation_angle2, glm::vec3(0.0f, 1.0f, 0.0f));
  matrix = glm::scale(matrix, glm::vec3(scale_size2, scale_size2, scale_size2));
  mat_PVM = mat_Proj*mat_View*matrix;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_T_rex.draw(loc_a_texcoord, loc_a_vertex, loc_a_normal, 0, loc_u_material_diffuse, 0, 0);


  glBindTexture(GL_TEXTURE_2D, textureid_triceratop);
  glUniform1i(loc_u_texid, 0);
  mat_Model = glm::mat4(1.0);
  matrix = glm::translate(mat_Model, ceratop_loc);
  matrix = glm::rotate(matrix, rotation_angle3, glm::vec3(0.0f, 1.0f, 0.0f));
  matrix = glm::scale(matrix, glm::vec3(scale_size3, scale_size3, scale_size3));
  mat_PVM = mat_Proj*mat_View*matrix;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_ceratop.draw(loc_a_texcoord, loc_a_vertex, loc_a_normal, 0, loc_u_material_diffuse, 0, 0);

  glBindTexture(GL_TEXTURE_2D, textureid_rham);
  glUniform1i(loc_u_texid, 0);
  mat_Model = glm::mat4(1.0);
  matrix = glm::translate(mat_Model, rham_loc);
  matrix = glm::rotate(matrix, rotation_angle4, glm::vec3(0.0f, 1.0f, 0.0f));
  matrix = glm::scale(matrix, glm::vec3(scale_size4, scale_size4, scale_size4));
  mat_PVM = mat_Proj*mat_View*matrix;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_rham.draw(loc_a_texcoord, loc_a_vertex, loc_a_normal, 0, loc_u_material_diffuse, 0, 0);

  glBindTexture(GL_TEXTURE_2D, textureid_stego);
  glUniform1i(loc_u_texid, 0);
  mat_Model = glm::mat4(1.0);
  matrix = glm::translate(mat_Model, stego_loc);
  matrix = glm::rotate(matrix, rotation_angle5, glm::vec3(0.0f, 1.0f, 0.0f));
  matrix = glm::scale(matrix, glm::vec3(scale_size5, scale_size5, scale_size5));
  mat_PVM = mat_Proj*mat_View*matrix;
  glUniformMatrix4fv(loc_u_pvm_matrix, 1, false, glm::value_ptr(mat_PVM));
  g_stego.draw(loc_a_texcoord, loc_a_vertex, loc_a_normal, 0, loc_u_material_diffuse, 0, 0);

  glUseProgram(0);
  Shader::check_gl_error("draw");
  glutSwapBuffers();
}

void reshape(int width, int height){
  glViewport(0, 0, width, height);
  aspect_ratio = width / (float)height;
}

void idle(){
  glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y){  
  if ('a' == key || 'A' == key){
    g_camera.rotate_left(0.1);
  }else if ('d' == key || 'D' == key){
    g_camera.rotate_right(0.1);
  }
  glutPostRedisplay();
}

void special(int key, int x, int y){ 
  switch (key){
  case GLUT_KEY_UP:
    g_camera.move_forward(1.0);
    break;

  case GLUT_KEY_DOWN:
    g_camera.move_backward(1.0);
    break;

  case GLUT_KEY_LEFT:
    g_camera.move_left(1.0);
    break;

  case GLUT_KEY_RIGHT:
    g_camera.move_right(1.0);
    break;

  default:
    break;
  }
  glutPostRedisplay();
}
