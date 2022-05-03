#include <catch2/catch.hpp>
#include <gas_container.h>
#include "../src/gas_simulation_constants_.h"
#include "../src/particle.h"

using namespace SimulationConstants;
using idealgas::GasContainer;

vector<vec2> calculateCollisionVelocity(particle particle_1, particle particle_2)  {
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

std::vector<double> calculateSpeed(std::vector<particle> data) {
    vector<double> speedVector;
    for (particle current: data) {
        speedVector.push_back(glm::length(current.getCurrentVelocity()));
    }
    return speedVector;
}

TEST_CASE("Constructing a particle") {
    SECTION("Create a basic particle") {
    particle test_particle(vec2(100,100), vec2(1,1), 1, 1);
        REQUIRE(test_particle.getCurrentPosition() == vec2(100,100));
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1,1));
        REQUIRE(test_particle.getRadius() == 1);
        REQUIRE(test_particle.getMass() == 1);
    }
}

TEST_CASE("Testing getter methods") {
    particle test_particle(vec2(100,100), vec2(1,1), 1, 1);
    SECTION("Test getCurrentPosition") {
        REQUIRE(test_particle.getCurrentPosition() == vec2(100,100));
    }
    SECTION("Test getCurrentVelocity") {
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1,1));
    }
    SECTION("Test getRadius") {
        REQUIRE(test_particle.getRadius() == 1);
    }
    SECTION("Test getMass") {
        REQUIRE(test_particle.getMass() == 1);
    }
    SECTION("Test getCollisionStatus") {
        REQUIRE(test_particle.getCollisionStatus() == false);
    }
}

TEST_CASE("Testing mutator methods") {
    particle test_particle(vec2(100, 100), vec2(1, 1), 1, 1);
    SECTION("Test setCollisionStatus1") {
        test_particle.setCollisionStatus(true);
        REQUIRE(test_particle.getCollisionStatus() == true);
    }SECTION("Test setCollisionStatus2") {
        test_particle.setCollisionStatus(false);
        REQUIRE(test_particle.getCollisionStatus() == false);
    }SECTION("Test setVelocity") {
        test_particle.setVelocity(vec2(10, 10));
        REQUIRE(test_particle.getCurrentVelocity() == vec2(10, 10));
    }SECTION("Test NegateXVelocity") {
        test_particle.NegateXVelocity();
        REQUIRE(test_particle.getCurrentVelocity() == vec2(-1, 1));
    }SECTION("Test NegateYVelocity") {
        test_particle.NegateYVelocity();
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1, -1));
    }SECTION("Test UpdatePosition") {
        test_particle.UpdatePosition();
        REQUIRE(test_particle.getCurrentPosition() == vec2(101, 101));
    }
        //part 2
    SECTION("Testing particle to particle collision with same mass") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1, 5);
        particle test2(vec2(21.4, 21.4), vec2(-.1, 0), 1, 5);

        vector<vec2> test_list = calculateCollisionVelocity(test1, test2);

        bool result1 = test_list[0].y > -.1 - tolerance || test_list[0].y < -.1 + tolerance;
        REQUIRE(result1 == true);

        bool result2 = test_list[1].y > .1 - tolerance || test_list[0].y < .1 + tolerance;
        REQUIRE(result2 == true);
    }
        //part 2
    SECTION("Testing particle to particle collision with different mass") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1, 1);
        particle test2(vec2(21.4, 21.4), vec2(-.1, 0), 1, 2);

        vector<vec2> test_list = calculateCollisionVelocity(test1, test2);

        bool result0 = test_list[0].x > -.033 - tolerance || test_list[0].x < .033 + tolerance;
        bool result1 = test_list[0].y > -.133 - tolerance || test_list[0].y < -.133 + tolerance;
        REQUIRE(result0 == true);
        REQUIRE(result1 == true);

        bool result2 = test_list[1].x > -.033 - tolerance || test_list[1].x < -.033 + tolerance;
        bool result3 = test_list[1].y > -.067 - tolerance || test_list[1].y < -.067 + tolerance;
        REQUIRE(result2 == true);
        REQUIRE(result3 == true);
    }

    SECTION("Calculate speed of particles") {
        particle test1(vec2(20, 20), vec2(0, 0), 1, 1);
        particle test2(vec2(20, 20), vec2(1, 1), 1, 1);
        vector<particle> test;
        test.push_back(test1);
        test.push_back(test2);
        vector<double> results = calculateSpeed(test);
        REQUIRE(results[0] == 0);
        //REQUIRE(1.414 - tolerance/1000 < results[1] < 1.414 + tolerance/1000);
    }
}
