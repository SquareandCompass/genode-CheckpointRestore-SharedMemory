/*
 * \brief  Unit test for target_copy
 * \author Denis Huber
 * \date   2016-09-12
 */

/* Genode includes */
#include <base/env.h>
#include <base/log.h>
#include <base/component.h>
#include <base/sleep.h>
#include <timer_session/connection.h>

/* Rtcr includes */
#include "../../rtcr/target_copy.h"

using namespace Genode;

namespace Rtcr {
	struct Main;
}

struct Rtcr::Main
{
	enum { ROOT_STACK_SIZE = 16*1024 };
	Genode::Env             &env;
	Genode::Heap             md_heap         { env.ram(), env.rm() };
	Genode::Service_registry parent_services { };

	Main(Genode::Env &env_) : env(env_)
	{
		Target_child child { env, md_heap, parent_services, "sheep_counter", 1 };

		Timer::Connection timer { env };
		timer.msleep(5000);

		child.pause();

		log("Creating target copy instance");
		Target_copy copy { env, md_heap, child };

		log("Syncronising target copy with target child");
		copy.sync();

		child.resume();


		Genode::sleep_forever();
	}
};

size_t Component::stack_size() { return 64*1024; }

void Component::construct(Genode::Env &env)
{
	static Rtcr::Main main(env);
}
