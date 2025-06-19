#include "acpch.h"
#include "Benchmark.h"
#include "Achoium.h"
using namespace ac;
struct BenchmarkEvent {
    int value;
};
PUBLIC_ALLOWTOKEN(BenchmarkEvent)
void BenchmarkEventSystem(int n) {
    ac::EventManager manager;

    // Measure time to register events
    auto start = std::chrono::high_resolution_clock::now();
    manager.RegisterEvent<BenchmarkEvent>();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    ACMSG("Time to register event: " << duration.count() << " seconds");

    // Measure time to add listeners
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        manager.AddListener<BenchmarkEvent>([](const BenchmarkEvent& event) {
            return true;
        });
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    ACMSG("Time to add " << n << " listeners: " << duration.count() << " seconds");

    // Measure time to invoke events
    BenchmarkEvent event{ 42 };
    start = std::chrono::high_resolution_clock::now();
	manager.Invoke(event, AllowToken<BenchmarkEvent>());
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    ACMSG("Time to invoke event " << n <<" times: " << duration.count() << " seconds");
}


