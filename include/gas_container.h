#pragma once

#include "cinder/gl/gl.h"
#include "../src/particle.h"
#include <map>
using glm::vec2;
using std::vector;

namespace idealgas {

/**
 * The container in which all of the gas particles are contained. This class
 * stores all of the particles and updates them on each frame of the simulation.
 */
class GasContainer {
 public:

  GasContainer(vector<particle> container_particles, vec2 container_start_position, vec2 container_dimensions);
  GasContainer();
  /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;
  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();
  void checkForCollision(particle& particle1, particle& particle2);
  vector<vec2> calculateCollisionVelocity(particle particle_1, particle particle_2) const;
  void checkForWallCollision(particle& current);
  int getCurrentAmountOfParticles();

 private:
    vector<particle> container_particles;
    glm::vec2 container_start_position;
    glm::vec2 container_dimensions;
};

}
