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
    SECTION("Testing horizontal wall collision") {
        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(9, 5), vec2(1,0), 1);
        test_particle.updatePosition();
        GasContainer test_container(test_particles, container_start, container_dimensions);

        test_container.checkForWallCollision(test_particle);
        REQUIRE(test_particle.getCurrentVelocity() == vec2(-1,0));
    }
    SECTION("Testing vertical wall collision") {
        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(10,10);
        particle test_particle(vec2(9, 9), vec2(0,1), 1);
        test_particle.updatePosition();
        GasContainer test_container(test_particles, container_start, container_dimensions);

        test_container.checkForWallCollision(test_particle);
        REQUIRE(test_particle.getCurrentVelocity() == vec2(0,-1));
    }
}

TEST_CASE("Testing particle collision calculations") {
    SECTION("Testing horizontal wall collision") {
        particle test1(vec2(20, 20), vec2(.1, 0), 1);
        particle test2(vec2(21.4, 21.4), vec2(-.1, 0), 1);

        std::vector<particle> test_particles = createRandomParticleVector();
        vec2 container_start = vec2(0,0);
        vec2 container_dimensions = vec2(100,100);
        GasContainer test_container(test_particles, container_start, container_dimensions);

        std::vector<vec2> test_list = test_container.calculateCollisionVelocity(test1, test2);
        std::cout << test_list[0] << std::endl;

        REQUIRE((int) test_list[0].x == 0);
        bool result = test_list[0].y > -.1 - tolerance || test_list[0].y < -.1 + .001;
        REQUIRE(result == true);
    }
}


