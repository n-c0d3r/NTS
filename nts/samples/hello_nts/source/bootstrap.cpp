#include <nts/.hpp>

using namespace nts;



int main()
{
	{
		auto task_system_p = TU<F_task_system>()(
			F_task_system_desc {
			}
		);

		au32 s = 0;

		F_task_counter counter2 = 0;
		task_system_p->schedule(
			[](u32 index)
			{
			},
			{
				.counter_p = &counter2
			}
		);

		F_task_counter counter = 0;
		task_system_p->schedule(
			[&s, &counter2](u32 index)
			{
				H_task_context::yield(
					F_wait_for_counter(&counter2)
				);
				s.fetch_add(1);
			},
			{
				.counter_p = &counter,
				.parallel_count = 1000000,
				.batch_size = 10000
			}
		);

		task_system_p->start();

		while(counter);

		task_system_p->join();

		NCPP_INFO() << s;
	}

	ncpp::pause_console();

	return 0;
}
