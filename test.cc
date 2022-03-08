#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm> 


std::pair<int, size_t>* MyAllReduce(std::pair<int, size_t>* a1, std::pair<int, size_t>* a2, int size) {


	std::pair<int, size_t>* temp = (std::pair<int, size_t>*)malloc(2 * sizeof(std::pair<int, size_t>) * size);
	std::pair<int, size_t>* res = (std::pair<int, size_t>*)malloc(sizeof(std::pair<int, size_t>) * size);
	int i = 0, j = 0, h = 0;

	while(i < size && j < size) {

		if(a1[i].second < a2[j].second) {
			temp[h].first = a1[i].first;
			temp[h].second = a1[i].second;
			// std::cout << a1[i].first << " " << a1[i].second << std::endl;
			i++; h++;

			
		}
		else {
			temp[h].first = a2[j].first;
			temp[h].second = a2[j].second;
			// std::cout << a2[j].first << " " << a2[j].second << std::endl;
			j++; h++;


		}
	}

	while (i < size) {
		temp[h].first = a1[i].first;
		temp[h].second = a1[i].second;
		// std::cout << a1[i].first << " " << a1[i].second << std::endl;
		i++; h++;
	}

	while (j < size) {
		temp[h].first = a2[j].first;
		temp[h].second = a2[j].second;
		// std::cout << a2[j].first << " " << a2[j].second << std::endl;
		j++; h++;
	}

	for (int i = size; i < 2 * size; i++) {
		res[i - size].first = temp[i].first;
		res[i - size].second = temp[i].second;
	}

	return res;

}


int main(int argc, char *argv[]) {


	int size = 10, rank, p;
    

    using vcp = std::pair<int, size_t>;
	std::vector<vcp> v1(size);
	std::vector<vcp> v2(size);
	std::pair<int, size_t>* res = (std::pair<int, size_t>*)malloc(sizeof(std::pair<int, size_t>) * size);

	for (int i = 0; i < size; i++) {
		v1[i].first = i;
		v1[i].second = 2 * i;

		v2[i].first = i;
		v2[i].second = 4 * i;
	}

	vcp* a1 = &v1[0];
	vcp* a2 = &v2[0];

	res = MyAllReduce(a1, a2, size);

	
	for (int i = 0; i < size; i++) {
		std::cout << res[i].first << " " << res[i].second << std::endl;
	}

	

	return 0;

}