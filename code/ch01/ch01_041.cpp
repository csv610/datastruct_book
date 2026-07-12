auto lambda = [count = 0]() mutable { return ++count; };
lambda();  // 0
lambda();  // 1
