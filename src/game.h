#pragma once

#include "base/base_inc.h"

#include "entity.h"

// @Event ///////////////////////////////////////////////////////////////////////////

typedef enum EventType
{
  EventType_EntityKilled,
} EventType;

typedef struct EventDesc EventDesc;
struct EventDesc
{
  u64 id;
  u64 type;
  b64 props;
};

typedef struct Event Event;
struct Event
{
  Event *next;
  EventType type;
  EventDesc desc;
};

typedef struct EventQueue EventQueue;
struct EventQueue
{
  Event *front;
  Event *back;
  u64 count;
};

void push_event(Game *game, EventType type, EventDesc desc);
void pop_event(Game *game);
Event *peek_event(Game *game);

// @Game ////////////////////////////////////////////////////////////////////////////

typedef struct Game Game;
struct Game
{
  Arena perm_arena;
  Arena frame_arena;
  Arena batch_arena;
  Arena entity_arena;

  EventQueue event_queue;
  EntityList entities;
  Entity *entity_draw_list;

  f64 t;
  f64 dt;
  Mat3x3F camera;
  bool is_sim_started;
  bool should_quit;
};

void init_game(Game *game);
void update_game(Game *game);
void handle_game_events(Game *game);
void draw_game(Game *game, Game *prev);
bool game_should_quit(Game *game);
void copy_game_state(Game *game, Game *prev);

Vec2F screen_to_world(Vec2F pos);
