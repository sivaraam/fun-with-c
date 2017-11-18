include <stdio.h>

char *get_auto_str(void) {
  char *automatic_str = NULL;

  // The unnamed object (with value "Unallocated string") has automatic storage duration.
  // So, it can't be returned from the function.
  automatic_str = "Automatic local string";

  // If automatic_str has automatic storage duration then how
  // could it be returned without issues ???
  return automatic_str;
}

const char *get_static_str(void) {
  static const char *static_str = NULL;

  // The unnamed onject (with value "Static string") has static storage duration.
  // So, it can be returned from the function.
  static_str = "Static string";

  return static_str;
}

void do_something(void) {
  char *str_1 = "Very very very very long string.";
  char *str_2 = "Just to clutter up the memory taken by"
                "previous function call.";
  char *str_3 = "Lets see how it works!";

  printf("%s %s %s\n", str_1, str_2, str_3);
}

int main(void) {
  char *at_str = get_auto_str();
  do_something();
  const char *st_str = get_static_str();

  printf("%s %s\n", at_str, st_str);
}
