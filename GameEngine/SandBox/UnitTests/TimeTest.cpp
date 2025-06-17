// TimeTest.cpp
#include "acpch.h"
#include "Achoium.h"
#include "TimeTest.h"
#include <thread> // Add this include for std::this_thread
#include <chrono> // Add this include for std::chrono

void TestTimeConstructor() {
    ac::Time time;
    // Check initial values after construction
    ACASSERT(time.Delta() == 0.0f, "TestTimeConstructor failed: Delta not initialized to 0.0f");
    ACASSERT(time.RealDelta() == 0.0f, "TestTimeConstructor failed: RealDelta not initialized to 0.0f");
    ACASSERT(time.TimeScale() == 1.0f, "TestTimeConstructor failed: TimeScale not initialized to 1.0f");
    ACASSERT(time.CurTime() == 0.0f, "TestTimeConstructor failed: CurTime not initialized to 0.0f");
    ACASSERT(time.RealCurTime() == 0.0f, "TestTimeConstructor failed: RealCurTime not initialized to 0.0f");
    ACMSG("TestTimeConstructor passed");
}

void TestTimeUpdate() {
    ac::Time time;
    // First update should set deltas but not affect times much
    time.Update();
    
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    
    // Second update should have measurable deltas
    time.Update();
    
    // Delta should be greater than 0 after update
    ACASSERT(time.RealDelta() > 0.0f, "TestTimeUpdate failed: RealDelta should be greater than 0");
    ACASSERT(time.Delta() > 0.0f, "TestTimeUpdate failed: Delta should be greater than 0");
    ACMSG("TestTimeUpdate passed");
}

void TestTimeDelta() {
    ac::Time time;
    time.Update();
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    time.Update();
    
    float delta = time.Delta();
    ACASSERT(delta > 0.0f, "TestTimeDelta failed: Delta() should return a value greater than 0");
    ACASSERT(delta < 0.1f, "TestTimeDelta failed: Delta() should return a reasonable delta (less than 0.1s)");
    ACMSG("TestTimeDelta passed");
}

void TestTimeRealDelta() {
    ac::Time time;
    time.Update();
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    time.Update();
    
    float realDelta = time.RealDelta();
    ACASSERT(realDelta > 0.0f, "TestTimeRealDelta failed: RealDelta() should return a value greater than 0");
    ACASSERT(realDelta < 0.1f, "TestTimeRealDelta failed: RealDelta() should return a reasonable delta (less than 0.1s)");
    ACMSG("TestTimeRealDelta passed");
}

void TestTimeScale() {
    ac::Time time;
    // Default time scale should be 1.0f
    ACASSERT(time.TimeScale() == 1.0f, "TestTimeScale failed: Default TimeScale should be 1.0f");
    
    // Test modifying the timeScale via reflection (since there's no setter)
    // Note: This is not recommended in real code, but for testing it helps verify the getter
    // Would need to add a SetTimeScale method to properly test this
    float* timeScalePtr = (float*)((char*)&time);  // Assuming timeScale is the first member
    *timeScalePtr = 0.5f;
    
    ACASSERT(time.TimeScale() == 0.5f, "TestTimeScale failed: TimeScale() should return the modified value");
    ACMSG("TestTimeScale passed");
}

void TestCurTime() {
    ac::Time time;
    ACASSERT(time.CurTime() == 0.0f, "TestCurTime failed: Initial CurTime should be 0.0f");
    
    time.Update();
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    time.Update();
    
    // CurTime should increase after updates
    float curTime = time.CurTime();
    ACASSERT(curTime > 0.0f, "TestCurTime failed: CurTime should be greater than 0.0f after updates");
    ACMSG("TestCurTime passed");
}

void TestRealCurTime() {
    ac::Time time;
    ACASSERT(time.RealCurTime() == 0.0f, "TestRealCurTime failed: Initial RealCurTime should be 0.0f");
    
    time.Update();
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    time.Update();
    
    float realCurTime = time.RealCurTime();
    ACASSERT(realCurTime > 0.0f, "TestRealCurTime failed: RealCurTime should be greater than 0.0f");
    ACMSG("TestRealCurTime passed");
}

void TestGetFrameRate() {
    ac::Time time;
    time.Update();
    // Sleep a small amount to ensure measurable delta
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    time.Update();
    
    float frameRate = time.GetFrameRate();
    // For a 16ms sleep, framerate should be around 60fps (or less)
    ACASSERT(frameRate > 0.0f, "TestGetFrameRate failed: Frame rate should be greater than 0");
    ACASSERT(frameRate <= 100.0f, "TestGetFrameRate failed: Frame rate should be reasonable (<= 100fps for 16ms sleep)");
    ACMSG("TestGetFrameRate passed");
}

void TestAllTimeMethods() {
    TestTimeConstructor();
    TestTimeUpdate();
    TestTimeDelta();
    TestTimeRealDelta();
    //TestTimeScale();
    TestCurTime();
    TestRealCurTime();
    TestGetFrameRate();
    ACMSG("All Time class tests completed");
}