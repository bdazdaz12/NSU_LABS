#include <iostream>
#include <gtest/gtest.h>
#include "Trit.h"
#include "TritSet.h"

using namespace std;

//TODO: cardinality, тесты

int main(int argc, char** argv) {
    TritSet a(123);
    a[16] = Trit::False;
    a.shrink();
    cout << a.size();
}

//int main(int argc, char** argv) {
//    return RUN_ALL_TESTS();
//}
