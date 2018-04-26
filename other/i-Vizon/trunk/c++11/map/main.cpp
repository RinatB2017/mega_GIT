// std::map example
// opposite words

#include <iostream>
#include <map>
#include <string>
using namespace std;

typedef std::map<std::string, int> TStrStrMap;
typedef std::pair<std::string, int> TStrStrPair;

int main(int argc, char *argv[])
{
    TStrStrMap tMap;

    tMap.insert(TStrStrPair("one", 1));
    tMap.insert(TStrStrPair("two", 2));
    tMap.insert(TStrStrPair("three", 3));
    tMap.insert(TStrStrPair("four", 4));

    std::string s;
    std::cout << "Enter word: ";
    std::cin >> s;


    int strValue = tMap[s];
    if(strValue!= 0)
    {
        // Show value
        std::cout << "Numeric: " << strValue << endl;
    }
    else
    {
        std::cout << "Word not in map." << std::endl;
//        TStrStrMap::iterator p;
//        bool bFound=false;
//        // Show key
//        for(p = tMap.begin(); p!=tMap.end(); ++p)
//        {
//            std::string strKey;
//            strValue = s;
//            strKey= p->second;
//            if( strValue  == strKey)
//            {
//                // Return key
//                std::cout << "Opposite: " << p->first << std::endl;
//                bFound = true;
//            }
//        }
//        // If not found opposite word
//        if(!bFound)
//        {
//            std::cout << "Word not in map." << std::endl;
//        }
    }
    return 0;
}
