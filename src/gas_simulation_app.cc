#include "gas_simulation_app.h"
#include "particle.h"

using std::vector;

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
    particle test1(vec2(10, 50), vec2(10, 10), 5);
    particle test2(vec2(55, 1), vec2(5, 10), 5);
    particle test3(vec2(79, 13), vec2(0, -10), 5);
    std::vector<particle> testData;
    testData.push_back(test1);
    testData.push_back(test2);
    testData.push_back(test3);

    container_ = GasContainer(testData, vec2(0,0), vec2(1000,1000));
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  container_.Display();
}

void IdealGasApp::update() {
  container_.AdvanceOneFrame();
}

}  // namespace idealgas
