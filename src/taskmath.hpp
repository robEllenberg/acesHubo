#include <math.h>
namespace ACES {

    /**
     * Class of math functions from C standard library, usable within a task context.
     * Since the addOperation method requires that functions be class members,
     * orocos script can't access the standard C math functions directly.
     * Until they make a math library part of the language, this hack is the
     * next best thing. Inherit from this class to add standard math functions,
     * as well as common methods to a given TaskContext (or child thereof).
     */
    template <class T>
    class TaskMath{
        public:
            //! Due to Operation restrictions, these functions can't be static!
            // Trigonometry
            T sin(T in){return std::sin(in);};
            T cos(T in){return std::cos(in);};
            T tan(T in){return std::tan(in);};
            T asin(T in){return std::asin(in);};
            T acos(T in){return std::acos(in);};
            T atan(T in){return std::atan(in);};
            T atan2(T y,T x){return std::atan2(y,x);};
            
            // Exponents and logs
            T sqrt(T in){return std::sqrt(in);};
            T exp(T in){return std::exp(in);};
            T log(T in){return std::log(in);};
            T log10(T in){return std::log10(in);};

            //Numerical functions
            T ceil(T in){return std::ceil(in);};
            T floor(T in){return std::floor(in);};
            T abs(T in){return std::abs(in);};
            T fmod(T in, T integrand){return std::fmod(in,integrand);};

            //Non-linear limiting functions
            T sat(T in,T min, T max);
            
    };

    template <class T>
    T TaskMath<T>::sat(T in,T min,T max){
        if (in>max) return max;
        if (in<min) return min;
        return in;
    }


}
