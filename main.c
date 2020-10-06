#include <stdio.h> /* Standard input/output definitions */
#include <string.h> /* String function definitions */
#include <unistd.h> /* UNIX standard function definitions */
#include <fcntl.h> /* File control definitions */
#include <errno.h> /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#include "ESimPrinterHandler.h"

/*
 * 'open_port()' - Open serial port 1.
 * 
 * Returns the file descriptor on success or -1 on error
 */

int open_port(char* port)
{
    int fd; /* File descriptor for the port */

    fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);

    if(fd == 1)
    {
        /*
         * Could not open the port
         */

        perror("open_port: Unable to open /dev/ttyUSB0 - ");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
    }
    
    return (fd);
}

// void write_port(int fd, Data* d)
// {
//     int n;
//     n = write(fd, d->data, d->length);

//     if(n < 0)
//         fputs("write_port() failed\n", stderr);
// }

void close_port(int fd)
{
    close(fd);
}

int main()
{
    ESimReader *reader = openESimCommandFile("command.esim");
    
    printf("%s\n", readESimCommand(reader));
    printf("%s\n", readESimCommand(reader));

    closeESimCommandFile(reader);

    // InitData(data);

    // SetData(&data, "Hej");
    // printf("%s\n", (&data)->data);

    // SetData(&data, "C");
    // printf("%s\n", (&data)->data);

    // SetData(&data, "Nu ar det langt");
    // printf("%s\n", (&data)->data);
}