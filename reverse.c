/* This programs reverses the text from input and prints the reversed result to output. */
/* Author: Tomi Helin */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#define ALLOC_SIZE 100;

int main(int argc, char *argv[]) {
	// If caller gives more than 2 parameters then show usage message.
	if (argc > 3) {
		fprintf(stderr, "usage: reverse <input> <output>\n");
		exit(1);
	}

	// By default use stdin as default input stream and stdout as default output stream.
	FILE* input_stream = stdin;
	FILE* output_stream = stdout;

	// If both input and output are given as parameter.
	if (argc == 3) {
		char* input_file = basename(argv[1]);
		char* output_file = basename(argv[2]);

		// Check that input and output are different.
		if (strcmp(input_file, output_file) == 0) {
			fprintf(stderr, "reverse: input and output file must differ\n");
			exit(1);
		}
		
		// Open output file for write.
		output_stream = fopen(argv[2], "w");
		if (output_stream == NULL) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[2]);
			exit(1);
		}
	}

	// Open input file.
	if (argc >= 2) {
		// Open input file in read mode.
		input_stream = fopen(argv[1], "r");
		if (input_stream == NULL) {
			fprintf(stderr, "reverse: cannot open file '%s'\n", argv[1]);
			exit(1);
		}
	}


	int nr_alloc = ALLOC_SIZE;
	int nr_used = 0;

	char** lines = NULL;
	lines = malloc(nr_alloc * sizeof(char*));

	// Now read input stream until end of file is reached.
	while (feof(input_stream) == 0) {
		char *buffer = NULL;
		size_t size = 0;
		
		// Read stream until new line and allocate memory to hold input. Remeber to free.
		ssize_t chars_read = getline(&buffer, &size, input_stream);
		if (chars_read == -1) {
			free(buffer);
			if (errno != 0) {
				fprintf(stderr, "malloc failed\n");
				exit(1);
			}
			else {
				break;
			}
		}
		
		if (nr_used == nr_alloc) {
			nr_alloc = nr_alloc + ALLOC_SIZE;
			char** new_ptr = realloc(lines, nr_alloc * sizeof(char*));
			if (new_ptr == NULL) {
				fprintf(stderr, "realloc failed\n");
				exit(1);
			}
			
			lines = new_ptr;
		}

		lines[nr_used++] = buffer;
	}

	// Write lines in reversed order to output stream.
	for (int i=nr_used-1;i>=0;--i) {
		fprintf(output_stream, "%s", lines[i]);
	}

	// Free resources.
	for (int i=0;i<nr_used;++i) {
		free(lines[i]);
	}

	free(lines);

	// Close input and output streams.
	fclose(input_stream);
	fclose(output_stream);

	return 0;
}
