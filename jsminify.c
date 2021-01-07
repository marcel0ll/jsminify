#include <string.h>
#include <stdio.h>
#include <math.h>
#include <node_api.h>
#include <tree_sitter/api.h>
#include "libs/tree-sitter-visitor/tree-sitter-visitor.h"

char * VERSION = "v0.0.20";
int debug = 0;
int BEAUTIFY = 0;
int KEEP_COMMENTS = 0;
FILE *new_stdout;

void node_keyword (TSNode node, struct visit_context * context) {
  printf("%s", ts_node_type(node));
}

void node_keyword_space (TSNode node, struct visit_context * context) {
  printf("%s ", ts_node_type(node));
}

void node_keyword_space_if_value (TSNode node, struct visit_context * context) {
  const char * type = ts_node_type(node);
  // hack to print break from break_statement (same for continue)
  while(*type != '_') printf("%c", *type++);

  TSNode value = ts_node_child_by_field_name(node, "label", 5);
  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_spaced_keyword (TSNode node, struct visit_context * context) {
  printf(" %s ", ts_node_type(node));
}

void node_class (TSNode node, struct visit_context * context) {
  if(!ts_node_is_named(node)) return;
    printf("class ");
}

void node_comma (TSNode node, struct visit_context * context) {
  TSNode parent = ts_node_parent(node);
  if (!ts_node_is_null(parent) && ts_node_type(parent) == "array") {
    TSNode next_sibling = ts_node_next_sibling(node);
    if (!ts_node_is_null(next_sibling) && ts_node_type(next_sibling) != "]") {
      printf(",");
    }
  } else {
    printf(",");
  }
}

TSNode find_next_update_leaf (TSNode node) {
  if (ts_node_is_null(node)) {
    return node;
  }

  char * type = ts_node_type(node);
  if (type == "update_expression" || type == "unary_expression") {
    return node;
  } else if (type == "binary_expression") {
    TSNode child_node = ts_node_child(node, 0);
    return find_next_update_leaf(child_node);
  } else {
    return node;
  }
}
void node_plus (TSNode node, struct visit_context * context) {
  TSNode parent = ts_node_parent(node);
  if (ts_node_is_null(parent) || ts_node_type(parent) != "binary_expression") {
    printf("+");
    return;
  }

  printf("+");
  TSNode next_sibling = ts_node_next_sibling(node);
  if (!ts_node_is_null(next_sibling)) {
    TSNode next_leaf = find_next_update_leaf(next_sibling);
    char * type = ts_node_type(next_leaf);
    if (type == "update_expression" || type == "unary_expression") {
      TSNode child_node = ts_node_child(next_leaf, 0);
      char * child_type = ts_node_type(child_node); 
      if (child_type == "++" || child_type == "+") {
        printf(" ");
      }
    }
  }
}

void node_minus (TSNode node, struct visit_context * context) {
  TSNode parent = ts_node_parent(node);
  if (ts_node_is_null(parent) || ts_node_type(parent) != "binary_expression") {
    printf("-");
    return;
  }

  printf("-");
  TSNode next_sibling = ts_node_next_sibling(node);
  if (!ts_node_is_null(next_sibling)) {
    TSNode next_leaf = find_next_update_leaf(next_sibling);
    char * type = ts_node_type(next_leaf);
    if (type == "update_expression" || type == "unary_expression") {
      TSNode child_node = ts_node_child(next_leaf, 0);
      char * child_type = ts_node_type(child_node); 
      if (child_type == "--" || child_type == "-") {
        printf(" ");
      }
    }
  }
}

void node_else (TSNode node, struct visit_context * context) {
  printf("else");
  TSNode next_sibling = ts_node_next_sibling(node);
  if (!ts_node_is_null(next_sibling) && ts_node_type(next_sibling) != "statement_block") {
    printf(" ");
  }
}

void node_import (TSNode node, struct visit_context * context) {
  if(ts_node_is_named(node)) return;
    printf("import");
}

void node_function (TSNode node, struct visit_context * context) {
  if(!ts_node_is_named(node)) return;
  printf("function");

  TSNode value = ts_node_child_by_field_name(node, "name", 4);
  if (!ts_node_is_null(value)) 
    printf(" ");
}

void node_asterisk (TSNode node, struct visit_context * context) {
  TSNode parent = ts_node_parent(node);
  TSNode parent_prev_sibling = ts_node_prev_sibling(parent);
  if (strcmp(ts_node_type(parent), "namespace_import") == 0) {
    if (ts_node_is_null(parent_prev_sibling)) {
      printf(" ");
    } else if (strcmp(ts_node_type(parent_prev_sibling), ",") != 0) {
      printf(" ");
    }
  }

  printf("*");
}

void node_from (TSNode node, struct visit_context * context) {
  printf(" from");
}

void node_identifier (TSNode node, struct visit_context * context) {
  TSNode parent = ts_node_parent(node);
  if (strcmp(ts_node_type(parent), "import_clause") == 0) {
    printf(" ");
  }

  printf("%s", ts_node_text(node, context));

  TSNode next_sibling = ts_node_next_sibling(node);
  if (ts_node_is_null(next_sibling) || (!ts_node_is_null(next_sibling) && strcmp(ts_node_type(next_sibling), ",") != 0)) { 
    TSNode next_parent_sibling = ts_node_next_sibling(parent);
    if(!ts_node_is_null(next_parent_sibling) && strcmp(ts_node_type(next_parent_sibling), "from") == 0) 
      printf(" "); 
  }
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

size_t count_precision(const char *str) {
  char * s = str;
  size_t count = 0;
  while(*s != '\0' && *s != 'e' && *s != 'E') {
    count++;
    s++;
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
    e = s;
    do {
      s--;
    } while((*s == '0') && s > str);
    s++;
    if (s < e)
      while((*s++ = *e++) != '\0')
        continue;
  }

  return str;
}

void node_number (TSNode node, struct visit_context * context) {
  char * text = ts_node_text(node, context);
  /* printf("%s\n", text); */
  // remove non relevant right 0
  text = strremovedot0(text);
  /* printf("%s\n", text); */

  long long int integer = 0;
  long long unsigned int fraction = 0;
  int precision = 0;
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
    e = 0;
  } else {
    char ip[17];
    char *p = text;
    memcpy(ip, text, 16);
    ip[16] = '\0';

    integer = strtoll(ip, NULL, 10);
    if ((p = strstr(text, ".")) != NULL) {
      p++;
      memcpy(ip, p, 16);
      ip[16] = '\0';
      precision = count_precision(ip);
      /* printf("%s\n", p); */
      fraction = strtoull(ip, NULL, 10);
      /* printf("%llu\n", fraction); */
    }
    if ((p = strstr(text, "e")) != NULL || (p = strstr(text, "E")) != NULL) {
      p = strreplace(p, "+0", "+");
      p = strreplace(p, "-0", "-");
      p++;
      e = strtol(p, &p, 10);
    }
  }
  /* printf("%lld.%llue%d  %d\n", integer, fraction, e, precision); */

  if (integer == 0 && (int) len_str_int(fraction) < precision) {
    e -= precision - len_str_int(fraction);
    precision = len_str_int(fraction);
  }

  /* printf("%lld.%llde%d  %d\n", integer, fraction, e, precision); */

  while (precision > 0) {
    /* printf("%lld.%llde%d\n", integer, fraction, e); */
    long long int d = pow(10, precision - 1);
    long long int i = fraction / d;
    fraction = fraction % d;

    integer = integer * 10LL + i;
    precision--;
    e--;
  }

  while (integer % 10LL == 0 && integer > 0) {
    /* printf("%lld.%llde%d\n", integer, fraction, e); */
    integer = integer / 10LL;
    e++;
  }

  if (e > 0 && e <= 2) {
    /* printf("%lld.%llde%d\n", integer, fraction, e); */
    int d = pow(10, e);
    integer = integer * d;
    e = 0;
  }

  /* printf("\n%lld e%d\n", integer, e); */
  /* printf("%d %d\n", len_str_int(integer), -e); */
  int difelen = -e - len_str_int(integer);
  // dif > 2
  // dif <= 2
  if (difelen <= 2) {
    while(e < 0) {
      /* printf("%lld.%llde%d\n", integer, fraction, e); */
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
      printf("%llu", fraction);
    }
    if (e != 0)
      printf("e%d", e);

    /* printf("\n%lld.%llde%d\n", integer, fraction, e); */
  }
}

void node_semi (TSNode node, struct visit_context * context) {
  TSNode next_sibling = ts_node_next_sibling(node);
  if (!ts_node_is_null(next_sibling) && ts_node_type(next_sibling) != "}") {
    printf(";");
  } else if(ts_node_is_null(next_sibling)) {
    printf(";");
  }

  if (BEAUTIFY)
    printf("\n");
}

void node_line_break (TSNode node, struct visit_context * context) {
  if (BEAUTIFY)
    printf("\n");
}

int parse_file(int argc, char * argv[]) {
  int i;
  char * output_path = NULL;
  int last_arg = 0;
  for (i = 0; i < argc; i++) {
    char * arg = argv[i];
    if (strcmp("-v", arg) == 0 || strcmp("--version", arg) == 0) {
      printf("@lotuz/jsminify %s\n", VERSION);
      return 0;
    } else if (strcmp("-h", arg) == 0 || strcmp("--help", arg) == 0) {
      printf("Usage: jsminify [OPTIONS] [FILE]\n");
      printf("\n");
      printf("Options: \n");
      printf("\t -h, --help: For printing jsminify help\n");
      printf("\t -v, --version: For printing jsminify version\n");
      printf("\t -d, --debug: For debugging minification, also helpful for bug report\n");
      printf("\t -o, --output: Sets the output file\n");
      printf("\t (WIP) -c, --keep-comments: Keeps comments while minifying\n");
      printf("\t (WIP) -b, --beautify: For pretty printing file\n");
      return 0;
    } else if (strcmp("-d", arg) == 0 || strcmp("--debug", arg) == 0) {
      debug = 1;
      last_arg = i;
    } else if (strcmp("-b", arg) == 0 || strcmp("--beautify", arg) == 0) {
      BEAUTIFY = 1;
      last_arg = i;
    } else if (strcmp("-o", arg) == 0 || strcmp("--output", arg) == 0) {
      if (i + 1 < argc) {
        output_path = argv[i + 1];
        i++;
      } else {
        printf("Missing output file path\n");
        return 1;
      }
      last_arg = i;
    } else if (strcmp("-k", arg) == 0 || strcmp("--keep-comments", arg) == 0) {
      KEEP_COMMENTS = 1;
      last_arg = i;
    }
  }
  size_t buffer_size = BUFSIZ;
  char * buffer = malloc(buffer_size);
  if (output_path != NULL) {
    new_stdout = freopen(output_path, "w", stdout);
    if(new_stdout == NULL) {
       perror("fopen"); 
       return 1;
    }
  } else {
    new_stdout = stdout;
  }

  setvbuf(new_stdout, buffer, _IOFBF, buffer_size);

  char * file_path = argv[argc - 1];
  if (file_path == NULL) {
    printf("No files passed...\n");
    return 1;
  }
  const char *source_code = get_source(file_path);

  TSLanguage *tree_sitter_javascript();
  TSParser *parser = ts_parser_new();
  ts_parser_set_language(parser, tree_sitter_javascript());


  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  TSNode root_node = ts_tree_root_node(tree);

  struct visit_context *context = context_new(source_code, debug);

  context_set_type_visitor(context, "identifier", node_identifier, NULL);

  context_set_type_visitor(context, "regex_pattern", node_text, NULL);
  context_set_type_visitor(context, "regex_flags", node_text, NULL);
  context_set_type_visitor(context, "string", node_text, NULL);
  context_set_type_visitor(context, "number", node_number, NULL);
  context_set_type_visitor(context, "statement_identifier", node_text, NULL);
  context_set_type_visitor(context, "property_identifier", node_text, NULL);
  context_set_type_visitor(context, "shorthand_property_identifier", node_text, NULL);
  context_set_type_visitor(context, "function_declaration", node_function_declaration, NULL);
  context_set_type_visitor(context, "function", node_function, NULL);
  context_set_type_visitor(context, "statement_block", node_line_break, NULL);
  context_set_type_visitor(context, "*", node_asterisk, NULL);
  context_set_type_visitor(context, "import", node_import, NULL);
  context_set_type_visitor(context, "namespace_import", NULL, node_space);
  context_set_type_visitor(context, ",", node_comma, NULL);
  context_set_type_visitor(context, "+", node_plus, NULL);
  context_set_type_visitor(context, "-", node_minus, NULL);
  context_set_type_visitor(context, "else", node_else, NULL);

  if (KEEP_COMMENTS) {
    context_set_type_visitor(context, "comment", node_text, NULL);
  }

  const char * class_types[] = {"class_declaration", "class", NULL};
  context_set_types_visitor(context, class_types, node_class, NULL);

  const char * semi_types[] = { "expression_statement", "variable_declaration",
    "lexical_declaration", "return_statement", "empty_statement",
    "throw_statement", "do_statement", "import_statement", NULL};
  context_set_types_visitor(context, semi_types, NULL, node_semi);

  const char * keyword_space_types[] = {"export", "default", "const", "new",
    "var", "let", "case", "throw", "void", "return", "do", "delete",
    "get", "set", "typeof", NULL};
  context_set_types_visitor(context, keyword_space_types, node_keyword_space, NULL);

  const char * keyword_space_if_value_types[] = { "break_statement",
    "continue_statement", NULL }; 
  context_set_types_visitor(context, keyword_space_if_value_types,
      node_keyword_space_if_value, node_semi);

  const char * spaced_keyword_types[] = { "in", "of", "as",
    "instanceof", NULL }; 
  context_set_types_visitor(context, spaced_keyword_types,
      node_spaced_keyword, NULL);

  const char * keyword_types[] = { "from", "for", "while", "this", "if",
    "switch", "undefined", "null", "debugger", "yield", "eval", ".", "...",
    "?", ":", "!", "==", "!=", "===", "!==", ">", ">=", "<", "<=", "++", "--",
    "=", "/", "%", "**", "<<", ">>", ">>>", "&", "^", "|", "&&", "||", "??",
    "+=", "-=", "*=", "/=", "%=", "<<=", ">>=", ">>>=", "&=", "^=", "|=",
    "&&=", "||=", "\?\?=", "~", "(", ")", "[", "]", "{", "}", "true", "false",
    "try", "catch", "finally", "with", "super", "extends", NULL };
  context_set_types_visitor(context, keyword_types, node_keyword, NULL);

  visit_tree_cursor(root_node, context);
  printf("\n");

  context_delete(context);
  ts_tree_delete(tree);
  ts_parser_delete(parser);

  fflush(new_stdout);
  free(buffer);
  return 0;
}


napi_value jsminify (napi_env env, napi_callback_info cbinfo) {
  size_t argc = 0;
  napi_get_cb_info(env, cbinfo, &argc, NULL, NULL, NULL);

  napi_value *argv = (napi_value *) malloc(argc * sizeof(uintptr_t));
  napi_get_cb_info(env, cbinfo, &argc, argv, NULL, NULL);

  if (argc < 1) {
    napi_throw_error(env, "EINVAL", "Too few arguments");
    return NULL;
  }

  char ** args = malloc(sizeof(char *) * argc);
  size_t str_len = 1024;
  for (size_t i = 0; i < argc; i++) {
    args[i] = malloc(str_len);
    if(napi_get_value_string_utf8(env, argv[i], (char *) args[i], 1024, &str_len) != napi_ok) {
      napi_throw_error(env, "EINVAL", "Expected string");
      return NULL;
    }
  }

  parse_file(argc, args);
  for (size_t i = 0; i < argc; i++)
    free(args[i]);
  free(args);

  return NULL;
}

napi_value init_all (napi_env env, napi_value exports) {
  napi_value jsminify_fn;
  napi_create_function(env, NULL, 0, jsminify, NULL, &jsminify_fn);
  napi_set_named_property(env, exports, "jsminify", jsminify_fn);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init_all)
