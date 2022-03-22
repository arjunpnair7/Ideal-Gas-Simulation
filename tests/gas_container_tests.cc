#include <catch2/catch.hpp>
#include <gas_container.h>
#include "../src/particle.h"
#include "../src/gas_simulation_constants.h"
#include <iostream>

using idealgas::GasContainer;
using namespace SimulationConstants;

std::vector<particle> createRandomParticleVector() {
    std::vector<particle> test_particles_vector;
    for (int i = 0; i < number_of_particles_in_test; i++) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> positions(min_start_position, max_start_position);
        std::uniform_int_distribution<std::mt19937::result_type> velocity(min_velocity,max_velocity);
        std::uniform_int_distribution<std::mt19937::result_type> radius(min_radius,min_radius); //Change this for part 2 of Ideal Ga
        float v1 =  velocity(rng);
        float v2 = velocity(rng);
        float x1 = positions(rng) + min_start_x_position;
        float x2 = positions(rng) + min_start_y_position;
        float particle_radius = radius(rng);
        test_particles_vector.push_back(particle(vec2(x1, x2), vec2(v1, v2), particle_radius));
    }
    return test_particles_vector;
}

TEST_CASE("Constructing a gas container") {
    SECTION("Test constructor for negative start position") {
        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(-1,-1);
        vec2 container_dimensions = vec2(1,1);
        REQUIRE_THROWS_AS(GasContainer(test_particles, container_start, container_dimensions), std::invalid_argument);
    }
    SECTION("Test constructor for negative container dimensions") {
        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(1,1);
        vec2 container_dimensions = vec2(-1,-1);
        REQUIRE_THROWS_AS(GasContainer(test_particles, container_start, container_dimensions), std::invalid_argument);
    }
    SECTION("Test constructor guards ignores invalid particles") {
        std::vector<particle> test_particles = createRandomParticleVector();
        test_particles.push_back(particle(vec2(-10, -10), vec2(0,0), 1));
        vec2 container_start = vec2(1,1);
        vec2 container_dimensions = vec2(10,10);
        GasContainer test_container(test_particles, container_start, container_dimensions);
        REQUIRE(test_container.getCurrentAmountOfParticles() == 10);
    }
}

TEST_CASE("Testing wall collisions") {
    SECTION("Testing horizontal right wall collision") {
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(9, 5), vec2(1,0), 1);
        std::vector<particle> test_particles;
        test_particles.push_back(test_particle);
        test_particle.updatePosition();
        GasContainer test_container(test_particles, container_start, container_dimensions);
        test_container.checkForWallCollision(test_particle);

        REQUIRE(test_particle.getCurrentVelocity() == vec2(-1,0));
    }
    /* In this test case, the particle is at a would be collision point with the right wall. However,
     * it has a negative x velocity(meaning it is not moving towards the right wall) so it not considered
     * a collision which means the velocity stays the same */
    SECTION("Testing horizontal right wall collision with negative x velocity") {
        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(10, 5), vec2(-1,0), 1);
        GasContainer test_container(test_particles, container_start, container_dimensions);

        test_container.checkForWallCollision(test_particle);
        REQUIRE(test_particle.getCurrentVelocity() == vec2(-1,0));
    }
    SECTION("Testing vertical upper wall collision") {
        std::vector<particle> test_particles;
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(9, 9), vec2(0,1), 1);
        test_particles.push_back(test_particle);
        test_particle.updatePosition();
        GasContainer test_container(test_particles, container_start, container_dimensions);

        test_container.checkForWallCollision(test_particle);
        REQUIRE(test_particle.getCurrentVelocity() == vec2(0,-1));
    }
    SECTION("Testing vertical upper wall collision with negative y velocity") {
        std::vector<particle> test_particles;
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(9, 10), vec2(0,-1), 1);
        test_particles.push_back(test_particle);
        GasContainer test_container(test_particles, container_start, container_dimensions);
        test_container.checkForWallCollision(test_particle);

        test_container.checkForWallCollision(test_particle);
        REQUIRE(test_particle.getCurrentVelocity() == vec2(0,-1));
    }
}

TEST_CASE("Testing particle to particle collision") {
    SECTION("Testing calculateCollisionVelocity") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1);
        particle test2(vec2(21.4, 21.4), vec2(-.1, 0), 1);

        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(100,100);
        GasContainer test_container(test_particles, container_start, container_dimensions);

        std::vector<vec2> test_list = test_container.calculateCollisionVelocity(test1, test2);

        REQUIRE((int) test_list[0].x == 0);
        bool result1 = test_list[0].y > -.1 - tolerance || test_list[0].y < -.1 + tolerance;
        REQUIRE(result1 == true);

        REQUIRE((int) test_list[1].x == 0);
        bool result2 = test_list[1].y > .1 - tolerance || test_list[0].y < .1 + tolerance;
        REQUIRE(result2 == true);
    }
    /* In this test case, the two particles are at a distance that would normally cause a collision. However,
     * because they are not moving in the same direction, no collision has occured so the resultant velocities
     * are the same as the particles original velocities. */
    SECTION("Testing calculateCollisionVelocity with particles moving in same direction") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1);
        particle test2(vec2(20, 20), vec2(.1, 0), 1);

        std::vector<particle> test_particles;
        test_particles.push_back(test1);
        test_particles.push_back(test2);
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(100,100);
        GasContainer test_container(test_particles, container_start, container_dimensions);

        std::vector<vec2> test_list = test_container.calculateCollisionVelocity(test1, test2);

        REQUIRE(test_container.getParticleList()[0].getCurrentVelocity() == vec2(.1, 0));
        REQUIRE(test_container.getParticleList()[1].getCurrentVelocity() == vec2(.1, 0));
    }
}

TEST_CASE("Advance one frame") {
    SECTION("Testing horizontal wall collision") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1);

        std::vector<particle> test_particles;
        test_particles.push_back(test1);
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(100,100);
        GasContainer test_container(test_particles, container_start, container_dimensions);
        test_container.AdvanceOneFrame();

        REQUIRE(test_container.getParticleList()[0].getCurrentPosition() == vec2(20.1, 20));
    }
}