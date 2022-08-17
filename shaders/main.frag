#version 460

in vec3 colorOut;
in vec2 textureOut;
in vec3 positionOut;

out vec4 fragColor;

uniform sampler2D textureImage;

void main() {
    // float xy = min( abs(positionOut.x), abs(positionOut.y));
    // float xz = min( abs(positionOut.x), abs(positionOut.z));
    // float yz = min( abs(positionOut.y), abs(positionOut.z));
    // float b = 0.495;
    // if ((xy > b) || (xz > b) || (yz > b))
    //     fragColor = vec4(1,0,1,1);
    // else
    //     fragColor = vec4(colorOut, 1.0) * texture(textureImage, textureOut);

    fragColor = vec4(colorOut, 1.0) * texture(textureImage, textureOut);
}
