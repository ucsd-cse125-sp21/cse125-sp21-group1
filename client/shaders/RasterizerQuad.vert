#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// PLEASE DO NOT MODIFY THIS FILE!
// THIS IS FOR DISPLAYING YOUR RASTERIZATION RESULTS!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 inTexCoords;

// Uniform variables can be updated by fetching their location and passing values to that location

// Outputs of the vertex shader are the inputs of the same name of the fragment shader.
// The default output, gl_Position, should be assigned something. You can define as many
// extra outputs as you need.
out float sampleExtraOutput;
out vec2 texCoord;

void main()
{
    // We are rendering the vertices in screen space so the quad always covers the viewport
    gl_Position = vec4(position, 1.0);
    sampleExtraOutput = 1.0f;
    texCoord = inTexCoords;
}
