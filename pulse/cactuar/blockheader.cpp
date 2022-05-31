
module cactuar:blockheader;

namespace pulse
{
	const hash_type PulseHash(const Blockheader& header)
	{
		//hash_type hash(HashType::SHA256);
		Hasher< HashType::SHA256> blockheader_hasher{};

		unsigned char data[sizeof(Blockheader)];

		std::memcpy(data, &header, sizeof(Blockheader));

		//EncodeData({ data });
		
		EncodeData({ data, sizeof(Blockheader) });
		
		//Change to operator() instead of Hash ?!!
		return blockheader_hasher.Hash(data);
	}


} // namespace pulse