// TestUnit.h
#pragma once

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

void TestCreateEntity();
void TestAddComponent();
void TestDeleteComponent();
void TestDeleteEntity();
void TestResourceSystem();
void TestSimpleView();
void TestRegisterEvent();
void TestAddListener();
void TestInvokeEvent();
void StartTest();
