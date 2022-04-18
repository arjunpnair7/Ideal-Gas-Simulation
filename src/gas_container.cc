#include "gas_container.h"
#include "particle.h"
#include "cinder/gl/gl.h"
#include <map>
#include <cmath>
#include "histogram.cpp"

namespace idealgas {
using glm::vec2;
using std::vector;
using namespace SimulationConstants;

//Seperated into lines based on feedback from part 1
GasContainer::GasContainer(vector<particle> container_particles,
                           vec2 container_start_position, vec2 container_dimensions) {
    if (container_start_position.x < 0 || container_start_position.y < 0) {
        throw std::invalid_argument("Negative start position");
    } else if (container_dimensions.x < 0 || container_dimensions.y < 0) {
        throw std::invalid_argument("Negative container dimensions");
    }
    for (size_t i = 0; i < container_particles.size(); i++) {
        if (!(container_particles[i].getCurrentPosition().x < container_start_position.x) && !(container_particles[i].getCurrentPosition().y < container_start_position.y)) {
            containers_particles.push_back(container_particles[i]);
        }
    }
    this->container_start_position = container_start_position;
    this->container_dimensions = container_dimensions;

     right_wall = container_dimensions.x;
     left_wall = container_start_position.x;
     up_wall = container_start_position.y;
     down_wall = container_dimensions.y;

    //histogram demo(glm::vec2(700, 100), mass1Particles, "Mass = " + std::to_string(particle_type_1_mass));
    //demo.drawHistogram();
//    this->mass1_graph = graph1;
//    this->mass2_graph = graph2;
//    this->mass3_graph = graph3;
}

size_t GasContainer::getCurrentAmountOfParticles() {
    return containers_particles.size();
}

vector<particle> GasContainer::getParticleList() {
    return containers_particles;
}

GasContainer::GasContainer() {}

void GasContainer::Display() const {
    for (size_t i = 0; i < containers_particles.size(); i++) {
//        if (i % 5 == 0) {
//            ci::gl::color(ci::Color(particle_color_1));
//        } else if (i % 5 == 1) {
//            ci::gl::color(ci::Color(particle_color_2));
//        } else if (i % 5 == 2){
//            ci::gl::color(ci::Color(particle_color_3));
//        } else if (i % 5 == 3) {
//            ci::gl::color(ci::Color(particle_color_4));
//        } else if (i % 5 == 4) {
//            ci::gl::color(ci::Color(particle_color_5));
//        }
        particle current = containers_particles[i];
        if (current.getMass() <= 5) {
            ci::gl::color(ci::Color(particle_color_1)); //lightest: red
        } else if (current.getMass() <= 20) {
            ci::gl::color(ci::Color(particle_color_2));//middle: blue
        } else if (current.getMass() <= 40) {
            ci::gl::color(ci::Color(particle_color_3));//heaviest: green
        }
        ci::gl::drawSolidCircle(vec2(current.getCurrentPosition().x, current.getCurrentPosition().y), current.getRadius());
    }
    ci::gl::color(ci::Color(container_color));
    ci::gl::drawStrokedRect(ci::Rectf(container_start_position, container_dimensions ));
    std::vector<particle> testData;
    for (size_t i = 0; i < containers_particles.size(); i++) {
        testData.push_back(containers_particles[i]);
    }

    vector<particle> mass1Particles;
    vector<particle> mass2Particles;
    vector<particle> mass3Particles;

    for (particle current: containers_particles) {
        if (current.getMass() == particle_type_1_mass) {
            mass1Particles.push_back(current);
        } else if (current.getMass() == particle_type_2_mass) {
            mass2Particles.push_back(current);
        } else {
            mass3Particles.push_back(current);
        }
    }

    histogram graph1(glm::vec2(700, 100), mass1Particles, "Mass = " + std::to_string(particle_type_1_mass));
    histogram graph2(glm::vec2(700, 350), mass2Particles, "Mass = " + std::to_string(particle_type_2_mass));
    histogram graph3(glm::vec2(700, 600), mass3Particles, "Mass = " + std::to_string(particle_type_3_mass));
    graph1.drawHistogram();
    graph2.drawHistogram();
    graph3.drawHistogram();
}

void GasContainer::AdvanceOneFrame() {
    for (particle& current: containers_particles) {
        current.updatePosition(up_wall, down_wall);
        current.setCollisionStatus(false);
        checkForWallCollision(current);
        //current.UpdateGravityForce(up_wall);
        for (size_t i = 0; i < containers_particles.size(); i++) {
            for (size_t j = 0; j < containers_particles.size(); j++) {
                if (j != i) {
                    checkForCollision(containers_particles[i], containers_particles[j]);
                }
            }
        }
    }
}

void GasContainer::checkForWallCollision(particle& current) {
//    size_t right_wall = container_dimensions.x;
//    size_t left_wall = container_start_position.x;
//    size_t up_wall = container_start_position.y;
//    size_t down_wall = container_dimensions.y;

    //If out of bounds on the right wall and particle was heading towards the right wall
    if ((current.getCurrentPosition().x + current.getRadius() >= right_wall && current.getCurrentVelocity().x > 0)
        || (current.getCurrentPosition().x - current.getRadius() <= left_wall && current.getCurrentVelocity().x < 0)) {
        current.negateXVelocity();
        return;
    }
    if ((current.getCurrentPosition().y + current.getRadius() >= down_wall && current.getCurrentVelocity().y > 0)
          || (current.getCurrentPosition().y - current.getRadius() <= up_wall && current.getCurrentVelocity().y < 0)) {
        current.negateYVelocity();
        return;
    }
}

void GasContainer::checkForCollision(particle& particle1, particle& particle2) {
    vec2 p1_p2_velocity_diff = particle1.getCurrentVelocity() - particle2.getCurrentVelocity();
    vec2 p1_p2_position_diff = particle1.getCurrentPosition() - particle2.getCurrentPosition();

    //The last component of the and statement makes sure both particles collide only if they are heading towards each other
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
    float p1_mass_adjustment = (2 * particle_2.getMass())/(particle_1.getMass() + particle_2.getMass());
    vec2 particle1_new_velocity = particle_1.getCurrentVelocity() - p1_mass_adjustment *
                                                                    (glm::dot(p1_p2_velocity_diff, p1_p2_position_diff)/p1_p2_length) * p1_p2_position_diff;

    //Calculate the new velocity of particle 2
    vec2 p2_p1_velocity_diff = particle_2.getCurrentVelocity() - particle_1.getCurrentVelocity();
    vec2 p2_p1_position_diff = particle_2.getCurrentPosition() - particle_1.getCurrentPosition();
    float p2_p1_length = pow(glm::length(p2_p1_position_diff), 2);
    float p2_mass_adjustment = (2 * particle_1.getMass())/(particle_1.getMass() + particle_2.getMass());
    vec2 particle2_new_velocity = particle_2.getCurrentVelocity() - p2_mass_adjustment *
                                                                    ((glm::dot(p2_p1_velocity_diff, p2_p1_position_diff)/p2_p1_length)) * p2_p1_position_diff;

    new_velocity_vector.push_back(particle1_new_velocity);
    new_velocity_vector.push_back(particle2_new_velocity);
    return new_velocity_vector;
    }

void GasContainer::increaseSpeed(float speed_change) {
    for (particle& current: containers_particles) {
        current.increase_speed(speed_change);
    }
}

void GasContainer::decreaseSpeed(float speed_change) {
    for (particle& current: containers_particles) {
        current.decrease_speed(speed_change);
    }
}
}


