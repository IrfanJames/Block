#pragma once

#include <iostream>
#include <fstream>

#ifdef NDEBUG
namespace LOG {

	// Making .txt File For Logging in Release Mode
	inline std::ofstream log_file("temp_files/log.txt", std::ios::out, std::ios::trunc);
}
#endif

// LOG Function
#ifdef MY_NDIST

#ifdef _DEBUG
#define LOG(x) std::cout << x;
#else
#define LOG(x) LOG::log_file << x;
#endif

#else
#define LOG(x) ;
#endif

// LOG_VEC Function
#ifdef MY_NDIST
#define LOG_VEC(vec) { LOG("\n(" << vec.size() << "): "); for (size_t p4gr64tb3rdy2trt7829ry7tc50315h7589y87ny3t28y3nrtu3vt6 = 0; p4gr64tb3rdy2trt7829ry7tc50315h7589y87ny3t28y3nrtu3vt6 < vec.size(); p4gr64tb3rdy2trt7829ry7tc50315h7589y87ny3t28y3nrtu3vt6++) LOG(vec[i] << " "); }
#else
#define LOG_VEC(vec) ;
#endif

// LOG_VEC2 Function
#ifdef MY_NDIST
#define LOG_TAB(n) for (int64_t gjignco87rytc567438r538t5675ye2htdr98239ey278h9d274621 = 0; gjignco87rytc567438r538t5675ye2htdr98239ey278h9d274621 < n; gjignco87rytc567438r538t5675ye2htdr98239ey278h9d274621++) LOG("   ")
#else
#define LOG_TAB(vec) ;
#endif

