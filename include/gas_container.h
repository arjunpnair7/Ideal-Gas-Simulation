#pragma once
#include "cinder/gl/gl.h"
#include "../src/particle.h"
#include "../src/histogram.h"
#include "cinder/app/App.h"
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
  size_t getCurrentAmountOfParticles();
  vector<particle> getParticleList();


  void increaseSpeed(float speed_change);
  void decreaseSpeed(float speed_change);

    /**
   * Displays the container walls and the current positions of the particles.
   */
  void Display() const;
  /**
   * Updates the positions and velocities of all particles (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();
  /**
   * Takes in two particles and determine if they are moving towards each other and are close enough
   * to be considered a collision. If the particles are considered in a collision state, then the new velocities
   * will be set accordingly.
   * @param particle1 The first particle part of the potential collision
   * @param particle2 The second particle part of the potential collision
   */
  void checkForCollision(particle& particle1, particle& particle2);
  /**
   * Calculates the new velocities of both particles and returns them in a vector
   * @param particle_1 The first particle part of the collision
   * @param particle_2 The second particle part of the collision
   * @return A vector containing the new velocities of the particles after the collision
   *        This vector always has two elements, index 0 has new velocity of particle 1 and
   *        index 1 has new velocity of particle 2
   */
  vector<vec2> calculateCollisionVelocity(particle particle_1, particle particle_2) const;
  /**
   * Checks if the passed in particle has collided with any walls. If it has, it's velocity will be adjusted
   * based upon the collision
   * @param current The particle used to see if there has been a collision
   */
  void checkForWallCollision(particle& current);


 private:
    vector<particle> containers_particles;
    glm::vec2 container_start_position; // Where the top-left of the container starts
    glm::vec2 container_dimensions; // The actual dimensions of the rectangular container
    histogram mass1_graph;
    histogram mass2_graph;
    histogram mass3_graph;
    size_t right_wall;
    size_t left_wall;
    size_t up_wall;
    size_t down_wall;
};

}
