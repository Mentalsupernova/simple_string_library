#ifndef SIMPLE_STRING_H
#define SIMPLE_STRING_H
#include <stdio.h>
#include <stdint.h>

#include <stdlib.h>

typedef struct {
  char * string;
  size_t length;

}simple_string_t;
typedef struct {
    simple_string_t *parts;
    size_t count;
} split_result_t;



extern size_t cstr_length(const char *str);
extern int64_t ss_to_i64(simple_string_t  str);
extern uint64_t ss_to_u64(simple_string_t str);
extern double ss_to_double(simple_string_t str);

extern simple_string_t i64_to_ss(int64_t number);
extern simple_string_t u64_to_ss(uint64_t number);
extern simple_string_t double_to_ss(double number);

extern simple_string_t trim_left(simple_string_t  str);
extern simple_string_t trim_right(simple_string_t  str);
extern simple_string_t trim_both(simple_string_t  str);


extern simple_string_t ss_from_cstr(const char * cstr, size_t length);


extern simple_string_t ss_concat_cstr(simple_string_t str, const char *cstr, size_t length);
extern simple_string_t ss_concat(simple_string_t str, simple_string_t other);



extern split_result_t ss_split(const simple_string_t *str, const char *delimiter);
extern void replace(simple_string_t* str,const char * pattern,const char* replace_pattern);


extern void free_split_result(split_result_t result);
extern void free_simple_string(simple_string_t str);

extern int ss_cstr_strncmp(const char *str1, const char *str2, size_t n);
extern void ss_cstr_strncpy(char *dest, const char *src, size_t n);
extern simple_string_t slice_string(const simple_string_t * str,size_t start,size_t end);
extern void rewrite_string(simple_string_t *dst, const char *new_content);







#ifdef SIMPLE_STRING_IMPLEMENTATION
void rewrite_string(simple_string_t *dst, const char *new_content) {
    if (dst == NULL || new_content == NULL) {
        return; // Do nothing if the input is invalid
    }

    size_t new_length = cstr_length(new_content);

    if (dst->string == NULL) {
        dst->string = (char *)malloc(new_length + 1); 
        if (dst->string == NULL) {
            dst->length = 0; 
            return;
        }
    } else {
        free(dst->string);
        dst->string = (char *)malloc(new_length + 1);
        if (dst->string == NULL) {
            dst->length = 0; 
            return;
        }
    }

    for (size_t i = 0; i < new_length; i++) {
        dst->string[i] = new_content[i];
    }
    dst->string[new_length] = '\0'; 

    dst->length = new_length;
}

simple_string_t slice_string(const simple_string_t *str, size_t start, size_t end) {
    simple_string_t result;
    result.string = NULL;
    result.length = 0;

    if (str == NULL || str->string == NULL || start >= str->length || end > str->length || start >= end) {
        return result; 
    }

    size_t slice_length = end - start;

    result.string = (char *)malloc(slice_length + 1); 
    if (result.string == NULL) {
        return result; 
    }

    for (size_t i = 0; i < slice_length; i++) {
        result.string[i] = str->string[start + i];
    }

    result.string[slice_length] = '\0'; 
    result.length = slice_length;

    return result;
}

void ss_cstr_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    for (; i < n; i++) {
        dest[i] = '\0'; // Null-pad the remainder
    }
}
int ss_cstr_strncmp(const char *str1, const char *str2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (str1[i] != str2[i] || str1[i] == '\0') {
            return str1[i] - str2[i];
        }
    }
    return 0;
}
simple_string_t double_to_ss(double number) {
    char buffer[64];
    int index = 0;

    if (number < 0) {
        buffer[index++] = '-';
        number = -number;
    }

    int64_t integer_part = (int64_t)number;
    number -= integer_part;

    simple_string_t integer_str = i64_to_ss(integer_part);
    for (size_t i = 0; i < integer_str.length; i++) {
        buffer[index++] = integer_str.string[i];
    }
    free(integer_str.string);

    buffer[index++] = '.';

    for (int i = 0; i < 6; i++) { 
        number *= 10;
        int digit = (int)number;
        buffer[index++] = '0' + digit;
        number -= digit;
    }

    buffer[index] = '\0'; 

    simple_string_t str;
    str.length = index;
    str.string = (char *)malloc(index + 1);
    for (int i = 0; i <= index; i++) {
        str.string[i] = buffer[i];
    }

    return str;
}

simple_string_t u64_to_ss(uint64_t number) {
    char buffer[21];
    int index = 0;

    do {
        buffer[index++] = '0' + (number % 10);
        number /= 10;
    } while (number > 0);

    for (int i = 0, j = index - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    buffer[index] = '\0'; 

    simple_string_t str;
    str.length = index;
    str.string = (char *)malloc(index + 1);
    for (int i = 0; i <= index; i++) {
        str.string[i] = buffer[i];
    }

    return str;
}

simple_string_t i64_to_ss(int64_t number) {
    char buffer[21];
    int index = 0;

    if (number < 0) {
        buffer[index++] = '-';
        number = -number;
    }

    int start_index = index;
    do {
        buffer[index++] = '0' + (number % 10);
        number /= 10;
    } while (number > 0);

    for (int i = start_index, j = index - 1; i < j; i++, j--) {
        char temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    buffer[index] = '\0'; 

    simple_string_t str;
    str.length = index;
    str.string = (char *)malloc(index + 1);
    for (int i = 0; i <= index; i++) {
        str.string[i] = buffer[i];
    }

    return str;
}

double ss_to_double(simple_string_t str) {
    double result = 0.0;
    double fraction = 0.0;
    double divisor = 1.0;
    int sign = 1;
    size_t i = 0;
    int is_fraction = 0;

    if (str.string[0] == '-') {
        sign = -1;
        i++;
    }

    for (; i < str.length; i++) {
        char c = str.string[i];
        if (c == '.') {
            is_fraction = 1;
            continue;
        }
        if (c < '0' || c > '9') {
            break; 
        }

        if (is_fraction) {
            fraction = fraction * 10 + (c - '0');
            divisor *= 10.0;
        } else {
            result = result * 10 + (c - '0');
        }
    }

    result += fraction / divisor;
    return sign * result;
}
uint64_t ss_to_u64(simple_string_t str) {
    uint64_t result = 0;
    for (size_t i = 0; i < str.length; i++) {
        char c = str.string[i];
        if (c < '0' || c > '9') {
            break; 
        }
        result = result * 10 + (c - '0');
    }

    return result;
}
int64_t ss_to_i64(simple_string_t str) {
    int64_t result = 0;
    int sign = 1;
    size_t i = 0;

    if (str.string[0] == '-') {
        sign = -1;
        i++;
    }

    for (; i < str.length; i++) {
        char c = str.string[i];
        if (c < '0' || c > '9') {
            break; 
        }
        result = result * 10 + (c - '0');
    }

    return sign * result;
}
void free_split_result(split_result_t result) {
    for (size_t i = 0; i < result.count; i++) {
        free(result.parts[i].string);
    }
    free(result.parts);
    result.count = 0;
    result.parts = NULL;
}

simple_string_t ss_concat(simple_string_t str, simple_string_t other) {
    return ss_concat_cstr(str, other.string, other.length);
}
size_t count_parts(const simple_string_t *str,const char *delimiter){
  size_t res = 0;
  size_t is_running = 1;

    size_t start = 0;
    while (is_running) {
        size_t end = start;

    size_t delimiter_length = cstr_length(delimiter);
    if (delimiter_length == 0 || str->length == 0) {
        return -1; 
    }
        while (end < str->length && str->string[end] != delimiter[0]) {
            end++;
        }
	res +=1;
        start = end + delimiter_length;
        if (start >= str->length) {
            is_running = 0;
        }
    }
    return res;

}
split_result_t ss_split(const simple_string_t *str, const char *delimiter) {
    split_result_t result;
    result.parts = NULL;
    result.count = 0;

    size_t delimiter_length = cstr_length(delimiter);
    if (delimiter_length == 0 || str->length == 0) {
        return result; 
    }

    size_t start = 0;



    size_t parts_count = count_parts(str,delimiter);
    if(parts_count<0){
      return result;
    }
    result.parts = malloc(parts_count * sizeof(simple_string_t));
    if (result.parts == NULL) {
        return result; // Return empty result if allocation fails
    }
    result.count = parts_count;

    size_t index = 0;

    while (index < parts_count) {
        size_t end = start;

        while (end < str->length && str->string[end] != delimiter[0]) {
            end++;
        }

        result.parts[index] = slice_string(str, start, end);


        start = end + delimiter_length;

        index++;
    }
    

    return result;
}




simple_string_t ss_concat_cstr(simple_string_t str, const char *cstr, size_t length) {
    size_t new_length = str.length + length;
    char *new_string = (char *)malloc(new_length + 1); 

    if (new_string == NULL) {
        return str; 
    }

    for (size_t i = 0; i < str.length; ++i) {
        new_string[i] = str.string[i];
    }

    for (size_t i = 0; i < length; ++i) {
        new_string[str.length + i] = cstr[i];
    }

    new_string[new_length] = '\0'; 

    free(str.string);

    str.string = new_string;
    str.length = new_length;

    return str;
}
void free_simple_string(simple_string_t str){
  if(str.string){
    free(str.string);
  }
}
size_t cstr_length(const char *str) {
    size_t length = 0;

    while (*str != '\0') {
        length++;
        str++;
    }

    return length;
}
simple_string_t trim_right(simple_string_t str) {
    size_t end = str.length;

    while (end > 0 && 
           (str.string[end - 1] == ' ' || str.string[end - 1] == '\t' || 
            str.string[end - 1] == '\n' || str.string[end - 1] == '\r' || 
            str.string[end - 1] == '\v' || str.string[end - 1] == '\f')) {
        --end;
    }

    str.string[end] = '\0'; // Null-terminate
    str.length = end;

    return str;
}

simple_string_t trim_both(simple_string_t str) {
    str = trim_left(str);   
    str = trim_right(str);  
    return str;
}
simple_string_t trim_left(simple_string_t str) {
    size_t start = 0;

    while (start < str.length && 
           (str.string[start] == ' ' || str.string[start] == '\t' || 
            str.string[start] == '\n' || str.string[start] == '\r' || 
            str.string[start] == '\v' || str.string[start] == '\f')) {
        ++start;
    }

    size_t new_length = str.length - start;

    for (size_t i = 0; i < new_length; ++i) {
        str.string[i] = str.string[start + i];
    }

    str.string[new_length] = '\0'; 
    str.length = new_length;

    return str;
}


simple_string_t ss_from_cstr(const char *cstr, size_t length) {
    simple_string_t s;

    if (cstr == NULL || length == 0) {
        s.string = NULL;
        s.length = 0;
        return s;
    }

    s.string = (char *)malloc((length + 1) * sizeof(char)); // +1 for null-terminator
    if (s.string == NULL) {
        s.length = 0;
        return s;
    }

    size_t i;
    for (i = 0; i < length && cstr[i] != '\0'; ++i) {
        s.string[i] = cstr[i];
    }
    s.string[i] = '\0'; // Null-terminate
    s.length = i;

    return s;
}


#endif

#endif
