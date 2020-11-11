#include <iostream>
#include <Simulator.h>

void test()
{
    // IZMENITE SADRZAJ UNUTAR '{' I '}' DA BISTE POKRETALI ZELJENI SKUP TESTOVA
    int test_numbers[] = { 0, 1, 2, 3, 4, 5 };
    std::vector<int> test_vector(test_numbers, test_numbers + sizeof(test_numbers) / sizeof(int));
    try
    {
        Simulator simulator;
        for (int test_number : test_vector)
        {
            std::cout << "TEST BROJ " << test_number << std::endl;
            simulator.loadCircuit("test_" + std::to_string(test_number) + ".txt");
            simulator.simulate("test_" + std::to_string(test_number) + "_output.txt");
            std::cout << std::endl;
            std::cout << "-------------------------------------------------" << std::endl << std::endl;
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }   
}

int main()
{
    void test();
    return 0;
}