#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc!=3) {
        fprintf(stderr,"Uso: %s <on|off> <dispositivo>\n",argv[0]);
        return EXIT_FAILURE;
    }
    const char *cmd = strcmp(argv[1],"on")==0 ? "ON" :
                      strcmp(argv[1],"off")==0? "OFF": NULL;
    if (!cmd) { fprintf(stderr,"Comando inválido\n"); return EXIT_FAILURE; }
    int fd = open(argv[2],O_RDWR);
    if (fd<0) { perror("open"); return EXIT_FAILURE; }
    if (write(fd,cmd,strlen(cmd))<0) { perror("write"); close(fd); return EXIT_FAILURE; }
    close(fd);
    return EXIT_SUCCESS;
}
