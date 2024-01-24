#ifndef CFS_H
#define CFS_H

//! A single header C filesystem library.
//! from https://github.com/Kn-Ht/cfs.h

#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CFS_PATHINFO_DUMP(_PI) \
    printf("{exists = %d, is_file = %d, is_dir = %d}\n", (_PI).exists, (_PI).is_file, (_PI).is_dir)

#ifdef _WIN32
#  ifndef S_ISDIR
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  define S_ISDIR(s) ((s).st_mode & S_IFDIR)
#else
#  include <unistd.h>
#endif

// CfsPath functions

typedef struct {
    bool is_file;
    bool is_dir;
    bool exists;
} PathInfo;

typedef struct {
    char buf[_MAX_PATH];
} Path;

Path cfs_path_new(const char* from) {
    Path c;
    strncpy(c.buf, from, sizeof c.buf);
    return c;
}

bool cfs_path_exists(Path path, errno_t* err) {
    struct stat st;
    int res = stat(path.buf, &st);

    if (res == -1) {
        if (err) *err = (errno == ENOENT)? 0 : errno;
        return false;
    } else {
        return true;
    }
}

bool cfs_path_is_dir(Path path, errno_t* err) {
    struct stat st;
    int res = stat(path.buf, &st);

    if (res == -1) {
        if (err) *err = (errno == ENOENT)? 0 : errno;
        return false;
    } else {
        return (st.st_mode & S_IFDIR) > 0;
    }
}

bool cfs_path_is_file(Path path, errno_t* err) {
    struct stat st;
    int res = stat(path.buf, &st);

    if (res == -1) {
        if(err) *err = (errno == ENOENT)? 0 : errno;
        return false;
    } else {
        return (st.st_mode & S_IFREG) > 0;
    }
}

PathInfo cfs_path_check(Path path, errno_t* err) {
    struct stat st;
    int res = stat(path.buf, &st);
    PathInfo pi = {0};

    if (res == -1) {
        if (err) *err = (errno == ENOENT)? 0 : errno;
        return pi;
    } else {
        pi.is_file = (st.st_mode & S_IFREG) > 0;
        pi.is_dir = (st.st_mode & S_IFDIR) > 0;
        pi.exists = true;
    }
}



#endif
#endif //CFS_H