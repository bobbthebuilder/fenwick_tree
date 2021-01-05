#include "../fenwick_tree.hpp"

#include <vector>
#include <iostream>

int main()
{
    std::vector<int> array {1, 6, 2, 4, 3, 5};
    fenwick_tree<int> t(array.begin(), array.end());
    std::cout << "sum(0, 5)=" << t.sum() << "\n"; // 1 + 6 + 2 + 4 + 3 + 5 = 21
    std::cout << "sum(0, 3)=" << t.sum(3) << "\n"; // 1 + 6 + 2 + 4 = 13
    std::cout << "sum(3, 4)=" << t.sum(3, 4) << "\n"; // 4 + 3 = 7
    t.update(3, 3, -1); // equals array[3] += -1
    std::cout << "sum(3, 4)=" << t.sum(3, 4) << "\n"; // 3 + 3 = 6

    std::cout << t << "\n";
    return 0;	
}
