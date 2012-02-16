/*
    Copyright (c) 2009-2012 250bpm s.r.o.
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of Crossroads I/O project.

    Crossroads I/O is free software; you can redistribute it and/or modify it
    under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Crossroads is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __XS_UTILS_H_INCLUDED__
#define __XS_UTILS_H_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif

/*  Handle DSO symbol visibility                                             */
#if defined _WIN32
#   if defined DLL_EXPORT
#       define XS_EXPORT __declspec(dllexport)
#   else
#       define XS_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined __SUNPRO_C  || defined __SUNPRO_CC
#       define XS_EXPORT __global
#   elif (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER
#       define XS_EXPORT __attribute__ ((visibility("default")))
#   else
#       define XS_EXPORT
#   endif
#endif

/*  Helper functions are used by perf tests so that they don't have to care   */
/*  about minutiae of different OS platforms.                                 */

/*  Starts the stopwatch. Returns the handle to the watch.                    */
XS_EXPORT void *xs_stopwatch_start (void);

/*  Stops the stopwatch. Returns the number of microseconds elapsed since     */
/*  the stopwatch was started.                                                */
XS_EXPORT unsigned long xs_stopwatch_stop (void *watch);

/*  Sleeps for specified number of seconds.                                   */
XS_EXPORT void xs_sleep (int seconds);

/*  Creates a new thread.                                                     */
XS_EXPORT void *xs_thread_create (void (*fn) (void *arg), void* arg);

/*  Wait for thread to finish.                                                */
XS_EXPORT void xs_thread_join (void *thread);

#undef XS_EXPORT

#ifdef __cplusplus
}
#endif

#endif
