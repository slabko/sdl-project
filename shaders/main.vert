#version 460

layout (location = 0) in vec3 vertexIn;
layout (location = 1) in vec3 colorIn;
layout (location = 2) in vec2 textureIn;
layout (location = 3) in float objectId;

out vec3 colorOut;
out vec2 textureOut;
out vec3 positionOut;

uniform mat4 models[10];
uniform mat4 view;
uniform mat4 proj;

void main() {
    colorOut = colorIn;
    textureOut = textureIn;
    positionOut = vertexIn;

    int objectIndex = int(objectId);
    gl_Position = proj * view * models[objectIndex] * vec4(vertexIn, 1.0);
}
