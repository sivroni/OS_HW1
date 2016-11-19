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
#include <stdlib.h>
#define MAX 1024
#define BYTE_SIZE 1

int main(int argc, char** argv)
{
	/* init variables */
	struct dirent *dpINPUT; /*struct directory */
	DIR *dirPointer; /* pointer to input directory */
	DIR *outputPointer; /* pointer to output directory */
	char *dirPath; /* path of input directory */
	char *outputDirectoryPath; /* path of output directory (if not exists - create one) */
	int bufferSizeInput=0; /* used in case we need to repeat on the key file */
	char filename_input[MAX]; /* input file path - string */
	char outputPathFile[MAX]; /* output file path - string */
	char buffer[BYTE_SIZE]; /* string from input file */
	char buffer_key[BYTE_SIZE]; /* string from key file */
	char buffer_output[BYTE_SIZE]; /*written to output file byte by byte */
	int fd = -1; /* input file - after open call*/
	int fd_output = -1; /* output file - after open call*/
	struct stat keyFileStat; /*create stat to determine size */
	int read_input_file; /*num of bytes read from input file */
	int read_key_file; /*num of bytes read from key file */
	int write_to_file; /*after write function */
	int i; /* iterates the buffers */
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
        	printf( "Error opening encrypton key file : %s\n", strerror(errno) );
		closedir(dirPointer);
        	return errno; 
   	}   
	close(encKeyFile); /* will open the key file on every iteration */

	/* check if the output directory exists */
	outputDirectoryPath = argv[3] ;
 	if ((outputPointer = opendir(outputDirectoryPath)) == NULL) {
		//printf("Creating new directory\n");
		int newOutputDir = mkdir(outputDirectoryPath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		/* check if success in mkdir */
		if (newOutputDir<0){
			closedir(dirPointer);
			printf( "Error opening  creating output directory : %s\n", strerror(errno) );
        		return errno; 
		}
	}

	// MAIN LOOP //
	// ========= //
	// go to input directory 
	// for each file in directory
		// open key file
		// create output file
		// xor with key file - xor one byte every time
		// close file from input + output + key
	// close input dir + output dir

	//printf( "Entering main loop\n");
	//int count_eof = 0;
	 while ((dpINPUT = readdir(dirPointer)) != NULL){ /* iterate on every file in input directory */
		
        	sprintf(filename_input, "%s/%s", dirPath,dpINPUT->d_name); /* full path to file in input */
	        sprintf(outputPathFile, "%s/%s", outputDirectoryPath,dpINPUT->d_name); /* full path to file in output */
        	
		struct stat inputFileStat; /*create stat to determine size */
		//printf( "Iterating source file %s\n", filename_input);
		if ( stat(filename_input, &inputFileStat)<0 ){ /*check for error */
			printf("Error while using fstat: %s\n", strerror(errno));
			closedir(dirPointer);
			closedir(outputPointer);
			return errno; 
		}
		 
        	if (S_ISDIR(inputFileStat.st_mode)) { /* skip directories */
			//printf("entered isdir\n");
			continue;
		}


 		fd=open(filename_input,O_RDONLY); /*open input file to read only */
		if (fd <0 ){ /*check for error */
			printf("Error opening input file: %s\n", strerror(errno));
			closedir(dirPointer);
			closedir(outputPointer);
			return errno; 
		}
		
		fd_output = open(outputPathFile, O_RDWR | O_CREAT | O_TRUNC,0777 ); /* opens/creates an output file */
		if (fd_output <0){ /*check for error */
			printf("Error opening output file: %s\n", strerror(errno));
			closedir(dirPointer);
			closedir(outputPointer);
			close(fd);
			return errno; 
	
		}

		
		
   		encKeyFile = open(argv[2], O_RDWR); /* opens key file from the beggining */
	
   		if( encKeyFile < 0 ){
        		printf( "Error opening  encrypton key file : %s\n", strerror(errno) );
			closedir(dirPointer);
			closedir(outputPointer);
			close(fd);
			close(fd_output);
        		return errno; 
   		}   

		if ( stat(argv[2], &keyFileStat)<0 ){ /*check for error + check size of key file */
			printf("Error while using fstat: %s\n", strerror(errno));
			closedir(dirPointer);
			closedir(outputPointer);
			close(fd);
			close(fd_output);
			return errno; 
		}

		
		while ((read_input_file = read(fd,buffer, BYTE_SIZE))>0){ /*read until EOF */
				

				read_key_file = read(encKeyFile,buffer_key, read_input_file );
				if (read_key_file < 0){
						printf("Error while readng from key file: %s\n", strerror(errno));
						closedir(dirPointer);
						closedir(outputPointer);
						close(fd);
						close(fd_output);
						close(encKeyFile);
						return errno; 
				}
				else {
					if (read_key_file == 0){ /* file key EOF - we need to open it again */
						//count_eof++;
						close(encKeyFile);
						encKeyFile = open(argv[2], O_RDWR); /* opens key file from the beggining */
	
   						if( encKeyFile < 0 ){
        						printf( "Error opening encrypton key file : %s\n", strerror(errno) );
							closedir(dirPointer);
							closedir(outputPointer);
							close(fd);
							close(fd_output);
        					return errno; 
   						}   

						read_key_file = read(encKeyFile,buffer_key, read_input_file );
						if (read_key_file < 0){
							printf("Error while readng from key file: %s\n", strerror(errno));
							closedir(dirPointer);
							closedir(outputPointer);
							close(fd);
							close(fd_output);
							close(encKeyFile);
						return errno; 
						}
					} 
				}
				for (i=0; i<read_input_file; i++)
					buffer_output[i] = (char) (buffer[i] ^ buffer_key[i]); /* XOR */
				write_to_file = write(fd_output, buffer_output, read_input_file);
				if (write_to_file <0){
					printf("Error while using writing to output file: %s\n", strerror(errno));
						closedir(dirPointer);
						closedir(outputPointer);
						close(fd);
						close(fd_output);
						close(encKeyFile);
						return errno; 
				}
			
		} /* end of while loop */
		if (read_input_file < 0){ /* if read_input_file == 0 do EOF */
				printf("Error while reading input file: %s\n", strerror(errno));
				closedir(dirPointer);
				closedir(outputPointer);
				close(fd);
				close(fd_output);
				close(encKeyFile);
				return errno; 
		}
		//printf("iterated on key %d times\n", count_eof);
		//count_eof = 0;
		close(encKeyFile); // close encKeyFile
		close(fd); // close input file
		close(fd_output); // close output file
	}
	
	closedir(dirPointer); /*maybe there is another func for this? */
	closedir(outputPointer);
	printf("end of main function\n");
}



