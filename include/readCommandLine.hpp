#ifndef READCOMMANDLINE_HPP
#define READCOMMANDLINE_HPP

#include "userOptions.hpp"

/**
 * Parses the command line arguments and updates the provided userOptions object.
 * 
 * @param argc Number of command line arguments.
 * @param argv Array of command line arguments.
 * @param user_options Reference to userOptions object to be updated based on command line arguments.
 */
void readCommandLine(int argc, char *argv[], userOptions& user_options);

#endif // READCOMMANDLINE_HPP
