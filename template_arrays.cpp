#include <iostream>
#include <queue>
#include <assert.h>

#define ID(...) { std::queue<size_t>({__VA_ARGS__}) }

template <class T, size_t arg1, size_t... argn>
struct MultiMatrix{
	using nested_type = typename MultiMatrix<T, argn...>::type;
	using type = nested_type[arg1];
};

template <class T, size_t arg>
struct MultiMatrix < T, arg > {
	using type = T[arg];
};

template <class T, size_t arg1, size_t... argx>
class Matrix{
	private:
		typename MultiMatrix<T, arg1, argx...>::type data;
		friend T& access_MultiMatrix(typename MultiMatrix< T, arg1, argx... >::type &matrix, std::queue<size_t> &indexing);
		friend T& access_MultiMatrix(typename MultiMatrix< T, arg1 >::type &matrix, std::queue<size_t> &indexing);
	public:
		T& operator()(std::queue<size_t> &indexing){
			return access_MultiMatrix<T, arg1, argx...>(data, indexing);
		}
};

template <class T, size_t arg1, size_t arg2, size_t... args>
T& access_MultiMatrix( typename MultiMatrix< T, arg1, arg2, args... >::type &matrix, std::queue<size_t> &indexing ){
	size_t i{ indexing.front() };
	assert(("index out of range",i < arg1));
	indexing.pop();
	return access_MultiMatrix<T, arg2, args...>(matrix[i], indexing);
}

template <class T, size_t arg1>
T& access_MultiMatrix(typename MultiMatrix< T, arg1 >::type &matrix, std::queue<size_t> &indexing){
	size_t i{ indexing.front() };
	assert(("index out of range",i < arg1));
	indexing.pop();
	return matrix[i];
}

int main(){
	Matrix< int, 2, 2 > m2x2;
	Matrix< int, 1, 1, 2 > m1x1x2;

	m2x2(ID(0, 1)) = -3;

	std::cerr << m2x2(ID(0, 1)) << std::endl;

	m1x1x2(ID(0, 0, 1)) = 3;

	std::cerr << m1x1x2(ID(0, 0, 1)) << std::endl;

	return 0;
}

