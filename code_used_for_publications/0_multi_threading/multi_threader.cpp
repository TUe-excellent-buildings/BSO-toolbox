#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <fstream>
#include <mutex>
#include <deque>
#include <future>
#include <vector>
#include <chrono>

class Synch_File {
private:
	std::string m_path;
	std::mutex m_write_mutex;
	std::once_flag m_file_open_flag;
	std::ofstream m_out;
public:
	Synch_File(const std::string& path) : m_path(path) {
		m_out.open(m_path.c_str());
	}
	~Synch_File() {if(m_out.is_open()) m_out.close();}

	void write (const std::string& dataToWrite) {
		// Ensure that only one thread can execute at a time
		std::lock_guard<std::mutex> lock(m_write_mutex);
		
		if (!m_out.is_open()) throw std::runtime_error("could not write to: " + m_path);
		m_out << dataToWrite;
		m_out.flush();
	}
};

void exec(const char* cmd, std::shared_ptr<Synch_File> sf) {
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    sf->write(result);
}

class Thread_Handler {
private:
	std::shared_ptr<Synch_File> m_writer;
	std::deque<std::string> m_task_queue;
	unsigned int m_number_of_threads;
	std::vector<std::thread> m_threads;
	std::mutex m_qeueu_mutex;
	std::mutex m_start_mutex;
	
	void thread_process(){
		bool finished = false;

		while (!finished) {
			std::string task;
			{ // lock while obtaining a task
				std::lock_guard<std::mutex> lock(m_qeueu_mutex);
				if (!m_task_queue.empty()) {
					task = m_task_queue.front();
					m_task_queue.pop_front();
				}
				else finished = true;
			}
			if (!finished) {
				exec(task.c_str(), m_writer);
			}
		}
	}
public:
	Thread_Handler(std::string input, std::string output, unsigned int threads)
	{
		m_writer = std::make_shared<Synch_File>(output);
		m_number_of_threads = threads;
		if (m_number_of_threads > std::thread::hardware_concurrency()*0.85) throw std::runtime_error(
			"Assigned too many threads (>85%), risking thread overload");

		std::ifstream in(input.c_str());
		if (!in.is_open()) throw std::runtime_error("could not open queue file!");
		std::string line;

		while (getline(in, line))
			if (line != "") m_task_queue.push_back(line);
		
		in.close();
	}
	
	void run_tasks() {
		for (unsigned int i = 0; i < m_number_of_threads; i++)
			m_threads.push_back(std::thread(&Thread_Handler::thread_process, this));

		for (auto& i : m_threads) i.join();
	}
};

int main (int argc, char* argv[]) {
	Thread_Handler handler(argv[1],argv[2], atoi(argv[3]));
	handler.run_tasks();

	return 0;
}