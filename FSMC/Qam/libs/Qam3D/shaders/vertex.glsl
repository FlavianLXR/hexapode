// vertex.glsl
// ----------------------------------------------------------------------------
// ADS (Ambient Diffuse Specular) per-vertex/per-fragment shading
// (c)2016-2018 by Alain Menu (Qam3D/GLam project)

#if ( __VERSION__ > 120 )
	#define IN	in
	#define	OUT	out
#else
	#define	IN	attribute
	#define	OUT	varying
#endif

// input data (from user program)

IN vec3	VertexPosition ;
IN vec3	VertexNormal ;
IN vec2	TexelCoord ;

uniform mat4 	ModelViewMatrix ;
uniform mat3 	NormalMatrix ;
uniform mat4 	MvpMatrix ;

uniform vec3	LightPosition ;

// output data (to fragment shader)

OUT	vec3	vPosition ;			// eye vector
OUT	vec3	vNormal ;			// normal vector
OUT vec2	vTexel ;			// texel coordinates
OUT vec3	vLight ;			// light vector

// ----------------------------------------------------------------------------

void main()
{
  vPosition = vec3( ModelViewMatrix * vec4( VertexPosition, 1.0 ) ) ;
  vNormal = normalize( NormalMatrix * VertexNormal ) ;
  vTexel = TexelCoord ;
  vLight = LightPosition - vPosition ;

  gl_Position = MvpMatrix * vec4( VertexPosition, 1.0 ) ;
}
