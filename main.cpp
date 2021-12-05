#include <cstdlib>
#include <unistd.h>
#include <fstream>
#include <iostream>

#define BUFSIZE 4096

inline uint64_t rdtsc()
{
    unsigned long a, d;
    asm volatile("rdtsc"
                 : "=a"(a), "=d"(d));
    return a | ((uint64_t)d << 32);
}

int llcs(char *a, int m, char *b, int n)
{
    int *LCS = (int *)malloc(sizeof(int) * 2 * (n + 1));
    int result = 0;
    for (int i = 0; i < m; i++)
    {
        int k = i % 2;
        for (int j = 0; j < n; j++)
        {
            if (i == 0 || j == 0)
                LCS[k * (n + 1) + j] = 0;
            else if (a[i - 1] == b[j - 1])
            {
                LCS[k * (n + 1) + j] = LCS[(1 - k) * (n + 1) + j - 1] + 1;
                if (LCS[k * (n + 1) + j] > result)
                    result = LCS[k * (n + 1) + j];
            }
            else
            {
                LCS[k * (n + 1) + j] = 0;
            }
        }
    }

    free(LCS);
    return result;
}

//using two arrays
int llcs2(char *a, int m, char *b, int n)
{
    int *LCS0 = (int *)malloc(sizeof(int) * (n + 1));
    int *LCS1 = (int *)malloc(sizeof(int) * (n + 1));
    int result = 0;
    for (int i = 0; i < m; i++)
    {
        int k = i % 2;
        for (int j = 0; j < n; j++)
        {
            if (k == 0)
            {
                if (i == 0 || j == 0)
                {
                    LCS0[j] = 0;
                }
                else if (a[i - 1] == b[j - 1])
                {
                    LCS0[j] = LCS1[j - 1] + 1;
                    if (LCS0[j] > result)
                        result = LCS0[j];
                }
                else
                {
                    LCS0[j] = 0;
                }
            }
            else
            {
                if (i == 0 || j == 0)
                {
                    LCS1[j] = 0;
                }
                else if (a[i - 1] == b[j - 1])
                {
                    LCS1[j] = LCS0[j - 1] + 1;
                    if (LCS1[j] > result)
                        result = LCS1[j];
                }
                else
                {
                    LCS1[j] = 0;
                }
            }
        }
    }

    free(LCS0);
    free(LCS1);
    return result;
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Usage: lcss <str_file_1> <str_file_2>\n");
        exit(1);
    }

    std::string file1 = argv[1];
    std::string file2 = argv[2];

    std::ifstream f1(file1, std::ios::in | std::ios::binary);
    if (!f1.good())
    {
        std::cout << "file 1 read error" << std::endl;
        f1.close();
        exit(1);
    }

    std::ifstream f2(file2, std::ios::in | std::ios::binary);
    if (!f2.good())
    {
        std::cout << "file 2 read error" << std::endl;
        f2.close();
        exit(1);
    }

    f1.seekg(0, f1.end);
    int length1 = f1.tellg();
    f1.seekg(0, f1.beg);

    f2.seekg(0, f2.end);
    int length2 = f2.tellg();
    f2.seekg(0, f2.beg);

    char *buf1 = (char *)malloc(length1);
    f1.read(buf1, length1);

    char *buf2 = (char *)malloc(length2);
    f2.read(buf2, length2);

    //printf("%s\n",buf1);
    //printf("%s\n",buf2);

    printf("Starting actual longestcommonstring...\n\n");
    int length;
    long start, end;
    //time keeping
    start = rdtsc();
    length = llcs2(buf1, length1, buf2, length2);
    end = rdtsc();

    printf("... length of lcs is [ %d ], and it took %ld clock cycles. \n", length, end - start);

    free(buf1);
    free(buf2);
    f1.close();
    f2.close();
    return 0;
}
