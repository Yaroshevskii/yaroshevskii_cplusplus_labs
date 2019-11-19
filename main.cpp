
#include "mLife.h"
#include "mUniverse.h"


using namespace std;



int main(int argc, char* argv[])
{

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << "SOURCE DESTINATION" << std::endl;
        return 1;
    }

    string inputfile = argv[1];
    int iterations = atoi(argv[2]);
    //cout << " filename = " << inputfile << " iteration = " << iterations << endl;



    mGeneration *gen = new mGeneration(inputfile);
    mUniverse life(gen, iterations);
    life.Go();


    delete gen;

    return 0;
}