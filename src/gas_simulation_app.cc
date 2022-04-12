
#include "gas_simulation_app.h"
#include "particle.h"
#include <random>
#include "gas_simulation_constants_.h"
#include "histogram.h"

using namespace SimulationConstants;
using std::vector;

namespace idealgas {

    IdealGasApp::IdealGasApp() {
        ci::app::setWindowSize(kWindowSize, kWindowSize);
        particle test1(vec2(19.9, 20), vec2(.1, 0), 1, 1.0);
        particle test2(vec2(21.5, 21.4), vec2(-.1, 0), 1, 1.0);
        std::vector<particle> testData;
        for (size_t i = 0; i < number_of_particles_in_simulation; i++) {
            //Code excerpt from:
            //https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
            std::random_device dev;
            std::mt19937 rng(dev());
            std::uniform_int_distribution<std::mt19937::result_type> positions(min_start_position, max_start_position);
            std::uniform_int_distribution<std::mt19937::result_type> velocity(min_velocity,max_velocity);
            std::uniform_int_distribution<std::mt19937::result_type> radius(min_radius,min_radius); //Change this for part 2 of Ideal Gas

            float v1 =  velocity(rng);
            float v2 = velocity(rng);
            float x1 = positions(rng) + min_start_x_position;
            float x2 = positions(rng) + min_start_y_position;
            float particle_radius = radius(rng);
            size_t mass;
            if (i % 3 == 0) {
                mass = particle_type_1_mass;
            } else if (i % 3 == 1) {
                mass = particle_type_2_mass;
            } else {
                mass = particle_type_3_mass;
            }
            particle random_particle(vec2(x1, x2), vec2(v1, v2), particle_radius, mass);
            testData.push_back(random_particle);
        }
        std::cout << "iteration" << std::endl;
        std::vector<particle> testData_;
        particle tester(vec2(19.9, 20), vec2(0, 0), 1, 1.0);
        particle tester2(vec2(19.9, 20), vec2(5, 5), 1, 1.0);

        testData_.push_back(tester);
        testData_.push_back(tester2);
        //histogram testHistogram(glm::vec2(150,150), testData_, "testTitle");
        container_ = GasContainer(testData,
                                  vec2(container_start_x_position,container_start_y_position),
                                  vec2(container_horizontal_dimension,container_vertical_dimension));
    }

    void IdealGasApp::draw() {
        ci::Color background_color("black");
        ci::gl::clear(background_color);
        container_.Display();
    }

    void IdealGasApp::update() {
        container_.AdvanceOneFrame();
    }

    void IdealGasApp::keyDown(ci::app::KeyEvent event) {
        //container_.getParticleList()
        switch (event.getCode()) {
            case ci::app::KeyEvent::KEY_UP:
                container_.increaseSpeed(.5);
                break;

            case ci::app::KeyEvent::KEY_DOWN:
                container_.decreaseSpeed(.5);
                std::cout << "SIMULATION DECREASE SPEED: " << simulation_speed << std::endl;
                break;
        }
    }
}
