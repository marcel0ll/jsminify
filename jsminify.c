#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <math.h>
#include <node_api.h>
#include "../tree-sitter-visitor/tree-sitter-visitor.h"

int DEBUG = 0;
int BEAUTIFY = 0;

void node_keyword (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_type(node));
}

void node_keyword_space (TSNode node, struct visit_context * context) {
  printf("%s ", ts_node_type(node));
}

void node_keyword_space_if_value (TSNode node, struct visit_context * context) {
  char * type = ts_node_type(node);
  while(*type != '_') printf("%c", *type++);

  TSNode value = ts_node_child_by_field_name(node, "label", 5);
  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_spaced_keyword (TSNode node, struct visit_context * context) {
  printf(" %s ", ts_node_type(node));
}

int class_counter = 0;
void node_class (TSNode node, struct visit_context * context) {
  if (class_counter) {
    printf("class ");
  }
  class_counter = (class_counter + 1) % 2;
}

void node_function (TSNode node, struct visit_context * context) {
  if(!ts_node_is_named(node)) return;
  printf("function");

  TSNode value = ts_node_child_by_field_name(node, "name", 4);
  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_identifier (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_function_declaration (TSNode node, struct visit_context * context) {
  printf("function ");
}

void node_text (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_text(node, context));
}

void node_space (TSNode node, struct visit_context * context) {
  printf(" ");
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

      e = r + replace_len + 1;
      c = (char *)replace;
      while(*c != '\0') {
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
    while((*s == '0' || *s == '.') && s > str) {
      s--;
      if(*s == '.') {
        s--;
        break;
      }
    }
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
  long long int integer = 0;
  long long int fraction = 0;
  int precision = 0;
  int digits = 0;
  int base = 10;
  int move = 2;
  int e = 0;
  if (*text == '0') {
    if (text[1] == 'b' || text[1] == 'B') {
      base = 2;
    } else if (text[1] == 'x' || text[1] == 'X') {
      base = 16;
    } else if (text[1] == 'o' || text[1] == 'O') {
      base = 8;
    } else if (strstr(text, ".") == NULL) {
      base = 8;
      move = 1;
    }
  }

  if (base != 10) {
    integer = strtoll(&text[move], NULL, base);
    precision = 0;
    if (integer == 0L) {
      integer = strtoll(text, NULL, 10);
    }
    digits = len_str_int(integer);
    e = 0;
  } else {
    digits = count_digits(text);
    precision = count_precision(text);
    char *p = text;
    integer = strtoll(p, &p, 10);
    if (strstr(text, ".") != NULL) {
      fraction = strtoll(&p[1], &p, 10);
    }
    if (strstr(p, "e") != NULL) {
      p = strreplace(p, "+0", "+");
      p = strreplace(p, "-0", "-");
      p++;
      e = strtol(p, &p, 10);
    }
  }

  if (integer == 0 && len_str_int(fraction) < precision) {
    e -= precision - len_str_int(fraction);
    precision = len_str_int(fraction);
  }

  while (precision > 0) {
    long long int d = pow(10, precision - 1);
    long long int i = fraction / d;
    fraction = fraction % d;

    integer = integer * 10LL + i;
    digits = len_str_int(integer);
    precision--;
    e--;
  }

  while (integer % 10LL == 0 && integer > 0) {
    integer = integer / 10LL;
    e++;
  }

  if (e > 0 && e <= 2) {
    int d = pow(10, e);
    integer = integer * d;
    e = 0;
  }

  int difelen = -e - len_str_int(integer);
  if (difelen < 3 && difelen >= -7) {
    while(e < 0) {
      long long int i = integer % 10;
      integer = integer / 10;
      fraction = fraction + i * (long long int)pow(10, precision);
      precision++;
      e++;
    }
  }

  if (fraction == 0 && !e) {
    if (len_str_int(integer) <= len_str_int_base(integer, 16) + 2)
      printf("%lld", integer);
    else
      printf("0x%llx", integer);
  } else {
    if (integer != 0) 
      printf("%lld", integer);
    if (fraction > 0) {
      printf(".");
      int zeroes = precision - len_str_int(fraction);
      for(int z = 0; z < zeroes ; z++) printf("0");
      printf("%lld", fraction);
    }
    if (e != 0)
      printf("e%d", e);

    /* printf("\n%lld.%llde%d\n", integer, fraction, e); */
  }
}

void node_spaced_text (TSNode node, struct visit_context * context) {
  printf(" %s", ts_node_text(node, context));
}

void node_semi (TSNode node, struct visit_context * context) {
  printf(";");
  if (BEAUTIFY)
    printf("\n");
}

void node_line_break (TSNode node, struct visit_context * context) {
  if (BEAUTIFY)
    printf("\n");
}

void parse_file(int argc, char * argv[]) {
  int i;
  /* for (i = 0; i < argc; i++) { */
    /* char * arg = argv[i]; */
    /* if (strcmp("-d", arg) == 0 || strcmp("--debug", arg) == 0) { */
      /* DEBUG = 1; */
    /* } */
    /* if (strcmp("-b", arg) == 0 || strcmp("--beautify", arg) == 0) { */
      /* BEAUTIFY = 1; */
    /* } */
  /* } */
  char * file_path = argv;
  if (file_path == NULL) {
    printf("No file passed...\n");
    return;
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
  context_add_visitor(context, visitor_new("unary_expression", node_space));
  context_add_visitor(context, visitor_new("update_expression", node_space));
  context_add_visitor(context, visitor_new("statement_block", node_line_break));

  const char * class_types[] = {"class_declaration", "class", NULL};
  context_add_multiple_visitors(context, class_types, node_class);

  const char * semi_types[] = { "expression_statement_out",
    "variable_declaration_out", "lexical_declaration_out",
    "return_statement_out", "empty_statement", "break_statement_out",
    "continue_statement_out", "throw_statement_out", "do_statement_out", NULL};
  context_add_multiple_visitors(context, semi_types, node_semi);

  const char * keyword_space_types[] = { "import", "export", "default",
    "const", "new", "var", "let", "else", "case", "throw", "void", "return",
    "do", "delete", "get", "set", NULL};
  context_add_multiple_visitors(context, keyword_space_types, node_keyword_space);

  const char * keyword_space_if_value_types[] = { "break_statement",
    "continue_statement", NULL }; 
  context_add_multiple_visitors(context, keyword_space_if_value_types,
      node_keyword_space_if_value);

  const char * spaced_keyword_types[] = { "in", "of", "as",
    "instanceof","typeof", NULL }; 
  context_add_multiple_visitors(context, spaced_keyword_types,
      node_spaced_keyword);

  const char * keyword_types[] = { "for", "while", "this", "if", "switch",
    "undefined", "null", "debugger", "yield", "eval", ".", "?",
    ":", "!", "==", "!=", "===", "!==", ">", ">=", "<", "<=", "++", "--", "=",
    "+", "-", "*", "/", "%", "**", "<<", ">>", ">>>", "&", "^", "|", "&&",
    "||", "??", "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", ">>>=", "&=", "^=",
    "|=", "&&=", "||=", "??=", "~", ",", "(", ")", "[", "]", "{", "}", "from",
    "true", "false", "try", "catch", "finally", "with", "super", "extends",
    NULL };
  context_add_multiple_visitors(context, keyword_types, node_keyword);

  visit_tree(root_node, context);
  printf("\n");

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return;
}


napi_value jsminify (napi_env env, napi_callback_info info) {
  napi_value argv[1];
  size_t argc = 1;

  napi_get_cb_info(env, info, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  char str[1024];
  size_t str_len;

  if (napi_get_value_string_utf8(env, argv[0], (char *) &str, 1024, &str_len) != napi_ok) {
    napi_throw_error(env, "EINVAL", "Expected string");
    return NULL;
  }

  parse_file(argc, str);

  return NULL;
}

napi_value init_all (napi_env env, napi_value exports) {
  napi_value jsminify_fn;
  napi_create_function(env, NULL, 0, jsminify, NULL, &jsminify_fn);
  napi_set_named_property(env, exports, "jsminify", jsminify_fn);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)
