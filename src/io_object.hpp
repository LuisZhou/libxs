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

#ifndef __XS_IO_OBJECT_HPP_INCLUDED__
#define __XS_IO_OBJECT_HPP_INCLUDED__

#include <stddef.h>

#include "stdint.hpp"
#include "poller_base.hpp"

namespace xs
{

    //  Simple base class for objects that live in I/O threads.
    //  It makes communication with the poller object easier and
    //  makes defining unneeded event handlers unnecessary.

    class io_object_t : public i_poll_events
    {
    public:

        io_object_t (xs::poller_base_t *io_thread_ = NULL);
        ~io_object_t ();

        //  When migrating an object from one I/O thread to another, first
        //  unplug it, then migrate it, then plug it to the new thread.
        void plug (xs::poller_base_t *io_thread_);
        void unplug ();

    protected:

        //  Methods to access underlying poller object.
        handle_t add_fd (fd_t fd_);
        void rm_fd (handle_t handle_);
        void set_pollin (handle_t handle_);
        void reset_pollin (handle_t handle_);
        void set_pollout (handle_t handle_);
        void reset_pollout (handle_t handle_);
        handle_t add_timer (int timout_);
        void rm_timer (handle_t handle_);

        //  i_poll_events interface implementation.
        void in_event (fd_t fd_);
        void out_event (fd_t fd_);
        void timer_event (handle_t handle_);

    private:

        poller_base_t *poller;

        io_object_t (const io_object_t&);
        const io_object_t &operator = (const io_object_t&);
    };

}

#endif
