#include <catch2/catch.hpp>
#include <gas_container.h>
#include "../src/particle.h"

using idealgas::GasContainer;

TEST_CASE("Constructing a particle") {
    SECTION("Create a basic particle") {
    particle test_particle(vec2(100,100), vec2(1,1), 1);
        REQUIRE(test_particle.getCurrentPosition() == vec2(100,100));
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1,1));
        REQUIRE(test_particle.getRadius() == 1);
    }
}

TEST_CASE("Testing getter methods") {
    particle test_particle(vec2(100,100), vec2(1,1), 1);
    SECTION("Test getCurrentPosition") {
        REQUIRE(test_particle.getCurrentPosition() == vec2(100,100));
    }
    SECTION("Test getCurrentVelocity") {
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1,1));
    }
    SECTION("Test getRadius") {
        REQUIRE(test_particle.getRadius() == 1);
    }
    SECTION("Test getCollisionStatus") {
        REQUIRE(test_particle.getCollisionStatus() == false);
    }
}

TEST_CASE("Testing mutator methods") {
    particle test_particle(vec2(100,100), vec2(1,1), 1);
    SECTION("Test setCollisionStatus1") {
        test_particle.setCollisionStatus(true);
        REQUIRE(test_particle.getCollisionStatus() == true);
    }
    SECTION("Test setCollisionStatus2") {
        test_particle.setCollisionStatus(false);
        REQUIRE(test_particle.getCollisionStatus() == false);
    }
    SECTION("Test setVelocity") {
        test_particle.setVelocity(vec2(10,10));
        REQUIRE(test_particle.getCurrentVelocity() == vec2(10, 10));
    }
    SECTION("Test negateXVelocity") {
        test_particle.negateXVelocity();
        REQUIRE(test_particle.getCurrentVelocity() == vec2(-1, 1));
    }
    SECTION("Test negateYVelocity") {
        test_particle.negateYVelocity();
        REQUIRE(test_particle.getCurrentVelocity() == vec2(1, -1));
    }
    SECTION("Test updatePosition") {
        test_particle.updatePosition();
        REQUIRE(test_particle.getCurrentPosition() == vec2(101, 101));
    }
}
