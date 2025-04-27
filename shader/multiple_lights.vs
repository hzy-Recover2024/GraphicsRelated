#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec4 color; //color.w is mat index

layout(colum_major) buffer;

out vec3 FragPos;
out vec3 Normal;
out vec3 color;

layout(binding = 0, std140) uniform cameraInfo
{
    mat4 view;
    mat4 projection;
    mat4 vp;
}

layout(binding = 0, std430) readonly buffer matInstanced
{
    mat4 matlist[];
};

void main()
{
    FragPos = vec3(view * matlist[int(color.w)] * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(matlist[int(color.w)]))) * aNormal;  
    
    gl_Position = vp * vec4(FragPos, 1.0);
}