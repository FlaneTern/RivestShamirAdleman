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
#elif 0
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
#else

	std::string plaintext = R"(To be, or not to be: that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune,
Or to take arms against a sea of troubles
And by opposing end them? To die: to sleep;
No more; and by a sleep to say we end
The heart-ache and the thousand natural shocks
That flesh is heir to, 'tis a consummation
Devoutly to be wish'd. To die, to sleep;
To sleep: perchance to dream: ay, there's the rub;
For in that sleep of death what dreams may come
When we have shuffled off this mortal coil,
Must give us pause: there's the respect
That makes calamity of so long life;
For who would bear the whips and scorns of time,
The oppressor's wrong, the proud man's contumely,
The pangs of despised love, the law's delay,
The insolence of office and the spurns
That patient merit of the unworthy takes,
When he himself might his quietus make
With a bare bodkin? who would fardels bear,
To grunt and sweat under a weary life,
But that the dread of something after death,
The undiscover'd country from whose bourn
No traveller returns, puzzles the will
And makes us rather bear those ills we have
Than fly to others that we know not of?
Thus conscience does make cowards of us all;
And thus the native hue of resolution
Is sicklied o'er with the pale cast of thought,
And enterprises of great pith and moment
With this regard their currents turn awry,
And lose the name of action. - Soft you now!
The fair Ophelia! Nymph, in thy orisons
Be all my sins remember'd.
)";
#endif

	bool printDecimal = true;
	bool log = true;

	//RSA rsa;
	//RSA rsa(4);
	RSA rsa("Test length 4");
	rsa.Log(printDecimal);
	std::string ciphertext = rsa.Encrypt(plaintext, log, printDecimal);
	std::string plaintext2 = rsa.Decrypt(ciphertext, log, printDecimal);

	//rsa.Save("Test length 4_2");

	std::cin.get();

	return 0;
}