/**
 * @file   Async.hpp
 * @author Bastien Brunnenstein
 */

#ifndef _IWBAN_ASYNC_HPP_
#define _IWBAN_ASYNC_HPP_

#include <Global.hpp>

namespace res
{

namespace async
{

typedef void (*AsyncFunction) (void *);

// Start workers
void initialize(bool enable_threading);

// Kill workers
void terminate();

// Run a function on a secondary thread
// Set priority to true to run it as soon as possible
// This function should be thread safe, so you can use it within a secondary thread
// If threading is disabled in Config, using it "recursively" will probably cause errors
void run(AsyncFunction function, void * param, bool priority = false);

}
// namespace async

}
// namespace res

#endif // _IWBAN_ASYNC_HPP_
