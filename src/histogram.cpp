//
// Created by Arjun Nair on 3/21/22.
//
#include "histogram.h"
#include "gas_simulation_constants_.h"

using namespace SimulationConstants;

histogram::histogram() {}

histogram::histogram(glm::vec2 topLeft ,std::vector<particle> frequencyData, std::string graphTitle) {
    this->frequencyData = calculateSpeed(frequencyData);
    this->topLeft = topLeft;
    this->graphTitle = graphTitle;
    drawHistogram();
}

void histogram::drawHistogram() const {

    float bar1Counter = 0;
    float bar2Counter = 0;
    float bar3Counter = 0;

    for (int i = 0; i < this->frequencyData.size(); i++) {
        float current_speed = this->frequencyData[i];
        if (current_speed <= x_axis_cutoff_1) {
            bar1Counter++;
        } else if (current_speed <= x_axis_cutoff_2) {
            bar2Counter++;
        } else if (current_speed <= x_axis_cutoff_3 || current_speed >= x_axis_cutoff_3) {
            bar3Counter++;
        }
    }
    float bar1Height = (bar1Counter/frequencyData.size()) * histogram_height;
    float bar2Height = (bar2Counter/frequencyData.size()) * histogram_height;
    float bar3Height = (bar3Counter/frequencyData.size()) * histogram_height;

    std::cout << bar1Height << std::endl;
    std::cout << bar2Height << std::endl;
    std::cout << bar3Height << std::endl;

    ci::gl::color(ci::Color("blue"));
    ci::gl::drawStrokedRect(ci::Rectf(topLeft, glm::vec2(topLeft.x + histogram_width,topLeft.y + histogram_height) ));
    ci::gl::color(ci::Color("red"));

    ci::gl::drawSolidRect(ci::Rectf(glm::vec2(topLeft.x, topLeft.y + histogram_height), glm::vec2(topLeft.x + bar_width, topLeft.y +
                                                                                                                         histogram_height - bar1Height) ));
    ci::gl::drawSolidRect(ci::Rectf(glm::vec2(topLeft.x + bar_width, topLeft.y + histogram_height), glm::vec2(topLeft.x + 2 * bar_width, topLeft.y +
                                                                                                                                         histogram_height - bar2Height) ));
    ci::gl::drawSolidRect(ci::Rectf(glm::vec2(topLeft.x + 2 * bar_width, topLeft.y + histogram_height), glm::vec2(topLeft.x + 3 * bar_width, topLeft.y +
                                                                                                                                             histogram_height - bar3Height) ));

    ci::gl::color(ci::Color("black"));
    ci::gl::drawStrokedRect(ci::Rectf(glm::vec2(topLeft.x, topLeft.y + histogram_height), glm::vec2(topLeft.x + bar_width, topLeft.y +
                                                                                                                           histogram_height - bar1Height) ));
    ci::gl::drawStrokedRect(ci::Rectf(glm::vec2(topLeft.x + bar_width, topLeft.y + histogram_height), glm::vec2(topLeft.x + 2 * bar_width, topLeft.y +
                                                                                                                                           histogram_height - bar2Height) ));
    ci::gl::drawStrokedRect(ci::Rectf(glm::vec2(topLeft.x + 2 * bar_width, topLeft.y + histogram_height), glm::vec2(topLeft.x + 3 * bar_width, topLeft.y +
                                                                                                                                               histogram_height - bar3Height) ));

    ci::gl::drawString("0-1", glm::vec2(topLeft.x + x_label_padding, topLeft.y + histogram_height + label_y_offset));
    ci::gl::drawString("1-2", glm::vec2(topLeft.x + x_label_padding + bar_width, topLeft.y + histogram_height + label_y_offset));
    ci::gl::drawString("2+", glm::vec2(topLeft.x + x_label_padding + 2 * bar_width, topLeft.y + histogram_height + label_y_offset));

    ci::gl::drawString("1", glm::vec2(topLeft.x - histogram_y_labels_padding, topLeft.y));
    ci::gl::drawString(".5", glm::vec2(topLeft.x - histogram_y_labels_padding, topLeft.y + histogram_y_label_midpoint));
    ci::gl::drawString("0", glm::vec2(topLeft.x - histogram_y_labels_padding, topLeft.y + histogram_height));

    ci::gl::drawString(graphTitle, glm::vec2(topLeft.x + x_label_padding + 2 * bar_width, topLeft.y));

}

std::vector<double> histogram::calculateSpeed(std::vector<particle> data) {
    vector<double> speedVector;
    for (particle current: data) {
        speedVector.push_back(glm::length(current.getCurrentVelocity()));
    }
    return speedVector;
}


