#include <iostream>
#include <filesystem>
#include <string>
#include <sstream>
#include <vector>
// #include <experimental/filesystem>
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
// namespace fs = std::experimental::filesystem;
// Symbols
const char SLASH = '/';
const std::string SINGLE_DOT = ".";
const std::string DOUBLE_DOT = "..";
const std::string DRIVES[2] = {"C:", "D:"};

vector<string> connectBase_Secret_Paths(const vector<string> b_path, const vector<string> s_path)
{
    vector<string> ultimate_path = b_path;
    int link; // this will help adding missing contents from secret path to base path
    bool done = false;

    // start at END of base path and at BEGIN of secret path
    for (int i = b_path.size() - 1; i >= 0; i--)
    {
        for (int j = 0; j < s_path.size(); j++)
        {
            // if they are the same, aka /CS453 == /CS453 - WE ARE DONE
            if (b_path[i] == s_path[j])
            {
                link = j;
                done = true;
                break;
            }
        }
        if (done)
            break;
    }
    // if common path was NOT found, we can assume
    // the user is using the same base folder
    if (!done)
    {
        /*  LINK everything together
            b_path = [  1   2   3   ]
            s_path = [  4   5   6   ]
            u_path = [  1   2   3   4   5   6   ]
        */
       for (auto it = s_path.begin(); it < s_path.end(); it++) {
           ultimate_path.push_back(*it);
       }
    }
    // if common path was found
    else if (done)
    {
        /*  LINK everything together
            b_path = [  1   2   3   ]
            s_path = [  3   4   5   ]
            u_path = [  1   2   3   4   5   ]
        */

        for (int i = link + 1; i < s_path.size(); i++)
        {
            ultimate_path.push_back(s_path[i]);
        }
    }

    return ultimate_path;
}

// Canoniaclization function returns... something...
// return the number of steps
// ./ -> same directory
// ../ -> one back
// /-> base directory
// C: or D: -> base
int canonicalize(const vector<string> v_path)
{
    int canonToken = 0;
    for (auto x : v_path)
    {
        // if same directory, do nothing
        if (x == SINGLE_DOT)
        {
            canonToken += 0;
        }
        // if going back to
        else if (x == DOUBLE_DOT)
        {
            canonToken -= 1;
        }
        else
        {
            canonToken += 1;
        }
        // cout << x << endl;
    }
    return canonToken;
}

// make a string BASE path into a vector
// make a string path into a vector path
void organizePath(const string path, vector<string> &v_path)
{
    string path_normal = path;
    if (path_normal[0] == '/')
        path_normal.erase(0,1);     // remove first letter, in case the paths start with '/' -> /test/test.txt
    // normalize path (if needed)
    for (int i = 0; i < path.size(); i++)
    {
        if (path_normal[i] == '\\')
        {
            path_normal[i] = SLASH;
        }
    }
    stringstream ss(path_normal);
    string dir;
    // vector<string> split_path;

    while (getline(ss, dir, SLASH))
    {
        v_path.push_back(dir);
    }
}

// first path is for regular path =
// second path is for forbidden path
bool manualTest()
{
    bool homographs = true;
    string hacker_path, secret_path;

    cout << "Specify the first filename: ";
    getline(cin, hacker_path);
    cout << "Specify the second filename: ";
    getline(cin, secret_path);

    /*
      Get user's path
    */
    std::filesystem::path system;
    system = std::filesystem::current_path();
    string current_path{system.u8string()};
    // string navigation_path = current_path;
    // current_path = std::filesystem::current_path();

    // cout << current_path << endl;

    vector<string> canon_hacker, canon_secret, canon_base;
    organizePath(hacker_path, canon_hacker);
    organizePath(secret_path, canon_secret);
    organizePath(current_path, canon_base);

    // examine path taken by the hacker and generate a token
    int hackerToken = canonicalize(canon_hacker);
    // connect base path with secret path - this way we know that the path exists
    vector<string> ultimate_path = connectBase_Secret_Paths(canon_base, canon_secret);

    
    //###########################################
    // #            DEBUGGING                   #
    //###########################################
    // cout << "token: " << hackerToken << endl;
    // //   range based loop
    // cout << "\n********************\n";
    // cout << "hacker:\n";
    // for (auto x: canon_hacker) {
    //     cout << x << endl;
    // }
    // cout << "\n********************\n";
    // cout << "ultimate:\n";
    // for (auto x: ultimate_path) {
    //     cout << x << endl;
    // }
    // cout << "\n********************\n";

    // Display paths -> for user's convenience
    cout << "\n\n**********************************\n";
    cout << "> Hacker's Path:\n";
    for (auto x: canon_hacker)
        cout << x << '/';
    cout << "\n\n> Secret Path:\n";
    for (auto x: ultimate_path)
        cout << x << '/';
    cout << "\n**********************************\n";


    // error checking
    if (hackerToken <= 0)
        return !homographs;
    // if same size, means nothing new was added -> they are not homographs (BASE DIRECTORY CANNOT BE A SECRET)
    if (ultimate_path.size() == canon_base.size())
        return !homographs;

    /*  NUMBER OF TOKENS = 2 (secret/text.txt)
        C:/users/john/cs453                     -> base path
        C:/users/john/cs453/secret/text.txt     -> ultimate_path
        ./../cs453/secret/text.txt              -> hacker_path
    */
    if (ultimate_path[(canon_base.size() - 1) + hackerToken] == canon_hacker[canon_hacker.size() - 1])
        return homographs;

    return !homographs; // in case something funny happens....
}

bool autoTest(string hacker_path, string secret_path) {
    //prepare variables
    bool homographs = true;
    std::filesystem::path system;
    system = std::filesystem::current_path();
    string current_path{system.u8string()};
    vector<string> canon_hacker, canon_secret, canon_base;
    organizePath(hacker_path, canon_hacker);
    organizePath(secret_path, canon_secret);
    organizePath(current_path, canon_base);

    // canonicalize and create secret path
    int hackerToken = canonicalize(canon_hacker);
    vector<string> ultimate_path = connectBase_Secret_Paths(canon_base, canon_secret);

    // define it strings are homographs
    if (hackerToken <= 0)
        return !homographs;
    if (ultimate_path.size() == canon_base.size())
        return !homographs;
    if (ultimate_path[(canon_base.size() - 1) + hackerToken] == canon_hacker[canon_hacker.size() - 1])
        return homographs;

    return !homographs;

}

void testCases() {
    string path1, path2;
    cout << "\n\nMost common case scenarios:\n  Feel free to manually test each of this scenarios,\n  try come up with your own!\n\n";
    
    // CASE 1
    path1 = "./secret/text.txt";
    path2 = "/secret/text.txt";
    cout << "# TEST CASE 1 - same directory #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

    // CASE 2
    path1 = "./../week 5/text.txt";
    path2 = "/text.txt";
    cout << "# TEST CASE 2 - previous directory #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

    // CASE 3
    path1 = "./../week 5/./text.txt";
    path2 = "../week 5/text.txt";
    cout << "# TEST CASE 3 - previous and same directory #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

    // CASE 4
    path1 = ".././week 5/secret/private/mystery/password.txt";
    path2 = "secret/private/mystery/password.txt";
    cout << "# TEST CASE 4 - extended path #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

     // CASE 5
    path1 = "../../../../../../../../../../system.txt";
    path2 = "/secret/system.txt";
    cout << "# TEST CASE 5 - out of scope directory #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

    // CASE 6
    path1 = "./../../Desktop/../../rodri/./Desktop/./week 5/password.txt";
    path2 = "/password.txt";
    cout << "# TEST CASE 6 - complex path #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

    // CASE 7
    path1 = "test.txt";
    path2 = "../../Desktop/week 5/test.txt";
    cout << "# TEST CASE 7 - EXTRA #\n";
    cout << "   BASE: " << "C:/Users/rodri/Desktop/week 5\n";
    cout << "   PATH 1: " << path1 << "\n";
    cout << "   PATH 2: " << path2 << "\n";
    std::cout << "   RESULTS:"
            << (autoTest(path1, path2) ? "The file paths are homographs" : "The file paths are not homographs")
            << std::endl << std::endl;

}

// Function used to display menu commands
void displayMenu()
{
    cout << "\n ----------------------------\n";
    cout << "|     Lab 05: Homographs     |\n";
    cout << "|----------------------------|\n";
    cout << "|  (1) Run Test Cases        |\n";
    cout << "|  (2) Manually Excute Test  |\n";
    cout << "|  (3) Quit Program          |\n";
    cout << " ----------------------------\n\n";
}

int main()
{
    // rules:
    // you can use ./ or blank at the start
    // ./ means same directory
    // ../ means go back
    // you can move
    string response;
    bool in_use = true;
    // if (manualTest())
    // {
    //     cout << "The paths are Homographss";
    // }
    // else
    // {
    //     cout << "The paths are NOT Homographss";
    // }
    // displayMenu();
    while (in_use)
    {
        displayMenu();
        cout << "> ";
        getline(cin, response);
        if (response == "1") {
            testCases();
        }
        else if (response == "2") {
            if (manualTest()) {
                cout << "*    The paths are Homographs    *";
                cout << "\n**********************************\n\n";
            }
            else {
                cout << "*  The paths are NOT Homographs  *";
                cout << "\n**********************************\n\n";
            }
        }
        else if (response == "3") {
            cout << "> Have a nice day!\n";
            break;
        }
        else {
            cout << "> ## INVALID COMMAND ##\n";
        }
    }
}