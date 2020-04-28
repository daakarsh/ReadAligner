#include <seqan3/core/debug_stream.hpp>
#include "hello_world2.h"

int main() {
  seqan3::debug_stream << "Hellow world\n";
  test();
  return 0;
}
