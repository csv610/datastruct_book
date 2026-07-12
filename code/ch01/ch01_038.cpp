auto shared = std::make_shared<int>(10);
std::weak_ptr<int> weak = shared;

if (auto locked = weak.lock()) {
    // locked is a shared_ptr; object is alive
    // *locked == 10
}
shared.reset();  // object destroyed
// weak.lock() now returns null
