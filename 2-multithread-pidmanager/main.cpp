/**
 * Driver to test PidManager functionalities
 *
 * @file main.cpp
 * @author Ricky Chon
 */

#include <iostream>
#include <thread>
#include <chrono>
#include "PidManager.h"
#include "Diagnostics.h"
#include "ThreadConstants.h"

void success_all();
void fail_pid_allocate();
void fail_pid_release();
void multithread_test();

PidManager manager;
Diagnostics d;

int main()
{
    int choice;

    printf("0. Test all successful\n");
    printf("1. Test failed PID allocation\n");
    printf("2. Test failed PID release\n");
    printf("3. Test multi-threading support\n");
    printf("Pick a test to run: ");
    std::cin >> choice;

    switch(choice)
    {
        case 0:
            success_all();
            break;
        case 1:
            fail_pid_allocate();
            break;
        case 2:
            fail_pid_release();
            break;
        case 3:
            multithread_test();
            break;
        default:
            break;
    }

    return 0;
}

void success_all()
{
    printf("\n====================\nTest Successful Routes\n====================\n");

    printf("\nAllocate map for use\n");
    d.output_result(PidAction::MapAllocation, manager.allocate_map());

    printf("\nAllocate a PID to be used\n");
    int PID = manager.allocate_pid();
    d.output_result(PidAction::PidAllocation, PID);

    printf("\nRelease the first PID\n");
    manager.release_pid(PID);
    d.output_result(PidAction::PidRelease, PID);

    // Allocate PID 4000 times
    for(int i = 0; i < 4000; i++)
    {
        manager.allocate_pid();
    }

    printf("\nRelease PID 3456\n");
    manager.release_pid(3456);
    d.output_result(PidAction::PidRelease, 3456);

    printf("\nAllocate a PID again to fill up the first vacant spot\n");
    int PID3 = manager.allocate_pid();
    d.output_result(PidAction::PidAllocation, PID3);
}

void fail_pid_allocate()
{
    printf("\n====================\nFailed PID Allocation\n====================\n");

    printf("\nAllocate map for use\n");
    d.output_result(PidAction::MapAllocation, manager.allocate_map());

    // Allocate PID 4699 times
    for(int i = 0; i < 4699; i++)
    {
        manager.allocate_pid();
    }

    printf("\nAllocate the final PID\n");
    int PID = manager.allocate_pid();
    d.output_result(PidAction::PidAllocation, PID);

    printf("\nTry to allocate a PID again\n");
    int PID2 = manager.allocate_pid();
    d.output_result(PidAction::PidAllocation, PID2);
}

void fail_pid_release()
{
    printf("\n====================\nFailed PID Release\n====================\n");

    printf("\nAllocate map for use\n");
    d.output_result(PidAction::MapAllocation, manager.allocate_map());

    printf("\nTry to release a PID that is out of range\n");
    manager.release_pid(7500);
    d.output_result(PidAction::PidRelease, 7500);
}

void exec_process(int t_id)
{
    // Random sleep time in the range of 500ms and 5000ms
    int sleep_time = rand() % 4500 + 500;

    printf("THREAD %d: Started.\n", t_id);
    printf("THREAD %d: Requesting PID\n", t_id);
    int PID = manager.allocate_pid();
    d.output_result(PidAction::PidAllocation, PID);

    printf("THREAD %d: Sleeping for %dms.\n", t_id, sleep_time);
    std::this_thread::sleep_for(std::chrono::milliseconds(sleep_time));

    printf("THREAD %d: Releasing PID %d\n", t_id, PID);
    manager.release_pid(PID);
    d.output_result(PidAction::PidRelease, PID);
    printf("THREAD %d: Ended.\n", t_id);
}

void multithread_test()
{
    std::thread threads[NUM_THREADS];

    printf("\n====================\nMulti-Thread Test\n====================\n");

    printf("\nAllocate PID map for use\n");
    d.output_result(PidAction::MapAllocation, manager.allocate_map());

    // Create a bunch of threads execute a single process in each
    for(int i = 0; i < NUM_THREADS; i++)
    {
        printf("MAIN: Created thread %d.\n", i);

        // Now we simulate a single process on the thread
        // and store it in an array for later use
        threads[i] = std::thread(exec_process, i);
    }

    printf("Spawning %d threads and waiting for them to join.\n", NUM_THREADS);

    // Sync all the threads
    for(int i = 0; i < NUM_THREADS; i++)
    {
        printf("MAIN: Joined thread %d.\n", i);
        threads[i].join();
    }

    printf("All threads joined!\n");
}