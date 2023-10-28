#include <SDL2/SDL.h>
#include "glad/glad.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb/stb_image.h"

#include "base/base_common.h"
#include "base/base_math.h"
#include "gl_render.h"

typedef R_Shader Shader;
typedef R_Texture2D Texture2D;
typedef R_GL_VAO VAO;

static void verify_shader(u32 id, u32 type);

bool _r_gl_check_error(void)
{
  bool result = FALSE;

  for (u32 err = -1; (err = glGetError());)
  {
    printf("[OpenGL Error]: %u\n", err);
    result = TRUE;
  }

  return result;
}

void _r_gl_clear_error(void)
{
  while (glGetError() != GL_NO_ERROR);
}

// @Buffer =====================================================================================

u32 r_gl_create_vertex_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ARRAY_BUFFER, id);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return id;
}

inline
void r_gl_bind_vertex_buffer(u32 id)
{
  glBindBuffer(GL_ARRAY_BUFFER, id);
}

inline
void r_gl_unbind_vertex_buffer(void)
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 r_gl_create_index_buffer(void *data, u32 size)
{
  u32 id;
  glGenBuffers(1, &id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  return id;
}

inline
void r_gl_bind_index_buffer(u32 id)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

inline
void r_gl_unbind_index_buffer(void)
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// @VAO ========================================================================================

VAO r_gl_create_vertex_array(u8 attrib_count)
{
  u32 id;
  GL_ASSERT(glGenVertexArrays(1, &id));
  GL_ASSERT(glBindVertexArray(id));

  return (VAO) {id, attrib_count, 0};
}

inline
void r_gl_bind_vertex_array(VAO *vao)
{
  GL_ASSERT(glBindVertexArray(vao->id));
}

inline
void r_gl_unbind_vertex_array(void)
{
  glBindVertexArray(0);
}

void r_gl_set_vertex_attribute(VAO *vao, GLenum data_type, u32 count)
{
  typedef struct Layout Layout;
  struct Layout
  {
    u32 index;
    u32 count;
    u32 data_type;
    bool normalized;
    u32 stride;
    void *offset;
  };

  u8 type_size;
  switch (data_type)
  {
    case GL_BYTE:  type_size = sizeof (i8);  break;
    case GL_SHORT: type_size = sizeof (i16); break;
    case GL_INT:   type_size = sizeof (i32); break;
    case GL_FLOAT: type_size = sizeof (f32); break;
    default: ASSERT(FALSE);
  }

  Layout layout = 
  {
    .index = vao->attrib_index,
    .count = count,
    .data_type = data_type,
    .normalized = FALSE,
    .stride = count * vao->attrib_count * type_size,
    .offset = (void *) (u64) (vao->attrib_index * count * type_size)
  };

  vao->attrib_index++;

  GL_ASSERT(glVertexAttribPointer(
                                  layout.index,
                                  layout.count,
                                  layout.data_type,
                                  layout.normalized,
                                  layout.stride,
                                  layout.offset));

  GL_ASSERT(glEnableVertexAttribArray(layout.index));
}

// @Shader =====================================================================================

Shader r_gl_create_shader(const i8 *vert_src, const i8 *frag_src)
{
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, &vert_src, NULL);
  glCompileShader(vert);

  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, &frag_src, NULL);
  glCompileShader(frag);

  #ifdef DEBUG
  verify_shader(vert, GL_COMPILE_STATUS);
  verify_shader(frag, GL_COMPILE_STATUS);
  #endif

  u32 id = glCreateProgram();
  glAttachShader(id, frag);
  glAttachShader(id, vert);
  glLinkProgram(id);

  #ifdef DEBUG
  verify_shader(id, GL_LINK_STATUS);
  #endif

  glDeleteShader(vert);
  glDeleteShader(frag);

  return (Shader) {id};
}

inline
void r_gl_bind_shader(Shader *shader)
{
  GL_ASSERT(glUseProgram(shader->id));
}

inline
void r_gl_unbind_shader(void)
{
  glUseProgram(0);
}

i32 r_gl_set_uniform_1u(Shader *shader, i8 *name, u32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1ui(loc, val);
  
  return loc;
}

i32 r_gl_set_uniform_1(Shader *shader, i8 *name, i32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1i(loc, val);
  return loc;
}

i32 r_gl_set_uniform_1f(Shader *shader, i8 *name, f32 val)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform1f(loc, val);
  
  return loc;
}

i32 r_gl_set_uniform_2f(Shader *shader, i8 *name, Vec2F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform2f(loc, vec.x, vec.y);

  return loc;
}

i32 r_gl_set_uniform_3f(Shader *shader, i8 *name, Vec3F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform3f(loc, vec.x, vec.y, vec.z);
  
  return loc;
}

i32 r_gl_set_uniform_4f(Shader *shader, i8 *name, Vec4F vec)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
  
  return loc;
}

i32 r_gl_set_uniform_4x4f(Shader *shader, i8 *name, Mat4x4F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix4fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

i32 r_gl_set_uniform_3x3f(Shader *shader, i8 *name, Mat3x3F mat)
{
  i32 loc = glGetUniformLocation(shader->id, name);
  glUniformMatrix3fv(loc, 1, FALSE, &mat.elements[0][0]);
  
  return loc;
}

static
void verify_shader(u32 id, u32 type)
{
  i32 success;

  if (type == GL_LINK_STATUS)
  {
    glValidateProgram(id);
  }

  glGetShaderiv(id, type, &success);

  if (!success)
  {
    i32 length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    i8 log[length];
    glGetShaderInfoLog(id, length, &length, log);

    if (type == GL_COMPILE_STATUS)
    {
      printf("[Error]: Failed to compile shader!\n");
    }
    else
    {
      printf("[Error]: Failed to link shaders!\n");
    }

    printf("%s", log);
  }
}

// @Texture2D ==================================================================================

Texture2D r_gl_load_texture2d(const i8 *path)
{
  Texture2D tex;
  glGenTextures(1, &tex.id);
  tex.data = stbi_load(path, &tex.width, &tex.height, &tex.num_channels, 0);

  glTexImage2D(
               GL_TEXTURE_2D, 
               0, 
               GL_RGB, 
               tex.width, 
               tex.height, 
               0, 
               GL_RGB, 
               GL_UNSIGNED_BYTE, 
               tex.data);

  glGenerateMipmap(GL_TEXTURE_2D);

  return tex;
}

inline
void r_gl_bind_texture2d(Texture2D *texture)
{
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

inline
void r_gl_unbind_texture2d(void)
{
  glBindTexture(GL_TEXTURE_2D, 0);
}

// @Draw =======================================================================================

inline
void r_gl_clear_screen(Vec4F color)
{
  glClearColor(color.r, color.g, color.b, color.a);
  glClear(GL_COLOR_BUFFER_BIT);
}

inline
void r_gl_draw_triangles(u32 vertex_count)
{
  GL_ASSERT(glDrawElements(GL_TRIANGLES, vertex_count, GL_UNSIGNED_INT, NULL));
}