#include <iostream>
#include "string.hpp"

using namespace std;

//constructors and destructors
String::String(const char *s)
    : buf(strdup(s)) {}

String::String(const String &s)
    : buf(strdup(s.buf)) {}

String::String(int length)
    : buf(new char[length + 1]) {}

String::~String()
{
    delete[] buf;
    buf = nullptr;
}

//class function definitions
String& String::operator=(String s)
{
    if (this == &s)
        return *this;

    delete[] buf;
    buf = strdup(s.buf);
    return *this;
}

char& String::operator[](int index)
{
    if (index >= strlen(buf))
    {
        cout << "ERROR: Index Out Of Bounds" << endl;
        return buf[0];
    }
    return buf[index];
}

const char& String::operator[](int index) const
{
    if (index >= strlen(buf))
    {
        cout << "ERROR: Index Out Of Bounds" << endl;
        return buf[0];
    }
    return buf[index];
}

int String::size() const
{
    return strlen(buf);
}

String String::reverse() const
{
    
    String reversed_string(strlen(buf) + 1);
    reverse_cpy(reversed_string.buf, buf);

    return reversed_string;
}

int String::indexOf(char c) const
{
    int status{static_cast<int>(strchr(buf, c) - buf)};
    return ((status >= 0 && status < strlen(buf)) ? status : -1);
}

int String::indexOf(String s) const
{
    int status{static_cast<int>(strstr(buf, s.buf) - buf)};
    return ((status >= 0 && status < strlen(buf)) ? status : -1);
}

void String::print(std::ostream &out) const
{
    for(int i = 0; buf[i] != '\0'; i++)
    {
        out << buf[i];
    }
}

void String::read(std::istream &in)
{
    int length{0};
    char next_char;
    char* temp{new char[1]{'\0'}};

    while (in.get(next_char))
    {
        if (next_char == ' ' || next_char == '\n')
            break;

        length++;
        delete[] buf;
        buf = new char[length + 1]{};
        for(int j = 0; temp[j] != '\0'; j++)
        {
            buf[j] = temp[j];
        }
        buf[length - 1] = next_char;
        buf[length] = '\0';
        delete[] temp;
        temp = strdup(buf);
    }
    delete[] temp;
}

//operator function overloads
bool String::operator==(String s) const
{
    return !strcmp(buf, s.buf);
}

bool String::operator!=(String s) const
{
    return strcmp(buf, s.buf);
}

bool String::operator>(String s) const
{
    return (strcmp(buf, s.buf) > 0);
}

bool String::operator<(String s) const
{
    return (strcmp(buf, s.buf) < 0);
}

bool String::operator>=(String s) const
{
    return (strcmp(buf, s.buf) >= 0);
}

bool String::operator<=(String s) const
{
    return (strcmp(buf, s.buf) <= 0);
}

String String::operator+(String s) const
{
    String new_string(0);
    strcat(new_string.buf, buf);
    strcat(new_string.buf, s.buf);
    return new_string;
}

String& String::operator+=(String s)
{
    strcat(buf, s.buf);
    return *this;
}

//static function definitions
int String::strlen(const char *s)
{
    int i = 0;
    while (s[i] != '\0')
    {
        i++;
    }
    return i;
}

char* String::strcpy(char *dest, const char *src)
{
    int i = 0;
    for( ; src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return dest;
}

char* String::strdup(const char* src)
{
    char* temp_array{new char[strlen(src) + 1]{}};
    strcpy(temp_array, src);
    return temp_array;
}

char* String::strncpy(char *dest, const char *src, int n)
{
    int i = 0;
    for( ; src[i] != '\0' && i < n; i++)
    {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return dest;
}

char* String::strcat(char *dest, const char *src)
{
    int length_dest = strlen(dest);

    char* temp = new char[strlen(src) + 1]{};
    strcpy(temp, dest);

    delete[] dest;
    dest = new char[length_dest + strlen(src) + 1]{};

    int i = 0;
    for(int j = 0; temp[j] != '\0'; j++)
        dest[j] = temp[j];
    for( ; src[i] != '\0'; i++)
        dest[length_dest + i] = src[i];

    dest[length_dest + i] = '\0';

    delete[] temp;
    return dest;
}

char* String::strncat(char *dest, const char *src, int n)
{
    int length_dest = strlen(dest);
    int length_src = strlen(src);

    char* temp = new char[strlen(src) + 1];
    strcpy(temp, dest);

    delete[] dest;
    if (n > length_src)
        dest = new char[length_dest + length_src + 1]{};
    else
        dest = new char[length_dest + n + 1]{};

    int i = 0;
    for(int j = 0; temp[j] != '\0'; j++)
        dest[j] = temp[j];
    for( ; src[i] != '\0' && i < n; i++)
        dest[length_dest + i] = src[i];

    dest[length_dest + i] = '\0';
    return dest;
}

int String::strcmp(const char* left, const char* right)
{
    for(int i = 0; left[i] != '\0'; i++)
    {
        if (right[i] == '\0')
            return 1;
        else if (left[i] != right[i])
            return left[i] - right[i];
    }
    if (strlen(left) < strlen(right))
        return -1;
    return 0;
}

int String::strncmp(const char* left, const char* right, int n)
{
    for(int i = 0; i < n && left[i] != '\0'; i++)
    {
        if (right[i] == '\0')
            return 1;
        else if (left[i] > right[i])
            return 1;
        else if (left[i] < right[i])
            return -1;
    }

    int length = strlen(left);
    if (n > length && length < strlen(right))
    {
        return -1;
    }
    return 0;
}

void String::reverse_cpy(char *dest, const char *src)
{
    int length = strlen(src);
    int i = 0;
    for( ; src[i] != '\0'; i++)
    {
        dest[i] = src[length - i - 1];
    }
    dest[i] = '\0';
}

const char* String::strchr(const char *str, char c)
{
    if (c == '\0')
        return &str[strlen(str)];
    for(int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == c)
            return &str[i];
    }
    return nullptr;
}

const char* String::strstr(const char *haystack, const char *needle)
{
    int needle_length = strlen(needle);
    int haystack_length = strlen(haystack);

    if(needle_length == 0)
        return haystack;

    if (needle_length > haystack_length)
        return nullptr;

    for(int i = 0, j = 0; i < haystack_length; i++)
    {
        if (haystack[i] == needle[j])
        {
            j++;
            if (j == needle_length)
                return &haystack[i - j + 1];
        }
        else if (haystack_length - i < needle_length)
        {
            return nullptr;
        }
        else if (haystack[i] == needle[0])
            j = 1;
        else
            j = 0;
    }
    return nullptr;
}

//operator redefined for strings
std::ostream &operator<<(std::ostream &out, String s)
{
    s.print(out);
    return out;
}

std::istream &operator>>(std::istream &in, String &s)
{
    s.read(in);
    return in;
}