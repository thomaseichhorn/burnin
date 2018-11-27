#include <stdexcept>

struct BurnInException: public std::runtime_error {
    using runtime_error::runtime_error;
};
