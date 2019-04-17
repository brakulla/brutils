//
// Created by brakulla on 24.02.2019.
//
#include <iostream>
#include <thread>

#include <brutils/br_object.hpp>

#include "br_threaded_object_test_class.h"
#include "br_threaded_object_test_class_2.h"

void testSlot(std::string const &input) {
  std::cout << "This is slot input parameter: " << input << std::endl;
}
int main() {
  // TEST 2
  TestClass firstTestClass;
  TestClass2 secondTestClass;

  firstTestClass.firstSignal.connect(secondTestClass.firstSlot);
//  secondTestClass.firstSignal.connect(secondTestClass.firstSlot);

  std::cout << "Doing some shit" << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(3));
  std::cout << "Emitting signal" << std::endl;
    std::stringstream ss;
    ss << "Sending some random shit within class within thread: " << std::this_thread::get_id();
    firstTestClass.firstSignal.emit(ss.str());
  std::cout << "Emitted" << std::endl;

  std::this_thread::sleep_for(std::chrono::seconds(3));
//  std::cout << "Doing second shit" << std::endl;
//  std::this_thread::sleep_for(std::chrono::seconds(1));
//  std::cout << "Emitting second signal" << std::endl;
//  secondTestClass.firstSignal.emit("Sending some second shit within class");
//  std::cout << "Emitted second signal" << std::endl;

  // TEST 1
//  std::cout << "Hello, World!" << std::endl;
//
//  brutils::slot<std::string> slot(testSlot);
//  std::cout << "Created slot" << std::endl;
//
//  brutils::signal<std::string> signal;
//  int firstConnection = signal.connect(slot, brutils::ConnectionType::Direct);
//  std::cout << "Connected" << firstConnection << std::endl;
//
//  std::cout << "Doing some shit" << std::endl;
//  std::this_thread::sleep_for(std::chrono::seconds(3));
//  signal.emit("Sending some random shit");
//
//  brutils::slot<std::string> slot2(testSlot);
//  std::cout << "Created second slot" << std::endl;
//  int secondConnection = signal.connect(slot2, brutils::ConnectionType::Direct);
//  std::cout << "Second connection" << secondConnection << std::endl;
//
//  std::cout << "Doing some more shit" << std::endl;
//  std::this_thread::sleep_for(std::chrono::seconds(3));
//  signal.emit("Some more shit");
//
//  signal.disconnect(firstConnection);
//
//  std::this_thread::sleep_for(std::chrono::seconds(1));
//  signal.emit("Second shit");
//
//  std::cout << "Some shit is done!" << std::endl;

  std::cout << "Finished" << std::endl;
  return 0;
}