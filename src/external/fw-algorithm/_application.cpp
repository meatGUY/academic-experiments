#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "algorithm.hpp"

#include "operations.hpp"

#include "graphs-cio.hpp"
#include "graphs-fio.hpp"
#include "graphs-generators.hpp"

#include "square-shape-graphs.hpp"
#include "square-shape-io.hpp"
#include "square-shape.hpp"

using namespace utilz;

// Constant value which indicates that there is no path between two vertexes.
// Please note: this value can be used ONLY when input paths are >= 0.
//
constexpr int
no_path_value()
{
  return ((std::numeric_limits<int>::max)() / 2) - 1;
};

// A precondition for input matrix to ensure proper algorithm execution.
// Ensures: matrix is a square matrix; all cell values are greater or equal to zero.
//
// template<typename T, T V>
// class rect_shape_precondition
// {
// private:
//   const T m_s = V;

// public:
//   rect_shape_precondition()
//   {}

//   bool operator()(const rect_shape<T>& s)
//   {
//     for (size_t i = 0; i < s.w(); ++i)
//       for (size_t j = 0; j < s.h(); ++j)
//         if (s(i, j) < this->m_s)
//           return false;

//     return s.w() == s.h();
//   }
// };

// using matrix_precondition     = rect_shape_precondition<long, 0>;
// using matrix_memory           = rect_shape_matrix_memory<long, no_path_value()>;
// using matrix_output           = rect_shape_matrix_output<long, matrix_memory>;
// using matrix_output_predicate = matrix_all_predicate<long>;
// using matrix_input            = rect_shape_matrix_input<long>;
// using matrix_input_predicate  = matrix_except_predicate<long, no_path_value()>;

template<typename T, T V>
class except_predicate
{
private:
  const T m_v = V;

public:
  except_predicate()
  {}

  bool operator()(const T& v) const
  {
    return v != this->m_v;
  }
};

int
main(int argc, char* argv[])
{
  std::mt19937_64               distribution_engine;
  std::uniform_int_distribution distribution(1, 10);

  operations::random_value          rv(distribution_engine, distribution);
  procedures::square_shape_at       at(rv);
  procedures::square_shape_drill_at drill_at(at, 5);

  square_shape<int> s(10);

  square_shape<square_shape<int>> bs(10, 5);
  for (auto i = 0; i < 10; ++i)
    for (auto j = 0; j < 10; ++j)
      bs.at(i, j) = square_shape<int>(bs.ssize());

  bs.at(0, 1).at(0, 2) = 10;

  procedures::square_shape_size<square_shape<int>> a;
  procedures::square_shape_size<square_shape<square_shape<int>>> b;


  size_t r1 = a(s);
  size_t r2 = b(bs);


  bool b1 = traits::is_square_shape<int>::value;
  bool b2 = traits::is_square_shape<square_shape<int>>::value;
  bool b3 = traits::is_square_shape<square_shape<square_shape<int>>>::value;
  bool b4 = traits::is_square_shape<square_shape<int>::value_type>::value;


  graphs::random_dag(10, 15, s, at);

  for (size_t i = 0; i < s.size(); ++i) {
    for (size_t j = 0; j < s.size(); ++j) {
      std::cout << "(" << i << "," << j << ") = " << s.at(i, j) << std::endl;
    }
  }
}
