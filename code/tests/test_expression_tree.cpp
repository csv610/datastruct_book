#include "expression_tree.h"
#include <cassert>
#include <string>
#include <vector>
#include <print>

int main() {
    // Build from postfix: "3 4 + 2 *" => (3 + 4) * 2
    {
        std::vector<std::string> postfix = {"3", "4", "+", "2", "*"};
        auto tree = dsa::build_expr_tree(postfix);
        assert(dsa::eval_expr_tree(tree.get()) == 14.0);
    }

    // Build from postfix: "5 1 2 + 4 * + 3 -" => 5 + ((1+2)*4) - 3 = 14
    {
        std::vector<std::string> postfix = {"5", "1", "2", "+", "4", "*", "+", "3", "-"};
        auto tree = dsa::build_expr_tree(postfix);
        assert(dsa::eval_expr_tree(tree.get()) == 14.0);
    }

    // Simple: "10 3 /" => 10/3
    {
        std::vector<std::string> postfix = {"10", "3", "/"};
        auto tree = dsa::build_expr_tree(postfix);
        assert(std::abs(dsa::eval_expr_tree(tree.get()) - 3.333) < 0.01);
    }

    // Infix output for "3 4 + 2 *" should be "3 + 4 * 2" or "(3+4)*2"
    {
        std::vector<std::string> postfix = {"3", "4", "+", "2", "*"};
        auto tree = dsa::build_expr_tree(postfix);
        std::vector<std::string> infix;
        dsa::inorder_traversal(tree.get(), infix);
        // Should contain parentheses for correct grouping
        bool has_parens = false;
        for (auto& s : infix) if (s == "(") has_parens = true;
        assert(has_parens);
    }

    // Single number
    {
        std::vector<std::string> postfix = {"42"};
        auto tree = dsa::build_expr_tree(postfix);
        assert(dsa::eval_expr_tree(tree.get()) == 42.0);
    }

    std::print("All expression tree tests passed\n");
    return 0;
}
