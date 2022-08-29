#include <iostream>
#include <thread>

int main()
{
    const auto processor_count = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << processor_count << std::endl;
    return 0;
}