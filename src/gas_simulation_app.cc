#include "gas_simulation_app.h"
#include "particle.h"

using std::vector;

namespace idealgas {

IdealGasApp::IdealGasApp() {
  ci::app::setWindowSize(kWindowSize, kWindowSize);
    particle test1(vec2(19.9, 20), vec2(.1, 0), 1);
    particle test2(vec2(21.5, 21.4), vec2(-.1, 0), 1);
//    particle test3(vec2(18, 22), vec2(-1, 4), 10);
//    particle test4(vec2(0, 5), vec2(-1, 1), 5);
//    particle test5(vec2(4, 50), vec2(2, -2), 8);
//    particle test6(vec2(9, 13), vec2(-4, -3), 12);

    std::vector<particle> testData;
    testData.push_back(test1);
    testData.push_back(test2);
//    testData.push_back(test3);
//    testData.push_back(test4);
//    testData.push_back(test5);
//    testData.push_back(test6);

    container_ = GasContainer(testData, vec2(0,0), vec2(300,300));
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
