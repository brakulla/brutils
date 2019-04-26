//
// Created by brakulla on 24.02.2019.
//
#include <iostream>
#include <thread>
#include <any>

#include <brutils/br_object.hpp>
#include <brutils/variant.h>

#include "br_threaded_object_test_class.h"
#include "br_threaded_object_test_class_2.h"
#include "br_object_test_class.h"

void testSlot(std::string const &input)
{
    std::cout << "This is slot input parameter: " << input << std::endl;
}

void recursiveVariantPrinter(std::any inputMap, int depth = 0)
{
    if (inputMap.type() == typeid(std::string)) {
        std::cout << depth << "This is string: " << std::any_cast<std::string>(inputMap) << std::endl;
    }
    else if (inputMap.type() == typeid(std::map<std::string, std::any>)) {
        std::cout << depth << "This is a map. Iterating over it." << std::endl;
        auto map = std::any_cast<std::map<std::string, std::any>>(inputMap);
        for (auto &item: map) {
            std::cout << item.first << std::endl;
            recursiveVariantPrinter(item.second, depth + 1);
        }
    }
}

void recursiveVarPrinter(brutils::variant var, int depth = 0)
{
    std::string tab;
    for (int i = 0; i < depth; ++i)
        tab.append("-");

    if (*var.getTypeInfo() == typeid(brutils::variant_map)) {
        std::cout << tab << "In map" << std::endl;
        brutils::variant_map varMap = var.toMap();
        for (auto &item: varMap) {
            std::cout << tab << "Key: " << item.first << std::endl;
            recursiveVarPrinter(item.second, depth + 1);
        }
    }
    else {
        std::cout << tab << "Value: " << var.toString() << std::endl;
    }
}

int main()
{
    // TEST 2
//    TestClass firstTestClass;
//    TestClass2 secondTestClass;
//
//    firstTestClass.firstSignal.connect(secondTestClass.firstSlot);
////  secondTestClass.firstSignal.connect(secondTestClass.firstSlot);
//
//    br_object_test_class thirdTestClass(&firstTestClass);
//    thirdTestClass.firstSignal.connect(secondTestClass.firstSlot);
//
//    firstTestClass.firstSignal.connect(thirdTestClass.firstSlot);
//
//    std::cout << "Doing some shit" << std::endl;
//    std::this_thread::sleep_for(std::chrono::seconds(3));
//    std::cout << "Emitting signal" << std::endl;
//    std::stringstream ss;
//    ss << "Sending some random shit within class within thread: " << std::this_thread::get_id();
//    firstTestClass.firstSignal.emit(ss.str());
//    std::cout << "Emitted" << std::endl;
//
//    std::this_thread::sleep_for(std::chrono::seconds(3));
//
//
//    std::cout << "Emitting thirdTestClass firstSignal" << std::endl;
//    thirdTestClass.firstSignal.emit("Tester 3");
//    std::this_thread::sleep_for(std::chrono::seconds(3));

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



    std::map<std::string, std::any> testMap;
    std::string testString;

    std::any mapRoot;
    std::map<std::string, std::any> anyMap;
    anyMap.insert(std::make_pair("DummyAStrKey", std::string("StrValue")));
    std::any innerMap = anyMap;
    anyMap.insert(std::make_pair("DummyInnerMapKey", innerMap));
    mapRoot = anyMap;

    recursiveVariantPrinter(mapRoot);

//    std::cout << "Type name: " << typeid(brutils::variant_map).name() << std::endl;
//    std::cout << "Type name: " << typeid(std::map<std::string, brutils::variant>).name() << std::endl;

    brutils::variant_map map;
    brutils::variant_map map1;
    brutils::variant_map map2;

    std::cout << "In here" << std::endl;
    map2["Dummy2"] = std::string("Dummy2Val");
    std::cout << "In here2" << std::endl;
    map1["Dummy1"] = std::string("Dummy1Val");
    std::cout << "In here3" << std::endl;
    map1["Dummy1Map"] = map2;
    std::cout << "In here4" << std::endl;
    map["Dummy"] = std::string("DummyVal");
    std::cout << "In here5" << std::endl;
    map["DummyMap"] = map1;
    std::cout << "In here6" << std::endl;

    recursiveVarPrinter(map);

    recursiveVarPrinter(map);


    std::cout << "Finished" << std::endl;
    return 0;
}