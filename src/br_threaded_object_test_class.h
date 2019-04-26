/**
 * @file br_threaded_object_test_class.h
 *
 * Description
 */

#ifndef BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H
#define BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H

#include <br_object.hpp>

class TestClass : public brutils::br_threaded_object {
 public:
  TestClass() : firstSignal(this), secondSignal(this), brutils::br_threaded_object() {
    std::cout << "TestClass thread id: " << this->getThreadId() << std::endl;
  }
  ~TestClass() override {
    std::cout << "Destructor" << std::endl;
  };

  brutils::signal<std::string> firstSignal;
  brutils::signal<int> secondSignal;
};

#endif //BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H
