/*
 * Common file for both server and client
 * This contains config items such as port numbers, IP address, etc.
 */

#include <string>
#include <climits>

using std::string;

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
const int TCP_PORT_AWS = 25000 + 366;
// define ports ends ----------------------

/*
 * define entry limit
 */
const int MAX_TCP_ENTRIES = 1000;

/*
 * define client - aws server message structure
 */
typedef enum e_ClientCalcCommand
{
	CMIN, CMAX, CSUM, CSOS, CRESULT
} ClientCalcCommand;

typedef struct s_ClientMessage
{
	ClientCalcCommand command; // RESULT if reply
	long resultValue; // must be LONG_MIN if sent from client
	int entriesCount; // length of data
	long data[MAX_TCP_ENTRIES]; // must be null if sent from A, B, C. Limit to 350 values
} ClientMessage;

/*
 * This function parses command line argument to get basic info
 */
static string DEF_NAME = "A";
void parse_command_line_args(int argc, char** argv, string& server_name = DEF_NAME);

/*
 * simple name converter for ClientCalcCommand
 */
string getClientCalcCommandName(ClientCalcCommand cmd);
