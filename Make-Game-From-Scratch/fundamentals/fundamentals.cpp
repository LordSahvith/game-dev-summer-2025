#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using namespace std;

int main()
{
    char character1 = 'A'; // characters require a single quote
    char character2 = 'a';

    int age = 666;
    string name = "Lord Savith";
    string message = "Hello " + name;

    bool isLightOn = true; // booleans are either 0 or 1 - false (0) or true (1)

    float floatVar = 12.34567;
    double doubleVar = 12.345678901234567;
    long double longDouble = 12.345678901234567890;

    cout << character1 << endl;
    cout << character2 << endl;
    cout << age << endl;
    cout << message << " " << age << endl;
    cout << "is light on? " << isLightOn << endl;
    cout << "float: " << setprecision(8) << floatVar << endl;          // precise
    cout << "float: " << setprecision(10) << floatVar << endl;         // loses precision on floats
    cout << "double: " << setprecision(17) << doubleVar << endl;       // precise
    cout << "double: " << setprecision(20) << doubleVar << endl;       // loses precision on doubles
    cout << "long double: " << setprecision(20) << longDouble << endl; // same as double?
    cout << "long double: " << setprecision(24) << longDouble << endl; // cuz it's determined by system (x64, x86, etc.)

    // bit is the smallest - binary ( 0, 1 ) - 00100110
    // 1 bit = 1 bit
    // 1 byte = 8 bits (1 x 8)
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

    // arrays are a fixed length when initalized
    string names[] = {"lord savith", "bobby singer", "bruce wayne"};
    int namesSize = sizeof(names) / sizeof(string); // size of array isn't stored in memory

    cout << names[0] << endl; // lord savith
    cout << names[1] << endl; // bobby singer
    cout << names[2] << endl; // bruce wayne
    cout << names[3] << endl; // out of bounds - undefined behavior (seems to print out message variable)
    cout << "names array length: " << namesSize << endl;

    vector<int> IDVector = {1234, 1334, 1434};

    cout << IDVector[0] << endl;
    cout << IDVector[1] << endl;
    cout << IDVector[2] << endl;
    cout << IDVector.size() << endl; // 3 - size of vector is stored in memory
    cout << "last index of names: " << names[namesSize - 1] << endl;
    cout << "last index of IDVector: " << IDVector.back() << endl; // 1434

    // Vectors can grow even after initialization
    // grow - push_back(newItem)
    IDVector.push_back(1534);
    cout << IDVector.size() << endl;                                 // 4 - size of vector is stored in memory
    cout << "first index of IDVector: " << IDVector.front() << endl; // 1234
    cout << "last index of IDVector: " << IDVector.back() << endl;   // 1534

    // insert - insert(whereToInsert, whatToInsert)
    IDVector.insert(IDVector.begin(), 1134);
    cout << "first index of IDVector: " << IDVector.front() << endl; // 1134

    // remove - erase(whatToRemove)
    IDVector.erase(IDVector.begin());
    cout << "first index of IDVector: " << IDVector.front() << endl; // 1234

    // Map - map<keyType, valueType> {key, value}
    map<string, int> IDs{{"lord savith", 1234}, {"bobby singer", 1334}, {"bruce wayne", 1434}};
    cout << "access map by key: " << IDs["lord savith"] << endl; // 1234

    // update
    IDs["lord savith"] = 666;
    cout << "access map by key: " << IDs["lord savith"] << endl; // 666

    // insert - insert({key, value})
    IDs.insert({"kylar stern", 1534});
    cout << "inserted item: " << IDs["kylar stern"] << endl; // 1534

    // remove - erase(key)
    IDs.erase("kylar stern");
    // if key doesn't exist, then checking for key adds {"kylar stern", 0}
    cout << "removed item: " << IDs["kylar stern"] << endl; // 0

    // map size
    cout << "map size: " << IDs.size() << endl; // 4

    // correctly check map
    IDs.erase("kylar stern");
    bool itemExists = IDs.find("kylar stern") != IDs.end();
    cout << "does Kylar exist: " << itemExists << endl; // 0 (false)

    return 0;
}