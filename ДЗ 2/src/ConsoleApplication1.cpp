// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <string>
#include <chrono>

int64_t MilliSecondsSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

std::vector<unsigned long long> SumOfColumn(unsigned N)
{
    if (N == 1)
    {
        return { 1,1,1,1,1,1,1,1,1,1 };
    }

    std::vector<unsigned long long> sum;

    if (N > 1)
    {
        auto sumOfColumn = SumOfColumn(N - 1);

    	std::vector<long long>::size_type sizeOfSum = sumOfColumn.size();
        std::vector<long long>::size_type cnt = 0;

    	unsigned start = 0;
        unsigned end = 0;

        while (start != (sizeOfSum-1))
        {
            unsigned long long s = 0;
            
            if (cnt > 9)
            {
                start++;
                end = cnt + 1;
            }
            else
                end = cnt+1;

            if (end >= sizeOfSum)
                end = sizeOfSum;

            for(unsigned i = start; i < end; ++i)
            {
                s += sumOfColumn[i];
            }
            sum.push_back(s);

            cnt++;
        }
    }

    return sum;
}

int main()
{
    int64_t t1 = MilliSecondsSinceEpoch();

    for(unsigned i = 0; i < 10; ++i)
    {
        const unsigned size = 11;
        char fileNameOutput[size]{0};

        snprintf(fileNameOutput, size, "test.%d.out", i);

        std::filesystem::path filePath(std::filesystem::current_path());
        filePath.append("Tickets");
        filePath.append(fileNameOutput);

    	std::ifstream outputStream(filePath.string().c_str(), std::ios::binary | std::ios::ate);
        if (!outputStream.is_open())
        {
            printf("Can't open file %s\n", filePath.string().c_str());
            return 0;
        }

        outputStream.seekg(0, std::ios::beg);
        std::string tempStr;
        std::getline(outputStream, tempStr);
        unsigned long long result = 0;
        
        if (sscanf_s(tempStr.c_str(), "%llu", &result) != 1)
        {
            printf("Can't scan result value from file %s\n", filePath.string().c_str());
            return 0;
        }

        auto t = SumOfColumn(i+1);
        unsigned long long sum = 0;
        std::for_each(t.cbegin(), t.cend(), [&](const unsigned long long& value)
            {
                sum += (value * value);
            });
        if (sum != result)
        {
            printf("Incorrect result (%llu) from file (%llu) %s\n", sum, result, filePath.string().c_str());
            return 0;
        }
    }

    int64_t t2 = MilliSecondsSinceEpoch();

    printf("Test succeeded. Elapsed time: %lld ms\n", t2-t1);
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
