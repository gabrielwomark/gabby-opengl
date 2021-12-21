#ifndef CAMERA_H
#define CAMERA_H
#include <glad/glad.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

#include "glm/glm/ext/matrix_transform.hpp"
#include "glm/glm/ext/vector_float3.hpp"
#include "glm/glm/fwd.hpp"
#include "glm/glm/geometric.hpp"

const float SPEED = 2.5;
const float SENSITIVITY = 0.1;
const float ZOOM = 45.0;

enum CameraMovement { FORWARD, BACKWARD, LEFT, RIGHT, UP, DOWN };

class Camera {
   public:
    // camera attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler angles
    float Yaw;    // rotation vertical axis (no elevation)
    float Pitch;  // increases elevation
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
           float pitch = 0.0)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
          MovementSpeed(SPEED),
          Zoom(ZOOM) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 GetViewMatrix() {
        return glm::lookAt(Position, Position + Front, Up);
    }

    void ProcessKeyboardInput(CameraMovement direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;

        glm::vec3 front_xz = glm::vec3(Front.x, 0.0f, Front.z);
        glm::vec3 right_xz = glm::vec3(Right.x, 0.0f, Right.z);
        switch (direction) {
            case FORWARD:
                Position += front_xz * velocity;
                break;
            case BACKWARD:
                Position -= front_xz * velocity;
                break;
            case RIGHT:
                Position += right_xz * velocity;
                break;
            case LEFT:
                Position -= right_xz * velocity;
                break;
            default:
                break;
        }
    }

    void ProcessMouseInput(float xoffset, float yoffset) {
        Yaw += xoffset * SENSITIVITY;
        Pitch += yoffset * SENSITIVITY;

        if (Pitch > 89.0) {
            Pitch = 89.0;
        }
        if (Pitch < -89.0) {
            Pitch = -89.0;
        }
        updateCameraVectors();
    }

   private:
    void updateCameraVectors() {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        Right = glm::normalize(glm::cross(Front, WorldUp));
        Up = glm::normalize(glm::cross(Right, Front));
    }
};

extern Camera camera;
extern float lastX;
extern float lastY;
extern bool firstMouse;
#endif
