// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

#include "gl_2_renderer.h"
#include "gl_includes.h"
#include "gl_shader.h"
#include "light.h"
#include "log.h"
#include "look_at.h"
#include "projection.h"

// Bias transforms (-1, 1) to (0, 1) space.
static const mat4 BIAS_MATRIX =
{
  0.5, 0.0, 0.0, 0.0,
  0.0, 0.5, 0.0, 0.0,
  0.0, 0.0, 0.5, 0.0,
  0.5, 0.5, 0.5, 1.0
};

gl_2_renderer::gl_2_renderer()
{
  m_shadow_map_size = 1024;
}

void gl_2_renderer::destroy_on_gl_thread()
{
  m_depth_shader.destroy_on_gl_thread();
  m_opaque_pass_shader.destroy_on_gl_thread();
  m_shadow_map.destroy_on_gl_thread();
}

void gl_2_renderer::init_on_gl_thread()
{
  m_shadow_map.set_render_flags(render_to_texture::RENDER_DEPTH);
  m_shadow_map.set_size(m_shadow_map_size, m_shadow_map_size); 
  m_shadow_map.init_on_gl_thread(); 

  // shader for shadow map
  m_depth_shader.load("shaders/gl_2_just_depth_v.txt", "shaders/gl_2_just_depth_f.txt");
  m_depth_shader.upload_on_gl_thread();
  m_depth_shader.use_on_gl_thread();

  m_opaque_pass_shader.load("shaders/gl_2_replicate_fixed_v.txt", "shaders/gl_2_replicate_fixed_f.txt");
  m_opaque_pass_shader.upload_on_gl_thread();
}

void gl_2_renderer::begin_render_on_gl_thread(const scene_graph& sg)
{
  renderer::begin_render_on_gl_thread(sg);

  GL_CHECK(glEnable(GL_CULL_FACE)); // test
  GL_CHECK(glFrontFace(GL_CW));
  GL_CHECK(glEnable(GL_DEPTH_TEST));

  clear_blended_nodes();
  shadow_map_pass(sg);
}

void gl_2_renderer::render_on_gl_thread(int view_index)
{
  m_opaque_pass_shader.use_on_gl_thread();
  
  m_opaque_pass_shader.set_int_on_gl_thread("diffuse_map", 0);
  m_opaque_pass_shader.set_int_on_gl_thread("shadow_map", 1);

//  sh.set_int_on_gl_thread("shadow_map_size", m_shadow_map_size);
  m_opaque_pass_shader.set_mat4_on_gl_thread("light_matrix", m_light_matrix);

  view& this_view = m_view[view_index];
  this_view.set_gl_viewport();
  const camera& cam = this_view.get_camera();

  m_opaque_pass_shader.set_mat4_on_gl_thread("look_at_matrix", cam.look_at_matrix);
  m_opaque_pass_shader.set_mat4_on_gl_thread("proj_matrix", cam.proj_matrix);

  frustum frust = this_view.calc_frustum();

  m_shadow_map.use_texture_on_gl_thread();

  // Render opaque geom
  opaque_pass(*m_sg, frust, &m_opaque_pass_shader);

  // Render blended nodes, for each eye
  draw_blended_nodes(frust);
}

void gl_2_renderer::end_render_on_gl_thread()
{
  m_shadow_map.use_texture_on_gl_thread();
  m_shadow_map.debug_draw_on_gl_thread();
}

void gl_2_renderer::clear_blended_nodes()
{
  m_blended_nodes.clear();
}

void gl_2_renderer::shadow_map_pass(const scene_graph& sg)
{
  m_depth_shader.use_on_gl_thread();

  // Orbiting light
  static float a = 0;
  a += 0.005f;
  float xz = 80.f; 
  float y = 20.f;
  vec3 light_pos(xz * cos(a), y, xz * sin(a));
  directional_light light(light_pos, -light_pos);
  light.calc_matrix(20.0, 0.1, 100.0);
  const mat4& m = light.get_matrix();
  
  m_depth_shader.set_mat4_on_gl_thread("view_proj_matrix", m);

  // Transform with bias and store for second pass
  m_light_matrix = mult(m, BIAS_MATRIX);

  m_shadow_map.begin_on_gl_thread();

  // Get light frustum (for each light which casts shadows?!)
  frustum light_frustum;

  GL_CHECK(glCullFace(GL_FRONT));

  traverse(sg, light_frustum, &m_depth_shader);

  m_shadow_map.end_on_gl_thread();
}

void gl_2_renderer::opaque_pass(const scene_graph& sg, const frustum& fr, gl_shader* override_shader)
{
  GL_CHECK(glCullFace(GL_BACK));

  GL_CHECK(glEnable(GL_CULL_FACE)); 


  GL_CHECK(glEnable(GL_DEPTH_TEST));
  traverse(sg, fr, override_shader);
}

void gl_2_renderer::draw_node(const scene_node& node, const frustum& fr, gl_shader* override_shader, const mat4& xform)
{
  gl_shader* sh = override_shader;

  // TODO use override shader if set, otherwise use material/shader for node
  // TODO should be material, not shader
  if (override_shader)
  {
    override_shader->use_on_gl_thread();
  }
  else
  {
    node.use_material_on_gl_thread();
  }
  node.use_material_on_gl_thread(); // TODO TEMP TEST

  sh->set_mat4_on_gl_thread("world_matrix", xform);
  node.render_on_gl_thread();
  m_render_stats.num_nodes_rendered++;
}

void gl_2_renderer::draw_blended_nodes(const frustum& fr)
{
}



