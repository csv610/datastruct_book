#pragma once
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace dsa {

// ---- Run-Length Encoding ----
inline std::string rle_encode(std::string_view s) {
    if (s.empty()) return "";
    std::string result;
    std::size_t i = 0;
    while (i < s.size()) {
        char c = s[i];
        std::size_t count = 1;
        while (i + count < s.size() && s[i + count] == c) ++count;
        result += c;
        if (count > 1)
            result += std::to_string(count);
        i += count;
    }
    return result;
}

inline std::string rle_decode(std::string_view s) {
    std::string result;
    std::size_t i = 0;
    while (i < s.size()) {
        char c = s[i++];
        std::size_t count = 0;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') {
            count = count * 10 + (s[i] - '0');
            ++i;
        }
        result.append(count == 0 ? 1 : count, c);
    }
    return result;
}

// ---- LZ77 Compression ----
struct lz77_token {
    int offset;      // distance back to match
    int length;      // length of match
    char next_char;  // first character after match
};

inline std::vector<lz77_token> lz77_encode(std::string_view s,
                                            int window_size = 4096,
                                            int lookahead = 18) {
    std::vector<lz77_token> tokens;
    std::size_t pos = 0;
    while (pos < s.size()) {
        int best_offset = 0, best_length = 0;
        std::size_t search_start = (pos > static_cast<std::size_t>(window_size))
                                       ? pos - static_cast<std::size_t>(window_size) : 0;

        for (std::size_t j = search_start; j < pos; ++j) {
            int len = 0;
            while (len < lookahead && pos + len < s.size() &&
                   s[j + len] == s[pos + len])
                ++len;
            if (len > best_length) {
                best_length = len;
                best_offset = static_cast<int>(pos - j);
            }
        }

        if (best_length > 0 && pos + best_length == s.size())
            --best_length;
        char next = (pos + best_length < s.size()) ? s[pos + best_length] : '\0';
        tokens.push_back({best_offset, best_length, next});
        pos += best_length + 1;
    }
    return tokens;
}

inline std::string lz77_decode(const std::vector<lz77_token>& tokens) {
    std::string result;
    for (const auto& t : tokens) {
        if (t.offset > 0) {
            std::size_t start = result.size() - t.offset;
            for (int i = 0; i < t.length; ++i)
                result += result[start + i];
        }
        result += t.next_char;
    }
    return result;
}

// ---- LZ78 Compression ----
struct lz78_token {
    int dict_index;  // index in dictionary (0 = empty)
    char next_char;
};

inline std::vector<lz78_token> lz78_encode(std::string_view s) {
    std::vector<lz78_token> tokens;
    std::vector<std::string> dict = {""};
    std::string current;

    for (char c : s) {
        current += c;
        auto it = std::find(dict.begin() + 1, dict.end(), current);
        if (it != dict.end()) {
            continue;  // already in dict, extend
        } else {
            int index = (current.size() == 1) ? 0
                        : static_cast<int>(std::find(dict.begin() + 1, dict.end(),
                                                     current.substr(0, current.size() - 1)) - dict.begin());
            tokens.push_back({index, c});
            dict.push_back(current);
            current.clear();
        }
    }
    if (!current.empty()) {
        auto it = std::find(dict.begin() + 1, dict.end(), current);
        int index = (it != dict.end()) ? static_cast<int>(it - dict.begin()) : 0;
        tokens.push_back({index, current.back()});
    }
    return tokens;
}

inline std::string lz78_decode(const std::vector<lz78_token>& tokens) {
    std::vector<std::string> dict = {""};
    std::string result;

    for (const auto& t : tokens) {
        std::string entry = (t.dict_index > 0 && t.dict_index < static_cast<int>(dict.size()))
                            ? dict[t.dict_index] + t.next_char
                            : std::string(1, t.next_char);
        result += entry;
        dict.push_back(entry);
    }
    return result;
}

// ---- LZW Compression ----
inline std::vector<int> lzw_encode(std::string_view s) {
    std::vector<int> codes;
    std::vector<std::string> dict;
    for (int i = 0; i < 256; ++i)
        dict.push_back(std::string(1, static_cast<char>(i)));

    std::string current;
    for (char c : s) {
        current += c;
        if (std::find(dict.begin(), dict.end(), current) == dict.end()) {
            current.pop_back();
            auto it = std::find(dict.begin(), dict.end(), current);
            codes.push_back(static_cast<int>(it - dict.begin()));
            dict.push_back(current + c);
            current = std::string(1, c);
        }
    }
    if (!current.empty()) {
        auto it = std::find(dict.begin(), dict.end(), current);
        codes.push_back(static_cast<int>(it - dict.begin()));
    }
    return codes;
}

inline std::string lzw_decode(const std::vector<int>& codes) {
    std::vector<std::string> dict;
    for (int i = 0; i < 256; ++i)
        dict.push_back(std::string(1, static_cast<char>(i)));

    std::string result;
    std::string prev = dict[codes[0]];
    result += prev;

    for (std::size_t i = 1; i < codes.size(); ++i) {
        std::string entry;
        if (codes[i] < static_cast<int>(dict.size())) {
            entry = dict[codes[i]];
        } else {
            entry = prev + prev[0];
        }
        result += entry;
        dict.push_back(prev + entry[0]);
        prev = entry;
    }
    return result;
}

}  // namespace dsa
