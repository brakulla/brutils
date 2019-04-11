/**
 * @file br_threaded_object_test_class.h
 *
 * Description
 */

#ifndef BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H
#define BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H

#include <brutils/br_object.hpp>

class TestClass : public brutils::br_threaded_object {
 public:
  TestClass() : brutils::br_threaded_object() {}
  ~TestClass() override {
    std::cout << "Destructor" << std::endl;
  };

  brutils::signal<std::string> firstSignal;
  brutils::signal<int> secondSignal;
};

#endif //BRUTILS_BR_THREADED_OBJECT_TEST_CLASS_H
