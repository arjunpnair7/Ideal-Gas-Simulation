#include "gas_container.h"
#include "particle.h"
#include "cinder/gl/gl.h"
#include <map>
#include <cmath>
#include "histogram.cpp"
#include <random>


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
     total_amount_of_particles = container_particles.size();

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

    histogram graph1(mass1Particles, "Mass = " + std::to_string(particle_type_1_mass), glm::vec2(700, 100));
    histogram graph2(mass2Particles, "Mass = " + std::to_string(particle_type_2_mass), glm::vec2(700, 350));
    histogram graph3(mass3Particles, "Mass = " + std::to_string(particle_type_3_mass), glm::vec2(700, 600));
    histogram graph4("pressure", containers_particles, glm::vec2(400, 700));

}

void GasContainer::AdvanceOneFrame() {
    for (particle& current: containers_particles) {
        current.UpdatePosition(up_wall, down_wall);
        current.setCollisionStatus(false);
        checkForWallCollision(current);
        current.updateGravitationalEnergy(down_wall);
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
    //If out of bounds on the right wall and particle was heading towards the right wall
    if ((current.getCurrentPosition().x + current.getRadius() >= right_wall && current.getCurrentVelocity().x > 0)
        || (current.getCurrentPosition().x - current.getRadius() <= left_wall && current.getCurrentVelocity().x < 0)) {
        current.NegateXVelocity();
        return;
    }
    if ((current.getCurrentPosition().y + current.getRadius() >= down_wall && current.getCurrentVelocity().y > 0)
          || (current.getCurrentPosition().y - current.getRadius() <= up_wall && current.getCurrentVelocity().y < 0)) {
        current.NegateYVelocity();
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
        current.Increase_speed(speed_change);
    }
}

void GasContainer::decreaseSpeed(float speed_change) {
    for (particle& current: containers_particles) {
        current.Decrease_speed(speed_change);
    }
}

    void GasContainer::IncreaseParticlesInContainer() {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> positions(min_start_position, max_start_position);
        std::uniform_int_distribution<std::mt19937::result_type> velocity(min_velocity,max_velocity);
        std::uniform_int_distribution<std::mt19937::result_type> radius(min_radius,min_radius); //Change this for part 2 of Ideal Gas
        std::uniform_int_distribution<std::mt19937::result_type> mass(0,4); //Change this for part 2 of Ideal Gas

        float v1 =  velocity(rng);
        float v2 = velocity(rng);
        float x1 = positions(rng) + min_start_x_position;
        float x2 = positions(rng) + min_start_y_position;
        float particle_radius = radius(rng);
        int i = mass(rng);
        size_t particle_mass;
        if (i % 3 == 0) {
            particle_mass = particle_type_1_mass;
        } else if (i % 3 == 1) {
            particle_mass = particle_type_2_mass;
        } else {
            particle_mass = particle_type_3_mass;
        }
        particle random_particle(vec2(x1, x2), vec2(v1, v2), particle_radius, particle_mass);
        containers_particles.push_back(random_particle);
    }

    void GasContainer::DecreaseParticlesInContainer() {
        if (containers_particles.size() > 1) {
            containers_particles.pop_back();
        }
    }
}


