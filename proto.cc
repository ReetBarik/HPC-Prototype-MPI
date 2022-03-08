#include <mpi.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <algorithm> 

std::pair<int, size_t>* merge (std::pair<int, size_t>* a1, std::pair<int, size_t>* a2, int size) {

	std::pair<int, size_t>* temp = (std::pair<int, size_t>*)malloc(2 * sizeof(std::pair<int, size_t>) * size);
	int i = 0, j = 0, h = 0;

	while(i < size && j < size) {

		if(a1[i].second < a2[j].second) {
			temp[h].first = a1[i].first;
			temp[h].second = a1[i].second;
			i++; h++;
		}
		else {
			temp[h].first = a2[j].first;
			temp[h].second = a2[j].second;
			j++; h++;
		}
	}

	while (i < size) {
		temp[h].first = a1[i].first;
		temp[h].second = a1[i].second;
		i++; h++;
	}

	while (j < size) {
		temp[h].first = a2[j].first;
		temp[h].second = a2[j].second;
		j++; h++;
	}

	return temp;

}

std::pair<int, size_t>* MyAllReduce(int rank, int size, std::pair<int, size_t> *localArray, int p) {

	int partner;
	std::pair<int, size_t>* result = (std::pair<int, size_t>*)malloc(sizeof(std::pair<int, size_t>) * size);
	std::pair<int, size_t>* temp;
	MPI_Status status;

	
	for (int t = 0; t < ceil(log2(p)); t++) {																																						// iterate through log2(p) steps
		partner = rank ^ (1 << t);																																									// XOR and bitshift
		MPI_Sendrecv(localArray, sizeof(std::pair<int, size_t>) * size, MPI_BYTE, partner, 0, result, sizeof(std::pair<int, size_t>) * size, MPI_BYTE, partner, 0, MPI_COMM_WORLD, &status);    	// exchange with partner process
		
		temp = merge(localArray, result, size);

		for (int i = size; i < 2 * size; i++) {
			localArray[i - size].first = temp[i].first;
			localArray[i - size].second = temp[i].second;
		}

	}

	for (int i = 0; i < size; i++) {
		result[i].first = localArray[i].first;
		result[i].second = localArray[i].second;
	}
	
	return result;		

}


int main(int argc, char *argv[]) {

	//mpicc -o pr proto.cc

	int size = 10, rank, p;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);


    using vcp = std::pair<int, size_t>;
	std::vector<vcp> v(size);
	std::pair<int, size_t>* res = (std::pair<int, size_t>*)malloc(sizeof(std::pair<int, size_t>) * size);

	for (int i = 0; i < size; i++) {
		v[i].first = i;
		v[i].second = (rank + 1) * i;
	}

	vcp* a = &v[0];

	res = MyAllReduce(rank, size, a, p);

	if (rank == 0){
	
		for (int i = 0; i < size; i++) {
			std::cout << res[i].first << " " << res[i].second << std::endl;
		}

	}

	MPI_Finalize();

}