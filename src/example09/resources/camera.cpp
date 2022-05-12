#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 pos)
{
	Camera::width = width;
	Camera::height = height;
	position = pos;
}

void Camera::Matrix(float FOVdeg, float near_plane, float far_plane, Shader& shader, const char* uniform)
{
	// initialize matrices
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	// make camera look in the right direction from the right position
	view = glm::lookAt(position, position + orientation, up);
	// add perspective to the scene
	projection = glm::perspective(glm::radians(FOVdeg), (float)(width/height), near_plane, far_plane);

	// export the camera matrix to the vertex shader
	glUniformMatrix4fv(glGetUniformLocation(shader.shader_program, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow* window)
{
	// key inputs
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{ // forward 
		position += speed * orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{ // left
		position += speed * glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{ // backward
		position += speed * -orientation;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{ // right
		position += speed * -glm::normalize(glm::cross(orientation, up));
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{ // down
		position += speed * up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{ // up
		position += speed * -up;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{ // increase speed when left shift is pressed
		speed = 0.4f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{ // return speed to default is left shift is released
		speed = 0.1f;
	}

	// mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// hide mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// prevent camera from jumping on the first click
		if (first_click)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			first_click = false;
		}

		// store the coordinates of the cursor
		double mouseX;
		double mouseY;
		// get the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// normalize and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// calculate upcoming vertical change in the Orientation
		glm::vec3 new_orientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		// check whether or not the next vertical Orientation is legal or not
		if (abs(glm::angle(new_orientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = new_orientation;
		}

		// rotate the Orientation left and right
		orientation = glm::rotate(orientation, glm::radians(-rotY), up);

		// set mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// unhide cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// make sure the next time the camera looks around it doesn't jump
		first_click = true;
	}
}