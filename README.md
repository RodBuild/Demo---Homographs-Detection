## Exploring Homograph Attacks
This project is about detecting if two given paths will lead to the same file while having different path symbols. If so, they would be called **Homographs**; if not, they would be called **No Homographs**. The base path is retrieved from the user's system and then used to evaluate if the two given paths lead to the same file.
In order for this program to work, the first path needs to properly target a fictious folder/file after base path. The first path can be assumed as the path for seeking prohibited files. The second path is the route to a sentitive file, which can be anywhere on the system.

_Example (lab05-2.cpp):_
```c++
/* Current path of user */
std::string BASE_PATH = "C:/Users/Git/Main/folder1/folder2";

/* Homographs */
std::string = "C:/Users/Git/Main/secret.txt";
std::string = "../../secret.txt";

/* Homographs */
std::string = "./secret/text.txt";
std::string = "../../folder1/folder2/secret/text.txt";

/* No Homographs */
std::string = "./../folder3/secret/text.txt";
std::string = "/secret/text.txt";
```

### Requires C++ 17
This program uses the _filesystem library_ for C++17, meaning that you need to compile your code on the version C++17 or above. Use following command to compile on C++17
``` c++
compile:    g++ -std=c++17 lab05-2.cpp -o lab05-2
run:        .\lab05-2
```
