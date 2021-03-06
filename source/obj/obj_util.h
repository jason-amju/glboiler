// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

#pragma once

#include <map>
#include <string>
#include <vector>
#include "aabb.h"
#include "resource.h"
#include "string_utils.h"
#include "vec2.h"
#include "vec3.h"
#include "vertex.h"

class binary_file;

// Useful types
// ------------

// Face type - assumes all faces are triangles,
//  so is most efficient.
struct Face
{
  Face();

  // only triangles are allowed!
  int m_pointIndex[3];
  int m_normalIndex[3];
  int m_uvIndex[3];
};

typedef std::vector<Face> Faces;
typedef std::map<std::string, Faces> FaceMap;

// Group - has name, material name and collection of faces.
struct Group : public gl_resource
{
  virtual void upload_on_gl_thread() override;
  virtual void use_on_gl_thread() override;
  virtual void destroy_on_gl_thread() override;

  // Reloaded when obj file is reloaded
  virtual void reload() {}

  bool load_binary(binary_file& f);
  bool save_binary(binary_file& f);

  const aabb& get_aabb() const { return m_aabb; }

  std::string m_materialName; // TODO int index
  tris m_tris; 

  // VAO
  unsigned int m_vao = 0;
  unsigned int m_vbo = 0;

  // Bounding vol
  aabb m_aabb;
};

// Group with extra info for saving
struct SaveGroup
{
  Faces m_faces;
};

// Map of group name to group object
typedef std::map<std::string, Group> Groups;

// Other useful typedefs for collections
typedef std::vector<vec3> Vecs;
typedef std::vector<vec2> UVs;


// Useful functions
// ----------------

// Convert a vector of four strings to a Vec3. 
// The zeroth string is ignored. Strings 1, 2 & 3 are
//  asssumed to be floats.
vec3 ToVec3(const strings& strs);

// Convert a vector of three strings to a Vec2. 
// The zeroth string is ignored. Strings 1 & 2 are
//  asssumed to be floats.
vec2 ToVec2(const strings& strs);

// Convert a vector of strings to a Face.
// The zeroth string is ignored. We only use the first 
//  three vertices.
Face ToFace(const strings& fstrs);

