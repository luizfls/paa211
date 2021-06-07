#include "../tree/avl.h"

#include <iostream>

int main(int argc, char* argv[])
{
    tree::avl<int> t;

    auto il = {10, 5, 20, 0, 15, 25};
    for(auto i : il)
        t.insert(i);

    std::cout << t.remove(0) << "\n";
    std::cout << t.remove(20) << "\n";
    t.insert(2);
    std::cout << t.remove(15) << "\n";

    std::cout << t << "\n";

    return 0;
}
