/* =============================================================================
     ____    ___    ____    ___    _   _    ___    __  __   ___  __  __ TM
    |  _ \  |_ _|  / ___|  / _ \  | \ | |  / _ \  |  \/  | |_ _| \ \/ /
    | |_) |  | |  | |     | | | | |  \| | | | | | | |\/| |  | |   \  /
    |  __/   | |  | |___  | |_| | | |\  | | |_| | | |  | |  | |   /  \
    |_|     |___|  \____|  \___/  |_| \_|  \___/  |_|  |_| |___| /_/\_\

    Copyright (c) 2008 - 2012 Pieter Conradie <https://piconomix.com>
 
    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
    
    Title:          px_systmr.h : Polled software timers
    Author(s):      Pieter Conradie
    Creation Date:  2008-02-11

============================================================================= */

/* _____PROJECT INCLUDES_____________________________________________________ */
#include "px_systmr.h"

/* _____LOCAL DEFINITIONS____________________________________________________ */

/* _____MACROS_______________________________________________________________ */

/* _____GLOBAL VARIABLES_____________________________________________________ */

/* _____LOCAL VARIABLES______________________________________________________ */

/* _____LOCAL FUNCTION DECLARATIONS__________________________________________ */

/* _____LOCAL FUNCTIONS______________________________________________________ */

/* _____GLOBAL FUNCTIONS_____________________________________________________ */
void px_systmr_start(px_systmr_t * systmr, const px_systmr_ticks_t delay_in_ticks)
{
    // Save delay in case timer is restarted
    systmr->delay_in_ticks = delay_in_ticks;

    // Store start tick
    systmr->start_tick = px_sysclk_get_tick_count();

    // Set state to indicate that timer has started
    systmr->state = PX_SYSTMR_STARTED;
}

bool px_systmr_has_started(const px_systmr_t * systmr)
{
    if (systmr->state == PX_SYSTMR_STARTED) 
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool px_systmr_has_expired(px_systmr_t * systmr)
{
    px_systmr_ticks_t tick;

    // See if timer has been stopped
    if (systmr->state == PX_SYSTMR_STOPPED) 
    {
        return false;
    }

    // See if timer has already expired
    if (systmr->state == PX_SYSTMR_EXPIRED) 
    {
        return true;
    }

    // Fetch current time
    tick = px_sysclk_get_tick_count();

    // Timer expire test
    if( (tick - systmr->start_tick) < systmr->delay_in_ticks )
    {
        return false;
    }

    // Set state to indicate that timer has expired
    systmr->state = PX_SYSTMR_EXPIRED;

    return true;
}

void px_systmr_stop(px_systmr_t * systmr)
{
    // Stop timer
    systmr->state = PX_SYSTMR_STOPPED;
}

void px_systmr_restart(px_systmr_t * systmr)
{
    // Store start tick
    systmr->start_tick = px_sysclk_get_tick_count();

    // Set state to indicate that timer has started
    systmr->state = PX_SYSTMR_STARTED;
}

void px_systmr_reset(px_systmr_t * systmr)
{
    // Calculate and store new start tick
    systmr->start_tick += systmr->delay_in_ticks;

    // Set state to indicate that timer has started
    systmr->state = PX_SYSTMR_STARTED;
}

void px_systmr_wait(const px_systmr_ticks_t delay_in_ticks)
{
    px_systmr_t systmr;

    px_systmr_start(&systmr, delay_in_ticks);

    while (!px_systmr_has_expired(&systmr))
    {
        ;
    }
}
px_systmr_ticks_t px_systmr_ticks_elapsed(px_systmr_t * systmr)
{
    // Fetch current time
    px_systmr_ticks_t tick = px_sysclk_get_tick_count();

    return (tick - systmr->start_tick);
}
