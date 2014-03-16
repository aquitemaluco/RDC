#include <sinet.h>

void run_19216801();
void run_19216802();


int main(int argc, char *argv[])
{
	/* simulation set up. */
	sinet_host_t hosts[2] = { 
		sinet_new_host("192.168.0.1", 24, "aa:bb:cc:dd:ee:ff"),
		sinet_new_host("192.168.0.2", 24, "11:22:33:44:55:66")
	};


	sinet_init(HUB);

	sinet_connect(host[0]);
	sinet_set_run(host[0], run_19216801);
	sinet_connect(host[1]);
	sinet_set_run(host[1], run_19216802);

	sinet_start_execution();

	sinet_free();
	return 0;
}

void run_19216801()
{
	sinet_message_t msg;

	sinet_send(sinet_new_message("192.168.0.1", "192.168.0.2", "teste"));
	sleep(1);

	msg = sinet_recv();
	printf("%s\n", (char *) msg.payload);
	sleep(1);
}


void run_19216802()
{
	sinet_message_t msg;

	msg = sinet_recv();
	printf("%s\n", (char *) msg.payload);
	sleep(1);

	sinet_send(sinet_new_message("192.168.0.2", "192.168.0.1", "yahoo"));
	sleep(1);
}

