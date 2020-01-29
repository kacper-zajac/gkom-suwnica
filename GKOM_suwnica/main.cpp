#define GLEW_STATIC
#include <GL/glew.h>
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Skybox.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>



#define numberOfTriangles 360
float moveFloat = 0.f;
float moveFloat2 = 0.f;






void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

GLfloat allCircleVertices[(numberOfTriangles) * 6 * 3 * 4];
Camera camera(glm::vec3(35.0f, 0.0f, -35.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void drawCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLint numberOfSides)
{
	GLfloat twicePi = 2.0f * M_PI;

	GLfloat circleVerticesX[3 * numberOfTriangles];
	GLfloat circleVerticesY[3 * numberOfTriangles];
	GLfloat circleVerticesZ[3 * numberOfTriangles];
	GLfloat circleNormalX = 0.f;
	GLfloat circleNormalY = 0.f;
	GLfloat circleNormalZ = 2 * z;


	for (int i = 0; i < numberOfTriangles; i++)
	{
		circleVerticesX[3 * i + 0] = x + (radius * cos(i * twicePi / (numberOfSides - 1)));
		circleVerticesY[3 * i + 0] = y + (radius * sin(i * twicePi / (numberOfSides - 2)));
		circleVerticesZ[3 * i + 0] = z;
		circleVerticesX[3 * i + 1] = x + (radius * cos((i + 1) * twicePi / (numberOfSides - 3)));
		circleVerticesY[3 * i + 1] = y + (radius * sin((i + 1) * twicePi / (numberOfSides - 4)));
		circleVerticesZ[3 * i + 1] = z;
		circleVerticesX[3 * i + 2] = x;
		circleVerticesY[3 * i + 2] = y;
		circleVerticesZ[3 * i + 2] = z;
	}

	// przednia sciana
	for (int i = 0; i < 3 * numberOfTriangles; i++)
	{
		allCircleVertices[i * 6] = circleVerticesX[i];
		allCircleVertices[(i * 6) + 1] = circleVerticesY[i];
		allCircleVertices[(i * 6) + 2] = circleVerticesZ[i];
		allCircleVertices[(i * 6) + 3] = circleNormalX;
		allCircleVertices[(i * 6) + 4] = circleNormalY;
		allCircleVertices[(i * 6) + 5] = circleNormalZ;

	} // 6 * 3 * num

	// tylna sciana
	for (int i = 0 ; i < 3 * numberOfTriangles; i++)
	{
		allCircleVertices[(3 * 6 * numberOfTriangles) + i * 6] = circleVerticesX[i];
		allCircleVertices[(3 * 6 * numberOfTriangles) + (i * 6) + 1] = circleVerticesY[i];
		allCircleVertices[(3 * 6 * numberOfTriangles) + (i * 6) + 2] = -circleVerticesZ[i];
		allCircleVertices[(3 * 6 * numberOfTriangles) + (i * 6) + 3] = circleNormalX;
		allCircleVertices[(3 * 6 * numberOfTriangles) + (i * 6) + 4] = circleNormalY;
		allCircleVertices[(3 * 6 * numberOfTriangles) + (i * 6) + 5] = -circleNormalZ;

	} // 3 * 6 * num + 3 * 6 * num

	// kopiujemy przednia i tylna sciane
	for (int i = 0; i < 6 * numberOfTriangles; i++)
	{
		allCircleVertices[(i * 6) + 6 * 6 * numberOfTriangles] = allCircleVertices[(i * 6)];
		allCircleVertices[1 + (i * 6) + 6 * 6 * numberOfTriangles] = allCircleVertices[(i * 6) + 1];
		allCircleVertices[2 + (i * 6) + 6 * 6 * numberOfTriangles] = allCircleVertices[(i * 6) + 2];
		allCircleVertices[3 + (i * 6) + 6 * 6 * numberOfTriangles] = 0.f;
		allCircleVertices[4 + (i * 6) + 6 * 6 * numberOfTriangles] = 0.f;
		allCircleVertices[5 + (i * 6) + 6 * 6 * numberOfTriangles] = 0.f;
	}// git


	for (int i = 0; i < numberOfTriangles; i ++)
	{
		allCircleVertices[(i * 6 * 3) + 12 + numberOfTriangles * 6 * 6] = allCircleVertices[(i * 6 * 3) + numberOfTriangles * 6 * 6];
		allCircleVertices[1 + (i * 6 * 3) + 12 + numberOfTriangles * 6 * 6] = allCircleVertices[1 + (i * 6 * 3) + numberOfTriangles * 6 * 6];
		allCircleVertices[2 + (i * 6 * 3) + 12 + numberOfTriangles * 6 * 6] = - allCircleVertices[2 + (i * 6 * 3) + numberOfTriangles * 6 * 6];
	}
	for (int i = 0; i < numberOfTriangles; i++)
	{
		allCircleVertices[(i * 6 * 3) + 12 + numberOfTriangles * 6 * 3 * 3] = allCircleVertices[(i * 6 * 3) + 6 + numberOfTriangles * 6 * 3 * 3];
		allCircleVertices[1 + (i * 6 * 3) + 12 + numberOfTriangles * 6 * 3 * 3] = allCircleVertices[1 + (i * 6 * 3) + 6 + numberOfTriangles * 6 * 3 * 3];
		allCircleVertices[2 + (i * 6 * 3) + 12 + numberOfTriangles * 6 * 3 * 3] = -allCircleVertices[2 + (i * 6 * 3) + 6 + numberOfTriangles * 6 * 3 * 3];
	}

}
int main()
{	
	glm::vec3 movement[] = {
		  glm::vec3(0.0f, -5.0f, -0.0f),
		  glm::vec3(1.0f, -5.0f, -0.0f),
		  glm::vec3(2.0f, -5.0f, -0.0f),
		  glm::vec3(3.0f, -5.0f, -0.0f),
		  glm::vec3(4.0f, -5.0f, -0.0f),
		  glm::vec3(5.0f, -5.0f, -0.0f),
		  glm::vec3(6.0f, -5.0f, -0.0f),
		  glm::vec3(7.0f, -5.0f, -0.0f),
		  glm::vec3(8.0f, -5.0f, -0.0f),
		  glm::vec3(9.0f, -5.0f, -0.0f),
		  glm::vec3(0.0f, -5.0f, -1.0f),
		  glm::vec3(1.0f, -5.0f, -1.0f),
		  glm::vec3(2.0f, -5.0f, -1.0f),
		  glm::vec3(3.0f, -5.0f, -1.0f),
		  glm::vec3(4.0f, -5.0f, -1.0f),
		  glm::vec3(5.0f, -5.0f, -1.0f),
		  glm::vec3(6.0f, -5.0f, -1.0f),
		  glm::vec3(7.0f, -5.0f, -1.0f),
		  glm::vec3(8.0f, -5.0f, -1.0f),
		  glm::vec3(9.0f, -5.0f, -1.0f),
		  glm::vec3(0.0f, -5.0f, -2.0f),
		  glm::vec3(1.0f, -5.0f, -2.0f),
		  glm::vec3(2.0f, -5.0f, -2.0f),
		  glm::vec3(3.0f, -5.0f, -2.0f),
		  glm::vec3(4.0f, -5.0f, -2.0f),
		  glm::vec3(5.0f, -5.0f, -2.0f),
		  glm::vec3(6.0f, -5.0f, -2.0f),
		  glm::vec3(7.0f, -5.0f, -2.0f),
		  glm::vec3(8.0f, -5.0f, -2.0f),
		  glm::vec3(9.0f, -5.0f, -2.0f),
		  glm::vec3(0.0f, -5.0f, -3.0f),
		  glm::vec3(1.0f, -5.0f, -3.0f),
		  glm::vec3(2.0f, -5.0f, -3.0f),
		  glm::vec3(3.0f, -5.0f, -3.0f),
		  glm::vec3(4.0f, -5.0f, -3.0f),
		  glm::vec3(5.0f, -5.0f, -3.0f),
		  glm::vec3(6.0f, -5.0f, -3.0f),
		  glm::vec3(7.0f, -5.0f, -3.0f),
		  glm::vec3(8.0f, -5.0f, -3.0f),
		  glm::vec3(9.0f, -5.0f, -3.0f),
		  glm::vec3(0.0f, -5.0f, -4.0f),
		  glm::vec3(1.0f, -5.0f, -4.0f),
		  glm::vec3(2.0f, -5.0f, -4.0f),
		  glm::vec3(3.0f, -5.0f, -4.0f),
		  glm::vec3(4.0f, -5.0f, -4.0f),
		  glm::vec3(5.0f, -5.0f, -4.0f),
		  glm::vec3(6.0f, -5.0f, -4.0f),
		  glm::vec3(7.0f, -5.0f, -4.0f),
		  glm::vec3(8.0f, -5.0f, -4.0f),
		  glm::vec3(9.0f, -5.0f, -4.0f),
		  glm::vec3(0.0f, -5.0f, -5.0f),
		  glm::vec3(1.0f, -5.0f, -5.0f),
		  glm::vec3(2.0f, -5.0f, -5.0f),
		  glm::vec3(3.0f, -5.0f, -5.0f),
		  glm::vec3(4.0f, -5.0f, -5.0f),
		  glm::vec3(5.0f, -5.0f, -5.0f),
		  glm::vec3(6.0f, -5.0f, -5.0f),
		  glm::vec3(7.0f, -5.0f, -5.0f),
		  glm::vec3(8.0f, -5.0f, -5.0f),
		  glm::vec3(9.0f, -5.0f, -5.0f),
		  glm::vec3(0.0f, -5.0f, -6.0f),
		  glm::vec3(1.0f, -5.0f, -6.0f),
		  glm::vec3(2.0f, -5.0f, -6.0f),
		  glm::vec3(3.0f, -5.0f, -6.0f),
		  glm::vec3(4.0f, -5.0f, -6.0f),
		  glm::vec3(5.0f, -5.0f, -6.0f),
		  glm::vec3(6.0f, -5.0f, -6.0f),
		  glm::vec3(7.0f, -5.0f, -6.0f),
		  glm::vec3(8.0f, -5.0f, -6.0f),
		  glm::vec3(9.0f, -5.0f, -6.0f),
		  glm::vec3(0.0f, -5.0f, -7.0f),
		  glm::vec3(1.0f, -5.0f, -7.0f),
		  glm::vec3(2.0f, -5.0f, -7.0f),
		  glm::vec3(3.0f, -5.0f, -7.0f),
		  glm::vec3(4.0f, -5.0f, -7.0f),
		  glm::vec3(5.0f, -5.0f, -7.0f),
		  glm::vec3(6.0f, -5.0f, -7.0f),
		  glm::vec3(7.0f, -5.0f, -7.0f),
		  glm::vec3(8.0f, -5.0f, -7.0f),
		  glm::vec3(9.0f, -5.0f, -7.0f),
		  glm::vec3(0.0f, -5.0f, -8.0f),
		  glm::vec3(1.0f, -5.0f, -8.0f),
		  glm::vec3(2.0f, -5.0f, -8.0f),
		  glm::vec3(3.0f, -5.0f, -8.0f),
		  glm::vec3(4.0f, -5.0f, -8.0f),
		  glm::vec3(5.0f, -5.0f, -8.0f),
		  glm::vec3(6.0f, -5.0f, -8.0f),
		  glm::vec3(7.0f, -5.0f, -8.0f),
		  glm::vec3(8.0f, -5.0f, -8.0f),
		  glm::vec3(9.0f, -5.0f, -8.0f),
		  glm::vec3(0.0f, -5.0f, -9.0f),
		  glm::vec3(1.0f, -5.0f, -9.0f),
		  glm::vec3(2.0f, -5.0f, -9.0f),
		  glm::vec3(3.0f, -5.0f, -9.0f),
		  glm::vec3(4.0f, -5.0f, -9.0f),
		  glm::vec3(5.0f, -5.0f, -9.0f),
		  glm::vec3(6.0f, -5.0f, -9.0f),
		  glm::vec3(7.0f, -5.0f, -9.0f),
		  glm::vec3(8.0f, -5.0f, -9.0f),
		  glm::vec3(9.0f, -5.0f, -9.0f),
			  glm::vec3(10.0f, -5.0f, -0.0f),
			  glm::vec3(11.0f, -5.0f, -0.0f),
			  glm::vec3(12.0f, -5.0f, -0.0f),
			  glm::vec3(13.0f, -5.0f, -0.0f),
			  glm::vec3(14.0f, -5.0f, -0.0f),
			  glm::vec3(15.0f, -5.0f, -0.0f),
			  glm::vec3(16.0f, -5.0f, -0.0f),
			  glm::vec3(17.0f, -5.0f, -0.0f),
			  glm::vec3(18.0f, -5.0f, -0.0f),
			  glm::vec3(19.0f, -5.0f, -0.0f),
			  glm::vec3(10.0f, -5.0f, -1.0f),
			  glm::vec3(11.0f, -5.0f, -1.0f),
			  glm::vec3(12.0f, -5.0f, -1.0f),
			  glm::vec3(13.0f, -5.0f, -1.0f),
			  glm::vec3(14.0f, -5.0f, -1.0f),
			  glm::vec3(15.0f, -5.0f, -1.0f),
			  glm::vec3(16.0f, -5.0f, -1.0f),
			  glm::vec3(17.0f, -5.0f, -1.0f),
			  glm::vec3(18.0f, -5.0f, -1.0f),
			  glm::vec3(19.0f, -5.0f, -1.0f),
			  glm::vec3(10.0f, -5.0f, -2.0f),
			  glm::vec3(11.0f, -5.0f, -2.0f),
			  glm::vec3(12.0f, -5.0f, -2.0f),
			  glm::vec3(13.0f, -5.0f, -2.0f),
			  glm::vec3(14.0f, -5.0f, -2.0f),
			  glm::vec3(15.0f, -5.0f, -2.0f),
			  glm::vec3(16.0f, -5.0f, -2.0f),
			  glm::vec3(17.0f, -5.0f, -2.0f),
			  glm::vec3(18.0f, -5.0f, -2.0f),
			  glm::vec3(19.0f, -5.0f, -2.0f),
			  glm::vec3(10.0f, -5.0f, -3.0f),
			  glm::vec3(11.0f, -5.0f, -3.0f),
			  glm::vec3(12.0f, -5.0f, -3.0f),
			  glm::vec3(13.0f, -5.0f, -3.0f),
			  glm::vec3(14.0f, -5.0f, -3.0f),
			  glm::vec3(15.0f, -5.0f, -3.0f),
			  glm::vec3(16.0f, -5.0f, -3.0f),
			  glm::vec3(17.0f, -5.0f, -3.0f),
			  glm::vec3(18.0f, -5.0f, -3.0f),
			  glm::vec3(19.0f, -5.0f, -3.0f),
			  glm::vec3(10.0f, -5.0f, -4.0f),
			  glm::vec3(11.0f, -5.0f, -4.0f),
			  glm::vec3(12.0f, -5.0f, -4.0f),
			  glm::vec3(13.0f, -5.0f, -4.0f),
			  glm::vec3(14.0f, -5.0f, -4.0f),
			  glm::vec3(15.0f, -5.0f, -4.0f),
			  glm::vec3(16.0f, -5.0f, -4.0f),
			  glm::vec3(17.0f, -5.0f, -4.0f),
			  glm::vec3(18.0f, -5.0f, -4.0f),
			  glm::vec3(19.0f, -5.0f, -4.0f),
			  glm::vec3(10.0f, -5.0f, -5.0f),
			  glm::vec3(11.0f, -5.0f, -5.0f),
			  glm::vec3(12.0f, -5.0f, -5.0f),
			  glm::vec3(13.0f, -5.0f, -5.0f),
			  glm::vec3(14.0f, -5.0f, -5.0f),
			  glm::vec3(15.0f, -5.0f, -5.0f),
			  glm::vec3(16.0f, -5.0f, -5.0f),
			  glm::vec3(17.0f, -5.0f, -5.0f),
			  glm::vec3(18.0f, -5.0f, -5.0f),
			  glm::vec3(19.0f, -5.0f, -5.0f),
			  glm::vec3(10.0f, -5.0f, -6.0f),
			  glm::vec3(11.0f, -5.0f, -6.0f),
			  glm::vec3(12.0f, -5.0f, -6.0f),
			  glm::vec3(13.0f, -5.0f, -6.0f),
			  glm::vec3(14.0f, -5.0f, -6.0f),
			  glm::vec3(15.0f, -5.0f, -6.0f),
			  glm::vec3(16.0f, -5.0f, -6.0f),
			  glm::vec3(17.0f, -5.0f, -6.0f),
			  glm::vec3(18.0f, -5.0f, -6.0f),
			  glm::vec3(19.0f, -5.0f, -6.0f),
			  glm::vec3(10.0f, -5.0f, -7.0f),
			  glm::vec3(11.0f, -5.0f, -7.0f),
			  glm::vec3(12.0f, -5.0f, -7.0f),
			  glm::vec3(13.0f, -5.0f, -7.0f),
			  glm::vec3(14.0f, -5.0f, -7.0f),
			  glm::vec3(15.0f, -5.0f, -7.0f),
			  glm::vec3(16.0f, -5.0f, -7.0f),
			  glm::vec3(17.0f, -5.0f, -7.0f),
			  glm::vec3(18.0f, -5.0f, -7.0f),
			  glm::vec3(19.0f, -5.0f, -7.0f),
			  glm::vec3(10.0f, -5.0f, -8.0f),
			  glm::vec3(11.0f, -5.0f, -8.0f),
			  glm::vec3(12.0f, -5.0f, -8.0f),
			  glm::vec3(13.0f, -5.0f, -8.0f),
			  glm::vec3(14.0f, -5.0f, -8.0f),
			  glm::vec3(15.0f, -5.0f, -8.0f),
			  glm::vec3(16.0f, -5.0f, -8.0f),
			  glm::vec3(17.0f, -5.0f, -8.0f),
			  glm::vec3(18.0f, -5.0f, -8.0f),
			  glm::vec3(19.0f, -5.0f, -8.0f),
			  glm::vec3(10.0f, -5.0f, -9.0f),
			  glm::vec3(11.0f, -5.0f, -9.0f),
			  glm::vec3(12.0f, -5.0f, -9.0f),
			  glm::vec3(13.0f, -5.0f, -9.0f),
			  glm::vec3(14.0f, -5.0f, -9.0f),
			  glm::vec3(15.0f, -5.0f, -9.0f),
			  glm::vec3(16.0f, -5.0f, -9.0f),
			  glm::vec3(17.0f, -5.0f, -9.0f),
			  glm::vec3(18.0f, -5.0f, -9.0f),
			  glm::vec3(19.0f, -5.0f, -9.0f),
				  glm::vec3(0.0f, -5.0f, -10.0f),
				  glm::vec3(1.0f, -5.0f, -10.0f),
				  glm::vec3(2.0f, -5.0f, -10.0f),
				  glm::vec3(3.0f, -5.0f, -10.0f),
				  glm::vec3(4.0f, -5.0f, -10.0f),
				  glm::vec3(5.0f, -5.0f, -10.0f),
				  glm::vec3(6.0f, -5.0f, -10.0f),
				  glm::vec3(7.0f, -5.0f, -10.0f),
				  glm::vec3(8.0f, -5.0f, -10.0f),
				  glm::vec3(9.0f, -5.0f, -10.0f),
				  glm::vec3(0.0f, -5.0f, -11.0f),
				  glm::vec3(1.0f, -5.0f, -11.0f),
				  glm::vec3(2.0f, -5.0f, -11.0f),
				  glm::vec3(3.0f, -5.0f, -11.0f),
				  glm::vec3(4.0f, -5.0f, -11.0f),
				  glm::vec3(5.0f, -5.0f, -11.0f),
				  glm::vec3(6.0f, -5.0f, -11.0f),
				  glm::vec3(7.0f, -5.0f, -11.0f),
				  glm::vec3(8.0f, -5.0f, -11.0f),
				  glm::vec3(9.0f, -5.0f, -11.0f),
				  glm::vec3(0.0f, -5.0f, -12.0f),
				  glm::vec3(1.0f, -5.0f, -12.0f),
				  glm::vec3(2.0f, -5.0f, -12.0f),
				  glm::vec3(3.0f, -5.0f, -12.0f),
				  glm::vec3(4.0f, -5.0f, -12.0f),
				  glm::vec3(5.0f, -5.0f, -12.0f),
				  glm::vec3(6.0f, -5.0f, -12.0f),
				  glm::vec3(7.0f, -5.0f, -12.0f),
				  glm::vec3(8.0f, -5.0f, -12.0f),
				  glm::vec3(9.0f, -5.0f, -12.0f),
				  glm::vec3(0.0f, -5.0f, -13.0f),
				  glm::vec3(1.0f, -5.0f, -13.0f),
				  glm::vec3(2.0f, -5.0f, -13.0f),
				  glm::vec3(3.0f, -5.0f, -13.0f),
				  glm::vec3(4.0f, -5.0f, -13.0f),
				  glm::vec3(5.0f, -5.0f, -13.0f),
				  glm::vec3(6.0f, -5.0f, -13.0f),
				  glm::vec3(7.0f, -5.0f, -13.0f),
				  glm::vec3(8.0f, -5.0f, -13.0f),
				  glm::vec3(9.0f, -5.0f, -13.0f),
				  glm::vec3(0.0f, -5.0f, -14.0f),
				  glm::vec3(1.0f, -5.0f, -14.0f),
				  glm::vec3(2.0f, -5.0f, -14.0f),
				  glm::vec3(3.0f, -5.0f, -14.0f),
				  glm::vec3(4.0f, -5.0f, -14.0f),
				  glm::vec3(5.0f, -5.0f, -14.0f),
				  glm::vec3(6.0f, -5.0f, -14.0f),
				  glm::vec3(7.0f, -5.0f, -14.0f),
				  glm::vec3(8.0f, -5.0f, -14.0f),
				  glm::vec3(9.0f, -5.0f, -14.0f),
				  glm::vec3(0.0f, -5.0f, -15.0f),
				  glm::vec3(1.0f, -5.0f, -15.0f),
				  glm::vec3(2.0f, -5.0f, -15.0f),
				  glm::vec3(3.0f, -5.0f, -15.0f),
				  glm::vec3(4.0f, -5.0f, -15.0f),
				  glm::vec3(5.0f, -5.0f, -15.0f),
				  glm::vec3(6.0f, -5.0f, -15.0f),
				  glm::vec3(7.0f, -5.0f, -15.0f),
				  glm::vec3(8.0f, -5.0f, -15.0f),
				  glm::vec3(9.0f, -5.0f, -15.0f),
				  glm::vec3(0.0f, -5.0f, -16.0f),
				  glm::vec3(1.0f, -5.0f, -16.0f),
				  glm::vec3(2.0f, -5.0f, -16.0f),
				  glm::vec3(3.0f, -5.0f, -16.0f),
				  glm::vec3(4.0f, -5.0f, -16.0f),
				  glm::vec3(5.0f, -5.0f, -16.0f),
				  glm::vec3(6.0f, -5.0f, -16.0f),
				  glm::vec3(7.0f, -5.0f, -16.0f),
				  glm::vec3(8.0f, -5.0f, -16.0f),
				  glm::vec3(9.0f, -5.0f, -16.0f),
				  glm::vec3(0.0f, -5.0f, -17.0f),
				  glm::vec3(1.0f, -5.0f, -17.0f),
				  glm::vec3(2.0f, -5.0f, -17.0f),
				  glm::vec3(3.0f, -5.0f, -17.0f),
				  glm::vec3(4.0f, -5.0f, -17.0f),
				  glm::vec3(5.0f, -5.0f, -17.0f),
				  glm::vec3(6.0f, -5.0f, -17.0f),
				  glm::vec3(7.0f, -5.0f, -17.0f),
				  glm::vec3(8.0f, -5.0f, -17.0f),
				  glm::vec3(9.0f, -5.0f, -17.0f),
				  glm::vec3(0.0f, -5.0f, -18.0f),
				  glm::vec3(1.0f, -5.0f, -18.0f),
				  glm::vec3(2.0f, -5.0f, -18.0f),
				  glm::vec3(3.0f, -5.0f, -18.0f),
				  glm::vec3(4.0f, -5.0f, -18.0f),
				  glm::vec3(5.0f, -5.0f, -18.0f),
				  glm::vec3(6.0f, -5.0f, -18.0f),
				  glm::vec3(7.0f, -5.0f, -18.0f),
				  glm::vec3(8.0f, -5.0f, -18.0f),
				  glm::vec3(9.0f, -5.0f, -18.0f),
				  glm::vec3(0.0f, -5.0f, -19.0f),
				  glm::vec3(1.0f, -5.0f, -19.0f),
				  glm::vec3(2.0f, -5.0f, -19.0f),
				  glm::vec3(3.0f, -5.0f, -19.0f),
				  glm::vec3(4.0f, -5.0f, -19.0f),
				  glm::vec3(5.0f, -5.0f, -19.0f),
				  glm::vec3(6.0f, -5.0f, -19.0f),
				  glm::vec3(7.0f, -5.0f, -19.0f),
				  glm::vec3(8.0f, -5.0f, -19.0f),
				  glm::vec3(9.0f, -5.0f, -19.0f),
					  glm::vec3(10.0f, -5.0f, -10.0f),
					  glm::vec3(11.0f, -5.0f, -10.0f),
					  glm::vec3(12.0f, -5.0f, -10.0f),
					  glm::vec3(13.0f, -5.0f, -10.0f),
					  glm::vec3(14.0f, -5.0f, -10.0f),
					  glm::vec3(15.0f, -5.0f, -10.0f),
					  glm::vec3(16.0f, -5.0f, -10.0f),
					  glm::vec3(17.0f, -5.0f, -10.0f),
					  glm::vec3(18.0f, -5.0f, -10.0f),
					  glm::vec3(19.0f, -5.0f, -10.0f),
					  glm::vec3(10.0f, -5.0f, -11.0f),
					  glm::vec3(11.0f, -5.0f, -11.0f),
					  glm::vec3(12.0f, -5.0f, -11.0f),
					  glm::vec3(13.0f, -5.0f, -11.0f),
					  glm::vec3(14.0f, -5.0f, -11.0f),
					  glm::vec3(15.0f, -5.0f, -11.0f),
					  glm::vec3(16.0f, -5.0f, -11.0f),
					  glm::vec3(17.0f, -5.0f, -11.0f),
					  glm::vec3(18.0f, -5.0f, -11.0f),
					  glm::vec3(19.0f, -5.0f, -11.0f),
					  glm::vec3(10.0f, -5.0f, -12.0f),
					  glm::vec3(11.0f, -5.0f, -12.0f),
					  glm::vec3(12.0f, -5.0f, -12.0f),
					  glm::vec3(13.0f, -5.0f, -12.0f),
					  glm::vec3(14.0f, -5.0f, -12.0f),
					  glm::vec3(15.0f, -5.0f, -12.0f),
					  glm::vec3(16.0f, -5.0f, -12.0f),
					  glm::vec3(17.0f, -5.0f, -12.0f),
					  glm::vec3(18.0f, -5.0f, -12.0f),
					  glm::vec3(19.0f, -5.0f, -12.0f),
					  glm::vec3(10.0f, -5.0f, -13.0f),
					  glm::vec3(11.0f, -5.0f, -13.0f),
					  glm::vec3(12.0f, -5.0f, -13.0f),
					  glm::vec3(13.0f, -5.0f, -13.0f),
					  glm::vec3(14.0f, -5.0f, -13.0f),
					  glm::vec3(15.0f, -5.0f, -13.0f),
					  glm::vec3(16.0f, -5.0f, -13.0f),
					  glm::vec3(17.0f, -5.0f, -13.0f),
					  glm::vec3(18.0f, -5.0f, -13.0f),
					  glm::vec3(19.0f, -5.0f, -13.0f),
					  glm::vec3(10.0f, -5.0f, -14.0f),
					  glm::vec3(11.0f, -5.0f, -14.0f),
					  glm::vec3(12.0f, -5.0f, -14.0f),
					  glm::vec3(13.0f, -5.0f, -14.0f),
					  glm::vec3(14.0f, -5.0f, -14.0f),
					  glm::vec3(15.0f, -5.0f, -14.0f),
					  glm::vec3(16.0f, -5.0f, -14.0f),
					  glm::vec3(17.0f, -5.0f, -14.0f),
					  glm::vec3(18.0f, -5.0f, -14.0f),
					  glm::vec3(19.0f, -5.0f, -14.0f),
					  glm::vec3(10.0f, -5.0f, -15.0f),
					  glm::vec3(11.0f, -5.0f, -15.0f),
					  glm::vec3(12.0f, -5.0f, -15.0f),
					  glm::vec3(13.0f, -5.0f, -15.0f),
					  glm::vec3(14.0f, -5.0f, -15.0f),
					  glm::vec3(15.0f, -5.0f, -15.0f),
					  glm::vec3(16.0f, -5.0f, -15.0f),
					  glm::vec3(17.0f, -5.0f, -15.0f),
					  glm::vec3(18.0f, -5.0f, -15.0f),
					  glm::vec3(19.0f, -5.0f, -15.0f),
					  glm::vec3(10.0f, -5.0f, -16.0f),
					  glm::vec3(11.0f, -5.0f, -16.0f),
					  glm::vec3(12.0f, -5.0f, -16.0f),
					  glm::vec3(13.0f, -5.0f, -16.0f),
					  glm::vec3(14.0f, -5.0f, -16.0f),
					  glm::vec3(15.0f, -5.0f, -16.0f),
					  glm::vec3(16.0f, -5.0f, -16.0f),
					  glm::vec3(17.0f, -5.0f, -16.0f),
					  glm::vec3(18.0f, -5.0f, -16.0f),
					  glm::vec3(19.0f, -5.0f, -16.0f),
					  glm::vec3(10.0f, -5.0f, -17.0f),
					  glm::vec3(11.0f, -5.0f, -17.0f),
					  glm::vec3(12.0f, -5.0f, -17.0f),
					  glm::vec3(13.0f, -5.0f, -17.0f),
					  glm::vec3(14.0f, -5.0f, -17.0f),
					  glm::vec3(15.0f, -5.0f, -17.0f),
					  glm::vec3(16.0f, -5.0f, -17.0f),
					  glm::vec3(17.0f, -5.0f, -17.0f),
					  glm::vec3(18.0f, -5.0f, -17.0f),
					  glm::vec3(19.0f, -5.0f, -17.0f),
					  glm::vec3(10.0f, -5.0f, -18.0f),
					  glm::vec3(11.0f, -5.0f, -18.0f),
					  glm::vec3(12.0f, -5.0f, -18.0f),
					  glm::vec3(13.0f, -5.0f, -18.0f),
					  glm::vec3(14.0f, -5.0f, -18.0f),
					  glm::vec3(15.0f, -5.0f, -18.0f),
					  glm::vec3(16.0f, -5.0f, -18.0f),
					  glm::vec3(17.0f, -5.0f, -18.0f),
					  glm::vec3(18.0f, -5.0f, -18.0f),
					  glm::vec3(19.0f, -5.0f, -18.0f),
					  glm::vec3(10.0f, -5.0f, -19.0f),
					  glm::vec3(11.0f, -5.0f, -19.0f),
					  glm::vec3(12.0f, -5.0f, -19.0f),
					  glm::vec3(13.0f, -5.0f, -19.0f),
					  glm::vec3(14.0f, -5.0f, -19.0f),
					  glm::vec3(15.0f, -5.0f, -19.0f),
					  glm::vec3(16.0f, -5.0f, -19.0f),
					  glm::vec3(17.0f, -5.0f, -19.0f),
					  glm::vec3(18.0f, -5.0f, -19.0f),
					  glm::vec3(19.0f, -5.0f, -19.0f)
	};

	

	float cubeVertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};

	float floorVertices[]{
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
	};

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};



	
	drawCircle(0.f, 0.f, 0.125f, 0.25f, numberOfTriangles);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	SCR_WIDTH = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
	SCR_HEIGHT = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Suwnica Model 3D", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	GLenum err = glewInit();
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glm::vec3 lightDirection(-280.0f, 150.0f, -360.0f);


	VertexArray skyboxVA, floorVA, elementVA, railsVA, circleVA;
	// Skybox
	VertexBuffer skyboxVB(skyboxVertices, sizeof(skyboxVertices));
	VertexBufferLayout skyboxLayout;
	skyboxLayout.Push<float>(3);
	skyboxVA.AddBuffer(skyboxVB, skyboxLayout);
	Skybox skybox;
	unsigned int x = skybox.loadCubemap();
	skybox.Bind();
	Shader skyboxShader("shaders/skybox.shader");
	skyboxShader.SetUniform1i("skybox", 0);
	// floor
	VertexBuffer floorVB(floorVertices, sizeof(floorVertices));
	VertexBufferLayout floorLayout;
	floorLayout.Push<float>(3);
	floorLayout.Push<float>(3);
	floorLayout.Push<float>(2);
 	floorVA.AddBuffer(floorVB, floorLayout); 
 	Shader floorShader("shaders/floorTexture.shader");
 	Texture floorTexture("textures/concrete2.jpeg");
 	floorShader.Bind();
 	floorShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
 	floorShader.SetUniform1i("material.diffuse", 0);
 	floorShader.SetUniform1f("material.shininess", 6.0f);
	floorShader.SetUniform3f("light.ambient", 0.6, 0.6f, 0.6f);
	floorShader.SetUniform3f("light.diffuse", 0.7f, 0.7f, 0.7f);
	floorShader.SetUniform3f("light.specular", 0.2f, 0.2f, 0.2f);
	//element
	VertexBuffer elementVB(cubeVertices, sizeof(cubeVertices));
	VertexBufferLayout cubeLayout;
	cubeLayout.Push<float>(3);
	cubeLayout.Push<float>(3);
	cubeLayout.Push<float>(2);
	elementVA.AddBuffer(elementVB, cubeLayout);
	Shader elementShader("shaders/lightMaterial.shader");
	elementShader.Bind();
	elementShader.SetUniform3f("objectColor", 1.0f, 0.82f, 0.0f);
	elementShader.SetUniform3f("material.ambient", 0.329412f, 0.223529f, 0.027451f);
	elementShader.SetUniform3f("material.diffuse", 0.780392f, 0.568627f, 0.113725f);
	elementShader.SetUniform1f("material.shininess", 28.0f);
	elementShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	elementShader.SetUniform3f("light.diffuse", 0.9f, 0.9f, 0.9f);
	elementShader.SetUniform3f("light.specular", 0.992157f, 0.941176f, 0.807843f);
	//rails
	railsVA.AddBuffer(elementVB, cubeLayout);
	Shader railsShader("shaders/lightMaterial.shader");
	railsShader.Bind();
	railsShader.SetUniform3f("objectColor", 0.0f, 0.02f, 0.0f);
	railsShader.SetUniform3f("material.ambient", 0.329412f, 0.223529f, 0.027451f);
	railsShader.SetUniform3f("material.diffuse", 0.780392f, 0.568627f, 0.113725f);
	railsShader.SetUniform1f("material.shininess", 28.0f);
	railsShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	railsShader.SetUniform3f("light.diffuse", 0.9f, 0.9f, 0.9f);
	railsShader.SetUniform3f("light.specular", 0.992157f, 0.941176f, 0.807843f);
	// metal
	Shader metalShader("shaders/lightMaterial.shader");
	metalShader.Bind();
	metalShader.SetUniform3f("objectColor", 0.9f, 0.9f, 0.98f);
	metalShader.SetUniform3f("material.ambient", 0.10f, 0.10f, 0.10f);
	metalShader.SetUniform3f("material.diffuse", 0.8f, 0.8f, 0.8f);
	metalShader.SetUniform1f("material.shininess", 75.0f);
	metalShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	metalShader.SetUniform3f("light.diffuse", 1.f, 1.f, 1.f);
	metalShader.SetUniform3f("light.specular", 1.f, 1.f, 1.f);
	// circle
	VertexBuffer circleVB(allCircleVertices, sizeof(allCircleVertices));
	VertexBufferLayout circleLayout;
	circleLayout.Push<float>(3);
	circleLayout.Push<float>(3);
	circleVA.AddBuffer(circleVB, circleLayout);
	Shader circleShader("shaders/light.shader");
	circleShader.Bind();
	circleShader.SetUniform3f("objectColor", 0.6f, 0.6f, 0.6f);
	circleShader.SetUniform3f("material.ambient", 0.10f, 0.10f, 0.10f);
	circleShader.SetUniform3f("material.diffuse", 0.8f, 0.8f, 0.8f);
	circleShader.SetUniform1f("material.shininess", 75.0f);
	circleShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	circleShader.SetUniform3f("light.diffuse", 1.f, 1.f, 1.f);
	circleShader.SetUniform3f("light.specular", 1.f, 1.f, 1.f);
	//barrel
	Shader barrelShader("shaders/texture.shader");
	barrelShader.Bind();
	barrelShader.SetUniform1i("material.diffuse", 6);
	barrelShader.SetUniform1i("material.specular", 1);
	barrelShader.SetUniform1f("material.shininess", 16.0f);
	barrelShader.SetUniform3f("light.ambient", 0.15f, 0.15f, 0.15f);
	barrelShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
	barrelShader.SetUniform3f("light.diffuse", .75f, .75f, .75f);
	barrelShader.SetUniform3f("light.specular", 1.f, 1.f, 1.f);

	Texture barrelTexture("textures/barrel.png");
	Texture barrelTextureMap("textures/map.png");
	floorTexture.Bind(0);
	barrelTextureMap.Bind(1);
	barrelTexture.Bind(6);


	// ImGui
	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	// ImGui
	Renderer renderer;	
	glEnable(GL_DEPTH_TEST);

	float moving = 0.f;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		renderer.Clear();
		ImGui_ImplGlfwGL3_NewFrame();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model = glm::mat4(1.0f);


		moving += moveFloat;
		// floor
		model = glm::mat4(1.0f);
		floorShader.Bind();
		floorShader.setMat4("projection", projection);
		floorShader.setMat4("view", view);
		floorShader.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		glm::vec3 move(0.0f, -5.0f, 0.0f);
		model = glm::translate(model, move);
		for (int i = 0; i < 400; i++) {
			model = glm::mat4(1.0f);
			model = glm::scale(model, glm::vec3(2.0f, 1.0f, 2.0f));
			model = glm::translate(model, movement[i]);
			floorShader.setMat4("model", model);
			renderer.Draw(floorVA, floorShader, 6);
		}
		// rails
		model = glm::mat4(1.0f);
		railsShader.Bind();
		railsShader.setMat4("projection", projection);
		railsShader.setMat4("view", view);
		railsShader.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		model = glm::translate(model, glm::vec3(15.0f, -4.375f, -15.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 20.0f));
		railsShader.setMat4("model", model);
		renderer.Draw(railsVA, railsShader, 36);
		model = glm::translate(model, glm::vec3(48.0f, 0.0f, 0.0f));
		railsShader.setMat4("model", model);
 		renderer.Draw(railsVA, railsShader, 36);


		// element
		model = glm::mat4(1.0f);
		elementShader.Bind();
		elementShader.setMat4("projection", projection);
		elementShader.setMat4("view", view);
		elementShader.SetUniform3f("viewPos",camera.Position.x, camera.Position.y, camera.Position.z);

		// podstawa
		model = glm::translate(model, glm::vec3(15.0f, -3.75f, -10.0f - moving));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 8.0f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::translate(model, glm::vec3(24.0f, 0.0f, 0.0f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

		// krzywe
		model = glm::scale(model, glm::vec3(2.f, 2.f, 0.125f));
		model = glm::translate(model, glm::vec3(0.35f, 4.0f, 2.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));		
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.37f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.37f, 0.0f, -4.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.37f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-11.3f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.37f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.37f, 0.0f, 4.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.37f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.4f, 8.0f, 0.4f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		
		//dach
		model = glm::scale(model, glm::vec3(2.5f, 0.125f, 2.5f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.f, 3.75f, -2.0f));
		model = glm::scale(model, glm::vec3(15.f, 0.5f, 4.f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

		//metal
		metalShader.Bind();
		metalShader.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		metalShader.setMat4("projection", projection);
		metalShader.setMat4("view", view);
		model = glm::scale(model, glm::vec3(0.8f, 0.5f, 0.0625f));
		model = glm::translate(model, glm::vec3(0.f, -0.75f, 0.0f));
		metalShader.setMat4("model", model);
		renderer.Draw(elementVA, metalShader, 36);

		railsShader.Bind();
		model = glm::translate(model, glm::vec3(0.f, -0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 1.f, 0.25f)); // 0.25/4 x 5 x 0.25/4
		railsShader.setMat4("model", model);
		renderer.Draw(elementVA, railsShader, 36);


		model = glm::scale(model, glm::vec3(0.09259259259f, 1.f, 1.f));
		metalShader.Bind();
		model = glm::translate(model, glm::vec3(0.f + moveFloat2, -9.95f, 0.0f));
		model = glm::scale(model, glm::vec3(0.015625f, 20.f, 0.25f)); // /64 x 5 x /64
		metalShader.setMat4("model", model);
		renderer.Draw(elementVA, metalShader, 36);
		
		model = glm::scale(model, glm::vec3(64.f, 0.2f, 64.f));
		model = glm::translate(model, glm::vec3(0.f, -3.f, 0.f));
		barrelShader.Bind();
		barrelShader.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		barrelShader.setMat4("projection", projection);
		barrelShader.setMat4("view", view);
		barrelShader.setMat4("model", model);
		renderer.Draw(elementVA, barrelShader, 36);


		//circle
		model = glm::mat4(1.0f);
		circleShader.Bind();
		circleShader.SetUniform3f("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
		circleShader.setMat4("projection", projection);
		circleShader.setMat4("view", view);
		model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 1.f, 0.f));
		model = glm::translate(model, glm::vec3(7.5f + moving, -4.f, 15.0f));
		circleShader.setMat4("model", model);
		renderer.Draw(circleVA, circleShader, numberOfTriangles * 4 * 3);
		model = glm::translate(model, glm::vec3(5.0f, 0.f, 0.f));
		circleShader.setMat4("model", model);
		renderer.Draw(circleVA, circleShader, numberOfTriangles * 4 * 3);
		model = glm::translate(model, glm::vec3(.0f, 0.0f, 12.0f));
		circleShader.setMat4("model", model);
		renderer.Draw(circleVA, circleShader, numberOfTriangles * 4 * 3);
		model = glm::translate(model, glm::vec3(-5.0f, 0.f, 0.f));
		circleShader.setMat4("model", model);
		renderer.Draw(circleVA, circleShader, numberOfTriangles * 4 * 3);

		// skybox
		model = glm::mat4(1.0f);
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Bind();
		skyboxShader.setMat4("projection", projection);
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		renderer.Draw(skyboxVA, skyboxShader, 36);
		glDepthFunc(GL_LESS);

		/*{
			//ImGui::SliderFloat3("light", &f.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			//ImGui::SliderFloat3("myPos", &y.x, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
				f.x += 0.0001f;
			if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
				f.x -= 0.0001f;
			if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
				f.y += 0.0001f;
			if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
				f.y -= 0.0001f;
			if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
				f.z += 0.0001f;
			if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
				f.z -= 0.0001f;
			
		}*/


		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	
	glfwTerminate();
	return 0;

}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		moveFloat += 0.0001f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		moveFloat -= 0.0001f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		if (moveFloat2 > - 5.f)
			moveFloat2 -= 0.002f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		if (moveFloat2 < 5.f)
			moveFloat2 += 0.002f;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}