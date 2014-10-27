/**
 * @file   Async.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ASYNC_HPP_
#define _IWBAN_ASYNC_HPP_

#include <Global.hpp>

#include <functional>

#ifndef NDEBUG
#   include <string>
#endif

namespace res
{

namespace async
{

typedef std::function<void (void *)> AsyncFunction;

// Start workers
void initialize(bool enable_threading);

// Kill workers
void terminate();

// Run a function on a secondary thread
// Set priority to true to run it as soon as possible
// This function should be thread safe, so you can use it within a secondary thread
// If threading is disabled in Config, using it "recursively" will probably cause errors
// Throw sys::ResourceError if all job slots are in use
void run(const AsyncFunction & function, void * param, bool priority = false);

// ---- ---- ---- ----

#ifndef NDEBUG

/**
 * Start standard input polling.
 *
 * Call this function only once.
 * This function creates a thread that is collected on program exit.
 */
void startStdInputPolling();

/**
 * Try to consume a line from standard input.
 *
 * @param[out] polled_input String that will contain the polled input.
 * @return @c true if an input has been polled.
 */
bool pollStdInput(std::string & polled_input);

#endif

}
// namespace async

}
// namespace res

#endif // _IWBAN_ASYNC_HPP_
