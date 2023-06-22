#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* The RGB values of a pixel. */
struct Pixel {
    int red;
    int green;
    int blue;
};

/* An image loaded from a PPM file. */
struct PPM {
    /* TODO: Question 1 */
    char * mNum;
    int height;
    int width;
    int max;
    struct Pixel *pixel;
};

/* Reads an image from an open PPM file.
 * Returns a new struct PPM, or NULL if the image cannot be read. */
struct PPM *getPPM(FILE * f)
{
    /* TODO: Question 2a */
    struct PPM *newPPM;
    int i;

    if ((newPPM = (struct PPM *) malloc(sizeof(struct PPM))) == NULL) {
        printf("Failed to allocate memory for new PPM\n");
        return NULL;
    }

    if ((newPPM->mNum = (char *) malloc(sizeof(char)*3)) == NULL) {
        printf("Failed to allocate memory for char array in new PPM\n");
        return NULL;
    }
    fscanf(f, "%s", newPPM->mNum);
    fscanf(f, "%d %d %d", &newPPM->width, &newPPM->height, &newPPM->max);

    newPPM->pixel = (struct Pixel *) malloc(sizeof(struct Pixel)*(newPPM->width)*(newPPM->height));

    for (i = 0; i < (newPPM->width)*(newPPM->height); i++) {
        fscanf(f, "%d %d %d", &newPPM->pixel[i].red, &newPPM->pixel[i].green, &newPPM->pixel[i].blue);
    }
    return newPPM;
}

/* Write img to stdout in PPM format. */
void showPPM(const struct PPM *img)
{
    int i;
    /* TODO: Question 2b */

    printf("%s\n", img->mNum);
    printf("%d %d\n%d\n", img->width, img->height, img->max);
    for (i = 0; i < img->height*img->width; i++) {
        printf("%d %d %d\n", img->pixel[i].red, img->pixel[i].green, img->pixel[i].blue);
    }

}

/* Opens and reads a PPM file, returning a pointer to a new struct PPM.
 * On error, prints an error message and returns NULL. */
struct PPM *readPPM(const char *filename)
{
    /* Open the file for reading */
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "File %s could not be opened.\n", filename);
        return NULL;
    }

    /* Load the image using getPPM */
    struct PPM *img = getPPM(f);

    /* Close the file */
    fclose(f);

    if (img == NULL) {
        fprintf(stderr, "File %s could not be read.\n", filename);
        return NULL;
    }

    return img;
}

/* Make a copy of the PPM structure. */
struct PPM *PPM_copy(const struct PPM *fcopy) {
    struct PPM *scopy;
    int MAX_ITER = (fcopy->height*fcopy->width);
    int i;

    if ((scopy = (struct PPM *) malloc(sizeof(struct PPM))) == NULL) {
        printf("Couldn't allocate memory for encoded PPM\n");
        return NULL;
    }

    if ((scopy->mNum = (char *) malloc(sizeof(char)*3)) == NULL) {
        printf("Couldn't allocate memory for char array in encoded PPM\n");
        return NULL;
    }

    scopy->mNum = fcopy->mNum;
    scopy->width = fcopy->width;
    scopy->height = fcopy->height;
    scopy->max = fcopy->max;

    if ((scopy->pixel = (struct Pixel *) malloc(sizeof(struct Pixel)*(scopy->width)*(scopy->height))) == NULL) {
        printf("Failed to allocate memory for pixel array in new PPM\n");
        return NULL;
    }
    for (i = 0; i < MAX_ITER; i++) {
        scopy->pixel[i].red = fcopy->pixel[i].red;
        scopy->pixel[i].green = fcopy->pixel[i].green;
        scopy->pixel[i].blue = fcopy->pixel[i].blue;
    }

    return scopy;
    
}

/* Encode the string text into the red channel of image img.
 * Returns a new struct PPM, or NULL on error. */
struct PPM *encode(const char *text, const struct PPM *img)
{
    /* TODO: Question 2c */
    int text_size = strlen(text);
    int i = 0;
    struct PPM *ePPM = PPM_copy(img);
    int rInt = 0;
    int MAX_ITER = ePPM->width * ePPM->height;

    ePPM->pixel[0].red = text_size;

    while (text[i] != '\0') {
        rInt += rand() % (MAX_ITER/text_size);
        if (ePPM->pixel[rInt].red == text[i])
            ePPM->pixel[rInt++].red = text[i++];
        else
            ePPM->pixel[rInt].red = text[i++];
    }

    return ePPM;
}

/* Extract the string encoded in the red channel of newimg, by comparing it
 * with oldimg. The two images must have the same size.
 * Returns a new C string, or NULL on error. */
char *decode(const struct PPM *oldimg, const struct PPM *newimg)
{
    /* TODO: Question 2d */
    char *message;
    int i; // iterate through PPM
    int j; // iterate through message

    int MAX_ITER = oldimg->height * oldimg->width;

    if ((strcmp(oldimg->mNum,newimg->mNum) != 0) || (oldimg->height != newimg->height) || (oldimg->width != newimg->width) || (oldimg->max != newimg->max)) {
        fprintf(stderr, "Image dimension mismatch");
        return NULL;
    }

    if ((message = (char *)malloc(sizeof(char)*(newimg->pixel[0].red))) == NULL) {
        printf("Couldn't allocate memory for decoded message\n");
        return NULL;
    }

    j = 0;
    for (i = 1; i < MAX_ITER; i++) {
        if (newimg->pixel[i].red != oldimg->pixel[i].red) {
            message[j++] = newimg->pixel[i].red;
        }
    }

    return message;
}

/* TODO: Question 3 */
int main(int argc, char *argv[])
{
    /* Initialise the random number generator, using the time as the seed */
    srand(time(NULL));

    /* Parse command-line arguments */
    if (argc == 3 && strcmp(argv[1], "t") == 0) {
        /* Mode "t" - test PPM reading and writing */

        struct PPM *img = readPPM(argv[2]);
        showPPM(img);
       

    } else if (argc == 3 && strcmp(argv[1], "e") == 0) {
        /* Mode "e" - encode PPM */

        struct PPM *oldimg = readPPM(argv[2]);

        const int MAX = (oldimg->height*oldimg->width)/5;

        /* TODO: prompt for a message from the user, and read it into a string */
        char message[MAX+1];
        fprintf(stderr, "Enter PPM encoding (Max Encoding = %d): ", MAX);
        fgets(message, MAX+1, stdin);
        fprintf(stderr,"%s", message);

        /* TODO: encode the text into the image with encode, and assign to newimg */
        struct PPM *newimg = encode(message, oldimg);

        /* TODO: write the image to stdout with showPPM */
        showPPM(newimg);

    } else if (argc == 4 && strcmp(argv[1], "d") == 0) {
        /* Mode "d" - decode PPM */

        struct PPM *oldimg = readPPM(argv[2]);
        /* TODO: get original file filename from argv, load it with
           readPPM, then assign to oldimg */

        struct PPM *newimg = readPPM(argv[3]);
        /* TODO: get encoded file filename from argv, load it with
           readPPM, then assign to newimg */

        /* TODO: decode the encodedPPM with the comparisonPPM and assign to message */
        char * decoded;
        decoded = decode(oldimg,newimg);
        /* TODO: print the decoded message to stdout */
        fprintf(stdout, "%s\n", decoded);


    } else {
        fprintf(stderr, "Unrecognised or incomplete command line.\n");
        return 1;
    }

    return 0;
}
