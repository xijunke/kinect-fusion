#if defined(__APPLE__) || defined(__MACOSX)
    #include <OpenCL/cl.hpp>
#else
    #include <CL/cl.hpp>
#endif

#include <iostream>
#include <Eigen>

int main(int argc, char * argv[])
{
    std::cout << "Hello, World!" << std::endl;

    Eigen::Matrix2i a;
    a << 1, 2, 3, 4;
    std::cout << a << std::endl;

    return 0;
}