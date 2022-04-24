
#include <time.h>

#include <etro/blockheader.h>
#include <etro/encoding.h>

#define NANO_TO_MILLI 1000000

void mine(blockheader_t* const header)
{
	//TODO: add error for invalid cactaur values
/*difficultyArray: = blockHeader.Target.As256Bit()

headerHash : = blockHeader.Hash()

for bytes.Compare(headerHash[:], difficultyArray[:]) != -1 {
	blockHeader.Nonce += 1
		headerHash = blockHeader.Hash()
}
*/

}

sha256hash_t hash(const blockheader_t* const header)
{
	/*//Double 256Hash
	func(blockHeader * BlockHeader) Hash()[sha256.Size]byte{
		hash: = sha256.New()

		//Encode not casting using unsafe ptr as MUST be little endian
		versionBytes : = cactuar.Encode32(blockHeader.Version)
		timestampBytes : = cactuar.Encode64(blockHeader.Timestamp)
		targetBytes : = cactuar.Encode32(blockHeader.Target)
		nonceBytes : = cactuar.Encode32(blockHeader.Nonce)

		hash.Write(versionBytes[:])
		hash.Write(timestampBytes[:])
		hash.Write(blockHeader.Prevhash[:])
		hash.Write(blockHeader.Transhash[:])
		hash.Write(targetBytes[:])
		hash.Write(nonceBytes[:])

		return sha256.Sum256(hash.Sum(nil))
	}*/
	sha256hash_t tmp;
	return tmp;
}

uint64_t generate_timestamp()
{
	struct timespec ts;
	if (timespec_get(&ts, TIME_UTC) == 0)
	{
		return 0;
	}
	uint64_t timestamp = ((uint64_t)ts.tv_sec) * 1000;
	timestamp += (uint64_t)(ts.tv_nsec / NANO_TO_MILLI);
	return timestamp;
}