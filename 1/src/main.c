#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

double f(double x) {
    return sqrt(4 - x*x);
}

int main(int argc, char* argv[]) {
    int numtasks, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start = MPI_Wtime();

    double a = 0, b = 2;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <number-of-intervals>\n", argv[0]);
        return 1;
    }
    int numintervals = atoi(argv[1]);

    double h = (b - a) / numintervals;

    double local_result = 0;
    for (int i = rank; i < numintervals; i += numtasks) {
        double x = a + h * (i + 1. / 2);
        local_result += h * f(x);
    }

    if (rank == 0) {
        double total_result = local_result;
        for (int i = 1; i < numtasks; ++i) {
            double remote_result;
            MPI_Recv(&remote_result, 1, MPI_DOUBLE, i, MPI_ANY_TAG,
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_result += remote_result;
        }
        printf("%f\n", total_result);

        double end = MPI_Wtime();
        fprintf(stderr, "%f\n", end - start);
    }
    else {
        MPI_Send(&local_result, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    /* printf("It works with %d tasks while I am number %d\n", numtasks, rank);
     * char buf[128] = "";
     * if (rank == 0) {
     *     strcpy(buf, "Message from 0");
     *     MPI_Send(buf, strlen(buf) + 1, MPI_CHAR, 1, 102, MPI_COMM_WORLD);
     * }
     * else if (rank == 1) {
     *     MPI_Status status;
     *     MPI_Recv(buf, sizeof(buf) / sizeof(buf[0]), MPI_CHAR, MPI_ANY_SOURCE,
     *             MPI_ANY_TAG, MPI_COMM_WORLD, &status);
     *     int count;
     *     MPI_Get_count(&status, MPI_CHAR, &count);
     *     printf("Received '%s' from %d with tag %d and size %d\n", buf,
     *             status.MPI_SOURCE, status.MPI_TAG, count);
     * }
     */
    MPI_Finalize();
    return 0;
}
