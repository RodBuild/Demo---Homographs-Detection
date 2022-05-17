#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
#include <filesystem>
#include <windows.h>
using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;
using std::filesystem::current_path;
// Symbols
const char SLASH = '/';
const std::string SINGLE_DOT = ".";
const std::string DOUBLE_DOT = "..";
const std::string DRIVES[2] = {"C:", "D:"};


/*************************
 * Run example test case *
 *************************/
void runTestCases() {
    cout << ":)\n";
}

/*************************************
 * Determine if paths are homographs *
 *************************************/
bool areHomographs(const vector<string> target_path, const vector<string> secret_path) {
    bool homographs = true;
    for (int i = 0; i < secret_path.size(); i++) {
        if (target_path[i] != secret_path[i]) {
            homographs = false;
            break;
        }
    }
    return homographs;
}

/****************************************
 * Create a canon token of a given path *
 ****************************************/
vector<string> canonicalize(const std::string p) {
    // delete first '/'
    string path = p;
    if (path[0] == '/') {
        path.erase(0,1);
    }

    vector<string> vgiven_path, vbase_path;
    std::filesystem::path system;
    system = std::filesystem::current_path();
    string base_path{system.u8string()};

    //normalize base path
    for (int i = 0; i < base_path.size(); i++)
    {
        if (base_path[i] == '\\')
        {
            base_path[i] = SLASH;
        }
    }
    //split base path and store in a vector
    stringstream ss(base_path);
    string dir;
    while (getline(ss, dir, SLASH))
        vgiven_path.push_back(dir);

    //split given path and store in a vector
    stringstream ss2(path);
    string dir2;
    while (getline(ss2, dir2, SLASH))
        vgiven_path.push_back(dir2);

    //use given path to navigate base path
    for (auto it = vgiven_path.begin(); it != vgiven_path.end(); it++) {
        if (*it == SINGLE_DOT) {
            //do nothing
        }
        else if (*it == DOUBLE_DOT) {
            vbase_path.pop_back();
        }
        else {
            vbase_path.push_back(*it);
        }

        //if base path empty at some point -> return the vector to show error
        if (vbase_path.empty()) {
            return vbase_path;
        }
    }

    /*DEBUG*/
    // display ultimate path (given path + base path)
    // for (auto x: vbase_path) {
    //     cout << x << "/";
    // }
    // cout << endl;

    return vbase_path;
}

/************************
 * Display Instructions *
 ************************/
void displayMenu() {
    cout << "\n ----------------------------\n";
    cout << "|     Lab 05: Homographs     |\n";
    cout << "|----------------------------|\n";
    cout << "|  (1) Run Test Cases        |\n";
    cout << "|  (2) Manually Excute Test  |\n";
    cout << "|  (3) Quit Program          |\n";
    cout << " ----------------------------\n";
}

int main() {

    string path1, path2, instruction;

    bool in_use = true;
    while (in_use) {
        // wait for user's instruction
        displayMenu();
        cout << "> ";
        getline(cin,instruction);
        cout << endl;

        // run test cases
        if (instruction == "1") {
            runTestCases();
        }
        // run manual test
        else if (instruction == "2") {
            cout << "First file path: ";
            getline(cin, path1);
            cout << "Second file path: ";
            getline(cin, path2);

            vector<string> target_path = canonicalize(path1);
            vector<string> secret_path = canonicalize(path2);

            if (target_path.empty())
                cout << "PATH1 empty error";
            else if (secret_path.empty())
                cout << "PATH2 empty error";
            else if (areHomographs(target_path, secret_path))
                cout << "> The paths are homographs\n";
            else
                cout << "> The paths are not homographs\n";   
        }
        // quit program
        else if (instruction == "3") {
            cout << "> Have a great day!\n";
            break;
        }
        // invalid command
        else {
            cout << "> ## INVALID COMMAND ##\n";
        }
    }

    return 0;
}
