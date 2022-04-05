#version 120

// vertex120.glsl
// ADS (Ambient Diffuse Specular) per-vertex/per-fragment shading
// (c)2016-2017 by Alain Menu (Qam3D/GLam project)

// input data (from user program)

attribute vec3	VertexPosition ;
attribute vec3	VertexNormal ;
attribute vec2	TexelCoord ;

uniform mat4 	ModelViewMatrix ;
uniform mat3 	NormalMatrix ;
uniform mat4 	MvpMatrix ;

uniform vec3	LightPosition ;

// output data (to fragment shader)

varying	vec3	vPosition ;			// eye vector
varying	vec3	vNormal ;			// normal vector
varying vec2	vTexel ;			// texel coordinates
varying vec3	vLight ;			// light vector

// ----------------------------------------------------------------------------

void main()
{
  vPosition = vec3( ModelViewMatrix * vec4( VertexPosition, 1.0 ) ) ;
  vNormal = normalize( NormalMatrix * VertexNormal ) ;
  vTexel = TexelCoord ;
  vLight = LightPosition - vPosition ;

  gl_Position = MvpMatrix * vec4( VertexPosition, 1.0 ) ;
}
