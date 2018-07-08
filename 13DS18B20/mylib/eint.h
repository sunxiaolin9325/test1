#ifndef __EINT_H__
#define __EINT_H_

typedef void (*irq_handler)(void);

static irq_handler handler1, handler2, handler3, handler4;

extern void eint_init(void);
extern void set_handler(irq_handler h1, irq_handler h2, irq_handler h3, irq_handler h4);

#endif


