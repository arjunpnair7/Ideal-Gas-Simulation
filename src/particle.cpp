//
// Created by Arjun Nair on 3/8/22.
//

#include "particle.h"
#include "gas_simulation_constants.h"

particle::particle(glm::vec2 position, glm::vec2 velocity, size_t radius) {
    current_position = position;
    current_velocity = velocity;
    this->radius = radius;
}

glm::vec2 particle::getCurrentPosition() const {
    return current_position;
}

glm::vec2 particle::getCurrentVelocity() const {
    return current_velocity;
}

size_t particle::getRadius()  const{
    return radius;
}

void particle::updatePosition() {
    current_position += current_velocity;
}

void particle::negateXVelocity() {
    current_velocity.x *= -1;
}

void particle::negateYVelocity() {
    current_velocity.y *= -1;
}

void particle::setVelocity(glm::vec2 new_velocity) {
    current_velocity = new_velocity;
}

bool particle::getCollisionStatus() const {
    return has_collided;
}

void particle::setCollisionStatus(bool collision_status) {
    has_collided = collision_status;
}
