
#include <gtest/gtest.h>

extern "C" {
#include <pulse/blockheader.h>
}



TEST(PulseBlockheader, BlockheaderSizeIs88)
{
	EXPECT_EQ(sizeof(blockheader_t), 88);
}
