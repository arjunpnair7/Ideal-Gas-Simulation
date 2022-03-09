//
// Created by Arjun Nair on 3/8/22.
//

#ifndef IDEAL_GAS_PARTICLE_H
#define IDEAL_GAS_PARTICLE_H
#include "cinder/gl/gl.h"

class particle {
public:
    particle(glm::vec2 position, glm::vec2 velocity, size_t radius);
    glm::vec2 getCurrentPosition() const;
    glm::vec2 getCurrentVelocity() const;
    size_t getRadius() const;
    void updatePosition();
    void negateXVelocity();
    void negateYVelocity();
    void setVelocity(glm::vec2 new_velocity);
    bool getCollisionStatus() const;
    void setCollisionStatus(bool collision_status);

private:
    glm::vec2 current_position;
    glm::vec2 current_velocity;
    size_t radius;
    bool has_collided;
};

#endif //IDEAL_GAS_PARTICLE_H
