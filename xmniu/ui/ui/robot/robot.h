#ifndef robot_h__
#define robot_h__

#include "types.h"

struct svr_t;



struct svr_t
{
    char *addr; // 服务器ip或者url:192.168.0.1或www.xxx.com
    int port; // 服务器端口列表

    unsigned long cast_addr;
};

class robot_t
{
public:
    void go(svr_t *svr);

    void end();

    int robot_connect(svr_t *svr);
    void on_connect_fail(svr_t *svr);

    bool robot_send(const char buf[], int n);
    void on_send_fail(const char buf[], int n);

    bool robot_recv(char buf[], int buf_len, ssize_t &n_recv);
    void on_recv_fail();

    // 记载任务失败原因
    void err_cache();

public:
    svr_t *m_svr;

    pthread_t m_t_id; // 所属线程id
    int m_idx;

    int m_sockfd;

    bool closed; // 是否已关闭
    int m_error_no; // 错误号

    time_in_ms m_start_conn_time; // 开始connect的时间
    time_in_ms m_conn_succ_time; // connect成功的事件，如不成功则为0
    time_in_ms m_finish_time; // robot一次任务成功执行后的事件

    time_in_ms m_error_occur_time; // 发生错误的时间
};

#endif // robot_h__
