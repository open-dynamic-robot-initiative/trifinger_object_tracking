/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft. All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <armadillo>
#include <string>


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

}  // namespace trifinger_object_tracking
