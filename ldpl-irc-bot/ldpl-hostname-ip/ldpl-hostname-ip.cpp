#include <stdio.h> //printf
//#include <string.h> //memset
#include <stdlib.h> //for exit(0);
//#include <sys/socket.h>
#include <netdb.h>	//hostent
#include <arpa/inet.h>
#include "ldpl-types.h"

using namespace std;

int hostname_to_ip(const char *, char *);

ldpl_text URLRESOLVER_HOSTNAME = "";
ldpl_text URLRESOLVER_IP = "";

void URLRESOLVER_RESOLVEURL_CPP(){
    struct hostent* pHostInfo;
    long nHostAddress;
    /* get IP address from name */
    pHostInfo = gethostbyname(URLRESOLVER_HOSTNAME.str_rep().c_str());
    if(!pHostInfo){
        printf("LDPL Url Resolver error: could not resolve host name.\n");
        exit(1);
    }
    /* copy address into long */
    memset(&nHostAddress, 0, sizeof(nHostAddress));
    memcpy(&nHostAddress, pHostInfo->h_addr, pHostInfo->h_length);

    char ip[256];
    inet_ntop (pHostInfo->h_addrtype, pHostInfo->h_addr, ip, 256);
    URLRESOLVER_IP = ip;
}
