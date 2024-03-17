#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"

#define QWERTY_ROWS 3
#define QWERTY_COLS 10

struct cords
{
  int x;
  int y;
};

static const char qwerty_table[QWERTY_ROWS][QWERTY_COLS] = {
  { 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p' },
  { 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ' ' },
  { 'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', ' ', ' ' }
};

static unsigned char is_latin_letter(const char ch)
{
  return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static int get_kb_cords(const char ch, struct cords *kb_cords)
{
  int i;
  int j;
  char lower_ch;

  if (!is_latin_letter(ch)) {
    return EINVAL;
  }

  lower_ch = tolower(ch);

  for (i = 0; i < QWERTY_ROWS; i++) {
    for (j = 0; j < QWERTY_COLS; j++) {
      if (qwerty_table[i][j] == lower_ch) {
        kb_cords->x = j;
        kb_cords->y = i;
        return 0;
      }
    }
  }

  return ERANGE;
}

static unsigned char is_adjacent_qwerty_buttons(struct cords kb1, struct cords kb2)
{
  if (abs(kb1.y - kb2.y) > 1) {
    return 0;
  }

  if (kb1.y == kb2.y) {
    return abs(kb1.x - kb2.x) == 1;
  }

  if (abs(kb1.x - kb2.x) < 2) {
    return (kb1.x == kb2.x) || 
           (kb1.x > kb2.x && kb1.y < kb2.y) ||
           (kb1.x < kb2.x && kb1.y > kb2.y);
  }

  return 0;
}

unsigned char is_adjacent_chars(const char ch1, const char ch2)
{
  struct cords kb1_cords;
  struct cords kb2_cords;
  int result;

  result = get_kb_cords(ch1, &kb1_cords);
  if (result != 0) {
    print_err("illegal char: %c (%s)", ch1, strerror(result));
    return 0;
  }

  result = get_kb_cords(ch2, &kb2_cords);
  if (result != 0) {
    print_err("illegal char: %c (%s)", ch2, strerror(result));
    return 0;
  }

  return is_adjacent_qwerty_buttons(kb1_cords, kb2_cords);
}

unsigned char is_word_qwerty_correct(const char *word)
{
  const char *p;

  if (word[0] == '\000' || word[1] == '\000') {
    return 1;
  }

  for (p = word; *(p + 1) != '\000'; p++) {
    if (*p == *(p + 1)) {
      continue;
    }

    if (!is_adjacent_chars(*p, *(p + 1))) {
      return 0;
    }
  }

  return 1;
}
