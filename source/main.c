// c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// linux
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// POSIX
#include <dirent.h>
#include <errno.h>

// print tabs
void LIST__print_tabs(unsigned long long depth) {
	// check if more tabs to print
	while (depth > 0) {
		// print one tab
		printf("| ");

		// one less tab to print
		depth -= 1;
	}

	// finished
	return;
}

// print file
void LIST__print_file(char* full_file_path, char* file_type, unsigned long long tab_depth) {
	struct stat statistics;

	// get file statistics
	stat(full_file_path, &statistics);

	// print tabs
	LIST__print_tabs(tab_depth);

	// print desired data
	printf("%s:%s:[%llu]\n", full_file_path, file_type, (unsigned long long)statistics.st_size);

	// finished
	return;
}

// print directory
void LIST__print_directory(char* full_directory_path, unsigned long long tab_depth) {
	DIR* directory;
	struct dirent* directory_entry;
	char file_path_temp[PATH_MAX];
	char* file_type_temp;
	char* file_type_names[] = {
		"block_device",
		"character_device",
		"directory",
		"fifo_named_pipe",
		"symbolic_link",
		"regular_file",
		"unix_domain_socket",
		"unknown_type",
	};

	// print the directory name
	LIST__print_tabs(tab_depth);
	printf("%s\n", full_directory_path);

	// open the directory
	directory = opendir(full_directory_path);

	// print all file's full names and the contents of each sub-directory ("." and ".." are printed but are not traversed)
	while (errno == 0 && (directory_entry = readdir(directory)) != NULL) {
		// reset temp path space
		for (unsigned long long i = 0; i < PATH_MAX; i++) {
			file_path_temp[i] = 0;
		}

		// build full path
		strcat(file_path_temp, full_directory_path);
		strcat(file_path_temp, "/");
		strcat(file_path_temp, (*directory_entry).d_name);

		// print directory
		if ((*directory_entry).d_type == DT_DIR) {
			// call this function to recurse directory
			if (strcmp((*directory_entry).d_name, ".") != 0 && strcmp((*directory_entry).d_name, "..") != 0) {
				// print descendant
				LIST__print_directory(file_path_temp, tab_depth + 1);

				// reset errno in case file or directory access was denied
				errno = 0;
			} else {
				// print '.' and '..'
				LIST__print_tabs(tab_depth + 1);
				printf("%s\n", file_path_temp);
			}
		// print files and others
		} else {
			// determine file type name
			if ((*directory_entry).d_type == DT_BLK) {
				file_type_temp = file_type_names[0];
			} else if ((*directory_entry).d_type == DT_CHR) {
				file_type_temp = file_type_names[1];
			} else if ((*directory_entry).d_type == DT_FIFO) {
				file_type_temp = file_type_names[3];
			} else if ((*directory_entry).d_type == DT_LNK) {
				file_type_temp = file_type_names[4];
			} else if ((*directory_entry).d_type == DT_REG) {
				file_type_temp = file_type_names[5];
			} else if ((*directory_entry).d_type == DT_SOCK) {
				file_type_temp = file_type_names[6];
			} else {
				file_type_temp = file_type_names[7];
			}

			// print file
			LIST__print_file(file_path_temp, file_type_temp, tab_depth + 1);
		}
	}

	// clean up
	closedir(directory);

	// finished
	return;
}

// program entry point
int main() {
	char* run_from;

	// inform user of program start
	printf("----------\n");

	// setup variables
	run_from = realpath(".", 0);

	// print current directory
	LIST__print_directory(run_from, 0);

	// clean up
	free(run_from);

	// inform user of program completion
	printf("----------\n");

	// exit program
	return 0;
}
