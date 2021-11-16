#include "t.hpp"

template void f<int>(); //definition
template void f<float>(); //definition. Derleyici t.cpp yi derleyince bu kez explicit instantiation definitionla bu specializationlar için instantiation yapacak.