#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define h  800 
#define w  800
#define k  4 // Assume that k can fit in an unsigned char

#define input_file  "input.raw"
#define output_file "output.raw"


// Generates cluster assignments from means and data
void generate_assignments(unsigned char* data, double* means, unsigned char* assignments)
{
	int i, j;
	for(i = 0; i < h * w; ++i) { // For every point, assign it to the closest mean
		// Compute closest mean
		double min_distance = abs(means[0] - data[i]);
		unsigned char closest_cluster = 0;

		for(j = 1; j < k; ++j) {
			double curr_distance = abs(means[j] - data[i]);
			if(curr_distance < min_distance) {
				min_distance = curr_distance;
				closest_cluster = j;
			}
		}

		// Assign to closest mean
		assignments[i] = closest_cluster;

	}
}

// Performs one k means update, updating the means in place
// Given means_i, produces means_(i+1) and assignments_i
void k_means_update(unsigned char* data, double* means, unsigned char* assignments) 
{
	// Generate assignments
	generate_assignments(data, means, assignments);

	// Use assignments to compute new means
	int i, j;
	for(i = 0; i < k; ++i) { // For each mean
		unsigned int mean = 0;
		unsigned int total_elements = 0;
		for(j = 0; j < h * w; ++j) { // Look through all assignemnts to that means
			if(assignments[j] == i) { // Matching assignments contribute to new mean
				mean += data[j];
				++total_elements;
			}
		}

		means[i] = ((double)mean) / total_elements;
	}

}

int main(int argc, char** argv){
    int i;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not open file\n");
		return 1;
	}
	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);

	// Allocate array to store means and assignments
	double* means = (double*) malloc (sizeof(double) * k);
	unsigned char* assignments = (unsigned char*) malloc (sizeof(unsigned char*) * h * w);

	// Initialize means according to problem 
	means[0] = 0;
	means[1] = 85;
	means[2] = 170;
	means[3] = 255;
    
	// measure the start time here
	//  Your code goes here
	struct timespec start, stop;
	double elapsed_time;

	if(clock_gettime(CLOCK_REALTIME, &start) == -1) { 
		printf("Error getting start time\n");
		return 1;
	}

	// Run 30 iterations
	for(i = 0; i < 30; ++i) {
		k_means_update(a, means, assignments);
	}

	generate_assignments(a, means, assignments);
		
	//
	
	// measure the end time here
	if(clock_gettime(CLOCK_REALTIME, &stop) == -1) { 
		printf("Error getting stop time\n");
		return 1;
	}
	
	// print out the execution time here
	elapsed_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	printf("Execution time: %f\n", elapsed_time);

	printf("Means:\n");
	for(i = 0; i < 4; ++i) {
		printf("%f\n", means[i]);
	}
	
	// Update a to reflect the final clustering
	for(i = 0; i < h*w; ++i) {
		a[i] = means[assignments[i]];
	}
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not open file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);
    
    return 0;
}