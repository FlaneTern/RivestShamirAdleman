#include "PCH.h"
#include "BigInt.h"
#include "RivestShamirAdleman.h"

RSA::RSA()
	: RSA(4){}


RSA::RSA(uint32_t primeLength)
{
	std::cout << "Generating P..." << '\n';
	m_P = BigInt::GenerateRandomPrime(4);
	std::cout << "Generating Q..." << '\n';
	m_Q = BigInt::GenerateRandomPrime(4);
	m_N = m_P * m_Q;
	m_LambdaN = LCM(m_P - 1, m_Q - 1);
	m_E = 65537;
	m_D = MultiplicativeInverse(m_E, m_LambdaN);
}

std::string RSA::Encrypt(std::string plaintext, bool log, bool printDecimal)
{
	if (log)
		std::cout << "Starting Encryption of Plaintext :\n" << plaintext << "\n\n\n";

	std::vector<BigInt> plaintexts = TextToBigInt(plaintext, m_N.Length() - 1);

	if (log)
	{
		std::cout << "Plaintext was split into " << plaintexts.size() << " blocks.\n";
		std::cout << "Plaintext Blocks :\n";
		for (int i = 0; i < plaintexts.size(); i++)
		{
			std::cout << "Block " << i << " :\n";
			plaintexts[i].PrintAll(printDecimal);
		}
		std::cout << "\n\n";
	}

	std::vector<BigInt> ciphertexts;
	ciphertexts.reserve(plaintexts.size());

	for (int i = 0; i < plaintexts.size(); i++)
		ciphertexts.push_back(ModularExponent(plaintexts[i], m_E, m_N));

	if (log)
	{
		std::cout << "Encrypted Ciphertext Blocks :\n";
		for (int i = 0; i < ciphertexts.size(); i++)
		{
			std::cout << "Block " << i << " :\n";
			ciphertexts[i].PrintAll(printDecimal);
		}
	}

	std::string ciphertext = BigIntToText(ciphertexts, m_N.Length());

	std::cout << "Encrypted Ciphertext :\n";
	std::cout << ciphertext << "\n\n\n";

	return ciphertext;
}

std::string RSA::Decrypt(std::string ciphertext, bool log, bool printDecimal)
{
	if (log)
		std::cout << "Starting Decryption of Ciphertext :\n" << ciphertext << "\n\n\n";

	std::vector<BigInt> ciphertexts = TextToBigInt(ciphertext, m_N.Length());

	if (log)
	{
		std::cout << "Ciphertext was split into " << ciphertexts.size() << " blocks.\n";
		std::cout << "Ciphertext Blocks :\n";
		for (int i = 0; i < ciphertexts.size(); i++)
		{
			std::cout << "Block " << i << " :\n";
			ciphertexts[i].PrintAll(printDecimal);
		}
	}

	std::vector<BigInt> plaintexts;
	plaintexts.reserve(ciphertexts.size());

	for (int i = 0; i < ciphertexts.size(); i++)
		plaintexts.push_back(ModularExponent(ciphertexts[i], m_D, m_N));

	if (log)
	{
		std::cout << "Decrypted Plaintext Blocks :\n";
		for (int i = 0; i < plaintexts.size(); i++)
		{
			std::cout << "Block " << i << " :\n";
			plaintexts[i].PrintAll(printDecimal);
		}
		std::cout << "\n\n";
	}

	std::string plaintext = BigIntToText(plaintexts, m_N.Length() - 1);

	std::cout << "Decrypted Plaintext :\n";
	std::cout << plaintext << "\n\n\n";

	return plaintext;
}

void RSA::Log(bool printDecimal)
{
	std::cout << "P\n";
	m_P.PrintAll(printDecimal);

	std::cout << "Q\n";
	m_Q.PrintAll(printDecimal);

	std::cout << "N\n";
	m_N.PrintAll(printDecimal);

	std::cout << "Lambda(N)\n";
	m_LambdaN.PrintAll(printDecimal);

	std::cout << "E\n";
	m_E.PrintAll(printDecimal);

	std::cout << "D\n";
	m_D.PrintAll(printDecimal);
}

std::vector<BigInt> RSA::TextToBigInt(std::string text, uint32_t length)
{
	uint32_t byteLength = sizeof(uint32_t) * length;
	text += std::string((byteLength)-(text.length() % byteLength), '0');

	std::vector<BigInt> bigInts;
	bigInts.reserve(text.length() / byteLength);

	for (int i = 0; i < text.length(); i += byteLength)
		bigInts.push_back(BigInt((uint32_t*)&text[i], length));

	return bigInts;
}

std::string RSA::BigIntToText(std::vector<BigInt> bigInts, uint32_t length)
{
	uint32_t byteLength = sizeof(uint32_t) * length;
	std::string text(bigInts.size() * byteLength, '0');

	for (int i = 0; i < bigInts.size(); i++)
		memcpy(text.data() + byteLength * i, bigInts[i].Data(), byteLength);

	return text;
}

std::string RelativePathToAbsolutePath(std::string path)
{
	std::cout << "path of " << path << " = " << std::filesystem::current_path().string() + "\\" + path << '\n';
	return std::filesystem::current_path().string() + "\\" + path;
}

RSA::RSA(std::string filePath)
{
	std::ifstream file(RelativePathToAbsolutePath(filePath), std::ios::binary);

	file.seekg(0, std::ios::end);
	size_t length = file.tellg();
	file.seekg(0, std::ios::beg);

	std::vector<std::byte> binary(length);
	std::byte* ptr = binary.data();

	file.read((char*)ptr, length);

	file.close();

	uint32_t offset = 0;

	m_P = BigInt(ptr + offset, offset);
	m_Q = BigInt(ptr + offset, offset);
	m_N = BigInt(ptr + offset, offset);
	m_LambdaN = BigInt(ptr + offset, offset);
	m_E = BigInt(ptr + offset, offset);
	m_D = BigInt(ptr + offset, offset);
}

void RSA::Save(std::string filePath)
{
	std::ofstream file(RelativePathToAbsolutePath(filePath), std::ios::binary);

	std::vector<std::byte> binary;

	binary = m_P.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	binary = m_Q.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	binary = m_N.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	binary = m_LambdaN.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	binary = m_E.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	binary = m_D.ToBinary();
	file.write((const char*)binary.data(), binary.size());

	file.close();
}