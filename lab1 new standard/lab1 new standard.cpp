#include <thread>
#include <chrono>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::thread;

static int const rangeOfArray = 20;
static int const numOfNegativeElem = 10;

void wait(int seconds)
{
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void worker(int time, int n, int* mass)
{
    int count = 0;
    int halfWayThrough = n / 2;

    cout << "Array:" << endl;
    for (int i = 0; i < n; i++)
    {
        cout << mass[i] << " ";
        if (mass[i]) {
            count++;
        }
        if (i == halfWayThrough)
        {
            wait(time);
        }
    }
    cout << endl;

    cout << "The number of non-zero elements = " << count << endl;
}

int main()
{
    int n;
    int* A;

    cout << "Input the size of the array." << endl;
    cin >> n;

    A = new int[n];
    srand((int)time(NULL));
    for (int i = 0; i < n; i++) {
        A[i] = (int)rand() % rangeOfArray - numOfNegativeElem;
    }

    int start, end;
    cout << "Input the time for starting the thread." << endl;
    cin >> start;
    cout << "Input the time for suspending the thread." << endl;
    cin >> end;

    wait(start);

    thread t(worker, end, n, A);
    t.join();

    delete[] A;
    return 0;
}