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

int main(int argc, char** argv)
{
	/* init variables */
	struct dirent *dp;
	DIR *dirPointer;
	DIR *outputPointer;
	char *dirPath;
	char *outputPath;

	/* check invalid number of arguments */
   	if (argc != 4){
		printf("Not enough arguments. Exiting... \n" );
		return -1;
	}

	/* check if input directory exists */
	dirPath = argv[1] ;
 	if ((dirPointer = opendir(dirPath)) == NULL) {
		printf("Error opening input directory: %s\n", strerror(errno));
		return errno; // ERROR!
	}

	/* check if encryption file exists */
   	int encKeyFile = open(argv[2], O_RDWR);

   	if( encKeyFile < 0 ){
        	printf( "Error opening  encrypton key file : %s\n", strerror(errno) );
        	return errno; /* what is it? */
   	}   

	/* check if the output directory exists */
	outputPath = argv[3] ;
 	if ((outputPointer = opendir(outputPath)) == NULL) {
		printf("Creating new directory\n"); /*TO DELETE*/
		int newOutputDir = mkdir(outputPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		/* check if success in mkdir */
		if (newOutputDir<0){
			printf( "Error opening  creating output directory : %s\n", strerror(errno) );
        		return errno; /* what is it? */
		}
	}

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

   	close(encKeyFile); // close encKeyFile
*/
	printf("end of main function\n");
}
