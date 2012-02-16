/*
    Copyright (c) 2009-2012 250bpm s.r.o.
    Copyright (c) 2007-2009 iMatix Corporation
    Copyright (c) 2007-2011 Other contributors as noted in the AUTHORS file

    This file is part of Crossroads project.

    Crossroads is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    Crossroads is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __XS_IO_THREAD_HPP_INCLUDED__
#define __XS_IO_THREAD_HPP_INCLUDED__

#include <vector>

#include "stdint.hpp"
#include "object.hpp"
#include "poller_base.hpp"
#include "mailbox.hpp"

namespace xs
{

    class ctx_t;

    //  Generic part of the I/O thread. Polling-mechanism-specific features
    //  are implemented in separate "polling objects".

    class io_thread_t : public object_t, public i_poll_events
    {
    public:

        io_thread_t (xs::ctx_t *ctx_, uint32_t tid_);

        //  Clean-up. If the thread was started, it's neccessary to call 'stop'
        //  before invoking destructor. Otherwise the destructor would hang up.
        ~io_thread_t ();

        //  Launch the physical thread.
        void start ();

        //  Ask underlying thread to stop.
        void stop ();

        //  Returns mailbox associated with this I/O thread.
        mailbox_t *get_mailbox ();

        //  i_poll_events implementation.
        void in_event (fd_t fd_);
        void out_event (fd_t fd_);
        void timer_event (int id_);

        //  Used by io_objects to retrieve the assciated poller object.
        poller_base_t *get_poller ();

        //  Command handlers.
        void process_stop ();

        //  Returns load experienced by the I/O thread.
        int get_load ();

    private:

        //  I/O thread accesses incoming commands via this mailbox.
        mailbox_t mailbox;

        //  Handle associated with mailbox' file descriptor.
        handle_t mailbox_handle;

        //  I/O multiplexing is performed using a poller object.
        poller_base_t *poller;

        io_thread_t (const io_thread_t&);
        const io_thread_t &operator = (const io_thread_t&);
    };

}

#endif
