#include <iostream>
#include <iomanip>
#include <numeric>
#include <format>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

constexpr int MAX_USER_COUNT = 100000;
constexpr short MAX_PAGE_COUNT = 1000;


enum class RequestType {
	READ,
	CHEER
};

static RequestType FromStringToRequestType(std::string_view str) {
	if (str == "READ"sv) {
		return RequestType::READ;
	}

	if (str == "CHEER"sv) {
		return RequestType::CHEER;
	}

	throw std::runtime_error(std::format("Unknown request: {}", str));
}

class Book {
public:
	Book() : readers_(MAX_USER_COUNT + 1, -1), reading_progress_(MAX_PAGE_COUNT + 1, 0) {
	}

	void Read(int id, short page) {
		if (readers_[id] != -1) {
			--reading_progress_[readers_[id]];
		} else {
			++count_readers;
		}
		readers_[id] = page;
		++reading_progress_[page];
	}

	double Cheer(int id) const {
		if (readers_[id] == -1) {
			return 0.0;
		}

		if (readers_[id] != -1 && count_readers == 1) {
			return 1.0;
		}

		int lower_progress_count = CountUsersWithLessProgress(id);
		// проверку деления на ноль выполнили выше
		double lower_page_ratio = static_cast<double>(lower_progress_count) / (count_readers - 1);
		return lower_page_ratio;
	}

private:
	int CountUsersWithLessProgress(int id) const {
		const short user_page = readers_[id];
		int count = std::accumulate(reading_progress_.begin(), reading_progress_.begin() + user_page, 0);
		return count;
	}
	// index - user id, value - user page
	std::vector<short> readers_;
	// index - count pages, value - count users
	std::vector<int> reading_progress_;
	int count_readers = 0;
};

static void ProcessRequest(std::istream& in, std::ostream& out) {
	Book book;
	int requests_count = 0;
	in >> requests_count;
	for (int i = 0; i < requests_count; ++i) {
		std::string req_str;
		in >> req_str;
		RequestType req_type = FromStringToRequestType(req_str);
		int user_id = -1;
		short page = 0;
		switch (req_type) {
		case RequestType::READ:
			in >> user_id >> page;
			book.Read(user_id, page);
			break;
		case RequestType::CHEER:
			in >> user_id;
			out << std::setprecision(6) << book.Cheer(user_id) << '\n';
			break;
		default:
			throw std::runtime_error("Unknown type of request");
			break;
		}
	}
}

int main() {
	ProcessRequest(std::cin, std::cout);
	return 0;
}
