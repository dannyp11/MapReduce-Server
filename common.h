/*
 * Common file for both server and client
 * This contains config items such as port numbers, IP address, etc.
 */

// debug macro
#define LOG(msg) \
    std::cout << __FILE__ << "(" << __LINE__ << "): " << msg << std::endl ;

#define TRACE(val) \
		std::cout << __FILE__ << "(" << __LINE__ << "): " << #val << " : " << val << std::endl ;

// define ports --------------------------
const int UDP_PORT_A = 21000 + 366;
const int UDP_PORT_B = 22000 + 366;
const int UDP_PORT_C = 23000 + 366;
const int UDP_PORT_AWS = 24000 + 366;
const int TCP_PORT_D = 25000 + 366;
// define ports ends ----------------------

/*
 * This function parses command line argument to get basic info
 */
void parse_command_line_args(int argc, char** argv);

