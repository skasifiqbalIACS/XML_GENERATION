#include <boost/program_options.hpp>
#include "userOptions.hpp"
#include "readCommandLine.hpp"

namespace po = boost::program_options;

void readCommandLine(int argc, char *argv[], userOptions& user_options) {
    po::options_description desc("XML Generation Project Options");
    po::variables_map vm;

    desc.add_options()
        ("help,h", "Display help message")
        ("rowSize,r", po::value<unsigned int>(), "Set row size for the grid")
        ("colSize,c", po::value<unsigned int>(), "Set column size for the grid")
        ("numBarriers,b", po::value<unsigned int>()->default_value(2), "Set number of barriers")
        ("numOilSpills,o", po::value<unsigned int>()->default_value(2), "Set number of oil spill locations")
        ("numChargingStations,s", po::value<unsigned int>()->default_value(1), "Set number of charging stations")
        ("numUnsatModels,u", po::value<unsigned int>()->default_value(1), "Set number of unsat model files required")
        ("depth,d", po::value<unsigned int>()->default_value(10), "Set the depth of paths to be checked")
        ("startingCharge,sc", po::value<double>()->default_value(10), "Set the starting charge for the agent");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        exit(0);
    }

    if (vm.count("rowSize")) {
        user_options.setRowSize(vm["rowSize"].as<unsigned int>());
    }

    if (vm.count("colSize")) {
        user_options.setColSize(vm["colSize"].as<unsigned int>());
    }

    if (vm.count("numBarriers")) {
        user_options.setNumBarriers(vm["numBarriers"].as<unsigned int>());
    }

    if (vm.count("numOilSpills")) {
        user_options.setNumOilSpills(vm["numOilSpills"].as<unsigned int>());
    }

    if (vm.count("numChargingStations")) {
        user_options.setNumChargingStations(vm["numChargingStations"].as<unsigned int>());
    }

    if (vm.count("numUnsatModels")) {
        user_options.setNumUnsatModels(vm["numUnsatModels"].as<unsigned int>());
    }

    if (vm.count("depth")) {
        user_options.setDepth(vm["depth"].as<unsigned int>());
    }

    if (vm.count("startingCharge")) {
        user_options.setStartingCharge(vm["startingCharge"].as<double>());
    }
}

/* int main(int argc, char *argv[]) {
    userOptions user_options;
    readCommandLine(argc, argv, user_options);

    // After reading command line, proceed with the rest of the program
    // For example, initializing the simulation with the options
    std::cout << "Row Size: " << user_options.getRowSize() << std::endl;
    std::cout << "Column Size: " << user_options.getColSize() << std::endl;
    std::cout << "Number of Barriers: " << user_options.getNumBarriers() << std::endl;
    std::cout << "Number of Oil Spills: " << user_options.getNumOilSpills() << std::endl;
    std::cout << "Number of Charging Stations: " << user_options.getNumChargingStations() << std::endl;
    std::cout << "Number of Unsat Models: " << user_options.getNumUnsatModels() << std::endl;

    return 0;
} */
