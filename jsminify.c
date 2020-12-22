#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <math.h>
#include "../tree-sitter-visitor/tree-sitter-visitor.h"

void node_keyword (TSNode node, char * text) {
  printf("%s", ts_node_type(node));
}

void node_keyword_semi (TSNode node, char * text) {
  printf("%s;", ts_node_type(node));
}

void node_keyword_space (TSNode node, char * text) {
  printf("%s ", ts_node_type(node));
}

void node_keyword_space_if_value (TSNode node, char * text) {
  TSNode value = ts_node_child_by_field_name(node, "value", 5);
  printf("%s", ts_node_type(node));

  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_spaced_keyword (TSNode node, char * text) {
  printf(" %s ", ts_node_type(node));
}

int class_counter = 0;
void node_class (TSNode node, struct visit_context * context) {
  if (class_counter) {
    printf("class ");
  }
  class_counter = (class_counter + 1) % 2;
}

int function_counter = 0;
void node_function (TSNode node, struct visit_context * context) {
  if (function_counter) {
    printf("function ");
  }
  function_counter = (function_counter + 1) % 2;
}

void node_identifier (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_statement_block(TSNode node, struct visit_context * context) {
  function_counter = 0;
}

void node_function_declaration (TSNode node, struct visit_context * context) {
  printf("function ");
}

void node_text (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

bool starts_with(const char *pre, const char *str) {
  size_t lenpre = strlen(pre),
         lenstr = strlen(str);
  return lenstr < lenpre ? false : memcmp(pre, str, lenpre) == 0;
}

int count_char_at_start(const char *str, const char *target) {
  size_t lenstr = strlen(str);
  int count = 0;
  char * c = str;

  while (memcmp(c, target, 1) == 0 && c < &str[lenstr - 1]) {
    count++;
    c++;
  }

  return count;
}

int count_char_at_end(const char *str, const char *target) {
  size_t lenstr = strlen(str);
  int count = 0;
  char * c = &str[lenstr - 1];

  while (memcmp(c, target, 1) == 0 && c > str) {
    count++;
    c--;
  }

  return count;
}

size_t len_str_int (long long int value) {
  if (value == 0) return 1;

  size_t sig = 0;
  if (value < 0) {
    value = value * -1;
    sig = 1;
  }

  return (size_t)((ceill(log10(value + 1))) + sig);
}

size_t len_str_int_base (long long int value, int base) {
  if (value == 0) return 1;

  size_t sig = 0;
  if (value < 0) {
    value = value * -1;
    sig = 1;
  }

  return (size_t)(ceill(log10(value + 1) / log10(base)) + sig);
}

char *strremove(char *str, const char *find, int ignore) {
    char *p, *q, *r;
    size_t len = strlen(find) - ignore;
    if ((q = r = strstr(str, find)) != NULL) {
        q = r = q + ignore;
        while ((r = strstr(p = r + len, find)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

size_t count_digits(const char *str) {
  char * s;
  size_t count = 0;
  if ((s = strstr(str, ".")) == NULL) {
    count = strlen(str);
  } else {
    char *p = str;
    while(p++ < s)
      count++;
  }

  return count;
}

size_t count_precision(const char *str) {
  char * s;
  size_t count = 0;
  if ((s = strstr(str, ".")) != NULL) {
    s++;
    while(*s != '\0' && *s != 'e' && *s != 'E') {
      count++;
      s++;
    }
  }

  return count;
}

char *strreplace(char *str, const char *find, const char *replace) {
    char *r;

    if ((r = strstr(str, find)) != NULL) {
      char *c, *e;
      size_t str_len = strlen(str);
      size_t find_len = strlen(find);
      size_t replace_len = strlen(replace);

      if ( replace_len > find_len) {
        char *s;
        size_t add_len = replace_len - find_len;
        str = realloc(str, strlen(str) + add_len + 1);
        s = e = str + str_len;
        c = str + str_len - add_len + 1;
        while(c <= e) {
          *s++ = *c++;
        }
      } 

      e = r + replace_len;
      c = (char *)replace;
      while(c != NULL) {
        *r++ = *c++;
      }

      if (r < e) {
        while ((*r++ = *e++) != '\0')
            continue;
      }
    }

    return str;
}

char *strremovedot0(char *str) {
  char *f;
  if ((f = strstr(str, ".")) != NULL) {
    char *s, *e;
    if ((s = strstr(str, "e")) == NULL)
      if ((s = strstr(str, "E")) == NULL)
        s = str + strlen(str);
    e = s--;
    while((*s == '0' || *s == '.') && s > str) 
      s--;
    s++;
    if (s < e)
      while((*s++ = *e++) != '\0')
        continue;
  }

  return str;
}

void node_number (TSNode node, struct visit_context * context) {
  char * text = ts_node_text(node, context);
  // remove non relevant right 0
  text = strremovedot0(text);

  size_t text_len = strlen(text);
  double value;
  int precision = 0;
  int digits = 0;
  int base = 10;
  int move = 2;
  int e = 0;
  if (starts_with("0b", text) || starts_with("0B", text)) {
    base = 2;
  } else if (starts_with("0x", text) || starts_with("0X", text)) {
    base = 16;
  } else if (starts_with("0o", text) || starts_with("0O", text)) {
    base = 8;
  } else if (strstr(text, ".") == NULL && starts_with("0", text)) {
    base = 8;
    move = 1;
  }

  if (base != 10) {
    value = strtoll(&text[move], NULL, base);
    if (value == 0L) {
      value = strtoll(text, NULL, 10);
    }
  } else {
    precision = count_precision(text);
    digits = count_digits(text);
    value = strtold(text, NULL);
  } 

  if (base != 16 && strstr(text, "e") != NULL) {
    e = 1;
  }

  if (precision == 0 && !e) {
    if (len_str_int(value) <= len_str_int_base(value, 16) + 2)
      printf("%lld", (long long int)value);
    else
      printf("0x%llx", (long long int)value);
  } else if (precision > 0 && !e) {
    char * format;
    size_t size = 2 + len_str_int(digits) + 1 + len_str_int(precision);
    format = malloc(size + 1);
    sprintf(format, "%%.%df", precision);
    printf(format, value);
  }else {
    printf("%g", value);
  }
}

void node_spaced_text (TSNode node, struct visit_context * context) {
  printf(" %s", ts_node_text(node, context));
}

void node_semi (TSNode node, struct visit_context * context) {
  printf(";");
}

int DEBUG = 0;
int main(int argc, char * argv[]) {
  int i;
  for (i = 0; i < argc; i++) {
    char * arg = argv[i];
    if (strcmp("-d", arg) == 0 || strcmp("--debug", arg) == 0) {
      DEBUG = 1;
    }
  }
  char * file_path = argv[argc - 1];
  if (file_path == NULL) {
    printf("No file passed...\n");
    return 1;
  }
  TSLanguage *tree_sitter_javascript();
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_javascript());

  const char *source_code = get_source(file_path);

  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  TSNode root_node = ts_tree_root_node(tree);

  struct visit_context *context = context_new(source_code, DEBUG);

  context_add_visitor(context, visitor_new("identifier", node_identifier));

  context_add_visitor(context, visitor_new("regex_pattern", node_text));
  context_add_visitor(context, visitor_new("regex_flags", node_text));
  context_add_visitor(context, visitor_new("string", node_text));
  context_add_visitor(context, visitor_new("number", node_number));
  context_add_visitor(context, visitor_new("statement_identifier", node_text));
  context_add_visitor(context, visitor_new("property_identifier", node_text));
  context_add_visitor(context, visitor_new("function_declaration", node_function_declaration));
  context_add_visitor(context, visitor_new("function", node_function));
  context_add_visitor(context, visitor_new("statement_block", node_statement_block));

  const char * class_types[] = {"class_declaration", "class", NULL};
  context_add_multiple_visitors(context, class_types, node_class);

  const char * semi_types[] = { "expression_statement_out",
    "variable_declaration_out", "lexical_declaration_out", "return_statement_out", "empty_statement",
    "break_statement_out", "continue_statement_out", NULL};
  context_add_multiple_visitors(context, semi_types, node_semi);

  const char * keyword_space_types[] = { "import", "export", "default", "const",
    "new", "var", "let", "else", "case", "throw", "void",
    "return", "delete", NULL};
  context_add_multiple_visitors(context, keyword_space_types, node_keyword_space);

  const char * keyword_space_if_value_types[] = { "break", "continue", NULL };
  context_add_multiple_visitors(context, keyword_space_if_value_types, node_keyword_space_if_value);

  const char * spaced_keyword_types[] = { "in", "of", "as",
    "instanceof","typeof", NULL };
  context_add_multiple_visitors(context, spaced_keyword_types, node_spaced_keyword);

  const char * keyword_types[] = { "for", "do", "while", "this", "if", "switch",
    "undefined", "null", "debugger", "get", "set", "yield", "eval", "!", "?", "%",
    "*", "**", "++", "+", "--", "-", "~", ".", ":", "=", "==", "+=", "-=",
    "*=", "/=", "&=", "|=", "<=", ">=", "!=", "===", "!==", ",",  "(",  ")",
    "<",  ">",  "|",  "^",  "&",  ">>", ">>>", "<<", "[",  "]",  "{",  "}",
    "||", "&&", "from", "true", "false", "try", "catch", "finally", "with",
    "super", "extends", "/", NULL };
  context_add_multiple_visitors(context, keyword_types, node_keyword);

  visit_tree(root_node, context);
  printf("\n");

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}

