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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

Camera camera(glm::vec3(35.0f, 0.0f, -35.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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


	VertexArray skyboxVA, floorVA, elementVA;
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
 	floorTexture.Bind();
 	floorShader.Bind();
 	floorShader.SetUniform3f("light.direction", lightDirection.x, lightDirection.y, lightDirection.z);
 	floorShader.SetUniform1i("material.diffuse", 0);
 	floorShader.SetUniform1f("material.shininess", 6.0f);
	floorShader.SetUniform3f("light.ambient", 0.6, 0.6f, 0.6f);
	floorShader.SetUniform3f("light.diffuse", 0.7f, 0.7f, 0.7f);
	floorShader.SetUniform3f("light.specular", 0.2f, 0.2f, 0.2f);
	Texture normalMap("textures/NormalMap.png");
	normalMap.Bind(3);
	floorShader.SetUniform1i("normalMap", 3);
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
		// element?

		model = glm::mat4(1.0f);
		elementShader.Bind();
		elementShader.setMat4("projection", projection);
		elementShader.setMat4("view", view);
		elementShader.SetUniform3f("viewPos",camera.Position.x, camera.Position.y, camera.Position.z);
		model = glm::translate(model, glm::vec3(15.0f, -0.5f, -10.0f));
		model = glm::scale(model, glm::vec3(1.0f, 8.0f, 1.0f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::translate(model, glm::vec3(12.0f, 0.0f, 0.0f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);
		model = glm::scale(model, glm::vec3(1.0f, 0.125f, 1.0f));
		model = glm::translate(model, glm::vec3(-6.0f, 4.0f, 0.0f));
		model = glm::scale(model, glm::vec3(11.5f, 1.0f, 1.0f));
		elementShader.setMat4("model", model);
		renderer.Draw(elementVA, elementShader, 36);

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

