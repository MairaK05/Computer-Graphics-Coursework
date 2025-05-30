#include <common/maths.hpp>
#include <common/camera.hpp>

glm::mat4 Maths::translate(const glm::vec3& v)
{
    glm::mat4 translate(1.0f);
    translate[3][0] = v.x, translate[3][1] = v.y, translate[3][2] = v.z;
    return translate;
}

glm::mat4 Maths::scale(const glm::vec3& v)
{
    glm::mat4 scale(1.0f);
    scale[0][0] = v.x; scale[1][1] = v.y; scale[2][2] = v.z;
    return scale;
}
float Maths::radians(float angle)
{
    return angle * 3.1416f / 180.0f;
}

glm::mat4 Maths::rotate(const float& angle, glm::vec3 v)
{
    v = glm::normalize(v);
    float c = cos(angle);
    float s = sin(angle);
    float x2 = v.x * v.x, y2 = v.y * v.y, z2 = v.z * v.z;
    float xy = v.x * v.y, xz = v.x * v.z, yz = v.y * v.z;
    float xs = v.x * s, ys = v.y * s, zs = v.z * s;

    glm::mat4 rotate;
    rotate[0][0] = (1 - c) * x2 + c;
    rotate[0][1] = (1 - c) * xy + zs;
    rotate[0][2] = (1 - c) * xz - ys;
    rotate[1][0] = (1 - c) * xy - zs;
    rotate[1][1] = (1 - c) * y2 + c;
    rotate[1][2] = (1 - c) * yz + xs;
    rotate[2][0] = (1 - c) * xz + ys;
    rotate[2][1] = (1 - c) * yz - xs;
    rotate[2][2] = (1 - c) * z2 + c;

    return rotate;
}

glm::mat4 Maths::lookAt(glm::vec3 eye, glm::vec3 target, glm::vec3 worldUp) {
    glm::vec3 front = glm::normalize(target - eye);
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    glm::vec3 up = Maths::cross(right, front);

    glm::mat4 rotate = glm::mat4(
        right.x, up.x, -front.x, 0.0f,
        right.y, up.y, -front.y, 0.0f,
        right.z, up.z, -front.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -eye);

    glm::mat4 view = rotate * translate;

    return view;
}


glm::mat4 Maths::perspective(float fov, float aspect, float near, float far) {
    float top = near * tan(fov / 2.0f);
    float right = aspect * top;

    float C = -(far + near) / (far - near);
    float D = -(2.0f * far * near) / (far - near);
    glm::mat4 projection(
        near / right, 0.0f, 0.0f, 0.0f,
        0.0f, near / top, 0.0f, 0.0f,
        0.0f, 0.0f, C, -1.0f,
        0.0f, 0.0f, D, 0.0f
    );

    return projection;
}

glm::vec3 Maths::cross(const glm::vec3& a, const glm::vec3& b) {
    glm::vec3 crossProduct;
    crossProduct.x = (a.y * b.z) - (a.z * b.y);
    crossProduct.y = (a.z * b.x) - (a.x * b.z);
    crossProduct.z = (a.x * b.y) - (a.y * b.x);
    return crossProduct;
}

