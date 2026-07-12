#include <string>
#include <algorithm>
using namespace std;

string addStrings(const string& a, const string& b) {
    string result;
    int carry = 0, i = a.size() - 1, j = b.size() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += a[i--] - '0';
        if (j >= 0) sum += b[j--] - '0';
        result.push_back('0' + sum % 10);
        carry = sum / 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

string subtractStrings(const string& a, const string& b) {
    string result;
    int borrow = 0, i = a.size() - 1, j = b.size() - 1;
    while (i >= 0) {
        int diff = (a[i--] - '0') - borrow;
        if (j >= 0) diff -= (b[j--] - '0');
        if (diff < 0) { diff += 10; borrow = 1; }
        else           { borrow = 0; }
        result.push_back('0' + diff);
    }
    while (result.size() > 1 && result.back() == '0')
        result.pop_back();
    reverse(result.begin(), result.end());
    return result;
}

string karatsuba(const string& x, const string& y) {
    int n = max(x.size(), y.size());
    if (n <= 9)
        return to_string(stoll(x) * stoll(y));

    int d = (n + 1) / 2;
    string x1 = x.substr(0, x.size() - d);
    string x0 = x.substr(x.size() - d);
    string y1 = y.substr(0, y.size() - d);
    string y0 = y.substr(y.size() - d);

    string z2 = karatsuba(x1, y1);
    string z0 = karatsuba(x0, y0);
    string sum1 = karatsuba(
        addStrings(x1, x0), addStrings(y1, y0));
    string z1 = subtractStrings(
        subtractStrings(sum1, z2), z0);

    for (int k = 0; k < 2 * d; k++) z2.push_back('0');
    for (int k = 0; k < d; k++)     z1.push_back('0');

    return addStrings(addStrings(z2, z1), z0);
}
