#include <iostream>
#include <iomanip>

using namespace std;

int main()
{
    string name = "Lord Savith";
    string message = "Hello " + name;
    int age = 666;

    char character1 = 'A'; // characters require a single quote
    char character2 = 'a';

    bool isLightOn = true; // booleans are either 0 or 1 - false (0) or true (1)

    float floatVar = 12.34567;
    double doubleVar = 12.345678901234567;
    long double longDouble = 12.345678901234567890;

    cout << message << " " << age << endl;
    cout << character1 << endl;
    cout << character2 << endl;
    cout << "is light on? " << isLightOn << endl;
    cout << "float: " << setprecision(8) << floatVar << endl;     // precise
    cout << "float: " << setprecision(10) << floatVar << endl;    // loses precision on floats
    cout << "double: " << setprecision(17) << doubleVar << endl;  // precise
    cout << "double: " << setprecision(20) << doubleVar << endl;  // loses precision on doubles
    cout << "double: " << setprecision(20) << longDouble << endl; // same as double?
    cout << "double: " << setprecision(24) << longDouble << endl; // cuz it's determined by system (x64, x86, etc.)

    // bit is the smallest - binary ( 0, 1 )  - 00100110
    // 1 bit = 1 bit
    // 1 byte = 8 bits
    // 1 KB = 1024 bytes (1024 x 8)
    // 1 MB = 1024 KB (1,048,576 x 8)
    // 1 GB = 1024 MB (1,073,741,824 x 8)
    // 1 TB = 1024 GB (1,099,511,627,776 x 8)
    // 1 PB = 1024 TB (1,125,899,906,842,624 x 8)
    // ...
    cout << "the size of a char is: " << sizeof(char) << " byte" << endl;                 // 1 byte - 00011001 (1 x 8)
    cout << "the size of a boolean is: " << sizeof(bool) << " byte" << endl;              // 1 byte - 00011001 (1 x 8)
    cout << "the size of an int is: " << sizeof(int) << " bytes" << endl;                 // 4 bytes - 4 x 8
    cout << "the size of a float is: " << sizeof(float) << " bytes" << endl;              // 4 bytes - 4 x 8
    cout << "the size of a double is: " << sizeof(double) << " bytes" << endl;            // 8 bytes - 8 x 8
    cout << "the size of a long double is: " << sizeof(long double) << " bytes" << endl;  // 16 bytes - 16 x 8
    cout << "the size of a string is: " << sizeof(string) << " bytes" << endl;            // 32 bytes - 32 x 8
    cout << "the size of a message variable is: " << sizeof(message) << " bytes" << endl; // 32 bytes - 32 x 8

    return 0;
}