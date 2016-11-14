#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h> // for open flags
#include <unistd.h>
#include <time.h> // for time measurement
#include <sys/time.h>
#include <assert.h>
#include <errno.h> 
#include <string.h>
#define MAX 1024

/* TO DO LIST - 
** 1. exit while closing all files/directories.
** 2. take care of all result values.
** 3. free memory allocation. 
**/ 

int main(int argc, char** argv)
{
	/* init variables */
	struct dirent *dpINPUT; /*struct directory
	DIR *dirPointer; /* pointer to input directory */
	DIR *outputPointer; /* pointer to output directory */
	char *dirPath; /* path of input directory */
	//char *outputPathFile; /*path to the input file after encrypy/decrypt */
	char *outputDirectoryPath; /* path of output directory (if not exists - create one) */
	int bufferSizeInput; /* number of bits to read from input file */
	int bufferSizeKey; /* number of bits to read from key file */
	char filename_input[MAX];
	char outputPathFile[MAX];
	int fd = -1; /* input file - after open */
	int fd_output = -1; /* output file - after open */
	struct stat keyFileStat; /*create stat to determine size */


	/* check invalid number of arguments */
   	if (argc != 4){
		printf("Not enough arguments. Exiting... \n" );
		return -1;
	}

	/* check if input directory exists */
	dirPath = argv[1] ;
 	if ((dirPointer = opendir(dirPath)) == NULL) {
		printf("Error opening input directory: %s\n", strerror(errno));
		return errno; 
	}

	/* check if encryption file exists */
   	int encKeyFile = open(argv[2], O_RDWR);

   	if( encKeyFile < 0 ){
        	printf( "Error opening  encrypton key file : %s\n", strerror(errno) );
        	return errno; 
   	}   

	if ( fstat(encKeyFile, &keyFileStat)<0 ){ /*check for error + check size of key file */
		printf("Error while using fstat: %s\n", strerror(errno));
		return errno; 
		}

	/* check if the output directory exists */
	outputDirectoryPath = argv[3] ;
 	if ((outputPointer = opendir(outputDirectoryPath)) == NULL) {
		printf("Creating new directory\n"); /*TO DELETE*/
		int newOutputDir = mkdir(outputDirectoryPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		/* check if success in mkdir */
		if (newOutputDir<0){
			printf( "Error opening  creating output directory : %s\n", strerror(errno) );
        		return errno; 
		}
	}

	// MAIN LOOP //
	// ========= //
	// go to input directory 
	// for each file in directory
		// create output file
		// xor with key file (check the buffer size and adjust) + 
		// move key to start
		// close file from input + output
	// close key file + input dir + output dir

	 while ((dpINPUT = readdir(dirPointer)) != NULL){ /* iterate on every file in imput directory */

        	sprintf(filename_input, "%s/%s", dirPath,dpINPUT->d_name); /* full path to file in input */
	        sprintf(outputPathFile, "%s/%s", outputDirectoryPath,dpINPUT->d_name); /* full path to file in output */
        	struct stat inputFileStat; /*create sts to determine size */

 		fd=open(filename_input,O_RDONLY); /*open input file to read only */
		if (fd == -1){ /*check for error */
			printf("Error opening file: %s\n", strerror(errno));
			return errno; 
		}

		if (fd_output == -1){
			printf("Error opening file: %s\n", strerror(errno));
			return errno; 
	
		}

		if ( fstat(fd, &inputFileStat)<0 ){ /*check for error */
			printf("Error while using fstat: %s\n", strerror(errno));
			return errno; 
		}

		if (inputFileStat.st_size > keyFileStat.st_size){

		}

		// Allocate enough to hold the whole contents plus a '\0' char.
		//char *buff = malloc(fileStat.st_size + 1);
		//free(buff);
		close(fd);
	}
	close(encKeyFile); // close encKeyFile

/*
struct stat fileStat;
fstat(fd, &fileStat); // Don't forget to check for an error return in real code
// Allocate enough to hold the whole contents plus a '\0' char.
char *buff = malloc(fileStat.st_size + 1);
free(buff);
*/
/*
    char filename_qfd[100] ;

    // read stream entries
    printf("list directory contents\n");

    while ((dp = readdir(dfd)) != NULL)
    {
        // full path to file
        sprintf(filename_qfd, "%s/%s", dir,dp->d_name) ;

        // call stat to get file metadata
        struct stat statbuf ;
        if (stat(filename_qfd,&statbuf ) < 0) {
			printf("Error getting stat of file: %s\n", strerror(errno));
			return -1; // ERROR!
		}

        // skip directories
        if (!S_ISDIR(statbuf.st_mode)) {
			printf("%s\n", filename_qfd);
		}
    }

closedir(dfd);
*/
 	// read first 1024 characters

////////////////////////////////
/*
	char buf[1024];
	
	ssize_t len = read(fd, buf, 1024);
   	if (len < 0) {
		printf("Error reading from file: %s\n", strerror(errno));
		return -1;
	}
	buf[len] = '\0'; // string closer

   	
*/
	printf("end of main function\n");
}
