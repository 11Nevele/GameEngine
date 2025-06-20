#include "acpch.h"
#include "Achoium.h"
#include "WorldTest.h"

// Test components
struct TestWorldComponent {
    int value;
};

struct TestWorldComponentB {
    float value;
};

// Test resources
struct TestWorldResource {
    std::string name;
    int data;
};

// Event tracking for OnAdded/OnDeleted tests
struct EventTracker {
    bool onAddedCalled = false;
    bool onDeletedCalled = false;
    ac::Entity addedEntityId = ac::NULL_ENTITY;
    ac::Entity deletedEntityId = ac::NULL_ENTITY;
    int addedValue = 0;
    int deletedValue = 0;
    
    void Reset() {
        onAddedCalled = false;
        onDeletedCalled = false;
        addedEntityId = ac::NULL_ENTITY;
        deletedEntityId = ac::NULL_ENTITY;
        addedValue = 0;
        deletedValue = 0;
    }
};

void TestWorldConstructor() {
    ac::World world;
    
    // Check that a newly constructed world has no entities
    ACASSERT(world.GetEntityCount() == 0, "TestWorldConstructor failed: new world should have 0 entities");
    
    // Check that EventManager resource is automatically added
    ACASSERT(world.GetResourse<ac::EventManager>().Contain<ac::OnAdded<TestWorldComponent>>() == false, 
            "TestWorldConstructor failed: event manager should not contain events for unregistered components");
            
    ACMSG("TestWorldConstructor passed");
}

void TestWorldCreateEntity() {
    ac::World world;
    
    // Create entities with and without tags
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    auto entity3 = world.CreateEntity(); // No tag
    
    // Verify entities were created
    ACASSERT(entity1 != ac::NULL_ENTITY, "TestWorldCreateEntity failed: entity1 is NULL_ENTITY");
    ACASSERT(entity2 != ac::NULL_ENTITY, "TestWorldCreateEntity failed: entity2 is NULL_ENTITY");
    ACASSERT(entity3 != ac::NULL_ENTITY, "TestWorldCreateEntity failed: entity3 is NULL_ENTITY");
    
    // Check entity count
    ACASSERT(world.GetEntityCount() == 3, "TestWorldCreateEntity failed: entity count is not 3");
    
    ACMSG("TestWorldCreateEntity passed");
}

void TestWorldRegisterType() {
    ac::World world;
    
    // Register a component type explicitly
    world.RegisterType<TestWorldComponent>();
    
    // Verify the type was registered
    ACASSERT(world.GetPoolCount() == 1, "TestWorldRegisterType failed: pool count should be 1");
    
    // Verify the OnAdded/OnDeleted events were registered for this component
    ACASSERT(world.GetResourse<ac::EventManager>().Contain<ac::OnAdded<TestWorldComponent>>(), 
            "TestWorldRegisterType failed: OnAdded event not registered");
    ACASSERT(world.GetResourse<ac::EventManager>().Contain<ac::OnDeleted<TestWorldComponent>>(), 
            "TestWorldRegisterType failed: OnDeleted event not registered");
            
    ACMSG("TestWorldRegisterType passed");
}

void TestWorldAddComponent() {
    ac::World world;
    
    // Create an entity and add components
    auto entity = world.CreateEntity("TestEntity");
    
    // Add component explicitly
    world.RegisterType<TestWorldComponent>();
    world.Add<TestWorldComponent>(entity, {42});
    
    // Add component implicitly (should auto-register)
    world.Add<TestWorldComponentB>(entity, {3.14f});
    
    // Check both components were added
    ACASSERT(world.Has<TestWorldComponent>(entity), 
            "TestWorldAddComponent failed: TestWorldComponent not added to entity");
    ACASSERT(world.Has<TestWorldComponentB>(entity), 
            "TestWorldAddComponent failed: TestWorldComponentB not added to entity");
    
    // Check component values
    ACASSERT(world.Get<TestWorldComponent>(entity).value == 42, 
            "TestWorldAddComponent failed: incorrect TestWorldComponent value");
    ACASSERT(world.Get<TestWorldComponentB>(entity).value == 3.14f, 
            "TestWorldAddComponent failed: incorrect TestWorldComponentB value");
    
    // Check component pool count
    ACASSERT(world.GetPoolCount() == 2, "TestWorldAddComponent failed: pool count should be 2");
    
    ACMSG("TestWorldAddComponent passed");
}

void TestWorldDeleteComponent() {
    ac::World world;
    
    // Create an entity and add components
    auto entity = world.CreateEntity("TestEntity");
    world.Add<TestWorldComponent>(entity, {42});
    world.Add<TestWorldComponentB>(entity, {3.14f});
    
    // Delete only one component
    world.Delete<TestWorldComponent>(entity);
    
    // Verify component was deleted
    ACASSERT(!world.Has<TestWorldComponent>(entity), 
            "TestWorldDeleteComponent failed: TestWorldComponent not deleted from entity");
    ACASSERT(world.Has<TestWorldComponentB>(entity), 
            "TestWorldDeleteComponent failed: TestWorldComponentB should still exist");
    
    ACMSG("TestWorldDeleteComponent passed");
}

void TestWorldDeleteEntity() {
    ac::World world;
    
    // Create and set up entities
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    
    world.Add<TestWorldComponent>(entity1, {42});
    world.Add<TestWorldComponentB>(entity2, {3.14f});
    
    // Delete first entity
    world.DeleteEntity(entity1);
    
    // Check entity was removed
    ACASSERT(world.GetEntityCount() == 1, "TestWorldDeleteEntity failed: entity count should be 1");
    
    // Try to access (should fail assertion in debug, but we'll check the component presence)
    bool hasComponent = false;
    try {
        hasComponent = world.Has<TestWorldComponent>(entity1);
    }
    catch (...) {
        // Expected to fail in debug mode with assertion
        hasComponent = false;
    }
    
    ACASSERT(!hasComponent, "TestWorldDeleteEntity failed: deleted entity still has components");
    ACASSERT(world.Has<TestWorldComponentB>(entity2), 
            "TestWorldDeleteEntity failed: other entity should still have its component");
    
    // Delete second entity
    world.DeleteEntity(entity2);
    
    // Check all entities are gone
    ACASSERT(world.GetEntityCount() == 0, "TestWorldDeleteEntity failed: entity count should be 0");
    
    ACMSG("TestWorldDeleteEntity passed");
}

void TestWorldHasMethods() {
    ac::World world;
    
    // Create and set up entities
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    
    // Add different combinations of components
    world.Add<TestWorldComponent>(entity1, {42});
    world.Add<TestWorldComponentB>(entity1, {3.14f});
    world.Add<TestWorldComponentB>(entity2, {6.28f});
    
    // Test Has<T>
    ACASSERT(world.Has<TestWorldComponent>(entity1), "TestWorldHasMethods failed: Has<T> should return true");
    ACASSERT(!world.Has<TestWorldComponent>(entity2), "TestWorldHasMethods failed: Has<T> should return false");
    
    // Test HasAll<T...>
    ACASSERT( (world.HasAll<TestWorldComponent, TestWorldComponentB>(entity1)), 
            "TestWorldHasMethods failed: HasAll should return true for entity1");
    ACASSERT(!(world.HasAll<TestWorldComponent, TestWorldComponentB>(entity2)), 
            "TestWorldHasMethods failed: HasAll should return false for entity2");
    
    // Test HasAny<T...>
    ACASSERT((world.HasAny<TestWorldComponent, TestWorldComponentB>(entity1)), 
            "TestWorldHasMethods failed: HasAny should return true for entity1");
    ACASSERT((world.HasAny<TestWorldComponent, TestWorldComponentB>(entity2)), 
            "TestWorldHasMethods failed: HasAny should return true for entity2");
    ACASSERT(!(world.HasAny<int, float>(entity1)), 
            "TestWorldHasMethods failed: HasAny should return false for unregistered types");
    
    ACMSG("TestWorldHasMethods passed");
}

void TestWorldResourceSystem() {
    ac::World world;
    
    // Add a custom resource
    world.AddResource<TestWorldResource>(new TestWorldResource{
        "TestResource", 
        100
    });
    
    // Retrieve and check resource
    TestWorldResource& resource = world.GetResourse<TestWorldResource>();
    
    ACASSERT(resource.name == "TestResource", 
            "TestWorldResourceSystem failed: incorrect name for resource");
    ACASSERT(resource.data == 100, 
            "TestWorldResourceSystem failed: incorrect data for resource");
    
    // Modify resource
    resource.data = 200;
    
    // Retrieve again and check modified data
    TestWorldResource& resource2 = world.GetResourse<TestWorldResource>();
    
    ACASSERT(resource2.data == 200, 
            "TestWorldResourceSystem failed: resource data modification not reflected");
    
    ACMSG("TestWorldResourceSystem passed");
}

void TestWorldSimpleView() {
    ac::World world;
    
    // Create entities with different component combinations
    auto entity1 = world.CreateEntity("Entity1"); // Will have both components
    auto entity2 = world.CreateEntity("Entity2"); // Will have only Component
    auto entity3 = world.CreateEntity("Entity3"); // Will have only ComponentB
    
    // Add components
    world.Add<TestWorldComponent>(entity1, {42});
    world.Add<TestWorldComponentB>(entity1, {3.14f});
    world.Add<TestWorldComponent>(entity2, {84});
    world.Add<TestWorldComponentB>(entity3, {6.28f});
    
    // Test view of single component
    auto viewA = world.View<TestWorldComponent>();
    
    std::vector<ac::Entity> entitiesWithA;
    viewA.ForEach([&](ac::Entity id, TestWorldComponent& a) {
        entitiesWithA.push_back(id);
    });
    
    ACASSERT(entitiesWithA.size() == 2, 
            "TestWorldSimpleView failed: view should contain exactly 2 entities");
    ACASSERT(std::find(entitiesWithA.begin(), entitiesWithA.end(), entity1) != entitiesWithA.end() &&
             std::find(entitiesWithA.begin(), entitiesWithA.end(), entity2) != entitiesWithA.end(),
             "TestWorldSimpleView failed: view doesn't contain expected entities");
    
    // Test view of multiple components
    auto viewAB = world.View<TestWorldComponent, TestWorldComponentB>();
    
    int entitiesWithABCount = 0;
    viewAB.ForEach([&](ac::Entity id, TestWorldComponent& a, TestWorldComponentB& b) {
        entitiesWithABCount++;
        ACASSERT(id == entity1, "TestWorldSimpleView failed: unexpected entity in multiple component view");
        ACASSERT(a.value == 42, "TestWorldSimpleView failed: incorrect component value");
        ACASSERT(b.value == 3.14f, "TestWorldSimpleView failed: incorrect component value");
    });
    
    ACASSERT(entitiesWithABCount == 1, 
            "TestWorldSimpleView failed: multi-component view should contain exactly 1 entity");
    
    ACMSG("TestWorldSimpleView passed");
}

void TestWorldReset() {
    ac::World world;
    
    // Create entities and add components
    auto entity1 = world.CreateEntity("Entity1");
    auto entity2 = world.CreateEntity("Entity2");
    
    world.Add<TestWorldComponent>(entity1, {42});
    world.Add<TestWorldComponentB>(entity2, {3.14f});
    
    // Reset the world
    world.Reset();
    
    // Verify the world is empty
    ACASSERT(world.GetEntityCount() == 0, "TestWorldReset failed: entity count should be 0");
    ACASSERT(world.GetPoolCount() == 0, "TestWorldReset failed: pool count should be 0");
    
    ACMSG("TestWorldReset passed");
}

void TestWorldGetEntityCount() {
    ac::World world;
    
    // Initially should be 0
    ACASSERT(world.GetEntityCount() == 0, "TestWorldGetEntityCount failed: initial count should be 0");
    
    // Create entities
    world.CreateEntity("Entity1");
    world.CreateEntity("Entity2");
    world.CreateEntity("Entity3");
    
    // Should now be 3
    ACASSERT(world.GetEntityCount() == 3, "TestWorldGetEntityCount failed: count should be 3");
    
    // Delete one entity
    world.DeleteEntity(1);
    
    // Should now be 2
    ACASSERT(world.GetEntityCount() == 2, "TestWorldGetEntityCount failed: count should be 2");
    
    ACMSG("TestWorldGetEntityCount passed");
}

void TestWorldGetPoolCount() {
    ac::World world;
    
    // Initially should be 0
    ACASSERT(world.GetPoolCount() == 0, "TestWorldGetPoolCount failed: initial count should be 0");
    
    // Register a type
    world.RegisterType<TestWorldComponent>();
    
    // Should now be 1
    ACASSERT(world.GetPoolCount() == 1, "TestWorldGetPoolCount failed: count should be 1");
    
    // Register another type
    world.RegisterType<TestWorldComponentB>();
    
    // Should now be 2
    ACASSERT(world.GetPoolCount() == 2, "TestWorldGetPoolCount failed: count should be 2");
    
    // Implicit registration through Add
    world.CreateEntity("Entity");
    world.Add<int>(0); // A new type
    
    // Should now be 3
    ACASSERT(world.GetPoolCount() == 3, "TestWorldGetPoolCount failed: count should be 3");
    
    ACMSG("TestWorldGetPoolCount passed");
}

void TestWorldOnAddedEvent() {
    ac::World world;
    EventTracker tracker;
    
    // Register the event listener for OnAdded
    world.GetResourse<ac::EventManager>().AddListener<ac::OnAdded<TestWorldComponent>>([&tracker](const ac::OnAdded<TestWorldComponent>& event) {
        tracker.onAddedCalled = true;
        tracker.addedEntityId = event.ID;
        tracker.addedValue = event.component.value;
        return true;
    });
    
    // Create an entity and add component
    auto entity = world.CreateEntity("TestEntity");
    world.Add<TestWorldComponent>(entity, {42});
    
    // Verify event was triggered
    ACASSERT(tracker.onAddedCalled, "TestWorldOnAddedEvent failed: OnAdded event wasn't triggered");
    ACASSERT(tracker.addedEntityId == entity, 
             "TestWorldOnAddedEvent failed: OnAdded event had incorrect entity ID");
    ACASSERT(tracker.addedValue == 42, 
             "TestWorldOnAddedEvent failed: OnAdded event had incorrect component value");
    
    ACMSG("TestWorldOnAddedEvent passed");
}

void TestWorldOnDeletedEvent() {
    ac::World world;
    EventTracker tracker;
    
    // Register the event listener for OnDeleted
    world.GetResourse<ac::EventManager>().AddListener<ac::OnDeleted<TestWorldComponent>>([&tracker](const ac::OnDeleted<TestWorldComponent>& event) {
        tracker.onDeletedCalled = true;
        tracker.deletedEntityId = event.ID;
        tracker.deletedValue = event.component.value;
        return true;
    });
    
    // Create an entity and add component
    auto entity = world.CreateEntity("TestEntity");
    world.Add<TestWorldComponent>(entity, {42});
    
    // Reset tracker (in case OnAdded triggered it)
    tracker.Reset();
    
    // Delete the component
    world.Delete<TestWorldComponent>(entity);
    
    // Verify event was triggered
    ACASSERT(tracker.onDeletedCalled, "TestWorldOnDeletedEvent failed: OnDeleted event wasn't triggered");
    ACASSERT(tracker.deletedEntityId == entity, 
             "TestWorldOnDeletedEvent failed: OnDeleted event had incorrect entity ID");
    ACASSERT(tracker.deletedValue == 42, 
             "TestWorldOnDeletedEvent failed: OnDeleted event had incorrect component value");
    
    ACMSG("TestWorldOnDeletedEvent passed");
}

// System-related tests implementation for WorldTest.cpp

// Tracking structure to test system execution
struct SystemExecutionTracker {
    bool preUpdateCalled = false;
    bool updateCalled = false;
    bool postUpdateCalled = false;
    int preUpdateValue = 0;
    int updateValue = 0;
    int postUpdateValue = 0;
    std::vector<int> executionOrder;

    void Reset() {
        preUpdateCalled = false;
        updateCalled = false;
        postUpdateCalled = false;
        preUpdateValue = 0;
        updateValue = 0;
        postUpdateValue = 0;
        executionOrder.clear();
    }
};

// Global tracker for test systems
SystemExecutionTracker systemTracker;

// Test system functions
void TestPreUpdateSystem(ac::World& world) {
    systemTracker.preUpdateCalled = true;
    systemTracker.preUpdateValue += 1;
    systemTracker.executionOrder.push_back(1);
}

void TestUpdateSystem(ac::World& world) {
    systemTracker.updateCalled = true;
    systemTracker.updateValue += 10;
    systemTracker.executionOrder.push_back(2);
}

void TestPostUpdateSystem(ac::World& world) {
    systemTracker.postUpdateCalled = true;
    systemTracker.postUpdateValue += 100;
    systemTracker.executionOrder.push_back(3);
}

// Priority system functions
void TestLowPrioritySystem(ac::World& world) {
    systemTracker.executionOrder.push_back(100);
}

void TestMediumPrioritySystem(ac::World& world) {
    systemTracker.executionOrder.push_back(200);
}

void TestHighPrioritySystem(ac::World& world) {
    systemTracker.executionOrder.push_back(300);
}

void TestWorldAddSystems() {
    ac::World world;
    systemTracker.Reset();

    // Add systems of different types
    world.AddPreUpdateSystem(TestPreUpdateSystem, 0);
    world.AddUpdateSystem(TestUpdateSystem, 0);
    world.AddPostUpdateSystem(TestPostUpdateSystem, 0);

    // No assertions here since we're just testing if they can be added without errors
    // The execution tests will verify they were actually added correctly

    ACMSG("TestWorldAddSystems passed");
}

void TestWorldExecuteSystems() {
    ac::World world;
    systemTracker.Reset();

    // Add systems
    world.AddPreUpdateSystem(TestPreUpdateSystem, 0);
    world.AddUpdateSystem(TestUpdateSystem, 0);
    world.AddPostUpdateSystem(TestPostUpdateSystem, 0);

    // Execute pre-update systems
    world.RunPreUpdateSystems();
    ACASSERT(systemTracker.preUpdateCalled, "TestWorldExecuteSystems failed: pre-update system wasn't called");
    ACASSERT(systemTracker.preUpdateValue == 1, "TestWorldExecuteSystems failed: pre-update system value incorrect");
    ACASSERT(!systemTracker.updateCalled, "TestWorldExecuteSystems failed: update system should not be called yet");
    ACASSERT(!systemTracker.postUpdateCalled, "TestWorldExecuteSystems failed: post-update system should not be called yet");

    // Execute update systems
    world.RunUpdateSystems();
    ACASSERT(systemTracker.updateCalled, "TestWorldExecuteSystems failed: update system wasn't called");
    ACASSERT(systemTracker.updateValue == 10, "TestWorldExecuteSystems failed: update system value incorrect");
    ACASSERT(!systemTracker.postUpdateCalled, "TestWorldExecuteSystems failed: post-update system should not be called yet");

    // Execute post-update systems
    world.RunPostUpdateSystems();
    ACASSERT(systemTracker.postUpdateCalled, "TestWorldExecuteSystems failed: post-update system wasn't called");
    ACASSERT(systemTracker.postUpdateValue == 100, "TestWorldExecuteSystems failed: post-update system value incorrect");

    // Check execution order
    ACASSERT(systemTracker.executionOrder.size() == 3, "TestWorldExecuteSystems failed: incorrect number of systems executed");
    ACASSERT(systemTracker.executionOrder[0] == 1, "TestWorldExecuteSystems failed: incorrect execution order");
    ACASSERT(systemTracker.executionOrder[1] == 2, "TestWorldExecuteSystems failed: incorrect execution order");
    ACASSERT(systemTracker.executionOrder[2] == 3, "TestWorldExecuteSystems failed: incorrect execution order");

    ACMSG("TestWorldExecuteSystems passed");
}

void TestWorldSystemPriority() {
    ac::World world;
    systemTracker.Reset();

    // Add systems with different priorities (in reverse order)
    world.AddUpdateSystem(TestHighPrioritySystem, 9);   // Higher number = lower priority = runs later
    world.AddUpdateSystem(TestMediumPrioritySystem, 5);
    world.AddUpdateSystem(TestLowPrioritySystem, 0);    // Lower number = higher priority = runs first

    // Execute update systems
    world.RunUpdateSystems();

    // Check execution order based on priority
    ACASSERT(systemTracker.executionOrder.size() == 3, "TestWorldSystemPriority failed: incorrect number of systems executed");
    ACASSERT(systemTracker.executionOrder[0] == 100, "TestWorldSystemPriority failed: priority order incorrect, low priority should run first");
    ACASSERT(systemTracker.executionOrder[1] == 200, "TestWorldSystemPriority failed: priority order incorrect, medium priority should run second");
    ACASSERT(systemTracker.executionOrder[2] == 300, "TestWorldSystemPriority failed: priority order incorrect, high priority should run last");

    // Test adding multiple systems at same priority level
    systemTracker.Reset();
    ac::World world2;

    // Add systems at same priority level, they should execute in order added
    world2.AddUpdateSystem(TestLowPrioritySystem, 0);
    world2.AddUpdateSystem(TestMediumPrioritySystem, 0);
    world2.AddUpdateSystem(TestHighPrioritySystem, 0);

    // Execute update systems
    world2.RunUpdateSystems();

    // Check that systems at same priority execute in order added
    ACASSERT(systemTracker.executionOrder.size() == 3, "TestWorldSystemPriority failed: incorrect number of systems executed");
    ACASSERT(systemTracker.executionOrder[0] == 100, "TestWorldSystemPriority failed: same priority order incorrect");
    ACASSERT(systemTracker.executionOrder[1] == 200, "TestWorldSystemPriority failed: same priority order incorrect");
    ACASSERT(systemTracker.executionOrder[2] == 300, "TestWorldSystemPriority failed: same priority order incorrect");

    ACMSG("TestWorldSystemPriority passed");
}

void TestWorldSystemInvalidPriority() {
    ac::World world;
    systemTracker.Reset();

    // Test with invalid priority - this should cause an assertion error in debug mode
    // In release mode, it may just silently fail, so we can't rely on catching the exception
    bool exceptionThrown = false;

    try {
        world.AddUpdateSystem(TestUpdateSystem, 10); // Priority > 9 is invalid
    }
    catch (...) {
        exceptionThrown = true;
    }

    // In debug builds with assertions enabled, this should throw an exception
    // In release builds, we can't guarantee this behavior
#ifdef _DEBUG
    ACASSERT(exceptionThrown, "TestWorldSystemInvalidPriority failed: invalid priority should trigger assertion");
#endif

    // Test with valid priority
    bool validPriorityExceptionThrown = false;

    try {
        world.AddUpdateSystem(TestUpdateSystem, 9); // Priority 9 is valid (0-9)
    }
    catch (...) {
        validPriorityExceptionThrown = true;
    }

    ACASSERT(!validPriorityExceptionThrown, "TestWorldSystemInvalidPriority failed: valid priority triggered exception");

    ACMSG("TestWorldSystemInvalidPriority passed");
}

// Main test runner function
void RunAllWorldTests() {
    TestWorldConstructor();
    TestWorldCreateEntity();
    TestWorldRegisterType();
    TestWorldAddComponent();
    TestWorldDeleteComponent();
    TestWorldDeleteEntity();
    TestWorldHasMethods();
    TestWorldResourceSystem();
    TestWorldSimpleView();
    TestWorldReset();
    TestWorldGetEntityCount();
    TestWorldGetPoolCount();
    TestWorldOnAddedEvent();
    TestWorldOnDeletedEvent();

    TestWorldAddSystems();
    TestWorldExecuteSystems();
    TestWorldSystemPriority();
    //TestWorldSystemInvalidPriority();
    
    ACMSG("=== All World class tests completed ===");
}