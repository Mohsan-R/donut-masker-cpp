#include "BatchProcessor.hpp"

#include <iostream>

int main()
{
    std::cout << "=========================================\n";
    std::cout << "       DONUT MASKER CPP\n";
    std::cout << "=========================================\n";

    BatchProcessor batch;

    batch.processFolder();

    return 0;
}