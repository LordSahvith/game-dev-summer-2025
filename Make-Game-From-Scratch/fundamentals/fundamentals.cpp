#include <iostream>

int main()
{
    std::string name = "Lord Savith";
    std::string message = "Hello " + name;
    int age = 666;

    std::cout << message << " " << age << std::endl;
    return 0;
}