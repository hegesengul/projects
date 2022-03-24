#version 330

in vec3 pos;

// Data from CPU 
uniform mat4 MVP; // ModelViewProjection Matrix
uniform mat4 MV; // ModelView idMVPMatrix
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform float heightFactor;//yüklendi

uniform float test;
// Texture-related data
uniform sampler2D rgbTexture;//yüklendi
uniform sampler2D heiTexture;//yüklendi

uniform int widthTexture;//yüklendi
uniform int heightTexture;//yüklendi
uniform int text_delay;

// Output to Fragment Shader
out vec2 textureCoordinate; // For texture-color
out vec3 vertexNormal; // For Lighting computation
out vec3 ToLightVector; // Vector from Vertex to Light;
out vec3 ToCameraVector; // Vector from Vertex to Camera;

vec3 find_normal(vec3 p1 , vec3 p2 , vec3 p3){
    vec3 normal;
    vec3 vector1;
    vec3 vector2;

    vector1 = p2 - p1;
    vector2 = p3 - p1;

    return normalize(cross(vector2 , vector1));
}

void main()
{
    vec3 position = pos;

  //  if (position.x == 0) position.x = 1;
  //  if (position.x == widthTexture - 1) position.x = widthTexture - 2;

    if (position.z == 0) position.z = 1;
    if (position.z == widthTexture - 1) position.z = heightTexture - 2;

/*  
    textureCoordinate = vec2(float(int(widthTexture -position.x + text_delay) % widthTexture) / widthTexture , (heightTexture - position.z) / heightTexture);

    float height = texture2D(heiTexture , textureCoordinate).x * heightFactor;
   
    vec3 center = vec3(position.x , height , position.z);
    
    vec3 v1 ,v2 , v3 , v4 , v5 , v6;

    vec2 text_coord1 = vec2(float(int(widthTexture - position.x - 1 + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z - 1)) / heightTexture);
    vec2 text_coord2 = vec2(float(int(widthTexture - position.x - 1 + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z    )) / heightTexture);
    vec2 text_coord3 = vec2(float(int(widthTexture - position.x     + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z + 1)) / heightTexture);
    vec2 text_coord4 = vec2(float(int(widthTexture - position.x + 1 + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z + 1)) / heightTexture);
    vec2 text_coord5 = vec2(float(int(widthTexture - position.x + 1 + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z    )) / heightTexture);
    vec2 text_coord6 = vec2(float(int(widthTexture - position.x     + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z - 1)) / heightTexture);

    v1 = vec3(float(int(position.x - 1) % widthTexture), texture2D(heiTexture , text_coord1).x * heightFactor , position.z - 1); 
    v2 = vec3(float(int(position.x - 1) % widthTexture), texture2D(heiTexture , text_coord2).x * heightFactor , position.z    );
    v3 = vec3(float(int(position.x    ) % widthTexture), texture2D(heiTexture , text_coord3).x * heightFactor , position.z + 1); 
    v4 = vec3(float(int(position.x + 1) % widthTexture), texture2D(heiTexture , text_coord4).x * heightFactor , position.z + 1);  
    v5 = vec3(float(int(position.x + 1) % widthTexture), texture2D(heiTexture , text_coord5).x * heightFactor , position.z    ); 
    v6 = vec3(float(int(position.x    ) % widthTexture), texture2D(heiTexture , text_coord6).x * heightFactor , position.z - 1);  */



      
    textureCoordinate =  vec2(float((widthTexture - int(position.x ) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z )) / heightTexture);

    float height = texture2D(heiTexture , textureCoordinate).x * heightFactor;
   
    vec3 center = vec3(position.x , height , position.z);
    
    vec3 v1 ,v2 , v3 , v4 , v5 , v6;

    vec2 text_coord1 = vec2(float((widthTexture - int(position.x - 1) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z - 1)) / heightTexture);
    vec2 text_coord2 = vec2(float((widthTexture - int(position.x - 1) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z    )) / heightTexture);
    vec2 text_coord3 = vec2(float((widthTexture - int(position.x    ) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z + 1)) / heightTexture);
    vec2 text_coord4 = vec2(float((widthTexture - int(position.x + 1) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z + 1)) / heightTexture);
    vec2 text_coord5 = vec2(float((widthTexture - int(position.x + 1) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z    )) / heightTexture);
    vec2 text_coord6 = vec2(float((widthTexture - int(position.x    ) % widthTexture + text_delay) % widthTexture) / widthTexture , (heightTexture - (position.z - 1)) / heightTexture);

    v1 = vec3(float(int(position.x - 1) % widthTexture), texture2D(heiTexture , text_coord1).x * heightFactor , position.z - 1); 
    v2 = vec3(float(int(position.x - 1) % widthTexture), texture2D(heiTexture , text_coord2).x * heightFactor , position.z    );
    v3 = vec3(float(int(position.x    ) % widthTexture), texture2D(heiTexture , text_coord3).x * heightFactor , position.z + 1); 
    v4 = vec3(float(int(position.x + 1) % widthTexture), texture2D(heiTexture , text_coord4).x * heightFactor , position.z + 1);  
    v5 = vec3(float(int(position.x + 1) % widthTexture), texture2D(heiTexture , text_coord5).x * heightFactor , position.z    ); 
    v6 = vec3(float(int(position.x    ) % widthTexture), texture2D(heiTexture , text_coord6).x * heightFactor , position.z - 1); 
  




    vertexNormal =
        normalize(
            ( vec4(find_normal(center,v1,v6) , 0)).xyz  
            + ( vec4(find_normal(center,v6,v5) , 0)).xyz 
            + ( vec4(find_normal(center,v5,v4) , 0)).xyz
            + ( vec4(find_normal(center,v4,v3) , 0)).xyz
            + ( vec4(find_normal(center,v3,v2) , 0)).xyz
            + ( vec4(find_normal(center,v2,v1) , 0)).xyz 
    );   

    
   /*vertexNormal =
        normalize(
            (transpose(inverse(MV)) * vec4(find_normal(center,v1,v6) , 0)).xyz + 
            (transpose(inverse(MV)) * vec4(find_normal(center,v6,v5) , 0)).xyz +
            (transpose(inverse(MV)) * vec4(find_normal(center,v5,v4) , 0)).xyz + 
            (transpose(inverse(MV)) * vec4(find_normal(center,v4,v3) , 0)).xyz + 
            (transpose(inverse(MV)) * vec4(find_normal(center,v3,v2) , 0)).xyz +
           (transpose(inverse(MV)) * vec4(find_normal(center,v2,v1) , 0)).xyz ) ;  */

          //  vertexNormal = (transpose(inverse(MV))*vec4(0,1,0,0)).xyz;

         //vertexNormal = (MV* vec4( 0 , -1 , 0 , 0)).xyz;
            
    //vec4 inter  =  (MVP * vec4(center ,1));
    //vec4 inter2 = inter/inter.w ;


    //ToLightVector = normalize(lightPosition - inter2.xyz);
    //ToCameraVector = normalize(cameraPosition - inter2.xyz);

    ToLightVector  = normalize(lightPosition  - center);
    ToCameraVector = normalize(cameraPosition - center);

    //vec3 laka = vec3(widthTexture/2,100,heightTexture/2); 
    //ToLightVector  = normalize((MV * vec4(lightPosition,1)).xyz  - (MV * vec4(center,1)).xyz);
    //ToCameraVector = normalize( - (MV*vec4(center,1)).xyz);

    gl_Position = MVP * vec4(position.x , height  , position.z , 1);
    
    //if(gl_Position.w <= 10 && gl_Position.w >= -10) gl_Position = vec4(0 , 0 , 0 , 1);
    //else gl_Position = gl_Position / gl_Position.w;

   // gl_Position = vec4(gl_Position.x / gl_Position.w , gl_Position.y / gl_Position.w , gl_Position.z / gl_Position.w , 1);
}

