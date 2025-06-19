// EventManagerTest.h
#pragma once
#include "Achoium.h"
using namespace ac;
// Test event types
struct BasicEvent {
    int value;
};
PUBLIC_ALLOWTOKEN(BasicEvent)

struct StringEvent {
    std::string message;
};
PUBLIC_ALLOWTOKEN(StringEvent)

struct ComplexEvent {
    int id;
    float x, y;
    std::string name;
};
PUBLIC_ALLOWTOKEN(ComplexEvent)

// Tests for EventManager
void TestEventRegistration();
void TestMultipleEventRegistration();
void TestDuplicateEventRegistration();
void TestEventListenerManagement();
void TestMultipleListenersForEvent();
void TestEventInvocation();
void TestMultipleEventInvocation();
void TestContainMethod();
void TestEventPriority();
void TestListenerRemoval();
void TestEdgeCases();
void RunAllEventManagerTests();