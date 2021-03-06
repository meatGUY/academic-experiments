#pragma once

#include <algorithm>
#include <chrono>
#include <random>
#include <stdexcept>
#include <vector>

namespace utilz {
namespace graphs {
namespace generators {

struct directed_acyclic_graph_tag
{};

template<
  typename Matrix,
  typename MatrixSetSizeOperation,
  typename MatrixSetValueOperation>
void
random_graph(
  typename MatrixSetSizeOperation::result_type v,
  typename MatrixSetSizeOperation::result_type e,
  Matrix&                                      m,
  MatrixSetSizeOperation&                      set_size,
  MatrixSetValueOperation&                     set_value,
  directed_acyclic_graph_tag)
{
  using size_type  = typename MatrixSetSizeOperation::result_type;
  using value_type = typename MatrixSetValueOperation::result_type;

  static_assert(std::is_unsigned<size_type>::value, "erro: matrix `set_size` operation has to use unsigned integral type");

  if (e >= (v * (v - 1) / 2))
    throw std::logic_error(
      "erro: edge count in direct acyclic graph can't exceed: `((v) * (v - 1) / 2)`, where `v` is a vertex count.");

  std::mt19937_64                          distribution_engine;
  std::uniform_int_distribution<size_type> distribution(size_type(0), v - size_type(1));

  distribution_engine.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());

  // Initialize vector of vertexes with vertex indexes
  // i.e. `vector[0]` represents vertex `0`, `vector[1]` vertex `1` and so on and so forth
  //
  std::vector<size_type> vertexes(v);
  std::generate(vertexes.begin(), vertexes.end(), [n = size_type(0)]() mutable { return n++; });

  // Initialize vector of edges between vertexes
  // i.e. vector contains true in `[i * v + j]` if there is an edge between
  // `i` and `j`
  //
  std::vector<bool> edges(v * v);
  std::fill(edges.begin(), edges.end(), false);

  // Permutate vector of vertexes
  //
  for (size_type i = 0; i < (v - size_type(1)); ++i)
    std::swap(vertexes[distribution(distribution_engine)], vertexes[i]);

  // Set output matrix size and start writing edges
  //
  set_size(m, size_type(v));

  // Pick two random vertexts indexes and create an edge between them.
  // Repeat until required number of edges.
  //
  for (size_type i = size_type(0), j = size_type(0), c = size_type(0), a = size_type(0); c < e;) {
    // Don't create self-cycles
    //
    if ((i = distribution(distribution_engine)) == (j = distribution(distribution_engine)))
      continue;

    i = vertexes[i];
    j = vertexes[j];

    // We should create edges directed from "lowel" vertexes to "higher" only
    // to ensure no cycles are created.
    //
    if (i > j)
      std::swap(i, j);

    if (!edges[i * v + j]) {
      // If output has no i -> j edge
      //
      set_value(m, i, j, value_type(1));
    } else if (a != e) {
      ++a;
      continue;
    } else {
      // If output has i -> j edge and we have tried to much to create
      // an edge (i.e. number of failed attempts is equal to required number of edges)
      // we simply perform a direct search to insert an edge
      //
      bool found = false;
      for (size_type _i = 0; _i < v && !found; ++_i)
        for (size_type _j = 0; _j < v && !found; ++_j) {
          i = vertexes[_i];
          j = vertexes[_j];

          if (i > j)
            std::swap(i, j);

          if (!edges[i * v + j]) {
            set_value(m, i, j, value_type(1));

            found = true;
          }
        }

      a = size_type(0);
    }

    // Indicate that we have created an edge between `i` -> `j`
    //
    ++c;
    edges[i * v + j] = true;
  };
};

} // namespace generators
} // namespace graphs
} // namespace utilz
