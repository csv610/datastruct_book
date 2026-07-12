# macOS
sudo sample ./program -duration 10  # produces a call tree

# Linux
perf record ./program && perf report

# macOS Instruments (GUI)
xcrun xctrace record --template "Time Profiler" --launch ./program
