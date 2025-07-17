// TestUnit.cpp
#include "acpch.h"
#include "Achoium.h"

#include "TestUnit.h"
#include "EventManagerTest.h"

void TestCreateEntity() {
    ac::World world;
    auto entity1 = world.CreateEntity("TestEntity1"); 
    auto entity2 = world.CreateEntity("TestEntity2");
    ACASSERT(entity1 != ac::NULL_ENTITY, "TestCreateEntity failed: entity1 is NULL_ENTITY");
    ACASSERT(entity2 != ac::NULL_ENTITY, "TestCreateEntity failed: entity2 is NULL_ENTITY");
    ACASSERT(world.GetEntityCount() == 2, "TestCreateEntity failed: entity count is not 2");
    ACASSERT(world.HasAll<TestComponent>(entity1) == false, "TestCreateEntity failed: entity1 should not have any components");
    ACASSERT(world.HasAll<TestComponent>(entity2) == false, "TestCreateEntity failed: entity2 should not have any components");
    ACMSG("TestCreateEntity passed");
}

void TestAddComponent() {
    ac::World world;
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    world.Add<TestComponent>(entity1, { 42 });
    world.Add<TestComponentB>(entity2, { 3.14f });
    ACASSERT(world.Has<TestComponent>(entity1), "TestAddComponent failed: TestComponent not added to entity1");
    ACASSERT(world.Get<TestComponent>(entity1).value == 42, "TestAddComponent failed: incorrect TestComponent value for entity1");
    ACASSERT(world.Has<TestComponentB>(entity2), "TestAddComponent failed: TestComponentB not added to entity2");
    ACASSERT(world.Get<TestComponentB>(entity2).value == 3.14f, "TestAddComponent failed: incorrect TestComponentB value for entity2");
    ACASSERT(!world.Has<TestComponent>(entity2), "TestAddComponent failed: entity2 should not have TestComponent");
    ACASSERT(!world.Has<TestComponentB>(entity1), "TestAddComponent failed: entity1 should not have TestComponentB");
    ACMSG("TestAddComponent passed");
}

void TestDeleteComponent() {
    ac::World world;
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    world.Add<TestComponent>(entity1, { 42 });
    world.Add<TestComponentB>(entity2, { 3.14f });
    world.Delete<TestComponent>(entity1);
    world.Delete<TestComponentB>(entity2);
    ACASSERT(!world.Has<TestComponent>(entity1), "TestDeleteComponent failed: TestComponent not deleted from entity1");
    ACASSERT(!world.Has<TestComponentB>(entity2), "TestDeleteComponent failed: TestComponentB not deleted from entity2");
    ACMSG("TestDeleteComponent passed");
}

void TestDeleteEntity() {
    ac::World world;
    auto entity1 = world.CreateEntity("TestEntity1");
    auto entity2 = world.CreateEntity("TestEntity2");
    world.Add<TestComponent>(entity1, { 42 });
    world.Add<TestComponentB>(entity2, { 3.14f });
    world.DeleteEntity(entity1);
    world.DeleteEntity(entity2);
    ACASSERT(world.GetEntityCount() == 0, "TestDeleteEntity failed: entity count is not 0");
    ACASSERT(!world.Has<TestComponent>(entity1), "TestDeleteEntity failed: TestComponent not deleted from entity1");
    ACASSERT(!world.Has<TestComponentB>(entity2), "TestDeleteEntity failed: TestComponentB not deleted from entity2");
    ACMSG("TestDeleteEntity passed");
}

void TestResourceSystem() {
    ac::World world;
    
    world.AddResource<TestResource>(new TestResource);
	world.GetResourse<TestResource>().name = "Resource1";
	world.GetResourse<TestResource>().data = 100;

    TestResource& retrievedResource1 = world.GetResourse<TestResource>();

    ACASSERT(retrievedResource1.name == "Resource1", "TestResourceSystem failed: incorrect name for resource1");
    ACASSERT(retrievedResource1.data == 100, "TestResourceSystem failed: incorrect data for resource1");

    ACMSG("TestResourceSystem passed");
}

void TestSimpleView() {
    ac::World world;
    auto entity1 = world.CreateEntity("Entity1");
    auto entity2 = world.CreateEntity("Entity2");
    auto entity3 = world.CreateEntity("Entity3");

    world.Add<TestComponent>(entity1, { 42 });
    world.Add<TestComponentB>(entity1, { 3.14f });
    world.Add<TestComponent>(entity2, { 84 });
    world.Add<TestComponentB>(entity3, { 6.28f });

    auto view = world.View<TestComponent, TestComponentB>();
    bool foundEntity1 = false;
    bool foundEntity3 = false;

    view.ForEach([&](ac::Entity id, TestComponent& a, TestComponentB& b) {
        if (id == entity1) {
            foundEntity1 = true;
            ACASSERT(a.value == 42, "TestSimpleView failed: incorrect TestComponent value for entity1");
            ACASSERT(b.value == 3.14f, "TestSimpleView failed: incorrect TestComponentB value for entity1");
        }
        if (id == entity3) {
            foundEntity3 = true;
            ACASSERT(a.value == 0, "TestSimpleView failed: incorrect TestComponent value for entity3");
            ACASSERT(b.value == 6.28f, "TestSimpleView failed: incorrect TestComponentB value for entity3");
        }
        });

    ACASSERT(foundEntity1, "TestSimpleView failed: entity1 not found in view");
    ACASSERT(!foundEntity3, "TestSimpleView failed: entity3 should not be in view");
    ACASSERT(view.GetPacked().size() == 1, "TestSimpleView failed: view should contain only one entity");
    ACMSG("TestSimpleView passed");
}

void TestRegisterEvent() {
    ac::EventManager manager;
    manager.RegisterEvent<TestEvent>();
    ACASSERT(manager.Contain<TestEvent>(), "TestRegisterEvent failed");
    ACMSG("TestRegisterEvent passed");
}

void TestAddListener() {
    ac::EventManager manager;
    manager.RegisterEvent<TestEvent>();
    bool listenerCalled = false;
    manager.AddListener<TestEvent>([&listenerCalled](const TestEvent& event) {
        listenerCalled = true;
        return true;
        });
    manager.Invoke(TestEvent{ 42 }, AllowToken<TestEvent>());
    ACASSERT(listenerCalled, "TestAddListener failed");
    ACMSG("TestAddListener passed");
}

void TestInvokeEvent() {
    ac::EventManager manager;
    manager.RegisterEvent<TestEvent>();
    int receivedValue = 0;
    manager.AddListener<TestEvent>([&receivedValue](const TestEvent& event) {
        receivedValue = event.value;
        return true;
        });
    manager.Invoke(TestEvent{ 42 }, AllowToken<TestEvent>());
    ACASSERT(receivedValue == 42, "TestInvokeEvent failed");
    ACMSG("TestInvokeEvent passed");
}

void TestInput()
{
	ac::EventManager eventManager;
    ac::WinWindow window({ "AC", 1280, 720 }, eventManager);
    ac::WindowsInput input(&window);
    for (int i = 0; i < 50; ++i)
    {
		window.OnUpdate();
		glm::vec2 v = input.GetMousePosition();
		ACMSG("MousePose: " << v.x << " " << v.y);
		//ACMSG("KeyPressed: " << input.IsKeyPressed(AC_KEY_SPACE, window));
		//ACMSG("MouseButtonPressed: " << input.IsMouseButtonPressed(AC_MOUSE_BUTTON_LEFT, window));
    }
    //window.Shutdown();

}

void StartTest() {
    TestCreateEntity();
    TestAddComponent();
    TestDeleteComponent();
    TestDeleteEntity();
    TestResourceSystem();
    TestSimpleView();
    TestRegisterEvent();
    TestAddListener();
    TestInvokeEvent();
    TestInput();

    TestAllTimeMethods();

    // Run the new comprehensive event tests
    RunAllEventManagerTests();

    RunAllWorldTests();

}
