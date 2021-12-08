#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>

#define BUFSIZE 4096

int llcs(char *a, int m, char *b, int n) {
    int *LCS = (int *)malloc(sizeof(int) * 2 * (n + 1));
    int result = 0;
    for (int i = 0; i < m; i++) {
        // printf("%ld\n", clock());
        int k = i % 2;
        for (int j = 0; j < n; j++) {
            if (i == 0 || j == 0)
                LCS[k * (n + 1) + j] = 0;
            else if (a[i - 1] == b[j - 1]) {
                LCS[k * (n + 1) + j] = LCS[(1 - k) * (n + 1) + j - 1] + 1;
                if (LCS[k * (n + 1) + j] > result)
                    result = LCS[k * (n + 1) + j];
            } else {
                LCS[k * (n + 1) + j] = 0;
            }
        }
    }

    free(LCS);
    return result;
}

// using two arrays for maximize space efficiency
// and improve prefetching
/*
int llcs2(char *a, int m, char *b, int n) {
    int *LCS0 = (int *)malloc(sizeof(int) * (n + 1));
    int *LCS1 = (int *)malloc(sizeof(int) * (n + 1));

    int result = 0;
    // move DP table initialization outside of main loop
    // to decrease branch prediction miss rate
    for (int i = 0; i < n; i++) {
        LCS0[i] = 0;
    }
    LCS1[0] = 0;
    for (int i = 1; i < m; i++) {
        // printf("%ld\n", clock());
        int k = i % 2;
        for (int j = 1; j < n; j++) {
            if (k == 0) {
                if (a[i - 1] == b[j - 1]) {
                    LCS0[j] = LCS1[j - 1] + 1;
                    if (LCS0[j] > result) result = LCS0[j];
                } else {
                    LCS0[j] = 0;
                }
            } else {
                if (a[i - 1] == b[j - 1]) {
                    LCS1[j] = LCS0[j - 1] + 1;
                    if (LCS1[j] > result) result = LCS1[j];
                } else {
                    LCS1[j] = 0;
                }
            }
        }
    }

    free(LCS0);
    free(LCS1);
    return result;
}
*/

int llcs3(char *a, int m, char *b, int n) {
    // int *LCS = (int *)malloc(sizeof(int) * 2 * (n + 1));
    int LCS[2*n+2];
    int result = 0;
    for(int i=0; i<n;i++){
        LCS[i]=0;
    }
    LCS[n+1]=0;
    for (int i = 1; i < m; i++) {
        // printf("%ld\n", clock());
        int k = i % 2;
        for (int j = 1; j < n; j++) {
            
            if (a[i - 1] == b[j - 1]) {
                LCS[k * (n + 1) + j] = LCS[(1 - k) * (n + 1) + j - 1] + 1;
                if (LCS[k * (n + 1) + j] > result)
                    result = LCS[k * (n + 1) + j];
            } else {
                LCS[k * (n + 1) + j] = 0;
            }
        }
    }

    // free(LCS);
    return result;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("Usage: lcss <str_file_1> <str_file_2>\n");
        exit(1);
    }

    std::string file1 = argv[1];
    std::string file2 = argv[2];

    std::ifstream f1(file1, std::ios::in | std::ios::binary);
    if (!f1.good()) {
        std::cout << "file 1 read error" << std::endl;
        f1.close();
        exit(1);
    }

    std::ifstream f2(file2, std::ios::in | std::ios::binary);
    if (!f2.good()) {
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

    // printf("%s\n",buf1);
    // printf("%s\n",buf2);

    printf("Starting actual longestcommonstring...\n\n");
    int length;
    std::clock_t start, end;
    // time keeping
    start = std::clock();
    length = llcs3(buf1, length1, buf2, length2);
    end = std::clock();

    printf("... length of lcs is [ %d ], and it took %ld clock cycles. \n",
           length, end - start);

    free(buf1);
    free(buf2);
    f1.close();
    f2.close();
    return 0;
}
