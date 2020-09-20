/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <armadillo>
#include <string>
#include <yaml-cpp/yaml.h>
#include <yaml-cpp/node/node.h>
#include <trifinger_object_tracking/cube_model.hpp>

namespace trifinger_object_tracking
{


    struct GmmParam
    {
        std::string color;
        arma::rowvec hefts; //weights
        arma::mat means;
        arma::cube fcovs;
    };

    arma::gmm_full update_model(arma::gmm_full model, std::string color);

    std::array<arma::gmm_full, FaceColor::N_COLORS> update_model_from_file();

    YAML::Node read_file(std::string path);

    arma::gmm_full update_for_color(FaceColor color, YAML::Node doc, std::string color_name);

}  // namespace trifinger_object_tracking
