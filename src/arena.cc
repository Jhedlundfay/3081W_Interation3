/**
 * @file arena.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <algorithm>
#include <iostream>

#include "src/arena.h"
#include "src/arena_params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Constructors/Destructor
 ******************************************************************************/
Arena::Arena(const struct arena_params *const params)
    : x_dim_(params->x_dim),
      y_dim_(params->y_dim),
      factory_(new EntityFactory),
      params_(),
      sensors_(),
      robots_(),
      entities_(),
      mobile_entities_(),
      game_status_(),
      f_e_ratio_() {
  set_params(*params);
  AddRobots(kRobot, params->n_robots);
  AddEntity(kFood, params->n_foods);
  AddEntity(kLight, params->n_lights);
  set_game_status(PLAYING);
}

Arena::~Arena() {
  for (auto &ent : entities_) {
    delete ent;
  } /* for(ent..) */
}

/*******************************************************************************
 * Member Functions
 ******************************************************************************/
void Arena::AddRobots(EntityType type, int quantity) {
  for (int i = 0; i < quantity; i++) {
    entities_.push_back(factory_->CreateEntity(type));
  }
  for (auto &ent : entities_) {
    if (ent->get_type() == kRobot) {
      robots_.push_back(static_cast<Robot*> (ent));
      mobile_entities_.push_back(static_cast<Robot*> (ent));
    }
  }
}

void Arena::AddEntity(EntityType type, int quantity) {
  for (int i = 0; i < quantity; i++) {
    entities_.push_back(factory_->CreateEntity(type));
  }
  for (auto &ent : entities_) {
    if (ent->get_type() == kLight) {
      mobile_entities_.push_back(static_cast<Light*> (ent));
    }
  }
}

void Arena::Reset() {
  for (auto ent : entities_) {
    ent->Reset();
  }
  set_game_status(PLAYING);
  /* for(ent..) */
} /* reset() */

// The primary driver of simulation movement. Called from the Controller
// but originated from the graphics viewer.
void Arena::AdvanceTime(double dt) {
  if (!(dt > 0)) {
    return;
  }
  for (size_t i = 0; i < 1; ++i) {
    UpdateEntitiesTimestep();
  } /* for(i..) */
} /* AdvanceTime() */

void Arena::UpdateEntitiesTimestep() {
  /*
   * First, update the position of all entities, according to their current
   * velocities.
   *
   */
  for (auto &robot : robots_) {
    if (robot->get_id() > static_cast<int>(robots_.size()*get_f_e_ratio())) {
      robot->set_l_behavior(EXPLORATION);
    } else {
      robot->set_l_behavior(FEAR);
    }
  }

  for (auto &ent : entities_) {
    if (ent->get_type() == kRobot && static_cast<Robot*> (ent)->is_starved()) {
      set_game_status(LOST);
    }
  }

  for (auto ent : entities_) {
    ent->TimestepUpdate(1);
  }

  for (auto &robot : robots_) {
    for (auto &sensor : robot->get_sensors()) {
      sensor->ReceiveInfo(entities_);
    }
  }

  for (auto &ent1 : mobile_entities_) {
    EntityType wall = GetCollisionWall(ent1);
    if (ent1->get_type() == kLight) {
      if (kUndefined != wall) {
        AdjustWallOverlap(ent1, wall);
        static_cast<Light*> (ent1)->HandleCollision(wall);
      }
      /* Determine if that mobile entity is colliding with any other entity.
       * Adjust the position accordingly so they don't overlap.
       */
      for (auto &ent2 : entities_) {
        if (ent2 == ent1) { continue; }
        if (IsColliding(ent1, ent2)) {
          if (ent2->get_type() == kRobot) { continue; }
          if (ent2->get_type() == kFood) { continue; }
          AdjustEntityOverlap(ent1, ent2);
          static_cast<Light*> (ent1)->
            HandleCollision(ent2->get_type(), ent2);
        }
      }
    } else {
      if (kUndefined != wall) {
        AdjustWallOverlap(ent1, wall);
        static_cast<Robot*> (ent1)->HandleCollision(wall);
      }
      for (auto &ent2 : entities_) {
        if (ent2 == ent1) { continue; }
        if (ent2->get_type() == kLight) { continue; }
        if (IsColliding(ent1, ent2)) {
          if (ent2->get_type() == kFood) {
            static_cast<Robot*> (ent1)->
              HandleCollision(ent2->get_type(), ent2);
            continue;
          }
          AdjustEntityOverlap(ent1, ent2);
          static_cast<Robot*> (ent1)->
            HandleCollision(ent2->get_type(), ent2);
        }
      }
    }
  }
}  // UpdateEntitiesTimestep()


// Determine if the entity is colliding with a wall.
// Always returns an entity type. If not collision, returns kUndefined.
EntityType Arena::GetCollisionWall(ArenaMobileEntity *const ent) {
  if (ent->get_pose().x + ent->get_radius() >= x_dim_) {
    return kRightWall;  // at x = x_dim_
  } else if (ent->get_pose().x - ent->get_radius() <= 0) {
    return kLeftWall;  // at x = 0
  } else if (ent->get_pose().y + ent->get_radius() >= y_dim_) {
    return kBottomWall;  // at y = y_dim
  } else if (ent->get_pose().y - ent->get_radius() <= 0) {
    return kTopWall;  // at y = 0
  } else {
    return kUndefined;
  }
} /* GetCollisionWall() */

/* The entity type indicates which wall the entity is colliding with.
* This determines which way to move the entity to set it slightly off the wall. */
void Arena::AdjustWallOverlap(ArenaMobileEntity *const ent, EntityType object) {
  Pose entity_pos = ent->get_pose();
  switch (object) {
    case (kRightWall):  // at x = x_dim_
    ent->set_position(x_dim_-(ent->get_radius()+5), entity_pos.y);
    break;
    case (kLeftWall):  // at x = 0
    ent->set_position(ent->get_radius()+5, entity_pos.y);
    break;
    case (kTopWall):  // at y = 0
    ent->set_position(entity_pos.x, ent->get_radius()+5);
    break;
    case (kBottomWall):  // at y = y_dim_
    ent->set_position(entity_pos.x, y_dim_-(ent->get_radius()+5));
    break;
    default:
    {}
  }
}

/* Calculates the distance between the center points to determine overlap */
bool Arena::IsColliding(
  ArenaMobileEntity * const mobile_e,
  ArenaEntity * const other_e) {
    double delta_x = other_e->get_pose().x - mobile_e->get_pose().x;
    double delta_y = other_e->get_pose().y - mobile_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    return
    (distance_between <= (mobile_e->get_radius() + other_e->get_radius()));
}

/* This is called when it is known that the two entities overlap.
* We determine by how much they overlap then move the mobile entity to
* the edge of the other
*/
/* @TODO: Add functionality to Pose to determine the distance distance_between two instances (e.g. overload operator -)
*/
/* @BUG: The robot will pass through the home food on occasion. The problem
 * is likely due to the adjustment being in the wrong direction. This could
 * be because the cos/sin generate the wrong sign of the distance_to_move
 * when the collision is in a specific quadrant relative to the center of the
 * colliding entities..
 */
void Arena::AdjustEntityOverlap(ArenaMobileEntity * const mobile_e,
  ArenaEntity *const other_e) {
    double delta_x = mobile_e->get_pose().x - other_e->get_pose().x;
    double delta_y = mobile_e->get_pose().y - other_e->get_pose().y;
    double distance_between = sqrt(delta_x*delta_x + delta_y*delta_y);
    double distance_to_move =
      mobile_e->get_radius() + other_e->get_radius() - distance_between;
    double angle = atan2(delta_y, delta_x);
    mobile_e->set_position(
      mobile_e->get_pose().x+cos(angle)*distance_to_move,
      mobile_e->get_pose().y+sin(angle)*distance_to_move);
}

// Accept communication from the controller. Dispatching as appropriate.
/** @TODO: Call the appropriate Robot functions to implement user input
  * for controlling the robot.
  */
void Arena::AcceptCommand(Communication com) {
  switch (com) {
  case(kPlay): paused_ = false;
    break;
  case(kPause): paused_ = true;
    break;
  case(kReset): Reset();
    break;
  case(kYesFood): for (auto& robot : robots_) {
      robot->food_exists_ = true;
    }
    break;
  case(kNoFood): for (auto& robot : robots_) {
      robot->food_exists_ = false;
    }
    break;
  case(kNone): break;
  default: break;
  }
} /* AcceptCommand */

NAMESPACE_END(csci3081);
