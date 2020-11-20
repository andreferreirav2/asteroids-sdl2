#version 330 core

attribute vec3 vertexPos;
attribute vec3 vertexNormal;
uniform vec3 materialDiffuse;
uniform float materialEmissiveness;
uniform mat4 mvp; // transform matrix
uniform mat3 m3x3InvTransp;
varying vec4 color;

struct lightSource
{
	vec4 position;
	vec4 diffuse;
};

lightSource light = lightSource(
	vec4(1.0, 1.0, 1.0, 0.0), // position
	vec4(1.0, 1.0, 1.0, 1.0)  // diffuse
);

void main()
{
	vec3 n = normalize(m3x3InvTransp * vertexNormal);
	vec3 l = normalize(vec3(light.position));
	vec3 diffuseReflection = vec3(light.diffuse)
		* materialDiffuse
		* max(materialEmissiveness, dot(n, l));

	color = vec4(diffuseReflection, 1.0);
	gl_Position = mvp * vec4(vertexPos, 1);
}