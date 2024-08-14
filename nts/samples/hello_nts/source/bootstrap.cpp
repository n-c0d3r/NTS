#include <nts/.hpp>

using namespace nts;



int main() {

	auto task_system_p = TU<F_task_system>()(
		F_task_system_desc {
		}
	);

	task_system_p->schedule(
		[](F_coroutine&, u32)
		{
		}
	);

	task_system_p->start();
	task_system_p->join();

	ncpp::pause_console();

	return 0;
}
