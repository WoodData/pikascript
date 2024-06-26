#ifndef _PIKA_HAL_STM32_COMMON_H_
#define _PIKA_HAL_STM32_COMMON_H_

#include "../PikaStdDevice/pika_hal.h"
#include "pika_adapter_rtt.h"

#ifndef rt_container_of
#define rt_container_of(ptr, type, member) \
    ((type *)((char *)(ptr) - (unsigned long)(&((type *)0)->member)))
#endif
    
#define CHIP_FAMILY_STM32
#define SOC_FAMILY_STM32
#define RT_USING_DEVICE_OPS
    
#define SERIES_OF(_start) (defined(_start##xx) || defined(_start##x8) || \
                          defined(_start##xB) || defined(_start##xC) || \
                          defined(_start##xD) || defined(_start##xE) || defined(_start##xG))

/* auto select HAL header */
#if SERIES_OF(STM32H750) || SERIES_OF(STM32H743) || SERIES_OF(STM32H753) || \
    SERIES_OF(STM32H755) || SERIES_OF(STM32H757)
#define SOC_SERIES_STM32H7
#define CHIP_SERIES_STM32H7
#include "stm32h7xx_hal.h"

#elif SERIES_OF(STM32F103) || SERIES_OF(STM32F102) || SERIES_OF(STM32F101)
#define SOC_SERIES_STM32F1
#define CHIP_SERIES_STM32F1
#include "stm32f1xx_hal.h"

#elif SERIES_OF(STM32G030) || SERIES_OF(STM32G031) || SERIES_OF(STM32G041) || \
      SERIES_OF(STM32G070) || SERIES_OF(STM32G071) || SERIES_OF(STM32G081)
#define SOC_SERIES_STM32G0
#define CHIP_SERIES_STM32G0
#include "stm32g0xx_hal.h"

#elif SERIES_OF(STM32F030) || SERIES_OF(STM32F031) || SERIES_OF(STM32F051) || \
      SERIES_OF(STM32F070) || SERIES_OF(STM32F042)
#define SOC_SERIES_STM32F0
#define CHIP_SERIES_STM32F0
#include "stm32f0xx_hal.h"

#elif SERIES_OF(STM32F401) || SERIES_OF(STM32F405) || SERIES_OF(STM32F407) || \
      SERIES_OF(STM32F411)
#define SOC_SERIES_STM32F4
#define CHIP_SERIES_STM32F4
#include "stm32f4xx_hal.h"

#elif SERIES_OF(STM32U585) || SERIES_OF(STM32U575)
#define SOC_SERIES_STM32U5
#define CHIP_SERIES_STM32U5
#include "stm32u5xx_hal.h"

#elif SERIES_OF(STM32L451) || SERIES_OF(STM32L452) || SERIES_OF(STM32L462) || \
      SERIES_OF(STM32L471) || SERIES_OF(STM32L475) || SERIES_OF(STM32L476) || \
      SERIES_OF(STM32L485) || SERIES_OF(STM32L486)
#define SOC_SERIES_STM32L4
#define CHIP_SERIES_STM32L4
#include "stm32l4xx_hal.h"

#else
#error "Unsupported STM32 series"
#endif

/* struct */

struct rt_pin_irq_hdr
{
    rt_base_t        pin;
    rt_uint8_t       mode; /* e.g. PIN_IRQ_MODE_RISING */
    void (*hdr)(void *args);
    void             *args;
};

struct rt_pin_ops
{
    void (*pin_mode)(struct rt_device *device, rt_base_t pin, rt_uint8_t mode);
    void (*pin_write)(struct rt_device *device, rt_base_t pin, rt_uint8_t value);
    rt_int8_t  (*pin_read)(struct rt_device *device, rt_base_t pin);
    rt_err_t (*pin_attach_irq)(struct rt_device *device, rt_base_t pin,
            rt_uint8_t mode, void (*hdr)(void *args), void *args);
    rt_err_t (*pin_detach_irq)(struct rt_device *device, rt_base_t pin);
    rt_err_t (*pin_irq_enable)(struct rt_device *device, rt_base_t pin, rt_uint8_t enabled);
    rt_base_t (*pin_get)(const char *name);
};


#define PIN_NONE                (-1)

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01
#define PIN_MODE_INPUT_PULLUP   0x02
#define PIN_MODE_INPUT_PULLDOWN 0x03
#define PIN_MODE_OUTPUT_OD      0x04

#define PIN_IRQ_MODE_RISING             0x00
#define PIN_IRQ_MODE_FALLING            0x01
#define PIN_IRQ_MODE_RISING_FALLING     0x02
#define PIN_IRQ_MODE_HIGH_LEVEL         0x03
#define PIN_IRQ_MODE_LOW_LEVEL          0x04

#define PIN_IRQ_DISABLE                 0x00
#define PIN_IRQ_ENABLE                  0x01

#define PIN_IRQ_PIN_NONE                PIN_NONE

#define RT_DEVICE_CTRL_RESUME           0x01            /**< resume device */
#define RT_DEVICE_CTRL_SUSPEND          0x02            /**< suspend device */
#define RT_DEVICE_CTRL_CONFIG           0x03            /**< configure device */
#define RT_DEVICE_CTRL_CLOSE            0x04            /**< close device */
#define RT_DEVICE_CTRL_NOTIFY_SET       0x05            /**< set notify func */
#define RT_DEVICE_CTRL_SET_INT          0x06            /**< set interrupt */
#define RT_DEVICE_CTRL_CLR_INT          0x07            /**< clear interrupt */
#define RT_DEVICE_CTRL_GET_INT          0x08            /**< get interrupt status */
#define RT_DEVICE_CTRL_CONSOLE_OFLAG    0x09            /**< get console open flag */
#define RT_DEVICE_CTRL_MASK             0x1f            /**< mask for contrl commands */

#define RT_DEVICE_FLAG_DEACTIVATE       0x000           /**< device is not not initialized */

#define RT_DEVICE_FLAG_RDONLY           0x001           /**< read only */
#define RT_DEVICE_FLAG_WRONLY           0x002           /**< write only */
#define RT_DEVICE_FLAG_RDWR             0x003           /**< read and write */

#define RT_DEVICE_FLAG_REMOVABLE        0x004           /**< removable device */
#define RT_DEVICE_FLAG_STANDALONE       0x008           /**< standalone device */
#define RT_DEVICE_FLAG_ACTIVATED        0x010           /**< device is activated */
#define RT_DEVICE_FLAG_SUSPENDED        0x020           /**< device is suspended */
#define RT_DEVICE_FLAG_STREAM           0x040           /**< stream mode */

#define RT_DEVICE_FLAG_INT_RX           0x100           /**< INT mode on Rx */
#define RT_DEVICE_FLAG_DMA_RX           0x200           /**< DMA mode on Rx */
#define RT_DEVICE_FLAG_INT_TX           0x400           /**< INT mode on Tx */
#define RT_DEVICE_FLAG_DMA_TX           0x800           /**< DMA mode on Tx */

#define RT_DEVICE_OFLAG_CLOSE           0x000           /**< device is closed */
#define RT_DEVICE_OFLAG_RDONLY          0x001           /**< read only access */
#define RT_DEVICE_OFLAG_WRONLY          0x002           /**< write only access */
#define RT_DEVICE_OFLAG_RDWR            0x003           /**< read and write */
#define RT_DEVICE_OFLAG_OPEN            0x008           /**< device is opened */
#define RT_DEVICE_OFLAG_MASK            0xf0f           /**< mask of open flag */

#ifdef RT_USING_DEVICE_OPS
/**
 * operations set for device object
 */
struct rt_device_ops
{
    /* common device interface */
    rt_err_t  (*init)   (rt_device_t dev);
    rt_err_t  (*open)   (rt_device_t dev, rt_uint16_t oflag);
    rt_err_t  (*close)  (rt_device_t dev);
    rt_ssize_t (*read)  (rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size);
    rt_ssize_t (*write) (rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size);
    rt_err_t  (*control)(rt_device_t dev, int cmd, void *args);
};
#endif /* RT_USING_DEVICE_OPS */

static inline rt_base_t rt_hw_interrupt_disable(void){
    pika_platform_disable_irq_handle();
    return 0;
}

static inline void rt_hw_interrupt_enable(rt_base_t level){
    pika_platform_enable_irq_handle();
}

struct rt_adc_device;
struct rt_adc_ops
{
    rt_err_t (*enabled)(struct rt_adc_device *device, rt_int8_t channel, rt_bool_t enabled);
    rt_err_t (*convert)(struct rt_adc_device *device, rt_int8_t channel, rt_uint32_t *value);
    rt_uint8_t (*get_resolution)(struct rt_adc_device *device);
    rt_int16_t (*get_vref) (struct rt_adc_device *device);
};

struct rt_adc_device
{
    struct rt_device parent;
    const struct rt_adc_ops *ops;
};
typedef struct rt_adc_device *rt_adc_device_t;

static rt_ssize_t _adc_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
    rt_err_t result = RT_EOK;
    rt_size_t i;
    struct rt_adc_device *adc = (struct rt_adc_device *)dev;
    rt_uint32_t *value = (rt_uint32_t *)buffer;

    for (i = 0; i < size; i += sizeof(int))
    {
        result = adc->ops->convert(adc, pos + i, value);
        if (result != RT_EOK)
        {
            return 0;
        }
        value++;
    }

    return i;
}

static inline rt_err_t rt_adc_enable(rt_adc_device_t dev, rt_int8_t channel)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev);

    if (dev->ops->enabled != RT_NULL)
    {
        result = dev->ops->enabled(dev, channel, RT_TRUE);
    }
    else
    {
        result = -RT_ENOSYS;
    }

    return result;
}

static inline rt_err_t rt_adc_disable(rt_adc_device_t dev, rt_int8_t channel)
{
    rt_err_t result = RT_EOK;

    RT_ASSERT(dev);

    if (dev->ops->enabled != RT_NULL)
    {
        result = dev->ops->enabled(dev, channel, RT_FALSE);
    }
    else
    {
        result = -RT_ENOSYS;
    }

    return result;
}

static inline rt_uint32_t rt_adc_read(rt_adc_device_t dev, rt_int8_t channel)
{
    rt_uint32_t value;

    RT_ASSERT(dev);

    dev->ops->convert(dev, channel, &value);

    return value;
}


#define PIN_NUM(port, no) (((((port) & 0xFu) << 4) | ((no) & 0xFu)))
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 4) & 0xFu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0xFu))

#if defined(SOC_SERIES_STM32MP1)
#if defined(GPIOZ)
#define gpioz_port_base (175) /* PIN_STPORT_MAX * 16 - 16 */
#define PIN_STPORT(pin) ((pin > gpioz_port_base) ? ((GPIO_TypeDef *)(GPIOZ_BASE )) : ((GPIO_TypeDef *)(GPIOA_BASE + (0x1000u * PIN_PORT(pin)))))
#else
#define PIN_STPORT(pin) ((GPIO_TypeDef *)(GPIOA_BASE + (0x1000u * PIN_PORT(pin))))
#endif /* GPIOZ */
#else
#define PIN_STPORT(pin) ((GPIO_TypeDef *)(GPIOA_BASE + (0x400u * PIN_PORT(pin))))
#endif /* SOC_SERIES_STM32MP1 */

#define PIN_STPIN(pin) ((uint16_t)(1u << PIN_NO(pin)))

/* e.g. PA7 */
rt_base_t _stm32_pin_get(const char *name);

#define RT_ADC_INTERN_CH_TEMPER     (-1)
#define RT_ADC_INTERN_CH_VREF       (-2)
#define RT_ADC_INTERN_CH_VBAT       (-3)


#define BSP_USING_GPIO
#define RT_USING_PIN
#define BSP_USING_PWM

#ifdef ADC1
#define BSP_USING_ADC1
#endif

#ifdef DAC1
#define BSP_USING_DAC1
#endif

#if defined(USART1) || defined(UART1)
#define BSP_USING_UART1
#endif

#if defined(USART2) || defined(UART2)
#define BSP_USING_UART2
#endif

#if defined(USART3) || defined(UART3)
#define BSP_USING_UART3
#endif

#if defined(USART4) || defined(UART4)
#define BSP_USING_UART4
#endif

#if defined(USART5) || defined(UART5)
#define BSP_USING_UART5
#endif

#if defined(USART6) || defined(UART6)
#define BSP_USING_UART6
#endif

#if defined(USART7) || defined(UART7)
#define BSP_USING_UART7
#endif

#if defined(USART8) || defined(UART8)
#define BSP_USING_UART8
#endif


#if defined(BSP_USING_UART1) || defined(BSP_USING_UART2) || defined(BSP_USING_UART3) || \
    defined(BSP_USING_UART4) || defined(BSP_USING_UART5) || defined(BSP_USING_UART6) || \
    defined(BSP_USING_UART7) || defined(BSP_USING_UART8) || defined(BSP_USING_LPUART1)
#define RT_USING_SERIAL
// #define RT_SERIAL_USING_DMA

#endif


#if defined(TIM1)
#define BSP_USING_PWM1
#endif

#if defined(TIM2)
#define BSP_USING_PWM2
#endif

#if defined(TIM3)
#define BSP_USING_PWM3
#endif

#if defined(TIM4)
#define BSP_USING_PWM4
#endif

#if defined(TIM5)
#define BSP_USING_PWM5
#endif

#if defined(TIM6)
#define BSP_USING_PWM6
#endif

#if defined(TIM7)
#define BSP_USING_PWM7
#endif

#if defined(TIM8)
#define BSP_USING_PWM8
#endif

#if defined(TIM9)
#define BSP_USING_PWM9
#endif

#if defined(TIM10)
#define BSP_USING_PWM10
#endif

#if defined(TIM11)
#define BSP_USING_PWM11
#endif

#if defined(TIM12)
#define BSP_USING_PWM12
#endif

#if defined(TIM13)
#define BSP_USING_PWM13
#endif

#if defined(TIM14)
#define BSP_USING_PWM14
#endif

#if defined(TIM15)
#define BSP_USING_PWM15
#endif

#if defined(TIM16)
#define BSP_USING_PWM16
#endif

#if defined(TIM17)
#define BSP_USING_PWM17
#endif

struct rt_dac_device;
struct rt_dac_ops
{
    rt_err_t (*disabled)(struct rt_dac_device *device, rt_uint32_t channel);
    rt_err_t (*enabled)(struct rt_dac_device *device, rt_uint32_t channel);
    rt_err_t (*convert)(struct rt_dac_device *device, rt_uint32_t channel, rt_uint32_t *value);
    rt_uint8_t (*get_resolution)(struct rt_dac_device *device);
};

struct rt_dac_device
{
    struct rt_device parent;
    const struct rt_dac_ops *ops;
};

struct rt_ringbuffer
{
    rt_uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     */

    rt_uint32_t read_mirror : 1;
    rt_uint32_t read_index : 31;
    rt_uint32_t write_mirror : 1;
    rt_uint32_t write_index : 31;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    rt_int32_t buffer_size;
};

struct rt_serial_rx_fifo
{
    struct rt_ringbuffer rb;

//    struct rt_completion rx_cpt;

    rt_uint16_t rx_cpt_index;

    /* software fifo */
    rt_uint8_t buffer[];
};

typedef struct rt_dac_device *rt_dac_device_t;

#define rt_interrupt_enter()
#define rt_interrupt_leave()

#define RT_ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))
#define RT_ALIGN_SIZE 8

#define RT_SERIAL_RX_MINBUFSZ 64
#define RT_SERIAL_TX_MINBUFSZ 64

#define RT_DEVICE_FLAG_RX_BLOCKING      0x1000
#define RT_DEVICE_FLAG_RX_NON_BLOCKING  0x2000

#define RT_DEVICE_FLAG_TX_BLOCKING      0x4000
#define RT_DEVICE_FLAG_TX_NON_BLOCKING  0x8000

#define RT_SERIAL_RX_BLOCKING           RT_DEVICE_FLAG_RX_BLOCKING
#define RT_SERIAL_RX_NON_BLOCKING       RT_DEVICE_FLAG_RX_NON_BLOCKING
#define RT_SERIAL_TX_BLOCKING           RT_DEVICE_FLAG_TX_BLOCKING
#define RT_SERIAL_TX_NON_BLOCKING       RT_DEVICE_FLAG_TX_NON_BLOCKING

void mp_hal_gpio_clock_enable(GPIO_TypeDef *gpio);
static void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        rt_uint8_t           *pool,
                        rt_int32_t            size)
{
    RT_ASSERT(rb != RT_NULL);
    RT_ASSERT(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}

/** return the size of empty space in rb */
#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))

enum rt_ringbuffer_state
{
    RT_RINGBUFFER_EMPTY,
    RT_RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RT_RINGBUFFER_HALFFULL,
};

static inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;
        else
            return RT_RINGBUFFER_FULL;
    }
    return RT_RINGBUFFER_HALFFULL;
}

static rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
    switch (rt_ringbuffer_status(rb))
    {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
    {
        rt_size_t wi = rb->write_index, ri = rb->read_index;

        if (wi > ri)
            return wi - ri;
        else
            return rb->buffer_size - (ri - wi);
    }
    }
}

#endif

