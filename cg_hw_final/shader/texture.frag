#version 120

uniform vec3 light_position;
uniform vec4 light_diffuse;

uniform vec4 material_diffuse;

uniform sampler2D u_texid;

varying vec3 v_vertex;
varying vec3 v_normal;
varying vec2 v_texcoord;

vec4 directional_diffuse() {
	vec4 color = vec4(0, 0, 0, 0);
	vec3 normal = normalize(v_normal);
	vec3 light_vector = normalize(light_position);

	float ndotl = max(0.0, dot(normal, light_vector));
	color += (ndotl * light_diffuse * material_diffuse);

	return color;
}

void main() {
  gl_FragColor = texture2D(u_texid, v_texcoord) + directional_diffuse();  
}
