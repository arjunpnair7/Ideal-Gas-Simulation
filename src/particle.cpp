//
// Created by Arjun Nair on 3/8/22.
//

#include "particle.h"
#include <math.h>

particle::particle(glm::vec2 position, glm::vec2 velocity, size_t radius, float mass) {
    //Assign gravity_ to some function of mass
    current_position = position; //5, 20, 40
    current_velocity = velocity; //subtract velocity by the gravity_ variable to simulate gravity
    this->radius = radius;
    this->mass = mass;
    this->gravity_ = .01 * mass;
    //current_velocity.y -= gravity_;
}

float particle::UpdateGravityForce(size_t height, float base_gravity) {//TODO:RENAME THIS
//    size_t height_diff = abs(height - current_position.y);
//    if (height_diff <= 25){
//        std::cout << "LESS THAN 50" << std::endl;
//        std::cout << "GRAVITY: " << gravity_ << std::endl;
//        base_gravity *= 1.01;
//    } else if (height_diff <= 50) {
//        base_gravity *= 1.05;
//        std::cout << "MORE THAN 50" << std::endl;
//        std::cout << "GRAVITY: " << gravity_ << std::endl;
//        //base_gravity *= 1.50;
//    } else if (height_diff <= 100) {
//        base_gravity *= 1.10;
//    }
    return base_gravity;
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

float particle::getMass()  const{
    return mass;
}

void particle::updatePosition(size_t height, size_t lower_wall) {
    float gravity_force = UpdateGravityForce(height, gravity_);
    current_velocity.y += gravity_force;
    current_position += current_velocity;
    std::cout << "CURRENT VELOCITY: " << current_velocity.y << std::endl;
    if (current_position.y >= lower_wall) {
        current_position.y = lower_wall;
        //current_velocity.y *= -1;
    }
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

void particle::increase_speed(float speed_change) {
    current_velocity.x += speed_change;
    current_velocity.y += speed_change;
}

void particle::decrease_speed(float speed_change) {
    current_velocity.x *= speed_change;
    current_velocity.y *= speed_change;
}


