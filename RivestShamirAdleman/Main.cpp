#include "PCH.h"
#include "BigInt.h"
#include "RivestShamirAdleman.h"

int main() 
{
	std::string plaintext = "";
	std::cout << "Input Plaintext! Write a line \'!!!DONE!!!\' to indicate that you're done inputting the plaintext!\n\n";
	while (!std::cin.eof())
	{
		std::string line;
		std::getline(std::cin, line);

		if (std::cin.fail())
			break;

		if (line == "!!!DONE!!!")
			break;
		else
			plaintext += line + '\n';
	}
	plaintext = plaintext.substr(0, plaintext.length() - 1);

	std::string keyFilepath;
	std::cout << "\n\n\nInput Key File Relative Path!\n\n";
	std::getline(std::cin, keyFilepath);

	bool printDecimal = true;
	bool log = true;

	//RSA rsa;
	//RSA rsa(4);
	RSA rsa(keyFilepath);
	rsa.Log(printDecimal);
	std::string ciphertext = rsa.Encrypt(plaintext, log, printDecimal);
	std::string plaintext2 = rsa.Decrypt(ciphertext, log, printDecimal);

	//rsa.Save("Key Length 1024_2");

	std::cin.get();

	return 0;
}