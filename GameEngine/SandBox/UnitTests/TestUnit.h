// TestUnit.h
#pragma once
#include "TimeTest.h"
#include "Benchmark.h"
#include "WorldTest.h"
#include "TestPhysics.h"
using namespace ac;
struct TestComponent {
    int value;
};

struct TestComponentB {
    float value;
};

struct TestResource {
    std::string name;
    int data;
};

struct TestEvent {
    int value;
};
PUBLIC_ALLOWTOKEN(TestEvent)

void TestCreateEntity();
void TestAddComponent();
void TestDeleteComponent();
void TestDeleteEntity();
void TestResourceSystem();
void TestSimpleView();
void TestRegisterEvent();
void TestAddListener();
void TestInvokeEvent();
void TestInput();
void StartTest();
