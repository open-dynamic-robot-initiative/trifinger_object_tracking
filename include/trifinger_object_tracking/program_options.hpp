/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 * @license BSD 3-clause
 */
#pragma once

#include <iostream>

#include <boost/program_options.hpp>

namespace trifinger_object_tracking
{
/**
 * @brief Wrapper around boost::program_options for easier use.
 */
class ProgramOptions
{
public:
    //! Returns a help text that is shown when using `-h/--help`.
    virtual std::string help() const = 0;

    //! Add program-specific options
    virtual void add_options(
        boost::program_options::options_description &options,
        boost::program_options::positional_options_description &positional) = 0;

    /**
     * @brief Called after parsing for potential post-processing.
     *
     * This method does nothing but can be overwritten by child classes.  It is
     * called after argument parsing so any post processing (like checking if a
     * was set) can be done here.
     *
     * @param args The parsed arguments.
     */
    virtual void postprocess(const boost::program_options::variables_map &)
    {
    }

    /**
     * @brief Parses command line options.
     *
     * @param argc Number of command line arguments.
     * @param argv Command line arguments.
     *
     * @return False on error or if -h/--help is set, otherwise true.
     */
    bool parse_args(int argc, char **argv);
};

}  // namespace trifinger_object_tracking
