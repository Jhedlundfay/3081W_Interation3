/**
 * @file controller.cc
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 */

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include <nanogui/nanogui.h>
#include <string>

#include "src/arena_params.h"
#include "src/common.h"
#include "src/controller.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

Controller::Controller() : last_dt(0) {
  // Initialize default properties for various arena entities
  arena_params aparams;
  aparams.n_lights = N_LIGHTS;
  aparams.x_dim = ARENA_X_DIM;
  aparams.y_dim = ARENA_Y_DIM;

  arena_ = new Arena(&aparams);

  // Start up the graphics (which creates the arena).
  // Run() will enter the nanogui::mainloop().
  viewer_ = new GraphicsArenaViewer(&aparams, arena_, this);
}

void Controller::Run() { viewer_->Run(); }

void Controller::AdvanceTime(double dt) {
  /** first, before anything, check if the arena needs to be renewed because
   * change in one of the slider values
   */
  if ((last_dt + dt) <= .05) {
    last_dt += dt;
    return;
  }
  last_dt = 0;
  arena_->AdvanceTime(dt);
}

void Controller::ChangeArena() {
  arena_params new_params;
  new_params.n_lights = viewer_->get_n_lights();
  new_params.n_foods = viewer_->get_n_foods();
  new_params.n_robots = viewer_->get_n_robots();
  new_params.x_dim = ARENA_X_DIM;
  new_params.y_dim = ARENA_Y_DIM;

  if (arena_->get_params() != new_params) {
    delete(arena_);
    arena_ = new Arena(&new_params);
    viewer_->set_arena(arena_);
  }
}

void Controller::SetArenaFERatio(float value) {
  arena_->set_f_e_ratio(value);
}

void Controller::UpdateLightIntensity(float value) {
  for (auto &ent : arena_->get_entities()) {
    if (ent->get_type() == kLight) {
                static_cast<Light*>
                  (ent)->set_intensity(static_cast<int>(value*1200));
    }
  }
}

void Controller::AcceptCommunication(Communication com) {
  arena_->AcceptCommand(ConvertComm(com));
}

/** Converts communication from one source to appropriate communication to
 * the other source. For example, the viewer sends a kKeyUp communication,
 * and this translate to a kIncreaseSpeed communication to Arena.
 */
/**
  * @TODO: Complete the conversion code for all key presses.
  */
Communication Controller::ConvertComm(Communication com) {
  switch (com) {
  case (kPlay) : return kPlay;
  case (kPause) : return kPause;
  case (kNewGame) : return kReset;
  case (kYesFood) : return kYesFood;
  case (kNoFood) : return kNoFood;
  default: return kNone;
  }
}

NAMESPACE_END(csci3081);
