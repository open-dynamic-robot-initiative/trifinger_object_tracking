/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <yaml-cpp/node/node.h>
#include <yaml-cpp/yaml.h>
#include <armadillo>
#include <string>
#include <trifinger_object_tracking/cube_model.hpp>

namespace trifinger_object_tracking
{
struct GmmParam
{
    std::string color;
    arma::rowvec hefts;  // weights
    arma::mat means;
    arma::cube fcovs;
};

// TODO unused?
arma::gmm_full update_model(arma::gmm_full model, const std::string &color);

std::array<arma::gmm_full, FaceColor::N_COLORS> update_model_from_file(
    const std::string &filename);

arma::gmm_full update_for_color(FaceColor color,
                                const YAML::Node &doc,
                                const std::string &color_name);

}  // namespace trifinger_object_tracking
