#include <stdio.h>
#include <limits.h>
#include "hwlib.h"
#include "hw.h"
#include "helpers.h"

// Part 2: Arrays and Strides

int sum_array(const int* arr, int size) {
    if (size <= 0) {
        return 0;
    }
    // =================> TODO: Return the sum of the first size elements (0 if size <= 0)
    int result = 0;
    for (int i = 0; i < size; i ++) {
        result += *(arr + i);
    }
    return result;
}

int find_max(const int* arr, int size) {
    // =================> TODO: Return the largest element (INT_MIN if size <= 0)
    if (size <= 0) {
        return INT_MIN;
    }
    int max = INT_MIN;
    for (int i = 0; i < size; i ++) {
        if (*(arr + i) > max) {
            max = *(arr + i);
        }
    }
    return max;
}

int filter_positive(const int* in, int* out, int size) {
    // =================> TODO: Copy each element > 0 into out and return the count
    int count = 0;
    for (int i = 0; i < size; i ++) {
        if (*in > 0) {
            *out = *in;
            out++;
            count++;
        }
        in++;
        
    }
    return count;
}

int find_max_offset(const int* arr, int size, int offset) {
    // =================> TODO: Return the max of arr[0], arr[offset], arr[2*offset], ...
    //                          Use pointer arithmetic (not arr[i]).
    //                          Return INT_MIN if size <= 0 or offset <= 0.
    if (size <= 0 || offset <= 0) {
        return INT_MIN;
    }
    int result = *arr; // arr[0]
    
    for (int i = 0; i < size; i += offset) {
        if ((*(arr + i)) > result) {
            result = *(arr + i);
        }
    }
    return result;

}
