#include "PCH.h"
#include "BigInt.h"
#include "RivestShamirAdleman.h"

int main() {

#if 0
	std::string plaintext = R"(Once upon a time, in a small village nestled between rolling hills, there lived a young girl named Lila. Lila was known far and wide for her kindness and her love for animals. She had a special bond with a wounded sparrow she had rescued and nursed back to health, naming him Pip.

		One day, as Lila was gathering herbs in the forest, she heard a faint cry for help.Following the sound, she stumbled upon a tiny fairy trapped in a thorny bush.The fairy's name was Elara, and she had been searching for her lost wand.

		With her gentle touch, Lila freed Elara from the bushand promised to help her find the wand.Together, they searched highand low, through thicketsand streams, until they finally found it in a clearing bathed in golden sunlight.

		Elara was overjoyedand thanked Lila by granting her a wish.Lila selflessly wished for the forest to always be a safe haven for all creatures, bigand small.Elara smiledand granted her wish, sprinkling magic dust over the forest.

		From that day on, the forest became a place of peaceand harmony, where animalsand fairies lived together in perfect balance.And Lila, with Pip by her sideand Elara watching over them, continued to spread kindnessand love wherever she went.

		And so, the story of Lila, the girl who saved the forest, became legend, a reminder of the power of kindnessand the magic that lies within us all.)";
#else
	std::string plaintext = R"(The quick brown fox jumps over the lazy dog. The quick
brown fox jumps over the lazy dog. The quick brown fox
jumps over the lazy dog. The quick brown fox jumps over
the lazy dog. The quick brown fox jumps over the lazy dog.
The quick brown fox jumps over the lazy dog. The quick
brown fox jumps over the lazy dog. The quick brown fox
jumps over the lazy dog. The quick brown fox jumps over
the lazy dog. The quick brown fox jumps over the lazy dog.
The quick brown fox jumps over the lazy dog. The quick
brown fox jumps over the lazy dog.)";
#endif

	bool printDecimal = true;

	RSA rsa;
	rsa.Log(printDecimal);
	std::string ciphertext = rsa.Encrypt(plaintext, true, printDecimal);
	std::string plaintext2 = rsa.Decrypt(ciphertext, true, printDecimal);

	std::cin.get();

	return 0;
}