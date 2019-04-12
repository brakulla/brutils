/**
 * @file br_threaded_object_test_class_2.h
 *
 * Description
 */

#ifndef BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_2_H
#define BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_2_H

#include <brutils/br_object.hpp>
#include <string>

class TestClass2 : public brutils::br_threaded_object {
 public:
  TestClass2() : firstSignal(this), firstSlot(this), secondSlot(this), brutils::br_threaded_object() {
    std::cout << "TestClass2 thread id: " << this->getThreadId() << std::endl;
    firstSlot.setSlotFunction([=](std::string input) {
      std::cout << "Running slot from: " << std::this_thread::get_id() << std::endl;
      std::cout << "Received \"" << input << "\" now emitting from thread" << std::endl;
      emitWithinThread();
    });
    secondSlot.setSlotFunction([=](std::string parameter) {
      std::cout << "This is received from second slot: " << parameter << std::endl;
    });
  }
  ~TestClass2() override {
    std::cout << "Destructor2" << std::endl;
  };

  brutils::slot<std::string> firstSlot;
  brutils::slot<std::string> secondSlot;
  brutils::signal<std::string> firstSignal;

 private:
  void emitWithinThread() {
    firstSignal.connect(secondSlot);
    firstSignal.emit("This is emitted within thread");
  }
};

#endif //BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_2_H
