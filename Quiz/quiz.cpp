#include <cstdio>
#include <cstdlib>

int sum( 
    int a00, int a01, int a02, int a03, int a04,
    int a05, int a06, int a07, int a08, int a09,
    int a10, int a11, int a12, int a13, int a14,
    int a15, int a16, int a17, int a18, int a19
)
{
    int a0001 = a00 + a01;
    int a0203 = a02 + a03;
    int a0405 = a04 + a05;
    int a0607 = a06 + a07;
    int a0809 = a08 + a09;
    int a1011 = a10 + a11;
    int a1213 = a12 + a13;
    int a1415 = a14 + a15;
    int a1617 = a16 + a17;
    int a1819 = a18 + a19;

    return 
    a0001 + a0203 + a0405 + a0607 + a0809 +
    a1011 + a1213 + a1415 + a1617 + a1819;
}

int main()
{
    auto s = sum(0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19);

    printf("%d\n", s);

    return EXIT_SUCCESS;
}