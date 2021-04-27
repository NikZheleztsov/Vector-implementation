#include "vector.h"
#include <iostream>
#include <algorithm>

int main ()
{
    Vector<int> v_0 {1, 2, 3, 4, 5};
    std::cout << "v_0 (init with ilist): \n";
    for (auto x : v_0)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::cout << "It's size is " << v_0.size() << std::endl << std::endl;

    Vector<int> v_1 (v_0);
    v_1.at(0) = 10;
    v_1[1] = 20;
    std::cout << "v_1 (copy of v_0 with at(0) = 10 and [1] = 20): \n";
    for (auto x : v_1)
        std::cout << x << ' ';
    std::cout << std::endl << std::endl;

    Vector<int> v_2 (std::move(v_1));
    v_2.pop_back();
    v_2.push_back(100);
    std::cout << "v_2 (move v_1 with pop_back() and push_back(100)): \n";
    for (auto x : v_2)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::cout << "It's size is " << v_2.size() << std::endl << std::endl;

    Vector<int> v_3 (100);
    std::cout << "v_3 capacity: " << v_3.capacity() << std::endl;
    std::cout << "v_3 max_size(): " << v_3.max_size() << std::endl;
    auto it_1 = v_0.begin(), it_2 = it_1 + 3;
    // [first, last) !
    v_3.insert(0, it_1, it_2);
    std::cout << "v_3 (insert (iterators)): \n";
    for (auto x : v_3)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::cout << "It's size is " << v_3.size() << std::endl;

    v_3.shrink_to_fit();
    std::cout << "Capacity after shrinking: " << v_3.capacity() << std::endl;
    auto it_3 = v_3.begin() + 1;
    v_3.insert (it_3, 100);
    std::cout << "v_3.insert ('before 2', 100): \n";
    for (auto x : v_3)
        std::cout << x << ' ';
    std::cout << std::endl;

    std::cout << "v_3.resize(2): \n";
    v_3.resize(2);
    for (auto x : v_3)
        std::cout << x << ' ';
    std::cout << std::endl;

    std::cout << "v_3.swap(v_0): \n";
    v_3.swap(v_0);
    for (auto x : v_3)
        std::cout << x << ' ';
    std::cout << std::endl;

    v_3.insert(v_3.begin() + 1, 2, 25);
    for (auto x : v_3)
        std::cout << x << ' ';
    std::cout << std::endl;

    v_3.resize(0);
    v_3.reserve(20);
    if (v_3.empty())
    {
        std::cout << "v_3 size: " << v_3.size() << std::endl;
        std::cout << "v_3 cap: " << v_3.capacity() << std::endl << std::endl;
    }
    
    Vector<char> vec_4 {'a', 'b', 'c'};
    for (auto x : vec_4)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::cout << "vec_4 front: " << vec_4.front() << std::endl;
    std::cout << "vec_4 back: " << vec_4.back() << std::endl;
    vec_4.erase(vec_4.begin());
    for (auto x : vec_4)
        std::cout << x << ' ';
    std::cout << std::endl;

    auto it_4 = vec_4.insert(vec_4.begin() + 1, {'d', 'f', 'g'});
    for (auto x : vec_4)
        std::cout << x << ' ';
    std::cout << std::endl;

    auto data = vec_4.data();
    for (int i = 0; i < vec_4.size(); i++)
        std::cout << data[i] << ' ';
    std::cout << std::endl;

    auto it_5 = vec_4.end();
    vec_4.erase(it_4, it_5);
    for (auto x : vec_4)
        std::cout << x << ' ';
    std::cout << std::endl;

    vec_4.clear();
    if (vec_4.empty())
        std::cout << "vec_4 is empty after clear()\n\n";

    /////////////////////////////////////////////////////////
    
    Vector<int> vec;
    for (int i = 0; i < 10; i++)
        vec.push_back(i);

    std::cout << "Working with STL algorithms: \n";
    std::for_each(vec.begin(), vec.end(), [] (auto x) { std::cout << x << ' ';});
    std::cout << std::endl;
    std::cout << "Pos of '5': " << std::find(vec.begin(), vec.end(), 5) - vec.begin() << std::endl;
    std::sort(vec.begin(), vec.end(), std::greater<int>());
    for (auto x : vec)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::cout << "Num of elements < 5: " << std::count_if(vec.begin(), vec.end(), [] (auto x) {
            return x<5;});
    std::cout << std::endl;
    std::reverse(vec.begin(), vec.end());
    for (auto x : vec)
        std::cout << x << ' ';
    std::cout << std::endl;
    std::swap(*std::max_element(vec.begin(), vec.end()), *std::min_element(vec.begin(), vec.end()));
    for (auto x : vec)
        std::cout << x << ' ';
    std::cout << std::endl;

    return 0;
}
