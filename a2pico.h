/*

MIT License

Copyright (c) 2022 Oliver Schmidt (https://a2retro.de/)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

#ifndef _A2PICO_H
#define _A2PICO_H

#include <hardware/pio.h>

#define SM_ADDR  0
#define SM_READ  1
#define SM_WRITE 2

#define GPIO_IRQ   24
#define GPIO_RESET 25

void a2pico_init(PIO pio);

static __always_inline uint32_t a2pico_getaddr(PIO pio) {
    while (pio->fstat & (1u << (PIO_FSTAT_RXEMPTY_LSB + SM_ADDR))) {
        tight_loop_contents();
    }
    return pio->rxf[SM_ADDR];
}

static __always_inline uint32_t a2pico_getdata(PIO pio) {
    while (pio->fstat & (1u << (PIO_FSTAT_RXEMPTY_LSB + SM_WRITE))) {
        tight_loop_contents();
    }
    return pio->rxf[SM_WRITE];
}

static __always_inline void a2pico_putdata(PIO pio, uint32_t data) {
    pio->txf[SM_READ] = data;
}

static __always_inline void a2pico_resetcallback(gpio_irq_callback_t callback) {
    gpio_set_irq_enabled_with_callback(GPIO_RESET, 
                GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, callback, callback);
}

static __always_inline void a2pico_irq(bool assert) {
    gpio_set_dir(GPIO_IRQ, assert ? GPIO_OUT : GPIO_IN);
}

#endif
