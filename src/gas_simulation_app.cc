#include "gas_simulation_app.h"
#include "particle.h"
#include <random>
#include "GasSimulationConstants.h"

using namespace SimulationConstants;
using std::vector;

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
    particle test1(vec2(19.9, 20), vec2(.1, 0), 1);
    particle test2(vec2(21.5, 21.4), vec2(-.1, 0), 1);
    std::vector<particle> testData;
    for (int i = 0; i < number_of_particles_in_simulation; i++) {
        //Code excerpt from:
        //https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> positions(min_start_position, max_start_position);
        std::uniform_int_distribution<std::mt19937::result_type> velocity(min_velocity,max_velocity);
        std::uniform_int_distribution<std::mt19937::result_type> radius(min_radius,max_radius);

        float v1 =  velocity(rng);
        float v2 = velocity(rng);
        float x1 = positions(rng) + min_start_x_position;
        float x2 = positions(rng) + min_start_y_position;
        float particle_radius = radius(rng);
        particle random_particle(vec2(x1, x2), vec2(v1, v2), particle_radius);
        testData.push_back(random_particle);
    }
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
}
