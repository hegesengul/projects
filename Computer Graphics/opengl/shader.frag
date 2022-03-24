#version 330

// Output Color
out vec4 color;

uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec3 cameraPosition;

// Texture-related data;
uniform sampler2D rgbTexture;
uniform int widthTexture;
uniform int heightTexture;
uniform float heightFactor;

// Data from Vertex Shader
in vec2 textureCoordinate;
in vec3 vertexNormal; // For Lighting computation
in vec3 ToLightVector; // Vector from Vertex to Light;
in vec3 ToCameraVector; // Vector from Vertex to Camera;

void main() {

	// Assignment Constants below
	// get the texture color
	vec4 textureColor = texture(rgbTexture, textureCoordinate);

	// apply Phong shading by using the following parameters
	vec4 ka = vec4(0.25,0.25,0.25,1.0); // reflectance coeff. for ambient
	vec4 Ia = vec4(0.3,0.3,0.3,1.0); // light color for ambient
	vec4 Id = vec4(1.0, 1.0, 1.0, 1.0); // light color for diffuse
	vec4 kd = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for diffuse
	vec4 Is = vec4(1.0, 1.0, 1.0, 1.0); // light color for specular
	vec4 ks = vec4(1.0, 1.0, 1.0, 1.0); // reflectance coeff. for specular
	int specExp = 100; // specular exponent
	color = vec4(1,0,0,1);
	

	float costheta = max(0 , dot(ToLightVector , vertexNormal));


	//float costheta = max(0 , dot(ToLightVector , vec3(0,1,0)));

	// compute ambient component
	vec4 ambient = vec4(0.075, 0.075, 0.075, 1);

	// compute diffuse component
	//vec4 diffuse = kd * Id * costheta;
	vec4 diffuse = kd * Id * costheta ;

	// compute specular component

	vec3 h = normalize(ToCameraVector + ToLightVector);

	float cosalpha = max(0 , dot(h , vertexNormal));
	//float cosalpha = max(0 , dot(h , vertexNormal));

	vec4 specular = ks * Is * pow(cosalpha , specExp);

	// compute the color using the following equation
	//color = vec4(clamp( textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);

	
	color = vec4(clamp(textureColor.xyz * vec3(ambient + diffuse + specular), 0.0, 1.0), 1.0);
	//color = vec4(0,1,0,1);
	

	
    //if(dot(ToLightVector , vertexNormal) >= 0.4) color = vec4(0,1,0,1); 
	//if(dot(h , vertexNormal) >= 0.7) color = vec4(1,0,0,1); 
	
}