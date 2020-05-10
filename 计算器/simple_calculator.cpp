#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
using namespace std;
enum SignType
{
	PLUS,
	MINUS
};
#define maxn 200

class Fraction
{
	string denominator, numerator;
	SignType sign;
	friend class BigNum;
	friend ostream &operator<<(ostream &out, Fraction);

public:
	Fraction(string n = "", string d = "", SignType s = PLUS) : numerator(n), denominator(d), sign(s) {}
};

class BigNum
{
	string value;
	SignType sign;

public:
	BigNum(string v = "", SignType s = PLUS) : value(v), sign(s) {}
	BigNum(const string &);
	BigNum(const BigNum &);
	~BigNum(){};
	BigNum operator=(const BigNum &);
	BigNum operator+(const BigNum &) const;
	BigNum operator-(const BigNum &) const;
	BigNum operator*(const BigNum &)const;
	BigNum operator/(const BigNum &) const;
	BigNum operator%(const BigNum &) const;
	BigNum operator^(const unsigned long long &) const;
	bool operator>(const BigNum &) const;
	bool operator<(const BigNum &) const;
	bool operator==(const BigNum &) const;
	bool frac_part(const BigNum &) const;
	unsigned long long convert() const;

	friend class Fraction;
	friend istream &operator>>(istream &in, BigNum &);
	friend ostream &operator<<(ostream &out, BigNum);
};

ostream &operator<<(ostream &out, Fraction T)
{
	SignType sign = T.sign;
	string numerator = T.numerator, denominator = T.denominator;
	if (sign == MINUS && numerator != "0")
		out << "-";
	if (numerator == "0")
		out << "0";
	else
		out << numerator << "/" << denominator;
	return out;
}

BigNum::BigNum(const BigNum &T)
{
	value = T.value;
	sign = T.sign;
}

BigNum::BigNum(const string &T)
{
	if ('-' == T[0])
	{
		sign = MINUS;
		value = T.substr(1, T.size() - 1);
	}
	else
	{
		sign = PLUS;
		value = T;
	}
}

istream &operator>>(istream &in, BigNum &T)
{
	string s;
	in >> s;
	if (s[0] == '-')
	{
		T.value = s.substr(1, s.size() - 1);
		T.sign = MINUS;
	}
	else if (s[0] == '+')
	{
		T.value = s.substr(1, s.size() - 1);
		T.sign = PLUS;
	}
	else
	{
		T.value = s;
		T.sign = PLUS;
	}
	return in;
}

ostream &operator<<(ostream &out, BigNum T)
{
	if (T.sign == MINUS && T.value != "0")
		out << '-';
	out << T.value;
	return out;
}

int compare(int a[], int b[])
{
	if (a[0] > b[0])
		return 1;
	if (a[0] < b[0])
		return -1;
	for (int i = a[0]; i > 0; i--)
	{
		if (a[i] > b[i])
			return 1;
		if (a[i] < b[i])
			return -1;
	}
	return 0;
}

bool BigNum::operator==(const BigNum &T) const
{
	string a = this->value, b = T.value;
	if (a.size() != b.size())
		return false;
	for (long i = 0; i < a.size(); i++)
		if (a.at(i) != b.at(i))
			return false;
	return true;
}

bool BigNum::operator>(const BigNum &T) const
{
	string a = this->value, b = T.value;
	if (a.length() > b.length())
		return true;
	if (a.length() < b.length())
		return false;
	for (long i = 0; i < a.length(); i++)
	{
		if (a.at(i) > b.at(i))
			return true;
		if (a.at(i) < b.at(i))
			return false;
	}
	return false;
}

bool BigNum::operator<(const BigNum &T) const
{
	string a = this->value, b = T.value;
	if (a.length() < b.length())
		return true;
	if (a.length() > b.length())
		return false;
	for (long i = 0; i < a.length(); i++)
	{
		if (a.at(i) < b.at(i))
			return true;
		if (a.at(i) > b.at(i))
			return false;
	}
	return false;
}

BigNum BigNum::operator=(const BigNum &T)
{
	this->value = T.value;
	this->sign = T.sign;
	return *this;
}

BigNum BigNum::operator+(const BigNum &T) const
{
	BigNum a = *this, b = T;
	string temp = "";
	if (b.sign == MINUS && a.sign == PLUS)
	{
		b.sign = PLUS;
		return a - b;
	}
	else if (b.sign == PLUS && a.sign == MINUS)
	{
		a.sign = PLUS;
		return b - a;
	}
	if (a.value.size() < b.value.size())
	{
		string tmp = a.value;
		a.value = b.value;
		b.value = tmp;
	}
	int flag = 0, lengtha = a.value.size(), lengthb = b.value.size(), i = 0;
	reverse(a.value.begin(), a.value.end());
	reverse(b.value.begin(), b.value.end());
	while (lengtha > 0)
	{
		char sum;
		if (lengthb > 0)
			sum = a.value[i] + b.value[i] - '0' - '0' + flag;
		else
			sum = a.value[i] - '0' + flag;
		temp.push_back(sum % 10 + '0');
		if (sum / 10 == 1)
			flag = 1;
		else
			flag = 0;
		i++;
		lengtha--;
		lengthb--;
	}
	if (flag == 1)
		temp.push_back('1');
	reverse(temp.begin(), temp.end());
	BigNum res;
	res.value = temp;
	if (T.sign == PLUS && this->sign == PLUS)
		res.sign = PLUS;
	else
		res.sign = MINUS;
	return res;
}

BigNum BigNum::operator-(const BigNum &T) const
{
	BigNum tempa = *this, tempb = T, res("", PLUS);
	string a = tempa.value, b = tempb.value;
	if (tempa.sign == PLUS && tempb.sign == MINUS)
	{
		tempb.sign = PLUS;
		return tempa + tempb;
	}
	else if (tempa.sign == MINUS && tempb.sign == PLUS)
	{
		tempb.sign = MINUS;
		return tempa + tempb;
	}
	if (tempa == tempb)
	{
		res.value = "0";
		return res;
	}
	if (tempa < tempb)
	{
		string temp = a;
		a = b;
		b = temp;
		if (tempa.sign == PLUS && tempb.sign == PLUS)
			res.sign = MINUS;
		if (tempa.sign == MINUS && tempb.sign == MINUS)
			res.sign = PLUS;
	}
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	std::size_t i;
	for (i = 0; i < b.length(); i++)
		b.at(i) -= '0';
	for (i = 0; i < a.length(); i++)
		a.at(i) -= '0';
	for (i = 0; i < b.length(); i++)
		a.at(i) -= b.at(i);
	for (i = 0; i < a.length() - 1; i++)
		if (a.at(i) < 0)
		{
			a.at(i) += 10;
			a.at(i + 1)--;
		}
	for (i = a.length() - 1; i >= 0; i--)
		if (a.at(i) > 0)
			break;
	a.erase(i + 1, a.length());
	for (i = 0; i < a.length(); i++)
		if (a.at(i) < 10)
			a.at(i) += '0';
	reverse(a.begin(), a.end());
	res.value = a;
	return res;
}

BigNum BigNum::operator*(const BigNum &T) const
{
	BigNum res;
	string a = this->value, b = T.value, s(1000, '0');
	if (this->sign == T.sign)
		res.sign = PLUS;
	else
		res.sign = MINUS;
	reverse(a.begin(), a.end());
	reverse(b.begin(), b.end());
	for (int i = 0; i < a.length(); i++)
		for (int j = 0; j < b.length(); j++)
		{
			int temp = (a[i] - '0') * (b[j] - '0');
			s[i + j + 1] = s[i + j + 1] - '0' + (s[i + j] - '0' + temp) / 10 + '0';
			s[i + j] = (s[i + j] - '0' + temp) % 10 + '0';
		}
	reverse(s.begin(), s.end());
	if (s.find_first_not_of('0') == string::npos)
		res.value = "0";
	else
		res.value = s.substr(s.find_first_not_of('0'));
	return res;
}

BigNum BigNum::operator/(const BigNum &T) const
{
	BigNum res;
	res.value = "0";
	try
	{
		if (T.value == "0")
			throw 'e';
	}
	catch (char e)
	{
		cout << "the divisor can't be zero!";
		return res;
	}
	if (this->sign == T.sign)
		res.sign = PLUS;
	else
		res.sign = MINUS;
	string x = this->value, y = T.value;
	int a[200], b[200], c[200], temp[200];
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	memset(c, 0, sizeof(c));
	a[0] = x.length();
	b[0] = y.length();
	for (int i = 1; i <= a[0]; i++)
		a[i] = x.at(a[0] - i) - '0';
	for (int i = 1; i <= b[0]; i++)
		b[i] = y.at(b[0] - i) - '0';
	c[0] = a[0] - b[0] + 1;
	for (int i = c[0]; i > 0; i--)
	{
		memset(temp, 0, sizeof(temp));
		for (int j = 1; j <= b[0]; j++)
			temp[j + i - 1] = b[j];
		temp[0] = b[0] + i - 1;
		while (compare(a, temp) >= 0)
		{
			c[i]++;
			int flag = compare(a, temp);
			if (flag == 0)
				a[0] = 0;
			if (flag == 1)
			{
				for (long i = 1; i <= a[0]; i++)
				{
					if (a[i] < temp[i])
					{
						a[i + 1]--;
						a[i] += 10;
					}
					a[i] -= temp[i];
				}
				while (a[0] > 0 && a[a[0]] == 0)
					a[0]--;
			}
		}
	}
	while (c[0] > 0 && c[c[0]] == 0)
		c[0]--;
	long int i;
	if (c[0] == 0)
		res.value = "0";
	else
	{
		for (i = c[0]; i > 0; i--)
		{
			c[i] += '0';
			res.value += c[i];
		}
		res.value.erase(0, 1);
	}
	if (res.value == "")
		res.value = "0";
	return res;
}

BigNum BigNum::operator%(const BigNum &T) const
{
	BigNum res;
	res.value = "0";
	try
	{
		if (T.value == "0")
			throw 'e';
	}
	catch (char e)
	{
		cout << "the divisor can't be zero!";
		return res;
	}
	if (this->sign == T.sign)
		res.sign = PLUS;
	else
		res.sign = MINUS;
	string x = this->value, y = T.value;
	int a[maxn], b[maxn], c[maxn], temp[maxn];
	memset(a, 0, sizeof(a));
	memset(b, 0, sizeof(b));
	memset(c, 0, sizeof(c));
	a[0] = x.length();
	b[0] = y.length();
	for (int i = 1; i <= a[0]; i++)
		a[i] = x.at(a[0] - i) - '0';
	for (int i = 1; i <= b[0]; i++)
		b[i] = y.at(b[0] - i) - '0';
	c[0] = a[0] - b[0] + 1;
	for (int i = c[0]; i > 0; i--)
	{
		memset(temp, 0, sizeof(temp));
		for (int j = 1; j <= b[0]; j++)
			temp[j + i - 1] = b[j];
		temp[0] = b[0] + i - 1;
		while (compare(a, temp) >= 0)
		{
			c[i]++;
			int flag = compare(a, temp);
			if (flag == 0)
				a[0] = 0;
			if (flag == 1)
			{
				for (long i = 1; i <= a[0]; i++)
				{
					if (a[i] < temp[i])
					{
						a[i + 1]--;
						a[i] += 10;
					}
					a[i] -= temp[i];
				}
				while (a[0] > 0 && a[a[0]] == 0)
					a[0]--;
			}
		}
	}
	long int i;
	if (a[0] == 0)
		res.value = "0";
	else
	{
		for (i = a[0]; i > 0; i--)
		{
			a[i] += '0';
			res.value += a[i];
		}
		res.value.erase(0, 1);
	}
	return res;
}

BigNum BigNum::operator^(const unsigned long long &T) const
{
	BigNum t, res;
	res.value = "1";
	if (T < 0)
	{
		res.value = "0";
		return res;
	}
	else if (T == 1)
	{
		return *this;
	}
	else if (T == 0)
		return res;
	long n = T, i;
	while (n > 1)
	{
		t = *this;
		for (i = 1; i << 1 <= n; i <<= 1)
			t = t * t;
		n -= i;
		res = res * t;
		if (n == 1)
			res = res * (*this);
	}
	return res;
}

unsigned long long BigNum::convert() const
{
	BigNum max("18446744073709551615", PLUS), min("0", PLUS);
	unsigned long long len = this->value.length(), res = 0, i = 0;
	string temp = this->value;
	char ch;
	if (temp == "0")
		return 0;
	try
	{
		if (*this<max &&*this> min)
		{
			while (i < len)
			{
				res *= 10;
				res += long(temp[i] - '0');
				i++;
			}
			return res;
		}
		else
			throw 1;
	}
	catch (int)
	{
		cout << "Order Too Big";
		return 0;
	}
}

bool BigNum::frac_part(const BigNum &T) const
{
	BigNum a = *this, b = T, c;
	c.value = "10";
	int count = 20;
	cout << ".";
	while (count--)
	{
		a = a * c;
		BigNum temp = a / b;
		cout << temp.value;
		a = a % b;
	}
	return true;
}

int main()
{
	BigNum a, b;
	char ch;
	for (;;)
	{
		cin >> a >> ch >> b;
		if (ch == '+')
			cout << a + b;
		if (ch == '-')
			cout << a - b;
		if (ch == '*')
			cout << a * b;
		if (ch == '/')
		{
			cout << a / b;
			BigNum tempa = a % b;
			cout << tempa.frac_part(b);
		}
		if (ch == '%')
			cout << a % b;
		if (ch == '^')
		{
			long n = b.convert();
			BigNum res = a ^ n;
			cout << res;
		}
	}
	return 0;
}
