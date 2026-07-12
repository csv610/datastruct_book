#include <string>
#include <vector>

struct activation_record {
    std::string function;
    int param_n;
    int local_product;
    int return_to;  // index in call sequence
};

int factorial_simulated(int n) {
    linked_stack<activation_record> call_stack;
    int next_call_id = 0;
    // push initial call
    call_stack.push({"fact", n, 1, -1});

    while (!call_stack.empty()) {
        auto& frame = call_stack.top();

        if (frame.function == "fact") {
            if (frame.param_n <= 1) {
                // base case: return 1
                int result = 1;
                call_stack.pop();
                if (!call_stack.empty()) {
                    // multiply into parent frame
                    auto& parent = call_stack.top();
                    parent.local_product =
                        parent.param_n * result;
                    parent.function = "fact_returned";
                }
                continue;
            }
            // recursive case: push new frame
            frame.function = "fact_awaiting";
            call_stack.push(
                {"fact", frame.param_n - 1, 1, 0});
        } else if (frame.function == "fact_awaiting") {
            // child returned, product is in local_product
            int result = frame.local_product;
            call_stack.pop();
            if (!call_stack.empty()) {
                auto& parent = call_stack.top();
                parent.local_product =
                    parent.param_n * result;
                parent.function = "fact_returned";
            }
        } else {  // fact_returned
            return frame.local_product;
        }
    }
    return 1;
}
