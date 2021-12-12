#include <cstdlib>
#include <cstring>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>

#define BUFSIZE 4096
#define THREADS 16

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

int llcs2(char *a, int m, char *b, int n) {
    // int *LCS = (int *)malloc(sizeof(int) * 2 * (n + 1));
    int LCS[2 * n + 2];
    int result = 0;
    for (int i = 0; i < n; i++) {
        LCS[i] = 0;
    }
    LCS[n + 1] = 0;
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
    return result;
}

void worker(int *res, char *a, char *b, int s1, int s2, int size) {
    int deez = 0;
    int max = 0;
    for (int j = 0; j < size; j++) {
        if (a[s1 + j] == b[s2 + j]) {
            deez++;
            if (deez > max) max = deez;
        }else{
            deez = 0;
        }
        // printf("%d   ", deez);
    }
    *res = std::max(max, *res);
}

int llcs_mt(char *a, int m, char *b, int n) {
    int max = 0;
    //std::cout<<"N = "<<n<<std::endl;
    int size = 0;
    for(int i = -n+1; i<m; i++){
        int s1 = std::max(0, i);
        int s2 = std::max(-i, 0);
        if(i<0) size++;
        if(i>n-1) size--;
        // printf("size = %d\n", size);
        std::thread t(worker, &max, a, b, s1, s2, size);
        t.detach();
    }

    return max;
}


// Single-threaded testbench for the same algorithm
int llcs_st(char *a, int m, char *b, int n){
    int max = 0;
    //std::cout<<"N = "<<n<<std::endl;
    int size = 1;
    for(int i = -n+1; i<m; i++){
        int s1 = std::max(0, i);
        int s2 = std::max(-i, 0);
        if(i<0) size++;
        if(i>n-1) size--;
        //std::cout<<"size"<<"____"<<size<<std::endl;
        worker(&max, a, b, s1, s2, size);
    }
    return max;
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
    auto start = std::chrono::system_clock::now();
    length = llcs_mt(buf1, length1, buf2, length2);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    printf("... length of lcs is [ %d ], and it took %f seconds. \n",
           length, elapsed_seconds);

    
    // free(buf1);
    // free(buf2);
    f1.close();
    f2.close();
    return 0;
}
