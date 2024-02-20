#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <set>
#include <random>
#include <algorithm>
#include <map>
#include <string>
#include "tinyxml2.h"
#include "warehouse_xml_generator.hpp"
#include "WarehouseAutomationPlantUMLGenerator.cpp"


using namespace tinyxml2;   

/* enum class SpecialLocationType {
    None,  // Normal location
    Type1, // Barrier special location type
    Type2, // Oil Spill special location type
    Type3, // Charging special location type
    Type4, // Fourth special location type 
}; */


void warehouse_xml_generator::AddParameters(tinyxml2::XMLNode* component, int numVariables, int numTransitions) {
    // x1, x2, c as parameters
    const char* variableNames[] = {"x1", "x2", "c"};
    for (int i = 0; i < numVariables; ++i) {
        XMLElement* param = component->GetDocument()->NewElement("param");
        param->SetAttribute("name", variableNames[i]);
        param->SetAttribute("type", "real");
        param->SetAttribute("local", "false");
        param->SetAttribute("d1", "1");
        param->SetAttribute("d2", "1");
        param->SetAttribute("dynamics", "any");
        component->InsertEndChild(param);
    }


    XMLComment* comment = component->GetDocument()->NewComment(" Transition Parameters ");
    component->InsertEndChild(comment);

    // transition labels as parameters
    for (int i = 1; i <= numTransitions; ++i) {
        XMLElement* param = component->GetDocument()->NewElement("param");
        param->SetAttribute("name", ("e" + std::to_string(i)).c_str());
        param->SetAttribute("type", "label");
        param->SetAttribute("local", "false");
        component->InsertEndChild(param);
    }
}



//void AddLocation(XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, bool isSpecial = false) {
void warehouse_xml_generator::AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType) {

    XMLElement* location = component->GetDocument()->NewElement("location");
    location->SetAttribute("id", id);
    location->SetAttribute("name", ("loc" + std::to_string(id)).c_str());

   /*  // Add a comment indicating the type of special location
    std::string typeComment;
    switch (specialType) {
        case SpecialLocationType::Type1:
            typeComment = "Barrier";
            break;
        case SpecialLocationType::Type2:
            typeComment = "Oil Spill";
            break;
        case SpecialLocationType::Type3:
            typeComment = "Charging Location";
            break;
        default:
           ;
    }
    XMLComment* comment = component->GetDocument()->NewComment(("Type: " + typeComment).c_str());
    location->InsertEndChild(comment);
 */

    XMLElement* invariant = component->GetDocument()->NewElement("invariant");
    // std::string invariantText = "x1>=" + std::to_string(x1Min) + " & x1<=" + std::to_string(x1Max) + 
    //                             " & x2>=" + std::to_string(x2Min) + " & x2<=" + std::to_string(x2Max) + 
    //                             " & c>=1";

    std::string invariantText = "x1>=" + std::to_string(x2Min) + " & x1<=" + std::to_string(x2Max) + 
                                " & x2>=" + std::to_string(x1Min) + " & x2<=" + std::to_string(x1Max) + 
                                " & c>=0.1";

    invariant->SetText(invariantText.c_str());
    location->InsertEndChild(invariant);

    //std::string flowText = isSpecial ? "x1'==0 & x2'==0 & c'==0" : "x1'==z & x2'==z & c'==-1";
    std::string flowText;
    switch (specialType) {
        case SpecialLocationType::Type1:
            flowText = "x1'==0 & x2'==0 & c'==-1";
            location->InsertEndChild(component->GetDocument()->NewComment("Barrier"));
            break;
        case SpecialLocationType::Type2:
            flowText = "x1'>=-1 & x1'<=1 & x2'>=-1 & x2'<=1 & c'==-2";
            location->InsertEndChild(component->GetDocument()->NewComment("Oil Spill"));
            break;
        case SpecialLocationType::Type3:
            flowText = "x1'>=-1 & x1'<=1 & x2'>=-1 & x2'<=1 & c'==0";
            location->InsertEndChild(component->GetDocument()->NewComment("Charging Location"));
            break;
        default:
            flowText = "x1'>=-1 & x1'<=1 & x2'>=-1 & x2'<=1 & c'==-1";
            // No comment for normal locations
            break;
    }
    
    XMLElement* flow = component->GetDocument()->NewElement("flow");
    flow->SetText(flowText.c_str());
    location->InsertEndChild(flow);

    component->InsertEndChild(location);
}



/*void AddTransition(XMLNode* component, int& transitionLabelCounter, int source, int target) {
    XMLElement* transition = component->GetDocument()->NewElement("transition");
    transition->SetAttribute("source", source);
    transition->SetAttribute("target", target);

    XMLElement* label = component->GetDocument()->NewElement("label");
    label->SetText(("e" + std::to_string(transitionLabelCounter++)).c_str());
    transition->InsertEndChild(label);

    XMLElement* guard = component->GetDocument()->NewElement("guard");
    guard->SetText(("x1==1 & x2>=" + std::to_string(source - 1) + " & x2<" + std::to_string(target - 1)).c_str());
    transition->InsertEndChild(guard);

    component->InsertEndChild(transition);
}*/

void warehouse_xml_generator::AddTransition(tinyxml2::XMLNode* component, int& transitionLabelCounter, int source, int target, int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations) {
    // source and target positions in the grid
    int sourceRow = (source - 1) / colSize;
    int sourceCol = (source - 1) % colSize;
    int targetRow = (target - 1) / colSize;
    int targetCol = (target - 1) % colSize;

    bool isHorizontal = sourceRow == targetRow;
    bool isVertical = sourceCol == targetCol;

    std::string guardCondition;
    if (isHorizontal) {
        // Horizontal transition
        guardCondition = "x1==" + std::to_string(sourceCol + 1) + " & x2>=" + std::to_string(sourceRow) + " & x2<" + std::to_string(sourceRow + 1);
    } else if (isVertical) {
        // Vertical transition
        guardCondition = "x1>=" + std::to_string(sourceCol) + " & x1<" + std::to_string(sourceCol + 1) + " & x2==" + std::to_string(sourceRow + 1);
    }
    
    // Transition from source to target
    XMLElement* transition = component->GetDocument()->NewElement("transition");
    transition->SetAttribute("source", source);
    transition->SetAttribute("target", target);

    XMLElement* label = component->GetDocument()->NewElement("label");
    label->SetText(("e" + std::to_string(transitionLabelCounter++)).c_str());
    transition->InsertEndChild(label);

    XMLElement* guard = component->GetDocument()->NewElement("guard");
    guard->SetText(guardCondition.c_str());
    transition->InsertEndChild(guard);

    // Checks if the source location type is Type3 and add assignment
    auto it = specialLocations.find(source);
    if (it != specialLocations.end() && it->second == SpecialLocationType::Type3) {
        XMLElement* assignment = component->GetDocument()->NewElement("assignment");
        assignment->SetText("c:=10");
        transition->InsertEndChild(assignment);
    }

    component->InsertEndChild(transition);

	// checks when the source location is blocked, if the target location is also not blocked
	
    // Transition from target back to source (back edge)
    XMLElement* backTransition = component->GetDocument()->NewElement("transition");
    backTransition->SetAttribute("source", target);
    backTransition->SetAttribute("target", source);

    XMLElement* backLabel = component->GetDocument()->NewElement("label");
    backLabel->SetText(("e" + std::to_string(transitionLabelCounter++)).c_str());
    backTransition->InsertEndChild(backLabel);

    std::string backGuardCondition;
    if (isHorizontal) {
        // Horizontal back transition
        backGuardCondition = "x1==" + std::to_string(targetCol) + " & x2>=" + std::to_string(targetRow) + " & x2<" + std::to_string(targetRow + 1);
    } else if (isVertical) {
        // Vertical back transition
        backGuardCondition = "x1>=" + std::to_string(targetCol) + " & x1<" + std::to_string(targetCol + 1) + " & x2==" + std::to_string(targetRow);
    }

    XMLElement* backGuard = component->GetDocument()->NewElement("guard");
    backGuard->SetText(backGuardCondition.c_str());
    backTransition->InsertEndChild(backGuard);

    component->InsertEndChild(backTransition);
}

void warehouse_xml_generator::GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename) {
    // Ensures the output directory exists and keep previous files
    std::string outputDir = "generated_models";
    std::string mkdirCommand = "mkdir -p " + outputDir;
    system(mkdirCommand.c_str()); // Execute command to create directory if it doesn't exist

    // Full path for the XML file
    std::string fullXmlPath = outputDir + "/" + xmlFilename;

    XMLDocument doc;
    int transitionLabelCounter = 1; // Counter for transition labels

    XMLNode* pRoot = doc.NewElement("sspaceex");
    doc.InsertFirstChild(pRoot);
    pRoot->ToElement()->SetAttribute("xmlns", "http://www-verimag.imag.fr/xml-namespaces/sspaceex");
    pRoot->ToElement()->SetAttribute("version", "0.2");
    pRoot->ToElement()->SetAttribute("math", "SpaceEx");

    XMLNode* component = doc.NewElement("component");
    component->ToElement()->SetAttribute("id", "warehouse_automation_agent");
    pRoot->InsertEndChild(component);

    // The total number of transitions is the sum of the transitions for each cell, excluding the last column and row
    // Each non-edge cell has two transitions (right and below) and two back edges    
    int numTransitions = 4 * (rowSize * colSize - rowSize);

    // Adds parameters
    AddParameters(component, 3, numTransitions);

    XMLText* newline = doc.NewText("\n");
    component->InsertEndChild(newline);
    component->InsertEndChild(newline);

    // Adds locations
    for (int row = 0; row < rowSize; ++row) {
        for (int col = 0; col < colSize; ++col) {
            int id = row * colSize + col + 1;
            auto it = specialLocations.find(id);
            SpecialLocationType specialType = (it != specialLocations.end()) ? it->second : SpecialLocationType::None;
            AddLocation(component, id, row, row + 1, col, col + 1, specialType);
        }
    }

    newline = doc.NewText("\n");
    component->InsertEndChild(newline);
    newline = doc.NewText("\n");
    component->InsertEndChild(newline);

    // Add transitions between adjacent locations, including back edges
    /*for (int row = 0; row < gridSize; ++row) {
        for (int col = 0; col < gridSize; ++col) {
            int id = row * gridSize + col + 1;
            if (std::find(specialLocations.begin(), specialLocations.end(), id) == specialLocations.end()) {
                if (col < gridSize - 1 && std::find(specialLocations.begin(), specialLocations.end(), id + 1) == specialLocations.end()) {
                    AddTransition(component, transitionLabelCounter, id, id + 1);
                }
                if (row < gridSize - 1 && std::find(specialLocations.begin(), specialLocations.end(), id + gridSize) == specialLocations.end()) {
                    AddTransition(component, transitionLabelCounter, id, id + gridSize);
                }
            }
        }
    }*/
    
   /* for (int row = 0; row < rowSize; row++) {
    for (int col = 0; col < colSize; col++) {
        // ID calculation
        int id = (rowSize - row - 1) * colSize + col + 1;

        // Checks if the current cell is a Type 1 special location
        auto currentCellIt = specialLocations.find(id);
        bool isCurrentCellType1 = currentCellIt != specialLocations.end() && currentCellIt->second == SpecialLocationType::Type1;

        // Adds transition to the right neighbor if it's not Type 1
        if (col < colSize - 1) {
            int rightNeighborId = id + 1;
            auto rightNeighborIt = specialLocations.find(rightNeighborId);
            bool isRightNeighborType1 = rightNeighborIt != specialLocations.end() && rightNeighborIt->second == SpecialLocationType::Type1;

            if (!isCurrentCellType1 && !isRightNeighborType1) {
                AddTransition(component, transitionLabelCounter, id, rightNeighborId, rowSize, colSize, specialLocations);
            }
        }

        // Adds transition to the below neighbor if it's not Type 1
        // and if it's not the bottom row
        if (row < rowSize - 1) {
            int belowNeighborId = id - colSize;
            auto belowNeighborIt = specialLocations.find(belowNeighborId);
            bool isBelowNeighborType1 = belowNeighborIt != specialLocations.end() && belowNeighborIt->second == SpecialLocationType::Type1;

            if (!isCurrentCellType1 && !isBelowNeighborType1) {
                AddTransition(component, transitionLabelCounter, id, belowNeighborId, rowSize, colSize, specialLocations);
            }
        }
    }
} */

    for (int row = 0; row < rowSize; row++) {
        for (int col = 0; col < colSize; col++) {
            // ID calculation, adjusted for the description
            int id = row * colSize + col + 1;

            // Find the special type of the current cell
            auto currentCellIt = specialLocations.find(id);
            bool isCurrentCellType1 = currentCellIt != specialLocations.end() && currentCellIt->second == SpecialLocationType::Type1;

            // Transition to the right neighbor if within bounds
            if (col < colSize - 1) {
                int rightNeighborId = id + 1;
                auto rightNeighborIt = specialLocations.find(rightNeighborId);
                bool isRightNeighborType1 = rightNeighborIt != specialLocations.end() && rightNeighborIt->second == SpecialLocationType::Type1;

                if (!isCurrentCellType1 && !isRightNeighborType1) {
                    AddTransition(component, transitionLabelCounter, id, rightNeighborId, rowSize, colSize, specialLocations);
                }       
            }

            // Transition to the top neighbor if within bounds
            if (row < rowSize - 1) { // 'row > 0' because row 0 is the topmost row
                int topNeighborId = id + colSize;
                auto topNeighborIt = specialLocations.find(topNeighborId);
                bool isTopNeighborType1 = topNeighborIt != specialLocations.end() && topNeighborIt->second == SpecialLocationType::Type1;

                if (!isCurrentCellType1 && !isTopNeighborType1) {
                    AddTransition(component, transitionLabelCounter, id, topNeighborId, rowSize, colSize, specialLocations);
                }        
            }
        }
    }

    // Save the XML document
    doc.SaveFile(fullXmlPath.c_str());
    std::cout << "Generated XML file: " << fullXmlPath << std::endl;
}


std::map<int, SpecialLocationType> warehouse_xml_generator::GenerateSpecialLocations(int rowSize, int colSize, int numType1, int numType2, int numType3) {
    std::map<int, SpecialLocationType> specialLocations;
    std::set<int> assignedCells;
    srand(time(nullptr)); 

    auto generateRandomCell = [&]() {
        return rand() % (rowSize * colSize) + 1;
    };

    // Assign Type1 cells
    for (int i = 0; i < numType1; ++i) {
        int cell;
        do {
            cell = generateRandomCell();
        } while (assignedCells.find(cell) != assignedCells.end());
        specialLocations[cell] = SpecialLocationType::Type1;
        assignedCells.insert(cell);
    }

    // Assign Type2 cells
    for (int i = 0; i < numType2; ++i) {
        int cell;
        do {
            cell = generateRandomCell();
        } while (assignedCells.find(cell) != assignedCells.end());
        specialLocations[cell] = SpecialLocationType::Type2;
        assignedCells.insert(cell);
    }

    // Assign Type3 cells
    for (int i = 0; i < numType3; ++i) {
        int cell;
        do {
            cell = generateRandomCell();
        } while (assignedCells.find(cell) != assignedCells.end());
        specialLocations[cell] = SpecialLocationType::Type3;
        assignedCells.insert(cell);
    }

    return specialLocations;
}


/* void generateCfgFile(const std::map<int, SpecialLocationType>& specialLocations, const std::string& filename, int rowSize, int colSize) {
    srand(static_cast<unsigned int>(time(nullptr))); // Seed random number generator

    auto generateRandomCell = [&]() {
        return rand() % (rowSize * colSize) + 1;
    };

    auto findValidLocation = [&](SpecialLocationType avoidType, int excludeLocation = -1) {
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
    };

    int initialLocation = findValidLocation(SpecialLocationType::Type1);
    // int initialLocation = 5;
    int forbiddenLocation = findValidLocation(SpecialLocationType::None, initialLocation);

    std::string initialDynamics = (specialLocations.at(initialLocation) == SpecialLocationType::None)
                                  ? "x1'==z & x2'==z & c'==-1"
                                  : "x1'==a & x2'==b & c'==c"; // Placeholder dynamics for special types

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
} */

void warehouse_xml_generator::generateCfgFile(const std::map<int, SpecialLocationType>& specialLocations, const std::string& filename, int rowSize, int colSize, int startingCharge) {
    srand(static_cast<unsigned int>(time(nullptr))); // Seeds random number generator
    // Prepends the directory path to the filename
    std::string directoryPath = "generated_models/"; // Ensures this directory exists or create it before writing the file
    std::string fullPath = directoryPath + filename;

    auto findValidLocation = [&](SpecialLocationType blocked, int excludeLocation = -1) -> int {
        std::vector<int> validLocations;
        for (const auto& loc : specialLocations) {
            if (loc.second != blocked/*  */ && loc.first != excludeLocation) {
                validLocations.push_back(loc.first);
            }
        }

        if (validLocations.empty()) {
            throw std::runtime_error("No valid locations available.");
        }

        return validLocations[rand() % validLocations.size()];
    };

    // Fixed initial location
    int initialLocation = findValidLocation(SpecialLocationType::Type1);
    int forbiddenLocation = findValidLocation(SpecialLocationType::Type1, initialLocation);

    // Calculate row and column from location ID
    int initialRow = (initialLocation - 1) / colSize;
    int initialCol = (initialLocation - 1) % colSize;

    // Define ranges based on row and column
    double x1Min = initialCol;
    double x1Max = initialCol + 1;
    double x2Min = initialRow;
    double x2Max = initialRow + 1;
    double cValue = startingCharge;

    // Generate random values within ranges
    double x1Value = static_cast<int>((x1Min + static_cast<double>(rand()) / RAND_MAX * (x1Max - x1Min)) * 10) / 10.0;
    double x2Value = static_cast<int>((x2Min + static_cast<double>(rand()) / RAND_MAX * (x2Max - x2Min)) * 10) / 10.0;

    // Construct the initially attribute
    std::ofstream cfgFile(fullPath); // Use fullPath to create the file in the desired directory
    if (cfgFile.is_open()) {
        cfgFile << "# analysis options\n";
        cfgFile << "system = \"warehouse_automation_agent\"\n";
        cfgFile << "initially = \"x1==" << x1Value << " & x2==" << x2Value << " & c==" << cValue << " & loc()==loc" << initialLocation << "\"\n";
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

void warehouse_xml_generator::GenerateFilesForModel(const std::string& baseName, int rowSize, int colSize, int numType1, int numType2, int numType3,
                                                    int depth, int startingCharge) {
    warehouse_xml_generator generator;
    // Step 0: Ensure the output directory exists
    std::string outputDir = "generated_models";
    std::string command = "mkdir -p " + outputDir; // 'mkdir -p' command works on Unix/Linux to create directories
    system(command.c_str()); // Execute the command to ensure the directory exists

    // srand(static_cast<unsigned int>(time(nullptr))); // Seeds random number generator

    // Step 1: Generate Special Locations
    auto specialLocations = GenerateSpecialLocations(rowSize, colSize, numType1, numType2, numType3);

    // Prepare file paths
    std::string xmlPath = baseName + ".xml";
    std::string cfgPath = baseName + ".cfg";

    // Generate the XML File
    GenerateXml(rowSize, colSize, specialLocations, xmlPath);

    // Generate the CFG File
    generateCfgFile(specialLocations, cfgPath, rowSize, colSize, startingCharge);

    std::cout << "XML and CFG files for " << baseName << " have been generated successfully in " << outputDir << std::endl;
    
    // Generate the PlantUML file
    std::string pumlPath = baseName + ".puml";
    WarehouseAutomationPlantUMLGenerator::generatePlantUML(specialLocations, rowSize, colSize, pumlPath);
}



/* void warehouse_xml_generator::run(int rowSize, int colSize, int numType1, int numType2, int numType3){
    // Generate special locations based on the parameters
    auto specialLocations = warehouse_xml_generator::GenerateSpecialLocations(rowSize, colSize, numType1, numType2, numType3);
    
    // Generate the XML file
    GenerateXml(rowSize, colSize, specialLocations);
    
    // Generate the CFG file
    generateCfgFile(specialLocations, "output.cfg", rowSize, colSize);
} */

/*int main() {
    int rowSize = 5;
    int colSize = 4;
    //std::vector<int> specialLocations = {2, 4}; // Example: Locations 2 and 4 are special
    const int numType1 = 2; // Number of Type1 special cells
    const int numType2 = 2; // Number of Type2 special cells
    const int numType3 = 1; // Number of Type3 special cells

    // Define more types if necessary

    auto specialLocations = GenerateSpecialLocations(rowSize, colSize, numType1, numType2, numType3);

    GenerateXml(rowSize, colSize, specialLocations);

    return 0;
}*/

/* int main() {
    int rowSize = 5;
    int colSize = 4;
    const int numType1 = 2;
    const int numType2 = 2;
    const int numType3 = 1;

    auto specialLocations = GenerateSpecialLocations(rowSize, colSize, numType1, numType2, numType3);
    GenerateXml(rowSize, colSize, specialLocations); // Generate XML

    generateCfgFile(specialLocations, "output.cfg", rowSize, colSize); // Generate CFG

    return 0;
}
 */