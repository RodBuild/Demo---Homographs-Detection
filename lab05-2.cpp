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

vector<string> canonicalize(const std::string p) {
    // delete first '/'
    string path = p;
    if (path[0] == '/') {
        path.erase(0,1);
    }

    vector<string> v_path, vb_path;
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
    stringstream ss(base_path);
    string dir;
    while (getline(ss, dir, SLASH))
        vb_path.push_back(dir);

    //normalize given path
    stringstream ss2(path);
    string dir2;
    while (getline(ss2, dir2, SLASH))
        v_path.push_back(dir2);
    
    
    //now cannonlize Union of base with path
    // for (auto x: v_path)
        // cout << x << endl;

    // int location = vb_path.size() - 1;
    for (auto it = v_path.begin(); it != v_path.end(); it++) {
        if (*it == SINGLE_DOT) {
            //do nothing
        }
        else if (*it == DOUBLE_DOT) {
            vb_path.pop_back();
        }
        else {
            vb_path.push_back(*it);
        }

        //if empty at some point -> return the vector to show error
        if (vb_path.empty()) {
            return vb_path;
        }
    }

    for (auto x: vb_path)
        cout << x << "/";

    cout << endl;

    return vb_path;
}

int main() {
    string path1, path2;
    cout << "Path 1: ";
    getline(cin, path1);
    cout << "Path 2: ";
    getline(cin, path2);
    // std::filesystem::path system;
    // system = std::filesystem::current_path();
    // string base_path{system.u8string()};

    vector<string> target_path = canonicalize(path1);
    vector<string> secret_path = canonicalize(path2);

    if (target_path.empty())
        cout << "PATH1 empty error";

    if (secret_path.empty())
        cout << "PATH2 empty error";

    bool homographs = true;
    for (int i = 0; i < secret_path.size(); i++) {
        if (target_path[i] != secret_path[i]) {
            homographs = false;
            break;
        }
     }
     if (homographs)
        cout << "Homographs";
    else if (!homographs)
        cout << "No homographs";


    return 0;
}
