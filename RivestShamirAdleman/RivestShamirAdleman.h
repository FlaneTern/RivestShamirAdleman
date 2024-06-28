#pragma once

class RSA
{
public:
	RSA();
	RSA(uint32_t primeLength);
	std::string Encrypt(std::string plaintext, bool log, bool printDecimal);
	std::string Decrypt(std::string ciphertext, bool log, bool printDecimal);

	void Log(bool printDecimal);

private:

	std::vector<BigInt> TextToBigInt(std::string text, uint32_t length);
	std::string BigIntToText(std::vector<BigInt> bigInts, uint32_t length);

	BigInt m_P;
	BigInt m_Q;
	BigInt m_N;
	BigInt m_LambdaN;
	BigInt m_E;
	BigInt m_D;
};