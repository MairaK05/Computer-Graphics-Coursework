#include <common/camera.hpp>
#include <common/maths.hpp>

Camera::Camera(const glm::vec3 Eye, const glm::vec3 Target)
{
    eye = Eye;
    target = Target;
}

void Camera::calculateMatrices()
{
    // Calculate camera vectors
    calculateCameraVectors();
    // Calculate the view matrix
    view = Maths::lookAt(eye, eye + front, worldUp);

    // Calculate the projection matrix
    projection = Maths::perspective(fov, aspect, near, far);



}

void Camera::calculateCameraVectors()
{
    front = glm::vec3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::cross(right, front);
}
