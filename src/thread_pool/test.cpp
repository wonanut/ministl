#include <iostream>
#include "CThreadPool.h"

int main() {
	tlib::CThreadPool pool(10);
	/*auto result = pool.enqueue([](int answer) {return answer; }, 42);
	std::cout << result.get() << std::endl;*/

	std::vector<std::future<int>> results;

	for (int i = 0; i < 50; i++) {
		results.emplace_back(
			pool.enqueue([i] {
				std::cout << "hello " << i << std::endl;
				std::this_thread::sleep_for(std::chrono::seconds(1));
				std::cout << "world " << i << std::endl;
				return i * i;
			})
		);
	}

	//for (auto&& result : results) {
	//	std::cout << result.get() << " ";
	//}
	//std::cout << std::endl;

	return 0;
}
