#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "gas_container.h"

namespace idealgas {

/**
 * An app for visualizing the behavior of an ideal gas.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;
//Deleted the comment here based upon last week's suggestion
  const int kWindowSize = 1000;
  const int kMargin = 300;

 private:
  GasContainer container_;
};

}  // namespace idealgas
