#include <stdio.h>
#include <unity.h>

void setUp(void) {
}

void tearDown(void) {
}

int add(int a, int b) {
    return a + b;
}

void test_add_adds_two_values(void) {
    int expected = 1 + 3;
    printf("expected = %d\r\n", expected);
    printf("\r\n");
    int actual = add(1, 3);
    TEST_ASSERT_EQUAL_INT32(expected, actual);
}

// pio test -e native --verbose
int main(int agrc, char** argv) {
    UNITY_BEGIN();
    RUN_TEST(test_add_adds_two_values);
    UNITY_END();
    return 0;
}