@ stdcall AcceptEx(long long ptr long long long ptr ptr)
@ stdcall EnumProtocolsA(ptr ptr ptr) ws2_32.WSAEnumProtocolsA
@ stdcall EnumProtocolsW(ptr ptr ptr) ws2_32.WSAEnumProtocolsW
@ stdcall GetAcceptExSockaddrs(ptr long long long ptr ptr ptr ptr)
@ stdcall GetAddressByNameA(long ptr ptr ptr long ptr ptr ptr ptr ptr)
@ stdcall GetAddressByNameW(long ptr ptr ptr long ptr ptr ptr ptr ptr)
@ stdcall GetNameByTypeA(ptr ptr long)
@ stdcall GetNameByTypeW(ptr ptr long)
@ stdcall GetServiceA(long ptr ptr long ptr ptr ptr)
@ stdcall GetServiceW(long ptr ptr long ptr ptr ptr)
@ stdcall GetTypeByNameA(ptr ptr)
@ stdcall GetTypeByNameW(ptr ptr)
@ stdcall MigrateWinsockConfiguration(long long long)
@ stdcall NPLoadNameSpaces(ptr ptr ptr)
@ stdcall NSPStartup(ptr ptr)
@ stdcall ServiceMain(long long)
@ stdcall SetServiceA(long long long ptr ptr ptr)
@ stdcall SetServiceW(long long long ptr ptr ptr)
@ stdcall StartWsdpService()
@ stdcall StopWsdpService()
@ stdcall SvchostPushServiceGlobals(long)
@ stdcall TransmitFile(long long long long ptr ptr long)
@ stdcall WSARecvEx(long ptr long ptr)
@ stdcall WSPStartup(long ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr)
@ stdcall dn_expand(ptr ptr ptr ptr long)
@ stdcall getnetbyname(ptr)
@ stdcall inet_network(ptr)
@ stdcall rcmd(ptr long ptr ptr ptr ptr)
@ stdcall rexec(ptr long ptr ptr ptr ptr)
@ stdcall rresvport(ptr)
@ stdcall s_perror(ptr)
@ stdcall sethostname(ptr long)
