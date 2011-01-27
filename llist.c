#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct list {
  struct list *next;
  struct list *prev;

  char *data;
} LIST;


LIST *LIST_HEAD;
LIST *LIST_TAIL;

LIST * list_init(LIST *list)
{
  LIST *new = NULL;

  if ( (new = malloc(sizeof(LIST))) == NULL) {
    /* FIXME */
    exit(-1);
  }
  new->next = NULL;
  new->prev = NULL;

  LIST_HEAD = new;
  LIST_TAIL = NULL;

  return new;
}

void list_free(LIST *list)
{
  if (list == NULL)
    return;

  free(list->data);
  list->next = NULL;
  list->prev = NULL;
  free(list);
}

void list_print(LIST *list)
{
  LIST *iter;

  if (list == NULL || !list) {
    fprintf(stdout, "LIST is NULL\n");
    return;
  }

  for (iter = list; iter != NULL; iter = iter->next) {
    fprintf(stdout, "List Data: [%s]\n", iter->data);
  }
}

LIST * list_add(LIST *list, char *data)
{
  LIST *new = NULL;
  char *newdata = NULL;
  int n = strlen(data);
  
  if ( (newdata = malloc(n+1)) == NULL) {
    /* FIXME */
    exit(-1);
  }
  strncpy(newdata, data, n);

  if (LIST_TAIL == NULL) { /* first node */
    LIST_HEAD->data = newdata;
    LIST_TAIL = LIST_HEAD;
    return new;
  }

  if ( (new = malloc(sizeof(LIST))) == NULL) {
    /* FIXME */
    exit(-1);
  }

  new->data = newdata;
  new->prev = LIST_TAIL;
  new->next = NULL;

  LIST_TAIL->next = new;
  LIST_TAIL = new;

  return new;
}

LIST * list_del_first(LIST *list)
{
  LIST *first = LIST_HEAD;
  LIST_HEAD = LIST_HEAD->next;

  if (LIST_HEAD != NULL) {
    LIST_HEAD->prev = NULL;
  }

  list_free(first);

  return LIST_HEAD;
}

LIST * list_del_last(LIST *list)
{
  LIST *last = LIST_TAIL;

  if (LIST_TAIL != LIST_HEAD) {
    LIST_TAIL = LIST_TAIL->prev;
  } 

  LIST_TAIL->next = NULL;

  list_free(last);
  return LIST_HEAD;
}

LIST * list_del_all(LIST *list)
{
  LIST *iter;

  do {
    iter = list;

    list = list->next;
    LIST_HEAD = list;

    if (list != NULL) {
      list->prev = NULL;
    }

    list_free(iter);
  } while (list != NULL);

  return list;
}

LIST * list_search_data(LIST *list, char *data)
{
  LIST *iter;

  int n = strlen(data);

  for (iter = list; iter != NULL; iter = iter->next) {
    if (strncmp(iter->data, data, n) == 0) {
      return iter;
    }
  }

  return NULL;
}

#ifdef TEST

int test_list(LIST *list)
{
  LIST *search;
  char *data = "Four - 4";

  list_add(list, "One - 1");
  list_add(list, "Two - 2");
  list_add(list, "Three - 3");
  list_add(list, "Four - 4");
  list_print(list);

  fprintf(stdout, "List search for %s\n", data);
  search = list_search_data(list, data);
  if (search) {
    list_print(search);
  } else {
    fprintf(stdout, "Search not found\n");
  }

  list = list_del_all(list);
  list_print(list);
  return 1;

  list = list_del_first(list);
  list_print(list);

  list = list_del_first(list);
  list_print(list);

  list = list_del_last(list);
  list_print(list);

  list_free_all(list);
  list_print(list);
  return 1;
}

int main(void)
{
  LIST *mylist = NULL;

  mylist = list_init(mylist);

  if (!(test_list(mylist))) {
    fprintf(stderr, "Test List Failed\n");
  } else {
    fprintf(stderr, "Test List Succeed\n");
  }
}
#endif

