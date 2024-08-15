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

		F_task_counter counter = 0;
		task_system_p->schedule(
			[&](F_task_context context, u32 index)
			{
				s.fetch_add(1);
			},
			{
				.counter_p = &counter,
				.parallel_count = 1000000,
				.batch_size = 10000
			}
		);

		task_system_p->start();
		task_system_p->join();
	}

	ncpp::pause_console();

	return 0;
}
