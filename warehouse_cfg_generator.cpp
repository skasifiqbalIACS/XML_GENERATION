#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <string>

enum class SpecialLocationType {
    None,  // Normal location
    Type1, // Barrier special location type
    Type2, // Oil Spill special location type
    Type3, // Charging special location type
};

class GridConfigGenerator {
public:
    GridConfigGenerator(int rows, int cols, const XmlGenerator& xmlGen)
        : rowSize(rows), colSize(cols), specialLocations(xmlGen.GetSpecialLocations()) {
        srand(static_cast<unsigned int>(time(nullptr)));
    }

    void generateSpecialLocations(int numType1, int numType2, int numType3) {
        // Assign Type1, Type2, Type3
        assignSpecialCells(numType1, SpecialLocationType::Type1);
        assignSpecialCells(numType2, SpecialLocationType::Type2);
        assignSpecialCells(numType3, SpecialLocationType::Type3);
    }

    void generateCfgFile(const std::string& filename) {
        int initialLocation = findValidLocation(SpecialLocationType::Type1);
        int forbiddenLocation = findValidLocation(SpecialLocationType::None, initialLocation);

        std::string initialDynamics = generateDynamics(specialLocations.at(initialLocation));

        std::ofstream cfgFile(filename);
    if (cfgFile.is_open()) {
        cfgFile << "# analysis options\n";
        cfgFile << "system = \"warehouse_automation_agent\"\n";
        cfgFile << "initially = \"" << initialDynamics << " & loc()==loc" << initialLocation << "\"\n";
        cfgFile << "forbidden = \"loc()==loc" << forbiddenLocation << "\"\n";
        cfgFile << "iter-max = 50\n";
        cfgFile << "rel-err = 1.0e-12\n";
        cfgFile << "abs-err = 1.0e-13\n";
        cfgFile.close();
        std::cout << "CFG file generated successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}


private:
    int rowSize, colSize;
    const std::map<int, SpecialLocationType>& specialLocations;

    int findValidLocation(SpecialLocationType avoidType, int excludeLocation = -1) {
        std::vector<int> validLocations;
        for (const auto& loc : specialLocations) {
            if (loc.second != avoidType && loc.first != excludeLocation) {
                validLocations.push_back(loc.first);
            }
        }

        if (validLocations.empty()) {
            throw std::runtime_error("No valid locations available");
        }

        return validLocations[rand() % validLocations.size()];
    }

    int generateRandomCell() {
        return rand() % (rowSize * colSize) + 1;
    }

    std::string generateDynamics(SpecialLocationType type) {
        switch (type) {
            case SpecialLocationType::Type1:
                return "x1'==a & x2'==b & c'==c";
            case SpecialLocationType::Type2:
                return "x1'==d & x2'==e & c'==f";
            default:
                return "x1'==z & x2'==z & c'==-1";
        }
    }
};

int main() {
    int rowSize = 5;
    int colSize = 4;
    const int numType1 = 2;
    const int numType2 = 2;
    const int numType3 = 1;

    XmlGenerator xmlGenerator(rowSize, colSize, numType1, numType2, numType3);
    xmlGenerator.GenerateXml();

    GridConfigGenerator configGenerator(rowSize, colSize, xmlGenerator);
    configGenerator.generateCfgFile("analysis.cfg");

    return 0;
}
