#include "gas_container.h"
#include "particle.h"
#include "cinder/gl/gl.h"
#include <map>
#include <cmath>


namespace idealgas {

using glm::vec2;
using std::vector;


GasContainer::GasContainer(vector<particle> container_particles, vec2 container_start_position, vec2 container_dimensions) {
    this->container_particles = container_particles;
    this->container_start_position = container_start_position;
    this->container_dimensions = container_dimensions;
}

GasContainer::GasContainer() {}

void GasContainer::Display() const {
//  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
//  particle test(vec2(10, 10), vec2(30, 0), 5);
//
//  ci::gl::color(ci::Color("blue"));
//  test.updatePosition();
//    std::cout << test.getCurrentPosition().x << std::endl;
//    ci::gl::drawSolidCircle(vec2(test.getCurrentPosition().x, test.getCurrentPosition().y), test.getRadius());
//
//  ci::gl::color(ci::Color("purple"));
//  ci::gl::drawStrokedRect(ci::Rectf(vec2(0, 0), vec2(100, 100)));
//    //glm::vec2 test(vec2(10, 10), vec2(0, 0));
//
//    //particle test(vec2(10, 10), vec2(0, 0));
    for (particle current: container_particles) {
        ci::gl::color(ci::Color("red"));
        ci::gl::drawSolidCircle(vec2(current.getCurrentPosition().x, current.getCurrentPosition().y), current.getRadius());
    }
    ci::gl::color(ci::Color("purple"));
    ci::gl::drawStrokedRect(ci::Rectf(vec2(0, 0), vec2(300, 300)));

}

void GasContainer::AdvanceOneFrame() {
    size_t right_wall = container_start_position.x + container_dimensions.x;
    size_t left_wall = container_start_position.x;
    size_t up_wall = container_start_position.y;
    size_t down_wall = container_start_position.y + container_dimensions.y;

    for (particle& current: container_particles) {
        current.updatePosition();
        current.setCollisionStatus(false);
        if ((current.getCurrentPosition().x >= right_wall && current.getCurrentVelocity().x > 0)
        || (current.getCurrentPosition().x <= left_wall && current.getCurrentVelocity().x < 0)) {
            current.negateXVelocity();
            return;
        } if ((current.getCurrentPosition().y >= down_wall && current.getCurrentVelocity().y > 0)
        || (current.getCurrentPosition().y <= up_wall && current.getCurrentVelocity().y < 0)) {
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
    vec2 p1_p2_velocity_diff = particle1.getCurrentVelocity() - particle2.getCurrentVelocity(); //(V1 - V2)
    vec2 p1_p2_position_diff = particle1.getCurrentPosition() - particle2.getCurrentPosition();

    if (glm::distance(particle1.getCurrentPosition(), particle2.getCurrentPosition()) < particle2.getRadius() + particle1.getRadius()
        && (!particle1.getCollisionStatus() && !particle2.getCollisionStatus())
        && glm::dot(p1_p2_velocity_diff, p1_p2_position_diff) < 0) {


        vector<vec2> new_velocity_vector = calculateCollisionVelocity(particle1, particle2);
        particle1.setVelocity(new_velocity_vector[0]);
        particle2.setVelocity(new_velocity_vector[1]);

        //std::cout << "out of function value: "  << particle1.getCurrentVelocity() << std::endl;

        particle1.setCollisionStatus(true);
        particle2.setCollisionStatus(true);

    }
}

    vector<vec2> GasContainer::calculateCollisionVelocity(particle particle_1, particle particle_2) {

    vector<vec2> new_velocity_vector;

    vec2 p1_p2_velocity_diff = particle_1.getCurrentVelocity() - particle_2.getCurrentVelocity();
    vec2 p1_p2_position_diff = particle_1.getCurrentPosition() - particle_2.getCurrentPosition();
    float p1_p2_length = pow(glm::length(p1_p2_position_diff), 2);
//    std::cout << "dot product: " << (glm::dot(p1_p2_velocity_diff, p1_p2_position_diff)) << std::endl;
//    std::cout << "denominator: " << p1_p2_length << std::endl;
    vec2 particle1_new_velocity = particle_1.getCurrentVelocity() - (glm::dot(p1_p2_velocity_diff, p1_p2_position_diff)/p1_p2_length) * p1_p2_position_diff;

    vec2 p2_p1_velocity_diff = particle_2.getCurrentVelocity() - particle_1.getCurrentVelocity();
    vec2 p2_p1_position_diff = particle_2.getCurrentPosition() - particle_1.getCurrentPosition();
    float p2_p1_length = pow(glm::length(p2_p1_position_diff), 2);
    vec2 particle2_new_velocity = particle_2.getCurrentVelocity() - ((glm::dot(p2_p1_velocity_diff, p2_p1_position_diff)/p2_p1_length)) * p2_p1_position_diff;

    new_velocity_vector.push_back(particle1_new_velocity);
    new_velocity_vector.push_back(particle2_new_velocity);
    std::cout << "function value: " << particle1_new_velocity << std::endl;

    return new_velocity_vector;
}


}
