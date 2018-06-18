#ifndef KS_FILE_LINE_OFFSETS
#define KS_FILE_LINE_OFFSETS

struct lines
{
  /**
   * TODO: see if you could do this as a double pointer.
   */
  unsigned long *byte_offsets;
  unsigned long lines;
};

/**
 *  For the given file generate an array indexed by line number
 *  which contains the byte offset of that particular line in the file.
 *
 *  A line is said to be a characters followed by a new line character.
 *  File is expected to end with a new line.
 */
struct lines *get_line_offsets(char *file_name);

#endif
