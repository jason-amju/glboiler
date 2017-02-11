#include <iostream>
#include "boiler_assert.h"
#include "file.h"
#include "gl_includes.h"
#include "obj_util.h"
#include "string_utils.h"
//?#include "ResourceManager.h"

// Convert a vector of four strings to a Vec3. 
// The zeroth string is ignored. Strings 1, 2 & 3 are
//  asssumed to be floats.
vec3 ToVec3(const strings& strs)
{
  if (strs.size() != 4)
  {
    std::cout << "Unexpected format for Vec3\n";
    gl_boiler_stop;
    return vec3();
  }
  return vec3(to_float(strs[1]), to_float(strs[2]), to_float(strs[3]));
}

// Convert a vector of three strings to a Vec2. 
// The zeroth string is ignored. Strings 1 & 2 are
//  asssumed to be floats.
vec2 ToVec2(const strings& strs)
{
  if (strs.size() != 3)
  {
    std::cout << "Unexpected format for Vec2\n";
    gl_boiler_stop;
    return vec2();
  }
  return vec2(to_float(strs[1]), to_float(strs[2]));
}

Face::Face()
{
  m_pointIndex[0] = m_pointIndex[1] = m_pointIndex[2] = 0;
  m_normalIndex[0] = m_normalIndex[1] = m_normalIndex[2] = 0;
  m_uvIndex[0] = m_uvIndex[1] = m_uvIndex[2] = 0;
}

// Convert a vector of strings to a Face.
// The zeroth string is ignored. We only use the first 
//  three vertices.
Face ToFace(const strings& fstrs)
{
  Face f;
  // strs[1] is "vertex[/UV[/normal]] vertex[/UV[/normal]] ..."
  // Where [ ] means optional. I.e. the data could just be the point index, or
  //  just the point index and UV index.
  // TODO We only handle triangles - handle other size polys ?
  if (fstrs.size() < 4)
  {
    std::cout << "Unexpected format for face";
    return f;
  }

  for (int i = 0; i < 3 /*strs.size()*/; i++)
  {
    strings strs = split(fstrs[i + 1], '/');
    //assert(strs.size() == 3);

    if (strs.size() > 0 && !strs[0].empty())
    {
      f.m_pointIndex[i] = to_int(strs[0]) - 1;
    }
    if (strs.size() > 1 && !strs[1].empty())
    {
      f.m_uvIndex[i] = to_int(strs[1]) - 1;
    }
    if (strs.size() > 2 && !strs[2].empty())
    {
      f.m_normalIndex[i] = to_int(strs[2]) - 1;
    }
  }
  return f;
}

void Group::upload_on_gl_thread()
{
  GL_CHECK(glGenVertexArrays(1, &m_vao));
  GL_CHECK(glBindVertexArray(m_vao));

  GL_CHECK(glGenBuffers(1, &m_vbo));
  GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vbo));
  GL_CHECK(glBufferData(GL_ARRAY_BUFFER, m_tris.size() * sizeof(vertex) * 3, &m_tris[0], GL_STATIC_DRAW));

  GL_CHECK(glEnableVertexAttribArray(0));
  GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), NULL));

  GL_CHECK(glEnableVertexAttribArray(1));
  GL_CHECK(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 3)));

  GL_CHECK(glEnableVertexAttribArray(2));
  GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 5)));

  GL_CHECK(glEnableVertexAttribArray(3));
  GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)(sizeof(float) * 8)));
}

void Group::use_on_gl_thread()
{
  if (!IsVisible())
  {
    return;
  }

  // TODO TEMP TEST - GL 1.1 stylee
  /*
  glBegin(GL_TRIANGLES);
  for (unsigned int i = 0; i < m_tris.size(); i++)
  {
    const Tri& tri = m_tris[i];
    for (int v = 0; v < 3; v++)
    {
      const Vert& vert = tri.m_verts[v];
      glNormal3fv(&vert.m_nx);
      glTexCoord2fv(&vert.m_u);
      glVertex3fv(&vert.m_x);
    }
  }
  glEnd();
  */

  // Should be:
  GL_CHECK(glBindVertexArray(m_vao));
  GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 3 * m_tris.size()));
  GL_CHECK(glBindVertexArray(0)); // right?
}

void Group::destroy_on_gl_thread()
{
  GL_CHECK(glDeleteBuffers(1, &m_vbo));
  GL_CHECK(glDeleteVertexArrays(1, &m_vao));
}