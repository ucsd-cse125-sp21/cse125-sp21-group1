#version 330 core
// This is a sample fragment shader.

// PLEASE DO NOT MODIFY THIS FILE!
// THIS IS FOR DISPLAYING YOUR RASTERIZATION RESULTS!

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec2 texCoord;

uniform vec3 color;
uniform sampler2D imageTexture;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
    fragColor = vec4(color, sampleExtraOutput);
    fragColor = texture(imageTexture, texCoord);
    // Below is a sanity check to make sure the tex coords are being passed
    //fragColor = vec4(texCoord, 0.f, 1.f);
}
