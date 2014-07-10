/**
 * @file   Async.cpp
 * @author Bastien Brunnenstein
 */

#include <Global.hpp>

#include <resources/Async.hpp>

#include <system/exceptions/ResourceError.hpp>

#include <condition_variable>
#include <mutex>
#include <thread>

namespace
{
    typedef std::thread             Thread;
    typedef std::mutex              Mutex;
    typedef std::condition_variable ConditionVariable;
    typedef std::lock_guard<Mutex>  Lock;
    typedef std::unique_lock<Mutex> UniqueLock;

    unsigned const WORKERS_DEFAULT  = 3;
    unsigned const WORKERS_MIN      = 2;
    unsigned const WORKERS_MAX      = 7;

    unsigned const JOBS_MAX         = 128;

    using typename res::async::AsyncFunction;

    class Job
    {
    public:
        Job *           next;
        AsyncFunction   function;
        void *          param;

    };
    // class Job

    bool                threading_enabled;

    IWBAN_DEBUG(bool    d_ready = false);

    Thread *            worker_pool[WORKERS_MAX];

    // Set to true to kill all the workers
    volatile bool       kill_workers;

    Mutex               jobs_mutex;

    ConditionVariable   jobs_condition;

    Job                 job_pool[JOBS_MAX];

    Job *               first_job;

    Job *               last_job;

    Job *               first_free_job;

    void schedule_job(const AsyncFunction & function, void * param, bool priority = false)
    {
        Lock lock(jobs_mutex);

        if (!first_free_job)
            throw sys::ResourceError("Cannot schedule task : no more job slots");

        Job * job = first_free_job;
        first_free_job = job->next;

        job->function = function;
        job->param = param;

        if (!first_job)
        {
            job->next = nullptr;
            first_job = job;
            last_job = job;
        }
        else if (priority)
        {
            job->next = first_job;
            first_job = job;
        }
        else
        {
            job->next = nullptr;
            last_job->next = job;
            last_job = job;
        }
        jobs_condition.notify_one();
    }

    Job * acquire_job()
    {
        UniqueLock lock(jobs_mutex);

        if (first_job == nullptr)
        {
            jobs_condition.wait(lock);
            if (first_job == nullptr)
                return nullptr;
        }

        Job * job = first_job;
        first_job = job->next;

        return job;
    }

    void free_job(Job * job)
    {
        Lock lock(jobs_mutex);

        job->next = first_free_job;
        first_free_job = job;
    }

    // Main function for workers
    void worker_main(void * _worker_num)
    {
        // int worker_num = reinterpret_cast<int>(_worker_num);

        while(!kill_workers)
        {
            Job * job = ::acquire_job();
            if (job)
            {
                job->function(job->param);
                ::free_job(job);
            }
        }
    }

} // namespace

// ---- ---- ---- ----

namespace res
{

namespace async
{

void initialize(bool enable_threading)
{
    BOOST_ASSERT_MSG(!d_ready, "Async module is already initialized");
    IWBAN_DEBUG(d_ready = true);

    threading_enabled = enable_threading;

    if (!enable_threading)
    {
        IWBAN_LOG_INFO("Threading is disabled");
        return;
    }

    // Jobs
    first_job = nullptr;
    last_job = nullptr;
    first_free_job = job_pool;

    for (unsigned ji = 0; ji < (JOBS_MAX - 1); ++ji)
    {
        job_pool[ji].next = &job_pool[ji+1];
    }
    job_pool[JOBS_MAX - 1].next = 0;

    // Workers
    kill_workers = false;

    unsigned threads = std::thread::hardware_concurrency();

    if (threads == 0)
        threads = WORKERS_DEFAULT;

    if (threads < WORKERS_MIN)
        threads = WORKERS_MIN;

    if (threads > WORKERS_MAX)
        threads = WORKERS_MAX;

    IWBAN_LOG_INFO("Starting %u workers", threads);

    std::memset(worker_pool, 0, sizeof(worker_pool));
    for (unsigned wi = 0; wi < threads; ++wi)
    {
        worker_pool[wi] = new Thread(worker_main, reinterpret_cast<void *>(wi));
    }
}

void terminate()
{
    BOOST_ASSERT_MSG(d_ready, "Async module is not initialized yet");
    IWBAN_DEBUG(d_ready = false);

    if (!threading_enabled)
        return;

    kill_workers = true;
    jobs_condition.notify_all();

    for (unsigned wi = 0; wi < WORKERS_MAX; ++wi)
    {
        if (worker_pool[wi])
        {
            worker_pool[wi]->join();
            worker_pool[wi] = 0;
        }
    }
}

void run(const AsyncFunction & function, void * param, bool priority)
{
    BOOST_ASSERT_MSG(d_ready, "Async module is not initialized yet");

    if (!threading_enabled)
        function(param);
    else
        ::schedule_job(function, param, priority);
}

}
// namespace async

}
// namespace res
