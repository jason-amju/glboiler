// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

#pragma once

#include "render_pass_type.h"
#include "resource_manager.h"

class frustum;
class gl_shader;
class mat4;
class renderer;
class scene_graph;
class scene_node;

// * render_pass *
// Render pass base class. Owned by a renderer, to which we have a weak
//  pointer.
class render_pass
{
public:
  // When we construct, point back to the owning renderer (i.e. weak ptr)
  void set_renderer(renderer* r) { m_renderer = r; }
  renderer* get_renderer() { return m_renderer; }

  virtual void init_on_gl_thread(resource_manager& rm) = 0;
  virtual void destroy_on_gl_thread() = 0;
  virtual void render_on_gl_thread() = 0;

protected:
  // Traverse the scene, rendering the nodes which should be rendered 
  //  for the given pass type.
  void traverse_scene_for_pass(
    unsigned int pass, // value from render_pass_type
    const frustum&);

  virtual void draw_node(
    const scene_node& node,
    const frustum& fr,
    const mat4& combined_ancestor_xform) = 0;

  virtual void draw_bounding_vol(
    const scene_node& node,
    const frustum& fr,
    const mat4& combined_ancestor_xform) {}

protected:
  renderer* m_renderer = nullptr; // weak ptr
};

