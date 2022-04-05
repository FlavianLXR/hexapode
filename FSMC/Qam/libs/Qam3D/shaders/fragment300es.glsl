#version 300 es

// fragment300es.glsl
// ADS (Ambient Diffuse Specular) per-vertex/per-fragment shading
// (c)2016-2017 by Alain Menu (Qam3D/GLam project)

#ifdef GL_ES
precision highp float ;				// requested with iOS...
#endif

// type definitions and constants

struct LightInfo {
	vec3 La ; 						// Ambient light intensity
	vec3 Ld ; 						// Diffuse light intensity
	vec3 Ls ; 						// Specular light intensity
} ;

struct MaterialInfo {
	vec3 Ka ; 						// Ambient reflectivity
	vec3 Kd ; 						// Diffuse reflectivity
	vec3 Ks ; 						// Specular reflectivity
	float Shininess ; 				// Specular shininess factor
	float Transparency ;			// Dissolve factor (1.0 = fully opaque)
} ;

const int FLATSHADING    = 0 ;
const int GOURAUDSHADING = 1 ;
const int PHONGSHADING   = 2 ;
const int TOONSHADING    = 3 ;

// input data from user program

uniform vec3 		 LightPosition ;
uniform LightInfo    Light ;
uniform MaterialInfo Material ;
uniform int 		 shadingEffect ;	//= FLATSHADING ;
uniform bool 		 hasTexture ;		//= false ;
uniform sampler2D 	 Texture ;

// input data from vertex shader
// (hardware interpolates these vectors)

in	vec3	vPosition ;			// pixel position (eye vector)
in	vec3	vNormal ;			// pixel normal
in  vec2	vTexel ;			// texel coordinates
in  vec3	vLight ;			// light vector

out vec4	fColor ;	// GLSL 3.X: own output instead of gl_FragColor

// ----------------------------------------------------------------------------

float toonShading( float ln )
{
	if ( ln > 0.75 )		ln = 0.75 ;
	else if ( ln > 0.50 )	ln = 0.50 ;
	else if ( ln > 0.25 )	ln = 0.25 ;
	else ln = 0.0 ;

	return ln ;
}

// ----------------------------------------------------------------------------

void main()
{
	bool perPixelShading = ( shadingEffect >= PHONGSHADING ) ;

	vec3 N = ( gl_FrontFacing ? vNormal : -vNormal ) ;
	if ( perPixelShading )	N = normalize( N ) ;
	vec3 E = normalize( -vPosition ) ;
	vec3 L = normalize( perPixelShading ? vLight : LightPosition - vPosition ) ;
	vec3 R = reflect( -L, N ) ;
	vec3 H = normalize( L + E ) ;

	// ambient component
	vec3 ambient = Light.La * Material.Ka ;

	// diffuse component
	float ln = max(dot(L,N), 0.0 ) ;
	if ( shadingEffect == TOONSHADING )	ln = toonShading( ln ) ;
	vec3 diffuse = ln * Light.Ld * Material.Kd ;

	// specular component
	vec3 specular = vec3( 0.0 ) ;
	if ( dot(L,N) > 0.0 ) {	// only if the light can see the point...
		float nh = max( ( perPixelShading ? dot(N,H) : dot(R,E) ), 0.0 ) ;
		nh = pow(nh, Material.Shininess ) ;
		specular = nh * Light.Ls * Material.Ks ;
	}

	// combine ADS contributions, transparency,...
	vec4 ads = vec4( ambient + diffuse + specular, Material.Transparency ) ;
	// ... and texture
//	if ( hasTexture )	ads = ads * texture2D( Texture, vTexel.st ) ;
	if ( hasTexture )	ads = ads * texture( Texture, vTexel.st ) ;

//	gl_FragColor = ads ;
	fColor = ads ;
}