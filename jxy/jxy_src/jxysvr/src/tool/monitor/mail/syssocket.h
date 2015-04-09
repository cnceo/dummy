/*==================================================================================================
 * 项目名称:网络模块
 *    功能:收发网络数据
 *    作者: 
 *    联系: 
 *    版本: v1.0.0
 ==================================================================================================*/
#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

/*********************************************************************************************
 *函   数   名：sysutil_activate_keepalive
 *功        能：发送保持活动包。
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_keepalive(int fd);

/*********************************************************************************************
 *函   数   名：sysutil_activate_reuseaddr
 *功        能：允许套接口和一个已在使用中的地址捆绑
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_reuseaddr(int fd);

/*********************************************************************************************
 *函   数   名：sysutil_set_nodelay
 *功        能：禁止发送合并的Nagle算法
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_set_nodelay(int fd);

/*********************************************************************************************
 *函   数   名：sysutil_activate_oobinline
 *功        能：在常规数据流中接收带外数据
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_oobinline(int fd);
/*********************************************************************************************
 *函   数   名：sysutil_set_iptos_throughput
 *功        能：在IP头中设置选项
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_set_iptos_throughput(int fd);
/*********************************************************************************************
 *函   数   名：sysutil_activate_linger
 *功        能：如关闭时有未发送数据，则逗留
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_linger(int fd);
/*********************************************************************************************
 *函   数   名：sysutil_deactivate_linger_failok
 *功        能：如关闭时有未发送数据，则逗留
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_deactivate_linger_failok(int fd);

/*********************************************************************************************
 *函   数   名：sysutil_activate_sigurg
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_sigurg(int fd);

/*********************************************************************************************
 *函   数   名：sysutil_activate_noblock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_activate_noblock(int fd);
/*********************************************************************************************
 *函   数   名：sysutil_deactivate_noblock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
void sysutil_deactivate_noblock(int fd);
/*********************************************************************************************
 *函   数   名：sysutil_recv_peek
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_recv_peek(const int fd, void* p_buf, unsigned int len);

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv4_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_get_ipv4_sock(void);

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv6_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_bind(int fd, const int port);


/*********************************************************************************************
 *函   数   sysutil_dns_resolve
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_dns_resolve(char *ip, const unsigned int max, const char * host);

/*********************************************************************************************
 *函   数   名：sysutil_get_ipv6_sock
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_listen(int fd, const unsigned int backlog);

/*********************************************************************************************
 *函   数   名：sysutil_accept_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_accept_timeout(int fd, struct sockaddr_in* p_sockaddr,
		unsigned int wait_seconds);

/*********************************************************************************************
 *函   数   名：sysutil_connect_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_connect_timeout(int fd, const char* ip, const unsigned int port,
		unsigned int wait_seconds);

/*********************************************************************************************
 *函   数   名：sysutil_read
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read(const int fd, void* p_buf, const unsigned int size);


/*********************************************************************************************
 *函   数   sysutil_read_timeout
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read_timeout(const int fd, void* p_buf, const unsigned int size,
		unsigned int wait_seconds);

/*********************************************************************************************
 *函   数   名：sysutil_write
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_write(const int fd, const void* p_buf, const unsigned int size);

/*********************************************************************************************
 *函   数   名：sysutil_read_loop
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_read_loop(const int fd, void* p_buf, unsigned int size);

/*********************************************************************************************
 *函   数   名：sysutil_write_loop
 *功        能：
 *输        入：
 *
 *输        出：
 *返        回：RT_OK：成功，RT_NG：失败
 *修 改  记 录：NONE
 *********************************************************************************************/
int sysutil_write_loop(const int fd, const void* p_buf, unsigned int size);



#endif /* TCPSOCKET_H_ */
