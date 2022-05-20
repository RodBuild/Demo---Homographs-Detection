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
const char DRIVES[2] = {'C', 'D'};


/*************************************
 * Determine if paths are homographs *
 *************************************/
bool areHomographs(const vector<string> target_path, const vector<string> secret_path)
{
    bool homographs = true;
    for (int i = 0; i < secret_path.size(); i++)
    {
        if (target_path[i] != secret_path[i])
        {
            homographs = false;
            break;
        }
    }
    return homographs;
}

/***************************************
 * Function to return user's base path *
 ***************************************/
string getBasePath() {
    std::filesystem::path system;
    system = std::filesystem::current_path();
    string base_path{system.u8string()};
    // normalize base path
    for (int i = 0; i < base_path.size(); i++)
        if (base_path[i] == '\\')
            base_path[i] = SLASH;
    
    return base_path;
}

/****************************************
 * Function to split string into vector *
 ****************************************/
vector<string> vectorizeString(const string path)
{
    vector<string> vectorString;
    stringstream ss(path);
    string value;
    while (getline(ss, value, SLASH)) {
        vectorString.push_back(value);
    }
    return vectorString;
}

/****************************************
 * Create a canon token of a given path *
 ****************************************/
vector<string> canonicalize(const std::string p, const std::string base_path)
{
    string path = p;
    vector<string> vbase_path, vgiven_path;
    
    /*  Full path is given - starts with C/D drive
            GOOD: C:/User/Rodri/Test    */
    if (path.length() > 1 && (path[0] == DRIVES[0] || path[0] == DRIVES[1]) && path[1] == ':')
    {
        vbase_path = vectorizeString(path);
        // return vbase_path;

    }
    /*  Full path is given - invalid cases
            ERROR:    /C:/User    ./C:/User    ../C:/User    */   
    else if (path.length() > 3 && ((path[0] == '/' && (path[1] == DRIVES[0] || path[1] == DRIVES[1]) && path[2] == ':') 
                                || (path[0] == '.' && path[1] == '/' && (path[2] == DRIVES[0] || path[2] == DRIVES[1]) && path[3] == ':')
                                || (path[0] == '.' && path[1] == '.' && (path[3] == DRIVES[0] || path[3] == DRIVES[1]) && path[3] == ':')))
    {
        vbase_path.push_back("WRONG_PATH");
        // return vbase_path;
    }
    /*  NOT a Full path - moves around base path
            GOOD: ../Test/secret.txt  */
    else {
        // If there is a '/' at the beginning of the path
        if (path[0] == '/')
        {
            path.erase(0, 1);
        }   
        
        // split string paths into vectors
        vbase_path = vectorizeString(base_path);
        vgiven_path = vectorizeString(path);
            
        //use given path to navigate base path
        for (auto it = vgiven_path.begin(); it != vgiven_path.end(); it++)
        {
            if (*it == SINGLE_DOT)
            {
                // do nothing
            }
            else if (*it == DOUBLE_DOT)
            {
                vbase_path.pop_back();
            }
            else
            {
                vbase_path.push_back(*it);
            }

            // if base path empty at some point -> return the vector to show error
            if (vbase_path.empty())
            {
                return vbase_path;
            }
        }
    
    }
    /*DEBUG
        CASE 1: User gave a full path (given path)
        CASE 2: User is moving around base path (given path + base path) */
    // for (auto x: vbase_path) {
    //     cout << x << "/";
    // }
    // cout << endl;
    /**/

    return vbase_path;
}

/************************************
 * Compare paths and output results *
 ************************************/
void runHomographFunction(const string path1, const string path2, const string base_path = getBasePath())
{
    vector<string> target_path = canonicalize(path1, base_path);
    vector<string> secret_path = canonicalize(path2, base_path);

    if (target_path.empty())
        cout << "PATH1 empty error";
    else if (secret_path.empty())
        cout << "PATH2 empty error";
    else if (areHomographs(target_path, secret_path))
        cout << "> The paths are homographs\n";
    else
        cout << "> The paths are not homographs\n";
}

/*************************
 * Run example test case *
 *************************/
void runTestCases()
{
    string path1, path2, current_path = "C:users/home/secret";
    
    cout << "Base path: " << current_path << endl << endl;

    cout << "# CASE 1: Same directory #" << endl;
    path1 = "C:users/home/secret/password.txt";
    path2 = "./password.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are homographs\n\n";

    cout << "# CASE 2: Different directory #" << endl;
    path1 = "C:users/home/secret/password.txt";
    path2 = "../password.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are not homographs\n\n";

    cout << "# CASE 3: Back track to different directory #" << endl;
    path1 = "C:users/home/secret/password.txt";
    path2 = "./../secret/password.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are homographs\n\n";

    cout << "# CASE 4: Different files #" << endl;
    path1 = "C:users/home/secret/password.txt";
    path2 = "./username.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are not homographs\n\n";

    cout << "# CASE 5: Non absolute path #" << endl;
    path1 = "./password.txt";
    path2 = "./../secret/password.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are homographs\n\n";

    cout << "# CASE 6: Non absolute path | different directory #" << endl;
    path1 = "./password.txt";
    path2 = "./../private/password.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are not homographs\n\n";

    cout << "# CASE 7: Invalid Input #" << endl;
    path1 = "./C:/users/home/secret/password.txt";
    path2 = "./username.txt";
    cout << "   path 1: " << path1 << endl;
    cout << "   path 2: " << path2 << endl;
    runHomographFunction(path1, path2, current_path);
    cout << "Expected: The paths are not homographs\n\n";
}

/************************
 * Display Instructions *
 ************************/
void displayMenu()
{
    cout << "\n ----------------------------\n";
    cout << "|     Lab 05: Homographs     |\n";
    cout << "|----------------------------|\n";
    cout << "|  (1) Run Test Cases        |\n";
    cout << "|  (2) Manually Execute Test |\n";
    cout << "|  (3) Quit Program          |\n";
    cout << " ----------------------------\n";
}

int main()
{

    string path1, path2, instruction;

    bool in_use = true;
    while (in_use)
    {
        // wait for user's instruction
        displayMenu();
        cout << "> ";
        getline(cin, instruction);
        cout << endl;

        // run test cases
        if (instruction == "1")
        {
            runTestCases();
        }
        // run manual test
        else if (instruction == "2")
        {
            cout << "Your current path: " << getBasePath() << endl;
            cout << "First file path: ";
            getline(cin, path1);
            cout << "Second file path: ";
            getline(cin, path2);

            runHomographFunction(path1, path2);
        }
        // quit program
        else if (instruction == "3")
        {
            cout << "> Have a great day!\n";
            break;
        }
        // invalid command
        else
        {
            cout << "> ## INVALID COMMAND ##\n";
        }
    }

    return 0;
}
