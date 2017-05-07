#include <iostream>      
#include <string>
#include <cstdlib>
#include <cstring>

int main (int argc, const char * argv[]) {
    char* data = (char*)calloc(10, sizeof(char));
    char arr[10] = "hello mr!";
    std::memcpy(data, arr, 10 * sizeof(char));
    std::string str(data);
    std::cout << str << std::endl;
    return 0;
}
