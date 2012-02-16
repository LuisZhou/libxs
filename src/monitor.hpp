/*
    Copyright (c) 2012 250bpm s.r.o.
    Copyright (c) 2012 Other contributors as noted in the AUTHORS file

    This file is part of 0MQ.

    0MQ is free software; you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    0MQ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef __ZMQ_MONITOR_HPP_INCLUDED__
#define __ZMQ_MONITOR_HPP_INCLUDED__

#include "own.hpp"
#include "io_object.hpp"

namespace zmq
{

    class io_thread_t;
    class socket_base_t;

    class monitor_t : public own_t, public io_object_t
    {
    public:

        monitor_t (zmq::io_thread_t *io_thread_);
        ~monitor_t ();

        void start ();
        void stop ();

    private:

        enum {timer_id = 0x44};

        //  Handlers for incoming commands.
        void process_plug ();
        void process_stop ();

        //  Events from the poller.
        void timer_event (int id_);

        monitor_t (const monitor_t&);
        const monitor_t &operator = (const monitor_t&);
    };

}

#endif
