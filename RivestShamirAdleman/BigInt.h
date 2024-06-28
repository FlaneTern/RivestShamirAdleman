#pragma once


class BigInt {
public:
    BigInt();
    BigInt(std::vector<uint32_t> arr);
    BigInt(uint32_t* ptr, uint32_t length);
    BigInt(const BigInt& other);
    BigInt(std::string str);
    BigInt(uint32_t i);

    inline std::vector<uint32_t> Value() const { return m_Value; }
    inline uint32_t Length() const { return m_Value.size(); }
    inline uint32_t* Data() { return m_Value.data(); }
    inline uint32_t& operator[](uint32_t i) { return m_Value[i]; }
    inline uint32_t& operator[](int i) { return m_Value[i]; }
    void RemoveLeadingZeroes();
    inline void PrefixZero() { m_Value.push_back(0); }
    void Shift32(int count);
    int LeadingNonZeroIndex();

    BigInt SubInt(int startIndex, int endIndex);
    explicit operator std::string();
    void PrintBinary();
    void PrintHex();
    void PrintAll(bool printDecimal);

    std::vector<std::byte> ToBinary();

    static BigInt GenerateRandom(uint64_t length);
    static BigInt GenerateRandomPrime(uint64_t length);
    static bool TestPrime(BigInt num);

private:
    std::vector<uint32_t> m_Value;
};

BigInt Exponent(BigInt a, BigInt exponent);
BigInt ModularExponent(BigInt a, BigInt exponent, BigInt mod);
BigInt ModularSubtraction(BigInt a, BigInt b, BigInt mod);
BigInt MultiplicativeInverse(BigInt a, BigInt mod);
BigInt operator+(BigInt a, BigInt b);
BigInt operator-(BigInt a, BigInt b);
BigInt operator*(BigInt a, BigInt b);
BigInt operator/(BigInt a, BigInt b);
BigInt operator%(BigInt a, BigInt b);
bool operator<(BigInt a, BigInt b);
bool operator>(BigInt a, BigInt b);
bool operator>=(BigInt a, BigInt b);
bool operator>=(BigInt a, BigInt b);
bool operator==(BigInt a, BigInt b);
bool operator!=(BigInt a, BigInt b);
BigInt GCD(BigInt a, BigInt b);
BigInt LCM(BigInt a, BigInt b);
