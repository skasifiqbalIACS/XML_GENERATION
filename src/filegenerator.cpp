#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // Include for std::stringstream
#include <cstdlib>
#include <unistd.h> // Include for sleep function
#include <boost/tokenizer.hpp>
#include <algorithm>    // Include for find function
#include "tinyxml2.h"
#include "filegenerator.hpp"
#include "warehouse_xml_generator.hpp" 

using namespace std;
using namespace tinyxml2;

/* class UnsatFileGenerator {
private:
    int targetUnsatFiles;
    int rowSize, colSize;
    int numType1, numType2, numType3; 
    warehouse_xml_generator xmlGenerator;

public: */
    // Constructor
    /* UnsatFileGenerator::UnsatFileGenerator(int targetFiles, int rows, int cols, int type1, int type2, int type3)
    : targetUnsatFiles(targetFiles), rowSize(rows), colSize(cols), numType1(type1), numType2(type2), numType3(type3) {} */

    /* UnsatFileGenerator::UnsatFileGenerator(int targetFiles, int rows, int cols, int type1, int type2, int type3, int depth, int startingCharge)
    : targetUnsatFiles(targetFiles), rowSize(rows), colSize(cols), numType1(type1), numType2(type2), numType3(type3), depth(depth), startingCharge(startingCharge) {
    }

    void UnsatFileGenerator::generateFiles()
    {
        int generatedUnsatFiles = 0;
        int attempt = 1;
        while (generatedUnsatFiles < targetUnsatFiles) {
            string baseName = "unsat_model_" + to_string(attempt++);
            
            // Generates XML and CFG files using the GenerateXml instance
            xmlGenerator.GenerateFilesForModel(baseName, rowSize, colSize, numType1, numType2, numType3, depth, startingCharge);

            // sleep(1);

            // The cH_Replay function should be adjusted to work with paths for XML and CFG files
            if (!cH_Replay(baseName)) {
                generatedUnsatFiles++;
                cout << "UNSAT path found for " << baseName << ". Keeping files." << endl;
            } else {
                // generated_models/ is the directory for storing these files
                string xmlPath = "generated_models/" + baseName + ".xml";
                string cfgPath = "generated_models/" + baseName + ".cfg";
                string pumlPath = "generated_models/" + baseName + ".puml";

                // Delete files if the path is SAT
                remove(xmlPath.c_str());
                remove(cfgPath.c_str());
                remove(pumlPath.c_str());
                cout << "SAT path found for " << baseName << ". Deleting files." << endl;
            }
        }
    } */
    UnsatFileGenerator::UnsatFileGenerator(int targetFiles, int satFiles, int rows, int cols, int type1, int type2, int type3, int d, int charge)
        : targetUnsatFiles(targetFiles), targetSatFiles(satFiles), rowSize(rows), colSize(cols),
          numType1(type1), numType2(type2), numType3(type3), depth(d), startingCharge(charge) {}

    void UnsatFileGenerator::generateFiles() {
        int generatedUnsatFiles = 0;
        int generatedSatFiles = 0; // Counter for SAT files
        int attempt = 1;

        while (generatedUnsatFiles < targetUnsatFiles || generatedSatFiles < targetSatFiles) {
            string baseName = "model_" + to_string(attempt++);
            
            xmlGenerator.GenerateFilesForModel(baseName, rowSize, colSize, numType1, numType2, numType3, depth, startingCharge);

            if (!cH_Replay(baseName)) {
                if (generatedUnsatFiles < targetUnsatFiles) {
                    generatedUnsatFiles++;
                    cout << "UNSAT path found for " << baseName << ". Keeping files." << endl;
                } else {
                    // If we already have enough UNSAT files
                    deleteFiles(baseName);
                }
            } else {
                if (generatedSatFiles < targetSatFiles) {
                    generatedSatFiles++;
                    cout << "SAT path found for " << baseName << ". Keeping files." << endl;
                } else {
                    // If we already have enough SAT files
                    deleteFiles(baseName);
                }
            }
        }
    }

    void UnsatFileGenerator::deleteFiles(const string& baseName) {
        string xmlPath = "generated_models/" + baseName + ".xml";
        string cfgPath = "generated_models/" + baseName + ".cfg";
        string pumlPath = "generated_models/" + baseName + ".puml";

        remove(xmlPath.c_str());
        remove(cfgPath.c_str());
        remove(pumlPath.c_str());
        cout << "Extra file for " << baseName << ". Deleting files." << endl;
    }

    /*bool UnsatFileGenerator::cH_Replay(const string& str) {
        // The string cmd_str for invoking a system command
    // string cmd_str = "runlim -s 4096 -t 1000 -r 1000 -o generated_models/" + str + ".runlim"
    //         + " " + "/home/asif/BACH-5.5/bach -t -p -v2 -solver cmsat -semantics mixed" + " " + "generated_models/" +
    //         str + ".xml" + " " + "generated_models/" + str + ".cfg"
    //         + " 20 >" + " " +"generated_models/" + str + "BACH.log";
    string cmd_str = std::string("/home/asif/BACH-5.5/bach -t -p v2 -solver cmsat -semantics mixed") + " " + "generated_models/" + 
            str + ".xml" + " " + "generated_models/" + str + ".cfg" + "20 >" + " " + "generated_models/" + str + "BACH.log";
    
    // std::string cmd_str = std::string("runlim -s 4096 -t 1000 -r 1000 -o ") + str + ".runlim "
    //         + "/home/asif/BACH-5.5/bach -t -p -v2 -solver cmsat -semantics mixed "
    //         + str + ".xml " + str + ".cfg "
    //         + "20 > " + "generated_models/" + str + "BACH.log";

    system(cmd_str.c_str()); // calling BACH

    bool path_feasible = false;	// returns feasibility of a path

    fstream file;
    string line;
    typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
    file.open(str + "BACH.log", ios::out | ios::in);
    std::cout << "opening " + str + "BACH.log" << endl;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.compare("") == 0) {
                continue;
            }

            boost::char_separator<char> sep(" $()[]-><=\t\"");
            tokenizer tokens(line, sep);
            tokenizer::iterator tok_iter = tokens.begin();

            if (*tok_iter == "sat") {
                path_feasible = true;
                break;
            }
            else {
                continue;
            }
        }
    }
    file.close();
    return path_feasible;
    }*/

/* bool UnsatFileGenerator::cH_Replay(const string& str) {
    // Defines the path to your BACH directory and the lib directory within it
    string bachPath = "/home/asif/BACH-5.5";
    string libPath = bachPath + "/lib"; // Assuming libcryptominisat5.so.5.6 is here
    string depthbound = to_string(depth); // Convert depth to string correctly

    // Constructing the command using stringstream for easier manipulation
    stringstream cmd_ss;
    cmd_ss << "bash -c '";
    cmd_ss << "LD_LIBRARY_PATH=" << libPath << " "; // Set LD_LIBRARY_PATH
    cmd_ss << bachPath << "/bach -t -p -v2 -solver z3 -semantics mixed "; // Command
    cmd_ss << "generated_models/" << str << ".xml "; // Input XML
    cmd_ss << "generated_models/" << str << ".cfg "; // Input CFG
    cmd_ss << depthbound << " > generated_models/" << str << "BACH.log 2>&1'"; // Output redirection

    string cmd_str = cmd_ss.str(); // Convert stringstream to string
    cout << "Executing command: " << cmd_str << endl; // Debug: Print the command

    system(cmd_str.c_str()); // Execute the command

    // Check the log file for the result
    bool path_feasible = false;
    ifstream file("generated_models/" + str + "BACH.log");
    string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("sat") != string::npos) {
                path_feasible = true; // SAT found
                break;
            }
        }
    }
    file.close();
    return path_feasible;
} */

bool UnsatFileGenerator::cH_Replay(const string& str) {
    // Defines the path to your BACH directory and the lib directory within it

    std::string bachPath = "/home/asif/BACH-5.5";
    std::string libPath = bachPath + "/lib"; // libcryptominisat5.so.5.6 is here
    std::string depthbound = std::to_string(depth);
    // std::string depthbound = std::to_string(20);

    // Constructs the command
    // std::string cmd_str = "LD_LIBRARY_PATH=" + libPath + " ";
    // cmd_str += bachPath + "/bach -t -p -v2 -solver cmsat -semantics mixed ";
    // cmd_str += "generated_models/" + str + ".xml ";
    // cmd_str += "generated_models/" + str + ".cfg ";
    // cmd_str += "20 > generated_models/" + str + "BACH.log"; // Redirecting both stdout and stderr to a log file "BACH.log 2>&1"

    // // Executes the command
    // system(cmd_str.c_str()); // calling BACH

    std::string cmd_str = "bash -c '";
    cmd_str += "echo LD_LIBRARY_PATH is $LD_LIBRARY_PATH && "; // Debugging line
    cmd_str += "LD_LIBRARY_PATH=" + libPath + " ";
    cmd_str += bachPath + "/bach -t -p -v2 -solver z3 -semantics mixed ";
    cmd_str += "generated_models/" + str + ".xml ";
    cmd_str += "generated_models/" + str + ".cfg ";
    // cmd_str += "20 > generated_models/" + str + "BACH.log 2>&1'"; // Capturing stderr for debugging "BACH.log 2>&1'"
    cmd_str += depthbound + " > generated_models/" + str + "BACH.log 2>&1'"; // Capturing stderr for debugging "BACH.log 2>&1'"
    // std::cout << "Executing command: " << cmd_str << std::endl;

    system(cmd_str.c_str());


    // Checks the log file for the result
    bool path_feasible = true;
    std::ifstream file("generated_models/" + str + "BACH.log");
    std::string line;
    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("UNREACHABLE") != std::string::npos) {
                path_feasible = false;
                break;
            }
        }
    }
    file.close();
    return path_feasible;
}

