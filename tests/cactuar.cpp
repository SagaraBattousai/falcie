
#include <cstring>

#include <gtest/gtest.h>

extern "C" {
#include <pulse/encoding.h>
}



TEST(CactuarEncoding, CovertZeroData)
{
  int x = 0;
  unsigned char expected[4] = {0, 0, 0, 0};
  unsigned char *actual = convert_data(&x, 4);
  EXPECT_EQ(std::memcmp(actual, expected, 4), 0);
}
