#version 300 es

// vertex300es.glsl
// ADS (Ambient Diffuse Specular) per-vertex/per-fragment shading
// (c)2016-2017 by Alain Menu (Qam3D/GLam project)

// input data (from user program)

in vec3	VertexPosition ;
in vec3	VertexNormal ;
in vec2	TexelCoord ;

uniform mat4 	ModelViewMatrix ;
uniform mat3 	NormalMatrix ;
uniform mat4 	MvpMatrix ;

uniform vec3	LightPosition ;

// output data (to fragment shader)

out	vec3	vPosition ;			// eye vector
out	vec3	vNormal ;			// normal vector
out vec2	vTexel ;			// texel coordinates
out vec3	vLight ;			// light vector

// ----------------------------------------------------------------------------

void main()
{
  vPosition = vec3( ModelViewMatrix * vec4( VertexPosition, 1.0 ) ) ;
  vNormal = normalize( NormalMatrix * VertexNormal ) ;
  vTexel = TexelCoord ;
  vLight = LightPosition - vPosition ;

  gl_Position = MvpMatrix * vec4( VertexPosition, 1.0 ) ;
}
