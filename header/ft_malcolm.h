#ifndef FT_MALCOLM_H
#define FT_MALCOLM_H

#pragma once

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <stdbool.h>

#include "../libft/libft.h"

// Colors Set
#define BLACK "\e[1;30m"
#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define YELLOW "\e[1;33m"
#define BLUE "\e[1;34m"
#define MAGENTA "\e[1;35m"
#define CYAN "\e[1;36m"
#define RESET "\e[0m"

bool	parsing(int ac, char **av);

#endif