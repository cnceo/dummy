#ifndef _ThreadPool_H_
#define _ThreadPool_H_
#pragma warning(disable: 4530)
#pragma warning(disable: 4786)
#include <cassert>
#include <vector>
#include <queue>
#include <windows.h>

using namespace std;

typedef unsigned (__stdcall *ThreadCb)(void *);

class ThreadJob  //��������
{
public:
    //���̳߳ص��õ��麯��
    virtual void DoJob(void *pPara) = 0;
};

class ThreadPool
{
public:
    //dwNum �̳߳ع�ģ
    ThreadPool(DWORD dwNum = 4) : _lThreadNum(0), _lRunningNum(0) 
    {
        InitializeCriticalSection(&_csThreadVector);
        InitializeCriticalSection(&_csWorkQueue);
        _EventComplete = CreateEvent(0, false, false, NULL);
        _EventEnd = CreateEvent(0, true, false, NULL);
        _SemaphoreCall = CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
        _SemaphoreDel =  CreateSemaphore(0, 0,  0x7FFFFFFF, NULL);
        assert(_SemaphoreCall != INVALID_HANDLE_VALUE);
        assert(_EventComplete != INVALID_HANDLE_VALUE);
        assert(_EventEnd != INVALID_HANDLE_VALUE);
        assert(_SemaphoreDel != INVALID_HANDLE_VALUE);
        AdjustSize(dwNum <= 0 ? 4 : dwNum);
    }
    ~ThreadPool()
    {
        DeleteCriticalSection(&_csWorkQueue);
        CloseHandle(_EventEnd);
        CloseHandle(_EventComplete);
        CloseHandle(_SemaphoreCall);
        CloseHandle(_SemaphoreDel);
        
        for(vector<ThreadItem*>::iterator iter = _ThreadVector.begin(); iter != _ThreadVector.end(); iter++)
        {
            ThreadItem *item = *iter;
            delete item;
            // if(*iter)
               // delete *iter;
        }
        DeleteCriticalSection(&_csThreadVector);
    }
    //�����̳߳ع�ģ
    int AdjustSize(int iNum)
    {
        if(iNum > 0)
        {
            ThreadItem *pNew;
            EnterCriticalSection(&_csThreadVector);
            for(int _i=0; _i<iNum; _i++)
            {
                _ThreadVector.push_back(pNew = new ThreadItem(this)); 
                assert(pNew);

                pNew->_Handle = (HANDLE)_beginthreadex(NULL, 0, DefaultJobProc, pNew, 0, NULL);
                // set priority
                SetThreadPriority(pNew->_Handle, THREAD_PRIORITY_BELOW_NORMAL);
                assert(pNew->_Handle);
            }
            LeaveCriticalSection(&_csThreadVector);
        }
        else
        {
            iNum *= -1;
            ReleaseSemaphore(_SemaphoreDel,  iNum > _lThreadNum ? _lThreadNum : iNum, NULL);
        }
        return (int)_lThreadNum;
    }
    //�����̳߳�
    void Call(ThreadCb pFunc, void *pPara = NULL)
    {
        assert(pFunc);
        EnterCriticalSection(&_csWorkQueue);
        _JobQueue.push(new JobItem(pFunc, pPara));
        LeaveCriticalSection(&_csWorkQueue);
        ReleaseSemaphore(_SemaphoreCall, 1, NULL);
    }
    //�����̳߳�
    inline void Call(ThreadJob * p, void *pPara = NULL)
    {
        Call(CallProc, new CallProcPara(p, pPara));
    }
    //�����̳߳�, ��ͬ���ȴ�
    bool EndAndWait(DWORD dwWaitTime = INFINITE)
    {
        SetEvent(_EventEnd);
        return WaitForSingleObject(_EventComplete, dwWaitTime) == WAIT_OBJECT_0;
    }
    //�����̳߳�
    inline void End()
    {
        SetEvent(_EventEnd);
    }
    inline int Size()
    {
        return (int)_lThreadNum;
    }
    inline DWORD GetRunningSize()
    {
        return (DWORD)_lRunningNum;
    }
    bool IsRunning()
    {
        return _lRunningNum > 0;
    }
protected:
    //�����߳�
    static unsigned __stdcall DefaultJobProc(LPVOID lpParameter = NULL)
    {
        ThreadItem *pThread = static_cast<ThreadItem*>(lpParameter);
        assert(pThread);
        ThreadPool *pThreadPoolObj = pThread->_pThis;
        assert(pThreadPoolObj);
        InterlockedIncrement(&pThreadPoolObj->_lThreadNum);
        HANDLE hWaitHandle[3];
        hWaitHandle[0] = pThreadPoolObj->_SemaphoreCall;
        hWaitHandle[1] = pThreadPoolObj->_SemaphoreDel;
        hWaitHandle[2] = pThreadPoolObj->_EventEnd;
        JobItem *pJob;
        bool fHasJob;
        for(;;)
        {
            DWORD wr = WaitForMultipleObjects(3, hWaitHandle, false, INFINITE);
            //��Ӧɾ���߳��ź�
            if(wr == WAIT_OBJECT_0 + 1)  
                break;
            //�Ӷ�����ȡ���û���ҵ
            EnterCriticalSection(&pThreadPoolObj->_csWorkQueue);
            if(fHasJob = !pThreadPoolObj->_JobQueue.empty())
            {
                pJob = pThreadPoolObj->_JobQueue.front();
                pThreadPoolObj->_JobQueue.pop();
                assert(pJob);
            }
            LeaveCriticalSection(&pThreadPoolObj->_csWorkQueue);
            //�ܵ������߳��ź� ȷ���Ƿ�����߳�(�����߳��ź� && �Ƿ��й���)
            if(wr == WAIT_OBJECT_0 + 2 && !fHasJob)  
                break;
            if(fHasJob && pJob)
            {
                InterlockedIncrement(&pThreadPoolObj->_lRunningNum);
                pThread->_dwLastBeginTime = GetTickCount();
                pThread->_dwCount++;
                pThread->_fIsRunning = true;
                pJob->_pFunc(pJob->_pPara); //�����û���ҵ
                delete pJob; 
                pThread->_fIsRunning = false;
                InterlockedDecrement(&pThreadPoolObj->_lRunningNum);
            }
        }
        //ɾ�������ṹ
        EnterCriticalSection(&pThreadPoolObj->_csThreadVector);
        pThreadPoolObj->_ThreadVector.erase(find(pThreadPoolObj->_ThreadVector.begin(), pThreadPoolObj->_ThreadVector.end(), pThread));
        LeaveCriticalSection(&pThreadPoolObj->_csThreadVector);
        delete pThread;
        InterlockedDecrement(&pThreadPoolObj->_lThreadNum);
        if(!pThreadPoolObj->_lThreadNum)  //�����߳̽���
            SetEvent(pThreadPoolObj->_EventComplete);
        return 0;
    }
    //�����û������麯��
    static unsigned __stdcall CallProc(void *pPara) 
    {
        CallProcPara *cp = static_cast<CallProcPara *>(pPara);
        assert(cp);
        if(cp)
        {
            cp->_pObj->DoJob(cp->_pPara);
            delete cp;
        }

        return 0;
    }
    //�û�����ṹ
    struct CallProcPara  
    {
        ThreadJob* _pObj;//�û����� 
        void *_pPara;//�û�����
        CallProcPara(ThreadJob* p, void *pPara) : _pObj(p), _pPara(pPara) { };
    };
    //�û������ṹ
    struct JobItem 
    {
        ThreadCb _pFunc;
        // unsigned (*_pFunc)(void  *);//����
        void *_pPara; //����
        JobItem(ThreadCb pFunc = NULL, void *pPara = NULL) : _pFunc(pFunc), _pPara(pPara) { };
    };
    //�̳߳��е��߳̽ṹ
    struct ThreadItem
    {
        HANDLE _Handle; //�߳̾��
        ThreadPool *_pThis;  //�̳߳ص�ָ��
        DWORD _dwLastBeginTime; //���һ�����п�ʼʱ��
        DWORD _dwCount; //���д���
        bool _fIsRunning;
        ThreadItem(ThreadPool *pthis) : _pThis(pthis), _Handle(NULL), _dwLastBeginTime(0), _dwCount(0), _fIsRunning(false) { };
        ~ThreadItem()
        {
            if(_Handle)
            {
                CloseHandle(_Handle);
                _Handle = NULL;
            }
        }
    };
    std::queue<JobItem *> _JobQueue;  //��������
    std::vector<ThreadItem *>  _ThreadVector; //�߳�����
    CRITICAL_SECTION _csThreadVector, _csWorkQueue; //���������ٽ�, �߳������ٽ�
    HANDLE _EventEnd, _EventComplete, _SemaphoreCall, _SemaphoreDel;//����֪ͨ, ����¼�, �����źţ� ɾ���߳��ź�
    long _lThreadNum, _lRunningNum; //�߳���, ���е��߳���
};
#endif //_ThreadPool_H_