
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







    mGeneration *gen = new mGeneration(inputfile);
    mLife *gen_bool = new mLife(inputfile);


    mUniverse life(gen_bool, iterations);
    life.Go();






    delete gen_bool;
    delete gen;
    return 0;
}