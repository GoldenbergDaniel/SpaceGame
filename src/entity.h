#pragma once

#include "base/base_common.h"
#include "base/base_arena.h"
#include "base/base_math.h"

#include "gfx/draw.h"
#include "physx/physx.h"

#define GRAVITY 48.0f // 0.8 p/f^2

#define PLAYER_SPEED 600.0f // 10 p/f
#define PLAYER_JUMP_FORCE 12.0f // 12.0 p/f
#define PLAYER_ACC 3.0f // 0.05 p/f^2
#define PLAYER_FRIC 8.0f // 0.13 p/f^2

#define TIMER_COMBAT 0
#define TIMER_HEALTH 1
#define TIMER_KILL 2

typedef struct Game Game;
typedef struct Entity Entity;
typedef struct EntityRef EntityRef;
typedef struct EntityList EntityList;
typedef struct Timer Timer;

typedef enum EntityType
{
  EntityType_Nil,
  EntityType_General,
  EntityType_Player,
  EntityType_EnemyShip,
  EntityType_Equipped,
  EntityType_Laser,
  EntityType_Wall,
  EntityType_DebugLine,
} EntityType;

typedef enum EntityProp
{
  EntityProp_Controlled = 1 << 0,
  EntityProp_Autonomous = 1 << 1,
  EntityProp_Hostile = 1 << 2,
  EntityProp_Movable = 1 << 3,
  EntityProp_Combatant = 1 << 4,
  EntityProp_Killable = 1 << 5,
  EntityProp_Collides = 1 << 6,
  EntityProp_Rendered = 1 << 7,
  EntityProp_Equipped = 1 << 8,
} EntityProp;

typedef enum EntityFlag
{
  EntityFlag_AbsolutePosition = 1 << 0,
  EntityFlag_AbsoluteRotation = 1 << 1,
  EntityFlag_AbsoluteScale = 1 << 2,
} EntityIgnore;

typedef enum MoveType
{
  MoveType_Walking,
  MoveType_Sliding,
  MoveType_Projectile,
  MoveType_Flying,
  MoveType_Rocket,
} MoveType;

typedef enum CombatType
{
  CombatType_Melee,
  CombatType_Ranged,
} CombatType;

typedef enum DrawType
{
  DrawType_Triangle,
  DrawType_Rectangle,
  DrawType_Sprite,
} DrawType;

struct Timer
{
  f64 max_duration;
  f64 curr_duration;
  bool should_tick;
  bool ticking;
  bool timeout;
  bool should_loop;
  bool start_at_zero;
};

struct EntityRef
{
  Entity *ptr;
  u64 id;
  u64 gen;
};

struct Entity
{
  Entity *next;
  Entity *next_free;

  EntityRef parent;
  Entity *first_child;
  Entity *last_child;
  Entity *next_child;
  u16 child_count;

  // General
  u64 id;
  u64 gen;
  EntityType type;
  MoveType move_type;
  CombatType combat_type;
  b64 props;
  b16 flags;
  bool active;
  bool visible;

  // Transform
  Vec2F pos;
  Vec2I origin;
  f32 rot;
  Vec2F scale;
  Mat3x3F xform;
  Mat3x3F model_mat;
  Mat3x3F scale_mat;
  Vec2F input_dir;

  // Physics
  Vec2F dir;
  Vec2F vel;
  Vec2F new_vel;
  f32 speed;
  bool grounded;
  
  // Drawing
  DrawType draw_type;
  Vec4F color;
  D_TextureID texture;
  Vec2F size;
  bool flip_x;
  bool flip_y;
  u16 z_index;

  // Collision
  Collider2D col;
  u8 col_layer;
  b8 col_mask;

  // Targetting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  u16 view_dist;

  i8 curr_health;

  Timer timers[3];
};

struct EntityList
{
  Entity *head;
  Entity *tail;
  Entity *first_free;
  u16 count;
};

// @InitEntity =================================================================================

void init_entity(Entity *entity, EntityType type);
void clear_entity(Entity *entity);

// @UpdateEntity ===============================================================================

void update_entity_collider(Entity *entity);
void update_entity_xform(Game *game, Entity *entity);

void update_controlled_entity_movement(Game *game, Entity *entity);
void update_autonomous_entity_movement(Game *game, Entity *entity);
void update_entity_projectile_movement(Game *game, Entity *entity);
void update_controlled_entity_combat(Game *game, Entity *entity);
void update_targetting_entity_combat(Game *game, Entity *entity);
void update_equipped_entity(Game *game, Entity *entity);

// @OtherEntity ================================================================================

Vec2F pos_from_entity(Entity *entity);
Vec2F origin_offset_from_entity(Entity *entity);
f32 rot_from_entity(Entity *entity);
Vec2F scale_from_entity(Entity *entity);
Vec2F size_from_entity(Entity *entity);
bool flip_x_from_entity(Entity *entity);
bool flip_y_from_entity(Entity *entity);

void set_entity_size(Entity *entity, Vec2F size);
void set_entity_target(Entity *entity, EntityRef target);

void sort_entities_by_z_index(Game *game);
bool is_entity_valid(Entity *entity);
void resolve_entity_collision(Entity *a, Entity *b);
void wrap_entity_at_edges(Entity *entity);
void damage_entity(Entity *entity, u8 damage);

// @EntityRef ==================================================================================

EntityRef ref_from_entity(Entity *entity);
Entity *entity_from_ref(EntityRef ref);

// @EntityList =================================================================================

Entity *create_entity(Game *game);
void destroy_entity(Game *game, Entity *entity);
Entity *get_entity_of_id(Game *game, u64 id);
Entity *get_nearest_entity_of_type(Game *game, Vec2F pos, EntityType type);

// @EntityTree =================================================================================

void set_entity_parent(Entity *entity, Entity *parent);
void add_entity_child(Entity *entity, Entity *child);
void remove_entity_child(Entity *entity, u64 id);
Entity *get_entity_child_at_index(Entity *entity, u8 index);
Entity *get_entity_child_of_id(Entity *entity, u64 id);
Entity *get_entity_child_of_type(Entity *entity, EntityType type);
