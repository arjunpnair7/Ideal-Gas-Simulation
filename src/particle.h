//
// Created by Arjun Nair on 3/8/22.
//

#ifndef IDEAL_GAS_PARTICLE_H
#define IDEAL_GAS_PARTICLE_H
#include "cinder/gl/gl.h"

/**
 A class that is used to model a gas particle
 */
class particle {
public:
    particle(glm::vec2 position, glm::vec2 velocity, size_t radius);
    glm::vec2 getCurrentPosition() const;
    glm::vec2 getCurrentVelocity() const;
    size_t getRadius() const;
    bool getCollisionStatus() const;
    void setCollisionStatus(bool collision_status);
    void setVelocity(glm::vec2 new_velocity);
    /**
     * Updates the current position vector by adding the velocity vector to position vector
     */
    void updatePosition();
    /**
     * Multiplies the x velocity by negative 1
     */
    void negateXVelocity();
    /**
     * Multiplies the y velocity by negative 1
     */
    void negateYVelocity();

private:
    glm::vec2 current_position;
    glm::vec2 current_velocity;
    size_t radius;
    //If the particle has collided this frame cycle, has_collided will be true
    bool has_collided = false;
};

#endif //IDEAL_GAS_PARTICLE_H
