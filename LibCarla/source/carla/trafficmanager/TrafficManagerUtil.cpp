// Copyright (c) 2019 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close

#include "carla/trafficmanager/TrafficManagerUtil.h"

#define INVALID_INDEX           -1
#define IP_DATA_BUFFER_SIZE     80

namespace carla {
namespace traffic_manager {

/// Get IP address of the local system
std::pair<std::string, uint16_t> TrafficManagerUtil::GetLocalIP(const uint16_t sport) {

	int err;
	std::pair<std::string, uint16_t> localIP;
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_INDEX) {
#if DEBUG_PRINT_TM
		std::cout << "Error number1: " << errno << std::endl;
		std::cout << "Error message: " << strerror(errno) << std::endl;
#endif
	} else {
		sockaddr_in loopback;
		memset(&loopback, 0, sizeof(loopback));
		loopback.sin_family = AF_INET;
		loopback.sin_addr.s_addr = INADDR_LOOPBACK;
		loopback.sin_port = htons(9);
		err = connect
				( sock
						, reinterpret_cast<sockaddr*>(&loopback)
						, sizeof(loopback));
		if(err == INVALID_INDEX) {
#if DEBUG_PRINT_TM
			std::cout << "Error number2: " << errno << std::endl;
			std::cout << "Error message: " << strerror(errno) << std::endl;
#endif
		} else {
			socklen_t addrlen = sizeof(loopback);
			err = getsockname
					( sock
							, reinterpret_cast<struct sockaddr*> (&loopback)
							, &addrlen);
			if(err == INVALID_INDEX) {
#if DEBUG_PRINT_TM
				std::cout << "Error number3: " << errno << std::endl;
				std::cout << "Error message: " << strerror(errno) << std::endl;
#endif
			} else {
				char buffer[IP_DATA_BUFFER_SIZE];
				const char* p = inet_ntop
						( AF_INET
								, &loopback.sin_addr, buffer
								, IP_DATA_BUFFER_SIZE);
				if(p != NULL) {
					localIP = std::pair<std::string, uint16_t>(std::string(buffer), sport);
				} else {
#if DEBUG_PRINT_TM
					std::cout << "Error number4: " << errno << std::endl;
					std::cout << "Error message: " << strerror(errno) << std::endl;
#endif
				}
			}
		}
		close(sock);
	}
	return localIP;
}

} // namespace traffic_manager
} // namespace carla
