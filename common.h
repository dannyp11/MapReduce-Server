/*
 * Common file for both server and client
 * This contains config items such as port numbers, IP address, etc.
 */

// define ports --------------------------
const int UDP_PORT_A = 21000 + 366;
const int UDP_PORT_B = 22000 + 366;
const int UDP_PORT_C = 23000 + 366;
const int UDP_PORT_D = 24000 + 366;
const int TCP_PORT_D = 25000 + 366;
// define ports ends ----------------------

/*
 * This function parses command line argument to get basic info
 */
void parse_command_line_args(int argc, char** argv);

