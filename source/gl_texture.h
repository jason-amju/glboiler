// -----------------------------------------------------------------------------
// glboiler - Jason Colman 2016-2017 - OpenGL experiments
// -----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include "colour.h"
#include "resource.h"
#include "vec2.h"

class gl_texture : public gl_resource
{
public:
  ~gl_texture();

  bool load(const std::string& filename);

  unsigned int get_width() const { return m_w; }
  unsigned int get_height() const { return m_h; }
  unsigned int get_bytes_per_pixel() const { return m_bytes_per_pixel; }

  // Look up texel colour in data: for this to work, the data must 
  //  not be deleted yet, of course! 
  colour get_texel_colour(const vec2& uv) const;

  virtual void upload_on_gl_thread() override;

  // Use special use_on_gl_thread to set the active texture ID
  virtual void use_on_gl_thread() override { use_on_gl_thread(0); }
  void use_on_gl_thread(int active_texture_id);

  virtual void destroy_on_gl_thread() override;

  virtual void reload() override;

private:
  void free_data();

private:
  std::string m_filename;
  std::vector<unsigned char> m_data;
  unsigned int m_bind_texture_id = 0;
  unsigned int m_w = 0;
  unsigned int m_h = 0;
  unsigned int m_bytes_per_pixel = 0;
  bool m_use_mipmaps = true;
  bool m_delete_data_after_upload = true;

  // TODO wrap/clamp, nearest/linear/aniso/etc  
};

