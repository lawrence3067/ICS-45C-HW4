#include <gtest/gtest.h>
#include <string.h>

#include <algorithm>

#include "string.hpp"
#include "alloc.hpp"

TEST(StringFunction, strlen) {
    EXPECT_EQ(String::strlen(""), 0);
    EXPECT_EQ(String::strlen("foo"), 3);
}

TEST(StringFunction, strcpy) {
    char result[10];
    EXPECT_EQ(String::strcpy(result, "foo"), result);
    EXPECT_STREQ(result, "foo");

    EXPECT_EQ(String::strcpy(result, "a"), result);
    EXPECT_STREQ(result, "a");

    EXPECT_EQ(String::strcpy(result, ""), result);
    EXPECT_STREQ(result, "");
}

TEST(StringFunction, strdup) {
    EXPECT_STREQ(String::strdup("jello"), "jello");
    EXPECT_STREQ(String::strdup(""), "");
}

TEST(StringFunction, strncpy) {
    char result[10];

    EXPECT_EQ(String::strncpy(result, "bruh", 3), result);
    EXPECT_STREQ(result, "bru");

    EXPECT_EQ(String::strcpy(result, ""), result);
    EXPECT_STREQ(result, "");
}

TEST(StringFunction, strcat) {
    char result[30]{"yes"};

    char first[10]{"Hello"};
    char second[10]{" World!"};

    EXPECT_STREQ(String::strcat(result, first), "yesHello");
    EXPECT_STREQ(String::strcat(result, second), "yesHello World!");
    EXPECT_STREQ(first, "Hello");
    EXPECT_STREQ(second, " World!");
}

TEST(StringFunction, strncat) {
    char result[30]{"yes"};
    char first[10]{"Hello"};
    char second[10]{" World!"};

    EXPECT_STREQ(String::strncat(result, first, 3), "yesHel");
    EXPECT_STREQ(String::strncat(result, second, 4), "yesHel Wor");
    EXPECT_STREQ(String::strncat(result, second, 0), "yesHel Wor");
    EXPECT_STREQ(first, "Hello");
    EXPECT_STREQ(second, " World!");
}

TEST(StringFunction, strcmp) {
    char first[10]{"Hello"};
    char second[10]{"Hello"};
    char third[10]{"hello"};

    EXPECT_EQ(String::strcmp(first, second), 0);
    EXPECT_EQ(String::strcmp(first, third), -32);
    EXPECT_EQ(String::strcmp(third, first), 32);
}

TEST(StringFunction, strncmp) {
    char first[10]{"foo"};
    char second[10]{"foobar"};
    char third[10]{""};
    char fourth[10]{""};

    EXPECT_EQ(String::strncmp(first, second, 3), 0);
    EXPECT_TRUE(String::strncmp(first, second, 4) < 0);
    EXPECT_TRUE(String::strncmp(second, first, 4) > 0);
    EXPECT_EQ(String::strncmp(third, fourth, 5), 0);
    EXPECT_TRUE(String::strncmp(third, first, 2) < 0);
    EXPECT_TRUE(String::strncmp(first, third, 2) > 0);
    EXPECT_TRUE(String::strncmp(first, second, 10) < 0);
    EXPECT_TRUE(String::strncmp(second, first, 10) > 0);
    EXPECT_EQ(String::strncmp(first, first, 3), 0);
}

TEST(StringFunction, reverse_cpy) {
    char first[10]{"Hello"};
    char result[10];

    String::reverse_cpy(result, first);
    EXPECT_STREQ(result, "olleH");
}

TEST(StringFunction, strchr) {
    char first[10]{"Hello"};

    EXPECT_STREQ(String::strchr(first, 'e'), "ello");
    EXPECT_STREQ(String::strchr(first, 'l'), "llo");
}

TEST(StringFunction, strstr) {
    char first[30]{"HelHellHellolo"};
    char second[30]{"ababababab"};
    char third[10]{""};
    char tester[10]{"c"};
    
    EXPECT_STREQ(String::strstr(first, "Hello"), "Hellolo");
    EXPECT_STREQ(String::strstr(first, "lH"), "lHellHellolo");
    EXPECT_STREQ(String::strstr(second, "abababab"), "ababababab");
    EXPECT_EQ(String::strstr(first, tester), nullptr);
    EXPECT_EQ(String::strstr(first, ""), first);
    EXPECT_EQ(String::strstr(third, tester), nullptr);
    EXPECT_EQ(String::strstr(third, ""), third);
}
