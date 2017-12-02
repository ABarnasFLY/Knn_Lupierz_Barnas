#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//#define TEST_SET_LENGTH 1000
#define SET_LENGTH 500
#define VECTOR_LENGTH 28*28

typedef struct{
    unsigned char vector[VECTOR_LENGTH];
    int label;
} element;
typedef struct{
    int label;
    double distance;
}neighbour;


int neighbour_comp(const void * elem1, const void * elem2)
{
    neighbour *a = (neighbour*) elem1;
    neighbour *b = (neighbour*) elem2;
    if(a->distance > b->distance) return 1;
    else if(a->distance < b->distance) return -1;
    else return 0;
}

void read_mnist(element images[], int number, const char *label, const char *image)
{
    FILE *file_labels = fopen(label, "rb");
    FILE *file_images = fopen(image, "rb");
    if (!file_labels || !file_images)
    {
        printf("ERROR");
    }
    else
    {
        printf("FILE READ SUCCESSFULLY\n");
        fseek(file_images, 46 + 2 * 28, SEEK_SET);
        fseek(file_labels, 8, SEEK_SET);

        for (int i=0; i < number; i ++ )
        {
            unsigned char bufor=0;
            fread(&bufor, 1, 1, file_labels);
            images[i].label = bufor;
            fread(&images[i].vector, 1, VECTOR_LENGTH, file_images);
        }

        fclose(file_images);
        fclose(file_labels);
    }
}

//function returns size of testing database, size of training database = numbeer - sizeof testing database
int split(element training[], element testing[], element images[], int number, int ratio)
{
    int tr = 0;
    int te = 0;
    srand(time(0));
    for (int i = 0; i < number; i++)
    {
        if (rand() % 100 < ratio)
        {
            training[tr] = images[i];
            tr++;

        }
        else
        {
            testing[te] = images[i];
            te++;
        }
    }
    return te;
}


float euklideanDistance (element instance1, element instance2)
{
    double distance = 0;
    for (int x = 0; x < VECTOR_LENGTH; x++)
    {
        distance += ((float)instance1.vector[x] - (float)instance2.vector[x]) * ((float)instance1.vector[x] - (float)instance2.vector[x]);
    }
    return sqrt(distance);
}

void getNeighbours(int k, element instance, element trainTab[], int train_size, neighbour kneighbours[], neighbour neighbours[])
{
    for(int i = 0; i < train_size; i++)
    {
        double distance = euklideanDistance(trainTab[i], instance);
        neighbours[i].distance = distance;
        neighbours[i].label = trainTab[i].label;
    }
    qsort(neighbours,train_size, sizeof(neighbour),neighbour_comp);
    for(int j = 0; j < k; j++)
    {
        kneighbours[j] = neighbours[j];
    }
}
//k < 100
int Knn_solver(element instance, int k, element trainTab[], int train_size, neighbour neighbours[])
{
    /*Do something*/
    neighbour kneighbours[100];
    int translate[10];
    for(int i = 0; i < 10; i++) translate[i] = 0;
    getNeighbours(k, instance, trainTab,train_size, kneighbours, neighbours);
    for(int i = 0; i < k; i++)
    {
        if(kneighbours[i].label < 10)
            translate[kneighbours[i].label]++;
    }
    int max = 0;
    for(int i = 1; i < 10; i++)
    {
        if(translate[i] > translate[max]) max = i;
    }
    return max;
}

int main()
{
//#define DEBUGING
#ifdef DEBUGING
    static element images[SET_LENGTH];
    static element training[SET_LENGTH];
    static element testing[SET_LENGTH];
    static neighbour neighbours[SET_LENGTH];

    read_mnist(images, SET_LENGTH, "test_labels.byte", "test_images.byte");
    int test = split(training,testing, images,SET_LENGTH,66);
    int train = SET_LENGTH - test;
#else
    static element training[59999];
    static element testing[9999];
    static neighbour neighbours[59999];

    read_mnist(testing, 9999, "../Data/test_labels.byte", "../Data/test_images.byte");
    read_mnist(training, 59999, "../Data/train_labels.byte", "../Data/train_images.byte");
    int test = 9999;
    int train = 59999;
#endif
    float errors = 0.0;
    int prc = 0;
    for(int i = 0; i < test; i++)
    {
        if(Knn_solver(testing[i], 5, training,train, neighbours) != testing[i].label) errors += 1.0;
        if(i%100 == 0) printf("%d\%\n", ++prc);
    }
    printf("\n---------\nSuccsess in : %f\n", (test - errors) / test);
    system("PAUSE");
    return 0;
}
