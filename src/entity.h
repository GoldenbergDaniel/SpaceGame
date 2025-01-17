#pragma once

#include "base/base.h"
#include "physics/physics.h"
#include "draw.h"

#define MAX_ENTITY_CHILDREN 16

#define PLAYER_ACC 3.0f
#define PLAYER_FRIC 8.0f

#define BULLET_KILL_TIME 5.0f
#define PLAYER_INVINCIBILITY_TIMER 0.5f
#define FLASH_TIME 0.075f

#define WALKER_SPEED 50.0f
#define BABY_CHICKEN_GROWTH_DURATION 3.0f
#define BLOAT_POUND_RANGE 200.0f

enum
{
  SPID_Nil,
  SPID_Player,
  SPID_Merchant,
  SPID_Gun,
};

typedef enum ColliderID
{
  Collider_Body,
  Collider_Head,
  Collider_Hit,

  Collider_COUNT,
} ColliderID;

typedef struct EntityRef EntityRef;
struct EntityRef
{
  Entity *ptr;
  u64 id;
};

typedef struct Timer Timer;
struct Timer
{
  bool ticking;
  f32 duration;
  f64 end_time;
};

// Particle /////////////////////////////////////////////////////////////////////////

typedef enum ParticleKind
{
  ParticleKind_Smoke,
  ParticleKind_Blood,
  ParticleKind_Death,
  ParticleKind_PickupCoin,
  ParticleKind_PickupSoul,
  ParticleKind_EggHatch,
  ParticleKind_Dirt,
  ParticleKind_Debug,

  ParticleKind_COUNT,
} ParticleKind;

typedef enum ParticleEmmissionType
{
  ParticleEmmissionType_Burst,
  ParticleEmmissionType_Linear,
} ParticleEmmissionType;

typedef enum ParticleProp
{
  ParticleProp_AffectedByGravity = 1 << 0,
  ParticleProp_CollidesWithGround = 1 << 1,
  ParticleProp_VariateColor = 1 << 2,
  ParticleProp_ScaleOverTime = 1 << 3,
  ParticleProp_SpeedOverTime = 1 << 4,
  ParticleProp_RotateOverTime = 1 << 5,
  ParticleProp_KillAfterTime = 1 << 6,
} ParticleProp;

typedef struct ParticleDesc ParticleDesc;
struct ParticleDesc
{ 
  ParticleEmmissionType emmission_type;
  b8 props;
  u32 count;
  f32 duration;
  f32 spread;
  Vec4F color_primary;
  Vec4F color_secondary;
  Vec2F scale;
  Vec2F scale_delta;
  f32 speed;
  f32 speed_delta;
  f32 rot_delta;
  Vec2F vel;
};

typedef struct Particle Particle;
struct Particle
{
  Vec4F color;
  Vec2F pos;
  Vec2F scale;
  f32 dir;
  f32 rot;
  Vec2F vel;
  f32 speed;
  bool is_grounded;
  bool is_active;
  
  EntityRef owner;
};

// Entity ///////////////////////////////////////////////////////////////////////////

typedef enum EntityGender
{
  EntityGender_Nil,
  EntityGender_Male,
  EntityGender_Female,
  
  EntityGender_COUNT,
} EntityGender;

typedef enum EntityState
{
  EntityState_Nil,
  EntityState_Idle,
  EntityState_Walk,
  EntityState_Jump,
  EntityState_LayEggBegin,
  EntityState_LayEggLaying,
  EntityState_LayEggEnd,
  EntityState_MerchantComing,
  EntityState_MerchantArrived,
  EntityState_MerchantLeaving,
  EntityState_MerchantGone, 
  EntityState_PoundBegin,
  EntityState_PoundEnd,
  EntityState_Dead,

  EntityState_COUNT,
} EntityState;

typedef struct Animation Animation;
struct Animation
{
  u16 frame_idx;
  u16 tick_counter;
};

typedef struct AnimationDesc AnimationDesc;
struct AnimationDesc
{
  Sprite frames[16];
  u16 frame_count;
  u16 ticks_per_frame;
  EntityState exit_state;
};

typedef enum EntityType
{
  EntityType_Nil,
  EntityType_Any,
  EntityType_Debug,
  EntityType_Player,
  EntityType_Zombie,
  EntityType_Equipped,
  EntityType_Ammo,
  EntityType_Egg,
  EntityType_Decoration,
  EntityType_Collider,
  EntityType_Collectable,
  EntityType_Merchant,
  EntityType_Corpse,
  EntityType_Shockwave,
} EntityType;

typedef enum EntityProp
{
  EntityProp_Renders =            bit(0),
  EntityProp_Collides =           bit(1),
  EntityProp_Controlled =         bit(2),
  EntityProp_Moves =              bit(3),
  EntityProp_Killable =           bit(4),
  EntityProp_Equipped =           bit(5),
  EntityProp_WrapsAtEdges =       bit(6),
  EntityProp_AffectedByGravity =  bit(7),
  EntityProp_CollidesWithGround = bit(8),
  EntityProp_BobsOverTime =       bit(9),
  EntityProp_Grounded =           bit(10),
  EntityProp_FlashWhite =         bit(11),
  EntityProp_HideAfterTime =      bit(12),
  EntityProp_LaysEggs =           bit(13),
  EntityProp_Morphs =             bit(14),
  EntityProp_DistortScaleX =      bit(15),
  EntityProp_DistortScaleY =      bit(16),
  EntityProp_KillAfterTime =      bit(17),
  EntityProp_LookAtPlayer =       bit(18),
} EntityProp;

typedef enum AmmoKind
{
  AmmoKind_Nil,
  AmmoKind_Bullet,
  AmmoKind_Pellet,
  AmmoKind_Laser,
} AmmoKind;

typedef enum WeaponKind
{
  WeaponKind_Nil,
  WeaponKind_Revolver,
  WeaponKind_Rifle,
  WeaponKind_Shotgun,
  WeaponKind_SMG,
  WeaponKind_BurstRifle,
  WeaponKind_LaserPistol,

  WeaponKind_COUNT,
} WeaponKind;

typedef struct WeaponDesc WeaponDesc;
struct WeaponDesc
{
  String name;
  Sprite sprite;
  AmmoKind ammo_kind;
  Vec2F ancor;
  Vec2F shot_point;
  f32 shot_cooldown;
  f32 bullet_speed;
  u16 damage;
  u16 ammo;
  f32 reload_duration;
  struct
  {
    Vec2F offset;
    u16 price;
  } merchant;
};

typedef enum CollectableKind
{
  CollectableKind_Nil,
  CollectableKind_Coin,
  CollectableKind_Soul,

  CollectableKind_COUNT,
} CollectableKind;

typedef struct CollectableDesc CollectableDesc;
struct CollectableDesc
{
  Sprite sprite;
  i32 draw_chance;
};

typedef enum MoveType
{
  MoveType_Nil,
  MoveType_Grounded,
  MoveType_Projectile,
  MoveType_Flying,
} MoveType;

typedef enum CombatType
{
  CombatType_Nil,
  CombatType_Melee,
  CombatType_Ranged,
  CombatType_Pound,
} CombatType;

typedef enum DrawType
{
  DrawType_Nil,
  DrawType_Primitive,
  DrawType_Sprite,
} DrawType;

typedef enum ZombieKind
{
  ZombieKind_Nil,
  ZombieKind_Walker,
  ZombieKind_Chicken,
  ZombieKind_BabyChicken,
  ZombieKind_Bloat,

  ZombieKind_COUNT,
} ZombieKind;

typedef struct ZombieDesc ZombieDesc;
struct ZombieDesc
{
  b64 props;
  MoveType move_type;
  CombatType combat_type;
  u16 speed;
  u16 health;
  u16 damage;
  f32 attack_cooldown;
};

typedef struct Entity Entity;
struct Entity
{
  Entity *next;
  Entity *next_free;

  EntityRef parent;
  EntityRef *children;
  i16 child_count;
  i16 *free_child_list;
  i16 free_child_count;

  // General
  u64 id;
  u8 spid;
  EntityType type;
  MoveType move_type;
  CombatType combat_type;
  EntityProp props;
  bool is_active;
  bool marked_for_death;
  bool marked_for_spawn;

  // Transform
  Vec2F pos;
  f32 rot;
  Vec2F scale;
  Mat3x3F xform;
  Mat3x3F model_mat;
  Vec2F input_dir;

  // Physics
  Vec2F vel;
  Vec2F new_vel;
  f32 speed;
  
  // Drawing
  DrawType draw_type;
  Vec4F tint;
  Sprite sprite;
  Vec2F dim;
  bool flip_x;
  bool flip_y;

  // Collision
  Entity *cols[Collider_COUNT];
  ColliderID col_id;
  P_ColliderType col_type;
  f32 radius;
  bool colliding_with_player;

  // Animation
  AnimationDesc *anim_descriptors;
  Animation anim;
  EntityState state;
  EntityState prev_state;
  struct
  {
    Vec2F range;
    i8 state;
  } bobbing;
  struct
  {
    f32 saved;
    f32 scale;
    f32 rate;
    i8 state;
  } distort_x;
  struct
  {
    f32 saved;
    f32 scale;
    f32 rate;
    i8 state;
  } distort_y;
  Timer flash_timer;

  // Targeting
  bool has_target;
  Vec2F target_pos;
  f32 target_angle;
  f32 view_dist;
  f32 stop_dist;

  // Morphing
  struct
  {
    Timer timer;
    EntityType into;
  } morphing;

  // Merchant slot
  struct
  {
    enum
    {
      MerchantSlotKind_Weapon,
      MerchantSlotKind_Coin,
      MerchantSlotKind_Powerup,
    } kind;
    WeaponKind weapon_kind;
    u16 price;
    u16 ammo_count;
    bool purchased;
  } merchant_slot;

  // Particle group
  ParticleDesc particle_desc;
  Timer particle_timer;
  u32 particles_killed;

  // Combat
  bool is_weapon_equipped;
  i16 health;
  i16 damage;
  Timer attack_timer;
  Timer damage_timer;
  Timer kill_timer;
  Timer invincibility_timer;
  Timer muzzle_flash_timer;
  Timer egg_timer;

  // Kinds
  ZombieKind zombie_kind;
  WeaponKind weapon_kind;
  CollectableKind item_kind;
};

typedef struct EntityList EntityList;
struct EntityList
{
  Entity *head;
  Entity *tail;
  Entity *first_free;
  u64 count;
};

Entity *NIL_ENTITY = &(Entity) {0};

Entity *create_entity(EntityType type);
Entity *spawn_entity(EntityType type, Vec2F pos);
Entity *spawn_ammo(AmmoKind kind, Vec2F pos);
Entity *spawn_zombie(ZombieKind kind, Vec2F pos);
Entity *spawn_collectable(CollectableKind kind, Vec2F pos);
Entity *spawn_particles(ParticleKind kind, Vec2F pos);
Entity *spawn_merchant(void);
void kill_entity(Entity *en, bool slain);

bool entity_has_prop(Entity *en, EntityProp prop);
void entity_add_prop(Entity *en, EntityProp prop);
void entity_rem_prop(Entity *en, EntityProp prop);

Vec2F pos_from_entity(Entity *en);
Vec2F pos_tl_from_entity(Entity *en);
Vec2F pos_tr_from_entity(Entity *en);
Vec2F pos_bl_from_entity(Entity *en);
Vec2F pos_br_from_entity(Entity *en);
Vec2F dim_from_entity(Entity *en);
Vec2F scale_from_entity(Entity *en);
f32 rot_from_entity(Entity *en);
bool flip_x_from_entity(Entity *en);
bool flip_y_from_entity(Entity *en);

void entity_look_at(Entity *en, Vec2F target_pos);
void entity_set_target(Entity *en, EntityRef target);
bool entity_is_valid(Entity *en);

void damage_entity(Entity *reciever, i16 damage);

// EntityRef ////////////////////////////////////////////////////////////////////////

EntityRef ref_from_entity(Entity *en);
Entity *entity_from_ref(EntityRef ref);

// EntityList ///////////////////////////////////////////////////////////////////////

Entity *alloc_entity(void);
void free_entity(Entity *en);
Entity *get_entity_by_id(u64 id);
Entity *get_entity_by_sp(u8 sp);

void attach_entity_child(Entity *en, Entity *child);
void attach_entity_child_at(Entity *en, Entity *child, u16 index);
void detach_entity_child(Entity *en, Entity *child);
Entity *get_entity_child_at(Entity *en, u16 index);
Entity *get_entity_child_by_id(Entity *en, u64 id);
Entity *get_entity_child_by_spid(Entity *en, u8 sp);
Entity *get_entity_child_by_type(Entity *en, EntityType type);

void entity_add_collider(Entity *en, ColliderID col_id);

// Timer /////////////////////////////////////////////////////////////////////////////

void timer_start(Timer *timer, f64 duration);
bool timer_timeout(Timer *timert);
f64 timer_remaining(Timer *timer);

// Misc //////////////////////////////////////////////////////////////////////////////

bool has_prop(b64 props, u64 prop);
P_CollisionParams collision_params_from_entity(Entity *en, Vec2F vel);
void equip_weapon(Entity *en, WeaponKind kind);
void entity_distort_x(Entity *en, f32 scale, f32 rate, f32 original);
void entity_distort_x(Entity *en, f32 scale, f32 rate, f32 original);
void entity_set_gender(Entity *en, EntityGender gender);
bool entity_is_laying(Entity *en);
bool slot_purchase_item(Entity *en);
void slot_populate_weapon(Entity *slot);
void slot_populate_ammo(Entity *slot);
void slot_populate_powerup(Entity *slot);
