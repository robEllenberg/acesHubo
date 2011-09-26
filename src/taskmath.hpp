#include <math.h>
//#include <rtt/TaskContext.hpp>

namespace ACES {

    /**
     * Class of math functions from C standard library, usable within a task context.
     * Since the addOperation method requires that functions be class members,
     * orocos script can't access the standard C math functions directly.
     * Until they make a math library part of the language, this hack is the
     * next best thing. Inherit from this class to add standard math functions,
     * as well as common methods to a given TaskContext (or child thereof).
     * @todo Would this work better as a separate component with its own interface? if all operations were executed in the client thread, then multiple threads could be calling the same operations. 
     */
    template <class T>
    class TaskMath{
        public:
            //! Due to Operation restrictions, these functions can't be static!
            //Trigonometry
            T sin(T in){return std::sin(in);};
            T cos(T in){return std::cos(in);};
            T tan(T in){return std::tan(in);};
            T asin(T in){return std::asin(in);};
            T acos(T in){return std::acos(in);};
            T atan(T in){return std::atan(in);};
            T atan2(T y,T x){return std::atan2(y,x);};
            
            //Hyperbolic functions
            T sinh(T in){return std::sinh(in);};
            T cosh(T in){return std::cosh(in);};
            T tanh(T in){return std::tanh(in);};

            //Exponents and logs
            T sqrt(T in){return std::sqrt(in);};
            T exp(T in){return std::exp(in);};
            T pow(T base, T exponent){return std::pow(base,exponent);};
            T log(T in){return std::log(in);};
            T log10(T in){return std::log10(in);};

            //Numerical functions
            T ceil(T in){return std::ceil(in);};
            T floor(T in){return std::floor(in);};
            T abs(T in){return std::abs(in);};
            T fmod(T in, T integrand){return std::fmod(in,integrand);};

            //Non-linear limiting functions
            T sat(T in, T min, T max);
            
    };

    /**
     * Simple saturation function.
     * NOTE: Only valid if max > min, but this fails gracefully.
     * If min > max, then this function will always return a limit value.
     * It's up to YOU to make sure that the space between the limit values
     * makes sense.
     */
    template <class T>
    T TaskMath<T>::sat(T in,T min,T max){
        if (in>max) return max;
        if (in<min) return min;
        return in;
    }
}
