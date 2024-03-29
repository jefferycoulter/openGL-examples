#ifndef CAMERA_INCL
#define CAMERA_INCL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shader.h"

class Camera
{
    public:
        glm::vec3 position;
        glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        // specify the "up" direction
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        // camera matrix
        glm::mat4 camera_matrix = glm::mat4(1.0f);

        // prevent the camera from jumping around when first clicking left click
	    bool first_click = true;

        // size of window
        int width;
        int height;

        // speed and sensitivity of camera movement
        float speed = 0.1f;
        float sensitivity = 100.0f;

        // constructor
        Camera(int width, int height, glm::vec3 pos);

        // update camera matrix to shader program
        void Update(float FOVdeg, float near_plane, float far_plane);
        // send view and project matrices to the shader program
        void Matrix(Shader& shader, const char* uniform);
        // camera inputs from user (e.g. keyboard, mouse events)
        void Inputs(GLFWwindow *window);
};

#endif // CAMERA_INCL