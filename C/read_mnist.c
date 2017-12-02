#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VECTOR_LENGTH 28*28
#define _CRT_SECURE_NO_DEPRECATE

typedef struct {
	unsigned char vector[VECTOR_LENGTH];
	int label;
} element;


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
	}
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

void main(void)
{
	element images[9000];
	element training[9000];
	element testing[9000];
	read_mnist(images, 9000, "test_labels.byte", "test_images.byte");

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
	system("PAUSE");
}
