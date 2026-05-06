#include "ls_app_uart.h"
#include "lisa_log.h"
#include "appinc.h"

#define LS_UART2_TAG "uart"

static ls_uart_receive_callback s_receive_cb = NULL;
static mod_inst_t s_uart_inst = NULL;

#if CONFIG_UPROT_ENABLE
static int ls_uart_on_recv(uint32_t size, void *data, void *user)
{
	if (s_receive_cb) {
		s_receive_cb(data, size);
	}
    return 0;
}
#endif

void ls_app_uart_init()
{
	if (s_uart_inst != NULL) {
		return;
	}
	LISA_LOGD(LS_UART2_TAG, "ls_app_uart_init enter");
#if CONFIG_UPROT_ENABLE
	s_uart_inst = __MAPI_FIND(CONFIG_UPROT_NAME);
    __mapi_hook(s_uart_inst, ls_uart_on_recv, NULL);
#endif
	LISA_LOGD(LS_UART2_TAG, "ls_app_uart_init end");
}

void ls_app_uart_send(const uint8_t *data, uint32_t len)
{
	LISA_LOGB(LS_UART2_TAG, data, len, "[TX] [%d]: -> ", len);
	if (s_uart_inst) {
		__mapi_write(s_uart_inst, data, len, 1000);
	}
}

void ls_app_uart_receive_cb(ls_uart_receive_callback cb)
{
	s_receive_cb = cb;
}