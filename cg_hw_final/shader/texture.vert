#version 120

uniform mat4 u_pvm_matrix;
uniform mat4 mv_matrix;
uniform mat3 normal_matrix;

attribute vec4 a_vertex;
attribute vec3 a_normal;
attribute vec2 a_texcoord;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec2 v_texcoord;

void main() {
 	v_vertex = (mv_matrix * a_vertex).xyz;
 	v_normal = normalize(normal_matrix * a_normal);
  	v_texcoord = a_texcoord; 

  	gl_Position = u_pvm_matrix * a_vertex;
}
