#include <SDL2/SDL.h>

#include "glad/glad.h"

#include <stdlib.h>
#include <time.h>

#include "base_common.h"
#include "base_math.h"
#include "render.h"
#include "draw.h"
#include "util.h"
#include "component.h"
#include "entity.h"
#include "game.h"
#include "main.h"

#define DEBUG
// #define LOG_PERF

static
void set_gl_attributes(void);

i32 main(void)
{
  Game game = {0};
  SDL_GLContext context;

  srand(time(NULL));
  SDL_InitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  set_gl_attributes();

  game.window = SDL_CreateWindow(
                                 "SPACE GAME", 
                                 W_CENTERED, 
                                 W_CENTERED, 
                                 W_WIDTH, 
                                 W_HEIGHT, 
                                 W_FLAGS);

  context = SDL_GL_CreateContext(game.window);
  SDL_GL_MakeCurrent(game.window, context);
  SDL_GL_SetSwapInterval(VSYNC_ON);

  gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress);

  game_init(&game);
  
  f64 elapsed_time = 0.0f;
  f64 current_time = SDL_GetTicks64() * 0.001f;
  f64 time_step = 1.0f / TARGET_FPS;
  f64 accumulator = 0.0f;

  while (game.running)
  {
    #ifdef LOG_PERF
    u64 frame_start = SDL_GetPerformanceCounter();
    #endif

    f64 new_time = SDL_GetTicks64() * 0.001f;
    f64 frame_time = new_time - current_time;

    current_time = new_time;
    accumulator += frame_time;

    while (accumulator >= time_step)
    {
      SDL_Event event;
      while (SDL_PollEvent(&event))
      {
        game_handle_event(&game, &event);
      }

      if (game_should_quit())
      {
        game.running = FALSE;
        break;
      }

      game.t = elapsed_time;
      game.dt = time_step;
      game_update(&game);

      elapsed_time += time_step;
      accumulator -= time_step;
    }

    #ifdef LOG_PERF
    u64 frame_end = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    f64 frame_diff = (f64) (frame_end - frame_start) / frequency * 1000.0f;

    printf("%.2lf ms\n", frame_diff);
    #endif

    game_draw(&game);
    SDL_GL_SwapWindow(game.window);
  }

  SDL_DestroyWindow(game.window);
  SDL_Quit();

  return 0;
}

static
void set_gl_attributes(void)
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}
