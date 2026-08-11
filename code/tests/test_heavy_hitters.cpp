#include "heavy_hitters.h"
#include <algorithm>
#include <cassert>
#include <print>

int main() {
  dsa::misra_gries<char> sketch(3);
  for (int i = 0; i < 8; ++i) sketch.add('A');
  for (int i = 0; i < 4; ++i) sketch.add('B');
  for (int i = 0; i < 2; ++i) sketch.add('C');
  const auto candidates = sketch.candidates();
  assert(std::find_if(candidates.begin(), candidates.end(),
                      [](const auto& entry) { return entry.first == 'A'; }) != candidates.end());
  assert(sketch.tracked() > 0);
  std::print("heavy hitters: all passed\n");
}
