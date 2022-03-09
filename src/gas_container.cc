#include "gas_container.h"
#include "particle.h"
#include "cinder/gl/gl.h"
#include <map>
#include <cmath>
#include "GasSimulationConstants.h"

namespace idealgas {
using glm::vec2;
using std::vector;
using namespace SimulationConstants;

GasContainer::GasContainer(vector<particle> container_particles, vec2 container_start_position, vec2 container_dimensions) {
    this->container_particles = container_particles;
    this->container_start_position = container_start_position;
    this->container_dimensions = container_dimensions;
}

GasContainer::GasContainer() {}

void GasContainer::Display() const {
    for (int i = 0; i < container_particles.size(); i++) {
        if (i % 5 == 0) {
            ci::gl::color(ci::Color(particle_color_1));
        } else if (i % 5 == 1) {
            ci::gl::color(ci::Color(particle_color_2));
        } else if (i % 5 == 2){
            ci::gl::color(ci::Color(particle_color_3));
        } else if (i % 5 == 3) {
            ci::gl::color(ci::Color(particle_color_4));
        } else if (i % 5 == 4) {
            ci::gl::color(ci::Color(particle_color_5));
        }
        particle current = container_particles[i];
        ci::gl::drawSolidCircle(vec2(current.getCurrentPosition().x, current.getCurrentPosition().y), current.getRadius() + 2);
    }
    ci::gl::color(ci::Color("purple"));
    ci::gl::drawStrokedRect(ci::Rectf(container_start_position, container_dimensions ));
}

void GasContainer::AdvanceOneFrame() {
    size_t right_wall = container_dimensions.x;
    size_t left_wall = container_start_position.x;
    size_t up_wall = container_start_position.y;
    size_t down_wall = container_dimensions.y;

    for (particle& current: container_particles) {
        current.updatePosition();
        current.setCollisionStatus(false);
        //If out of bounds on the right wall and particle was heading towards the right wall
        if ((current.getCurrentPosition().x + current.getRadius() >= right_wall && current.getCurrentVelocity().x > 0)
            || (current.getCurrentPosition().x - current.getRadius() <= left_wall && current.getCurrentVelocity().x < 0)) {
            current.negateXVelocity();
            return;
        } if ((current.getCurrentPosition().y + current.getRadius() >= down_wall && current.getCurrentVelocity().y > 0)
            || (current.getCurrentPosition().y - current.getRadius() <= up_wall && current.getCurrentVelocity().y < 0)) {
            current.negateYVelocity();
            return;
        }
        for (int i = 0; i < container_particles.size(); i++) {
            for (int j = 0; j < container_particles.size(); j++) {
                if (j != i) {
                    checkForCollision(container_particles[i], container_particles[j]);
                }
            }
        }
    }
}

void GasContainer::checkForCollision(particle& particle1, particle& particle2) {
    vec2 p1_p2_velocity_diff = particle1.getCurrentVelocity() - particle2.getCurrentVelocity();
    vec2 p1_p2_position_diff = particle1.getCurrentPosition() - particle2.getCurrentPosition();

    if (glm::distance(particle1.getCurrentPosition(), particle2.getCurrentPosition()) < particle2.getRadius() + particle1.getRadius()
        && (!particle1.getCollisionStatus() && !particle2.getCollisionStatus())
        && glm::dot(p1_p2_velocity_diff, p1_p2_position_diff) < 0) {

        vector<vec2> new_velocity_vector = calculateCollisionVelocity(particle1, particle2);
        particle1.setVelocity(new_velocity_vector[0]);
        particle2.setVelocity(new_velocity_vector[1]);
        //Set the collision statuses for both particles so that they do not collide again in the same frame
        particle1.setCollisionStatus(true);
        particle2.setCollisionStatus(true);
    }
}

    vector<vec2> GasContainer::calculateCollisionVelocity(particle particle_1, particle particle_2) const {
    vector<vec2> new_velocity_vector;
    //Calculate the new velocity of particle 1
    vec2 p1_p2_velocity_diff = particle_1.getCurrentVelocity() - particle_2.getCurrentVelocity(); //(X1 - X2)
    vec2 p1_p2_position_diff = particle_1.getCurrentPosition() - particle_2.getCurrentPosition(); //(V1 - V2)
    float p1_p2_length = pow(glm::length(p1_p2_position_diff), 2); // ||X1 - X2||^2

    vec2 particle1_new_velocity = particle_1.getCurrentVelocity() - (glm::dot(p1_p2_velocity_diff, p1_p2_position_diff)/p1_p2_length) * p1_p2_position_diff;
    //Calculate the new velocity of particle 2
    vec2 p2_p1_velocity_diff = particle_2.getCurrentVelocity() - particle_1.getCurrentVelocity();
    vec2 p2_p1_position_diff = particle_2.getCurrentPosition() - particle_1.getCurrentPosition();
    float p2_p1_length = pow(glm::length(p2_p1_position_diff), 2);
    vec2 particle2_new_velocity = particle_2.getCurrentVelocity() - ((glm::dot(p2_p1_velocity_diff, p2_p1_position_diff)/p2_p1_length)) * p2_p1_position_diff;

    new_velocity_vector.push_back(particle1_new_velocity);
    new_velocity_vector.push_back(particle2_new_velocity);
    return new_velocity_vector;
}
}
