#include "PCH.h"
#include "BigInt.h"

static std::mt19937 s_Random(std::chrono::high_resolution_clock::now().time_since_epoch().count());

BigInt::BigInt() 
    : BigInt(0) {}

BigInt::BigInt(std::vector<uint32_t> arr)
    : m_Value(arr) {}

BigInt::BigInt(uint32_t* ptr, uint32_t length) {
    m_Value.resize(length);
    memcpy(m_Value.data(), ptr, length * sizeof(uint32_t));
}

BigInt::BigInt(const BigInt& other) {
    m_Value = other.m_Value;
}

BigInt::BigInt(std::string str) {
    BigInt ten(std::vector<uint32_t>{ 10 });
    BigInt temp(std::vector<uint32_t>{ (uint32_t)str[0] - '0' });

    for (int i = 1; i < str.length(); i++) {
        BigInt temp2 = BigInt(std::vector<uint32_t>{ (uint32_t)str[i] - '0' });
        temp = temp * ten;
        temp = temp + temp2;
    }

    m_Value = temp.m_Value;
}

BigInt::BigInt(uint32_t i)
    : m_Value({ i }) {}

BigInt BigInt::SubInt(int startIndex, int endIndex) {
    std::vector<uint32_t> temp(endIndex - startIndex + 1);
    for (int i = 0; i <= endIndex - startIndex; i++)
        temp[i] = m_Value[i + startIndex];

    return BigInt(temp);
}

int BigInt::LeadingNonZeroIndex() {
    for (int i = m_Value.size() - 1; i >= 0; i--) {
        if (m_Value[i] != 0)
            return i;
    }
    return -1;
}

void BigInt::RemoveLeadingZeroes() {
    while (m_Value.back() == 0 && m_Value.size() > 1)
        m_Value.pop_back();
}

void BigInt::Shift32(int count) {
    if (count > 0) {
        m_Value.resize(m_Value.size() + count);
        for (int i = m_Value.size() - count - 1; i >= 0; i--)
            m_Value[i + count] = m_Value[i];
        for (int i = 0; i < count; i++)
            m_Value[i] = 0;
    }
    else if (count < 0) {
        for (int i = -1 * count; i < m_Value.size(); i++)
            m_Value[i + count] = m_Value[i];
        m_Value.resize(m_Value.size() + count);
    }
}

BigInt operator+(BigInt a, BigInt b) {
    if (a > b) {
        BigInt temp = b;
        b = a;
        a = temp;
    }

    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();

    std::vector<uint32_t> result(b.Length() + 1);

    int i = 0;

    for (; i < a.Length(); i++) {
        result[i] += a[i] + b[i];
        if (result[i] < a[i]) 
            result[i + 1]++;
    }

    for (; i < b.Length(); i++) {
        result[i] += b[i];
        if (result[i] < b[i]) 
            result[i + 1]++;
    }

    return BigInt(result);
}

BigInt operator-(BigInt a, BigInt b) {
    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();
    if (a < b)
        throw std::runtime_error("Negative Subtraction of BigInt !");

    std::vector<uint32_t> result(a.Length());

    
    bool borrow = false;
    for (int i = 0; i < b.Length(); i++) {
        if (borrow) {
            if (a[i] - 1 < b[i])
                borrow = true;
            else
                borrow = false;
            result[i] = a[i] - b[i] - 1;
        }
        else {
            if (a[i] < b[i])
                borrow = true;
            else
                borrow = false;
            result[i] = a[i] - b[i];
        }
    }

    for (int i = b.Length(); i < a.Length(); i++) {
        result[i] = a[i];
        if (borrow) {
            result[i]--;
            borrow = false;
        }
    }

    return BigInt(result);
}


BigInt operator*(BigInt a, BigInt b) {
    if (a > b) {
        BigInt temp = b;
        b = a;
        a = temp;
    }
    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();

    std::vector<uint32_t> temp(a.Length() + 1);
    std::vector<uint32_t> result(a.Length() + b.Length());

    for (int k = 0; k < b.Length(); k++) {
        temp.resize(a.Length() + 1);
        for (int i = 0; i < a.Length(); i++) {

            uint64_t prod = (uint64_t)a[i] * (uint64_t)b[k];
            uint32_t* low = (uint32_t*)&prod;
            uint32_t* high = (uint32_t*)&prod + 1;

            temp[i] += *low;

            if (temp[i] < *low) 
                temp[i + 1]++;

            temp[i + 1] += *high;

            if (temp[i + 1] < *high) 
                temp[i + 2]++;
        }

        for (int i = 0; i < temp.size(); i++) {
            result[k + i] += temp[i];
            if (result[k + i] < temp[i]) 
                result[k + i + 1]++;
        }
        temp.clear();
    }

    return BigInt(result);
}


BigInt operator/(BigInt a, BigInt b) {
    if (a < b)
        return 0;

    if (b == 0)
        throw std::runtime_error("DIVISION BY ZERO");


    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();

    BigInt f(std::vector<uint32_t>{ (uint32_t)(((uint64_t)1 << 32) / ((uint64_t)b[b.Length() - 1] + 1))});
    a = a * f;
    b = b * f;
    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();


    if (a.Length() == 1 && b.Length() == 1)
        return BigInt(a[0] / b[0]);

    a.PrefixZero();

    int aLengthOri = a.Length();

    std::vector<uint32_t> result;

    for (int i = 0; i < a.Length() - b.Length(); i++) { 
        BigInt idd = a.SubInt(a.Length() - i - 1 - b.Length(), a.Length() - i - 1);

        uint64_t xy = ((uint64_t)idd[idd.Length() - 1] << 32) + (uint64_t)idd[idd.Length() - 2];

        uint64_t d = (xy / (uint64_t)b[b.Length() - 1]);

        for (uint32_t j = 0; j < 3; j++) {
            BigInt di(std::vector<uint32_t>{(uint32_t)d - j});
            BigInt temp = b * di;

            if (idd >= temp) {
                result.push_back(d - j);

                temp.Shift32(a.Length() - b.Length() - i - 1);
                a = a - temp;
                while(a.Length() < aLengthOri)
                    a.PrefixZero();
                break;
            }
                
            if(j == 2)
                throw std::runtime_error("DIVISION IS NOT WORKING =(");
        }
    }

    std::reverse(result.begin(), result.end());

    BigInt resultB(result);

    return BigInt(result);
}

BigInt operator%(BigInt a, BigInt b) {

    a.RemoveLeadingZeroes();
    b.RemoveLeadingZeroes();

    return a - (b * (a / b));
}

BigInt Exponent(BigInt a, BigInt exponent) {
    BigInt res = 1;

    while (exponent > 0) {
        if (exponent[0] & 1)
            res = res * a;
        a = a * a;
        exponent = exponent / 2;
    }

    res.RemoveLeadingZeroes();
    return res;
}

BigInt ModularExponent(BigInt a, BigInt exponent, BigInt mod) {
    BigInt res = 1;
    a = a % mod;

    while (exponent > 0) {
        if (exponent[0] & 1)
            res = (res * a) % mod;
        a = (a * a) % mod;
        exponent = exponent / 2;
    }

    res.RemoveLeadingZeroes();
    return res;
}

BigInt ModularSubtraction(BigInt a, BigInt b, BigInt mod) {
    while (a < b)
        a = a + mod;
    return (a - b) % mod;
}

BigInt GCD(BigInt a, BigInt b) {
    BigInt temp = 0;
    while (b > 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    a.RemoveLeadingZeroes();
    return a;
}

BigInt LCM(BigInt a, BigInt b) {
    BigInt temp = (a * b) / GCD(a, b);
    temp.RemoveLeadingZeroes();
    return temp;
}

BigInt MultiplicativeInverse(BigInt a, BigInt mod) {
    BigInt t = 0;
    BigInt r = mod;
    BigInt newt = 1;
    BigInt newr = a;

    while (newr != 0) {
        BigInt quotient = (r / newr) % mod;

        BigInt temp = newt;
        newt = ModularSubtraction(t, (quotient * newt), mod);
        t = temp % mod;

        temp = newr;
        newr = ModularSubtraction(r, (quotient * newr), mod);
        r = temp % mod;
    }
    
    if (r > 1)
        throw std::runtime_error("NON INVERTIBLE");
    else
        t = t + mod;

    t = t % mod;
    t.RemoveLeadingZeroes();
    return t;
}

BigInt::operator std::string() {
    BigInt temp(*this);

    std::stringstream sstream;

    while (temp > 0) {
        BigInt temp2 = temp % 10;
        sstream << temp2[0];
        temp = temp / 10;
    }

    std::string out = sstream.str();
    std::reverse(out.begin(), out.end());

    return out;


}

void BigInt::PrintBinary() {
    for (int i = this->Length() - 1; i >= 0; i--)
        std::cout << std::bitset<32>(m_Value[i]) << ' ';
    std::cout << '\n';
}

void BigInt::PrintHex() {
    std::cout << std::hex;
    for (int i = this->Length() - 1; i >= 0; i--)
    {
        uint8_t* ptr = (uint8_t*)&m_Value[i];
        for (int j = 3; j >= 0; j--)
            std::cout << std::setw(2) << std::setfill('0') << +*(ptr + j);
        std::cout << ' ';
    }
    std::cout << std::dec;

    std::cout << '\n';
}

void BigInt::PrintAll(bool printDecimal) {
    std::cout << "Binary\t= "; PrintBinary();
    std::cout << "Hex\t= "; PrintHex();
    if (printDecimal)
        std::cout << "Decimal\t= " << (std::string)*this;
    std::cout << "\n\n";
}

bool operator<(BigInt a, BigInt b) {
    if (a.LeadingNonZeroIndex() < b.LeadingNonZeroIndex())
        return true;

    if (a.LeadingNonZeroIndex() > b.LeadingNonZeroIndex())
        return false;   


    for (int i = a.LeadingNonZeroIndex(); i >= 0; i--) {
        if (a[i] < b[i])
            return true;
        else if (a[i] > b[i])
            return false;
    }
    return false;
}

bool operator>(BigInt a, BigInt b) {
    if (a.LeadingNonZeroIndex() > b.LeadingNonZeroIndex())
        return true;

    if (a.LeadingNonZeroIndex() < b.LeadingNonZeroIndex())
        return false;

    for (int i = a.LeadingNonZeroIndex(); i >= 0; i--) {
        if (a[i] > b[i])
            return true;
        else if (a[i] < b[i])
            return false;
    }
    return false;
}

bool operator<=(BigInt a, BigInt b) {
    return !(a > b);
}

bool operator>=(BigInt a, BigInt b) {
    return !(a < b);
}

bool operator==(BigInt a, BigInt b) {
    return !(a > b) && !(a < b);
}

bool operator!=(BigInt a, BigInt b) {
    return !(a == b);
}

BigInt BigInt::GenerateRandom(uint64_t length)
{
    std::vector<uint32_t> arr;
    arr.reserve(length);

    for (int i = 0; i < length; i++)
        arr.push_back(s_Random());

    return BigInt(arr);
}

BigInt BigInt::GenerateRandomPrime(uint64_t length)
{
    BigInt num = GenerateRandom(length);
    num.m_Value[0] |= 1;
    while(!TestPrime(num))
        num = GenerateRandom(length);

    return num;
}

bool BigInt::TestPrime(BigInt num)
{
    BigInt d = num - 1;
    BigInt n1 = num - 1;
    BigInt s = 0;
    
    while (d.Length() > 0 && !(d.m_Value[0] & 1))
    {
        d = d / 2;
        s = s + 1;
    }

    for (int i = 0; i < 3; i++)
    {
        BigInt a = GenerateRandom(1);
        while (a >= n1)
            a = GenerateRandom(1);
        BigInt x = ModularExponent(a, d, num);
        for (BigInt j("0"); j < s; j = j + 1)
        {
            BigInt y = ModularExponent(x, 2, num);
            if (y == 1 && x != 1 && x != n1)
                return false;
            x = y;
        }
        if (x != 1)
            return false;
    }

    return true;
}

std::vector<std::byte> BigInt::ToBinary()
{
    std::vector<std::byte> bytes;
    size_t bytesSize = bytes.size();

    uint32_t length = Length();
    bytes.resize(bytesSize + sizeof(length));
    std::memcpy(&bytes[bytesSize], &length, sizeof(length));
    bytesSize = bytes.size();
    
    bytes.resize(bytes.size() + Length() * sizeof(m_Value[0]));
    std::memcpy(&bytes[bytesSize], &m_Value[0], Length() * sizeof(m_Value[0]));
    bytesSize = bytes.size();

    return bytes;
}

BigInt::BigInt(std::byte* ptr, uint32_t& outOffset)
{
    uint32_t length;
    std::memcpy(&length, ptr, sizeof(length));

    m_Value.resize(length);
    memcpy(m_Value.data(), ptr + sizeof(length), length * sizeof(uint32_t));

    outOffset += sizeof(length) + length * sizeof(uint32_t);
}