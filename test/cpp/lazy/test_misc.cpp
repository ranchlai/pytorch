#include <gtest/gtest.h>
#include <string>

#include "torch/csrc/lazy/core/hash.h"
#include "c10/util/int128.h"

namespace torch {
namespace lazy {

template <typename T>
void test_hash_repeatable_sensitive(T example_a, T example_b) {
  // repeatable
  EXPECT_EQ(Hash(example_a), Hash(example_a));

  // sensitive
  EXPECT_NE(Hash(example_a), Hash(example_b));
}

TEST(HashTest, Sanity) {
  // String
  test_hash_repeatable_sensitive(
      std::string(
          "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut at suscipit purus."),
      std::string(
          "Lorem Jpsum dolor sit amet, consectetur adipiscing elit. Ut at suscipit purus."));

  // Number types
  test_hash_repeatable_sensitive(true, false);
  test_hash_repeatable_sensitive((int8_t)0xfa, (int8_t)0xfb);
  test_hash_repeatable_sensitive((int16_t)0xface, (int16_t)0xfade);
  test_hash_repeatable_sensitive((int32_t)0xfaceb000, (int32_t)0xfadeb000);
  test_hash_repeatable_sensitive((int64_t)0x1faceb000, (int64_t)0x1fadeb000);
  test_hash_repeatable_sensitive((uint8_t)0xfa, (uint8_t)0xfb);
  test_hash_repeatable_sensitive((uint16_t)0xface, (uint16_t)0xfade);
  test_hash_repeatable_sensitive((uint32_t)0xfaceb000, (uint32_t)0xfadeb000);
  test_hash_repeatable_sensitive((uint64_t)0x1faceb000, (uint64_t)0x1fadeb000);

  // c10 types
  test_hash_repeatable_sensitive(c10::ScalarType::Bool, c10::ScalarType::Byte);
  test_hash_repeatable_sensitive(c10::Scalar(1.334), c10::Scalar(1.335));
  test_hash_repeatable_sensitive(c10::Scalar(true), c10::Scalar(false));
  test_hash_repeatable_sensitive(c10::Scalar(12345), c10::Scalar(12354));

  // c10::optional
  test_hash_repeatable_sensitive(
      c10::optional<std::string>("I have value!"),
      c10::optional<std::string>(c10::nullopt));

  // Containers
  test_hash_repeatable_sensitive(
      std::vector<int32_t>({0, 1, 1, 2, 3, 5, 8}),
      std::vector<int32_t>({1, 1, 2, 3, 5, 8, 12}));

}

} // namespace lazy
} // namespace torch
