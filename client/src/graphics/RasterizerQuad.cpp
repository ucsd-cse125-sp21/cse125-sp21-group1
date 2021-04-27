#include "RasterizerQuad.h"
#include <iostream>

/*
 * YOU ONLY NEED TO CHANGE THE FOLLOWING FUNCTIONS!
 *   rasterize()
 *   clearBuffers()
 * YOU SHOULD NOT CHANGE ANYTHING ELSE
 */

using namespace glm;

RasterizerQuad::RasterizerQuad() {
	// Initialize quad data
	vertices = {
		// Upper left
		vec3(-1.f, 1.f, 0),
		// Upper right
		vec3(1.f, 1.f, 0),
		// Lower left
		vec3(-1.f, -1.f, 0),
		// Lower right
		vec3(1.f, -1.f, 0)
	};
	// Triangles are specified in CCW order
	indices = {
		// Upper left triangle
		ivec3(0, 2, 1),
		// Lower left triangle
		ivec3(1, 2, 3)
	};
	// Texture coordinates
	// UV coordinates with the origin at the lower left
	// range for both u and v should be [0,1]
	std::vector<vec2> texCoords = {
		// Upper left
		vec2(0, 1.f),
		// Upper right
		vec2(1.f, 1.f),
		// Lower left
		vec2(0, 0),
		// Lower right
		vec2(1.f, 0)
	};
	this->texCoords = texCoords;

	// Tell OpenGL the vertex data and indices
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Read in the texture coordinates to the GPU through OpenGL
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), 
		texCoords.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// To draw the triangles, OpenGL needs to know the order to draw each vertex
	// Use an element array buffer and pass in the indices to do this
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Set up our textures
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, 0, GL_RGB, GL_FLOAT, 0);

	// Unbind the vbo, we are done making changes to it
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind the vao, we are done making changes to it
	glBindVertexArray(0);
}

RasterizerQuad::RasterizerQuad(int width, int height) : RasterizerQuad() {
	this->width = width;
	this->height = height;
	
	// Initialize our RasterizerQuad with the correct buffer sizes
	pixelBuffer = new float[width * height * 3];
	zBuffer = new float[width * height];
}

RasterizerQuad::~RasterizerQuad() {
	// We want to free the GPU memory
	// We don't want memory leaks after all!
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
	glDeleteTextures(1, &texID);

	delete[] pixelBuffer;
	delete[] zBuffer;
}

void RasterizerQuad::draw() {
	// Bind the VAO
	glBindVertexArray(vao);
	// Draw the quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO
	glBindVertexArray(0);
}

void RasterizerQuad::update() {}


void RasterizerQuad::updateBufSiz(int width, int height) {
	this->width = width;
	this->height = height;

	// Free the space allocated to our old buffers and allocate new space
	delete[] pixelBuffer;
	delete[] zBuffer;
	pixelBuffer = new float[width * height * 3];
	zBuffer = new float[width * height];
}
void RasterizerQuad::clearBuffers() {
	// TODO: Section 5: Clear your buffers for the next render cycle
	// You should set each color in the pixel buffer to whatever clear color you 
	// decide. For here, we set the clear color to be black (0, 0, 0)

	// Clearing your zBuffer does not equal setting all elements to 0
	// Think about how we pick which point is in front of which
    for(int i = 0; i < width * height * 3; i++) pixelBuffer[i]=0;
    for(int i = 0; i < width * height; i++) zBuffer[i]=FLT_MAX;
    
}

void RasterizerQuad::rasterize(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 D, PointCloud& obj) {
	/*
	 * TODO: Section 5: Fill in the pixel buffer by implementing your
	 * rasterizer. Draw out only one model. Your rasterizer should implement
	 * the rasterization pipeline: p' = D*P*V*M*p, then use drawPoint to color
	 * the pixel point p' using normal coloring.
	 */
    
    auto points = obj.getPoints();
    auto vn = obj.getVn();
    unsigned long ttlpts = vn->size();
    fprintf(stderr, "rasterizing \n");
    for(int k = 0; k < ttlpts; k++) {
        
        auto pts = (*points)[k];
        auto pp = D * P * V * M * glm::vec4(pts[0], pts[1] ,pts[2], 1);
        glm::vec3 pp3 = glm::vec3(pp[0] / pp[3], pp[1] / pp[3], pp[2] / pp[3]);
        int im = (int)(obj.getPointSize());
        
        for(int i=0; i< im; i++)
            for(int j=0; j< im; j++)
                drawPoint((int)pp3[0]+i, (int)pp3[1]+j, pp3[2], (*vn)[k][0], (*vn)[k][1], (*vn)[k][2]);
        
    }
        
	// Display the results of your rasterization
	displayResult();
}

void RasterizerQuad::drawPoint(int x, int y, float z, float r, float g, float b) {
	/*
	 * Color a pixel at coordinates x, y using the r, g, b values. We only color
	 * that pixel if the point passed in is closer to the camera than the previous
	 * point, and we can figure that out using z and the z-buffer.
	 */
	// Check if a point lies within our screen
	if (x >= width || x < 0 || y >= height || y < 0)
		return;

	int offset = y * width * 3 + x * 3;
	// zBuffer algorithm
	if (z < zBuffer[x + y * width]) {
		zBuffer[x + width * y] = z;
		pixelBuffer[offset] = r;
		pixelBuffer[offset + 1] = g;
		pixelBuffer[offset + 2] = b;
	}
}

void RasterizerQuad::displayResult() {
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &(pixelBuffer[0]));
	// Use the program for texturing the quad
	glBindVertexArray(vao);
    glClearColor(0, 0, 0, 1);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw the quad
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
