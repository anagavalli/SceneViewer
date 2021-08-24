/**
 * Implementations of transforms defined in transform.h.
 */

#include "transform.h"

// Helper rotation function.  Please implement this.  
mat3 transform::rotate(const float degrees, const vec3& axis)
{
  mat3 identity(1.0);
  mat3 k(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0);
  float theta = glm::radians(degrees);

  return identity + sin(theta) * k + (1 - cos(theta)) * k * k;
}

void transform::left(float degrees, vec3& eye, vec3& up)
{
    eye = rotate(degrees, up) * eye;
}

void transform::up(float degrees, vec3& eye, vec3& up)
{
    vec3 w = normalize(cross(eye, up));
    mat3 rotMat = rotate(degrees, w);
    eye = rotMat * eye;
    up = rotMat * up;
}

mat4 transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
    vec3 w = normalize(eye - center);
    vec3 u = normalize(cross(up, w));
    vec3 v = cross(w, u);

    mat4 camera = mat4(
            u.x, v.x, w.x, 0,
            u.y, v.y, w.y, 0,
            u.z, v.z, w.z, 0,
            0, 0, 0, 1);
    mat4 translate = mat4(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -eye.x, -eye.y, -eye.z, 1);

    return camera * translate;
}

mat4 transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    const float focal = 1.0f / tanf(glm::radians(fovy / 2.0f));
    return mat4(
            focal / aspect, 0, 0, 0,
            0, focal, 0, 0,
            0, 0, (zFar + zNear) / (zNear - zFar), -1,
            0, 0, 2 * zFar * zNear / (zNear - zFar), 0);
}

mat4 transform::scale(const float &sx, const float &sy, const float &sz)
{
    return mat4(
            sx, 0, 0, 0,
            0, sy, 0, 0,
            0, 0,sz, 0,
            0, 0, 0, 1);
}

mat4 transform::translate(const float &tx, const float &ty, const float &tz)
{
  return mat4(
          1, 0, 0, 0,
          0, 1, 0, 0,
          0, 0, 1, 0,
          tx, ty, tz, 1);
}

vec3 transform::upvector(const vec3 &up, const vec3 & zvec)
{
  vec3 x = glm::cross(up,zvec); 
  vec3 y = glm::cross(zvec,x); 
  vec3 ret = glm::normalize(y); 
  return ret; 
}

transform::transform()
{

}

transform::~transform()
{

}
