/*
===========================================================================
Copyright (C) 1997-2001 Id Software, Inc.

This file is part of Quake 2 source code.

Quake 2 source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake 2 source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Quake 2 source code; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// win_net.c

#include "winsock2.h"	// Knightmare- use newer winsock, was winsock.h
#include "wsipx.h"
#include "winnewerror.h"
#include "../qcommon/qcommon.h"

#define	MAX_LOOPBACK	4

typedef struct
{
	byte	data[MAX_MSGLEN];
	int		datalen;
} loopmsg_t;

typedef struct
{
	loopmsg_t	msgs[MAX_LOOPBACK];
	int			get, send;
} loopback_t;


//cvar_t			*net_shownet;	// unused
static cvar_t	*noudp;
static cvar_t	*noipx;

loopback_t	loopbacks[2];
int			ip_sockets[2];
int			ipx_sockets[2];

char *NET_ErrorString (void);


//=============================================================================

void NetadrToSockadr (netadr_t *a, struct sockaddr *s)
{
	memset (s, 0, sizeof(*s));

	if (a->type == NA_BROADCAST)
	{
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_port = a->port;
		((struct sockaddr_in *)s)->sin_addr.s_addr = INADDR_BROADCAST;
	}
	else if (a->type == NA_IP)
	{
		((struct sockaddr_in *)s)->sin_family = AF_INET;
		((struct sockaddr_in *)s)->sin_addr.s_addr = *(int *)&a->ip;
		((struct sockaddr_in *)s)->sin_port = a->port;
	}
	else if (a->type == NA_IPX)
	{
		((struct sockaddr_ipx *)s)->sa_family = AF_IPX;
		memcpy(((struct sockaddr_ipx *)s)->sa_netnum, &a->ipx[0], 4);
		memcpy(((struct sockaddr_ipx *)s)->sa_nodenum, &a->ipx[4], 6);
		((struct sockaddr_ipx *)s)->sa_socket = a->port;
	}
	else if (a->type == NA_BROADCAST_IPX)
	{
		((struct sockaddr_ipx *)s)->sa_family = AF_IPX;
		memset(((struct sockaddr_ipx *)s)->sa_netnum, 0, 4);
		memset(((struct sockaddr_ipx *)s)->sa_nodenum, 0xff, 6);
		((struct sockaddr_ipx *)s)->sa_socket = a->port;
	}
}

void SockadrToNetadr (struct sockaddr *s, netadr_t *a)
{
	if (s->sa_family == AF_INET)
	{
		a->type = NA_IP;
		*(int *)&a->ip = ((struct sockaddr_in *)s)->sin_addr.s_addr;
		a->port = ((struct sockaddr_in *)s)->sin_port;
	}
	else if (s->sa_family == AF_IPX)
	{
		a->type = NA_IPX;
		memcpy(&a->ipx[0], ((struct sockaddr_ipx *)s)->sa_netnum, 4);
		memcpy(&a->ipx[4], ((struct sockaddr_ipx *)s)->sa_nodenum, 6);
		a->port = ((struct sockaddr_ipx *)s)->sa_socket;
	}
}


/*
===================
NET_CompareAdr
===================
*/
qboolean	NET_CompareAdr (netadr_t a, netadr_t b)
{
	if (a.type != b.type)
		return false;

	if (a.type == NA_LOOPBACK)
		return true;

	if (a.type == NA_IP)
	{
		if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port)
			return true;
		return false;
	}

	if (a.type == NA_IPX)
	{
		if ((memcmp(a.ipx, b.ipx, 10) == 0) && a.port == b.port)
			return true;
		return false;
	}

	// Knightmare- warning fix from mxd
	Com_Printf(S_COLOR_YELLOW"NET_CompareAdr: bad address type: %i\n", a.type);
	return false;
}

/*
===================
NET_CompareBaseAdr

Compares without the port
===================
*/
qboolean	NET_CompareBaseAdr (netadr_t a, netadr_t b)
{
	if (a.type != b.type)
		return false;

	if (a.type == NA_LOOPBACK)
		return TRUE;

	if (a.type == NA_IP)
	{
		if (a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3])
			return true;
		return false;
	}

	if (a.type == NA_IPX)
	{
		if ((memcmp(a.ipx, b.ipx, 10) == 0))
			return true;
		return false;
	}

	// Knightmare- warning fix from mxd
	Com_Printf(S_COLOR_YELLOW"NET_CompareBaseAdr: bad address type: %i\n", a.type);
	return false;
}

/*
===================
NET_AdrToString
===================
*/
char	*NET_AdrToString (netadr_t a)
{
	static	char	s[64];

	if (a.type == NA_LOOPBACK)
		Com_sprintf (s, sizeof(s), "loopback");
	else if (a.type == NA_IP)
		Com_sprintf (s, sizeof(s), "%i.%i.%i.%i:%i", a.ip[0], a.ip[1], a.ip[2], a.ip[3], ntohs(a.port));
	else
		Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x:%02x%02x%02x%02x%02x%02x:%i", a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9], ntohs(a.port));

	return s;
}


/*
=============
NET_StringToAdr

localhost
idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
#define DO(src,dest)	\
	copy[0] = s[src];	\
	copy[1] = s[src + 1];	\
	sscanf (copy, "%x", &val);	\
	((struct sockaddr_ipx *)sadr)->dest = val

qboolean	NET_StringToSockaddr (char *s, struct sockaddr *sadr)
{
	struct hostent	*h;
	char	*colon;
	int		val;
	char	copy[128];
	
	memset (sadr, 0, sizeof(*sadr));

	if ((strlen(s) >= 23) && (s[8] == ':') && (s[21] == ':'))	// check for an IPX address
	{
		((struct sockaddr_ipx *)sadr)->sa_family = AF_IPX;
		copy[2] = 0;
		DO(0, sa_netnum[0]);
		DO(2, sa_netnum[1]);
		DO(4, sa_netnum[2]);
		DO(6, sa_netnum[3]);
		DO(9, sa_nodenum[0]);
		DO(11, sa_nodenum[1]);
		DO(13, sa_nodenum[2]);
		DO(15, sa_nodenum[3]);
		DO(17, sa_nodenum[4]);
		DO(19, sa_nodenum[5]);
	//	sscanf (&s[22], "%u", &val);
		if (sscanf (&s[22], "%u", &val) == EOF) {
			Com_Printf (S_COLOR_YELLOW"NET_StringToSockaddr: invalid IPX address '%s'.\n", s);
		}
		((struct sockaddr_ipx *)sadr)->sa_socket = htons((unsigned short)val);
	}
	else
	{
		((struct sockaddr_in *)sadr)->sin_family = AF_INET;
		
		((struct sockaddr_in *)sadr)->sin_port = 0;

	//	strncpy (copy, s);
		Q_strncpyz (copy, sizeof(copy), s);
		// strip off a trailing :port if present
		for (colon = copy ; *colon ; colon++)
			if (*colon == ':')
			{
				*colon = 0;
				((struct sockaddr_in *)sadr)->sin_port = htons((short)atoi(colon+1));	
			}
		
		if (copy[0] >= '0' && copy[0] <= '9')
		{
			*(int *)&((struct sockaddr_in *)sadr)->sin_addr = inet_addr(copy);
		}
		else
		{
			if (! (h = gethostbyname(copy)) )
				return 0;
			*(int *)&((struct sockaddr_in *)sadr)->sin_addr = *(int *)h->h_addr_list[0];
		}
	}
	
	return true;
}

#undef DO

/*
=============
NET_StringToAdr

localhost
idnewt
idnewt:28000
192.246.40.70
192.246.40.70:28000
=============
*/
qboolean	NET_StringToAdr (char *s, netadr_t *a)
{
	struct sockaddr sadr;
	
	if (!strcmp (s, "localhost"))
	{
		memset (a, 0, sizeof(*a));
		a->type = NA_LOOPBACK;
		return true;
	}

	if (!NET_StringToSockaddr (s, &sadr))
		return false;
	
	SockadrToNetadr (&sadr, a);

	return true;
}


qboolean	NET_IsLocalAddress (netadr_t adr)
{
	return adr.type == NA_LOOPBACK;
}

/*
=============================================================================

LOOPBACK BUFFERS FOR LOCAL PLAYER

=============================================================================
*/

qboolean	NET_GetLoopPacket (netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
	int		i;
	loopback_t	*loop;

	loop = &loopbacks[sock];

	if (loop->send - loop->get > MAX_LOOPBACK)
		loop->get = loop->send - MAX_LOOPBACK;

	if (loop->get >= loop->send)
		return false;

	i = loop->get & (MAX_LOOPBACK-1);
	loop->get++;

	memcpy (net_message->data, loop->msgs[i].data, loop->msgs[i].datalen);
	net_message->cursize = loop->msgs[i].datalen;
	memset (net_from, 0, sizeof(*net_from));
	net_from->type = NA_LOOPBACK;
	return true;

}


void NET_SendLoopPacket (netsrc_t sock, int length, void *data, netadr_t to)
{
	int		i;
	loopback_t	*loop;

	loop = &loopbacks[sock^1];

	i = loop->send & (MAX_LOOPBACK-1);
	loop->send++;

	memcpy (loop->msgs[i].data, data, length);
	loop->msgs[i].datalen = length;
}

//=============================================================================
void SV_DropClientFromAdr (netadr_t address);

qboolean NET_GetPacket (netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message)
{
	int 	ret;
	struct	sockaddr from;
	int		fromlen;
	int		net_socket;
	int		protocol;
	int		err;

	if (NET_GetLoopPacket (sock, net_from, net_message))
		return true;

	for (protocol = 0 ; protocol < 2 ; protocol++)
	{
		if (protocol == 0)
			net_socket = ip_sockets[sock];
		else
			net_socket = ipx_sockets[sock];

		if (!net_socket)
			continue;

		fromlen = sizeof(from);
		ret = recvfrom (net_socket, net_message->data, net_message->maxsize
			, 0, (struct sockaddr *)&from, &fromlen);

		SockadrToNetadr (&from, net_from);

		if (ret == -1)
		{
			err = WSAGetLastError();
 
			if (err == WSAEWOULDBLOCK)
				continue;
			if (err == WSAEMSGSIZE)
			{
				Com_Printf (S_COLOR_YELLOW"Warning:  Oversize packet from %s\n",
						NET_AdrToString(*net_from));
				continue;
			}
			// Knightmare- added Jitspoe's fix for WSAECONNRESET bomb-out
			if (err == WSAECONNRESET)
			{
				Com_Printf ("NET_GetPacket: %s from %s\n", NET_ErrorString(),
					NET_AdrToString(*net_from));
				// drop this client to not flood the console with above message
				SV_DropClientFromAdr (*net_from);
				continue;
			}
			//if (dedicated->integer)	// let dedicated servers continue after errors
			// let servers continue after errors
				Com_Printf ("NET_GetPacket: %s from %s\n", NET_ErrorString(),
					NET_AdrToString(*net_from));
			//else
			//	Com_Error (ERR_DROP, "NET_GetPacket: %s from %s", 
			//		NET_ErrorString(), NET_AdrToString(*net_from));
			continue;
		}

		if (ret == net_message->maxsize)
		{
			Com_Printf ("Oversize packet from %s\n", NET_AdrToString (*net_from));
			continue;
		}

		net_message->cursize = ret;
		return true;
	}

	return false;
}

//=============================================================================

void NET_SendPacket (netsrc_t sock, int length, void *data, netadr_t to)
{
	int		ret = 0;
	struct sockaddr	addr;
	int		net_socket = 0;

	if ( to.type == NA_LOOPBACK )
	{
		NET_SendLoopPacket (sock, length, data, to);
		return;
	}

	if (to.type == NA_BROADCAST)
	{
		net_socket = ip_sockets[sock];
		if (!net_socket)
			return;
	}
	else if (to.type == NA_IP)
	{
		net_socket = ip_sockets[sock];
		if (!net_socket)
			return;
	}
	else if (to.type == NA_IPX)
	{
		net_socket = ipx_sockets[sock];
		if (!net_socket)
			return;
	}
	else if (to.type == NA_BROADCAST_IPX)
	{
		net_socket = ipx_sockets[sock];
		if (!net_socket)
			return;
	}
	else
		Com_Error (ERR_FATAL, "NET_SendPacket: bad address type");

	NetadrToSockadr (&to, &addr);

	ret = sendto (net_socket, data, length, 0, &addr, sizeof(addr) );
	if (ret == -1)
	{
		int err = WSAGetLastError();

		// wouldblock is silent
		if (err == WSAEWOULDBLOCK)
			return;

		// some PPP links dont allow broadcasts
		// NO ERROR fix for pinging servers w/ unplugged LAN cable
	//	if (((err == WSAEADDRNOTAVAIL) || !strcmp(NET_ErrorString(), "NO ERROR"))
		// Pat- WSAEHOSTUNREACH, this can occur if there is no network, or unplug ?
		if (((err == WSAEADDRNOTAVAIL) || err == WSAEHOSTUNREACH)
			&& ((to.type == NA_BROADCAST) || (to.type == NA_BROADCAST_IPX)))
			return;

	//	if (dedicated->value)	// let dedicated servers continue after errors
		if (dedicated->integer)	// let dedicated servers continue after errors
		{
			Com_Printf ("NET_SendPacket ERROR: %s to %s\n", NET_ErrorString(),
				NET_AdrToString (to));
		}
		else
		{
			if (err == WSAEADDRNOTAVAIL)
			{
				Com_DPrintf (S_COLOR_YELLOW"NET_SendPacket Warning: %s : %s\n", 
						NET_ErrorString(), NET_AdrToString (to));
			}
			else
			{
				Com_Error (ERR_DROP, "NET_SendPacket ERROR: %s to %s\n", 
						NET_ErrorString(), NET_AdrToString (to));
			}
		}
	}
}


//=============================================================================


/*
====================
NET_Socket
====================
*/
int NET_IPSocket (char *net_interface, int port)
{
	int					newsocket;
	struct sockaddr_in	address;
//	qboolean			_true = true;
	int					_true = 1;	// Knightmare- fix warning, was qboolean
	int					i = 1;
	int					err;

	if ((newsocket = socket (PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		err = WSAGetLastError();
		if (err != WSAEAFNOSUPPORT)
			Com_Printf (S_COLOR_YELLOW"WARNING: UDP_OpenSocket: socket: %s", NET_ErrorString());
		return 0;
	}

	// make it non-blocking
	if (ioctlsocket (newsocket, FIONBIO, &_true) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: UDP_OpenSocket: ioctl FIONBIO: %s\n", NET_ErrorString());
		return 0;
	}

	// make it broadcast capable
	if (setsockopt(newsocket, SOL_SOCKET, SO_BROADCAST, (char *)&i, sizeof(i)) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: UDP_OpenSocket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString());
		return 0;
	}

	if (!net_interface || !net_interface[0] || !stricmp(net_interface, "localhost"))
		address.sin_addr.s_addr = INADDR_ANY;
	else
		NET_StringToSockaddr (net_interface, (struct sockaddr *)&address);

	if (port == PORT_ANY)
		address.sin_port = 0;
	else
		address.sin_port = htons((short)port);

	address.sin_family = AF_INET;

	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: UDP_OpenSocket: bind: %s\n", NET_ErrorString());
		closesocket (newsocket);
		return 0;
	}

	return newsocket;
}


/*
====================
NET_OpenIP
====================
*/
void NET_OpenIP (void)
{
	cvar_t	*ip;
	int		port;
	int		dedicated;

	ip = Cvar_Get ("ip", "localhost", CVAR_NOSET);

	dedicated = Cvar_VariableValue ("dedicated");

	if (!ip_sockets[NS_SERVER])
	{
		port = Cvar_Get("ip_hostport", "0", CVAR_NOSET)->value;
		if (!port)
		{
			port = Cvar_Get("hostport", "0", CVAR_NOSET)->value;
			if (!port)
			{
				port = Cvar_Get("port", va("%i", PORT_SERVER), CVAR_NOSET)->value;
			}
		}
		ip_sockets[NS_SERVER] = NET_IPSocket (ip->string, port);
		if (!ip_sockets[NS_SERVER] && dedicated)
			Com_Error (ERR_FATAL, "Couldn't allocate dedicated server IP port");
	}


	// dedicated servers don't need client ports
	if (dedicated)
		return;

	if (!ip_sockets[NS_CLIENT])
	{
		port = Cvar_Get("ip_clientport", "0", CVAR_NOSET)->value;
		if (!port)
		{
			port = Cvar_Get("clientport", va("%i", PORT_CLIENT), CVAR_NOSET)->value;
			if (!port)
				port = PORT_ANY;
		}
		ip_sockets[NS_CLIENT] = NET_IPSocket (ip->string, port);
		if (!ip_sockets[NS_CLIENT])
			ip_sockets[NS_CLIENT] = NET_IPSocket (ip->string, PORT_ANY);
	}
}


/*
====================
IPX_Socket
====================
*/
int NET_IPXSocket (int port)
{
	int					newsocket;
	struct sockaddr_ipx	address;
	int					_true = 1;
	int					err;

	if ((newsocket = socket (PF_IPX, SOCK_DGRAM, NSPROTO_IPX)) == -1)
	{
		err = WSAGetLastError();
		if (err != WSAEAFNOSUPPORT)
			Com_Printf (S_COLOR_YELLOW"WARNING: IPX_Socket: socket: %s\n", NET_ErrorString());
		return 0;
	}

	// make it non-blocking
	if (ioctlsocket (newsocket, FIONBIO, &_true) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: IPX_Socket: ioctl FIONBIO: %s\n", NET_ErrorString());
		return 0;
	}

	// make it broadcast capable
	if (setsockopt(newsocket, SOL_SOCKET, SO_BROADCAST, (char *)&_true, sizeof(_true)) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: IPX_Socket: setsockopt SO_BROADCAST: %s\n", NET_ErrorString());
		return 0;
	}

	address.sa_family = AF_IPX;
	memset (address.sa_netnum, 0, 4);
	memset (address.sa_nodenum, 0, 6);
	if (port == PORT_ANY)
		address.sa_socket = 0;
	else
		address.sa_socket = htons((short)port);

	if( bind (newsocket, (void *)&address, sizeof(address)) == -1)
	{
		Com_Printf (S_COLOR_YELLOW"WARNING: IPX_Socket: bind: %s\n", NET_ErrorString());
		closesocket (newsocket);
		return 0;
	}

	return newsocket;
}


/*
====================
NET_OpenIPX
====================
*/
void NET_OpenIPX (void)
{
	int		port;
	int		dedicated;

	dedicated = Cvar_VariableValue ("dedicated");

	if (!ipx_sockets[NS_SERVER])
	{
		port = Cvar_Get("ipx_hostport", "0", CVAR_NOSET)->value;
		if (!port)
		{
			port = Cvar_Get("hostport", "0", CVAR_NOSET)->value;
			if (!port)
			{
				port = Cvar_Get("port", va("%i", PORT_SERVER), CVAR_NOSET)->value;
			}
		}
		ipx_sockets[NS_SERVER] = NET_IPXSocket (port);
	}

	// dedicated servers don't need client ports
	if (dedicated)
		return;

	if (!ipx_sockets[NS_CLIENT])
	{
		port = Cvar_Get("ipx_clientport", "0", CVAR_NOSET)->value;
		if (!port)
		{
			port = Cvar_Get("clientport", va("%i", PORT_CLIENT), CVAR_NOSET)->value;
			if (!port)
				port = PORT_ANY;
		}
		ipx_sockets[NS_CLIENT] = NET_IPXSocket (port);
		if (!ipx_sockets[NS_CLIENT])
			ipx_sockets[NS_CLIENT] = NET_IPXSocket (PORT_ANY);
	}
}


/*
====================
NET_Config

A single player game will only use the loopback code
====================
*/
void	NET_Config (qboolean multiplayer)
{
	int		i;
	static	qboolean	old_config;

	if (old_config == multiplayer)
		return;

	old_config = multiplayer;

	if (!multiplayer)
	{	// shut down any existing sockets
		for (i=0 ; i<2 ; i++)
		{
			if (ip_sockets[i])
			{
				closesocket (ip_sockets[i]);
				ip_sockets[i] = 0;
			}
			if (ipx_sockets[i])
			{
				closesocket (ipx_sockets[i]);
				ipx_sockets[i] = 0;
			}
		}
	}
	else
	{	// open sockets
	//	if (! noudp->value)
		if (! noudp->integer)
			NET_OpenIP ();
	//	if (! noipx->value)
		if (! noipx->integer)
			NET_OpenIPX ();
	}
}

// sleeps msec or until net socket is ready
void NET_Sleep (int msec)
{
    struct timeval timeout;
	fd_set	fdset;
	extern cvar_t *dedicated;
	int i;

//	if (!dedicated || !dedicated->value)
	if (!dedicated || !dedicated->integer)
		return; // we're not a server, just run full speed

	FD_ZERO(&fdset);
	i = 0;
	if (ip_sockets[NS_SERVER]) {
		FD_SET(ip_sockets[NS_SERVER], &fdset); // network socket
		i = ip_sockets[NS_SERVER];
	}
	if (ipx_sockets[NS_SERVER]) {
		FD_SET(ipx_sockets[NS_SERVER], &fdset); // network socket
		if (ipx_sockets[NS_SERVER] > i)
			i = ipx_sockets[NS_SERVER];
	}
	timeout.tv_sec = msec/1000;
	timeout.tv_usec = (msec%1000)*1000;
	select(i+1, &fdset, NULL, NULL, &timeout);
}

//===================================================================


static WSADATA		winsockdata;

/*
====================
NET_Init
====================
*/
void NET_Init (void)
{
	WORD	wVersionRequested; 
	int		r;

	wVersionRequested = MAKEWORD(1, 1); 

	r = WSAStartup (MAKEWORD(1, 1), &winsockdata);

	if (r)
		Com_Error (ERR_FATAL,"Winsock initialization failed.");

	Com_Printf("Winsock Initialized\n");

	noudp = Cvar_Get ("noudp", "0", CVAR_NOSET);
	Cvar_SetDescription ("noudp", "Disables use of UDP protocol when set to 1.");
	noipx = Cvar_Get ("noipx", "0", CVAR_NOSET);
	Cvar_SetDescription ("noipx", "Disables use if IPX protocol when set to 1.");

//	net_shownet = Cvar_Get ("net_shownet", "0", 0);	// unused
}


/*
====================
NET_Shutdown
====================
*/
void	NET_Shutdown (void)
{
	NET_Config (false);	// close sockets

	WSACleanup ();
}


/*
====================
NET_ErrorString
====================
*/
char *NET_ErrorString (void)
{
	int		code;
	static char errstr[64];

	code = WSAGetLastError ();
	switch (code)
	{
#if 1	// Jitspoe's more complete error codes
	case WSAEINTR: return "WSAEINTR";
	case WSAEBADF: return "WSAEBADF";
	case WSAEACCES: return "WSAEACCES";
	case WSAEFAULT: return "WSAEFAULT";
	case WSAEINVAL: return "WSAEINVAL";
	case WSAEMFILE: return "WSAEMFILE";
	case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
	case WSAEINPROGRESS: return "WSAEINPROGRESS";
	case WSAEALREADY: return "WSAEALREADY";
	case WSAENOTSOCK: return "WSAENOTSOCK";
	case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
	case WSAEMSGSIZE: return "WSAEMSGSIZE";
	case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
	case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
	case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
	case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
	case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
	case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
	case WSAEADDRINUSE: return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN: return "WSAENETDOWN";
	case WSAENETUNREACH: return "WSAENETUNREACH";
	case WSAENETRESET: return "WSAENETRESET";
	case WSAECONNABORTED: return "WSAECONNABORTED";
	case WSAECONNRESET: return "WSAECONNRESET";
	case WSAENOBUFS: return "WSAENOBUFS";
	case WSAEISCONN: return "WSAEISCONN";
	case WSAENOTCONN: return "WSAENOTCONN";
	case WSAESHUTDOWN: return "WSAESHUTDOWN";
	case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
	case WSAETIMEDOUT: return "WSAETIMEDOUT";
	case WSAECONNREFUSED: return "WSAECONNREFUSED";
	case WSAELOOP: return "WSAELOOP";
	case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
	case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
	case WSAEHOSTUNREACH: return "WSAEHOSTUNREACH";
	case WSAENOTEMPTY: return "WSAENOTEMPTY";
	case WSAEPROCLIM: return "WSAEPROCLIM";
	case WSAEUSERS: return "WSAEUSERS";
	case WSAEDQUOT: return "WSAEDQUOT";
	case WSAESTALE: return "WSAESTALE";
	case WSAEREMOTE: return "WSAEREMOTE";
	case WSASYSNOTREADY: return "WSASYSNOTREADY";
	case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
	case WSANOTINITIALISED: return "WSANOTINITIALISED";
	case WSAEDISCON: return "WSAEDISCON";
	case WSAENOMORE: return "WSAENOMORE";
	case WSAECANCELLED: return "WSAECANCELLED";
	case WSAEINVALIDPROCTABLE: return "WSAEINVALIDPROCTABLE";
	case WSAEINVALIDPROVIDER: return "WSAEINVALIDPROVIDER";
	case WSAEPROVIDERFAILEDINIT: return "WSAEPROVIDERFAILEDINIT";
	case WSASYSCALLFAILURE: return "WSASYSCALLFAILURE";
	case WSASERVICE_NOT_FOUND: return "WSASERVICE_NOT_FOUND";
	case WSATYPE_NOT_FOUND: return "WSATYPE_NOT_FOUND";
	case WSA_E_NO_MORE: return "WSA_E_NO_MORE";
	case WSA_E_CANCELLED: return "WSA_E_CANCELLED";
	case WSAEREFUSED: return "WSAEREFUSED";
	case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
	case WSATRY_AGAIN: return "WSATRY_AGAIN";
	case WSANO_RECOVERY: return "WSANO_RECOVERY";
	case WSANO_DATA: return "WSANO_DATA"; 
	case WSA_QOS_RECEIVERS: return "WSA_QOS_RECEIVERS";
	case WSA_QOS_SENDERS: return "WSA_QOS_SENDERS";
	case WSA_QOS_NO_SENDERS: return "WSA_QOS_NO_SENDERS";
	case WSA_QOS_NO_RECEIVERS: return "WSA_QOS_NO_RECEIVERS";
	case WSA_QOS_REQUEST_CONFIRMED: return "WSA_QOS_REQUEST_CONFIRMED";
	case WSA_QOS_ADMISSION_FAILURE: return "WSA_QOS_ADMISSION_FAILURE";
	case WSA_QOS_POLICY_FAILURE: return "WSA_QOS_POLICY_FAILURE";
	case WSA_QOS_BAD_STYLE: return "WSA_QOS_BAD_STYLE";
	case WSA_QOS_BAD_OBJECT: return "WSA_QOS_BAD_OBJECT";
	case WSA_QOS_TRAFFIC_CTRL_ERROR: return "WSA_QOS_TRAFFIC_CTRL_ERROR";
	case WSA_QOS_GENERIC_ERROR: return "WSA_QOS_GENERIC_ERROR";
	case WSA_QOS_ESERVICETYPE: return "WSA_QOS_ESERVICETYPE";
	case WSA_QOS_EFLOWSPEC: return "WSA_QOS_EFLOWSPEC";
	case WSA_QOS_EPROVSPECBUF: return "WSA_QOS_EPROVSPECBUF";
	case WSA_QOS_EFILTERSTYLE: return "WSA_QOS_EFILTERSTYLE";
	case WSA_QOS_EFILTERTYPE: return "WSA_QOS_EFILTERTYPE";
	case WSA_QOS_EFILTERCOUNT: return "WSA_QOS_EFILTERCOUNT";
	case WSA_QOS_EOBJLENGTH: return "WSA_QOS_EOBJLENGTH";
	case WSA_QOS_EFLOWCOUNT: return "WSA_QOS_EFLOWCOUNT";
	case WSA_QOS_EUNKOWNPSOBJ: return "WSA_QOS_EUNKOWNPSOBJ";
	case WSA_QOS_EPOLICYOBJ: return "WSA_QOS_EPOLICYOBJ";
	case WSA_QOS_EFLOWDESC: return "WSA_QOS_EFLOWDESC";
	case WSA_QOS_EPSFLOWSPEC: return "WSA_QOS_EPSFLOWSPEC";
	case WSA_QOS_EPSFILTERSPEC: return "WSA_QOS_EPSFILTERSPEC";
	case WSA_QOS_ESDMODEOBJ: return "WSA_QOS_ESDMODEOBJ";
	case WSA_QOS_ESHAPERATEOBJ: return "WSA_QOS_ESHAPERATEOBJ";
	case WSA_QOS_RESERVED_PETYPE: return "WSA_QOS_RESERVED_PETYPE";
	default:
		Com_sprintf(errstr, sizeof(errstr), "Unhandled WSA code: %d", code);
		return errstr;

#else

	case WSAEINTR: return "WSAEINTR";
	case WSAEBADF: return "WSAEBADF";
	case WSAEACCES: return "WSAEACCES";
	case WSAEDISCON: return "WSAEDISCON";
	case WSAEFAULT: return "WSAEFAULT";
	case WSAEINVAL: return "WSAEINVAL";
	case WSAEMFILE: return "WSAEMFILE";
	case WSAEWOULDBLOCK: return "WSAEWOULDBLOCK";
	case WSAEINPROGRESS: return "WSAEINPROGRESS";
	case WSAEALREADY: return "WSAEALREADY";
	case WSAENOTSOCK: return "WSAENOTSOCK";
	case WSAEDESTADDRREQ: return "WSAEDESTADDRREQ";
	case WSAEMSGSIZE: return "WSAEMSGSIZE";
	case WSAEPROTOTYPE: return "WSAEPROTOTYPE";
	case WSAENOPROTOOPT: return "WSAENOPROTOOPT";
	case WSAEPROTONOSUPPORT: return "WSAEPROTONOSUPPORT";
	case WSAESOCKTNOSUPPORT: return "WSAESOCKTNOSUPPORT";
	case WSAEOPNOTSUPP: return "WSAEOPNOTSUPP";
	case WSAEPFNOSUPPORT: return "WSAEPFNOSUPPORT";
	case WSAEAFNOSUPPORT: return "WSAEAFNOSUPPORT";
	case WSAEADDRINUSE: return "WSAEADDRINUSE";
	case WSAEADDRNOTAVAIL: return "WSAEADDRNOTAVAIL";
	case WSAENETDOWN: return "WSAENETDOWN";
	case WSAENETUNREACH: return "WSAENETUNREACH";
	case WSAENETRESET: return "WSAENETRESET";
	case WSAECONNABORTED: return "WSWSAECONNABORTEDAEINTR";
	case WSAECONNRESET: return "WSAECONNRESET";
	case WSAENOBUFS: return "WSAENOBUFS";
	case WSAEISCONN: return "WSAEISCONN";
	case WSAENOTCONN: return "WSAENOTCONN";
	case WSAESHUTDOWN: return "WSAESHUTDOWN";
	case WSAETOOMANYREFS: return "WSAETOOMANYREFS";
	case WSAETIMEDOUT: return "WSAETIMEDOUT";
	case WSAECONNREFUSED: return "WSAECONNREFUSED";
	case WSAELOOP: return "WSAELOOP";
	case WSAENAMETOOLONG: return "WSAENAMETOOLONG";
	case WSAEHOSTDOWN: return "WSAEHOSTDOWN";
	case WSASYSNOTREADY: return "WSASYSNOTREADY";
	case WSAVERNOTSUPPORTED: return "WSAVERNOTSUPPORTED";
	case WSANOTINITIALISED: return "WSANOTINITIALISED";
	case WSAHOST_NOT_FOUND: return "WSAHOST_NOT_FOUND";
	case WSATRY_AGAIN: return "WSATRY_AGAIN";
	case WSANO_RECOVERY: return "WSANO_RECOVERY";
	case WSANO_DATA: return "WSANO_DATA";
	default: return "NO ERROR";
#endif
	}
}
