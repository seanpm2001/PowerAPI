#include "oshw_cpudev.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int cpudev_verbose = 0;

int online_cpulist[1000];
int num_cpus;
int started = 0;

int online_cpus(int number_desired)
{
    int i = 0;
    
    //int num_cpus = sysconf(_SC_NPROCESSORS_CONF);

    for (i = 1; i <= number_desired - 1; i++){
        if (online_cpulist[i] == 0){ 
            int fd;
            char one = '1';
            char cpupath[100];
            sprintf(cpupath,"/sys/devices/system/cpu/cpu%i/online",i);
            fd = open(cpupath, O_WRONLY);
            write (fd, &one, 1);
            close(fd);
            online_cpulist[i] = 1;
        }
        else{
            //printf("skip, already on\n");
        }
    }
    for (i = number_desired; i < num_cpus; i++){
        if (online_cpulist[i] == 1){
            int fd;
            char zero = '0';
            char cpupath[100];
            sprintf(cpupath,"/sys/devices/system/cpu/cpu%i/online",i);
            fd = open(cpupath, O_WRONLY);
            write (fd, &zero, 1);
            close(fd);
            online_cpulist[i] = 0;
        }
        else{
            //printf("skip, already off\n");
        }
    }

    return 0;
}

int parallel_hint(PWR_Obj obj, PWR_Hint hint, int parallel)
{
    if( started == 0 ) {
        //printf("first time hint is run, bootstrapping \n");
        num_cpus = sysconf(_SC_NPROCESSORS_CONF);
        int i;
        for (i = 1; i <= num_cpus - 1; i++){
            int fd;
            char one = '1';
            char cpupath[100];
            sprintf(cpupath,"/sys/devices/system/cpu/cpu%i/online",i);
            fd = open(cpupath, O_WRONLY);
            write (fd, &one, 1);
            close(fd);
            online_cpulist[i] = 1;
        }
        started = 1;
    }

    /* For now we assume that this always operates on the node */
    switch( hint ) {
       case PWR_REGION_PARALLEL:
           online_cpus(parallel);
           break;
       case PWR_REGION_SERIAL:
           online_cpus(1);
           break;
       case PWR_REGION_COMPUTE:
           break;
       case PWR_REGION_COMMUNICATE:
           online_cpus(2);
           break;
       default:
           fprintf(stderr,"INVALID HINT\n");
    }

    return 0;
}

pwr_dev_t oshw_cpudev_init( const char *initstr )
{
    return 0x0;
}

int oshw_cpudev_final( pwr_dev_t dev )
{
    return 0;
}

pwr_fd_t oshw_cpudev_open( pwr_dev_t dev, const char *initstr )
{
    return 0x0;
}

int oshw_cpudev_close( pwr_fd_t fd )
{
    return 0;
}

int oshw_cpudev_read( pwr_fd_t fd, PWR_AttrName attr, void *value, unsigned int len, PWR_Time *timestamp )
{
    if( len != sizeof(double) ) {
        printf( "Error: value field size of %u incorrect, should be %ld\n", len, sizeof(double) );
        return -1;
    }

    switch( attr ) {
        case PWR_ATTR_PSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_CSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_SSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_FREQ:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_TEMP:
            *((double *)value) = (double)0;
            break;
        default:
            printf( "Warning: unknown OSHW reading attr (%u) requested\n", attr );
            break;
    }
    *timestamp = 0;

    if( cpudev_verbose )
        printf( "Info: reading of type %u at time %llu with value %lf\n",
                attr, *(unsigned long long *)timestamp, *(double *)value );

    return 0;
}

int oshw_cpudev_write( pwr_fd_t fd, PWR_AttrName attr, void *value, unsigned int len )
{
    if( len != sizeof(double) ) {
        printf( "Error: value field size of %u incorrect, should be %ld\n", len, sizeof(double) );
        return -1;
    }

    switch( attr ) {
        case PWR_ATTR_PSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_CSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_SSTATE:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_FREQ:
            *((double *)value) = (double)0;
            break;
        case PWR_ATTR_TEMP:
            *((double *)value) = (double)0;
            break;
        default:
            printf( "Warning: unknown OSHW writing attr (%u) requested\n", attr );
            break;
    }

    if( cpudev_verbose )
        printf( "Info: writing of type %u with value %lf\n", attr, *(double *)value );

    return 0;
}

int oshw_cpudev_readv( pwr_fd_t fd, unsigned int arraysize,
    const PWR_AttrName attrs[], void *values, PWR_Time timestamp[], int status[] )
{
    unsigned int i;

    for( i = 0; i < arraysize; i++ ) {
        switch( attrs[i] ) {
            case PWR_ATTR_PSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_CSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_SSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_FREQ:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_TEMP:
                *((double *)values+i) = (double)0;
                break;
            default:
                printf( "Warning: unknown OSHW reading attr (%u) requested at position %u\n", attrs[i], i );
                break;
        }
        timestamp[i] = 0;

        if( cpudev_verbose )
            printf( "Info: reading of type %u with value %lf\n", attrs[i], *((double *)(values+i)) );
    }

    return 0;
}

int oshw_cpudev_writev( pwr_fd_t fd, unsigned int arraysize,
    const PWR_AttrName attrs[], void *values, int status[] )
{
    unsigned int i;

    for( i = 0; i < arraysize; i++ ) {
        switch( attrs[i] ) {
            case PWR_ATTR_PSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_CSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_SSTATE:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_FREQ:
                *((double *)values+i) = (double)0;
                break;
            case PWR_ATTR_TEMP:
                *((double *)values+i) = (double)0;
                break;
            default:
                printf( "Warning: unknown OSHW writing attr (%u) requested at position %u\n", attrs[i], i );
                break;
        }

        if( cpudev_verbose )
            printf( "Info: writing of type %u with value %lf\n", attrs[i], *((double *)(values+i)) );
    }

    return 0;
}

int oshw_cpudev_time( pwr_fd_t fd, PWR_Time *timestamp )
{
    return 0;
}

int oshw_cpudev_clear( pwr_fd_t fd )
{
    return 0;
}

static plugin_dev_t dev = {
    .init   = oshw_cpudev_init,
    .final  = oshw_cpudev_final,
    .open   = oshw_cpudev_open,
    .close  = oshw_cpudev_close,
    .read   = oshw_cpudev_read,
    .write  = oshw_cpudev_write,
    .readv  = oshw_cpudev_readv,
    .writev = oshw_cpudev_writev,
    .time   = oshw_cpudev_time,
    .clear  = oshw_cpudev_clear
};

plugin_dev_t* getDev() {
    return &dev;
}
