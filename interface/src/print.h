/**
 * author: Brando
 * date: 2/9/23
 */

#ifndef PRINT_H
#define PRINT_H

#ifdef TESTING
#define print(...)
#else
#define print(...) printf(__VA_ARGS__)
#endif 

#endif // PRINT_H

