/**
 * @file br_object_test_class.h
 *
 * Description
 */

#ifndef BR_OBJECT_TEST_CLASS_H
#define BR_OBJECT_TEST_CLASS_H

#include "brutils/br_object.hpp"

class br_object_test_class: brutils::br_object
{
public:
    br_object_test_class(br_object *parent)
        : br_object(parent), firstSignal(this), firstSlot(this)
    {
        firstSlot.setSlotFunction([](std::string parameter)
                                  {
                                      std::cout << "Received this on br_object slot: " << parameter << std::endl;
                                  });
    }
    brutils::signal<std::string> firstSignal;
    brutils::slot<std::string> firstSlot;
};

#endif //BR_OBJECT_TEST_CLASS_H
