#pragma once
#include "Achoium.h"

void TestWorldConstructor();
void TestWorldCreateEntity();
void TestWorldRegisterType();
void TestWorldAddComponent();
void TestWorldDeleteComponent();
void TestWorldDeleteEntity();
void TestWorldHasMethods();
void TestWorldResourceSystem();
void TestWorldSimpleView();
void TestWorldReset();
void TestWorldGetEntityCount();
void TestWorldGetPoolCount();
void TestWorldOnAddedEvent();
void TestWorldOnDeletedEvent();

// Main test runner function
void RunAllWorldTests();