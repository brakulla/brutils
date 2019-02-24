//
// Created by brakulla on 24.02.2019.
//
#include <iostream>
#include <brutils/Signal.hpp>
#include <thread>

void testSlot(std::string const input)
{
    std::cout << "This is slot input parameter: " << input << std::endl;
}
int main()
{
    std::cout << "Hello, World!" << std::endl;

    brutils::Signal<std::string> signal;
    int firstConnection = signal.connect(testSlot);
    std::cout << "Connected" << std::endl;

    std::cout << "Doing some shit" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    signal.emit("Some random shit");

    signal.connect(testSlot);
    std::cout << "Second connection" << std::endl;

    std::cout << "Doing some more shit" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    signal.emit("Some more shit");

    signal.disconnect(firstConnection);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    signal.emit("Second shit");

    std::cout << "Some shit is done!" << std::endl;

    return 0;
}