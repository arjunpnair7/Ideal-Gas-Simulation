//
// Created by Arjun Nair on 3/8/22.
//

#include "particle.h"

particle::particle(glm::vec2 position, glm::vec2 velocity, size_t radius) {
    current_position = position;
    current_velocity = velocity;
    this->radius = radius;
}

glm::vec2 particle::getCurrentPosition() {
    return current_position;
}

glm::vec2 particle::getCurrentVelocity() {
    return current_velocity;
}

size_t particle::getRadius() {
    return radius;
}

void particle::updatePosition() {
    current_position += current_velocity;
    //std::cout << "updatePosition()" << std::endl;
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
