#include "warehouse_xml_generator_deterministic.hpp"
#include <sstream>
#include <iostream>
#include <fstream>

/* ModifiedWarehouseXmlGenerator::ModifiedWarehouseXmlGenerator(int rowSize, int colSize, int numType1, int numType2, int numType3, int depth, int charge)
: warehouse_xml_generator(rowSize, colSize, numType1, numType2, numType3, depth, charge) {
    // Constructor implementation
    // Initialize 
} */

/* ModifiedWarehouseXmlGenerator::~ModifiedWarehouseXmlGenerator() {
    // Cleanup code if needed
} */

void ModifiedWarehouseXmlGenerator::GenerateXml(int rowSize, int colSize, const std::map<int, SpecialLocationType>& specialLocations, const std::string& xmlFilename) {
    /* // Custom XML generation logic for the derived class
    std::cout << "Generating modified XML with additional parameters and different flows..." << std::endl;

    tinyxml2::XMLDocument doc;
    auto* pRoot = doc.NewElement("sspaceex");
    doc.InsertFirstChild(pRoot);
    pRoot->SetAttribute("xmlns", "http://www-verimag.imag.fr/xml-namespaces/sspaceex");
    pRoot->SetAttribute("version", "0.2");
    pRoot->SetAttribute("math", "SpaceEx");

    auto* component = doc.NewElement("component");
    component->SetAttribute("id", "modified_warehouse_automation_agent");
    pRoot->InsertEndChild(component);

    // Adding an additional parameter
    auto* param = doc.NewElement("param");
    param->SetAttribute("name", "additionalParam");
    param->SetAttribute("type", "real");
    param->SetAttribute("local", "false");
    component->InsertEndChild(param);

    // Continues with the XML generation logic as in the base class or with modifications
    // override or replicate methods like AddLocation and AddTransition as needed here

    std::stringstream ss;
    ss << "generated_models/_deterministic_" << xmlFilename ; // Assuming a directory "generated_models" exists
    doc.SaveFile(ss.str().c_str()); */

    // Ensure output directory exists
        std::string outputDir = "generated_models";
        std::string mkdirCommand = "mkdir -p " + outputDir;
        system(mkdirCommand.c_str());

        // Full path for the XML file
        std::string fullXmlPath = outputDir + "/" + xmlFilename;

        tinyxml2::XMLDocument doc;
        int transitionLabelCounter = 1; // Counter for transition labels

        auto* pRoot = doc.NewElement("sspaceex");
        doc.InsertFirstChild(pRoot);
        pRoot->ToElement()->SetAttribute("xmlns", "http://www-verimag.imag.fr/xml-namespaces/sspaceex");
        pRoot->ToElement()->SetAttribute("version", "0.2");
        pRoot->ToElement()->SetAttribute("math", "SpaceEx");

        auto* component = doc.NewElement("component");
        component->ToElement()->SetAttribute("id", "warehouse_automation_agent");
        pRoot->InsertEndChild(component);

        // AddParameters(component);

        int numTransitions = calculateTotalTransitions(specialLocations, rowSize, colSize);

        // Adds parameters 
        AddParameters(component, 4, numTransitions); // New parameter count and adjustment in numTransitions if needed

        auto* newline = doc.NewText("\n");
        auto* tabspace = doc.NewText("\t\t");
        component->InsertEndChild(newline);
        newline = doc.NewText("\n");
        component->InsertEndChild(newline);
        // component->InsertEndChild(tabspace);
        
        // Adds locations with modified flows
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
        component->InsertEndChild(tabspace);

        // Add transitions between adjacent locations, including back edges
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
        
        /* for (int row = 0; row < rowSize; row++) {
            for (int col = 0; col < colSize; col++) {
                int id = row * colSize + col + 1;

                // Check for special location types and potentially modify transition logic
                auto currentCellIt = specialLocations.find(id);
                SpecialLocationType currentType = (currentCellIt != specialLocations.end()) ? currentCellIt->second : SpecialLocationType::Type1;

                // Calculate IDs for right and top neighbors to add transitions
                if (col < colSize - 1) { // Right neighbor
                    int rightNeighborId = id + 1;
                    auto rightNeighborIt = specialLocations.find(rightNeighborId);
                    SpecialLocationType rightType = (rightNeighborIt != specialLocations.end()) ? rightNeighborIt->second : SpecialLocationType::Type1;
                    
                    // Example modification: Only add a transition if neither cell is of a certain type
                    if (currentType != SpecialLocationType::Type1 && rightType != SpecialLocationType::Type1) {
                        AddTransition(component, transitionLabelCounter, id, rightNeighborId, rowSize, colSize, specialLocations);
                    }
                }

                if (row < rowSize - 1) { // Top neighbor
                    int topNeighborId = id + colSize;
                    auto topNeighborIt = specialLocations.find(topNeighborId);
                    SpecialLocationType topType = (topNeighborIt != specialLocations.end()) ? topNeighborIt->second : SpecialLocationType::Type1;
                    
                    // Modify transition logic as needed
                    if (currentType != SpecialLocationType::Type1 && topType != SpecialLocationType::Type1) {
                        AddTransition(component, transitionLabelCounter, id, topNeighborId, rowSize, colSize, specialLocations);
                    }
                }
            }
        }
 */
        // Save the XML document
        doc.SaveFile(fullXmlPath.c_str());
        std::cout << "Generated modified XML file: " << fullXmlPath << std::endl;
}

void ModifiedWarehouseXmlGenerator::generateCfgFile(const std::tuple<double, double, int, int>& initialSetup, const std::string& filename, int startingCharge){
    // Directly call the base class implementation to handle everything except for modifying the 'initially' condition
    warehouse_xml_generator::generateCfgFile(initialSetup, filename + ".cfg", startingCharge);

    // Modify the 'initially' line in the generated CFG file
    std::string fullPath = "generated_models/" + filename + ".cfg";

    // Open the original CFG file to read
    std::ifstream originalCfgFile(fullPath);
    std::string content = "";
    std::string line;

    if (originalCfgFile.is_open()) {
        while (getline(originalCfgFile, line)) {
            // Modify the 'initially' line
            if (line.find("initially") != std::string::npos) {
                auto pos = line.find("& loc()");
                if (pos != std::string::npos) {
                    // Insert the additional condition before the location condition
                    line = line.substr(0, pos) + " & z>=-1 & z<=1 " + line.substr(pos);
                }
            }
            content += line + "\n";
        }
        originalCfgFile.close();
    } else {
        std::cerr << "Unable to open the original CFG file for reading." << std::endl;
        return;
    }

    fullPath = "generated_models/" + filename + "_base.cfg";
    // Write the modified content back to the CFG file
    std::ofstream modifiedCfgFile(fullPath);
    if (modifiedCfgFile.is_open()) {
        modifiedCfgFile << content;
        modifiedCfgFile.close();
        std::cout << "Modified CFG file with updated 'initially' condition generated successfully." << std::endl;
    } else {
        std::cerr << "Unable to open the CFG file for writing." << std::endl;
    }
}

void ModifiedWarehouseXmlGenerator::AddLocation(tinyxml2::XMLNode* component, int id, int x1Min, int x1Max, int x2Min, int x2Max, SpecialLocationType specialType) {
    // Custom implementation for adding locations
    // This is just an example and may need adjustments based on your XML schema and requirements
    
    // Example: Creating a location element with custom flow conditions
    auto* location = component->GetDocument()->NewElement("location");
    location->SetAttribute("id", std::to_string(id).c_str());
    location->SetAttribute("name", ("loc" + std::to_string(id)).c_str());

    auto* invariant = component->GetDocument()->NewElement("invariant");
    // std::string invariantText = "x1>=" + std::to_string(x1Min) + " & x1<=" + std::to_string(x1Max) + 
    //                             " & x2>=" + std::to_string(x2Min) + " & x2<=" + std::to_string(x2Max) + 
    //                             " & c>=1";

    std::string invariantText = "x1>=" + std::to_string(x2Min) + " & x1<=" + std::to_string(x2Max) + 
                                " & x2>=" + std::to_string(x1Min) + " & x2<=" + std::to_string(x1Max) + 
                                " & c>=0.1";

    invariant->SetText(invariantText.c_str());
    location->InsertEndChild(invariant);

    // Customizing flow for demonstration
    std::string flowText; // Custom flow for all locations
    switch (specialType) {
        case SpecialLocationType::Type1:
            flowText = "x1'==z & x2'==z & c'==-1";
            // location->InsertEndChild(component->GetDocument()->NewComment("Barrier"));
            break;
        case SpecialLocationType::Type2:
            flowText = "x1'==z & x2'==z & c'==-2";
            // location->InsertEndChild(component->GetDocument()->NewComment("Oil Spill"));
            break;
        case SpecialLocationType::Type3:
            flowText = "x1'==z & x2'==z & c'==0";
            // location->InsertEndChild(component->GetDocument()->NewComment("Charging Location"));
            break;
        default:
            flowText = "x1'==z & x2'==z & c'==-1";
            break;
    }
    auto* flow = component->GetDocument()->NewElement("flow");
    flow->SetText(flowText.c_str());
    location->InsertEndChild(flow);

    component->InsertEndChild(location);
}

void ModifiedWarehouseXmlGenerator::AddParameters(tinyxml2::XMLNode *component, int numVariables, int numTransitions){
    const char* variableNames[] = {"x1", "x2", "c", "z"};

    for (int i = 0; i < numVariables; ++i) {
        tinyxml2::XMLElement* param = component->GetDocument()->NewElement("param");
        param->SetAttribute("name", variableNames[i]);
        param->SetAttribute("type", "real");
        param->SetAttribute("local", "false");
        param->SetAttribute("d1", "1");
        param->SetAttribute("d2", "1");
        param->SetAttribute("dynamics", "any");
        component->InsertEndChild(param);
    }


    tinyxml2::XMLComment* comment = component->GetDocument()->NewComment(" Transition Parameters ");
    component->InsertEndChild(comment);

    // transition labels as parameters
    for (int i = 1; i <= numTransitions; ++i) {
        tinyxml2::XMLElement* param = component->GetDocument()->NewElement("param");
        param->SetAttribute("name", ("e" + std::to_string(i)).c_str());
        param->SetAttribute("type", "label");
        param->SetAttribute("local", "false");
        component->InsertEndChild(param);
    }
}

void ModifiedWarehouseXmlGenerator::GenerateFilesForModel(const std::map<int, SpecialLocationType>& specialLocations,
                                                    const std::string& baseName, int rowSize, int colSize, int depth, int startingCharge, 
                                                    const std::tuple<double, double, int, int> initialSetup)  {
    
    std::string xmlPath = baseName + "_base.xml";
    // std::string cfgPath = baseName + "_base.cfg";
    // std::string cfgPath = baseName + ".cfg";
    std::string cfgPath = baseName ;

    // Use the generator (base class logic) to generate initial files
    GenerateXml(rowSize, colSize, specialLocations, xmlPath);
    generateCfgFile(initialSetup, cfgPath, startingCharge);
    // WarehouseAutomationPlantUMLGenerator::generatePlantUML(specialLocations, rowSize, colSize, pumlPath);

    
}

