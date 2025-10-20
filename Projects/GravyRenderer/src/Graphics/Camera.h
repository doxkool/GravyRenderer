#pragma once

#include "Core/Base.h"
#include "Core/Time.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

//struct CameraSetings
//{
//    float movementSpeed     = 25.0f;
//    float fov               = 65.0f;
//    float nearPlane         = 0.1f;
//    float farPlane          = 1000.0f;
//    bool fixedAspectRatio   = false;
//    bool constrainPitch     = true;
//};


namespace Renderer
{
    // An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        Camera();
        Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        virtual ~Camera() = default;

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();

        glm::mat4 GetProjectionMatrix(float nearPlane = 0.1f, float farPlane = 1000.0f);

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void Move(camera_Movement direction);

        void Update();

        //void ConstrainMousePitch (bool constrainPitch);
        //void EnableMouseInput(bool enable);        
        //void SetPosition(glm::vec3 position);
        //void SetMovementSpeed(float speed) { MovementSpeed = speed; }
    
    public:
        // camera Attributes
        glm::vec3 Position      = {0.0f, 0.0f, 0.0f};
        glm::vec3 Front;
        glm::vec3 Up            = {0.0f, 1.0f, 0.0f};
        glm::vec3 Right;
        glm::vec3 WorldUp       = {0.0f, 1.0f, 0.0f};

        // euler Angles
        float Yaw               = 90.0f;
        float Pitch             = 0.0f;

        // camera options
        float MovementSpeed     = 50.0f;
        float MouseSensitivity  = 0.1f;
        float FOV               = 65.0f;
        bool b_Primary          = false;
        bool b_FixedAspectRatio = false;
        bool b_ConstrainPitch   = true;
        bool b_MouseInput       = false;

    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void UpdateCameraVectors();
    };
}
