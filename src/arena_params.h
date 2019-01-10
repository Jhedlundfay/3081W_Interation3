/**
 * @file arena_params.h
 *
 * @copyright 2018 3081 Staff, All rights reserved.
 *
 */

#ifndef SRC_ARENA_PARAMS_H_
#define SRC_ARENA_PARAMS_H_

/*******************************************************************************
 * Includes
 ******************************************************************************/
#include "src/common.h"
#include "src/params.h"

/*******************************************************************************
 * Namespaces
 ******************************************************************************/
NAMESPACE_BEGIN(csci3081);

/*******************************************************************************
 * Structure Definitions
 ******************************************************************************/
/**
* @brief Struct holding parameters for initializing the Arena.
*
* These parameters include the parameters for Arena's geometry as well as the
* parameters for initializing ALL entities within the Arena.
*/
struct arena_params {
  bool operator==(const arena_params other) const {
    return (n_robots == other.n_robots &&
      n_lights == other.n_lights &&
      n_foods == other.n_foods &&
      x_dim == other.x_dim &&
      y_dim == other.y_dim);
  }
  bool operator!=(const arena_params other) const {
    return (n_robots != other.n_robots ||
      n_lights != other.n_lights ||
      n_foods != other.n_foods ||
      x_dim != other.x_dim ||
      y_dim != other.y_dim);
  }

  size_t n_robots{N_ROBOTS};
  size_t n_lights{N_LIGHTS};
  size_t n_foods{N_FOODS};
  uint x_dim{ARENA_X_DIM};
  uint y_dim{ARENA_Y_DIM};
};

NAMESPACE_END(csci3081);

#endif  // SRC_ARENA_PARAMS_H_
