#include "dll.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Application specific data structures */
typedef struct person_ {
  char name[32];
  int age;
  int weight;
} person_t;

static void
print_person_details(person_t *person) {
  printf("Name = %s\n", person->name);
  printf("Age = %d\n", person->age);
  printf("Weight = %d\n", person->weight);
  printf("\n");
}

static void
print_person_db(dll_t *person_db) {
#if 0
  if (!person_db || !person_db->head) return;

  dll_node_t *head = person_db->head;
  person_t *data = NULL;

  while (head) {
    data = head->data;
    print_person_details(data);
    head = head->next;
  }
#endif
  dll_node_t *node_ptr = NULL;
  person_t *data = NULL;
  ITERATE_LIST_BEGIN(person_db, node_ptr) {
    data = node_ptr->data;
    print_person_details(data);
  } ITERATE_LIST_END
}

static person_t *new_person(const char *name, int age, int weight) {
  person_t *person = calloc(1, sizeof(person_t));
  strncpy(person->name, name, strlen(name));
  person->age = age;
  person->weight = weight;

  return person;
}

int
main(int argc, char **argv) {
  person_t *person1 = new_person("Abhishek", 31, 75);
  person_t *person2 = new_person("Joseph", 41, 70);
  person_t *person3 = new_person("Jack", 29, 55);

  /* Create a new Linked List */
  dll_t *person_db = get_new_dll();
  add_data_to_dll(person_db, person1);
  add_data_to_dll(person_db, person2);
  add_data_to_dll(person_db, person3);

  print_person_db(person_db);

  if (is_dll_empty(person_db) == 0) {
    printf("db empty.\n");
  } else {
    printf("db not empty.\n");
  }

  printf("remove person1\n");
  remove_data_from_dll_by_data_ptr(person_db, person1);
  printf("drain dll\n");
  drain_dll(person_db);

  if (is_dll_empty(person_db) == 0) {
    printf("db empty.\n");
  } else {
    printf("db not empty.\n");
  }

  return 0;
}
