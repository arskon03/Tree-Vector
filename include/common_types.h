#pragma once // #include το πολύ μία φορά

// Τύποι που χρησιμοποιούνται σε πολλά modules

// Χρήση του τύπου "bool" για μεταβλητές που παίρνουν μόνο τιμές true / false
#include <stdbool.h> 
#include <stdint.h> 

// Pointer προς ένα αντικείμενο οποιουδήποτε τύπου. Απλά είναι πιο ευανάγνωστο από το "void*" που μοιάζει με το "void"
typedef void* Pointer;

// unsigned int, για συντομία
typedef unsigned int uint;

// unsigned 8-bit int is a byte
typedef uint8_t byte;

// Δείκτης σε συνάρτηση που συγκρίνει 2 στοιχεία a και b και επιστρέφει:
// < 0  αν a < b
//   0  αν a και b είναι ισοδύναμα (_όχι_ αναγναστικά ίσα)
// > 0  αν a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Δείκτης σε συνάρτηση που καταστρέφει ένα στοιχείο value
typedef void (*DestroyFunc)(Pointer value);