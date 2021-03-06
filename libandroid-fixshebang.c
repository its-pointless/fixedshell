#define _GNU_SOURCE
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <stdio.h>
#include <libgen.h>
#define OLD_PATH "/usr/bin"
#define BIN_SH "/data/data/com.termux/files/usr/bin/sh"
#define USR_BIN_ENV "/data/data/com.termux/files/usr/bin/env"
#define termbinpath  "/data/data/com.termux/files/usr/bin"

int execve(const char *filename, char *const argv[], char *const envp[]) {
	FILE *fp;
	char buffer[1024], *buffer_p, *buffer_e, *fgets_ret;
	int (*real_execve)(const char *filename, char *const argv[], char *const envp[]) = NULL;
	const char *newFilename;
	char **newArgv;
	int argc;
	char *DN = dirname(filename);
	char *BN = basename(filename);
	char st[strlen(BN) + strlen(termbinpath) + 2];
	newFilename = filename;
	newArgv = (char **) argv;


	if (strcmp(filename, "/usr/bin/env") == 0) {
		newFilename = USR_BIN_ENV;
		goto final;
	}

	if (strcmp(filename, "/bin/sh") == 0) {
		newFilename = BIN_SH;
		goto final;
	}

	if (strcmp(DN, OLD_PATH) == 0){
		sprintf(st,"%s/%s", termbinpath, BN);
		newFilename = st;
		goto final;
	}
	fp = fopen(filename, "r");
	if (fp == NULL) {
		goto final;
	}
	fgets_ret = fgets(buffer, sizeof(buffer), fp);
	fclose(fp);

	if (fgets_ret != buffer) {
		goto final;
	}

	if (strlen(buffer) < 3) {
		goto final;
	}

	if (buffer[strlen(buffer) - 1] != '\n') {
		goto final;
	}
	buffer[strlen(buffer) - 1] = '\0';

	if (buffer[0] != '#' || buffer[1] != '!') {
		goto final;
	}

	buffer_p = buffer;
	buffer_p += 2;

	while (buffer_p[0] == ' ') {
		buffer_p++;
	}

	buffer_e = strchr(buffer_p, ' ');
	if (buffer_e != NULL) {
		*buffer_e = '\0';
		buffer_e++;
	}

/*	if (strcmp(buffer_p, "/usr/bin/env") !=0  && strcmp(buffer_p, "/bin/sh") != 0 && strcmp(DN, OLD_PATH) != 0) {
		goto final;
	}
	*/
	
	if  ( strcmp(dirname(buffer_p), OLD_PATH) == 0){
		char SBN[strlen(basename(buffer_p)) + 1];
		strncpy(SBN, basename(buffer_p), sizeof(SBN)) ;
	if (strcmp(SBN ,"python") == 0 && getenv("SHEBANG_PYTHON2") != NULL) { 
		strcpy(SBN, "python2");}

		
	sprintf(st,"%s/%s", termbinpath, SBN);
                newFilename = st;
	for (argc = 0; argv[argc]; argc++) {}

        argc++;

        newArgv = malloc(sizeof(*newArgv) * (argc + 3));
        newArgv[0] = st;
        newArgv[1] =  filename;
        newArgv[2] = buffer_e;

        for (argc = 0; argv[argc]; argc++) {
                newArgv[argc + 2] = argv[argc];
        }
        newArgv[argc + 2] = NULL;
        }
        	
	if  (strcmp(buffer_p, "/bin/sh") ==0 ){
        newFilename = USR_BIN_ENV;
        for (argc = 0; argv[argc]; argc++) {}

        argc++;
        newArgv = malloc(sizeof(*newArgv) * (argc + 3));
        newArgv[0] = USR_BIN_ENV;
	newArgv[1] =  "sh"; 
	newArgv[2] = buffer_e;
         
	for (argc = 0; argv[argc]; argc++) {
                newArgv[argc + 2] = argv[argc];
        }
        newArgv[argc + 2] = NULL;
        }
	if  (strcmp(buffer_p, "/usr/bin/env") ==0 ){
        newFilename = USR_BIN_ENV;
        for (argc = 0; argv[argc]; argc++) {}

	while (buffer_e[0] == ' ') {
                buffer_e++;
        }
	
	argc++;
        
	newArgv = malloc(sizeof(*newArgv) * (argc + 3));
        newArgv[0] = USR_BIN_ENV;
        newArgv[1] = buffer_e;
	
	for (argc = 0; argv[argc]; argc++) {
                newArgv[argc + 2] = argv[argc];
	}        
        newArgv[argc + 2] = NULL;
	}
final:
        real_execve = dlsym(RTLD_NEXT, "execve");

        return(real_execve(newFilename, newArgv, envp));
}
