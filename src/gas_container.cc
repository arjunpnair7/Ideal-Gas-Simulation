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
      if (current.getCurrentPosition().x >= right_wall || current.getCurrentPosition().x <= left_wall) {
          current.negateXVelocity();
      }
      if (current.getCurrentPosition().y >= down_wall || current.getCurrentPosition().y <= up_wall) {
          current.negateYVelocity();
      }
  }
  for (int i = 0; i < container_particles.size(); i++) {
      for (int j = 0; j < container_particles.size(); j++) {

      }
  }
}

void GasContainer::checkForCollision(particle& particle1, particle& particle2) {
    if (glm::distance(particle1.getCurrentPosition(), particle2.getCurrentPosition()) < particle2.getRadius() + particle1.getRadius()) {
        vec2 p1_p2_velocity_diff = particle1.getCurrentVelocity() = particle2.getCurrentVelocity(); //(V1 - V2)
        vec2 p1_p2_position_diff = particle1.getCurrentPosition() = particle2.getCurrentPosition();
        size_t p1_p2_length = pow(glm::length(p1_p2_position_diff), 2);
        float quotient_term = ((glm::dot(p1_p2_velocity_diff, p1_p2_position_diff)/p1_p2_length));

        vec2 particle1_new_velocity = particle1.getCurrentVelocity() - quotient_term * p1_p2_position_diff;

        vec2 p2_p1_velocity_diff = particle2.getCurrentVelocity() = particle1.getCurrentVelocity(); //(V2 - V1)
        vec2 p2_p1_position_diff = particle2.getCurrentPosition() = particle1.getCurrentPosition();
        size_t p2_p1_length = pow(glm::length(p2_p1_position_diff), 2);
        quotient_term = ((glm::dot(p2_p1_velocity_diff, p2_p1_position_diff)/p2_p1_length));

        vec2 particle2_new_velocity = particle2.getCurrentVelocity() - quotient_term * p2_p1_position_diff;

        particle1.setVelocity(particle1_new_velocity);
        particle2.setVelocity(particle2_new_velocity);
    }
}


}  // namespace idealgas
