#ifdef __cplusplus
extern "C" {
#endif

#ifndef __PIKA_ADAPTER_OLD_API_H__
#define __PIKA_ADAPTER_OLD_API_H__

/* micro pika configuration */
#include "./pika_config_valid.h"

/*
 * This file is used to support old api, it's not recommended to use it.
 * In new project, please use new api instead.
 */

#if PIKA_OLD_API_ENABLE
#define __platform_enable_irq_handle pika_platform_enable_irq_handle
#define __platform_disable_irq_handle pika_platform_disable_irq_handle
#define __platform_printf pika_platform_printf
#define __platform_vsnprintf pika_platform_vsnprintf
#define __platform_sprintf pika_sprintf
#define __platform_strdup pika_platform_strdup
#define __platform_tick_from_millisecond pika_platform_tick_from_millisecond
#define __platform_malloc pika_platform_malloc
#define __platform_realloc pika_platform_realloc
#define __platform_calloc pika_platform_calloc
#define __platform_free pika_platform_free
#define __platform_memset pika_platform_memset
#define __platform_memcpy pika_platform_memcpy
#define __platform_memcmp pika_platform_memcmp
#define __platform_memmove pika_platform_memmove
#define __platform_wait pika_platform_wait
#define __platform_getchar pika_platform_getchar
#define __platform_putchar pika_platform_putchar
#define __platform_fopen pika_platform_fopen
#define __platform_fclose pika_platform_fclose
#define __platform_fwrite pika_platform_fwrite
#define __platform_fread pika_platform_fread
#define __platform_fseek pika_platform_fseek
#define __platform_ftell pika_platform_ftell
#define __platform_error_handle pika_platform_error_handle
#define __platform_panic_handle pika_platform_panic_handle
#define __platform_thread_delay pika_platform_thread_delay
#define __platform_getTick pika_platform_get_tick
#define pika_platform_getTick pika_platform_get_tick
#define __platform_sleep_ms pika_platform_sleep_ms
#define __platform_sleep_s pika_platform_sleep_s

#define __pks_hook_instruct pika_hook_instruct
#define __pks_hook_arg_cache_filter pika_hook_arg_cache_filter
#define __user_malloc pika_user_malloc
#define __user_free pika_user_free
#define __is_locked_pikaMemory pika_is_locked_pikaMemory

/* old api */
#define __platform_socket pika_platform_socket
#define __platform_bind pika_platform_bind
#define __platform_listen pika_platform_listen
#define __platform_accept pika_platform_accept
#define __platform_connect pika_platform_connect
#define __platform_send pika_platform_send
#define __platform_recv pika_platform_recv
#define __platform_gethostname pika_platform_gethostname
#define __platform_getaddrinfo pika_platform_getaddrinfo
#define __platform_freeaddrinfo pika_platform_freeaddrinfo
#define __platform_setsockopt pika_platform_setsockopt
#define __platform_close pika_platform_close
#define __platform_write pika_platform_write
#define __platform_fcntl pika_platform_fcntl

#define pks_eventLicener_registEvent pks_eventListener_registEvent
#define pks_eventLisener_init pks_eventListener_init
#define pks_eventLisener_getEventHandleObj pks_eventListener_getEventHandleObj
#define pks_eventLisener_sendSignal pks_eventListener_sendSignal
#define pks_eventLisener_deinit pks_eventListener_deinit
#define pks_eventLicener_removeEvent pks_eventListener_removeEvent

#define pika_platform_timer_init pika_platform_thread_timer_init
#define pika_platform_timer_cutdown pika_platform_thread_timer_cutdown
#define pika_platform_timer_is_expired pika_platform_thread_timer_is_expired
#define pika_platform_timer_remain pika_platform_thread_timer_remain
#define pika_platform_timer_now pika_platform_thread_timer_now
#define pika_platform_timer_usleep pika_platform_thread_timer_usleep

#endif

#endif

#ifdef __cplusplus
}
#endif
