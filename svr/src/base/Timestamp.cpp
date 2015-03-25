#include "Timestamp.h"

#include <stdio.h>
#include "Logging.h"

namespace timeutil
{
#ifdef WIN32 
    Timestamp FileTimeToInt64(const FILETIME& time)  
    {  
        ULARGE_INTEGER tt;  
        tt.LowPart = time.dwLowDateTime;  
        tt.HighPart = time.dwHighDateTime;  
        return(tt.QuadPart);  
    }

    FILETIME Int64ToFileTime(const Timestamp& stamp)  
    {  
        ULARGE_INTEGER tt;
        tt.QuadPart = stamp;

        FILETIME filetime;
        filetime.dwLowDateTime = tt.LowPart;
        filetime.dwHighDateTime = tt.HighPart;

        return filetime;
    }

    Timestamp GetTimeOfDay()
    {
        FILETIME now;
        GetSystemTimeAsFileTime (&now); // 获得系统UTC格式时间

        int64 nowIn100ns = FileTimeToInt64(now);// 单位100ns
        return nowIn100ns / 10000; //转化为ms单位
    }

    Timestamp GetLocalTime()
    {
        FILETIME local;
        GetSystemTimeAsFileTime (&local); // 获得系统UTC格式时间
        FileTimeToLocalFileTime(&local, &local); // 转换为本地时间

        int64 localIn100ns = FileTimeToInt64(local);// 单位100ns
        return localIn100ns / 10000;
    }

    const char* FormatTime(const Timestamp& time)
    {
        Timestamp utc = time * 10000;

        FILETIME filetime = Int64ToFileTime(utc);

        SYSTEMTIME systime;
        FileTimeToSystemTime(&filetime, &systime);

        static char format[32] = {0};
        sprintf_s(format, sizeof format, "%02d/%02d/%d-%02d:%02d:%02d:%-6d",
            systime.wDay, systime.wMonth, systime.wYear,
            systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds
            );

        return format;
    }

    const char* FormatNow()
    {
        FILETIME local;
        SYSTEMTIME systime;
        GetSystemTimeAsFileTime (&local); // 获得系统UTC格式时间
        FileTimeToLocalFileTime(&local, &local); // 转换为本地时间
        FileTimeToSystemTime(&local, &systime);

        static char format[32] = {0};
        sprintf_s(format, sizeof format, "%02d/%02d/%d-%02d:%02d:%02d:%-6d",
            systime.wDay, systime.wMonth, systime.wYear,
            systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds
            );

        return format;
    }

    void EchoNow()
    {
        LOG_INFO << FormatNow();
    }

#else
    Timestamp GetTimeOfDay()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        Timestamp seconds = tv.tv_sec;
        return seconds * kMicroSecondsPerSecond + tv.tv_usec;
    }
#endif
}
