#include <math.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

//#define TEST_SET_LENGTH 1000
#define SET_LENGTH 5000
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
        printf("SUCCESS");
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

void getNeighbours(int k, element instance, element trainTab[], int train_size, neighbour kneighbours[])
{
    neighbour neighbours[SET_LENGTH];
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

int main()
{
    static element images[5000];
    static element training[5000];
    static element testing[5000];
    read_mnist(images, 5000, "test_labels.byte", "test_images.byte");
    int test = split(training,testing, images,5000,66);
    float errors = 0.0;
    for(int i = 0; i < test; i++)
    {
        neighbour n;
        getNeighbours(1, testing[i], training, 5000-test, &n);
        if(n.label != testing[i].label) errors+= 1.0;
        if(!(i%100)) printf(".");
    }
    printf("Succsess in : %f\n", (test - errors) / test);
    /*
    for (int i = 0; i < 20; i++)
    {
        printf("%d\n", images[i].label);
    }
    for (int j = 0; j < 20; j++)
    {
        for (int k = 0; k < 28; k++)
        {
            for (int l = 0; l < 28; l++)
            {
                if (images[j].vector[k * 28 + l] > 50)
                    printf("@");
                else
                    printf("~");
            }
            printf("\n");
        }
        printf("%d\n", images[j].label);
    }

    printf("Size of testing database: %d\n", split(training, testing, images, 20, 66));*/

    return 0;
}
