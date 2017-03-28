#include <iostream>
#include <math.h>
#include <assert.h>
#include <mutex>
#include <thread>
#include <vector>
#include <fstream>

using namespace std;

mutex mx;


float func_calculation(int m, float x1, float x2)
{
    float sum1 = 0;
    float sum2 = 0;
    float g;
    for (int i = 1; i <= m; ++i)
    {
        sum1 += i * cos((i + 1) * x1 + 1);
        sum2 += i * cos((i + 1) * x2 + 1);
    }

    g = - sum1 * sum2;

    std::cout << "Result for f(" << x1 << ", " << x2 << ") and m=" << m << " : " << g << std::endl;
    return g;
}

float integration(float x0, float x, float y0, float y, int m, float pr)
{
    assert (m >= 5);
    float sum = 0;
    float x_i = x0;
    mx.lock();
    while (x_i < x)
    {
        float y_i = y0;
        while (y_i < y)
        {
            sum += func_calculation(m, x_i + pr / 2.0, y_i + pr / 2.0) * (pr*pr);
            y_i += pr;
        }
        x_i += pr;

    }
    mx.unlock();
    std::cout << "Result: " << sum << std::endl;

    return sum;

}

vector<string> readConfig(string fileName) {
    ifstream fin(fileName);
    string line, l;



    size_t pos;
    vector<string> lst;
    string token, l1;
    string delimiter = "=";
    if (fin.is_open()) {
        while (getline(fin, line)) {
            pos = line.find(delimiter);
            l = line.substr(pos + 1);
            lst.push_back(l);
        }

//        for (size_t i =0; i < lst.size(); ++i){
//            cout << lst[i] << endl;
//        }
        fin.close();
        return lst;
    }
    else cout << "Error opening file";
}

int main()
{
//    readConfig("config.txt");
    clock_t t1, t2;
    t1 = clock();

    int num_of_threads;
    num_of_threads = atoi(readConfig("config.txt")[2].c_str());

    thread threads[num_of_threads];
    // choose random numbers   --   later: read from configuration file
    float x0 = atoi(readConfig("config.txt")[3].c_str());;
    float x = atoi(readConfig("config.txt")[4].c_str());;
    float y0 = atoi(readConfig("config.txt")[5].c_str());;
    float y = atoi(readConfig("config.txt")[6].c_str());;
    int m = atoi(readConfig("config.txt")[7].c_str());;
    float pr = 0.05;

    float x_step = (x - x0) / (pr * num_of_threads);    /// 10
    float x_start = x0;                                 /// 0

    float y_step = (y - y0) / (pr * num_of_threads);    /// 10
    float y_start = y0;                                 /// 0

    int i = 0;
    for (int i = 0; i < num_of_threads; ++i)
    {
        threads[i] = thread(integration, x_start, x_start + x_step, y_start, y_start + y_step, m, pr);
        x_start += x_step + pr;
        y_start += y_step + pr;
    }
    for (int j = 0; j < num_of_threads; ++j)
    {
        threads[j].join();
    }

    t2 = clock();
    string ms = "ms";
    cout << float((t2 - t1))/(CLOCKS_PER_SEC / 1000)<< ms  << endl;
    ofstream min_time;//write min time every time when program runs
    min_time.open("min_time.txt");
    min_time << float((t2 - t1))/(CLOCKS_PER_SEC / 1000) << endl;
//        std::cout << "----------------\n";
//        integration(0, 1, 0, 1, 5, 0.05);

    return 0;
}