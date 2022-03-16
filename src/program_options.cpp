/**
 * @file
 * @copyright 2020, Max Planck Gesellschaft.  All rights reserved.
 * @license BSD 3-clause
 */
#include <trifinger_object_tracking/program_options.hpp>

namespace trifinger_object_tracking
{
bool ProgramOptions::parse_args(int argc, char **argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    po::positional_options_description p;

    // add --help/-h by default
    desc.add_options()("help,h", "Show help message.");
    // add user-specific options
    add_options(desc, p);

    try
    {
        po::variables_map args;
        po::store(po::command_line_parser(argc, argv)
                      .options(desc)
                      .positional(p)
                      .run(),
                  args);

        if (args.count("help"))
        {
            std::cout << help() << desc << std::endl;
            return false;
        }
        po::notify(args);

        postprocess(args);
    }
    catch (const po::error &e)
    {
        std::cerr << "Invalid arguments: " << e.what() << std::endl;
        return false;
    }

    return true;
}
}  // namespace trifinger_object_tracking
