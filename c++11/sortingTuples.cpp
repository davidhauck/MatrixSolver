#include <tuple>
#include <iostream>
#include <complex>
#include <list>
#include <random>
#include <chrono>



/* 
   This code sorts a list of tuples<int, int, int> fairly efficiently
   and quickly. I've tested in when N = 1,000,000 -- so let me know
   what you think.

   I made a comparison method which is decently quick. 

*/

bool myComparison(const std::tuple<int, int, int>& first, const std::tuple<int, int, int>& second) {
    int firstVar = std::get<0> (first), secondVar = std::get<0> (second);
    if (firstVar < secondVar) {
        return true;
    }
    else if (firstVar > secondVar) {
        return false;
    } else {
        return (std::get<1> (first) < std::get<1> (second));
    }
}

int main() {

    int N = 1000000;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::minstd_rand0 generator (seed); // minstd_rand0 is a standard linear_congruential_engine
    // generator() will now make a random number...

    std::list<std::tuple<int, int, int>> mylist;



    for (int i = 0; i < N; ++i) {
        mylist.push_back(std::tuple<int, int, int> (generator(), generator(), generator()));
    }

    // ----------- Algorithm for sorting.. timing
    auto t1 = std::chrono::high_resolution_clock::now();

    mylist.sort(myComparison);

    auto t2 = std::chrono::high_resolution_clock::now();

    // ----------- End timing for sorting

    int count = 0;

    for (std::list<std::tuple<int, int, int>>::iterator it = mylist.begin(); it != mylist.end() && count < 15; ++it) {
        count++;
        std::cout << '[' << std::get<0>(*it) << ", " << std::get<1>(*it) << ']' << ": " << std::get<2>(*it) << std::endl;;
    }


    std::cout << "\n\nTime taken: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count() << std::endl;




}
