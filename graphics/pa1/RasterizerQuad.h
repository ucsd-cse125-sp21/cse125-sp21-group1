#pragma once
#include "Object.h"
#include "PointCloud.h"

// PLEASE DO NOT CHANGE ANYTHING HERE!
// THIS CODE IS NECESSARY FOR YOU TO SEE YOUR RASTERIZATION RESULTS

class RasterizerQuad : public Object
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::ivec3> indices;

	// Pixel buffer array
	float* pixelBuffer;
	// Z-buffer array
	float* zBuffer;
	// Size of our window
	int width, height;

	// VAO and VBO IDs for rendering the quad
	GLuint vao, vbos[3];
	// For texturing the quad
	GLuint texID;
public:
	RasterizerQuad();
	RasterizerQuad(int width, int height);
	~RasterizerQuad();
	
	void draw();
	void update();

	void updateBufSiz(int width, int height);
	/*
	 * TODO: Section 5: The functions below are crucial for running your
	 * rasterizer. Implement the functions below
	 *   clearBuffers()
	 *   rasterize()
	 */

	/*
	 * Make sure to clear the color buffer and the z buffer
	 * Zero out all the elements of your color buffer
	 * How would you reinitialize the z-buffer? Hint: don't zero out
	 */
	void clearBuffers();
	/*
	 * Draws a point at location x,y on the screen using the given rgb values
	 *   int x: x-coordinate in screen space
	 *   int y: y-coordinate in screeen space
	 *   float z: depth of the point in screen space
	 *   float r,g,b: Red, green, blue color values (between 0 and 1)
	 */
	void drawPoint(int x, int y, float z, float r, float g, float b);
	/*
	 * Rasterize your bunny/dragon point cloud here
	 * Implement the rasterization pipeline: p' = D*P*V*M*p
	 * Then use drawPoint to write the color values into the color buffer
	 * Remember to use normal coloring in range [0,1]
	 *   mat4 M: model matrix
	 *   mat4 V: view matrix
	 *   mat4 P: projection matrix
	 *   mat4 D: viewport transformation matrix
	 *   PointCloud& obj: Object to be rasterized
	 */
	void rasterize(glm::mat4 M, glm::mat4 V, glm::mat4 P, glm::mat4 D, PointCloud& obj);

	// This function displays the results of your rasterizer and is called within rasterize()
	// after you filled in your pixel buffer
	void displayResult();
};

