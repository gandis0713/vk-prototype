#version 450

layout(location = 0) in vec2 inTexCoord;
layout(location = 0) out vec4 outColor;

// layout(binding = 0) uniform sampler2D texSampler;

void main()
{
    // outColor = texture(texSampler, inTexCoord);
    outColor = vec4(1.0, 0.0, 0.0, 1.0);
}