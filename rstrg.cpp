#include <fstream>
#include <ctime>
#include <iostream>
#include <unistd.h>

std::string gen_random(const int len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    
    return tmp_s;
}

int main(int argc, char *argv[]) {
    if(argc<2){
        std::cout<<"Usage: rand_gen <sizeofstring>"<<std::endl;
        exit(1);
    }

    srand((unsigned)time(NULL) * getpid());     
    std::string rand1 = gen_random(atoi(argv[1]));
    srand((unsigned)time(NULL));
    std::string rand2 = gen_random(atoi(argv[1]));
    
    std::ofstream out1("filea.txt");
    out1<<rand1<<std::endl;
    out1.close();

    std::ofstream out2("fileb.txt");
    out2<<rand2<<std::endl;
    out2.close();
    
    return 0;
}
