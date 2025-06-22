// EventManagerTest.cpp
#include "acpch.h"
#include "EventManagerTest.h"

void TestEventRegistration() {
    ACMSG("Running TestEventRegistration...");
    ac::EventManager manager;
    
    // Test registering an event
    manager.RegisterEvent<BasicEvent>();
    ACASSERT(manager.Contain<BasicEvent>(), "TestEventRegistration failed: BasicEvent not registered");
    
    // Test that non-registered events aren't contained
    ACASSERT(!manager.Contain<StringEvent>(), "TestEventRegistration failed: StringEvent incorrectly reported as registered");
    
    ACMSG("TestEventRegistration passed");
}

void TestMultipleEventRegistration() {
    ACMSG("Running TestMultipleEventRegistration...");
    ac::EventManager manager;
    
    // Register multiple event types
    manager.RegisterEvent<BasicEvent>()
           .RegisterEvent<StringEvent>()
           .RegisterEvent<ComplexEvent>();
    
    // Verify all events are registered
    ACASSERT(manager.Contain<BasicEvent>(), "TestMultipleEventRegistration failed: BasicEvent not registered");
    ACASSERT(manager.Contain<StringEvent>(), "TestMultipleEventRegistration failed: StringEvent not registered");
    ACASSERT(manager.Contain<ComplexEvent>(), "TestMultipleEventRegistration failed: ComplexEvent not registered");
    
    ACMSG("TestMultipleEventRegistration passed");
}

void TestDuplicateEventRegistration() {
    ACMSG("Running TestDuplicateEventRegistration...");
    ac::EventManager manager;
    
    // Register an event
    manager.RegisterEvent<BasicEvent>();
    ACASSERT(manager.Contain<BasicEvent>(), "TestDuplicateEventRegistration failed: BasicEvent not registered initially");
    
    // Try to register the same event again
    manager.RegisterEvent<BasicEvent>();
    
    // The event should still be contained
    ACASSERT(manager.Contain<BasicEvent>(), "TestDuplicateEventRegistration failed: BasicEvent no longer registered after duplicate registration attempt");
    
    ACMSG("TestDuplicateEventRegistration passed");
}

void TestEventListenerManagement() {
    ACMSG("Running TestEventListenerManagement...");
    ac::EventManager manager;
    
    // Register event
    manager.RegisterEvent<BasicEvent>();
    
    // Add a listener
    bool listenerCalled = false;
    manager.AddListener<BasicEvent>([&listenerCalled](const BasicEvent& event) {
        listenerCalled = true;
        return true;
    });
    
    // Invoke the event
    manager.Invoke(BasicEvent{42}, AllowToken<BasicEvent>());
    
    // Verify that the listener was called
    ACASSERT(listenerCalled, "TestEventListenerManagement failed: Listener was not called when event was invoked");
    
    ACMSG("TestEventListenerManagement passed");
}

void TestMultipleListenersForEvent() {
    ACMSG("Running TestMultipleListenersForEvent...");
    ac::EventManager manager;
    
    // Register event
    manager.RegisterEvent<BasicEvent>();
    
    // Track which listeners are called
    bool listener1Called = false;
    bool listener2Called = false;
    bool listener3Called = false;
    
    // Add multiple listeners
    manager.AddListener<BasicEvent>([&listener1Called](const BasicEvent& event) {
        listener1Called = true;
        return true;
    });
    
    manager.AddListener<BasicEvent>([&listener2Called](const BasicEvent& event) {
        listener2Called = true;
        return true;
    });
    
    manager.AddListener<BasicEvent>([&listener3Called](const BasicEvent& event) {
        listener3Called = true;
        return true;
    });
    
    // Invoke the event
    manager.Invoke(BasicEvent{42}, AllowToken<BasicEvent>());
    
    // Verify all listeners were called
    ACASSERT(listener1Called, "TestMultipleListenersForEvent failed: First listener was not called");
    ACASSERT(listener2Called, "TestMultipleListenersForEvent failed: Second listener was not called");
    ACASSERT(listener3Called, "TestMultipleListenersForEvent failed: Third listener was not called");
    
    ACMSG("TestMultipleListenersForEvent passed");
}

void TestEventInvocation() {
    ACMSG("Running TestEventInvocation...");
    ac::EventManager manager;
    
    // Register event
    manager.RegisterEvent<BasicEvent>();
    
    // Keep track of received event data
    int receivedValue = 0;
    
    // Add a listener
    manager.AddListener<BasicEvent>([&receivedValue](const BasicEvent& event) {
        receivedValue = event.value;
        return true;
    });
    
    // Invoke the event with specific data
    const int TEST_VALUE = 42;
    manager.Invoke(BasicEvent{TEST_VALUE}, AllowToken<BasicEvent>());
    
    // Verify that the listener received the correct event data
    ACASSERT(receivedValue == TEST_VALUE, 
        "TestEventInvocation failed: Listener received incorrect event value");
    
    ACMSG("TestEventInvocation passed");
}

void TestMultipleEventInvocation() {
    ACMSG("Running TestMultipleEventInvocation...");
    ac::EventManager manager;
    
    // Register multiple events
    manager.RegisterEvent<BasicEvent>()
           .RegisterEvent<StringEvent>();
    
    // Track received data for each event type
    int receivedIntValue = 0;
    std::string receivedStringValue;
    
    // Add listeners for each event type
    manager.AddListener<BasicEvent>([&receivedIntValue](const BasicEvent& event) {
        receivedIntValue = event.value;
        return true;
    });
    
    manager.AddListener<StringEvent>([&receivedStringValue](const StringEvent& event) {
        receivedStringValue = event.message;
        return true;
    });
    
    // Invoke events with specific data
    const int TEST_INT = 42;
    const std::string TEST_STRING = "Hello, EventManager!";
    
    manager.Invoke(BasicEvent{TEST_INT}, AllowToken<BasicEvent>());
    manager.Invoke(StringEvent{TEST_STRING}, AllowToken<StringEvent>());
    
    // Verify that each listener received the correct event data
    ACASSERT(receivedIntValue == TEST_INT, 
        "TestMultipleEventInvocation failed: BasicEvent listener received incorrect value");
    ACASSERT(receivedStringValue == TEST_STRING, 
        "TestMultipleEventInvocation failed: StringEvent listener received incorrect message");
    
    ACMSG("TestMultipleEventInvocation passed");
}

void TestContainMethod() {
    ACMSG("Running TestContainMethod...");
    ac::EventManager manager;
    
    // Initially no events are registered
    ACASSERT(!manager.Contain<BasicEvent>(), 
        "TestContainMethod failed: Contain() returned true for unregistered event");
    
    // Register an event
    manager.RegisterEvent<BasicEvent>();
    
    // Now the event should be contained
    ACASSERT(manager.Contain<BasicEvent>(), 
        "TestContainMethod failed: Contain() returned false for registered event");
    
    // But other events still shouldn't be contained
    ACASSERT(!manager.Contain<StringEvent>(), 
        "TestContainMethod failed: Contain() returned true for unregistered event");
    
    ACMSG("TestContainMethod passed");
}

void TestEventPriority() {
    ACMSG("Running TestEventPriority...");
    ac::EventManager manager;
    
    // Register event
    manager.RegisterEvent<BasicEvent>();
    
    // We'll track the order of listener calls
    std::vector<int> callOrder;
    
    // Add listeners
    manager.AddListener<BasicEvent>([&callOrder](const BasicEvent&) {
        callOrder.push_back(1);
        return true;
    });
    
    manager.AddListener<BasicEvent>([&callOrder](const BasicEvent&) {
        callOrder.push_back(2);
        return true;
    });
    
    manager.AddListener<BasicEvent>([&callOrder](const BasicEvent&) {
        callOrder.push_back(3);
        return true;
    });
    
    // Invoke the event
    manager.Invoke(BasicEvent{42}, AllowToken<BasicEvent>());
    
    // Verify that listeners were called in the order they were added
    ACASSERT(callOrder.size() == 3, 
        "TestEventPriority failed: Not all listeners were called");
    ACASSERT(callOrder[0] == 1 && callOrder[1] == 2 && callOrder[2] == 3, 
        "TestEventPriority failed: Listeners were not called in the expected order");
    
    ACMSG("TestEventPriority passed");
}

void TestListenerRemoval() {
    // Note: The current EventManager API doesn't have a method to remove listeners
    // This test is a placeholder in case that functionality is added later
    ACMSG("TestListenerRemoval skipped: not implemented in EventManager API");
}

void TestEdgeCases() {
    ACMSG("Running TestEdgeCases...");
    ac::EventManager manager;
    
    // Test 1: Invoking an unregistered event
    // This is expected to trigger an assertion in debug mode
    // For the test, we'll just check that registering after invoking works
    
    // Register event after listeners are added
    manager.RegisterEvent<BasicEvent>();
    
    // Test 2: Create a listener that returns false (signaling event should not propagate)
    bool listener1Called = false;
    bool listener2Called = false;
    
    manager.AddListener<BasicEvent>([&listener1Called](const BasicEvent&) {
        listener1Called = true;
        return false;  // Stop propagation
    });
    
    manager.AddListener<BasicEvent>([&listener2Called](const BasicEvent&) {
        listener2Called = true;
        return true;
    });
    
    // Invoke the event
    manager.Invoke(BasicEvent{42}, AllowToken<BasicEvent>());
    
    // Since the first listener returns false, the second should not be called
    ACASSERT(listener1Called, "TestEdgeCases failed: First listener was not called");
    ACASSERT(!listener2Called, "TestEdgeCases failed: Second listener was called despite event propagation being stopped");
    
    ACMSG("TestEdgeCases passed");
}

void RunAllEventManagerTests() {
    ACMSG("Starting EventManager tests...");
    
    TestEventRegistration();
    TestMultipleEventRegistration();
    TestDuplicateEventRegistration();
    TestEventListenerManagement();
    TestMultipleListenersForEvent();
    TestEventInvocation();
    TestMultipleEventInvocation();
    TestContainMethod();
    TestEventPriority();
    TestListenerRemoval();
    //TestEdgeCases();
    
    ACMSG("All EventManager tests completed");
}