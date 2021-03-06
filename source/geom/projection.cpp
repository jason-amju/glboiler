// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

#include <cmath>
#include "projection.h"

void perspective::set_matrix(mat4& m) const
{
  float sine, cotangent, deltaZ;
  float radians = fov_y_degs / 2.0f * static_cast<float>(M_PI) / 180.0f;

  deltaZ = far_distance - near_distance; 
  sine = sin(radians);
  if ((deltaZ == 0) || (sine == 0) || (aspect_ratio == 0)) 
  {
    return;
  }

  cotangent = cos(radians) / sine;

  m.load_identity();
  m[0] = cotangent / aspect_ratio;
  m[5] = cotangent;
  m[10] = -(far_distance + near_distance) / deltaZ;
  m[11] = -1;
  m[14] = -2 * near_distance * far_distance / deltaZ;
  m[15] = 0;
}

void ortho::set_matrix(mat4& m) const
{
  m.load_identity();
  m[0] = 2.0f / (right - left);
  m[5] = 2.0f / (top - bottom);
  m[10] = -2.0f / (far_distance - near_distance);
  m[12] = -(right + left) / (right - left);
  m[13] = -(top + bottom) / (top - bottom);
  m[14] = -(far_distance + near_distance) / (far_distance - near_distance);
}

