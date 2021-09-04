
# STL Pretty-Print [![license](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/MisaghM/STL-Pretty-Print/blob/main/LICENSE "Repository License")

## About

A simple pretty-printing library for C++.  
  
This header-only library offers a collection of templated `operator<<` overloads for standard containers, std::pair, std::tuple, and C-style arrays.  
  
Just include `prettyprint.hpp` and you can `ostream <<` containers.  
  
*Requires at least C++11.*

## Example Usage

```cpp
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <map>
#include <utility>
#include <tuple>

#include "prettyprint.hpp"

int main() {
    //There are 3 templated operator<< overloads in prettyprint.hpp
    //There is also a 'pprint' namespace which contains:
    //'pprint::detail': is_container and other metafunctions
    //'pprint::ptuple': tuple print function and an implementation of index_sequence

    //Standard containers:
    std::vector<int> vec {1, 2, 3, 4};
    std::cout << vec << '\n';
    //[1, 2, 3, 4]

    //Pair:
    std::pair<int, std::string> pair {12, "Test"};
    std::cout << pair << '\n';
    //(12, Test)

    //Tuple:
    std::tuple<int, float, char> tuple {1, 2.4, 'Z'};
    std::cout << tuple << '\n';
    //(1, 2.4, Z)

    //C-style array:
    int cstyle[4] {1, 2, 3, 4};
    std::cout << cstyle << '\n';
    //[1, 2, 3, 4]

    //Nested:
    std::map<char, std::array<int, 2>> nested {{'a', {1, 1}}, {'b', {2, 2}}};
    std::cout << nested;
    //[(a, [1, 1]), (b, [2, 2])]

    return 0;
}
```
