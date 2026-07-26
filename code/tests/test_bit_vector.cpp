#include "bit_vector.h"
#include <cassert>
#include <print>

int main() {
  dsa::BitVector bv(64);
  assert(bv.size() == 64);
  assert(bv.none());

  bv.set(0);
  bv.set(3);
  bv.set(63);
  assert(bv.get(0));
  assert(!bv.get(1));
  assert(bv.get(3));
  assert(bv.get(63));
  assert(bv.count() == 3);

  bv.flip(0);
  assert(!bv.get(0));
  bv.reset(3);
  assert(!bv.get(3));
  assert(bv.count() == 1);

  dsa::BitVector bv2(8);
  bv2.set(1);
  bv2.set(3);
  bv2.set(5);

  dsa::BitVector bv3(8);
  bv3.set(1);
  bv3.set(4);
  bv3.set(5);

  auto and_result = bv2 & bv3;
  assert(and_result.get(1));
  assert(and_result.get(5));
  assert(!and_result.get(3));
  assert(and_result.count() == 2);

  auto or_result = bv2 | bv3;
  assert(or_result.count() == 4);

  auto xor_result = bv2 ^ bv3;
  assert(xor_result.count() == 2);
  assert(xor_result.get(3));
  assert(xor_result.get(4));

  auto not_result = ~bv2;
  assert(not_result.get(0));
  assert(!not_result.get(1));
  assert(not_result.get(2));

  assert(bv2.find_first() == 1);
  assert(bv2.find_next(1) == 3);
  assert(bv2.find_next(3) == 5);
  assert(bv2.find_next(5) == 8);

  dsa::BitVector empty(100);
  assert(empty.find_first() == 100);

  dsa::BitVector large(200);
  large.set(100);
  large.set(199);
  assert(large.find_first() == 100);
  assert(large.find_next(100) == 199);

  std::print("bit_vector: all passed\n");
}
