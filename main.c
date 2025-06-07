#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// #define iters 1000000

int main(int argc, char** argv)
{
    printf("\n");
    if (argc <= 1)
    {
        printf("usage: [program.exe] [int iterations] [int seed]\n");
        return 1;
    }
    if (argc <= 2)
    {
        printf("usage: [program.exe] [int iterations] [int seed]\n");
        return 1;
    }
    int iters = atoi(argv[1]);
    int seed0 = atoi(argv[2]);
    // printf("%d iterations                                                          \n", iters);
    // srand(time(NULL));
    srand(seed0);
    //branching 0, no omp, if branching
    int seed = rand() % 256;
    int arr[32768];
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    clock_t start = clock();
    long long total = 0;
    // #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        for (int j = 0; j < 32768; j++)
        {
            if (arr[j] >= 128) {total += arr[j];}
        }
    }
    clock_t end = clock();
    float elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    float totalElapsed = 0;
    totalElapsed += elapsed;
    printf("branching  0 sum : %lld, time: %08.3fs (no omp,      if branching)\n", total, elapsed);

    //branching 1, no omp, ternary branching
    seed = rand() % 256;
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    start = clock();
    total = 0;
    // #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        for (int j = 0; j < 32768; j++)
        {
            total = (arr[j] >= 128) ? total + arr[j] : total;
        }
    }
    end = clock();
    elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    totalElapsed += elapsed;
    printf("branching  1 sum : %lld, time: %08.3fs (no omp, ternary branching)\n", total, elapsed);
    
    //branching 1, omp, if branching
    seed = rand() % 256;
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    start = clock();
    total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        for (int j = 0; j < 32768; j++)
        {
            if (arr[j] >= 128) {total += arr[j];}
        }
    }
    end = clock();
    elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    totalElapsed += elapsed;
    printf("branching  2 sum : %lld, time: %08.3fs    (omp,      if branching)\n", total, elapsed);
    
    //branching 2, omp, ternary branching
    seed = rand() % 256;
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    start = clock();
    total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        for (int j = 0; j < 32768; j++)
        {
            total = (arr[j] >= 128) ? total + arr[j] : total;
        }
    }
    end = clock();
    elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    totalElapsed += elapsed;
    printf("branching  3 sum : %lld, time: %08.3fs    (omp, ternary branching)\n", total, elapsed);

    //branchless, no omp
    seed = rand() % 256;
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    start = clock();
    total = 0;
    // #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        for (int j = 0; j < 32768; j++)
        {
            int mask = (arr[j] - 128) >> 31;
            total += ~mask & arr[j];    
        }
    }
    end = clock();
    elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    totalElapsed += elapsed;
    printf("branchless 1 sum : %lld, time: %08.3fs (no omp,      no branching)\n", total, elapsed);

    //branchless omp
    seed = rand() % 256;
    for (int i = 0; i < 32768; i++)
    {
        arr[i] = seed + (rand() % 256);
    }
    start = clock();
    total = 0;
    #pragma omp parallel for reduction(+:total)
    for (int i = 0; i < iters; i++)
    {
        int localsum = 0;
        for (int j = 0; j < 32768; j++)
        {
            int mask = (arr[j] - 128) >> 31;;
            localsum += ~mask & arr[j];
        }
        total += localsum;
    }
    
    end = clock();
    elapsed = (float)(end - start) / CLOCKS_PER_SEC;
    totalElapsed += elapsed;
    printf("branchless 2 sum : %lld, time: %08.3fs    (omp,      no branching)\n", total, elapsed);
    printf("total time taken :");
    char c[_MAX_U64TOSTR_BASE10_COUNT]; _i64toa_s(total, c, _countof(c), 10); int a = strlen(c) + 9; for (int i = 0; i < a; i++){printf(" ");}
    printf("%08.3fs", totalElapsed); for (int i = 0; i < 28; i++){printf(" ");}
    printf("\n\n");
    return 0;
}
