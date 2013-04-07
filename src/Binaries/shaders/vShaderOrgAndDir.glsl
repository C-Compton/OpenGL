#version 150 
// VertexProgram

uniform mat4 uRotationMatrix;
uniform vec4 uCameraPosition;

in vec2 vRayPosition;

out vec3 org, dir;

void main()
{
    float width = 512;
    float height = 512;
    
    float invWidth = 1 / width;
    float invHeight = 1 / height;
    float fov = 30;
    float aspectratio = width / height;
    float angle = tan(45 * fov / 180);
    
    float x = vRayPosition.x * 256 + 256;
    float y = vRayPosition.y * 256 + 256;
    
    float xx = (2 * ((x + 0.5) * invWidth) - 1) * angle * aspectratio;
    float yy = (1 - 2 * ((y + 0.5) * invHeight)) * angle;
    
    org = uCameraPosition.xyz;
    dir = normalize(uRotationMatrix * vec4(xx, yy, -1.0, 0.0)).xyz;
    
    gl_Position = vec4(xx, yy, -1.0, 1.0);
}
